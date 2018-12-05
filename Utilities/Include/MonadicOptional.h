#ifndef _HELPER_FUNCTIONS_H_
#define _HELPER_FUNCTIONS_H_
#include <optional>
#include <functional>
#include <type_traits>

namespace Utilities
{
	template <class T>
	class optional : public std::optional<T> {
	public:
		optional() : std::optional<T>() {} 
		optional(std::nullopt_t nullopt) : std::optional<T>(nullopt) {}
		optional(T && v) : std::optional<T>(v) {}
		optional(T const& v) : std::optional<T>(v) {}

		template <class Return>
		optional<Return> map(Return(func)(T)) noexcept
		{
			if (!this->has_value())
				return std::nullopt;
			return func(**this);
		}
		template <class Return, class C>
		optional<Return> map(Return(C::*CMethod)()) noexcept
		{
			if (!this->has_value())
				return std::nullopt;
			return (&this->value()->*CMethod)();
		}
		template <class T>
		auto map(T const& lambda) noexcept
		{
			if (!this->has_value())
				return optional(std::nullopt);
			return optional(lambda(**this));
		}
	};

	using index = ptrdiff_t;
	template <typename Array, typename Type>
	const std::optional<index> find(const Array& arr, const Type type)
	{
		if (const auto result = std::find(std::begin(arr), std::end(arr), type); result != std::end(arr))
			return result - std::begin(arr);
		return std::nullopt;
	}
}
#endif