#include <Utilities/Console/Commands.h>
#include <Windows.h>
#include <cstdarg>
#include <Utilities/Console/Console.h>
#include <algorithm>
#include <string>
#include <Utilities/StringUtilities.h>

const size_t hash_string( std::string_view str )
{
	return std::hash<std::string_view>{}(str);
}

Utilities::Commands::Commands()
{
	commands.emplace( hash_string( "commands" ),
					  Command_Structure{
						  "commands",
						  "Lists all available commands",
						  [this]( Console* const console, const std::vector<std::string>& args )
					  {
						  console->print( "\n\n" );
						  for ( const auto& c : commands )
							  console->print( "%s\t-\t%s\n", c.second.name, c.second.description );
						  console->print( "\n" );
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
void Utilities::Commands::parse_and_execute_command( Console* const console, std::string_view command_line_to_parse )noexcept
{
	const std::vector<std::string> args = String::split( command_line_to_parse );
	if ( args.size() > 0 )
		if ( auto command = commands.find( hash_string( args[0] ) ); command != commands.end() )
			command->second.commandFunction( console, args );
		else
			console->print( "Unknown command, %s\n\n", args[0] );

}

const std::map<size_t, Utilities::Commands::Command_Structure>& Utilities::Commands::get_commands()
{
	return this->commands;
}