#ifndef _UTILZ_MATH_H_
#define _UTILZ_MATH_H_

#include <string>
#include <Windows.h>
#include <vector>

namespace Utilities
{
	namespace Math
	{
		inline float degrees_to_radians(float degrees)
		{
			return degrees * 0.0174532925f;
		}
		inline float radians_to_degrees(float radians)
		{
			return radians *57.2957795f;
		}
	}
}

#endif