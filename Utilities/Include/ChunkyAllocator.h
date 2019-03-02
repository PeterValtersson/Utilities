#ifndef _UTILITIES_CHUNKY_ALLOCATOR_H_
#define _UTILITIES_CHUNKY_ALLOCATOR_H_

#include <stdint.h>
#include <mutex>
#include <list>
#include <stack>
#include <random>

namespace Utilities
{
	namespace Memory
	{


		class ChunkyAllocator {
		public:
			struct ChunkInfo {
				friend ChunkyAllocator;
				//void setUsedState( bool inUse ) { freeForDefrag = !inUse; }
				ChunkInfo( ChunkInfo& other ) : allocator(other.allocator)
				{
					handle = other.handle;
					other.handle = ~0;
				}
				ChunkInfo& operator=( ChunkInfo& other )
				{
					allocator = other.allocator;
					handle = other.handle;
					other.handle = ~0;
				}
				~ChunkInfo()
				{
					free();
				}
				void free()
				{
					if ( handle != ~0 )
						allocator->free( *this );
					handle = ~0;
				}
			private:
				ChunkInfo( ChunkyAllocator* allocator ) : allocator( allocator ){ };
				ChunkyAllocator* allocator;
				uint32_t handle;
			};

			friend ChunkInfo;
			ChunkyAllocator( uint32_t numblocks );
			~ChunkyAllocator();

			ChunkInfo allocate( std::size_t size );

			bool defrag( );

			static size_t blocksize( void ) { return _blocksize; }
			//char* Data( uint32_t block ) { return  _pool + block * _blocksize; }

			size_t freeMemory( void ) const { return _numfreeblocks * _blocksize; }
			size_t maxMemory( void ) const { return _numblocks * _blocksize; }

			std::string strOccupancy( void );

		private:
			struct FreeChunk {
				size_t blocks = 0;
				bool inUse = false;
				FreeChunk * previous = nullptr;
				FreeChunk* next = nullptr;	
			};

		private:
			ChunkyAllocator( const ChunkyAllocator& other ) = delete;
			ChunkyAllocator& operator=( const ChunkyAllocator& rhs ) = delete;

			void setupfreeBlockList( void );
			void free( const ChunkInfo& info );
		private:
			char* _pool = nullptr;
			static const size_t _blocksize = 100;// 512 * 1024;
			size_t _numblocks = 0;
			size_t _numfreeblocks = 0;

			FreeChunk _rootChunk, _endChunk; // Really just for stack storage, I actually use the pointers instead
			FreeChunk* _root = &_rootChunk;
			FreeChunk* _end = &_endChunk;

			std::vector<FreeChunk*> allocatedChunks;
			std::stack<uint32_t> freeHandles;
			std::default_random_engine generator;
			//std::mutex _allocLock;
		};


		ChunkyAllocator::ChunkyAllocator( uint32_t blocks )
		{
			_numfreeblocks = _numblocks = blocks;
			_pool = new char[_numblocks * _blocksize];

			// Make blocks form a linked list (all of them at startup)
			setupfreeBlockList();
		}

		ChunkyAllocator::~ChunkyAllocator()
		{
			delete[] _pool;
		}

		// Returns the slot that was allocated at. If no suitable slot was found, throw
		ChunkyAllocator::ChunkInfo ChunkyAllocator::allocate( std::size_t size )
		{
			// This first part finds a suitable allocation slot

			//_allocLock.lock();

			FreeChunk* walker = _root->next;
			auto actualSizeNeeded = size + sizeof( size_t ) + sizeof( bool );
			size_t numberOfNeededblocks = actualSizeNeeded / _blocksize;
			if ( actualSizeNeeded % _blocksize )
				numberOfNeededblocks++;

			// Keep trying until we find a free chunk with enough blocks in.
			while ( walker != _end )
			{
				if ( walker->blocks >= numberOfNeededblocks )
					break;

				walker = walker->next;
			}

			if ( walker == _end )
			{
				//_allocLock.unlock();
				throw 1; // TODO
			}

			// Given a valid allocation slot and number of blocks -- extract those blocks
			// from the list of free blocks and fix the broken links. In case the chunk
			// has more blocks than required, it's split to keep the remaining ones.
			FreeChunk* nextChunk;
			if ( walker->blocks > numberOfNeededblocks )
			{
				nextChunk = reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(walker) + numberOfNeededblocks * _blocksize);
				nextChunk->blocks = walker->blocks - numberOfNeededblocks;
				nextChunk->previous = walker->previous;
				nextChunk->next = walker->next;
				walker->previous->next = nextChunk;
				walker->next->previous = nextChunk;
			}
			else
			{
				nextChunk = walker->next;
				nextChunk->previous = walker->previous;
				walker->previous->next = nextChunk;
			}

