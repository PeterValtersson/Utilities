#pragma once

#ifdef DLL_EXPORT
#define DLLE __declspec(dllexport)
#else
#define DLLE __declspec(dllimport)
#endif

namespace TT
{


	class IBackend
	{
	public:
		virtual	~IBackend() {};

		virtual int DoSomething(int asd) = 0;

		virtual void DoSomethingMore() = 0;
	protected:
		IBackend() {};

	};

	DLLE IBackend* CreateBackend();
}
