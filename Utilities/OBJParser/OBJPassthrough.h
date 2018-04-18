// Passthough that accepts .obj format and turns it into .arf

#pragma once
#include <stdint.h>

#include "ObjParser.h"

#define DLL_EXPORT extern "C" _declspec(dllexport) 
DLL_EXPORT int32_t Parse(uint32_t guid, void* data, uint64_t size, void** parsedData, uint64_t* parsedSize);
DLL_EXPORT int32_t Destroy(uint32_t guid, void* data, uint64_t size);