			_numfreeblocks -= numberOfNeededblocks;

			//_allocLock.unlock();

			// Convert pointer to slot index
			//walker->blocks
			//return (reinterpret_cast<char*>(walker) - _pool) / _blocksize;

			walker->inUse = false;
			walker->blocks = numberOfNeededblocks;

			ChunkInfo info( this );
			if ( !freeHandles.empty() )
			{
				info.handle = freeHandles.top();
				freeHandles.pop();
				allocatedChunks[info.handle] = walker;
			}
			else
			{
				info.handle = uint32_t( allocatedChunks.size() );
				allocatedChunks.push_back(walker);
			}
			return info;
		}

		// frees certain blocks by inserting them into the free block list.
		void ChunkyAllocator::free( const ChunkInfo& info )
		{
			//_allocLock.lock();

			FreeChunk* returnChunk = allocatedChunks[info.handle];
	
			// We must find where in the list to insert and properly update the blocks
			// before and after those we are returning (if existing).
			FreeChunk* before = _root;
			while ( before->next != _end && before->next < returnChunk )
			{
				before = before->next;
			}
			FreeChunk* after = before->next;

			// If we return blocks at the end of a chunk we merge it right away.
			if ( (reinterpret_cast<char*>(before) + before->blocks * _blocksize) == reinterpret_cast<char*>(returnChunk) )
			{
				before->blocks += returnChunk->blocks;
				returnChunk = before; // We do this to simplify potential two-way merge later
			}
			// Otherwise they must be linked together
			else
			{
				before->next = returnChunk;
				returnChunk->previous = before;
			}

			// If the end of the returned chunk coincides with the next free chunk we
			// can merge that way as well.
			if ( after != _end && (reinterpret_cast<char*>(returnChunk) + returnChunk->blocks * _blocksize) == reinterpret_cast<char*>(after) )
			{
				returnChunk->blocks += after->blocks;
				returnChunk->next = after->next;
				returnChunk->next->previous = returnChunk;
			}
			// Otherwise they must be linked together
			else
			{
				returnChunk->next = after;
				returnChunk->next->previous = returnChunk;
			}

			_numfreeblocks += returnChunk->blocks;

			freeHandles.push( info.handle );

			//_allocLock.unlock();
		}

