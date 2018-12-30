#ifndef _UTILZ_COMPILE_TIME_STRING_H_
#define _UTILZ_COMPILE_TIME_STRING_H_
#include <stdint.h>
#include <cstddef>
#include <type_traits>


namespace Utilities
{

	using HashValue = uint32_t;

	static constexpr HashValue s_PrimeTable[] = {
		5039u, 5051u, 5059u, 5077u, 5081u, 5087u, 5099u, 5101u, 5107u,
		5113u, 5119u, 5147u, 5153u, 5167u, 5171u, 5179u, 5189u, 5197u,
		5209u, 5227u, 5231u, 5233u, 5237u, 5261u, 5273u, 5279u, 5281u
	};
	constexpr uint32_t s_PrimeNumCount = sizeof(s_PrimeTable) / sizeof(s_PrimeTable[0]);

	
	constexpr HashValue hashString(const char* toHash, size_t size)
	{
		HashValue hash = 0x811c9dc5;
		for (size_t i = 0; i < size; ++i) {
			uint8_t value = toHash[i];
			hash = hash ^ value;
			hash = HashValue(uint64_t(hash) * (s_PrimeTable[(static_cast<size_t>(toHash[i]) << 5) % s_PrimeNumCount]));
		}
		return hash;
	}
	template<size_t N>
	class ConstexprString
	{
		const char* string;
	public:
		
		constexpr ConstexprString(const char(&a)[N]) : string(a) {  };
		constexpr HashValue hash() { return hashString(string, N - 1); }
		constexpr char operator[](std::size_t n) const { return (n >= N-1) ? '\0' : string[n]; };
		constexpr std::size_t size()const { return N-1; };
		constexpr operator const char*()const { return string; };
	};

	template<size_t N>
	constexpr HashValue hashString(const ConstexprString<N>& toHash)
	{
		return hashString(toHash, static_cast<uint32_t>(toHash.size()));
	}
	template<size_t N>
	constexpr HashValue hashString(const char(&a)[N])
	{
		return hashString(a, N-1);
	}
	template<HashValue v>
	struct EnsureHash {
		static constexpr HashValue value = v;
	};
	template<size_t N>
	ConstexprString<N> CreateCString(const char(&a)[N])
	{
		return ConstexprString<N>(a);
	}
}

constexpr Utilities::HashValue operator "" _hash(const char* str, size_t size)
{
	return Utilities::hashString(str, size - 1);
}

#endif //_UTILZ_COMPILE_TIME_STRING_H_