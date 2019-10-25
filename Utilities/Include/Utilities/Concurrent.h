#ifndef _UTILITIES_CONCURRENT_H_
#define _UTILITIES_CONCURRENT_H_
#include <mutex>
namespace Utilities 
{
	template<class T> 
	class Concurrent {
	public:
		template<class F, typename... Param>
		inline void operator()(F const& f, Param... args)
		{
			std::lock_guard<std::mutex> lg(lock);
			f(obj, std::forward<Param>(args)...);
		}
	private:
		std::mutex lock;
		T obj;
	};
}
#endif