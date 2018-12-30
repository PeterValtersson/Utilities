#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/MonadicOptional.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	
	int Add(int mon)
	{
		return mon + 1;
	}
	int AddE(int mon, int b)
	{
		return mon + b;
	}
	void AddV(int mon)
	{
		Logger::WriteMessage(("AddV: " + std::to_string(mon)).c_str());
	}
	void AddVe(int mon, int a, bool t)
	{
		Logger::WriteMessage(("AddVe: " + std::to_string(mon)).c_str());
	}
	int Five()
	{
		return 5;
	}
	void Five2()
	{
		Logger::WriteMessage("Five");
	}

	int Add2(int& mon)
	{
		mon = mon + 1;
		return mon;
	}
	int Add2e(int& mon, int a)
	{
		mon = mon + a;
		return mon;
	}
	
	void Add2V(int& mon)
	{
		mon++;
		Logger::WriteMessage(("Add2V: " + std::to_string(mon)).c_str());
	}
	void Add2Ve(int& mon, int b)
	{
		mon += b;
		Logger::WriteMessage(("Add2V: " + std::to_string(mon)).c_str());
	}
	int Add2C(int const& mon)
	{
		return mon + 1;
	}
	void Add2VC(int const& mon, int b)
	{
		Logger::WriteMessage(("Add2VC: " + std::to_string(mon)).c_str());
	}
	int AddP(int mon, int a)
	{
		return mon + a;
	}
	void AddPV(int mon, int a)
	{
		Logger::WriteMessage(("Add2VC: " + std::to_string(mon + a)).c_str());
	}

	int foo(int a)
	{
		return a;
	}
	void fooV(int a)
	{
		Logger::WriteMessage(("fooV: " + std::to_string(a)).c_str());
	}
	TEST_CLASS(MonadicTest)
	{
	public:
		TEST_METHOD(MonadicTest_Map_FreeFunctions_Value)

		{
			Utilities::optional<int> tmo(1);
			Assert::AreEqual(1, *tmo);
			*tmo = 2;
			Assert::AreEqual(2, *tmo);

			auto r = tmo.map(&Add).map(&Add);
			Assert::AreEqual(2, *tmo);
			Assert::AreEqual(4, *r);
			tmo.map(&AddV);
			tmo.map(&AddVe, 5, false);
		}
		TEST_METHOD(MonadicTest_Map_FreeFunctions_Ref)
		{
			Utilities::optional<int> tmo(0);
			auto r = tmo.map(&Add2).map(&Add2);
			Assert::AreEqual(1, *tmo);
			Assert::AreEqual(2, *r);

			tmo.map(&Add2V);
			Assert::AreEqual(2, *tmo);
			tmo.map(&Add2Ve, 2);
			Assert::AreEqual(4, *tmo);
		}
		TEST_METHOD(MonadicTest_Map_FreeFunctions_CRef)
		{
			Utilities::optional<int> tmo(0);
			auto r = tmo.map(&Add2C);
			Assert::AreEqual(0, *tmo);
			Assert::AreEqual(1, *r);
			tmo.map(&Add2VC, 2);
			Assert::AreEqual(0, *tmo);
		}
		TEST_METHOD(MonadicTest_Map_FreeFunctions_Void)
		{
			Utilities::optional<int> tmo(0);
			Assert::AreEqual(5, *tmo.map(&Five));
			Assert::AreEqual(0, *tmo);
			tmo.map(&Five2);
			Assert::AreEqual(0, *tmo);
			tmo.map(&Add, 2);
			tmo.map(&AddPV, 2, 4);
		}
		




		class A {
		public:
			int a;
			A(int a) : a(a) {}
			int Add()
			{
				a++;
				return a;
			}
			void AddV()
			{
				a++;
			}
			void AddR(int& b)
			{
				b = a;
			}
		};
		TEST_METHOD(MonadicTest_Map_Methods)
		{
			Utilities::optional<A> tmo(1);
			auto r = tmo.map(&A::Add);
			Assert::AreEqual(2, *r);
			Assert::AreEqual(2, tmo->a);
			tmo.map(&A::AddV);
			Assert::AreEqual(3, tmo->a);
			int a = 7;
			Assert::AreEqual(7, a);
			tmo.map<int&>(&A::AddR, a);
			Assert::AreEqual(3, a);
		}
		TEST_METHOD(MonadicTest_Map_Lambda)
		{
			Utilities::optional<int> tmo(1);
			auto r = tmo.map([](int a) {return a + 1; });
			Assert::AreEqual(2, *r);
			Assert::AreEqual(1, *tmo);
			Assert::AreEqual(5, *tmo.map([](int) {return 5; }));
			Assert::AreEqual(6, *tmo.map([](int a, int b) {return a + b; }, 5));
		}
		
		TEST_METHOD(MonadicTest_Or_Else_FreeFunctions)
		{	
			Utilities::optional<int> tmo;
			Assert::IsFalse(tmo.has_value());
			tmo = tmo.or_else(&foo, 1);
			Assert::IsFalse(tmo.has_value());
			tmo = std::nullopt;
			tmo.or_else(&fooV, 1337);
			



		}

		TEST_METHOD(MonadicTest_Or_Else_This_Lambda)
		{
			{
				Utilities::optional<int> tmo;
				auto res = tmo.or_else_this([] {
					return int(3);
				}).map([](int i)
				{
					return i == 2;
				});
				Assert::IsFalse(*res);
			}
			{
				Utilities::optional<int> tmo;
				auto res = tmo.or_else_this([] {
					return int(3);
				}).map([](int i)
				{
					return i == 3;
				});
				Assert::IsTrue(*res);
			}
		}
	};


}