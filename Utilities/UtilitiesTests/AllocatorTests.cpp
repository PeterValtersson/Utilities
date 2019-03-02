#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/ChunkyAllocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	TEST_CLASS( AllocatorTests )
	{
	public:
		TEST_METHOD( AllocatorTests_Chunky )
		{
			Utilities::Memory::ChunkyAllocator allocator( 1000 );
			
			auto m1 = allocator.allocate( 100 );
			auto m2 = allocator.allocate( 10 );
			auto m3 = allocator.allocate( 100 );
			auto m4 = allocator.allocate( 100 );
			Logger::WriteMessage( allocator.strOccupancy().c_str() );
			m3.free();

			Logger::WriteMessage(allocator.strOccupancy().c_str());
		}


	};
}
