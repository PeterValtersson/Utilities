#ifndef _UTILITIES_MEMORYBLOCK_H_
#define _UTILITIES_MEMORYBLOCK_H_
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
		private:
			void* const data;

		};



	}
}

#endif