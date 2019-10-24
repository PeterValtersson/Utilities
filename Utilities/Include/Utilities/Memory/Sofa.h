#pragma once
#include <cstddef>
#include <tuple>
#include <array>
#include <unordered_map>
#include <list>
#include <Utilities/MonadicOptional.h>
#include <fstream>
#include <Utilities/FStreamHelpers.h>

namespace Utilities
{
	namespace Memory
	{

		template<class Key, class KeyHash, class... Types>
		class SofA{
		public:
			SofA( std::size_t size = 64 ) : data( nullptr ), numEntries( 0 ), maxEntries( size ), byteWidth( 0 )
			{
				for ( auto&& v : typeSizes )
					byteWidth += v;
				Allocate( maxEntries );
			}
			~SofA()
			{
				operator delete(data);
			}

			/*@brief Clear the entries, only sets numEntries to 0. Memory and data is intact*/
			inline void clear()
			{
				numEntries = 0;
				map.clear();
			}

			inline size_t MaxEntries()const
			{
				return maxEntries;
			}

			/*@brief Shrinks the block to exactly fit the number of entries.*/
			inline void shrink_to_fit()
			{
				Allocate( numEntries );
			}
			inline optional<std::size_t> find( const Key key )const
			{
				if ( auto const find = map.find( key ); find != map.end() )
					return { find->second };
				else
					return std::nullopt;
			}

			inline std::size_t size()const
			{
				return numEntries;
			};

			void add( const Key key, const Types... args )
			{
				if ( numEntries + 1 > maxEntries )
					Allocate( maxEntries * 2 );
				auto index = map[key] = numEntries++;
				const auto tpl = std::make_tuple( key, args... );
				setValue<0, Key, Types...>( typePointers, tpl, index );
			}

			std::size_t add( const Key key )
			{
				if ( auto entry = find( key ); entry.has_value() )
					return *entry;

				if ( numEntries + 1 > maxEntries )
					Allocate( maxEntries * 2 );
				std::get<0>( typePointers )[numEntries] = key;
				std::size_t ret = map[key] = numEntries++;
				return ret;
			}

			template<std::size_t N, class type>
			inline void set( std::size_t index, const type&& t )
			{
				std::get<N>( typePointers )[index] = t;
			}
			template<std::size_t N, class type>
			inline void set( std::size_t index, const type& t )
			{
				std::get<N>( typePointers )[index] = t;
			}

			template<std::size_t N>
			inline auto& get( std::size_t index )
			{
				return std::get<N>( typePointers )[index];
			}
			template<std::size_t N>
			inline const auto& peek( std::size_t index )const
			{
				return std::get<N>( typePointers )[index];
			}
			template<std::size_t N>
			inline auto& get()
			{
				return std::get<N>( typePointers );
			}
			template<std::size_t N>
			inline const auto& peek()const
			{
				return std::get<N>( typePointers );
			}

			bool erase( const Key key )
			{
				if ( const auto find = map.find( key ); find != map.end() )
				{
					erase( find->second );
					return true;
				}
				return false;
			}

			void erase( std::size_t at )
			{
				if ( at >= numEntries )
				{
				}
				auto last = --numEntries;

				auto at_key = std::get<0>( typePointers )[at];
				auto last_key = std::get<0>( typePointers )[last];

				copyValue<0, Key, Types...>( typePointers, at, last );

				map[last_key] = at;
				map.erase( at_key );
			}

			std::size_t GetMemoryUsage()const
			{
				return byteWidth * maxEntries;
			}
			std::size_t GetWriteToFileSize()const
			{
				return GetMemoryUsage() + sizeof( version ) + sizeof( byteWidth ) + sizeof( std::size_t ) + sizeof( numEntries );
			}
			void Allocate( std::size_t newSize )
			{
				std::size_t newmaxEntries = newSize;
				void* newData = operator new(newmaxEntries * byteWidth);
				type newTypePointers;

				std::get<0>( newTypePointers ) = (Key*)newData;
				setupPointers<1, Key, Types...>( newTypePointers, newmaxEntries );

				memcpyTuple<0, Key, Types...>( newTypePointers, typePointers );

				operator delete(data);
				typePointers = newTypePointers;
				maxEntries = newmaxEntries;
				data = newData;
			}

			void Reinit( std::size_t newnumEntries )
			{
				numEntries = newnumEntries;
				map.clear();
				for ( std::size_t i = 0; i < numEntries; i++ )
					map[std::get<0>( typePointers )[i]] = i;
			}


