#ifndef _UTILZ_STRING_READ_WRITE_H_
#define _UTILZ_STRING_READ_WRITE_H_
#include <string>

namespace Utilities
{
	inline void string_binary_write(std::ostream& out, const std::string& str)
	{
		uint32_t size = static_cast<uint32_t>(str.size());
		out.write((char*)&size, sizeof(size));
		out.write(str.c_str(), size);
	}
	inline void string_binary_read(std::istream& in, std::string& str)
	{
		uint32_t size = 0;
		in.read((char*)&size, sizeof(size));
		char buffer[512];
		in.read(buffer, size);
		str = std::string(buffer, size);
	}
	namespace Operators
	{
		std::ostream& operator<<(std::ostream& out, const std::string& str)
		{
			Utilities::string_binary_write(out, str);
			return out;
		}
		std::istream& operator>>(std::istream& in, std::string& str)
		{
			Utilities::string_binary_read(in, str);
			return in;
		}
	}
	inline std::string tabs(int numTabs, std::string_view c = "\t")
	{
		std::string tabs;
		for (int tab = 0; tab < numTabs; tab++)
			tabs += c;
		return tabs;
	}
}

#endif