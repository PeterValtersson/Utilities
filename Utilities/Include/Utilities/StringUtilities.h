#ifndef _UTILZ_STRING_READ_WRITE_H_
#define _UTILZ_STRING_READ_WRITE_H_
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <codecvt>
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
namespace Utilities
{
	namespace String
	{
		template<class _Container = std::vector<std::string>>
		void split( const std::string_view str, _Container& container, std::string_view token = " " )
		{
			std::size_t current, offset = 0;
			for ( current = str.find( token ); current != std::string::npos; current = str.find( token, offset ) )
			{
				if ( current != offset )
					container.insert( container.end(), std::string( str.substr( offset, current - offset ) ) );
				offset = current + 1;
			}
			if ( offset != str.size() )
				container.insert( container.end(), std::string( str.substr( offset, current - offset ) ) );
		}

		template<class _Container = std::vector<std::string>>
		inline const _Container split( const std::string_view str, std::string_view token = " " )
		{
			_Container container;
			split( str, container, token );
			return container;
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

		inline std::wstring utf8_2_utf16( std::string_view str )
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
			return conv.from_bytes( str.data() );
		}

		inline std::string utf16_2_utf8( std::wstring_view str )
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
			return conv.to_bytes( str.data() );
		}
		//inline std::wstring string_2_wstring( std::string_view str )
		//{
		//	return utf8_2_utf16( str );
		//}
		//inline std::string wstring_2_string( std::wstring_view str )
		//{
		//	Multi
		//	return utf16_2_utf8( str );
		//}
	}
}

#endif