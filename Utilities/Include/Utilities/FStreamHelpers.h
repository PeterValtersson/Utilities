#ifndef _UTILITIES_FSTREAM_HELPERS_H_
#define _UTILITIES_FSTREAM_HELPERS_H_
#include <fstream>

namespace Utilities
{
	namespace Binary_Stream
	{
		template <class _Stream, class _StringType = char, class _Size_Type = uint32_t>
		void write_string_w_size( _Stream& out_stream, std::basic_string_view<_StringType> str )
		{
			_Size_Type size = static_cast<_Size_Type>(str.size());
			out_stream.write( (char*)&size, sizeof( size ) );
			out_stream.write( str.data(), size * sizeof( _StringType ) );
		}

		template <class _Stream, class _StringType = char, class _Size_Type = uint32_t>
		void read_string_w_size( _Stream& in_stream, std::basic_string<_StringType>& str )
		{
			_Size_Type size = 0;
			in_stream.read( (char*)&size, sizeof( size ) );
			str.resize( size );
			in_stream.read( str.data(), size * sizeof( _StringType ) );
		}

		template <class _Stream, class _StringType = char, class _Size_Type = uint32_t, size_t _BufferSize = 256>
		const std::basic_string<_StringType> read_string_w_size( _Stream& in_stream )
		{
			_Size_Type size = 0;
			in_stream.read( (char*)&size, sizeof( size ) );

			_StringType buffer[_BufferSize];
			in_stream.read( buffer, sizeof( buffer ) );
			return std::basic_string<_StringType>( buffer, size );
		}

		template <class _Stream, class _StringType = char, class _Size_Type = uint32_t>
		inline std::ostream& operator<<( _Stream& out_stream, std::basic_string_view<_StringType> str )
		{
			write_string_w_size< _Stream, _StringType, _Size_Type>( out_stream, str );
			return out_stream;
		}
		template <class _Stream, class _StringType = char, class _Size_Type = uint32_t>
		inline std::istream& operator>>( _Stream& in_stream, std::string& str )
		{
			read_string_w_size<_Stream, _StringType, _Size_Type>( in_stream, str );
			return in_stream;
		}

		template <class ST, class T>
		inline void write( ST& file, const T& value )
		{
			file.write( (char*)&value, sizeof( T ) );
		}
		template <class ST, class T>
		inline void read( ST& file, T& value )
		{
			file.read( (char*)&value, sizeof( T ) );
		}

		template <class ST, class T>
		inline void write( ST& file, const T * const value, const size_t size )
		{
			file.write( (char*)value, size );
		}
		template <class ST, class T>
		inline void read( ST& file, T * const value, const size_t size )
		{
			file.read( (char*)value, size );
		}


	}

}

#endif