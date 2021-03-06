#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Utilities/Memory/Sofa.h>
#include <Utilities/GUID.h>
#include <filesystem>
#include <Utilities/Memory/ChunkyAllocator.h>
#include <Utilities/Concurrent.h>
namespace fs = std::filesystem;



namespace UtilitiesTests
{
	int test( int& i )
	{
		i = 5;
		return i;
	}

	TEST_CLASS( SofA ){
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
	TEST_METHOD( set )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			double> s;
		auto i = s.add( "First", 1, 2.3 );
		s.set<1>( i, 1337 );
		Assert::AreEqual( 1337, s.get<1>( i ) );
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
		auto i = s.add( "One", 1, 1.1 );
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
	TEST_METHOD( Add_Many_Resize_After )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		for ( int i = 0; i < 100000; i++ )
		{
			s.add( i, i, i % 2 );
		}
		s.allocate( 200000 );
		for ( int i = 0; i < 100000; i++ )
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

		s.allocate( 100000 );
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
	TEST_METHOD( ShrinkToFit )
	{
		Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
			int,
			bool> s;

		s.allocate( 100000 );
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
	TEST_METHOD( File_Write_Read )
	{
		if ( fs::exists( "test.f" ) )
			fs::remove( "test.f" );
		{
			Utilities::Memory::SofA<Utilities::GUID, Utilities::GUID::Hasher,
				int,
				bool> s;

			s.allocate( 100000 );
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


	TEST_CLASS( AllocatorTests ){
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
	TEST_METHOD( peek )
	{
		Utilities::Memory::ChunkyAllocator allocator( 1000 );
		int v = 1337;
		auto h = allocator.allocate( sizeof( v ) );
		allocator.peek_data( h, [=]( const Utilities::Memory::ConstMemoryBlock m )
		{
			Assert::AreEqual<size_t>( sizeof( v ), m.used_size );
		} );
	}
	TEST_METHOD( use_data_write )
	{
		Utilities::Memory::ChunkyAllocator allocator( 1000 );
		int v = 1337;
		decltype( v ) v2 = 1338;
		auto h = allocator.allocate( sizeof( v ) );
		allocator.use_data( h, [=]( Utilities::Memory::MemoryBlock m )
		{
			m.write( v );
		} );
		allocator.peek_data( h, [=]( const Utilities::Memory::ConstMemoryBlock m )
		{
			Assert::AreEqual( v, m.peek<decltype( v )>() );
		} );

		allocator.use_data( h, [=]( Utilities::Memory::MemoryBlock m )
		{
			m.write( v2 );
		} );
		allocator.peek_data( h, [=]( const Utilities::Memory::ConstMemoryBlock m )
		{
			Assert::AreEqual( v2, m.peek<decltype( v2 )>() );
		} );
	}

	TEST_METHOD( use_data_multiple )
	{
		Utilities::Memory::ChunkyAllocator allocator( 1000 );
		auto h = allocator.allocate( sizeof( int ) );
		allocator.use_data( h, [=]( Utilities::Memory::MemoryBlock m )
		{
			m = 1337;
		} );

		auto h2 = allocator.allocate( sizeof( int ) );
		allocator.use_data( h2, [=]( Utilities::Memory::MemoryBlock m )
		{
			m = 1333;
		} );

		allocator.use_data( h, [=]( Utilities::Memory::MemoryBlock m )
		{
			Assert::AreEqual( 1337, m.peek<int>() );
		} );

		allocator.use_data( h2, [=]( Utilities::Memory::MemoryBlock m )
		{
			Assert::AreEqual( 1333, m.peek<int>() );
		} );
	}

	TEST_METHOD( use_data_write_larger )
	{
		Utilities::Memory::ChunkyAllocator allocator( 1000 );
		int v = 1337;
		long int v2 = 1338;
		auto h = allocator.allocate( sizeof( v ) );
		char* d = new char[Utilities::Memory::ChunkyAllocator::blocksize() + 1];
		allocator.use_data( h, [=]( Utilities::Memory::MemoryBlock m )
		{
			m.write( v );
		} );
		allocator.use_data( h, [=]( Utilities::Memory::MemoryBlock m )
		{

			m.write( d, Utilities::Memory::ChunkyAllocator::blocksize() + 1 );
			Assert::AreEqual<size_t>( Utilities::Memory::ChunkyAllocator::blocksize() + 1, m.get_used_size() );
			Assert::AreEqual<size_t>( Utilities::Memory::ChunkyAllocator::blocksize() * 2 - sizeof( size_t ) * 2, m.get_total_size() );
		} );
		allocator.peek_data( h, [=]( const Utilities::Memory::ConstMemoryBlock m )
		{
			Assert::AreEqual<size_t>( Utilities::Memory::ChunkyAllocator::blocksize() + 1, m.used_size );
			Assert::AreEqual<size_t>( Utilities::Memory::ChunkyAllocator::blocksize() * 2 - sizeof( size_t ) * 2, m.total_size );
		} );
		delete[] d;
	}

	TEST_METHOD( write_data )
	{
		Utilities::Memory::ChunkyAllocator allocator( 1000 );
		int v = 1337;
		auto h = allocator.allocate( sizeof( v ) );
		allocator.write_data( h, &v, sizeof( v ) );
		allocator.peek_data( h, [=]( Utilities::Memory::ConstMemoryBlock m )
		{
			Assert::AreEqual( v, m.peek<decltype( v )>() );
		} );
	}
	};
	TEST_CLASS( MemoryTests ){
public:
	TEST_METHOD( Memory_Literals )
	{
		Assert::AreEqual<size_t>( 1024, 1_kb, L"_kb wrong" );
		Assert::AreEqual<size_t>( 1024 * 1024, 1_mb, L"_kb wrong" );
		Assert::AreEqual<size_t>( 1024 * 1024 * 1024, 1_gb, L"_kb wrong" );
	}


	};

	class Test{
		Utilities::Concurrent<int> ci = 1;
	public:
		int op()const
		{
			return ci( []( const int& i )->const int&
			{
				return i;
			} );
		}
	};

	TEST_CLASS( Concurrent ){
	public:
		TEST_METHOD( No_Return )
		{
			Utilities::Concurrent<int> ci;
			ci( []( int& i )
			{
				i = 1;
			} );
		}
		TEST_METHOD( Return )
		{
			Utilities::Concurrent<int> ci;
			auto i = ci( []( int& i )
			{
				i = 1;
				return i;
			} );
			Assert::AreEqual( 1, i );

			Utilities::Concurrent<size_t> cs;
			auto s = cs( []( size_t& i )
			{
				i = 1337;
				return i;
			} );
			Assert::AreEqual<size_t>( 1337, s );
		}
		TEST_METHOD( Return_Function )
		{
			Utilities::Concurrent<int> ci;
			auto i = ci( &test );
			Assert::AreEqual( 5, i );
		}
		TEST_METHOD( Const )
		{
			Test t;
			Assert::AreEqual( 1, t.op() );
		}
	};



	TEST_CLASS( SofV ){
	public:
		TEST_METHOD( Add )
		{

			Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::string,
				std::vector<int>,
				std::vector<std::string>> a;
			std::vector<int> vi = { 1, -2, 3 };
			int i = 1;
			a.add( "Test", i, "test", { 1, -2, 3 }, { "Test2", "Test3" } );
			auto find = a.find( "Test" );
			Assert::IsTrue( find.has_value(), L"Could not find", LINE_INFO() );
			Assert::AreEqual( Utilities::GUID( "Test" ).id, a.peek<0>( *find ).id, L"Incorrect ID", LINE_INFO() );
			Assert::AreEqual( 1, a.peek<1>( *find ), L"Incorrect", LINE_INFO() );
			Assert::AreEqual<std::string>( "test", a.peek<2>( *find ), L"Incorrect", LINE_INFO() );
			auto& iv = a.peek<3>( *find );
			Assert::AreEqual<size_t>( 3, iv.size(), L"Incorrect", LINE_INFO() );
			Assert::AreEqual( 1, iv[0], L"Incorrect", LINE_INFO() );
			Assert::AreEqual( -2, iv[1], L"Incorrect", LINE_INFO() );
			Assert::AreEqual( 3, iv[2], L"Incorrect", LINE_INFO() );
			auto& sv = a.peek<4>( *find );
			Assert::AreEqual<size_t>( 2, sv.size(), L"Incorrect", LINE_INFO() );
			Assert::AreEqual<std::string>( "Test2", sv[0], L"Incorrect", LINE_INFO() );
			Assert::AreEqual<std::string>( "Test3", sv[1], L"Incorrect", LINE_INFO() );
		}
		TEST_METHOD( Add_Recursive_Sofa )
		{

			typedef Utilities::Memory::SofA<
				Utilities::GUID, Utilities::GUID::Hasher,
				int> sofa;
			typedef Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::vector<int>> sofv;

			Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::string,
				std::vector<int>,
				std::vector<std::string>,
				sofa,
				sofv> a;
			a.add( "Test" );
			a.get<1>( 0 ) = 1;
			a.get<2>( 0 ) = "test";
			a.get<3>( 0 ) = { 1, -2, 3 };
			a.get<4>( 0 ) = { "Test2", "Test3" };

			auto find = a.find( "Test" );
			Assert::IsTrue( find.has_value(), L"Could not find", LINE_INFO() );
			Assert::AreEqual( Utilities::GUID( "Test" ).id, a.peek<0>( *find ).id, L"Incorrect ID", LINE_INFO() );
			Assert::AreEqual( 1, a.peek<1>( *find ), L"Incorrect", LINE_INFO() );
			Assert::AreEqual<std::string>( "test", a.peek<2>( *find ), L"Incorrect", LINE_INFO() );
			auto& iv = a.peek<3>( *find );
			Assert::AreEqual<size_t>( 3, iv.size(), L"Incorrect", LINE_INFO() );
			Assert::AreEqual( 1, iv[0], L"Incorrect", LINE_INFO() );
			Assert::AreEqual( -2, iv[1], L"Incorrect", LINE_INFO() );
			Assert::AreEqual( 3, iv[2], L"Incorrect", LINE_INFO() );
			auto& sv = a.peek<4>( *find );
			Assert::AreEqual<size_t>( 2, sv.size(), L"Incorrect", LINE_INFO() );
			Assert::AreEqual<std::string>( "Test2", sv[0], L"Incorrect", LINE_INFO() );
			Assert::AreEqual<std::string>( "Test3", sv[1], L"Incorrect", LINE_INFO() );

			a.get<5>( 0 ).add( "Test", 1 );
			Assert::AreEqual( 1, a.peek<5>( 0 ).peek<1>( 0 ), L"Incorrect", LINE_INFO() );
			a.get<6>( 0 ).add( "Test", 1, { -1 } );
			Assert::AreEqual( 1, a.peek<6>( 0 ).peek<1>( 0 ), L"Incorrect", LINE_INFO() );
			auto& iv2 = a.peek<6>( 0 ).peek<2>( 0 );
			Assert::AreEqual( -1, iv2[0], L"Incorrect", LINE_INFO() );
		}
		TEST_METHOD( Erase )
		{
			typedef Utilities::Memory::SofA<
				Utilities::GUID, Utilities::GUID::Hasher,
				int> sofa;
			typedef Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::vector<int>> sofv;

			Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::string,
				std::vector<int>,
				std::vector<std::string>,
				sofa,
				sofv> a;
			a.add( "Test" );
			a.get<1>( 0 ) = 1;
			a.get<2>( 0 ) = "test";
			a.get<3>( 0 ) = { 1, -2, 3 };
			a.get<4>( 0 ) = { "Test2", "Test3" };
			a.get<5>( 0 ).add( "Test", 1 );
			a.get<6>( 0 ).add( "Test", 1, { -1 } );

			a.erase( "Test" );
			Assert::AreEqual<size_t>( 0, a.size(), L"Incorrect", LINE_INFO() );
			a.add( "Test" );
			auto& iv = a.peek<3>( 0 );

			Assert::AreEqual<size_t>( 0, iv.size(), L"Incorrect", LINE_INFO() );

			auto& sv = a.peek<4>( 0 );
			Assert::AreEqual<size_t>( 0, sv.size(), L"Incorrect", LINE_INFO() );

			Assert::AreEqual<size_t>( 0, a.peek<5>( 0 ).size(), L"Incorrect", LINE_INFO() );
			Assert::AreEqual<size_t>( 0, a.peek<6>( 0 ).size(), L"Incorrect", LINE_INFO() );
		}

		TEST_METHOD( Clear )
		{
			typedef Utilities::Memory::SofA<
				Utilities::GUID, Utilities::GUID::Hasher,
				int> sofa;
			typedef Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::vector<int>> sofv;

			Utilities::Memory::SofV<
				Utilities::GUID, Utilities::GUID::Hasher,
				int,
				std::string,
				std::vector<int>,
				std::vector<std::string>,
				sofa,
				sofv> a;
			a.add( "Test" );
			a.get<1>( 0 ) = 1;
			a.get<2>( 0 ) = "test";
			a.get<3>( 0 ) = { 1, -2, 3 };
			a.get<4>( 0 ) = { "Test2", "Test3" };
			a.get<5>( 0 ).add( "Test", 1 );
			a.get<6>( 0 ).add( "Test", 1, { -1 } );

			a.clear();
			Assert::AreEqual<size_t>( 0, a.size(), L"Incorrect", LINE_INFO() );
			a.add( "Test" );
			auto& iv = a.peek<3>( 0 );

			Assert::AreEqual<size_t>( 0, iv.size(), L"Incorrect", LINE_INFO() );

			auto& sv = a.peek<4>( 0 );
			Assert::AreEqual<size_t>( 0, sv.size(), L"Incorrect", LINE_INFO() );

			Assert::AreEqual<size_t>( 0, a.peek<5>( 0 ).size(), L"Incorrect", LINE_INFO() );
			Assert::AreEqual<size_t>( 0, a.peek<6>( 0 ).size(), L"Incorrect", LINE_INFO() );
		}
	};
}