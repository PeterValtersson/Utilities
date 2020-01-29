#ifndef _UTILITIES_STOP_WATCH_H_
#define _UTILITIES_STOP_WATCH_H_
#pragma once
#include <chrono>
#include <vector>
#include "Time_Typedefs.h"
#include <numeric>
namespace Utilities
{
	namespace Time
	{
		class StopWatch{


		public:
			StopWatch()
			{
				
			};
			~StopWatch()
			{};
			inline void start()noexcept
			{
				if ( !watchState )
				{
					watchState = true;
					time_stamps.clear();
					time_stamps.push_back( std::chrono::high_resolution_clock::now() );
				}
			}

			template<typename TimeType = std::chrono::milliseconds>
			inline float stamp()noexcept
			{
				if ( watchState )
				{
					time_stamps.push_back( std::chrono::high_resolution_clock::now() );
					return ( float )std::chrono::duration_cast< TimeType >( time_stamps.back() - time_stamps[0] ).count();
				}
				return 0;
			}

			template<typename TimeType = std::chrono::milliseconds>
			inline float stop()noexcept
			{
				watchState = false;
				return stamp();
			}

			inline std::vector<Timestamp> get_time_stamps()noexcept
			{
				return time_stamps;
			}

			template<typename TimeType = std::chrono::milliseconds>
			inline std::vector<float> get_durations()noexcept
			{
				std::vector<float> durations;
				std::adjacent_difference( time_stamps.begin(), time_stamps.end(), durations.begin(), []( const Timestamp& t1, const Timestamp& t2 )
				{
					return ( float )std::chrono::duration_cast< TimeType >( t2 - t1 ).count();
				} );
				return durations;
			}
		private:
			std::vector<Timestamp> time_stamps;
			bool watchState = false;

		};
	}
}
#endif