#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/StringUtilities.h"
#include <vector>

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
		TEST_METHOD( String_split_4_with_space )
		{
			const std::vector<std::string> splits = Utilities::String::split<std::vector<std::string>>(" This   is  a string ");
			Assert::AreEqual( 4ull, splits.size() );
			Assert::AreEqual<std::string>( "This", splits[0] );
			Assert::AreEqual<std::string>( "is", splits[1] );
			Assert::AreEqual<std::string>( "a", splits[2] );
			Assert::AreEqual<std::string>( "string", splits[3] );
		}
		TEST_METHOD( String_split_1_space_start )
		{
			const std::vector<std::string> splits = Utilities::String::split<std::vector<std::string>>( " Test" );
			Assert::AreEqual( 1ull, splits.size() );
			Assert::AreEqual<std::string>( "Test", splits[0] );
		}
		TEST_METHOD( String_split_1 )
		{
			const std::vector<std::string> splits = Utilities::String::split<std::vector<std::string>>( "Test" );
			Assert::AreEqual( 1ull, splits.size() );
			Assert::AreEqual<std::string>( "Test", splits[0] );
		}
		TEST_METHOD( String_split_2_No_space_end )
		{
			const std::vector<std::string> splits = Utilities::String::split<std::vector<std::string>>( "Test asd" );
			Assert::AreEqual( 2ull, splits.size() );
			Assert::AreEqual<std::string>( "Test", splits[0] );
			Assert::AreEqual<std::string>( "asd", splits[1] );
		}
		TEST_METHOD( String_split_2_space_end )
		{
			const std::vector<std::string> splits = Utilities::String::split<std::vector<std::string>>( "Test asd " );
			Assert::AreEqual( 2ull, splits.size() );
			Assert::AreEqual<std::string>( "Test", splits[0] );
			Assert::AreEqual<std::string>( "asd", splits[1] );
		}
	};
}