#ifndef SE_DEV_CONSOLE_CMD_CONSOLE_H_
#define SE_DEV_CONSOLE_CMD_CONSOLE_H_
#include <Console/Console_Backend.h>
#include <mutex>
#include <thread>
namespace Utilities
{
	/**
	*
	* @brief A console backend that starts a seperate CMD window.
	*
	* @warning Show/Hide functions may not function as intended.
	*
	**/
	class CMDConsole : public Console_Backend
	{
	public:
		CMDConsole()noexcept;
		~CMDConsole()noexcept;

		void show()noexcept override;
		void hide()noexcept override;
		bool is_visible()noexcept  override;
		void set_visible( bool visible )noexcept  override;
	
		void clear()noexcept  override;


		void print_w_channel( std::string_view channel, std::string_view format_string, ...)noexcept override;
		void print( std::string_view format_string, ...)noexcept override;
		/*void VPrint(const char* line, va_list args)override;
		void VPrint(const char* channel, const char* line, va_list args)override;*/

		const std::string get_input()noexcept override;
	private:
		void Run();
		bool running;

		std::thread myThread;

		Commands commands;
	};
}
#endif //SE_DEV_CONSOLE_CMD_CONSOLE_H_