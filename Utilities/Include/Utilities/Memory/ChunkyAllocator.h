#ifndef _UTILITIES_CHUNKY_ALLOCATOR_H_
#define _UTILITIES_CHUNKY_ALLOCATOR_H_

#include <stdint.h>
//#include <mutex>
//#include <list>
//#include <stack>
#include <random>
#include <map>
#include <Utilities/ErrorHandling.h>
#include "MemoryBlock.h"
#include <sstream>
#include <functional>
#include "Memory_Literals.h"
#undef max
#undef min
namespace Utilities
{
	namespace Memory
	{
		struct InvalidHandle : public Utilities::Exception{
			InvalidHandle( const std::string& where ) : Utilities::Exception( "Invalid handle in: " + where )
			{}
		};
		struct OutOfMemory : public Utilities::Exception{
			OutOfMemory() : Utilities::Exception( "Ran out of memory when trying to allocate" )
			{}
		};

		typedef size_t Handle;
		constexpr Handle null_handle = std::numeric_limits<Handle>::max();

		class ChunkyAllocator{
		public:
			ChunkyAllocator( const size_t numblocks )noexcept
			{
				_numfreeblocks = _numblocks = numblocks;
				_pool = new char[_numblocks * _blocksize];

				// Make blocks form a linked list (all of them at startup)
				setupfreeBlockList();
			}

			ChunkyAllocator( ChunkyAllocator&& other )noexcept
			{
				_numblocks = other._numblocks;
				_numfreeblocks = other._numfreeblocks;
				_pool = other._pool; other._pool = nullptr;
				_rootChunk = std::move( other._rootChunk );
				_endChunk = std::move( other._endChunk );
				_root = &_rootChunk;
				_end = &_endChunk;

				setupfreeBlockList();

				allocatedChunks = std::move( other.allocatedChunks );
				handleToIndex = std::move( other.handleToIndex );
				allocatedChunks = std::move( other.allocatedChunks );
			}
			~ChunkyAllocator()
			{
				delete[] _pool;
			}

			const size_t _allocate( const size_t size, const Handle handle = null_handle )
			{
				// This first part finds a suitable allocation slot

				//_allocLock.lock();

				FreeChunk* walker = _root->next;
				auto actualSizeNeeded = size + sizeof( size_t ) * 2;
				size_t numberOfNeededblocks = actualSizeNeeded / _blocksize + ((actualSizeNeeded % _blocksize) > 0);

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
					throw OutOfMemory();
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


				walker->blocks = numberOfNeededblocks;
				walker->used_size = size;

				allocatedChunks.push_back( { walker, handle } );
				return allocatedChunks.size() - 1;
			}
			const Handle allocate( const std::size_t size )
			{
				const auto index = _allocate( size );

				std::uniform_int_distribution<size_t> distribution( 0U, SIZE_MAX - 1 );
				size_t handle = distribution( generator );
				while ( handleToIndex.find( handle ) != handleToIndex.end() )
					handle = distribution( generator );


				handleToIndex[handle] = index;
				allocatedChunks.back().handle = handle;

				return handle;
			}
			void free( const Handle handle )
			{
				if ( auto findIndex = handleToIndex.find( handle ); findIndex == handleToIndex.end() )
					throw InvalidHandle( "Utilities::Memory::ChunkyAllocator::free" );
				else
				{
					auto index = findIndex->second;
					FreeChunk* returnChunk = allocatedChunks[index].chunk;

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

					allocatedChunks[index] = allocatedChunks.back();
					handleToIndex[allocatedChunks[index].handle] = index;
					allocatedChunks.pop_back();
					handleToIndex.erase( handle );
				}
			}

			void use_data( const Handle handle, const std::function<void( MemoryBlock )>& callback )
			{
				if ( auto findIndex = handleToIndex.find( handle ); findIndex == handleToIndex.end() )
					throw InvalidHandle( "Utilities::Memory::ChunkyAllocator::use_data" );
				else
				{
					auto chunk = allocatedChunks[findIndex->second].chunk;
					std::function<MemoryBlock( size_t )> realloc_callback;
					realloc_callback = [&]( size_t size )->MemoryBlock // Might want to switch to using a general allocator interface instead and use this instead.
					{

						free( handle );
						const auto index = _allocate( size, handle );
						handleToIndex[handle] = index;
						allocatedChunks.back().handle = handle;
						auto chunk = allocatedChunks[index].chunk;

						return  MemoryBlock( (char*)chunk + sizeof( size_t ) * 2,
											 chunk->used_size,
											 chunk->blocks * _blocksize - sizeof( size_t ) * 2,
											 realloc_callback );
					};
					callback( MemoryBlock( (char*)chunk + sizeof( size_t ) * 2,
										   chunk->used_size,
										   chunk->blocks * _blocksize - sizeof( size_t ) * 2,
										   realloc_callback ) );
				}
			}

			void peek_data( const Handle handle, const std::function<void( const ConstMemoryBlock )>& callback )const
			{
				if ( auto findIndex = handleToIndex.find( handle ); findIndex == handleToIndex.end() )
					throw InvalidHandle( "Utilities::Memory::ChunkyAllocator::peek_data" );
				else
				{
					auto index = findIndex->second;
					callback( { (char*)allocatedChunks[index].chunk + sizeof( size_t ) * 2, allocatedChunks[index].chunk->used_size, allocatedChunks[index].chunk->blocks * _blocksize - sizeof( size_t ) * 2 } );
				}
			}