			int readFromFile( std::istream& file )
			{
				Binary_Stream::read( file, version );

				std::size_t readByteWidth;
				Binary_Stream::read( file, readByteWidth );
				if ( this->byteWidth != readByteWidth )
					return -1;

				std::size_t totalSize;
				Binary_Stream::read( file, totalSize );

				if ( totalSize % byteWidth != 0 )
					return -2;

				maxEntries = totalSize / byteWidth;
				Allocate( maxEntries );

				std::size_t numEntires;
				Binary_Stream::read( file, numEntires );

				Binary_Stream::read( file, data, totalSize );

				Reinit( numEntires );

				return 0;
			}

			void writeToFile( std::ostream& file )
			{
				//shrink_to_fit();
				auto totalSize = GetMemoryUsage();
				Binary_Stream::write( file, version );
				Binary_Stream::write( file, byteWidth );
				Binary_Stream::write( file, totalSize );
				Binary_Stream::write( file, numEntries );
				Binary_Stream::write( file, data, totalSize );
			}

		private:
				// Parameter pack pointer converter
			template<class T> struct make_ptr_t{
				typedef T* type;
			};
			template<class T> struct make_ptr_t<T*>{
				typedef T* type;
			};


			uint32_t version = 000001;
			void* data;
			std::size_t numEntries;
			std::size_t maxEntries;
			std::size_t byteWidth;

			const std::array<std::size_t, sizeof...(Types) + 1> typeSizes{ sizeof( Key ), sizeof( Types )... };
			typedef std::tuple<typename make_ptr_t<Key>::type, typename make_ptr_t<Types>::type...> type;
			type typePointers;

			std::unordered_map<Key, std::size_t, KeyHash> map;


			

			// Goes through the tuple and sets up the pointers for each attribute in the one allocation block. ( The stop recursion template)
			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				setupPointers( std::tuple<typename make_ptr_t<Types>::type...>& t, size_t maxEntries )
			{}

			// Goes through the tuple and sets up the pointers for each attribute in the one allocation block. 
			/*
			// Setup the new pointers
			newData.data = operator new(newSize);
			newData.key = (int*)newData.data;
			newData.v1 = (int*)(newData.key + newData.maxEntries);
			newData.v2 = (bool*)(newData.v1 + newData.maxEntries);
			newData.v3 = (char*)(newData.v2 + newData.maxEntries);

			|0|1|2|3|4|5|6|7|
			 ^		 ^
			 |		 |
			 int *	 bool*
			*/
			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				setupPointers( std::tuple<typename make_ptr_t<Types>::type...> & t, size_t maxEntries )
			{
				std::get<I>( t ) = reinterpret_cast<typename std::tuple_element<I, std::tuple<typename make_ptr_t<Types>::type...>>::type>(std::get<I - 1>( t ) + maxEntries);
				setupPointers<I + 1, Types...>( t, maxEntries );
			}

			// 'Takes one tuple of values and inserts them into the tuple holding arrays in the correct location.( The stop recursion template)
			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				setValue( std::tuple<typename make_ptr_t<Types>::type...>& tp, const std::tuple<Types...>& t, std::size_t index )
			{}

			// Takes one tuple of values and inserts them into the tuple holding arrays in the correct location.
			/*
			|tp		|0		|1		|2		|3		|4		|5		|6	|
			-------------------------------------------------------------
			|int*	|123	|321	|1		|2		|2		|4		|8	|
			|bool*	|true	|false	|true	|true	|true	|false	|true


			| t		| Insert into slot 3
			-----
			| 1337	|
			| false	|

			|tp		|0		|1		|2		|3		|4		|5		|6	|
			-------------------------------------------------------------
			|int*	|123	|321	|1		| 1337	|2		|4		|8	|
			|bool*	|true	|false	|true	| false	|true	|false	|true
			*/
			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				setValue( std::tuple<typename make_ptr_t<Types>::type...> & tp, const std::tuple<Types...> & t, std::size_t index )
			{
				std::get<I>( tp )[index] = std::get<I>( t );
				setValue<I + 1, Types...>( tp, t, index );
			}

