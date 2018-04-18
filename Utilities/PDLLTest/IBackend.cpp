#include "IBackend.h"
#include "Backend.h"
namespace TT
{


	DLLE IBackend * CreateBackend()
	{
		return new Backend;
	}
}