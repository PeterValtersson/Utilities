#ifndef _UTILITIES_MEMORYBLOCK_H_
#define _UTILITIES_MEMORYBLOCK_H_
namespace Utilities
{
	namespace Memory
	{
		struct MemoryBlock {
			void* data;
			size_t used_size;
			size_t total_size;
		};


	
	}
}

#endif