			void write_data( const Handle handle, const void* const data, const size_t size )
			{
				if ( auto findIndex = handleToIndex.find( handle ); findIndex == handleToIndex.end() )
					throw InvalidHandle( "Utilities::Memory::ChunkyAllocator::use_data" );
				else
				{
					auto chunk = allocatedChunks[findIndex->second].chunk;
					std::function<const MemoryBlock( size_t )> realloc_callback;
					realloc_callback = [&]( size_t size )->const MemoryBlock // Might want to switch to using a general allocator interface instead and use this instead.
					{

						free( handle );
						const auto index = _allocate( size, handle );
						handleToIndex[handle] = index;
						allocatedChunks.back().handle = handle;
						auto chunk = allocatedChunks[index].chunk;

						return  MemoryBlock( (char*)chunk + sizeof( size_t ) * 2,
											 chunk->used_size,
											 chunk->blocks * _blocksize - sizeof( size_t ) * 2,
											 realloc_callback );
					};
					MemoryBlock m( (char*)chunk + sizeof( size_t ) * 2,
								   chunk->used_size,
								   chunk->blocks * _blocksize - sizeof( size_t ) * 2,
								   realloc_callback );
					m.write( data, size );
				}
			}

			const bool defrag()
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

				// Na�ve defragmentation that just stuffs upcoming occupied blocks into the free one.
				std::uniform_int_distribution<size_t> distribution( 0U, allocatedChunks.size() - 1U );
				size_t i = distribution( generator );

				// Found an allocation that comes directly after the free block (defrag left)
				if ( reinterpret_cast<char*>(free) + free->blocks * _blocksize == (char*)allocatedChunks[i].chunk )
				{
					// Make a copy of the old free block before overwriting data
					FreeChunk oldfree;
					oldfree = *free;

					// Copy data into the free chunk (memmove supports overlaps, so even
					// if the free chunk is just one block, we can still move 2 blocks)
					memmove( free, allocatedChunks[i].chunk, allocatedChunks[i].chunk->blocks * _blocksize );

					// Move the pointer
					allocatedChunks[i].chunk = free;

					// Move start of free chunk to accomodate the newly inserted data and
					// incorporate the newly freed slots when we're at it
					FreeChunk* newfree = reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(free) + allocatedChunks[i].chunk->blocks * _blocksize);
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
				else if ( reinterpret_cast<char*>(free) + free->blocks * _blocksize < (char*)allocatedChunks[i].chunk && allocatedChunks[i].chunk->blocks == free->blocks )
				{
					// Make a copy of the old free block before overwriting data
					FreeChunk oldfree;
					oldfree = *free;

					// Copy data into the free chunk
					memcpy( free, allocatedChunks[i].chunk, allocatedChunks[i].chunk->blocks * _blocksize );

					// Move the pointer
					allocatedChunks[i].chunk = free;

					// Move start of free chunk to accomodate the newly inserted data and
					// incorporate the newly freed slots when we're at it
					FreeChunk* newfree = reinterpret_cast<FreeChunk*>(reinterpret_cast<char*>(free) + allocatedChunks[i].chunk->blocks * _blocksize);
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


				//	_allocLock.unlock();
				return false;
			}

			constexpr static size_t blocksize( void )
			{
				return _blocksize;
			}


			const size_t freeMemory( void ) const
			{
				return _numfreeblocks * _blocksize;
			}
			const size_t maxMemory( void ) const
			{
				return _numblocks * _blocksize;
			}

			const std::string strOccupancy( void )
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


			const bool isValid( const Handle handle )const
			{
				return handleToIndex.find( handle ) != handleToIndex.end();
			}
		private:
			struct FreeChunk{
				size_t blocks = 0;
				size_t used_size = 0;
				FreeChunk* previous = nullptr;
				FreeChunk* next = nullptr;
			};

			struct AllocatedChunkInfo{
				FreeChunk* chunk;
				Handle handle;
			};
		private:
			ChunkyAllocator( const ChunkyAllocator& other ) = delete;
			ChunkyAllocator& operator=( const ChunkyAllocator& rhs ) = delete;

			void setupfreeBlockList( void )
			{
				//_allocLock.lock();

				_root->next = _end;
				_root->previous = nullptr;

				_end->next = nullptr;
				_end->previous = _root;

				if ( _numblocks == 0 )
					return;

				FreeChunk* walker = reinterpret_cast<FreeChunk*>(_pool);
				walker->blocks = _numblocks;

				_root->next = walker;
				walker->previous = _root;
				walker->next = _end;
				_end->previous = walker;

				//_allocLock.unlock();
			}

		private:
			char* _pool = nullptr;
			constexpr static size_t _blocksize = 512 * 1024;
			size_t _numblocks = 0;
			size_t _numfreeblocks = 0;

			FreeChunk _rootChunk, _endChunk; // Really just for stack storage, I actually use the pointers instead
			FreeChunk* _root = &_rootChunk;
			FreeChunk* _end = &_endChunk;



			std::vector<AllocatedChunkInfo> allocatedChunks;
			std::map<size_t, size_t> handleToIndex;
			std::default_random_engine generator;
			//std::mutex _allocLock;
		};



	}
}
#endif // !_UTILITIES_CHUNKY_ALLOCATOR_H_