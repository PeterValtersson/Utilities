#include "stdafx.h"
#include "CppUnitTest.h"
#include <Profiler.h>
#include <future>
#include "../ProfileDLLTest/head.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	void DoSomething()
	{
		PROFILE;
	}
	void Entry()
	{
		PROFILE;
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
			PROFILER_INIT;
			Entry();
			auto handle = std::async(std::launch::async, []
			{
				for (int i = 0; i < 100000; i++)
					Entry();
				Main_Entry();
			});


			Main_Entry();


			for (int i = 0; i < 100000; i++)
				Entry();



			handle.get();

#ifdef _ENABLE_PROFILER_
			Logger::WriteMessage(Utilities::Profiler_Master::get()->str().c_str());
			Utilities::Profiler_Master::get()->createDotAndBat("Profiles", true);
#endif
		}


	};
}
