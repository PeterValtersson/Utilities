#include "Backend.h"
#include "../Profiler/Profiler.h"
#include <iostream>
namespace TT
{


	Backend::Backend()
	{
	}


	Backend::~Backend()
	{
	}

	int Backend::DoSomething(int asd)
	{
		StartProfile;
		return (asd*(asd^asd) - asd) | asd&(1 - asd) ^ 3;
	}

	void Backend::DoSomethingMore()
	{

		StartProfile;
		std::cout << "Hello World" << std::endl;
	}
}