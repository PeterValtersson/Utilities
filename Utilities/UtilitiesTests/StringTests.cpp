#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/StringUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{		
	TEST_CLASS(StringTests)
	{
	public:
		TEST_METHOD( get_path_split_last2_three )
		{
			Assert::AreEqual( "test2/test3", Utilities::String::get_path_split_last2( "test/test2/test3" ) );
		}
		TEST_METHOD( get_path_split_last2_two )
		{
			Assert::AreEqual( "test2/test3", Utilities::String::get_path_split_last2( "test2/test3" ) );
		}
		TEST_METHOD( get_path_split_last2_one )
		{
			Assert::AreEqual( "test3", Utilities::String::get_path_split_last2( "test3" ) );
		}
		TEST_METHOD( get_function_name_closest_namespace_three )
		{
			Assert::AreEqual( "test2::test3", Utilities::String::get_function_name_closest_namespace( "test::test2::test3" ) );
		}
		TEST_METHOD( get_function_name_closest_namespace_two )
		{
			Assert::AreEqual( "test2::test3", Utilities::String::get_function_name_closest_namespace( "test2::test3" ) );
		}
		TEST_METHOD( get_function_name_closest_namespace_one )
		{
			Assert::AreEqual( "test3", Utilities::String::get_function_name_closest_namespace( "test3" ) );
		}
	};
}