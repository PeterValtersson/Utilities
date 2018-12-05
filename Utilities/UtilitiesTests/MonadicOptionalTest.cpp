#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/MonadicOptional.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	class A {
	public:
		int a;
		A(int a) : a(a) {}
		int Add()
		{
			a++;
			return a;
		}

	};
	int Add(int mon)
	{
		return mon + 1;
	}
	int& Add2(int& mon)
	{
		mon = mon + 1;
		return mon;
	}
	TEST_CLASS(MonadicTest)
	{
	public:
		TEST_METHOD(MonadicTest_Map)
		{
			Utilities::optional<int> tmo(1);
			Assert::AreEqual(1, *tmo);
			*tmo = 2;
			Assert::AreEqual(2, *tmo);
			tmo = tmo.map(&Add);
			Assert::AreEqual(3, *tmo);
			tmo = tmo.map([](int a) {return a * 2; });
			Assert::AreEqual(6, *tmo);
			tmo.map(&Add2);
			Assert::AreEqual(7, *tmo);

			Utilities::optional<A> tmoa{5};
			Assert::AreEqual(5, tmoa->a);
			auto tmoa2 = tmoa.map(&A::Add);
			Assert::AreEqual(6, tmoa->a);
			Assert::AreEqual(6, *tmoa2);
		}


	};
}