		// Performs one defragmentation iteration. Returns which index of the provided
		// list was moved or -1 if none.
		bool ChunkyAllocator::defrag( )
		{

			//std::uniform_int_distribution<size_t> distribution( 0U, allocatedChunks.size() - 1U );
			//size_t i = distribution( generator );
			//if ( !allocatedChunks[i]->inUse )
			//{
			//	FreeChunk* free = _root->next;
			//	if (free  )



			//}
			//_allocLock.lock();

			FreeChunk* free = _root->next;

			// No free chunks, nothing to defrag
			if ( free == _end )
			{
				//_allocLock.unlock();
				return false;
			}

			// Naïve defragmentation that just stuffs upcoming occupied blocks into the free one.
			std::uniform_int_distribution<size_t> distribution( 0U, allocatedChunks.size() - 1U );
			size_t i = distribution( generator );
			if ( !allocatedChunks[i]->inUse )
			{
				// Found an allocation that comes directly after the free block (defrag left)
				if ( reinterpret_cast<char*>(free) + free->blocks * _blocksize == (char*)allocatedChunks[i] )
				{
					// Make a copy of the old free block before overwriting data
					FreeChunk oldfree;
					oldfree = *free;

					// Copy data into the free chunk (memmove supports overlaps, so even
					// if the free chunk is just one block, we can still move 2 blocks)
					memmove( free, allocatedChunks[i], allocatedChunks[i]->blocks * _blocksize );
					
					// Move the pointer
					allocatedChunks[i] = free;

					// Move start of free chunk to accomodate the newly inserted data and
					// incorporate the newly freed slots when we're at it
					FreeChunk* newfree = reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(free) + allocatedChunks[i]->blocks * _blocksize);
					newfree->blocks = oldfree.blocks;
					newfree->previous = oldfree.previous;
					newfree->next = oldfree.next;
					newfree->previous->next = newfree;
					newfree->next->previous = newfree;

				
					// If the next free block comes immediately after, we merge
					if ( newfree->next == reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(newfree) + newfree->blocks * _blocksize) )
					{
						newfree->blocks += newfree->next->blocks;
						newfree->next = newfree->next->next;
						newfree->next->previous = newfree;
					}
					//	_allocLock.unlock();
					return true;
				}
				// Found an allocation that comes after the free block and is the same size of the free space (defrag left)
				else if( reinterpret_cast<char*>(free) + free->blocks * _blocksize < (char*)allocatedChunks[i] && allocatedChunks[i] ->blocks == free->blocks)
				{
					// Make a copy of the old free block before overwriting data
					FreeChunk oldfree;
					oldfree = *free;

					// Copy data into the free chunk
					memcpy( free, allocatedChunks[i], allocatedChunks[i]->blocks * _blocksize );

					// Move the pointer
					allocatedChunks[i] = free;

					// Move start of free chunk to accomodate the newly inserted data and
					// incorporate the newly freed slots when we're at it
					FreeChunk* newfree = reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(free) + allocatedChunks[i]->blocks * _blocksize);
					newfree->blocks = oldfree.blocks;
					newfree->previous = oldfree.previous;
					newfree->next = oldfree.next;
					newfree->previous->next = newfree;
					newfree->next->previous = newfree;


					// If the next free block comes immediately after, we merge
					if ( newfree->next == reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(newfree) + newfree->blocks * _blocksize) )
					{
						newfree->blocks += newfree->next->blocks;
						newfree->next = newfree->next->next;
						newfree->next->previous = newfree;
					}
				}
			}

			//	_allocLock.unlock();
			return false;
		}

		// Little debug function that outputs occupancy of blocks where O indicates open
		// and X indicates occupied.
		std::string  ChunkyAllocator::strOccupancy( void )
		{
			std::stringstream ss;
			if ( _numblocks == 0 )
			{
				return "";
			}

			char* walker = _pool;
			FreeChunk* free = _root->next; // Should not be end

										   // As long as walker hasn't reached the end
			while ( walker < (_pool + _numblocks * _blocksize) )
			{
				if ( reinterpret_cast<FreeChunk*>(walker) == free )
				{
					for ( uint32_t i = 0; i < free->blocks; ++i )
					{
						//OutputDebugStringA( "[O]" );
						ss << "[O]";
					}

					walker += free->blocks * _blocksize;
					free = free->next;
				}
				else
				{
					//OutputDebugStringA( "[X]" );
					ss << "[X]";
					walker += _blocksize;
				}
			}

			//OutputDebugStringA( "\n" );
			ss << std::endl;
			return ss.str();
		}

		void ChunkyAllocator::setupfreeBlockList( void )
		{
			//_allocLock.lock();

			_root->next = _end;
			_root->previous = nullptr;
			_root->inUse = false;
			_end->next = nullptr;
			_end->previous = _root;
			_end->inUse = false;

			if ( _numblocks == 0 )
				return;

			FreeChunk* walker = reinterpret_cast<FreeChunk*>(_pool);
			walker->blocks = _numblocks;
			walker->inUse = false;
			_root->next = walker;
			walker->previous = _root;
			walker->next = _end;
			_end->previous = walker;

			//_allocLock.unlock();
		}
	}
}
#endif // !_UTILITIES_CHUNKY_ALLOCATOR_H_