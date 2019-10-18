#ifndef _UTILITIES_PROFILER_H_
#define _UTILITIES_PROFILER_H_
#ifdef _ENABLE_PROFILER_
#include <string_view>
#include <memory>
#include <vector>
#include <thread>
#include "Concurrent.h"
#include "MonadicOptional.h"
#include "CompileTimeString.h"
#include "StringUtilities.h"
#include <sstream>
#include "Profile_Export.h"
#include <filesystem>
#include <fstream>
#include "TimeUtilities.h"
#include <chrono>

namespace Utilities
{

	/**************************** ProfileEntry *******************************/
	// Container for all the information for a function.
	// A "Child" is s function that is called from this function.
	// There is no limit to the number of children (First child is in child, then child->nextChild, then child->nextChild->nextChild, and so on).
	// Parent is the calling function
	struct ProfileEntry {
		ProfileEntry(const char* str) : name(str), file(""), timesCalled(0), parent(nullptr), hash(0), timeSpent(0) {};
		ProfileEntry(const char* str, HashValue hash, const char* file, std::shared_ptr<ProfileEntry> parent) : name(str), hash(hash), file(file), timesCalled(0), parent(parent), timeSpent(0) {};
		std::string name;
		std::string file;
		HashValue hash;
		size_t timesCalled;
		std::shared_ptr<ProfileEntry> parent;
		std::shared_ptr<ProfileEntry> child;
		std::shared_ptr<ProfileEntry> nextChild;
		std::chrono::nanoseconds timeSpent;
		std::chrono::high_resolution_clock::time_point start;
	};



	/****************************** Profiler ************************************/
	// One created for each thread.
	class Profiler {
	public:
		inline static std::shared_ptr<Profiler> get();
		void start(HashValue hash, const char* str, const char* file)
		{
			current = *findEntry(hash).or_else_this([&]
				{
					auto newChild = std::make_shared<ProfileEntry>(str, hash, file, current);
					addChild(newChild);
					return newChild;
				});

			current->timesCalled++;
			current->start = std::chrono::high_resolution_clock::now();

		}
		void stop() noexcept
		{
			auto stop = std::chrono::high_resolution_clock::now();
			current->timeSpent += stop - current->start;
			current = current->parent;
		}
		std::string str(int tabDepth) noexcept
		{
			std::stringstream ss;
			auto walker = root->child;
			while (walker)
			{
				str(ss, walker, tabDepth);
				walker = walker->nextChild;
			}
			return ss.str();
		}
		void str(std::stringstream& ss, std::shared_ptr<ProfileEntry> entry, int tabDepth) noexcept
		{
			ss << tabs(tabDepth) << entry->name << ": Times called: " << entry->timesCalled
				<< " Time spent: " << std::chrono::duration_cast<std::chrono::nanoseconds>(entry->timeSpent).count()
				<< " Average: " << std::chrono::duration_cast<std::chrono::nanoseconds>(entry->timeSpent).count() / entry->timesCalled << std::endl;
			auto walker = entry->child;
			while (walker)
			{
				str(ss, walker, tabDepth + 1);
				walker = walker->nextChild;
			}

		}

		template <class timeUnit>
		std::string getDotTree(std::string_view unit) noexcept
		{
			std::stringstream ss;
			auto walker = root->child;
			while (walker)
			{
				getDotTree<timeUnit>(ss, walker, unit);
				walker = walker->nextChild;
			}
			return ss.str();
		}
	private:
		std::shared_ptr<ProfileEntry> root = std::make_shared<ProfileEntry>("Root");
		std::shared_ptr<ProfileEntry> current = root;

		void addChild(std::shared_ptr<ProfileEntry> parent, std::shared_ptr<ProfileEntry> child) noexcept
		{
			if (!parent->nextChild)
				parent->nextChild = child;
			else
				addChild(parent->nextChild, child);
		}
		void addChild(std::shared_ptr<ProfileEntry> child) noexcept
		{
			if (!current->child)
				current->child = child;
			else
				addChild(current->child, child);
		}

