#ifndef _UTILITIES_CHUNKY_ALLOCATOR_H_
#define _UTILITIES_CHUNKY_ALLOCATOR_H_

#include <stdint.h>
#include <mutex>
#include <list>

namespace Utilities
{
	namespace Memory
	{


		class ChunkyAllocator {
		public:
			struct ChunkInfo {
				friend ChunkyAllocator;
				void setUsedState( bool inUse ) { freeForDefrag = !inUse; }

			private:
				ChunkInfo* previous = nullptr;
				ChunkInfo* next = nullptr;
				bool freeForDefrag;
				uint32_t blocks = 0;
			};
			ChunkyAllocator( uint32_t numblocks );
			~ChunkyAllocator();

			ChunkInfo& allocate( std::size_t size );
			void free( int32_t firstBlock, uint32_t numblocks );
			bool defrag( std::list<std::pair<uint32_t&, uint32_t>>& allocs );

			static uint32_t blocksize( void ) { return _blocksize; }
			//char* Data( uint32_t block ) { return  _pool + block * _blocksize; }

			uint32_t freeMemory( void ) const { return _numfreeblocks * _blocksize; }
			uint32_t maxMemory( void ) const { return _numblocks * _blocksize; }

			void printOccupancy( void );

		private:
			

		private:
			ChunkyAllocator( const ChunkyAllocator& other ) = delete;
			ChunkyAllocator& operator=( const ChunkyAllocator& rhs ) = delete;

			void _SetupfreeBlockList( void );

		private:
			char* _pool = nullptr;
			static const uint32_t _blocksize = 512 * 1024;
			uint32_t _numblocks = 0;
			uint32_t _numfreeblocks = 0;

			ChunkInfo _rootChunk, _endChunk; // Really just for stack storage, I actually use the pointers instead
			ChunkInfo* _root = &_rootChunk;
			ChunkInfo* _end = &_endChunk;

			//std::mutex _allocLock;
		};


		ChunkyAllocator::ChunkyAllocator( uint32_t blocks )
		{
			_numfreeblocks = _numblocks = blocks;
			_pool = new char[ _numblocks * _blocksize];

			// Make blocks form a linked list (all of them at startup)
			_SetupfreeBlockList();
		}

		ChunkyAllocator::~ChunkyAllocator()
		{
			delete[] _pool;
		}

