#include "stdafx.h"
#include "CppUnitTest.h"
#define __PROFILE
#include "../Include/Profiler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{

	void foo(int a)
	{
		StartProfile;
	}

	TEST_CLASS(ProfileTestClass)
	{
	public:

		TEST_METHOD(SingleFunction)
		{
			foo(5);
		}
		
	};
}