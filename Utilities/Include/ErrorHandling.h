#ifndef _UTILITIES_ERROR_HANDLING_H_
#define _UTILITIES_E#RROR_HANDLING_H_
#include <exception>
#include <string>

namespace Utilities
{
	struct Exception : public std::exception {
		Exception(std::string&& what) : _what(what) { }
		virtual const char* what() const
		{
			return _what.c_str();
		}
	private:
		std::string _what;
	};
}
#endif