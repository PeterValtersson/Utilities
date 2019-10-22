#ifndef SE_DEV_CONSOLE_ICONSOLE_BACKEND_H_
#define SE_DEV_CONSOLE_ICONSOLE_BACKEND_H_
#include <string_view>
#include "Commands.h"
namespace Utilities
{
	
	class Console_Backend {
	public:
		template<class T>
		struct Default_Converter{
			const T operator()( std::string_view str )
			{
				return str;
			}
		};

		virtual ~Console_Backend()noexcept
		{};

		virtual void show()noexcept = 0;
		virtual void hide()noexcept = 0;
		virtual bool is_visible()noexcept = 0;

		virtual void set_visible( bool visible )noexcept = 0;

		/**
		* @brief	Some helpers when drawing the console. You may use all or none.
		*
		**/
		virtual void begin_frame()noexcept
		{};
		virtual void frame()noexcept
		{};
		virtual void end_frame()noexcept
		{};

		virtual void clear()noexcept = 0;

		virtual void print_w_channel( std::string_view channel, std::string_view format_string, ... )noexcept = 0;
		virtual void print( std::string_view format_string, ... )noexcept = 0;
		/*virtual void VPrint( std::string_view format_string, va_list args ) = 0;
		virtual void VPrint( std::string_view channel, std::string_view format_string, va_list args ) = 0;
		*/
		virtual const std::string get_input( )noexcept = 0;
		template<class _TYPE, class _CONVERT = Default_Converter<_TYPE>>
		const _TYPE get_input( )
		{
			return _CONVERT{}(get_input());
		}

		/**
		* @brief	Add a command that can be called from the console.
		*
		* @details	A command is a lambda, a name, and a short description.
		*
		* @param[in] name Name of the command.
		* @param[in] description A short description of the command.
		* @param[in] lambda The command lambda to add.
		*
		*
		* @warning lambda must follow the declaration of Command_Lambda precisely.
		*
		* Example code:
		* @code
		* console->add_command([](Console_Backend& backend, const std::vector<const std::string>& args)
		* {
		*  backend.print("Hello World!");
		* },
		* "hello",
		* "prints \"Hello World!\" to the console");
		* @endcode
		*/
		inline void add_command( const std::string& name, const std::string& description, const Command_Lambda& lambda )
		{
			commands.add_command( name, description, lambda );
		}
		inline void remove_command( std::string_view name )
		{
			commands.remove_command( name );
		}
	protected:
		Console_Backend()noexcept
		{}
		Console_Backend( const Console_Backend& other ) = delete;
		Console_Backend( const Console_Backend&& other ) = delete;
		Console_Backend& operator=( const Console_Backend& other ) = delete;

		Commands commands;
	};
}

#endif