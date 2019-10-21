#include "head.h"
#include "../Include/Profiler/Profiler.h"

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