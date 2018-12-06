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
		template <class Return, typename... Param>
		auto map(Return(func)(T, Param...), Param... args) -> optional<Return>
		{

			if (!this->has_value())
				return std::nullopt;
			return func(**this, std::forward<Param>(args)...);
		}
//#pragma region Free_Functions
//
//		// By value
//		template <class Return>
//		auto map(Return(func)(T)) noexcept
//		{
//			if (!this->has_value())
//				return optional<Return>(std::nullopt);
//			return optional<Return>(func(**this));
//		}
//		
//		template <>
//		auto map(void(func)(T)) noexcept
//		{
//			if (this->has_value())
//				func(**this);
//			return;
//		}
//		template <typename... PARAMS>
//		auto map(void(func)(T, PARAMS...)) noexcept
//		{
//			if (this->has_value())
//				func(**this);
//			return;
//		}
//		// By reference
//		template <class Return>
//		auto map(Return(func)(T&)) noexcept
//		{
//			if (!this->has_value())
//				return optional<Return>(std::nullopt);
//			return optional<Return>(func(**this));
//		}
//		template <>
//		auto map(void(func)(T&)) noexcept
//		{
//			if (this->has_value())
//				func(**this);
//			return;
//		}
//
//		// By const ref.
//		template <class Return>
//		auto map(Return(func)(T const&)) noexcept
//		{
//			if (!this->has_value())
//				return optional<Return>(std::nullopt);
//			return optional<Return>(func(**this));
//		}
//		template <>
//		auto map(void(func)(T const&)) noexcept
//		{
//			if (this->has_value())
//				func(**this);
//			return;
//		}
//
//		// void
//		template <class Return>
//		auto map(Return(func)()) noexcept
//		{
//			if (!this->has_value())
//				return optional<Return>(std::nullopt);
//			return optional<Return>(func());
//		}
//		template <>
//		auto map(void(func)()) noexcept
//		{
//			if (this->has_value())
//				func();
//			return;
//		}
//
//#pragma endregion Free Functions
//	
//#pragma region Methods
//
//			template <class Return, class C>
//			auto map(Return(C::*CMethod)()) noexcept
//			{
//				if (!this->has_value())
//					return optional<Return>(std::nullopt);
//				return optional<Return>((&this->value()->*CMethod)());
//			}
//			template <class C>
//			auto map(void(C::*CMethod)()) noexcept
//			{
//				if (this->has_value())
//					(&this->value()->*CMethod)();
//				return;
//			}
//#pragma endregion Methods
			/*********** Lambda **************/

				/*
				template <class T>
				auto map(T const& lambda) noexcept
				{
					if (!this->has_value())
						return optional(std::nullopt);
					return optional(lambda(**this));
				}*/

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