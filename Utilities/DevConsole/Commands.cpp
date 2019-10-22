#include <Console/Commands.h>
#include <Windows.h>
#include <cstdarg>
#include <Console/Console_Backend.h>
#include <algorithm>
#include <string>

constexpr size_t hash_string( std::string_view str )
{
	return std::hash<std::string_view>{}(str);
}

Utilities::Commands::Commands()
{

	commands.emplace( hash_string( "commands" ),
					  Command_Structure{
						  "commands",
						  "Lists all available commands",
						  [this]( Console_Backend& backend, const std::vector<const std::string>& args )
					  {
						  backend.print( "\n\n" );
						  for ( const auto& c : commands )
							  backend.print( "%s\t-\t%s\n", c.second.name, c.second.description );
						  backend.print( "\n" );
					  }
					  } );
}
Utilities::Commands::~Commands()
{}

void Utilities::Commands::add_command( const std::string& name, const std::string& description, const Command_Lambda& lambda )
{
	commands.emplace( hash_string( name ), Command_Structure{ name ,description, lambda } );
}

void Utilities::Commands::remove_command( std::string_view name )
{
	commands.erase( hash_string( name ) );
}
void Utilities::Commands::parse_and_execute_command( std::string_view command_line_to_parse )noexcept
{
	std::vector<const std::string> args;
	std::spli
	int argc = 0;
	char* argv[MAX_ARGUMENTS];
	std::string c = commandd;
	char* command = new char[c.size() + 1];
	memcpy( command, commandd, c.size() );
	command[c.size()] = '\0';
	ParseCommandString( command, &argc, argv );

	if ( argc > 0 )
	{
		auto hash = std::hash<std::string>{}(argv[0]);

		auto find = commands.find( hash );
		if ( find != commands.end() )
			find->second.commandFunction( console, argc, argv );
		else
			console->print( "Unknown command, %s\n\n", argv[0] );
	}
	delete[] command;
}

const std::map<size_t, Utilities::Commands::Command_Structure> Utilities::Commands::get_commands()
{
	commands = this->commands;
}

void Utilities::Commands::ParseCommandString( char* command, int* argc, char** argv )
{
	*argc = 0;
	int j = 0;
	int i = 0;
	char c = command[i];
	while ( c != '\0' )
	{
		while ( !(c == ' ' || c == '\0') )
		{
			i++;
			c = command[i];
		}
		argv[*argc] = &command[j];
		(*argc)++;
		if ( command[i] != '\0' )
		{
			command[i] = '\0';
			i++;
			j = i;
			c = command[i];
			if ( *argc >= MAX_ARGUMENTS )
			{
				break;
			}

		}


	}
}