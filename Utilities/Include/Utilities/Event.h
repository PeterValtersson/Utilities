#ifndef _UTILITIES_EVENT_H_
#define _UTILITIES_EVENT_H_
#pragma once
#include "Delegate.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <type_traits>

namespace Utilities
{
	template<typename T1, typename T2>
	struct not_same{
		static constexpr bool value = !std::is_same<T1,T2>::value;
	};
	template <typename T> class Event;

	/**
	*@brief A collection of delegates.
	*/
	template<typename RET, typename... PARAMS>
	class Event<RET( PARAMS... )>{
		std::vector<Delegate<RET( PARAMS... )>> invokerList;
	public:

		/**
		*@brief Register a delegate to the event.
		* Example code:
		* @code
		* void foo(){cout << "Hello World" << endl;}
		* void foo2(){cout << "Hello World" << endl;}
		*class A
		*{
		* public:
		* void foo(){cout << "Hello World" << endl};
		*}
		* Delegate<void()> del(&foo);
		* ev += del;
		* ev += &foo2;
		* ev += [](){ cout << "Hello World" << endl;};
		* ev += {&a, &A::foo};
		* ev(); // Prints "Hello World" four times
		* @endcode
		*/
		Event& operator+=( const Delegate<RET( PARAMS... )>& other )noexcept
		{
			invokerList.push_back( other );
			return *this;
		}
		/**
		*@brief Makes a copy of the invokerlist
		*/
		Event& operator=( const Event<RET( PARAMS... )>& other )noexcept
		{
			this->invokerList = other.invokerList;
			return *this;
		}

		/**
		*@brief Unregister all callbacks.
		*/
		inline void Clear()noexcept
		{
			invokerList.clear();
		}
		inline size_t NumRegisteredCallbacks()const noexcept
		{
			return invokerList.size();
		}
		/**
		*@brief unregistering.
		*/
		Event& operator-=( const Delegate<RET( PARAMS... )>& other )noexcept
		{
			invokerList.erase( std::remove_if( invokerList.begin(),
											   invokerList.end(),
											   [&]( const Delegate<RET( PARAMS... )>& d )
			{
				return d == other;
			} ), invokerList.end() );
			return *this;
		}

		/**
		*@brief No eqaulity.
		*/
		bool operator==( const Event<RET( PARAMS... )>& other ) = delete;


		/**
		*@brief Invoke all the delegates. No return handling.
		*/
		inline void operator()( PARAMS... args )const noexcept
		{
			for ( auto& i : invokerList )
				i( std::forward<PARAMS>( args )... );
		}


		/**
		*@brief Invoke all the delegates. With return handling.
		* Example code:
		* @code
		* Event<int(int)> eve;
		* eve += [](int i) {return i * 2; };
		* int i = 5;
		* eve += [i](int a) {return i + a; };
		*
		* eve(1, [](size_t index, int* ret) {
		* 	std::cout << *ret << std::endl;
		* }); // Prints 2, 6
		* @endcode
		*/
		template<typename HANDLER>
		void operator()( PARAMS... param, HANDLER handler ) const noexcept
		{
			size_t index = 0;
			for ( auto& item : invokerList )
			{
				RET value = item( std::forward<PARAMS>( param )... );
				handler( index, value );
				++index;
			}
		}


		  /**
		  *@brief Invoke all the delegates. With return handling.
		  * Example code:
		  * @code
		  * Event<int(int)> eve;
		  * eve += [](int i) {return i * 2; };
		  * int i = 5;
		  * eve += [i](int a) {return i + a; };
		  *
		  * eve(1, [](size_t index, int* ret) {
		  * 	std::cout << *ret << std::endl;
		  * }); // Prints 2, 6
		  * @endcode
		  */
		template<class T = std::enable_if_t<!std::is_same<void, RET>::value, RET > >
		void operator()( PARAMS... args, Delegate<void( size_t, T& )> handler ) const noexcept
		{
			operator() < decltype( handler ) > ( std::forward<PARAMS>( args )..., handler );
		}

		/**
		*@brief Invoke all the delegates. With return handling.
		* Example code:
		* @code
		* Event<int(int)> eve;
		* eve += [](int i) {return i * 2; };
		* int i = 5;
		* eve += [i](int a) {return i + a; };
		*
		* eve(1, [](size_t index, int* ret) {
		* 	std::cout << *ret << std::endl;
		* }); // Prints 2, 6
		* @endcode
		*/
		template<class T = std::enable_if_t<!std::is_same<void, RET>::value, RET > >
		void operator()( PARAMS... args, std::function<void( size_t, T& )> handler ) const noexcept
		{
			operator() < decltype( handler ) > ( std::forward<PARAMS>( args )..., handler );
		}
	};

}

#endif