#ifndef _UTILITIES_MEMORYBLOCK_H_
#define _UTILITIES_MEMORYBLOCK_H_
#include <fstream>
namespace Utilities
{
	namespace Memory
	{
		class MemoryBlock{
		public:
			MemoryBlock( void* const data, size_t& used, const size_t total ) : data( data ), used_size( used ), total_size( total )
			{}
			size_t& used_size;
			const size_t total_size;

			template<class T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
			void write( const T& v )const
			{
				used_size = sizeof( T ) <= total_size ? sizeof( T ) : total_size;
				memcpy( data, &v, used_size );
			}
			template<class T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
			inline void operator=( const T& v )const
			{
				write( v );
			}
			void write_to_stream( std::ostream& stream )const
			{
				stream.write( (char*)data, used_size );
			}
			void read_from_stream( std::istream& stream )const
			{
				stream.read( (char*)data, used_size );
			}
			template<class T>
			const T& peek()const
			{
				return *(T*)data;
			}
		private:
			void* const data;

		};

		class ConstMemoryBlock {
		public:
			ConstMemoryBlock( const void* const data, const size_t used, const size_t total ) : data( data ), used_size( used ), total_size( total )
			{}
			const size_t used_size;
			const size_t total_size;

			void write_to_stream( std::ostream& stream )const
			{
				stream.write( (char*)data, used_size );
			}
			template<class T>
			const T& peek()const
			{
				return *(T*)data;
			}
		private:
			const void* const data;

		};

	}
}

#endif