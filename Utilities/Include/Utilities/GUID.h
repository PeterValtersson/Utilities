#ifndef _UTILZ_GUID_H_
#define _UTILZ_GUID_H_
#include "CompileTimeString.h"
#include <string>
#include <string_view>

namespace Utilities
{
	struct GUID{
		struct Compare{
			constexpr bool operator() ( const GUID& lhs, const GUID& rhs ) const
			{
				return lhs.id < rhs.id;
			}
		};
		struct Hasher{
			constexpr StringHash operator()( const GUID& g ) const
			{
				return g.id;
			}
		};
		constexpr GUID() : id( 0 )
		{};
		GUID( const StringHash idi )noexcept : id( idi )
		{};
		GUID( std::string_view str )noexcept : id( hashString( str.data(), uint32_t( str.size() ) ) )
		{};
		template<std::size_t N>
		constexpr GUID( const char( &a )[N] )noexcept : id( hashString( a ) )
		{};
		constexpr bool operator!=( const GUID& other )const noexcept
		{
			return id != other.id;
		}
		constexpr bool operator==( const GUID& other )const noexcept
		{
			return id == other.id;
		}
		GUID& operator=( std::string_view str )noexcept
		{
			this->id = hashString( str.data(), str.size() ); return *this;
		}
		constexpr operator StringHash()noexcept
		{
			return id;
		};
		const GUID operator+( const GUID& other )const noexcept
		{
			return id ^ other.id;
		}
		bool is_valid()const noexcept 
		{
			return id > 0;
		}
		std::string to_string()const
		{
			return std::to_string( id );
		}
		StringHash id;
	};
}


#endif //SE_UTILZ_GUID_H_