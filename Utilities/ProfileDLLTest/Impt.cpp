#include "head.h"
#include "../Include/Profiler.h"

void DoStuff2()
{
	Profile;
}

void Main_Entry()
{
	Profile;
	DoStuff2();
	DoStuff2();
}