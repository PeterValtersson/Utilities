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

#pragma region Map_Functions
		
#pragma region Free_Functions

		// By value
		template <class Return, typename... Param>
		auto map(Return(func)(T, Param...), Param... args) noexcept -> optional<Return>
		{
			if (!this->has_value())
				return std::nullopt;
			return func(**this, std::forward<Param>(args)...);
		}		
		template <typename... Param>
		auto map(void(func)(T, Param...), Param... args) noexcept -> void
		{
			if (this->has_value())
				func(**this, std::forward<Param>(args)...);
			return;
		}

		// By reference
		template <class Return, typename... Param>
		auto map(Return(func)(T&, Param...), Param... args) noexcept -> optional<Return>
		{
			if (!this->has_value())
				return std::nullopt;
			return func(**this, std::forward<Param>(args)...);
		}
		template <typename... Param>
		auto map(void(func)(T&, Param...), Param... args) noexcept -> void
		{
			if (this->has_value())
				func(**this, std::forward<Param>(args)...);
			return;
		}

		// By const ref.
		template <class Return, typename... Param>
		auto map(Return(func)(T const&, Param...), Param... args) noexcept -> optional<Return>
		{
			if (!this->has_value())
				return std::nullopt;
			return func(**this, std::forward<Param>(args)...);
		}
		template <typename... Param>
		auto map(void(func)(T const&, Param...), Param... args) noexcept -> void
		{
			if (this->has_value())
				func(**this, std::forward<Param>(args)...);
			return;
		}

		// void
		template <class Return>
		auto map(Return(func)(void)) noexcept
		{
			if (!this->has_value())
				return optional<Return>(std::nullopt);
			return optional<Return>(func());
		}
		template <>
		auto map(void(func)(void)) noexcept
		{
			if (this->has_value())
				func();
			return;
		}

#pragma endregion Free Functions
	
#pragma region Methods

			template <class Return, typename... Param, class U = T>
			auto map(Return(U::*CMethod)(Param...), Param... args) noexcept -> typename std::enable_if<std::is_class<U>::value, optional<Return>>::type
			{
				if (!this->has_value())
					return std::nullopt;
				return (&this->value()->*CMethod)(std::forward<Param>(args)...);
			}
			template <typename... Param, class U = T>
			auto map(void(U::*CMethod)(Param...), Param... args) noexcept -> typename std::enable_if<std::is_class<U>::value>::type
			{
				if (this->has_value())
					(&this->value()->*CMethod)(std::forward<Param>(args)...);
				return;
			}
#pragma endregion Methods
#pragma region Methods Lambda

				
			template <class T>
			auto map(T const& lambda) noexcept
			{
				if (!this->has_value())
					return optional(std::nullopt);
				return optional(lambda(**this));
			}

#pragma endregion Methods Lambda

#pragma endregion Map_Functions
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