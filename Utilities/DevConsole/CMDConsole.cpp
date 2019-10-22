
#include <Console/CMDConsole.h>
#include <Windows.h>
#include <cstdarg>
#include <iostream>
using namespace std::chrono_literals;
//
//
//void Utilities::CMDConsole::Run()
//{
//	char buffer[256];
//	while (running)
//	{
//		Getline(buffer, 256);
//		commands.InterpretCommand(this, buffer);
//
//		std::this_thread::sleep_for(500ms);
//	}
//}


Utilities::CMDConsole::CMDConsole()noexcept : Console_Backend()
{
	start();
}


Utilities::CMDConsole::~CMDConsole()noexcept
{
}
//
//int Utilities::CMDConsole::Initialize()
//{
//	running = true;
//	myThread = std::thread(&CMDConsole::Run, this);
//
//	return 0;
//}
//
//void Utilities::CMDConsole::Shutdown()
//{
//	running = false;
//	myThread.join();
//}

void Utilities::CMDConsole::show()noexcept
{
	if (AllocConsole())
	{
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
	}

	print("<----||Console Initialized||---->\n\n");

}

void Utilities::CMDConsole::hide()noexcept
{
	FreeConsole();
}

bool Utilities::CMDConsole::is_visible()noexcept
{
	return false;
}

void Utilities::CMDConsole::set_visible( bool visible )noexcept
{
}


void Utilities::CMDConsole::print_w_channel( std::string_view channel, std::string_view format_string, ...)noexcept
{
	va_list args;
	va_start(args, format_string.data() );
	printf("%s: ", channel );
	vprintf( format_string.data(), args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}

void Utilities::CMDConsole::print( std::string_view format_string, ...)noexcept
{
	va_list args;
	va_start(args, format_string.data() );
	vprintf( format_string.data(), args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}/*
void Utilities::CMDConsole::VPrint(const char * line, va_list args)
{
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
}
void Utilities::CMDConsole::VPrint(const char* channel, const char * line, va_list args)
{
	printf("%s: ", channel);
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
}*/

const std::string Utilities::CMDConsole::get_input()noexcept
{
	std::string input;
	std::getline( std::cin, input );
	return input;
}

//size_t Utilities::CMDConsole::Getline(const char * buffer, size_t size)
//{
//	std::string in;
//	std::getline(std::cin, in);
//	_ASSERT(in.size() + 1 <= size);
//	memcpy((void*)buffer, in.c_str(), in.size());
//	memcpy((void*)(buffer + in.size()), "\0", 1);
//	return in.size();
//}
