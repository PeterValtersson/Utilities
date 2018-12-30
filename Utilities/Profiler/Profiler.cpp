#include <Profiler.h>

#pragma data_seg (".SHAREDMEMORY")
bool init = true;
std::shared_ptr<Utilities::Profiler_Master> profile_master = nullptr;
#pragma data_seg() 
#pragma comment(linker,"/SECTION:.SHAREDMEMORY,RWS")

std::shared_ptr<Utilities::Profiler_Master> Utilities::Profiler_Master::get()
{
	if (!profile_master)
		profile_master = std::shared_ptr<Profiler_Master>(new Profiler_Master());

	return profile_master;
}