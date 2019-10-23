#ifndef _UTILITIES_ERROR_HANDLING_H_
#define _UTILITIES_ERROR_HANDLING_H_
#include <exception>
#include <string>

namespace Utilities
{
	struct Exception : public std::exception {
		Exception( std::string_view what ) : _what( what ) { }
		Exception( std::string_view what, std::string_view file, uint32_t line ) : _what( std::string(what) + "\nFile: \n" + std::string( file ) + ".\nLine: " + std::to_string( line ) + "." ) { }
		virtual const char* what() const override
		{
			return _what.c_str();
		}
	private:
		std::string _what;
	};
}


#define UNKOWN_ERROR Utilities::Exception( "Unknown error has occured.", __FILE__, __LINE__ )
#endif