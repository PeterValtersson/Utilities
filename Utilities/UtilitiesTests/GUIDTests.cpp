#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/GUID.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{		
	TEST_CLASS(GUIDTests)
	{
	public:
		
		TEST_METHOD(GUID_Performance_std)
		{
			std::string str = "asasdasd";
			uint32_t hash = 0;
			for (int i = 0; i < 200000; i++)
				hash = uint32_t(std::hash<std::string>{}(str));
		}
		TEST_METHOD(GUID_Performance_GUIDRuntime)
		{
			std::string str = "asasdasd";
			Utilities::GUID hash = 0;
			for (int i = 0; i < 200000; i++)
				hash = str;
		}
		TEST_METHOD(GUID_Performance_GUIDCompiletime)
		{
			std::string str = "asasdasd";
			Utilities::GUID hash = 0;
			for (int i = 0; i < 200000; i++)
				hash = Utilities::GUID("asasdasd");
		}

	};
}