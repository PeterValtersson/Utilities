#ifndef _UTILZ_GUID_H_
#define _UTILZ_GUID_H_
#include "CompileTimeString.h"
#include <string>

namespace Utilities
{
	struct GUID
	{
		struct Compare
		{
			inline bool operator() (const GUID& lhs, const GUID& rhs) const
			{
				return lhs.id < rhs.id;
			}
		};

		struct Hasher
		{
			inline StringHash operator()(const GUID& g) const
			{
				return g.id;
			}
		};


		GUID() : id(0) {};
		GUID(StringHash idi) : id(idi) { };
		GUID(const std::string& str) : id(hashString(str.c_str(), uint32_t(str.size()))) { };
		template<std::size_t N>
		constexpr GUID(const char(&a)[N]) : id(hashString(a, N - 1)) { };
		GUID(const GUID& other) : id(other.id) {}
		GUID(const GUID&& other) : id(other.id) {}
		bool operator!=(const GUID& other) const { return id != other.id; }
		bool operator==(const GUID& other) const { return id == other.id; }
		GUID& operator=(const GUID& other) { this->id = other.id; return *this; }
		constexpr operator StringHash() { return id; };
		GUID operator+(const GUID &other) const
		{
			return id ^ other.id;
		}
		StringHash id;
	};


}


#endif //SE_UTILZ_GUID_H_