		// Returns the slot that was allocated at. If no suitable slot was found, throw
		ChunkyAllocator::ChunkInfo& ChunkyAllocator::allocate( std::size_t size )
		{
			// This first part finds a suitable allocation slot

			//_allocLock.lock();

			ChunkInfo* walker = _root->next;
			uint32_t numberOfNeededblocks = size / _blocksize;
			if ( size % _blocksize )
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
			ChunkInfo* nextChunk;
			if ( walker->blocks > numberOfNeededblocks )
			{
				nextChunk = reinterpret_cast<ChunkInfo*>(reinterpret_cast<char*>(walker) + numberOfNeededblocks * _blocksize);
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
			walker->blocks
			return (reinterpret_cast<char*>(walker) - _pool) / _blocksize;
		}

		// frees certain blocks by inserting them into the free block list.
		void ChunkyAllocator::free( int32_t first, uint32_t numblocks )
		{
			//_allocLock.lock();

			ChunkInfo* returnChunk = reinterpret_cast<ChunkInfo*>(_pool + first * _blocksize);
			returnChunk->blocks = numblocks;

			// We must find where in the list to insert and properly update the blocks
			// before and after those we are returning (if existing).
			ChunkInfo* before = _root;
			while ( before->next != _end && before->next < returnChunk )
			{
				before = before->next;
			}
			ChunkInfo* after = before->next;

			// If we return blocks at the end of a chunk we merge it right away.
			if ( (reinterpret_cast<char*>(before) + before->blocks * _blocksize) == reinterpret_cast<char*>(returnChunk) )
			{
				before->blocks += numblocks;
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

			_numfreeblocks += numblocks;

			//_allocLock.unlock();
		}

		// Performs one defragmentation iteration. Returns which index of the provided
		// list was moved or -1 if none.
		bool ChunkyAllocator::defrag( std::list<std::pair<uint32_t&, uint32_t>>& allocs )
		{
			//_allocLock.lock();

			ChunkInfo* free = _root->next;

			// No free chunks, nothing to defrag
			if ( free == _end )
			{
				//_allocLock.unlock();
				return false;
			}

			// Naïve defragmentation that just stuffs upcoming occupied blocks into the free one.
			for ( auto& alloc : allocs )
			{
				// Found an allocation that comes after the free block (defrag left)
				if ( reinterpret_cast<char*>(free) + free->blocks * _blocksize == _pool + alloc.first * _blocksize )
				{
					// Make a copy of the old free block before overwriting data
					ChunkInfo oldfree;
					oldfree = *free;

					// Copy data into the free chunk (memmove supports overlaps, so even
					// if the free chunk is just one block, we can still move 2 blocks)
					memmove( free, _pool + alloc.first * _blocksize, alloc.second * _blocksize );

					// Move start of free chunk to accomodate the newly inserted data and
					// incorporate the newly freed slots when we're at it
					ChunkInfo* newfree = reinterpret_cast<ChunkInfo*>(reinterpret_cast<char*>(free) + alloc.second * _blocksize);
					newfree->blocks = oldfree.blocks;
					newfree->previous = oldfree.previous;
					newfree->next = oldfree.next;
					newfree->previous->next = newfree;
					newfree->next->previous = newfree;

					// Allocation was moved to the start of the free chunk before, so its
					// alloc slot is shifted by the number of blocks of the free chunk.
					alloc.first -= newfree->blocks;

					// If the next free block comes immediately after we merge
					if ( newfree->next == reinterpret_cast<ChunkInfo*>(reinterpret_cast<char*>(newfree) + newfree->blocks * _blocksize) )
					{
						newfree->blocks += newfree->next->blocks;
						newfree->next = newfree->next->next;
						newfree->next->previous = newfree;
					}
				//	_allocLock.unlock();
					return true;
				}

			}

		//	_allocLock.unlock();
			return false;
		}

		// Little debug function that outputs occupancy of blocks where O indicates open
		// and X indicates occupied.
		void ChunkyAllocator::printOccupancy( void )
		{
			if ( _numblocks == 0 )
			{
				return;
			}

			char* walker = _pool;
			ChunkInfo* free = _root->next; // Should not be end

										   // As long as walker hasn't reached the end
			while ( walker < (_pool + _numblocks * _blocksize) )
			{
				if ( reinterpret_cast<ChunkInfo*>(walker) == free )
				{
					for ( uint32_t i = 0; i < free->blocks; ++i )
					{
						//OutputDebugStringA( "[O]" );
						printf( "[O]" );
					}

					walker += free->blocks * _blocksize;
					free = free->next;
				}
				else
				{
					//OutputDebugStringA( "[X]" );
					printf( "[X]" );
					walker += _blocksize;
				}
			}

			//OutputDebugStringA( "\n" );
			printf( "\n" );
		}

		void ChunkyAllocator::_SetupfreeBlockList( void )
		{
			//_allocLock.lock();

			_root->next = _end;
			_root->previous = nullptr;
			_end->next = nullptr;
			_end->previous = _root;

			if ( _numblocks == 0 )
				return;

			ChunkInfo* walker = reinterpret_cast<ChunkInfo*>(_pool);
			walker->blocks = _numblocks;
			_root->next = walker;
			walker->previous = _root;
			walker->next = _end;
			_end->previous = walker;

			//_allocLock.unlock();
		}
	}
}
#endif // !_UTILITIES_CHUNKY_ALLOCATOR_H_