		optional<std::shared_ptr<ProfileEntry>> findEntry(HashValue hash) noexcept
		{
			std::shared_ptr<ProfileEntry> walker;
			walker = current->child;
			while (walker && walker->hash != hash)
				walker = walker->nextChild;
			if (walker)
				return walker;
			return std::nullopt;
		}

		template <class timeUnit>
		void getDotTree(std::stringstream& ss, std::shared_ptr<ProfileEntry> node, std::string_view unit) noexcept
		{
			ss << "\"" << node << "\"" << "[\n shape = none" << std::endl;
			ss << "label = <<table border=\"0\" cellspacing = \"0\">" << std::endl;
			double div = 0.0;
			if (node->parent && node->parent->hash != 0)
				div = ((double)node->timeSpent.count() / node->parent->timeSpent.count());
			ss << "<tr><td port=\"port1\" border=\"1\" bgcolor = \"#"
				<< charToHex(uint8_t(150 * div)) << charToHex(50) << charToHex(uint8_t(50 * (1.0 - div))) << "\">" << std::endl;
			ss << "<font color=\"white\">" << node->file << ": " << replaceAll(replaceAll(node->name, "<", "\\<"), ">", "\\>") << "</font></td></tr>\n" << std::endl;

			ss << "<tr><td border=\"1\">" << "Times Called: " << node->timesCalled << "</td></tr>" << std::endl;
			ss << "<tr><td border=\"1\">" << "Time Spent(IC): " << std::chrono::duration_cast<timeUnit>(node->timeSpent).count() << " " << unit;
			if (node->parent && node->parent->hash != 0)
				ss << " " << div * 100 << " % of parents.</td></tr>" << std::endl;
			else
				ss << "</td></tr>" << std::endl;

			ss << "<tr><td border=\"1\">" << "Time Spent(avg): " << std::chrono::duration_cast<timeUnit>(node->timeSpent).count() / double(node->timesCalled) << " " << unit << "</td></tr>" << std::endl;

			if (node->child)
			{
				auto walker = node->child;
				auto timeEC = node->timeSpent;
				while (walker)
				{
					timeEC -= walker->timeSpent;
					walker = walker->nextChild;
				}
				ss << "<tr><td border=\"1\">" << "Time Spent(EC): " << std::chrono::duration_cast<timeUnit>(timeEC).count() << " " << unit << "</td></tr>" << std::endl;
			}

			ss << "</table>>]" << std::endl;

			auto walker = node->child;
			while (walker)
			{
				getDotTree<timeUnit>(ss, walker, unit);
				ss << "\"" << node << "\":port1 -> \"" << walker << "\":port1" << std::endl;
				walker = walker->nextChild;
			}

			ss << "" << std::endl;
		}
	};

	struct ID_Profiler_Pair {
		std::thread::id threadID; std::shared_ptr<Profiler> profiler;
	};
	inline bool operator==(ID_Profiler_Pair const& l, std::thread::id r) { return l.threadID == r; }


	/******************************** Profiler_Master *********************************/
	// Only one is created.
	// Creates and handles the Profilers for each individual thread.
	class Profiler_Master {
	public:
		DECLSPEC_PROFILER static std::shared_ptr<Profiler_Master> get();
		std::shared_ptr<Profiler> getProfiler(std::thread::id threadID);
		~Profiler_Master() {}
		inline std::string str() noexcept;
		template <class timeUnit = std::chrono::milliseconds>
		int createDotAndBat(const std::string& folder, bool convertDotToPdf = false, std::string_view unit = "ms") noexcept;
	private:
		Utilities::Concurrent<std::vector<ID_Profiler_Pair>> profilers;
		Profiler_Master() {}
	};


	/**************** Profiler_Master Function definitions ****************************/
	inline std::shared_ptr<Profiler> Utilities::Profiler_Master::getProfiler(std::thread::id threadID)
	{
		std::shared_ptr<Profiler> profiler;
		profilers.operate([&](std::vector<ID_Profiler_Pair>& v)
			{
				profiler = *Utilities::find(v, threadID).or_else_this([&]
					{
						v.push_back({ threadID,  std::make_shared<Profiler>() });
						return v.size() - 1;
					}).map([&](index i)
						{
							return v[i].profiler;
						});
			});

		return profiler;

	}
	inline std::string Utilities::Profiler_Master::str() noexcept
	{
		std::stringstream ss;
		profilers.operate([&](auto& ps)
			{
				ss << std::endl;
				for (auto profiler : ps)
				{
					ss << "Profile Thread " << profiler.threadID << std::endl;
					ss << profiler.profiler->str(1) << std::endl;
				}
			});
		return ss.str();
	}

