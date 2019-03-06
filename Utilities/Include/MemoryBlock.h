#ifndef _UTILITIES_MEMORYBLOCK_H_
#define _UTILITIES_MEMORYBLOCK_H_
namespace Utilities
{
	namespace Allocators
	{
		struct MemoryBlock {
			void* data;
			size_t size;
		};


		using Handle = size_t;
		struct ChunkyData {
			ChunkyData( ChunkyAllocator& chunkyAllocator, Handle handle, MemoryBlock data ) : chunkyAllocator( chunkyAllocator ), handle( handle ), _data( data )
			{
				chunkyAllocator.tallyUp( handle );
			}
			~ChunkyData()
			{
				chunkyAllocator.tallyDown( handle );
			}
			ChunkyData( const ChunkyData& other ) : chunkyAllocator( other.chunkyAllocator ), handle( other.handle ), _data( other._data )
			{
				chunkyAllocator.tallyUp( handle );
			}
			ChunkyData& operator=( const ChunkyData& other ) = delete;

			inline MemoryBlock& data() { return _data; }
		private:
			ChunkyAllocator & chunkyAllocator;
			Handle handle;
			MemoryBlock _data;
		};
	}
}

#endif