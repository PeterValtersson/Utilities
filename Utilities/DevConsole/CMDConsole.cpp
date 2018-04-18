
#include <CMDConsole.h>
#include <Windows.h>
#include <cstdarg>
#include <iostream>

void Utilities::CMDConsole::Run()
{
	using namespace std::chrono_literals;

	char buffer[256];
	while (running)
	{
		Getline(buffer, 256);
		commands.InterpretCommand(this, buffer);

		std::this_thread::sleep_for(500ms);
	}
}


Utilities::CMDConsole::CMDConsole()
{
}


Utilities::CMDConsole::~CMDConsole()
{
}

int Utilities::CMDConsole::Initialize()
{
	running = true;
	myThread = std::thread(&CMDConsole::Run, this);

	return 0;
}

void Utilities::CMDConsole::Shutdown()
{
	running = false;
	myThread.join();
}

void Utilities::CMDConsole::Show()
{
	if (AllocConsole())
	{
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
	}

	Print("<----||Console Initialized||---->\n\n");

}

void Utilities::CMDConsole::Hide()
{
	FreeConsole();
}

bool Utilities::CMDConsole::IsVisible()
{
	return false;
}

void Utilities::CMDConsole::Toggle()
{
}

void Utilities::CMDConsole::BeginFrame()
{
}

void Utilities::CMDConsole::Frame()
{
}

void Utilities::CMDConsole::EndFrame()
{
}

void Utilities::CMDConsole::Clear()
{
}

void Utilities::CMDConsole::PrintChannel(const char * line, const char * channel, ...)
{
	va_list args;
	va_start(args, line);
	printf("%s: ", channel);
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}

void Utilities::CMDConsole::Print(const char * line, ...)
{
	va_list args;
	va_start(args, line);
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}
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
}

void Utilities::CMDConsole::Getline(std::string& string)
{
	std::getline(std::cin, string);
}

size_t Utilities::CMDConsole::Getline(const char * buffer, size_t size)
{
	std::string in;
	std::getline(std::cin, in);
	_ASSERT(in.size() + 1 <= size);
	memcpy((void*)buffer, in.c_str(), in.size());
	memcpy((void*)(buffer + in.size()), "\0", 1);
	return in.size();
}

int Utilities::CMDConsole::AddCommand(const DevConsole_Command & commandFunction, char * name, char * description)
{
	return commands.AddCommand(commandFunction, name, description);
}

int Utilities::CMDConsole::RemoveCommand(const char* name)
{
	return 0;
}

void Utilities::CMDConsole::AddFrameCallback(const std::function<void()>& frameCallback)
{
}

void * Utilities::CMDConsole::GetContext()
{
	return nullptr;
}
