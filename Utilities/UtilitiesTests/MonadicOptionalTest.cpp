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
	void AddV(int mon)
	{
		Logger::WriteMessage(("AddV: " + std::to_string(mon)).c_str());
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
	void Add2V(int& mon)
	{
		mon++;
		Logger::WriteMessage(("Add2V: " + std::to_string(mon)).c_str());
	}
	int Add2C(int const& mon)
	{
		return mon + 1;
	}
	void Add2VC(int const& mon)
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

	
	TEST_CLASS(MonadicTest)
	{
	public:
		TEST_METHOD(MonadicTest_Map_FreeFunctions)
		{
			Utilities::optional<int> tmo(1);
			Assert::AreEqual(1, *tmo);
			*tmo = 2;
			Assert::AreEqual(2, *tmo);

			tmo.map(&Add);

			/*tmo = tmo.map(&Five2);
			Assert::AreEqual(3, *tmo);*/
			//tmo.map(&AddV);

			//tmo.map(&Add2);
			//Assert::AreEqual(4, *tmo);
			//tmo.map(&Add2V);
			//Assert::AreEqual(5, *tmo);

			//tmo = tmo.map(&Add2C);
			//Assert::AreEqual(6, *tmo);
			//tmo.map(&Add2VC);
			//Assert::AreEqual(6, *tmo);


			//Assert::AreEqual(5, *tmo.map(&Five));
			//tmo.map(&Five2);


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
		};
		TEST_METHOD(MonadicTest_Map_Methods)
		{
			/*Utilities::optional<A> tmo(1);
			auto r = tmo.map(&A::Add);
			Assert::AreEqual(2, *r);
			Assert::AreEqual(2, tmo->a);
			tmo.map(&A::AddV);
			Assert::AreEqual(3, tmo->a);*/
		}


	};
}