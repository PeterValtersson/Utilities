#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/Profiler.h"
#include <future>
#include "../ProfileDLLTest/head.h"

#pragma comment(lib, "ProfileDLLTest.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	void DoSomething()
	{
		Profile;
	}
	void Entry()
	{
		Profile;
		DoSomething();
		DoSomething();
		DoSomething();
		DoSomething();
	}

	TEST_CLASS(ProfilerTests)
	{
	public:
		TEST_METHOD(Profiler_Basic)
		{
			

			auto handle = std::async(std::launch::async, []()
			{
				for (int i = 0; i < 100000; i++)
					Entry();
			});


			Main_Entry();


			for (int i = 0; i < 100000; i++)
				Entry();





			handle.get();
			Logger::WriteMessage(Utilities::Profiler_Data_Collector::get()->str().c_str());
			

		}


	};
}