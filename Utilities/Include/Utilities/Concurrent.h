#ifndef _UTILITIES_CONCURRENT_H_
#define _UTILITIES_CONCURRENT_H_
#include <mutex>
namespace Utilities
{
	template<class T>
	class Concurrent{
	public:
		Concurrent(  )
		{}
		Concurrent( T&& t ) : obj( std::move(t) )
		{}
		template<class F, typename... Param>
		inline auto operator()( F const& f, Param... args )const -> typename std::enable_if<std::is_void<typename std::result_of<F( T&, Param... )>::type>::value, void>::type
		{
			std::lock_guard<std::mutex> lg( lock );
			f( obj, std::forward<Param>( args )... );
		}

		template<class F, typename... Param>
		inline auto operator()( F const& f, Param... args )const -> typename std::enable_if<!std::is_void<typename std::result_of<F( T&, Param... )>::type>::value, const typename  std::result_of<F( T&, Param... )>::type>::type
		{
			std::lock_guard<std::mutex> lg( lock );
			return f( obj, std::forward<Param>( args )... );
		}

		template<class F, typename... Param>
		inline auto operator()( F const& f, Param... args ) -> typename std::enable_if<std::is_void<typename std::result_of<F(T&, Param...)>::type>::value, void>::type
		{
			std::lock_guard<std::mutex> lg( lock );
			f( obj, std::forward<Param>( args )... );
		}

		template<class F, typename... Param>
		inline auto operator()( F const& f, Param... args ) -> typename std::enable_if<!std::is_void<typename std::result_of<F( T&, Param... )>::type>::value,typename  std::result_of<F( T&, Param... )>::type>::type
		{
			std::lock_guard<std::mutex> lg( lock );
			return f( obj, std::forward<Param>( args )... );
		}
	private:
		mutable std::mutex lock;
		T obj;
	};
}
#endif