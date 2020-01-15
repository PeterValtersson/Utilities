#pragma once
#include <cstddef>
#include <tuple>
#include <array>
#include <unordered_map>
#include <list>
#include <Utilities/MonadicOptional.h>
#include <fstream>
#include <Utilities/FStreamHelpers.h>
#include <string_view>
#include <array>
#include <numeric>

namespace Utilities
{
	namespace Memory
	{
		template<class T>
		struct make_ptr_t{
			typedef T* type;
		};
		template<class T>
		struct make_ptr_t<T*>{
			typedef T* type;
		};

		template<typename T, std::size_t N>
		constexpr T compile_time_accumulator( std::array<T, N> const& A, int const i = 0 )
		{
			return ( i < N ) ? A[i] + compile_time_accumulator( A, i + 1 ) : T( 0 );
		}

		template<typename... Types>
		constexpr auto size_of_prm_pack()
		{
			return compile_time_accumulator( std::array<std::size_t, sizeof...( Types )>{ sizeof( Types )... } );
		}

		template <class Key, class KeyHash, bool Enable, typename... Types>
		class SofA_Imp;


		template <class Key, class KeyHash, typename... Types>
		class SofA_Imp<Key, KeyHash, true, Types...>{
		public:
			constexpr static size_t num_types = sizeof...( Types );
			constexpr static size_t num_types_p_key = sizeof...( Types ) + 1;
			template<class T>
			struct char_conv{
				typedef T type;
			};
			template<size_t N>
			struct char_conv<char[N]>{
				typedef std::string_view type;
			};
			template<size_t N>
			struct char_conv<wchar_t[N]>{
				typedef std::wstring_view type;
			};
			typedef std::tuple<const Key, const typename char_conv<Types>::type...> Types_Tuple;
			typedef std::tuple<typename make_ptr_t<Key>::type, typename make_ptr_t<Types>::type...> type_ptrs_p_key;

			SofA_Imp( std::size_t size = 64 ) : data( nullptr ), numEntries( 0 ), maxEntries( size ), byteWidth( 0 )
			{
				for ( auto&& v : typeSizes )
					byteWidth += v;
				allocate( maxEntries );
			}

			SofA_Imp( SofA_Imp&& other )
			{
				data = other.data; other.data = nullptr;
				numEntries = other.numEntries;
				maxEntries = other.maxEntries;
				byteWidth = other.byteWidth;

				typePointers = std::move( other.typePointers );

				map = std::move( other.map );

			}
			void operator=( SofA_Imp&& other )
			{
				data = other.data; other.data = nullptr;
				numEntries = other.numEntries;
				maxEntries = other.maxEntries;
				byteWidth = other.byteWidth;

				typePointers = std::move( other.typePointers );

				map = std::move( other.map );
			}
			~SofA_Imp()
			{
				operator delete( data );
			}

			/*@brief Clear the entries, only sets numEntries to 0. Memory and data is intact*/
			inline void clear()
			{
				numEntries = 0;
				map.clear();
			}

			inline size_t MaxEntries()const noexcept
			{
				return maxEntries;
			}

			/*@brief Shrinks the block to exactly fit the number of entries.*/
			inline void shrink_to_fit() noexcept
			{
				allocate( numEntries );
			}
			inline optional<std::size_t> find( const Key key )const noexcept
			{
				if ( auto const find = map.find( key ); find != map.end() )
					return { find->second };
				else
					return std::nullopt;
			}

			inline std::size_t size()const noexcept
			{
				return numEntries;
			};

			std::size_t add( const Key key, const typename char_conv<Types>::type... args )noexcept
			{
				if ( numEntries + 1 > maxEntries )
					allocate( maxEntries * 2 );
				auto index = map[key] = numEntries++;
				setValue( typePointers, std::make_tuple( key, args... ), index );
				return index;
			}

			std::size_t add( const Key key )noexcept
			{
				if ( auto entry = find( key ); entry.has_value() )
					return *entry;

				if ( numEntries + 1 > maxEntries )
					allocate( maxEntries * 2 );
				std::get<0>( typePointers )[numEntries] = key;
				std::size_t ret = map[key] = numEntries++;
				return ret;
			}

			template<std::size_t N>
			inline void set( std::size_t index, const typename std::tuple_element<N, Types_Tuple>::type&& t )noexcept
			{
				std::get<N>( typePointers )[index] = t;
			}
			template<std::size_t N>
			inline void set( std::size_t index, const typename std::tuple_element<N, Types_Tuple>::type& t )noexcept
			{
				std::get<N>( typePointers )[index] = t;
			}