			// Copies an entry from one location to another ( The stop recursion template)
			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				copyValue( std::tuple<typename make_ptr_t<Types>::type...>& tp, std::size_t to, std::size_t from )
			{}
			// Copies an entry from one location to another
			/*
			|tp		|0		|1		|2		|3		|4		|5		|6	|
			-------------------------------------------------------------
			|int*	|123	|321	|1		|2		|2		|4		|8	|
			|bool*	|true	|false	|true	|true	|true	|false	|true

			Copy slot 6 to slot 3
			|tp		|0		|1		|2		|3		|4		|5		|6	|
			-------------------------------------------------------------
			|int*	|123	|321	|1		|8		|2		|4		|8	|
			|bool*	|true	|false	|true	|true	|true	|false	|true
			*/

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				copyValue( std::tuple<typename make_ptr_t<Types>::type...> & tp, std::size_t to, std::size_t from )
			{
				std::get<I>( tp )[to] = std::get<I>( tp )[from];
				copyValue<I + 1, Types...>( tp, to, from );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				memcpyTuple( std::tuple<typename make_ptr_t<Types>::type...>& t1, std::tuple<typename make_ptr_t<Types>::type...>& t2 )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				memcpyTuple( std::tuple<typename make_ptr_t<Types>::type...> & t1, std::tuple<typename make_ptr_t<Types>::type...> & t2 )
			{
				memcpy( std::get<I>( t1 ), std::get<I>( t2 ), typeSizes[I] * numEntries );
				memcpyTuple<I + 1, Types...>( t1, t2 );
			}

		};

		template<class Key, class KeyHash, class... Types>
		class SofV{
		public:
			SofV( std::size_t size = 64 ) : numEntries( 0 ), maxEntries( size )
			{
				Allocate( maxEntries );
			}

			~SofV()
			{

			}

			void clear()
			{
				numEntries = 0;
			}

			inline void shrink_to_fit()
			{
				Allocate( numEntries );
			}
			inline optional<std::size_t> find( const Key key )const
			{
				if ( auto const find = map.find( key ); find != map.end() )
					return { find->second };
				else
					return std::nullopt;
			}

			inline std::size_t size()const
			{
				return numEntries;
			};

			std::size_t add( const Key key )
			{
				if ( numEntries + 1 > maxEntries )
					Allocate( maxEntries * 2 );
				auto index = numEntries++;
				std::get<0>( tvec )[index] = key;
				return map[key] = index;
			}

			void add( const Key key, const Types... args )
			{
				if ( numEntries + 1 > maxEntries )
					Allocate( maxEntries * 2 );
				auto index = map[key] = numEntries++;
				const auto tpl = std::make_tuple( key, args... );
				setValue<0, Key, Types...>( tvec, tpl, index );
			}

			template<std::size_t N, class type>
			inline void set( std::size_t index, type& t )
			{
				std::get<N>( tvec )[index] = t;
			}
			template<std::size_t N, class type>
			inline void set( std::size_t index, type&& t )
			{
				std::get<N>( tvec )[index] = t;
			}

			template<std::size_t N>
			inline auto& get( std::size_t index )
			{
				return std::get<N>( tvec )[index];
			}
			template<std::size_t N>
			inline const auto& getConst( std::size_t index )const
			{
				return std::get<N>( tvec )[index];
			}


			template<std::size_t N>
			inline auto get()
			{
				return std::get<N>( tvec ).data();
			}

			bool erase( const Key key )
			{
				if ( const auto find = map.find( key ); find != map.end() )
				{
					erase( find->second );
					return true;
				}
				return false;
			}


			void erase( std::size_t at )
			{
				auto last = --numEntries;

				auto at_key = std::get<0>( tvec )[at];
				auto last_key = std::get<0>( tvec )[last];

				copyValue<0, Key, Types...>( tvec, at, last );

				map[last_key] = at;
				map.erase( at_key );
			}


		private:
			std::size_t numEntries;
			std::size_t maxEntries;

			std::tuple<std::vector<Key>, std::vector<Types>...> tvec;
			std::unordered_map<Key, std::size_t, KeyHash> map;

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				setValue( std::tuple<std::vector<Types>...>& tuple, const std::tuple<Types...>& t, std::size_t index )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				setValue( std::tuple<std::vector<Types>...> & tuple, const std::tuple<Types...> & t, std::size_t index )
			{
				std::get<I>( tuple )[index] = std::get<I>( t );
				setValue<I + 1, Types...>( tuple, t, index );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				copyValue( std::tuple<std::vector<Types>...>& tuple, std::size_t to, std::size_t from )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				copyValue( std::tuple<std::vector<Types>...> & tuple, std::size_t to, std::size_t from )
			{
				std::get<I>( tuple )[to] = std::move( std::get<I>( tuple )[from] );
				copyValue<I + 1, Types...>( tuple, to, from );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				resizeVectorsInTuple( std::tuple<std::vector<Types>...>& tuple, std::size_t newSize )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				resizeVectorsInTuple( std::tuple<std::vector<Types>...> & tuple, std::size_t newSize )
			{
				std::get<I>( tuple ).resize( newSize );
				resizeVectorsInTuple<I + 1, Types...>( tuple, newSize );
			}

			void Allocate( std::size_t newSize )
			{
				resizeVectorsInTuple( tvec, newSize );
				maxEntries = newSize;
			}
		};

	}
}












