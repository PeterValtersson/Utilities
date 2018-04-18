#pragma once
#include <cstddef>
#include <tuple>
#include <array>
#include <unordered_map>
#include <optional>
#include <list>

namespace Utilities
{
	namespace SofA
	{
		namespace Array
		{
			// Parameter pack pointer converter
			template<class T> struct make_ptr_t { typedef T* type; };
			template<class T> struct make_ptr_t<T*> { typedef T* type; };


			// Goes through the tuple and sets up the pointers for each attribute in the one allocation block. ( The stop recursion template)
			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				setupPointers(std::tuple<typename make_ptr_t<Types>::type...>& t, size_t allocated)
			{ }

			// Goes through the tuple and sets up the pointers for each attribute in the one allocation block. 
			/*
			// Setup the new pointers
			newData.data = operator new(newSize);
			newData.key = (int*)newData.data;
			newData.v1 = (int*)(newData.key + newData.allocated);
			newData.v2 = (bool*)(newData.v1 + newData.allocated);
			newData.v3 = (char*)(newData.v2 + newData.allocated);

			|0|1|2|3|4|5|6|7|
			 ^		 ^
			 |		 |
			 int *	 bool*
			*/
			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				setupPointers(std::tuple<typename make_ptr_t<Types>::type...>& t, size_t allocated)
			{
				std::get<I>(t) = reinterpret_cast<typename std::tuple_element<I, std::tuple<typename make_ptr_t<Types>::type...>>::type>(std::get<I - 1>(t) + allocated);
				setupPointers<I + 1, Types...>(t, allocated);
			}

			// 'Takes one tuple of values and inserts them into the tuple holding arrays in the correct location.( The stop recursion template)
			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				setValue(std::tuple<typename make_ptr_t<Types>::type...>& tp, const std::tuple<Types...>& t, std::size_t index)
			{ }

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
				setValue(std::tuple<typename make_ptr_t<Types>::type...>& tp, const std::tuple<Types...>& t, std::size_t index)
			{
				std::get<I>(tp)[index] = std::get<I>(t);
				setValue<I + 1, Types...>(tp, t, index);
			}

			// Copies an entry from one location to another ( The stop recursion template)
			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				copyValue(std::tuple<typename make_ptr_t<Types>::type...>& tp, std::size_t to, std::size_t from)
			{ }
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
				copyValue(std::tuple<typename make_ptr_t<Types>::type...>& tp, std::size_t to, std::size_t from)
			{
				std::get<I>(tp)[to] = std::get<I>(tp)[from];
				copyValue<I + 1, Types...>(tp, to, from);
			}

			template<class Key, class KeyHash, class... Types>
			class SofA
			{
			public:
				SofA(std::size_t size = 64) : used(0), allocated(size), byteWidth(0)
				{
					for (auto&& v : typeSizes)
						byteWidth += v;
					Allocate(allocated);
				}
				~SofA()
				{
					operator delete(data);
				}
				/*@brief Clear the entries, only sets used to 0. All memory and data is intact*/
				inline void clear()
				{
					used = 0;
					map.clear();
				}
				inline size_t Allocated()const
				{
					return allocated;
				}

				/*@brief Shrinks the block to exactly fit the amount used.*/
				inline void shrink_to_fit()
				{
					Allocate(used);
				}
				inline std::optional<std::pair<Key, std::size_t>> find(const Key key)const
				{
					if (auto const find = map.find(key); find != map.end())
						return { { find->first, find->second } };
					else
						return std::nullopt;
				}

				inline std::size_t size()const { return used; };

				void add(const Key key, const Types... args)
				{
					if (used + 1 > allocated)
						Allocate(allocated * 2);
					auto index = map[key] = used++;
					const auto tpl = std::make_tuple(key, args...);
					setValue<0, Key, Types...>(typePointers, tpl, index);
				}

				std::size_t add(const Key key)
				{
					if (used + 1 > allocated)
						Allocate(allocated * 2);
					std::get<0>(typePointers)[used] = key;
					return map[key] = used++;
				}

				template<std::size_t N, class type>
				inline void set(std::size_t index, const type&& t)
				{
					std::get<N>(typePointers)[index] = t;
				}

				template<std::size_t N>
				inline auto& get(std::size_t index)
				{
					return std::get<N>(typePointers)[index];
				}
				template<std::size_t N>
				inline const auto& getConst(std::size_t index)const
				{
					return std::get<N>(typePointers)[index];
				}
				template<std::size_t N>
				inline auto& get()
				{
					return std::get<N>(typePointers);
				}
				template<std::size_t N>
				inline const auto& getConst()const
				{
					return std::get<N>(typePointers);
				}

				bool erase(const Key key)
				{
					if (const auto find = map.find(key); find != map.end())
					{
						destroy(find->second);
						return true;
					}
					return false;
				}


				void destroy(std::size_t at)
				{
					auto last = --used;

					auto at_key = std::get<0>(typePointers)[at];
					auto last_key = std::get<0>(typePointers)[last];

					copyValue<0, Key, Types...>(typePointers, at, last);

					map[last_key] = at;
					map.erase(at_key);
				}

