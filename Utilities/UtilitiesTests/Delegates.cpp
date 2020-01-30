#include "stdafx.h"
#include "CppUnitTest.h"
#include <Utilities/Delegate.h>
#include <Utilities/Event.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	int free_func()
	{
		return 5;
	}
	int free_func2()
	{
		return 5;
	}
	int free_func3()
	{
		return 1;
	}
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

		A b;
		Utilities::Delegate<int()> d2{ &b, &A::a };
		Assert::IsTrue( d == d );
		Assert::IsFalse( d == d2 );
	}

	TEST_METHOD( FreeFunction )
	{
		Utilities::Delegate<int()> d = &free_func;
		Assert::AreEqual( 5, d() );
		Assert::IsTrue( d == d );

		Utilities::Delegate<int()> d2 = free_func;
		Assert::IsTrue( d == d2 );

		Utilities::Delegate<int()> d3 = &free_func2;
		Assert::IsFalse( d == d3 );

		Utilities::Delegate<int()> d4 = &free_func3;
		Assert::IsFalse( d == d4 );

	}

	TEST_METHOD( Lambda )
	{
		auto l = std::function<int()>( []()
		{
			return 5;
		} );
		auto l2 = std::function<int()>( []()
		{
			return 5;
		} );
		Utilities::Delegate<void()> dd( []
		{} );

		Utilities::Delegate<int()> d = l;

		Assert::AreEqual( 5, d() );
		Assert::IsTrue( d == d );

		Utilities::Delegate<int()> d2 = l;
		Assert::IsTrue( d == d2 );

		Utilities::Delegate<int()> d3 = l2;
		Assert::IsFalse( d == d3 );

	}

	TEST_METHOD( Event )
	{
		Utilities::Event<int()> e;
		e += free_func;

		struct A{
			int a()
			{
				return 1337;
			};
		}a;

		e += {&a, & A::a};


		auto l = std::function<int()>( []()
		{
			return 5;
		} );

		e += l;

		Assert::AreEqual<size_t>( 3, e.NumRegisteredCallbacks() );

		e();

		e( []( size_t i, int& v )
		{
			if ( i == 0 )
				Assert::AreEqual( 5, v );
			else if ( i == 1 )
				Assert::AreEqual( 1337, v );
			else
				Assert::AreEqual( 5, v );
		} );

		e -= free_func;
		Assert::AreEqual<size_t>( 2, e.NumRegisteredCallbacks() );

		e -= {&a, & A::a};
		Assert::AreEqual<size_t>( 1, e.NumRegisteredCallbacks() );

		e -= l;
		Assert::AreEqual<size_t>( 0, e.NumRegisteredCallbacks() );

	}
	};
}