#include <Commands.h>
#include <Windows.h>
#include <cstdarg>
#include <IConsole.h>

void SE::DevConsole::Commands::InterpretCommand(IConsole* console, const char * commandd)
{
	int argc = 0;
	char* argv[MAX_ARGUMENTS];
	std::string c = commandd;
	char* command = new char[c.size() + 1];
	memcpy(command, commandd, c.size());
	command[c.size()] = '\0';
	ParseCommandString(command, &argc, argv);

	if (argc > 0)
	{
		auto hash = std::hash<std::string>{}(argv[0]);

		auto find = commands.find(hash);
		if (find != commands.end())
			find->second.commandFunction(console, argc, argv);
		else
			console->Print("Unknown command, %s\n\n", argv[0]);
	}
	delete[] command;
}

void SE::DevConsole::Commands::GetMap(std::map<size_t, Command_Structure>& commands)
{
	commands = this->commands;
}

void SE::DevConsole::Commands::ParseCommandString(char * command, int * argc, char ** argv)
{
	*argc = 0;
	int j = 0;
	int i = 0;
	char c = command[i];
	while (c != '\0')
	{
		while (!(c == ' ' || c == '\0'))
		{
			i++;
			c = command[i];
		}
		argv[*argc] = &command[j];
		(*argc)++;
		if (command[i] != '\0')
		{
			command[i] = '\0';
			i++;
			j = i;
			c = command[i];
			if (*argc >= MAX_ARGUMENTS)
			{
				break;
			}

		}


	}
}

SE::DevConsole::Commands::Commands()
{
	commands[std::hash<std::string>{}("commands")] =
	{
		[this](IConsole* backend, int argc, char** argv) 
	{
		
		backend->Print("\n\n");
		for (auto& c : commands)
		{		
			backend->Print("%s\t-\t%s\n", c.second.name.c_str(), c.second.description.c_str());
		}
		backend->Print("\n");
	},
		"commands",
		"Lists all available commands"
	};
}
SE::DevConsole::Commands::~Commands()
{
}

/*Hash the identifier and add the command*/
int SE::DevConsole::Commands::AddCommand(const DevConsole_Command& commandFunction, char * name, char * description)
{
	auto hash = std::hash<std::string>{}(name);
	commands[hash] = { commandFunction, name ,description };
	return 0;
}

int SE::DevConsole::Commands::RemoveCommand(const char* name)
{
	auto hash = std::hash<std::string>{}(name);
	auto find = commands.find(hash);
	if (find != commands.end())
	{
		commands.erase(find);
		return 0;
	}
	return -1;
}
