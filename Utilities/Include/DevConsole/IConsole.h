#ifndef SE_DEV_CONSOLE_ICONSOLE_BACKEND_H_
#define SE_DEV_CONSOLE_ICONSOLE_BACKEND_H_
#include <string>
#include "Commands.h"
namespace SE
{
	namespace DevConsole
	{
		
		class IConsole
		{
		public:
			virtual ~IConsole() {};

			/**
			* @brief	Initialize the console backend
			*
			* @retval 0 On success
			*
			*/
			virtual int Initialize() = 0;
			virtual void Shutdown() = 0;

			virtual void Show() = 0;
			virtual void Hide() = 0;
			virtual bool IsVisible() = 0;
			/*
			* @brief Toggles the console on or off.
			*/
			virtual void Toggle() = 0;
			/*
			* @brief Start the frame.
			*/
			virtual void BeginFrame() = 0;
			/*
			* @brief Draws the console if the console is visible.
			*/
			virtual void Frame() = 0;
			/*
			* @brief Ends the frame.
			*/
			virtual void EndFrame() = 0;

			/*
			* @brief Clears messages printed in the console.
			*/
			virtual void Clear() = 0;

			virtual void PrintChannel(const char* channel, const char* line, ...) = 0;
			virtual void Print(const char* line, ...) = 0;
			virtual void VPrint(const char* line, va_list args) = 0;
			virtual void VPrint(const char* channel, const char* line, va_list args) = 0;

			virtual void Getline(std::string& string) = 0;
			virtual size_t Getline(const char* buffer, size_t size) = 0;

			/**
			* @brief	Add a callback that will be called each frame.
			*
			**/
			virtual void AddFrameCallback(const std::function<void()>& frameCallback) = 0;

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
			* console->AddCommand([this](IConsoleBackend* backend, int argc, char** argv)
			* {
			*  backend->Print("Printing %d\n", this->d);
			* },
			* "write",
			* "prints d to the console");
			* @endcode
			*/
			virtual int AddCommand(const DevConsole_Command& commandFunction, char* name, char* description) = 0;
			
			/* 
			 * @brief Removes a previously added command.
			 * @param[in] name Name of the command to remove
			 * @retval 0 On success
			 * @retval -1 On Failure
			 */
			virtual int RemoveCommand(const char* name) = 0;


			virtual void* GetContext() = 0;
		protected:
			IConsole() {};
			IConsole(const IConsole& other) = delete;
			IConsole(const IConsole&& other) = delete;
			IConsole& operator=(const IConsole& other) = delete;

		};
	}
}

#endif //SE_UTILZ_ICONSOLE_BACKEND_H_