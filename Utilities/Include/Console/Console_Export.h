#ifndef _DLL_EXPORT_CONSOLE_H_
#define _DLL_EXPORT_CONSOLE_H_
#ifdef _CONSOLE_EXPORT_
#define DECLSPEC_CONSOLE __declspec(dllexport)
#else
#define DECLSPEC_CONSOLE __declspec(dllimport)
#endif
#endif