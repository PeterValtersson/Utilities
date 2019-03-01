#ifndef _UTILITIES_FSTREAM_HELPERS_H_
#define _UTILITIES_FSTREAM_HELPERS_H_
#include <fstream>

namespace Utilities
{
	namespace Binary
	{
		template <class ST, class T>
		inline void write( ST& file, const T& value )
		{
			file.write( (char*)&value, sizeof( T ) );
		}
		template <class ST, class T>
		inline void read( ST& file, T& value )
		{
			file.read( (char*)&value, sizeof( T ) );
		}

		template <class ST, class T>
		inline void write( ST& file, const T* value, size_t size )
		{
			file.write( (char*)value, size );
		}
		template <class ST, class T>
		inline void read( ST& file, T* value, size_t size )
		{
			file.read( (char*)value, size );
		}
	}

}

#endif