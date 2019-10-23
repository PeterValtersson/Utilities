#ifndef _UTILITIES_PROFILER_H_
#define _UTILITIES_PROFILER_H_

#include <string_view>
#include <memory>
#include <vector>
#include <Utilities/CompileTimeString.h>
#include <Utilities/StringUtilities.h>
#include <sstream>
#include "Profile_Export.h"
#include <chrono>
namespace Utilities
{
	/****************************** Profiler ************************************/
	// One created for each thread.
	class Profiler{
	public:
		Profiler();
		DECLSPEC_PROFILER static std::shared_ptr<Profiler> get();
		DECLSPEC_PROFILER void start( StringHash hash, const char* str, const char* file )noexcept;
		DECLSPEC_PROFILER void stop() noexcept;
		const std::string to_str( int tabDepth )const noexcept;

		const std::string generate_tree()const noexcept;
	private:
		/**************************** ProfileEntry *******************************/
		// Container for all the information for a function.
		// A "Child" is s function that is called from this function.
		// There is no limit to the number of children (First child is in child, then child->nextChild, then child->nextChild->nextChild, and so on).
		// Parent is the calling function
		struct ProfileEntry;


		constexpr static std::string_view timeunit_str = "ms";
		using timeunit_chrono = std::chrono::milliseconds;

		std::shared_ptr<ProfileEntry> root;
		std::shared_ptr<ProfileEntry> current;

		void addChild( std::shared_ptr<ProfileEntry> parent, const std::shared_ptr<ProfileEntry> child ) noexcept;
		void addChild( const std::shared_ptr<ProfileEntry> child ) noexcept;

		void generate_tree( std::stringstream& ss, const std::shared_ptr<ProfileEntry> node )const noexcept;
		void to_str( std::stringstream& ss, const std::shared_ptr<ProfileEntry> entry, const int tabDepth )const noexcept;
	};

	class Profiler_Start_Stop{
	public:
		Profiler_Start_Stop( std::shared_ptr<Profiler> profiler, StringHash hash, const char* str, const char* file ) : profiler( profiler )
		{
			_ASSERT( profiler );
			profiler->start( hash, str, file );
		}
		~Profiler_Start_Stop()
		{
			profiler->stop();
		}
	private:
		std::shared_ptr<Profiler> profiler;
	};
}

#ifdef _ENABLE_PROFILER_
#define PROFILE Utilities::Profiler_Start_Stop __FUNCTION__##_profile(Utilities::Profiler::get(), Utilities::EnsureComplileTime<Utilities::hashString(__FUNCTION__)>::value, Utilities::String::get_function_name_closest_namespace(__FUNCTION__), Utilities::String::get_path_split_last2(__FILE__));
#define PROFILER_INIT Utilities::Profiler_Master::get()
#else
#define PROFILE
#endif
#endif