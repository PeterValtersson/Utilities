#ifndef _UTILZ_STRING_READ_WRITE_H_
#define _UTILZ_STRING_READ_WRITE_H_
#include <string>
#include <sstream>
#include <iomanip>

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
	namespace Binary_Operators
	{
		inline std::ostream& operator<<(std::ostream& out, const std::string& str)
		{
			Utilities::string_binary_write(out, str);
			return out;
		}
		inline std::istream& operator>>(std::istream& in, std::string& str)
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
	inline std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}
	inline std::string charToHex(unsigned char c)
	{
		std::stringstream ss;
		ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex;
		ss << +c;
		return ss.str();
	}
}

#endif