			template<std::size_t N>
			inline auto& get( std::size_t index ) noexcept
			{
				return std::get<N>( typePointers )[index];
			}
			template<std::size_t N>
			inline const auto& peek( std::size_t index )const noexcept
			{
				return std::get<N>( typePointers )[index];
			}
			template<std::size_t N>
			constexpr inline auto& get()noexcept
			{
				return std::get<N>( typePointers );
			}
			template<std::size_t N>
			constexpr inline const auto& peek()const noexcept
			{
				return std::get<N>( typePointers );
			}

			bool erase( const Key key )noexcept
			{
				if ( const auto find = map.find( key ); find != map.end() )
				{
					erase( find->second );
					return true;
				}
				return false;
			}

			void erase( std::size_t at )noexcept
			{
				if ( at >= numEntries )
					return;
				auto last = --numEntries;

				auto at_key = std::get<0>( typePointers )[at];
				auto last_key = std::get<0>( typePointers )[last];

				copyValue( typePointers, at, last );

				map[last_key] = at;
				map.erase( at_key );
			}

			std::size_t get_memory_usage()const noexcept
			{
				return byteWidth * maxEntries;
			}
			std::size_t GetWriteToFileSize()const noexcept
			{
				return get_memory_usage() + sizeof( version ) + sizeof( byteWidth ) + sizeof( std::size_t ) + sizeof( numEntries );
			}
			void allocate( std::size_t newSize )noexcept
			{
				std::size_t newmaxEntries = newSize;
				void* newData = operator new( newmaxEntries * byteWidth );
				type_ptrs_p_key newTypePointers;

				std::get<0>( newTypePointers ) = ( Key* )newData;
				setupPointers<1>( newTypePointers, newmaxEntries );

				memcpyTuple( newTypePointers, typePointers );

				operator delete( data );
				typePointers = newTypePointers;
				maxEntries = newmaxEntries;
				data = newData;
			}

			void reinit( std::size_t newnumEntries )noexcept
			{
				numEntries = newnumEntries;
				map.clear();
				for ( std::size_t i = 0; i < numEntries; i++ )
					map[std::get<0>( typePointers )[i]] = i;
			}


			int readFromFile( std::istream& file )noexcept
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
				allocate( maxEntries );

				std::size_t numEntires;
				Binary_Stream::read( file, numEntires );

				Binary_Stream::read( file, data, totalSize );

				reinit( numEntires );

				return 0;
			}

			void writeToFile( std::ostream& file )const noexcept
			{
				//shrink_to_fit();
				auto totalSize = get_memory_usage();
				Binary_Stream::write( file, version );
				Binary_Stream::write( file, byteWidth );
				Binary_Stream::write( file, totalSize );
				Binary_Stream::write( file, numEntries );
				Binary_Stream::write( file, data, totalSize );
			}

			class Entry_Ref{
			public:
				friend class SofA_Imp;
				template<std::size_t N>
				inline auto& get()noexcept
				{
					return *std::get<N>( ptrs );
				}
				bool operator==( const Entry_Ref& r )const noexcept
				{
					return equal( r.ptrs );
				}
			private:
				Entry_Ref( type_ptrs_p_key& tp, size_t index )noexcept
				{
					init_test_ptrs<0>( ptrs, tp, index );
				}

				type_ptrs_p_key ptrs;

				template<std::size_t I = 0>
				inline typename std::enable_if<I == num_types_p_key, void>::type
					init_test_ptrs( type_ptrs_p_key& ptrs, type_ptrs_p_key& tp, std::size_t from )
				{}
				template<std::size_t I = 0>
				inline typename std::enable_if < I < num_types_p_key, void>::type
					init_test_ptrs( type_ptrs_p_key & ptrs, type_ptrs_p_key & tp, std::size_t from )
				{
					std::get<I>( ptrs ) = &std::get<I>( tp )[from];
					init_test_ptrs<I + 1>( ptrs, tp, from );
				}

				template<std::size_t I = 0>
				inline typename std::enable_if<I == num_types_p_key, bool>::type
					equal( type_ptrs_p_key& tp )const
				{
					return true;
				}
				template<std::size_t I = 0>
				inline typename std::enable_if < I < num_types_p_key, bool>::type
					equal( type_ptrs_p_key & r )const
				{
					return *std::get<I>( ptrs ) == *std::get<I>( r ) && equal<I + 1>( r );
				}
			};

