#ifndef _UTILZ_STRING_READ_WRITE_H_
#define _UTILZ_STRING_READ_WRITE_H_
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>

namespace Utilities
{
	namespace String
	{
		template<class _Container = std::vector<std::string>>
		const _Container split( const std::string_view str, std::string_view token = " " )
		{
			_Container container;
			size_t offset = 0;
			for ( auto f = str.find( token, offset ); f != std::string::npos; f = str.find( " ", offset ) )
			{
				if ( f != offset )
					container.insert( container.end(), std::string( str.substr( offset, f - offset ) ) );
				offset = f + 1;
			}
			return container;
		}
		inline void string_binary_write( std::ostream& out, const std::string& str )
		{
			uint32_t size = static_cast<uint32_t>(str.size());
			out.write( (char*)&size, sizeof( size ) );
			out.write( str.c_str(), size );
		}
		inline void string_binary_read( std::istream& in, std::string& str )
		{
			uint32_t size = 0;
			in.read( (char*)&size, sizeof( size ) );
			char buffer[512];
			in.read( buffer, size );
			str = std::string( buffer, size );
		}
		namespace Binary_Operators
		{
			inline std::ostream& operator<<( std::ostream& out, const std::string& str )
			{
				string_binary_write( out, str );
				return out;
			}
			inline std::istream& operator>>( std::istream& in, std::string& str )
			{
				string_binary_read( in, str );
				return in;
			}
		}
		inline std::string tabs( int numTabs, std::string_view c = "\t" )
		{
			std::string tabs;
			for ( int tab = 0; tab < numTabs; tab++ )
				tabs += c;
			return tabs;
		}
		inline std::string replaceAll( std::string str, const std::string& from, const std::string& to )
		{
			size_t start_pos = 0;
			while ( (start_pos = str.find( from, start_pos )) != std::string::npos )
			{
				str.replace( start_pos, from.length(), to );
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return str;
		}
		inline std::string charToHex( unsigned char c )
		{
			std::stringstream ss;
			ss << std::uppercase << std::setw( 2 ) << std::setfill( '0' ) << std::hex;
			ss << +c;
			return ss.str();
		}


		// Removed all folders from path execept the last two
		constexpr const char* const get_path_split_last2( const char* const path )
		{
			const char* f2 = nullptr;
			const char* f = nullptr;
			const char* c = path;
			while ( *c != '\0' )
			{
				if ( *c == '/' || *c == '\\' )
				{
					f2 = f;
					f = c;
				}

				++c;
			}
			if ( !f2 )
				return path;
			else if ( f2 )
				return ++f2;
			else
				return ++f;
		}

		// Get Function/Method name with only closest namespace
		constexpr const char* const get_function_name_closest_namespace( const char* const function_name )
		{
			const char* f2 = nullptr;
			const char* f = nullptr;
			const char* c = function_name;
			while ( *c != '\0' )
			{
				if ( *c == ':' )
				{
					++c;
					f2 = f;
					f = c;

				}

				++c;
			}
			if ( !f2 )
				return function_name;
			else if ( f2 )
				return ++f2;
			else
				return ++f;
		}
	}
}

#endif