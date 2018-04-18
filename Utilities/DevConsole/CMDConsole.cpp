
#include <CMDConsole.h>
#include <Windows.h>
#include <cstdarg>
#include <iostream>

void SE::DevConsole::CMDConsole::Run()
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


SE::DevConsole::CMDConsole::CMDConsole()
{
}


SE::DevConsole::CMDConsole::~CMDConsole()
{
}

int SE::DevConsole::CMDConsole::Initialize()
{
	running = true;
	myThread = std::thread(&CMDConsole::Run, this);

	return 0;
}

void SE::DevConsole::CMDConsole::Shutdown()
{
	running = false;
	myThread.join();
}

void SE::DevConsole::CMDConsole::Show()
{
	if (AllocConsole())
	{
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
	}

	Print("<----||Console Initialized||---->\n\n");

}

void SE::DevConsole::CMDConsole::Hide()
{
	FreeConsole();
}

bool SE::DevConsole::CMDConsole::IsVisible()
{
	return false;
}

void SE::DevConsole::CMDConsole::Toggle()
{
}

void SE::DevConsole::CMDConsole::BeginFrame()
{
}

void SE::DevConsole::CMDConsole::Frame()
{
}

void SE::DevConsole::CMDConsole::EndFrame()
{
}

void SE::DevConsole::CMDConsole::Clear()
{
}

void SE::DevConsole::CMDConsole::PrintChannel(const char * line, const char * channel, ...)
{
	va_list args;
	va_start(args, line);
	printf("%s: ", channel);
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}

void SE::DevConsole::CMDConsole::Print(const char * line, ...)
{
	va_list args;
	va_start(args, line);
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}
void SE::DevConsole::CMDConsole::VPrint(const char * line, va_list args)
{
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
}
void SE::DevConsole::CMDConsole::VPrint(const char* channel, const char * line, va_list args)
{
	printf("%s: ", channel);
	vprintf(line, args);
	printf("\n");
	fflush(stdout);
}

void SE::DevConsole::CMDConsole::Getline(std::string& string)
{
	std::getline(std::cin, string);
}

size_t SE::DevConsole::CMDConsole::Getline(const char * buffer, size_t size)
{
	std::string in;
	std::getline(std::cin, in);
	_ASSERT(in.size() + 1 <= size);
	memcpy((void*)buffer, in.c_str(), in.size());
	memcpy((void*)(buffer + in.size()), "\0", 1);
	return in.size();
}

int SE::DevConsole::CMDConsole::AddCommand(const DevConsole_Command & commandFunction, char * name, char * description)
{
	return commands.AddCommand(commandFunction, name, description);
}

int SE::DevConsole::CMDConsole::RemoveCommand(const char* name)
{
	return 0;
}

void SE::DevConsole::CMDConsole::AddFrameCallback(const std::function<void()>& frameCallback)
{
}

void * SE::DevConsole::CMDConsole::GetContext()
{
	return nullptr;
}
