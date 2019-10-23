#include "stdafx.h"
#include "CppUnitTest.h"
#include <Utilities/Profiler/Profiler_Master.h>
#include <future>
#include "../ProfileDLLTest/head.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	void Calculate()
	{
		PROFILE;
	}
	void test_main()
	{
		PROFILE;
		Calculate();
		Calculate();
		Calculate();
		Calculate();
	}
	
	TEST_CLASS(ProfilerTests)
	{
	public:
		TEST_METHOD(Profiler_Basic)
		{
			auto master = Utilities::Profiler_Master::get();
			test_main();
			auto handle = std::async(std::launch::async, []
			{
				for (int i = 0; i < 100000; i++)
					test_main();
				Main_Entry();
			});


			Main_Entry();


			for (int i = 0; i < 100000; i++)
				test_main();



			handle.get();

#ifdef _ENABLE_PROFILER_
			Logger::WriteMessage( master->to_str().c_str());
			master->generate_tree("Profiles", true);
#endif
		}


	};
}
