
#include "CMDConsole.h"
#include <Windows.h>
#include <cstdarg>
#include <iostream>
using namespace std::chrono_literals;


Utilities::CMDConsole::CMDConsole()noexcept : Console(), stream(nullptr)
{
	myThread = std::thread(&CMDConsole::run, this);
}


Utilities::CMDConsole::~CMDConsole()noexcept
{
	running = false;
	myThread.join();
}

void Utilities::CMDConsole::show()noexcept
{
	if (AllocConsole())
	{
		freopen_s(&stream, "conin$", "r", stdin);
		freopen_s( &stream, "conout$", "w", stdout);
		freopen_s( &stream, "conout$", "w", stderr);

		print( "<----||Console Initialized||---->\n\n" );
	}
}

void Utilities::CMDConsole::hide()noexcept
{
	FreeConsole();
	stream = nullptr;
}

bool Utilities::CMDConsole::is_visible()noexcept
{
	return stream != nullptr;
}

void Utilities::CMDConsole::clear() noexcept
{}


void Utilities::CMDConsole::print_w_channel( std::string_view channel, std::string_view format_string, ...)noexcept
{
	va_list args;
	va_start(args, format_string );
	printf("%s: ", channel.data() );
	vprintf( format_string.data(), args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}

void Utilities::CMDConsole::print( std::string_view format_string, ...)noexcept
{
	va_list args;
	va_start(args, format_string );
	vprintf( format_string.data(), args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}

const std::string Utilities::CMDConsole::get_input()noexcept
{
	std::string input;
	std::getline( std::cin, input );
	return input;
}
