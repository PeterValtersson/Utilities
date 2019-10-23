#include "head.h"
#include <Utilities/Profiler/Profiler.h>

void DoStuff2()
{
	PROFILE;
}

void Main_Entry()
{
	PROFILE;
	DoStuff2();
	DoStuff2();
}