#ifndef SE_DEV_CONSOLE_CONSOLE_H_
#define SE_DEV_CONSOLE_CONSOLE_H_

#include <map>
#include <functional>
#include <string>
namespace SE
{
	namespace DevConsole
	{
		/**
		*
		* @brief This is a class that you can use for a command console.
		*
		* @details Initialize console with Console::Initialize and give it a backend.
		*
		* @sa IConsoleBackend
		*
		**/

		class IConsole;

		typedef std::function<void(IConsole* console, int argc, char** argv)> DevConsole_Command;
		class Commands
		{
		public:
			struct Command_Structure
			{
				DevConsole_Command commandFunction;
				std::string name;
				std::string description;
			};

			static const int MAX_ARGUMENTS = 20;
			
			Commands();
			~Commands();

			/**
			* @brief	Add a command that can be called from the console.
			*
			* @details	A command is a lambda, a name, and a short description.
			*
			* @param[in] commandFunction The command to be called.
			* @param[in] name Name of the command.
			* @param[in] description A short description of the command.
			*
			* @retval 0 On success.
			*
			* @warning The commandFunction must follow the function declaration DevConsole_Command precisely.
			*
			* Example code:
			* @code
			* console->AddCommand([this](IConsole* backend, int argc, char** argv)
			* {
			*  backend->Print("Printing %d\n", this->d);
			* },
			* "write",
			* "prints d to the console");
			* @endcode
			*/
			int AddCommand(const DevConsole_Command& commandFunction, char* name, char* description);

			int RemoveCommand(const char* name);

			void InterpretCommand(IConsole* console,const char* command);

			void GetMap(std::map<size_t, Command_Structure>& commands);
		private:
		

			std::map<size_t, Command_Structure> commands;

			
			void ParseCommandString(char* command, int* argc, char** argv);
		};

	}
}
#endif // SE_DEV_CONSOLE_CONSOLE_H_