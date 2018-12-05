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
	void AddV(int mon)
	{
		Logger::WriteMessage(("AddV: " + std::to_string(mon)).c_str());
	}
	int Add2(int& mon)
	{
		mon = mon + 1;
		return mon;
	}
	void Add2V(int& mon)
	{
		Logger::WriteMessage(("Add2V: " + std::to_string(mon)).c_str());
	}
	int Five()
	{
		return 5;
	}
	void Five2()
	{
		Logger::WriteMessage("Five");
	}
	void Add3(int& mon)
	{
		mon++;
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
			tmo.map(&AddV);
			Assert::AreEqual(3, *tmo);
			tmo.map(&AddV);

			tmo.map(&Add2);
			Assert::AreEqual(4, *tmo);
			tmo.map(&Add2V);
			//tmo = tmo.map([](int a) {return a * 2; });
			//Assert::AreEqual(8, *tmo);
			Assert::AreEqual(5, *tmo.map(&Five));
			tmo.map(&Five2);

			/////*tmo.map(&Add3);
			////Assert::AreEqual(8, *tmo);*/

			//Utilities::optional<A> tmoa{5};
			//Assert::AreEqual(5, tmoa->a);
			//auto tmoa2 = tmoa.map(&A::Add);
			//Assert::AreEqual(6, tmoa->a);
			//Assert::AreEqual(6, *tmoa2);
		}


	};
}