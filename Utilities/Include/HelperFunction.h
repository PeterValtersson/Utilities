#ifndef _HELPER_FUNCTIONS_H_
#define _HELPER_FUNCTIONS_H_
#include <optional>
using index = ptrdiff_t;
template <typename Array, typename Type>
const std::optional<index> find(const Array& arr, const Type type)
{
	if (const auto result = std::find(std::begin(arr), std::end(arr), type); result != std::end(arr))
		return result - std::begin(arr);
	return std::nullopt;
}

#endif