				std::size_t GetMemoryUsage()const
				{
					return byteWidth * allocated;
				}
				void Allocate(std::size_t newSize)
				{
					std::size_t newAllocated = newSize;
					void* newData = operator new(newAllocated*byteWidth);
					type newTypePointers;

					std::get<0>(newTypePointers) = (Key*)newData;
					setupPointers<1, Key, Types...>(newTypePointers, newAllocated);

					memcpyTuple<0, Key, Types...>(newTypePointers, typePointers);

					operator delete(data);
					typePointers = newTypePointers;
					allocated = newAllocated;
					data = newData;
				}
				void Reinit(std::size_t newUsed)
				{
					used = newUsed;
					map.clear();
					for (std::size_t i = 0; i < used; i++)
						map[std::get<0>(typePointers)[i]] = i;
				}
			private:
				void *data;
				std::size_t used;
				std::size_t allocated;
				std::size_t byteWidth;

				const std::array<std::size_t, sizeof...(Types)+1> typeSizes{ sizeof(Key), sizeof(Types)... };
				typedef std::tuple<typename make_ptr_t<Key>::type, typename make_ptr_t<Types>::type...> type;
				type typePointers;

				std::unordered_map<Key, std::size_t, KeyHash> map;




				template<std::size_t I = 0, typename... Types>
				inline typename std::enable_if<I == sizeof...(Types), void>::type
					memcpyTuple(std::tuple<typename make_ptr_t<Types>::type...>& t1, std::tuple<typename make_ptr_t<Types>::type...>& t2)
				{ }

				template<std::size_t I = 0, class... Types>
				inline typename std::enable_if < I < sizeof...(Types), void>::type
					memcpyTuple(std::tuple<typename make_ptr_t<Types>::type...>& t1, std::tuple<typename make_ptr_t<Types>::type...>& t2)
				{
					memcpy(std::get<I>(t1), std::get<I>(t2), typeSizes[I] * used);
					memcpyTuple<I + 1, Types...>(t1, t2);
				}

			};

		}
		namespace Vector
		{

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				setValue(std::tuple<std::vector<Types>...>& tuple, const std::tuple<Types...>& t, std::size_t index)
			{ }

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				setValue(std::tuple<std::vector<Types>...>& tuple, const std::tuple<Types...>& t, std::size_t index)
			{
				std::get<I>(tuple)[index] = std::get<I>(t);
				setValue<I + 1, Types...>(tuple, t, index);
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				copyValue(std::tuple<std::vector<Types>...>& tuple, std::size_t to, std::size_t from)
			{ }

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				copyValue(std::tuple<std::vector<Types>...>& tuple, std::size_t to, std::size_t from)
			{
				std::get<I>(tuple)[to] = std::move(std::get<I>(tuple)[from]);
				copyValue<I + 1, Types...>(tuple, to, from);
			}

			template<std::size_t I = 0, typename... Types>
			inline typename std::enable_if<I == sizeof...(Types), void>::type
				resizeVectorsInTuple(std::tuple<std::vector<Types>...>& tuple, std::size_t newSize)
			{ }

			template<std::size_t I = 0, class... Types>
			inline typename std::enable_if < I < sizeof...(Types), void>::type
				resizeVectorsInTuple(std::tuple<std::vector<Types>...>& tuple, std::size_t newSize)
			{
				std::get<I>(tuple).resize(newSize);
				resizeVectorsInTuple<I + 1, Types...>(tuple, newSize);
			}





			template<class Key, class KeyHash, class... Types>
			class SofA
			{
			public:
				SofA(std::size_t size = 64) : used(0), allocated(size)
				{
					Allocate(allocated);
				}

				~SofA()
				{

				}

				void clear()
				{
					used = 0;
				}

				inline void shrink_to_fit()
				{
					Allocate(used);
				}
				inline std::optional<std::pair<Key, std::size_t>> find(const Key key)const
				{
					if (auto const find = map.find(key); find != map.end())
						return { { find->first, find->second } };
					else
						return std::nullopt;
				}

				inline std::size_t size()const { return used; };

				std::size_t add(const Key key)
				{
					if (used + 1 > allocated)
						Allocate(allocated * 2);
					auto index = used++;
					std::get<0>(tvec)[index] = key;
					return map[key] = index;
				}

				void add(const Key key, const Types... args)
				{
					if (used + 1 > allocated)
						Allocate(allocated * 2);
					auto index = map[key] = used++;
					const auto tpl = std::make_tuple(key, args...);
					setValue<0, Key, Types...>(tvec, tpl, index);
				}

				template<std::size_t N, class type>
				inline void set(std::size_t index, type& t)
				{
					std::get<N>(tvec)[index] = t;
				}
				template<std::size_t N, class type>
				inline void set(std::size_t index, type&& t)
				{
					std::get<N>(tvec)[index] = t;
				}

				template<std::size_t N>
				inline auto& get(std::size_t index)
				{
					return std::get<N>(tvec)[index];
				}
				template<std::size_t N>
				inline const auto& getConst(std::size_t index)const
				{
					return std::get<N>(tvec)[index];
				}


				template<std::size_t N>
				inline auto get()
				{
					return std::get<N>(tvec).data();
				}

				bool erase(const Key key)
				{
					if (const auto find = map.find(key); find != map.end())
					{
						destroy(find->second);
						return true;
					}
					return false;
				}


				void destroy(std::size_t at)
				{
					auto last = --used;

					auto at_key = std::get<0>(tvec)[at];
					auto last_key = std::get<0>(tvec)[last];

					copyValue<0, Key, Types...>(tvec, at, last);

					map[last_key] = at;
					map.erase(at_key);
				}


			private:
				std::size_t used;
				std::size_t allocated;

				std::tuple<std::vector<Key>, std::vector<Types>...> tvec;
				std::unordered_map<Key, std::size_t, KeyHash> map;

				void Allocate(std::size_t newSize)
				{
					resizeVectorsInTuple(tvec, newSize);
					allocated = newSize;
				}
			};

		}
	}
}