			const Entry_Ref get_entry( size_t index )noexcept
			{
				return { typePointers, index };
			}
		private:
			// Parameter pack pointer converter




			uint32_t version = 000001;
			void* data;
			std::size_t numEntries;
			std::size_t maxEntries;
			//static constexpr std::size_t byteWidth = size_of_prm_pack<Types...>();
			std::size_t byteWidth;

			const std::array<std::size_t, sizeof...( Types ) + 1> typeSizes{ sizeof( Key ), sizeof( Types )... };

			type_ptrs_p_key typePointers;

			std::unordered_map<Key, std::size_t, KeyHash> map;




			// Goes through the tuple and sets up the pointers for each attribute in the one allocation block. ( The stop recursion template)
			template<std::size_t I = 0>
			inline typename std::enable_if<I == num_types_p_key, void>::type
				setupPointers( type_ptrs_p_key& t, size_t maxEntries )
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
			template<std::size_t I = 0>
			inline typename std::enable_if < I < num_types_p_key, void>::type
				setupPointers( type_ptrs_p_key & t, size_t maxEntries )
			{
				std::get<I>( t ) = reinterpret_cast< typename std::tuple_element<I, type_ptrs_p_key>::type >( std::get<I - 1>( t ) + maxEntries );
				setupPointers<I + 1>( t, maxEntries );
			}

			// 'Takes one tuple of values and inserts them into the tuple holding arrays in the correct location.( The stop recursion template)
			template<std::size_t I = 0>
			inline typename std::enable_if<I == num_types_p_key, void>::type
				setValue( type_ptrs_p_key& tp, const Types_Tuple& t, std::size_t index )
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
			template<class T>
			void copy_help( T& t, const T& t2 )
			{
				t = t2;
			}
			template<size_t N>
			void copy_help( char( &t )[N], std::string_view t2 )
			{
				memcpy( &t, t2.data(), t2.size() + 1 );
				//t2.copy( (char*)&t, t2.size() + 1 );
			}
			template<size_t N>
			void copy_help( wchar_t( &t )[N], std::wstring_view t2 )
			{
				memcpy( &t, t2.data(), ( t2.size() + 1 ) * sizeof( wchar_t ) );
				//t2.copy( (char*)&t, t2.size() + 1 );
			}
			template<std::size_t I = 0>
			inline typename std::enable_if < I < num_types_p_key, void>::type
				setValue( type_ptrs_p_key & tp, const Types_Tuple & t, std::size_t index )
			{
				copy_help( std::get<I>( tp )[index], std::get<I>( t ) );
				setValue<I + 1>( tp, t, index );
			}

			// Copies an entry from one location to another ( The stop recursion template)
			template<std::size_t I = 0>
			inline typename std::enable_if<I == num_types_p_key, void>::type
				copyValue( type_ptrs_p_key& tp, std::size_t to, std::size_t from )
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

			template<std::size_t I = 0>
			inline typename std::enable_if < I < num_types_p_key, void>::type
				copyValue( type_ptrs_p_key & tp, std::size_t to, std::size_t from )
			{
				std::get<I>( tp )[to] = std::get<I>( tp )[from];
				copyValue<I + 1>( tp, to, from );
			}

			template<std::size_t I = 0>
			inline typename std::enable_if<I == num_types_p_key, void>::type
				memcpyTuple( type_ptrs_p_key& t1, type_ptrs_p_key& t2 )
			{}

			template<std::size_t I = 0>
			inline typename std::enable_if < I < num_types_p_key, void>::type
				memcpyTuple( type_ptrs_p_key & t1, type_ptrs_p_key & t2 )
			{
				memcpy( std::get<I>( t1 ), std::get<I>( t2 ), typeSizes[I] * numEntries );
				memcpyTuple<I + 1>( t1, t2 );
			}

		};

		template <class Key, class KeyHash, typename... Types>
		using SofA = SofA_Imp<Key, KeyHash, std::conjunction<std::is_trivially_copyable<Types>...>::value, Types...>;




		template <size_t I, class... Ts>
		decltype( auto ) get_n_arg( const Ts&... ts )
		{
			return std::get<I>( std::forward_as_tuple( ts... ) );
		}

