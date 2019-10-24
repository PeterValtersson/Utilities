#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Utilities/Memory/Sofa.h>
#include <Utilities/GUID.h>
#include <filesystem>
namespace fs = std::filesystem;

namespace UtilitiesTests
{
	TEST_CLASS( MemoryTests ){
public:
	TEST_METHOD( Sofa_Create )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			double> s;
		s.add( "First", 1, 2.3 );
		Assert::AreEqual( 1ui64, s.size() );
		Assert::IsTrue( s.find( "First" ).has_value(), L"Not found" );
		Assert::AreEqual<Utilities::StringHash>( "First"_hash, s.peek<0>( 0 ).id );
		Assert::AreEqual( 1, s.peek<1>( 0 ) );
		Assert::AreEqual( 2.3, s.peek<2>( 0 ) );
	}

	TEST_METHOD( Sofa_Add_Many )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;
		
		for ( int i = 0; i < 100000; i++ )
		{
			s.add( i, i, i % 2 );
		}

		for ( int i = 0; i < 100000; i++ )
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>( )[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i%2, s.peek<2>()[i] );
		}
	}
	TEST_METHOD( Sofa_Add_Many_Resize_After )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		for ( int i = 0; i < 100000; i++ )
		{
			s.add( i, i, i % 2 );
		}
		s.Allocate( 200000 );
		for ( int i = 0; i < 100000; i++ )
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}
	}
	TEST_METHOD( Sofa_Add_Many_With_Resize )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		s.Allocate( 100000 );
		for ( int i = 0; i < 100000; i++ )
		{
			s.add( i, i, i % 2 );
		}

		for ( int i = 0; i < 100000; i++ )
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}
	}
	TEST_METHOD( SofA_ShrinkToFit )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		s.Allocate( 100000 );
		for ( int i = 0; i < 50000; i++ )
		{
			s.add( i, i, i % 2 );
		}

		s.shrink_to_fit();
		Assert::AreEqual( 50000ui64, s.MaxEntries() );
		for ( int i = 0; i < 50000; i++ )
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}

	}
	TEST_METHOD( SofA_File_Write_Read )
	{
		if ( fs::exists( "test.f" ) )
			fs::remove( "test.f" );
		{
			Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
				int,
				bool> s;

			s.Allocate( 100000 );
			for ( int i = 0; i < 100000; i++ )
			{
				s.add( i, i, i % 2 );
			}

			std::fstream f( "test.f", std::ios::binary | std::ios::out );
			Assert::IsTrue( f.is_open() );
			s.writeToFile( f );
		}
		{
			Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
				int,
				bool> s;

			std::fstream f( "test.f", std::ios::binary | std::ios::in );
			Assert::IsTrue( f.is_open() );
			s.readFromFile( f );
			Assert::AreEqual( 100000ui64, s.MaxEntries() );
			for ( int i = 0; i < 100000; i++ )
			{
				Assert::IsTrue( s.find( i ).has_value() );
				Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
				Assert::AreEqual( i, s.peek<1>()[i] );
				Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
			}
		}
	}
	};
}