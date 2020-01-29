#ifndef _UTILITIES_TIMER_H_
#define _UTILITIES_TIMER_H_
#pragma once
#include <chrono>
#include "Time_Typedefs.h"

namespace Utilities
{
	namespace Time
	{
		class Timer{


		public:
			Timer()
			{
				curTime = std::chrono::high_resolution_clock::now();
				prevTime = curTime;
			};
			~Timer()
			{};
			inline void Tick()
			{
				prevTime = curTime;
				curTime = std::chrono::high_resolution_clock::now();
			};

			template<typename Ratio = std::milli>
			inline float GetDelta() const
			{
				return std::chrono::duration<float, Ratio>( curTime - prevTime ).count();
			};

		private:
			Timestamp curTime;
			Timestamp prevTime;

		};
	}
}
#endif