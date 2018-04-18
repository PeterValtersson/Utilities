#ifndef _UTILZ_COMPILE_TIME_STRING_H_
#define _UTILZ_COMPILE_TIME_STRING_H_
#include <stdint.h>
#include <cstddef>

	namespace Utilz
	{


		static constexpr uint32_t s_PrimeTable[] = {
			5039u, 5051u, 5059u, 5077u, 5081u, 5087u, 5099u, 5101u, 5107u,
			5113u, 5119u, 5147u, 5153u, 5167u, 5171u, 5179u, 5189u, 5197u,
			5209u, 5227u, 5231u, 5233u, 5237u, 5261u, 5273u, 5279u, 5281u
		};

		using HashValue = uint32_t;

		static constexpr uint32_t s_PrimeNumCount = sizeof(s_PrimeTable) / sizeof(s_PrimeTable[0]);
#define NULL_HASH 0

		namespace StringHash
		{

			class ConstexprString
			{
				const char* p;
				std::size_t sz;
			public:
				template<std::size_t N>
				constexpr ConstexprString(const char(&a)[N]) : p(a), sz(N - 1) {  };
				constexpr char operator[](std::size_t n) const { return (n >= sz) ? '\0' : p[n]; };
				constexpr std::size_t size()const { return sz; };
				constexpr operator const char*()const { return p; };
			};


			constexpr HashValue GetHash_ConstexprString(const char* toHash, uint32_t size)
			{
			/*	HashValue hash = NULL_HASH;

				for (uint32_t i = 0; i < size; i++)
				{
					hash += static_cast<uint32_t>(toHash[i]);
					hash += hash << 10;
					hash += hash >> 6;
					hash ^= hash * (s_PrimeTable[(static_cast<uint32_t>(toHash[i]) << 5) % s_PrimeNumCount]);
				}
				return hash;
*/

				//64
				/* uint64_t hash = 0xcbf29ce484222325;
				  uint64_t prime = 0x100000001b3;*/

				HashValue hash = 0x811c9dc5;
				//HashValue prime = 0x1000193;

				for (uint32_t i = 0; i < size; ++i) {
					uint8_t value = toHash[i];
					hash = hash ^ value;
					hash *= (s_PrimeTable[(static_cast<uint32_t>(toHash[i]) << 5) % s_PrimeNumCount]);
				}

				return hash;
			}

			constexpr HashValue GetHash_ConstexprString(const ConstexprString& toHash)
			{
				return GetHash_ConstexprString(toHash, static_cast<uint32_t>(toHash.size()));
			}
		}


		class ConstexprStringAndHash
		{
			StringHash::ConstexprString string;
			HashValue hash;
		public:
			struct Hasher
			{
				inline HashValue operator()(const ConstexprStringAndHash& g) const
				{
					return g.hash;
				}
			};
			HashValue ID()const { return hash; }
			template<std::size_t N>
			constexpr ConstexprStringAndHash(const char(&a)[N]) : string(a) { hash = StringHash::GetHash_ConstexprString(string); };
			operator const char*()const { return string; };
			const char* c_str() const { return string; };

		};
	}
	constexpr Utilz::HashValue operator "" _hash(const char* toHash, std::size_t size)
	{
		return Utilz::StringHash::GetHash_ConstexprString(toHash, static_cast<uint32_t>(size));
	}


#endif //_UTILZ_COMPILE_TIME_STRING_H_