#ifndef _UTILITIES_TIME_H_
#define _UTILITIES_TIME_H_
#include <string>
#include <time.h>

namespace Utilities
{
	namespace Time
	{
		inline const std::string currentDateTime()
		{
			time_t     now = time(NULL);
			struct tm  tstruct;
			char       buf[80];
#ifdef _WIN32
			localtime_s(&tstruct, &now);
#else
			tstruct = *localtime(&now);
#endif
			strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

			return buf;
		}
	}
}
#endif