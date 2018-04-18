#pragma once
#include "IBackend.h"
namespace TT
{


	class Backend : public IBackend
	{
	public:
		Backend();
		~Backend();

		int DoSomething(int asd);
		void DoSomethingMore();
	};

}