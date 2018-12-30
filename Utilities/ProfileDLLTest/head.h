#pragma once
#ifdef DECLSPEC_PROFILE_TEST_DECL
#define DECLSPEC_PROFILE_TEST __declspec(dllexport)
#else
#define DECLSPEC_PROFILE_TEST __declspec(dllimport)
#endif


DECLSPEC_PROFILE_TEST void Main_Entry();