		template <typename T, typename = int>
		struct has_clear : std::false_type{};

		template <typename T>
		struct has_clear <T, decltype( &T::clear, 0 )> : std::true_type{};

		template<class Key, class KeyHash, class... Types>
		class SofV{
		public:
			SofV( std::size_t size = 64 ) : numEntries( 0 ), maxEntries( size )
			{
				allocate( maxEntries );
			}
			SofV( SofV&& other )
			{

				numEntries = other.numEntries;
				maxEntries = other.maxEntries;
				tvec = std::move( other.tvec );
				map = std::move( other.map );

			}
			void operator=( SofV&& other )
			{

				numEntries = other.numEntries;
				maxEntries = other.maxEntries;
				tvec = std::move( other.tvec );
				map = std::move( other.map );

			}
			~SofV()
			{

			}

			void clear()
			{
				clearUnderlying<1>( tvec );
				numEntries = 0;
			}

			inline void shrink_to_fit()
			{
				allocate( numEntries );
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
					allocate( maxEntries * 2 );
				auto index = numEntries++;
				std::get<0>( tvec )[index] = key;
				return map[key] = index;
			}

			void add( const Key key, const Types&... args )
			{
				if ( numEntries + 1 > maxEntries )
					allocate( maxEntries * 2 );
				auto index = map[key] = numEntries++;
				setValue<0, Key, Types...>( tvec, index, key, args ... );
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
			inline const auto& peek( std::size_t index )const
			{
				return std::get<N>( tvec )[index];
			}

			template<std::size_t N>
			inline const auto& peek()const
			{
				return std::get<N>( tvec );
			}

			template<std::size_t N>
			inline auto get()
			{
				return std::get<N>( tvec );
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

				clearUnderlying<1>( tvec, last );

				map[last_key] = at;
				map.erase( at_key );
			}


		private:
			std::size_t numEntries;
			std::size_t maxEntries;

			std::tuple<std::vector<Key>, std::vector<Types>...> tvec;
			std::unordered_map<Key, std::size_t, KeyHash> map;


			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...( Types ), void>::type
				setValue( std::tuple<std::vector<Types>...>& tuple, std::size_t index, const Types&... args )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...( Types ), void>::type
				setValue( std::tuple<std::vector<Types>...> & tuple, std::size_t index, const Types &... args )
			{
				std::get<I>( tuple )[index] = get_n_arg<I>( args... );
				setValue<I + 1, Types...>( tuple, index, args... );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...( Types ), void>::type
				copyValue( std::tuple<std::vector<Types>...>& tuple, std::size_t to, std::size_t from )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...( Types ), void>::type
				copyValue( std::tuple<std::vector<Types>...> & tuple, std::size_t to, std::size_t from )
			{
				std::get<I>( tuple )[to] = std::move( std::get<I>( tuple )[from] );
				copyValue<I + 1, Types...>( tuple, to, from );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...( Types ), void>::type
				clearUnderlying( std::tuple<std::vector<Types>...>& tuple, std::size_t index )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...( Types ), void>::type
				clearUnderlying( std::tuple<std::vector<Types>...> & tuple, std::size_t index )
			{
				if constexpr ( has_clear< typename std::tuple_element<I, std::tuple<Types... >>::type >::value )
					std::get<I>( tuple )[index].clear();
				clearUnderlying<I + 1, Types...>( tuple, index );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...( Types ), void>::type
				clearUnderlying( std::tuple<std::vector<Types>...>& tuple )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...( Types ), void>::type
				clearUnderlying( std::tuple<std::vector<Types>...> & tuple )
			{
				if constexpr ( has_clear< typename std::tuple_element<I, std::tuple<Types... >>::type >::value )
					for ( auto& u : std::get<I>( tuple ) )
						u.clear();
				clearUnderlying<I + 1, Types...>( tuple );
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...( Types ), void>::type
				resizeVectorsInTuple( std::tuple<std::vector<Types>...>& tuple, std::size_t newSize )
			{}

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...( Types ), void>::type
				resizeVectorsInTuple( std::tuple<std::vector<Types>...> & tuple, std::size_t newSize )
			{
				std::get<I>( tuple ).resize( newSize );
				resizeVectorsInTuple<I + 1, Types...>( tuple, newSize );
			}

			void allocate( std::size_t newSize )
			{
				resizeVectorsInTuple( tvec, newSize );
				maxEntries = newSize;
			}
		};

	}
}












