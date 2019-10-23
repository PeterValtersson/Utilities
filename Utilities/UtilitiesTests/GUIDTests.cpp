#include "stdafx.h"
#include "CppUnitTest.h"
#include <Utilities/GUID.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

constexpr size_t num_exec = 2000000;
namespace UtilitiesTests
{
	TEST_CLASS( GUIDTests ){
public:
	TEST_METHOD( Performance_std )
	{
		std::string str = "asasdasd";
		uint32_t hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = uint32_t( std::hash<std::string>{}(str) );
	}
	TEST_METHOD( Performance_GUIDRuntime )
	{
		std::string str = "asasdasd";
		Utilities::GUID hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = str;
	}
	TEST_METHOD( Performance_GUIDCompiletime )
	{
		std::string str = "asasdasd";
		Utilities::GUID hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = Utilities::GUID( "asasdasd" );
	}
	TEST_METHOD( Performance_hashString )
	{
		std::string str = "asasdasd";
		Utilities::StringHash hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = Utilities::hashString( "asasdasd" );
	}
	TEST_METHOD( Performance_hash )
	{
		std::string str = "asasdasd";
		Utilities::StringHash hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = "asasdasd"_hash;
	}
	TEST_METHOD( Performance_EnsureComplileTime_hash )
	{
		std::string str = "asasdasd";
		Utilities::StringHash hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = Utilities::EnsureComplileTime<"asasdasd"_hash>::value;
	}
	TEST_METHOD( Performance_EnsureComplileTime_hashString )
	{
		std::string str = "asasdasd";
		Utilities::StringHash hash = 0;
		for ( int i = 0; i < num_exec; i++ )
			hash = Utilities::EnsureComplileTime<Utilities::hashString( "asasdasd" )>::value;
	}
	TEST_METHOD( Constexpr_String )
	{
		Utilities::ConstexprString<4> asd( "asd" );
		Assert::AreEqual( 3ui64, asd.size() );
		Assert::AreEqual( 3712466179u, Utilities::EnsureComplileTime<"asd"_hash>::value );
	}

	};
}