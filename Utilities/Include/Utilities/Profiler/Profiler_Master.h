#pragma once
#include <Utilities/Profiler/Profile_Export.h>
#include <Utilities/Profiler/Profiler.h>
#include <memory>
#include <thread>
#include <Utilities/Concurrent.h>
namespace Utilities
{
	class Profiler_Master;
	/******************************** Profiler_Master *********************************/
		// Only one is created accross all DLLs and Threads.
		// Creates and handles the Profilers for each individual thread.
	class Profiler_Master {
	public:		
		Profiler_Master()
		{}
		DECLSPEC_PROFILER static std::shared_ptr<Profiler_Master> get()noexcept;
		std::shared_ptr<Profiler> get_profiler( std::thread::id threadID )noexcept;
		~Profiler_Master()
		{}
		DECLSPEC_PROFILER const std::string to_str() noexcept;
		DECLSPEC_PROFILER const int generate_tree( const std::string& folder, bool convertDotToPdf = false, std::string_view unit = "ms" ) noexcept;
	private:
		struct ID_Profiler_Pair {
			std::thread::id threadID; std::shared_ptr<Profiler> profiler;
			inline bool operator==(const std::thread::id ID )const noexcept
			{
				return threadID == ID;
			}
		};
		
		Utilities::Concurrent<std::vector<ID_Profiler_Pair>> profilers;

	};

}