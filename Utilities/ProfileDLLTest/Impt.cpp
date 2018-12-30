#include "head.h"
#include "../Include/Profiler.h"
Utilities::collector = nullptr;
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