#ifndef _UTILZ_COMPILE_TIME_STRING_H_
#define _UTILZ_COMPILE_TIME_STRING_H_
#include <stdint.h>
#include <cstddef>


namespace Utilities
{


	static constexpr uint32_t s_PrimeTable[] = {
		5039u, 5051u, 5059u, 5077u, 5081u, 5087u, 5099u, 5101u, 5107u,
		5113u, 5119u, 5147u, 5153u, 5167u, 5171u, 5179u, 5189u, 5197u,
		5209u, 5227u, 5231u, 5233u, 5237u, 5261u, 5273u, 5279u, 5281u
	};
	constexpr uint32_t s_PrimeNumCount = sizeof(s_PrimeTable) / sizeof(s_PrimeTable[0]);

	using HashValue = uint32_t;

	constexpr HashValue hashString(const char* toHash, size_t size)
	{
		HashValue hash = 0x811c9dc5;
		for (size_t i = 0; i < size; ++i) {
			uint8_t value = toHash[i];
			hash = hash ^ value;
			hash *= (s_PrimeTable[(static_cast<uint32_t>(toHash[i]) << 5) % s_PrimeNumCount]);
		}
		return hash;
	}
	class ConstexprString
	{
		const char* string;
		size_t stringSize;
	public:
		struct Hasher
		{
			constexpr inline HashValue operator()(const ConstexprString& g) const
			{
				return g.hash();
			}
		};
		constexpr HashValue hash()const { return hashString(string, stringSize); }
		template<size_t N>
		constexpr ConstexprString(const char(&a)[N]) : p(a), stringSize(N - 1) {  };
		constexpr ConstexprString(const char* pS, const size_t size) : string(pS), stringSize(size - 1) {  };
		constexpr char operator[](std::size_t n) const { return (n >= stringSize) ? '\0' : string[n]; };
		constexpr std::size_t size()const { return stringSize; };
		constexpr operator const char*()const { return string; };
	};

	constexpr HashValue hashString(const ConstexprString& toHash)
	{
		return hashString(toHash, static_cast<uint32_t>(toHash.size()));
	}
}
constexpr Utilities::ConstexprString operator "" _cString(const char* toHash, std::size_t size)
{
	return Utilities::ConstexprString(toHash, size);
}
constexpr Utilities::HashValue operator "" _hash(const char* toHash, std::size_t size)
{
	return Utilities::ConstexprString(toHash, size).hash();
}

#endif //_UTILZ_COMPILE_TIME_STRING_H_