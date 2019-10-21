#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/StringUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{		
	TEST_CLASS(GUIDTests)
	{
	public:
		TEST_METHOD( get_path_split_last2 )
		{
			Assert::AreEqual( "test2/test3", Utilities::String::get_path_split_last2( "test/test2/test3" ) );
		}
		TEST_METHOD( get_function_name_closest_namespace )
		{
			Assert::AreEqual( "test2::test3", Utilities::String::get_path_split_last2( "test::test2::test3" ) );
		}
	};
}