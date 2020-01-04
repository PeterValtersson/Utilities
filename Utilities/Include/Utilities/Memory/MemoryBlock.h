#ifndef _UTILITIES_MEMORYBLOCK_H_
#define _UTILITIES_MEMORYBLOCK_H_
#include <fstream>
#include <functional>
#include <Utilities/FStreamHelpers.h>

namespace Utilities
{
	namespace Memory
	{
		class MemoryBlock {
		public:
			MemoryBlock( void* const data, size_t& used, const size_t total, const std::function<MemoryBlock( size_t )>& realloc_callback ) : data( data ), used_size( used ), total_size( total ), realloc_callback( realloc_callback )
			{}
			inline void realloc( const MemoryBlock& o )
			{
				new ( this ) MemoryBlock( o.data, o.used_size, o.total_size, o.realloc_callback );
			}
			template<class T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
			inline void write( const T& v )
			{
				write( &v, sizeof( T ) );
			}

			void write( const void* const data, const size_t size )
			{
				if ( total_size < size )
					realloc( realloc_callback( size ) );
				used_size = size <= total_size ? size : total_size;
				memcpy( this->data, data, used_size );
			}

			template<class T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
			inline void operator=( const T& v )
			{
				write( v );
			}
			inline void write_to_stream( std::ostream& stream )const
			{
				stream.write( (char*)data, used_size );
			}
			inline void read_from_stream( std::istream& stream )
			{
				stream.read( (char*)data, used_size );
			}
			template<class T>
			inline const auto peek()const
			{
				return *(T* const)data;
			}
			inline const size_t get_used_size()const
			{
				return used_size;
			}
			inline const size_t get_total_size()const
			{
				return total_size;
			}
		private:
			void* const data;
			size_t& used_size;
			const size_t total_size;
			std::function<MemoryBlock( size_t )> realloc_callback;
		};

		class ConstMemoryBlock {
		public:
			ConstMemoryBlock( const void* const data, const size_t used, const size_t total ) : data( data ), used_size( used ), total_size( total )
			{}
			const size_t used_size;
			const size_t total_size;

			inline void write_to_stream( std::ostream& stream )const
			{
				stream.write( (char*)data, used_size );
			}
			template<class T>
			inline const T& peek()const
			{
				return *( (const T* const)data );
			}
			inline const char* const get_char()const
			{
				return (const char* const)data;
			}
			std::istream get_stream()const
			{
				return Binary_Stream::create_stream_from_data<std::istream>( data, total_size );
			}
		private:
			const void* const data;

		};

	}
}

#endif