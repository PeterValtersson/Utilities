#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Utilities/Memory/Sofa.h>
#include <Utilities/GUID.h>
#include <filesystem>
#include <Utilities/Memory/ChunkyAllocator.h>

namespace fs = std::filesystem;



namespace UtilitiesTests
{
	TEST_CLASS( SofA ) {
public:
	TEST_METHOD( Create_GUID )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			Utilities::GUID> s;
		s.add( "First", "test" );
	}
	
	TEST_METHOD( Create_CHAR_ARRAY )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			char[128]> s;
		s.add( "First", "test" );
		Assert::AreEqual( "test", s.get<1>( 0 ) );
	}
	TEST_METHOD( Create_WCHAR_ARRAY )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			wchar_t[128]> s;
		s.add( "First", L"test" );
		Assert::AreEqual( L"test", s.get<1>( 0 ) );
	}
	TEST_METHOD( Create )
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

		auto refs = s.get_entry( 0 );
		auto a = refs.get<0>();

		Assert::AreEqual<Utilities::StringHash>( "First"_hash, refs.get<0>().id );
		Assert::AreEqual( 1, refs.get<1>() );
		Assert::AreEqual( 2.3, refs.get<2>() );
	}
	TEST_METHOD( Create_3_Delete_First )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			double> s;
		s.add( "One", 1, 1.1 );
		s.add( "Two", 2, 2.2 );
		s.add( "Three", 3, 3.3 );

		Assert::AreEqual( 3ui64, s.size() );

		s.erase( "One" );
		Assert::AreEqual( 2ui64, s.size() );

		Assert::AreEqual<Utilities::StringHash>( "Three"_hash, s.peek<0>( 0 ).id );
		Assert::AreEqual( 3, s.peek<1>( 0 ) );
		Assert::AreEqual( 3.3, s.peek<2>( 0 ) );

		Assert::AreEqual<Utilities::StringHash>( "Two"_hash, s.peek<0>( 1 ).id );
		Assert::AreEqual( 2, s.peek<1>( 1 ) );
		Assert::AreEqual( 2.2, s.peek<2>( 1 ) );

	}
	TEST_METHOD( Create_3_Delete_Mid )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			double> s;
		s.add( "One", 1, 1.1 );
		s.add( "Two", 2, 2.2 );
		s.add( "Three", 3, 3.3 );

		Assert::AreEqual( 3ui64, s.size() );
		s.erase( "Two" );
		Assert::AreEqual( 2ui64, s.size() );

		Assert::AreEqual<Utilities::StringHash>( "One"_hash, s.peek<0>( 0 ).id );
		Assert::AreEqual( 1, s.peek<1>( 0 ) );
		Assert::AreEqual( 1.1, s.peek<2>( 0 ) );

		Assert::AreEqual<Utilities::StringHash>( "Three"_hash, s.peek<0>( 1 ).id );
		Assert::AreEqual( 3, s.peek<1>( 1 ) );
		Assert::AreEqual( 3.3, s.peek<2>( 1 ) );
	}
	TEST_METHOD( Create_3_Delete_Last )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			double> s;
		s.add( "One", 1, 1.1 );
		s.add( "Two", 2, 2.2 );
		s.add( "Three", 3, 3.3 );

		Assert::AreEqual( 3ui64, s.size() );
		s.erase( "Three" );
		Assert::AreEqual( 2ui64, s.size() );

		Assert::AreEqual<Utilities::StringHash>( "One"_hash, s.peek<0>( 0 ).id );
		Assert::AreEqual( 1, s.peek<1>( 0 ) );
		Assert::AreEqual( 1.1, s.peek<2>( 0 ) );

		Assert::AreEqual<Utilities::StringHash>( "Two"_hash, s.peek<0>( 1 ).id );
		Assert::AreEqual( 2, s.peek<1>( 1 ) );
		Assert::AreEqual( 2.2, s.peek<2>( 1 ) );
	}
	TEST_METHOD( Add_Many )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		for (int i = 0; i < 100000; i++)
		{
			s.add( i, i, i % 2 );
		}

		for (int i = 0; i < 100000; i++)
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}
	}
	TEST_METHOD( Add_Many_Resize_After )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		for (int i = 0; i < 100000; i++)
		{
			s.add( i, i, i % 2 );
		}
		s.Allocate( 200000 );
		for (int i = 0; i < 100000; i++)
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}
	}
	TEST_METHOD( Add_Many_With_Resize )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		s.Allocate( 100000 );
		for (int i = 0; i < 100000; i++)
		{
			s.add( i, i, i % 2 );
		}

		for (int i = 0; i < 100000; i++)
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}
	}
	TEST_METHOD( ShrinkToFit )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		s.Allocate( 100000 );
		for (int i = 0; i < 50000; i++)
		{
			s.add( i, i, i % 2 );
		}

		s.shrink_to_fit();
		Assert::AreEqual( 50000ui64, s.MaxEntries() );
		for (int i = 0; i < 50000; i++)
		{
			Assert::IsTrue( s.find( i ).has_value() );
			Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
			Assert::AreEqual( i, s.peek<1>()[i] );
			Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
		}

	}
	TEST_METHOD( File_Write_Read )
	{
		if (fs::exists( "test.f" ))
			fs::remove( "test.f" );
		{
			Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
				int,
				bool> s;

			s.Allocate( 100000 );
			for (int i = 0; i < 100000; i++)
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
			for (int i = 0; i < 100000; i++)
			{
				Assert::IsTrue( s.find( i ).has_value() );
				Assert::AreEqual<Utilities::StringHash>( i, s.peek<0>()[i].id );
				Assert::AreEqual( i, s.peek<1>()[i] );
				Assert::AreEqual<bool>( i % 2, s.peek<2>()[i] );
			}
		}
	}
	};


	TEST_CLASS( AllocatorTests ) {
	public:
		TEST_METHOD( allocate )
		{
			Utilities::Memory::ChunkyAllocator allocator( 1000 );

			auto m1 = allocator.allocate( 100 );
			auto m2 = allocator.allocate( 10 );
			auto m3 = allocator.allocate( 100 );
			auto m4 = allocator.allocate( 100 );
			Logger::WriteMessage( allocator.strOccupancy().c_str() );
			allocator.free( m3 );

			Logger::WriteMessage( allocator.strOccupancy().c_str() );
		}
	};
	TEST_CLASS( MemoryTests ) {
	public:
		TEST_METHOD( Memory_Literals )
		{
			Assert::AreEqual<size_t>( 1024, 1_kb, L"_kb wrong" );
			Assert::AreEqual<size_t>( 1024*1024, 1_mb, L"_kb wrong" );
			Assert::AreEqual<size_t>( 1024*1024*1024, 1_gb, L"_kb wrong" );
		}

	};
}