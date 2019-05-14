#pragma once
#include <type_traits>
constexpr std::size_t operator "" _kb( std::size_t bytes )
{
	return bytes / 1024;
}
constexpr std::size_t operator "" _mb( std::size_t bytes )
{
	return bytes / 1024*1024;
}
constexpr std::size_t operator "" _gb( std::size_t bytes )
{
	return bytes / 1024 * 1024 * 1024;
}