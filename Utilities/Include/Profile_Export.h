#ifndef _DLL_EXPORT_PROFILER_H_
#define _DLL_EXPORT_PROFILER_H_
#ifdef _PROFILER_EXPORT_
#define DECLSPEC_PROFILER __declspec(dllexport)
#else
#define DECLSPEC_PROFILER __declspec(dllimport)
#endif
#endif