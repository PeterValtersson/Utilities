#ifndef SE_DEV_CONSOLE_COMMANDS_H_
#define SE_DEV_CONSOLE_COMMANDS_H_

#include <map>
#include <functional>
#include <string>
namespace Utilities
{
	class Console;

	typedef std::function<void( Console* const console, const std::vector<std::string> & args )> Command_Lambda;
	class Commands{
	public:
		struct Command_Structure{
			std::string name;
			std::string description;
			Command_Lambda commandFunction;
		};

		static const int MAX_ARGUMENTS = 20;

		Commands();
		~Commands();

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
		* console->add_command([](Console& backend, const std::vector<const std::string>& args)
		* {
		*  backend.print("Hello World!");
		* },
		* "hello",
		* "prints \"Hello World!\" to the console");
		* @endcode
		*/
		void add_command( const std::string& name, const std::string& description, const Command_Lambda& lambda );
		void remove_command( std::string_view name );

		void parse_and_execute_command( Console* const console, std::string_view command_line_to_parse )noexcept;

		const std::map<size_t, Command_Structure>& get_commands();
	private:
		std::map<size_t, Command_Structure> commands;
	};

}
#endif // SE_DEV_CONSOLE_CONSOLE_H_