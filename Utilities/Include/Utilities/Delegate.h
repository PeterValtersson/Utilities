#ifndef _UTILITIES_DELEGATE_H_
#define _UTILITIES_DELEGATE_H_
#pragma once
#include <functional>
#include <type_traits>

namespace Utilities
{



	template <typename T> class Delegate;

	template<typename RET, typename... PARAMS>
	class Delegate<RET( PARAMS... )>{
		std::function<RET( PARAMS... )> invoker;
		size_t uniqueIdentifier;
	public:

		Delegate()
		{}

		operator bool()
		{
			return invoker.operator bool();
		}

	/**
	*@brief Copy constructor.
	*/
		Delegate( const Delegate& other )
		{
			this->invoker = other.invoker;
			this->uniqueIdentifier = other.uniqueIdentifier;
		}
		/**
		*@brief Copy constructor with rvalue.
		*/
		Delegate( Delegate&& other )
		{
			this->invoker = std::move( other.invoker );
			this->uniqueIdentifier = other.uniqueIdentifier;
			other.uniqueIdentifier = 0;
		}

		/**
		*@brief Create delegate from function pointer.
		*@param ptr The function pointer. (&foo)
		* Example code:
		* @code
		* void Call(const Delegate<void()>& del) { del();}
		* void foo(){cout << "Hello World" << endl;}
		*
		* Delegate<void()> del(&foo);
		* del(); // Prints "Hello World"
		* Call(&foo); // Prints "Hello World"
		* @endcode
		*/
		Delegate( RET( ptr )( PARAMS... ) )
		{
			invoker = ptr;
			uniqueIdentifier = (size_t)ptr;
		}

		/**
		*@brief Create delegate from lambda.
		* Example code:
		* @code
		* void Call(const Delegate<void()>& del) { del();}
		* void foo(){cout << "Hello World" << endl;}
		*
		* Delegate<void()> del([](){ cout << "Hello World" << endl;});
		* del(); // Prints "Hello World"
		* Call([&del](){foo(); del();}); // Prints "Hello World" twice
		* @endcode
		*/
		template <typename T>
		Delegate( const T& lambda )
		{
			invoker = lambda;
			if constexpr ( std::is_same<T, std::function< RET( PARAMS... ) >>::value )
				uniqueIdentifier = lambda.target_type().hash_code();
			else
				uniqueIdentifier = ( size_t )lambda;
		}


		/**
		*@brief Create delegate from class method.
		*@param [in] instance The pointer to the class object. (Both this, and &myClass) works.
		*@param [in] TMethod The class method pointer. (&Class::Method)
		* Example code:
		* @code
		* void Call(const Delegate<void()>& del) { del();}
		* class A
		*{
		* public:
		* void foo(){cout << "Hello World" << endl;}
		*}
		*
		* A a;
		* Delegate<void()> del = {&a, &A::foo};
		* del(); // Prints "Hello World"
		* Call(del); // Prints "Hello World"
		* Call({&a, &A::foo}); // Prints "Hello World"
		* @endcode
		*/


		template <class T>
		Delegate( T* instance, RET( T::* TMethod )( PARAMS... ) )
		{
			invoker = [instance, TMethod]( PARAMS... params ) -> RET
			{
				T* p = static_cast< T* >( instance );
				return ( instance->*TMethod )( std::forward<PARAMS>( params )... );
			};
			union MCVRT				{
				size_t iptr;
				RET( T::* ptr )( PARAMS... );
			};
			MCVRT mcvrt;
			mcvrt.ptr = TMethod;
			uniqueIdentifier = (size_t)instance ^ mcvrt.iptr;
		}


		/**
		*@brief Create delegate from const class method.
		*@param [in] instance The pointer to the class object. (Both this, and &myClass) works.
		*@param [in] TMethod The class method pointer. (&Class::Method)
		* Example code:
		* @code
		* void Call(const Delegate<void()>& del) { del();}
		* class A
		*{
		* public:
		* void foo()const{cout << "Hello World" << endl;}
		*}
		*
		* A a;
		* Delegate<void()> del = {&a, &A::foo};
		* del(); // Prints "Hello World"
		* Call(del); // Prints "Hello World"
		* Call({&a, &A::foo}); // Prints "Hello World"
		* @endcode
		*/
		template <class T>
		Delegate( const T* instance, RET( T::* TMethod )( PARAMS... ) const )
		{
			invoker = [instance, TMethod]( PARAMS... params ) -> RET
			{
				T* const p = const_cast< T* >( instance );
				return ( instance->*TMethod )( std::forward<PARAMS>( params )... );
			};
			union MCVRT{
				size_t iptr;
				RET( T::* ptr )( PARAMS... );
			};
			MCVRT mcvrt;
			mcvrt.ptr = TMethod;
			uniqueIdentifier = ( size_t )instance ^ mcvrt.iptr;
		}

		/**
		*@brief Equal operator.
		*/
		constexpr bool operator==( const Delegate& other )const
		{
			return uniqueIdentifier == other.uniqueIdentifier;
		}
		constexpr bool operator!=( const Delegate& other )const
		{
			return uniqueIdentifier != other.uniqueIdentifier;
		}
		bool operator+=( const Delegate& other )const = delete;
		bool operator-=( const Delegate& other )const = delete;
		bool operator+( const Delegate& other )const = delete;
		bool operator-( const Delegate& other )const = delete;

		/**
		*@brief Assignment from Delegate to Delegate.
		*/
		Delegate& operator=( const Delegate& other )
		{
			this->invoker = other.invoker;
			uniqueIdentifier = other.uniqueIdentifier;
			return *this;
		}

		/**
		*@brief Assignment from Delegate to Delegate, with rvalue.
		*/
		Delegate& operator=( const Delegate&& other )
		{
			this->invoker = std::move( other.invoker );
			uniqueIdentifier = other.uniqueIdentifier;
			return *this;
		}

		/**
		*@brief Invoke the delegate.
		*/
		inline RET operator()( PARAMS... args )const
		{
			return invoker( std::forward<PARAMS>( args )... );
		}
	};
}
#endif