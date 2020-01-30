#include "stdafx.h"
#include "CppUnitTest.h"
#include <Utilities/Delegate.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	TEST_CLASS( DelegateTests ){
public:
	TEST_METHOD( Method )
	{
		struct A{
			int a()
			{
				return 1337;
			};
		}a;
		Utilities::Delegate<int()> d{ &a, &A::a };
		d = { &a, &A::a };

		Assert::AreEqual( 1337, d() );
		static constexpr size_t todo = 10000000;
		for ( size_t i = 0; i < todo; i++ )
			d();
	}
	};
}