	template <class timeUnit>
	int Utilities::Profiler_Master::createDotAndBat(const std::string& folder, bool convertDotToPdf, std::string_view unit) noexcept
	{

		std::stringstream ss;

		ss << "digraph Profile_Graph{\n";
		ss << "rankdir = LR;" << std::endl;

		profilers.operate([&](std::vector<ID_Profiler_Pair>& locked_profilers)
			{
				for (auto& profiler : locked_profilers)
				{
					ss << "subgraph \"cluster_" << profiler.threadID << "\" {" << std::endl;
					ss << "label = \"Thread " << profiler.threadID << "\";" << std::endl;
					ss << profiler.profiler->getDotTree<timeUnit>(unit) << std::endl;
					ss << "}" << std::endl;
				}


			});


		ss << std::endl << "}" << std::endl;


		std::filesystem::create_directories(folder);
		std::stringstream filename;
		filename << folder << "/Profile_" << Time::currentDateTime();
		auto strFilename = replaceAll(replaceAll(filename.str(), " ", "_"), ":", "-");
		std::ofstream out(strFilename + ".dot");
		if (!out.is_open())
			return 1 << 16;

		out << ss.str();
		out.close();
		if (convertDotToPdf)
		{
#include <stdlib.h>
			std::stringstream cmd;
			cmd << "dot -Tpdf " << strFilename << ".dot" << " -o " << strFilename << ".pdf";
			int ret = system(cmd.str().c_str());
			if (ret == 0)
			{
				std::stringstream cmd;
				cmd << "del " << strFilename << ".dot";
				system(cmd.str().c_str());
			}
			return ret;
		}
		return 0;
	}
	/******************** Profiler Function definitions *********************************/
	inline std::shared_ptr<Profiler> Utilities::Profiler::get()
	{
		thread_local static std::shared_ptr<Profiler> profiler;
		if (!profiler)
		{
			profiler = Profiler_Master::get()->getProfiler(std::this_thread::get_id());
		}
		return profiler;
	}

	class Profiler_Start_Stop {
	public:
		Profiler_Start_Stop(std::shared_ptr<Profiler> profiler, HashValue hash, const char* str, const char* file) : profiler(profiler)
		{
			_ASSERT(profiler);
			profiler->start(hash, str, file);
		}
		~Profiler_Start_Stop()
		{
			profiler->stop();
		}
	private:
		std::shared_ptr<Profiler> profiler;
	};



	namespace Basename
	{

		// Retrieves the filename (and the closest folder) from a path
		constexpr const char* fileName(const char* path)
		{
			const char* f2 = nullptr;
			const char* f = nullptr;
			const char* c = path;
			while (*c != '\0')
			{
				if (*c == '/' || *c == '\\')
				{
					f2 = f;
					f = c;
				}

				++c;
			}
			if (f2)
				return ++f2;
			else if (f)
				return ++f;
			else
				return path;
		}

		// Removed all the namespaces from an indentifier except the last two. 
		constexpr const char* functionName(const char* file)
		{
			const char* f2 = nullptr;
			const char* f = nullptr;
			const char* c = file;
			while (*c != '\0')
			{
				if (*c == ':')
				{
					++c;
					f2 = f;
					f = c;

				}

				++c;
			}
			if (f2)
				return ++f2;
			else if (f)
				return ++f;
			else
				return file;
		}
	}
}

#define PROFILE Utilities::Profiler_Start_Stop __FUNCTION__##_profile(Utilities::Profiler::get(), Utilities::EnsureHash<Utilities::hashString(__FUNCTION__)>::value, Utilities::Basename::functionName(__FUNCTION__), Utilities::Basename::fileName(__FILE__));
#define PROFILER_INIT Utilities::Profiler_Master::get()
#else
#define PROFILE
#endif
#endif