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
		template <class Return, typename... Param>
		auto map(Return(func)(Param...), Param... args) noexcept
		{
			if (!this->has_value())
				return optional<Return>(std::nullopt);
			return optional<Return>(func(std::forward<Param>(args)...));
		}
		template <typename... Param>
		auto map(void(func)(Param...), Param... args) noexcept
		{
			if (this->has_value())
				func(std::forward<Param>(args)...);
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


		template <class T, typename... Param>
		auto map(T const& lambda, Param... args) noexcept
		{
			if (!this->has_value())
				return  optional<decltype(lambda(**this, args...))>(std::nullopt);
			return optional<decltype(lambda(**this, args...))>(lambda(**this, std::forward<Param>(args)...));
		}

#pragma endregion Methods Lambda

#pragma endregion Map_Functions

#pragma region Or_Else

#pragma region Free_Functions
		// void
		template <class Return, typename... Param>
		auto or_else(Return(func)(Param...), Param... args) noexcept
		{
			if (!this->has_value())
				func(std::forward<Param>(args)...);
			return *this;	
		}

#pragma endregion Free Functions

#pragma region Lambda
		template <class T, typename... Param>
		auto or_else(T const& lambda, Param... args)const noexcept
		{
			if (!this->has_value())
				lambda(std::forward<Param>(args)...);
			return *this;
		}
		template <class T, typename... Param>
		inline auto or_else_this(T const& lambda, Param... args)const noexcept
		{
			if (!this->has_value())
				return optional(lambda(std::forward<Param>(args)...));
			return *this;
		}
#pragma endregion Lambda
#pragma endregion Or_Else

		//template <class L1, class L2, typename... Param>
		//auto and_or_else(L1 const& lambdaTrue, L2 const& lambdaFalse, Param... args)const noexcept
		//{
		//	if (this->has_value())
		//		return optional(lambdaTrue(**this, std::forward<Param>(args)...));
		//	else
		//		lambdaFalse(std::forward<Param>(args)...);
		//	return *this;
		//}

	};

	using index = ptrdiff_t;
	template <typename Array, typename Type>
	optional<index> find(const Array& arr, const Type type)
	{
		if (const auto result = std::find(std::begin(arr), std::end(arr), type); result != std::end(arr))
			return result - std::begin(arr);
		return std::nullopt;
	}
}
#endif