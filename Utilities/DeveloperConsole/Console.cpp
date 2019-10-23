#include <Utilities/Console/Console.h>
#include "CMDConsole.h"

std::shared_ptr<Utilities::Console> Utilities::create_cmd_console()
{
	return std::make_shared<CMDConsole>();
}
