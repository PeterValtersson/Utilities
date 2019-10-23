#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Include/Console/Console.h"
#include <thread>
#include <cstdarg>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	class TestConsole : public Utilities::Console{
	public:
		TestConsole()noexcept : ready( false )
		{
			myThread = std::thread( &TestConsole::run, this );
		}
		~TestConsole()noexcept
		{
			ready = true;
			running = false;
			myThread.join();
		}

		void show()noexcept override
		{}
		void hide()noexcept override
		{}
		bool is_visible()noexcept  override
		{
			return true;
		}

		void clear()noexcept  override
		{}


		void print_w_channel( std::string_view channel, std::string_view format_string, ... )noexcept override
		{}
		void print( std::string_view format_string, ... )noexcept override
		{
			va_list args;
			va_start( args, format_string );
			str.resize( 256 );
			str.resize(vsprintf_s( &str[0], str.size(), format_string.data(), args ));
			va_end( args );
			ready = true;
		}

		const std::string get_input()noexcept override
		{
			while ( !ready );
			ready = false;
			return str;
		}

	private:
		std::string str;
		std::thread myThread;
		bool ready;
	};

	TEST_CLASS( ConsoleTests ){
	public:
		TEST_METHOD( Create_cmd_console )
		{
			auto console = Utilities::create_cmd_console();
		}
		TEST_METHOD( Show_cmd_console )
		{
			auto console = Utilities::create_cmd_console();
			console->show();
		}
		TEST_METHOD( Exit_Command )
		{
			volatile bool run = true;
			TestConsole c;
			c.add_command( "exit", "Exit app", [&run]( Utilities::Console* c, Utilities::Args args )
			{
				run = false;

			} );
			while ( run )
				c.print( "exit" );
		}
		TEST_METHOD( Command_With_arg )
		{
			TestConsole c;
			Utilities::Console* pc = &c;
			int arg = 1337;
			int argr = 0;
			volatile bool finished = false;
			pc->add_command( "carg", "Command with arg", [&]( Utilities::Console* c, Utilities::Args args )
			{
				argr = std::stoi( args[1] );
				finished = true;
			} );
			
			pc->print( "carg %d", arg );
			while ( !finished );
			Assert::AreEqual( arg, argr );
			
		}
	};
}