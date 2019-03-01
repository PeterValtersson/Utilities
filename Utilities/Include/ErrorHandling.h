#ifndef _UTILITIES_ERROR_HANDLING_H_
#define _UTILITIES_E#RROR_HANDLING_H_
#include <exception>
#include <string>

namespace Utilities
{
	struct Exception : public std::exception {
		Exception( std::string&& what ) : _what( what ) { }
		Exception( std::string&& what, std::string&& file, uint32_t line ) : _what( what + "\nFile: \n" + file + ".\nLine: " + std::to_string( line ) + "." ) { }
		virtual const char* what() const
		{
			return _what.c_str();
		}
	private:
		std::string _what;
	};
}


#define UNKOWN_ERROR Utilities::Exception( "Unknown error has occured.", __FILE__, __LINE__ )
#endif