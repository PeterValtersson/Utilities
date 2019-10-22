#include <Profiler/Profiler_Master.h>
#include <filesystem>
#include <StringUtilities.h>
#include <TimeUtilities.h>
#include <stdlib.h>
#include <fstream>
#include <MonadicOptional.h>

#pragma data_seg (".SHAREDMEMORY")
std::shared_ptr<Utilities::Profiler_Master> profile_master = nullptr;
#pragma data_seg() 
#pragma comment(linker,"/SECTION:.SHAREDMEMORY,RWS")

std::shared_ptr<Utilities::Profiler_Master> Utilities::Profiler_Master::get()noexcept
{
	if (!profile_master)
		profile_master = std::shared_ptr<Profiler_Master>(new Profiler_Master());

	return profile_master;
}

std::shared_ptr<Utilities::Profiler> Utilities::Profiler_Master::get_profiler(std::thread::id threadID)noexcept
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

const std::string Utilities::Profiler_Master::to_str() noexcept
{
	std::stringstream ss;
	profilers.operate([&](auto& ps)
		{
			ss << std::endl;
			for (auto profiler : ps)
			{
				ss << "Profile Thread " << profiler.threadID << std::endl;
				ss << profiler.profiler->to_str(1) << std::endl;
			}
		});
	return ss.str();
}

const int Utilities::Profiler_Master::generate_tree(const std::string& folder, bool convertDotToPdf, std::string_view unit) noexcept
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
				ss << profiler.profiler->generate_tree() << std::endl;
				ss << "}" << std::endl;
			}


		});


	ss << std::endl << "}" << std::endl;


	std::filesystem::create_directories(folder);
	std::stringstream filename;
	filename << folder << "/Profile_" << Time::currentDateTime();
	auto strFilename = String::replaceAll( String::replaceAll(filename.str(), " ", "_"), ":", "-");
	std::ofstream out(strFilename + ".dot");
	if (!out.is_open())
		return 1 << 16;

	out << ss.str();
	out.close();
	if (convertDotToPdf)
	{
		std::stringstream cmd;
		cmd << "dot -Tpdf " << strFilename << ".dot" << " -o " << strFilename << ".pdf";
		int ret = system(cmd.str().c_str());
		if (ret == 0)
		{
			std::stringstream cmd;
			cmd << "del /Q /F " << strFilename << ".dot";
			ret = system(cmd.str().c_str());
		}
		return ret;
	}
	return 0;
}