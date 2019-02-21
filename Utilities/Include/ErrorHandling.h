#ifndef _UTILITIES_ERROR_HANDLING_H_
#define _UTILITIES_E#RROR_HANDLING_H_
#include <string>

namespace Utilities
{
	struct Exception {
		virtual const std::string what() const = 0;
	};
}
#endif