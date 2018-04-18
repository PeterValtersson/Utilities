#include "OBJPassthrough.h"
#include <File_Error.h>
struct membuf : std::streambuf
{
	membuf(char* begin, char* end) {
		this->setg(begin, begin, end);
	}
	pos_type seekoff(off_type off,
		std::ios_base::seekdir dir,
		std::ios_base::openmode which = std::ios_base::in) {
		if (dir == std::ios_base::cur)
			gbump(off);
		else if (dir == std::ios_base::end)
			setg(eback(), egptr() + off, egptr());
		else if (dir == std::ios_base::beg)
			setg(eback(), eback() + off, egptr());
		return gptr() - eback();
	}
	pos_type seekpos(pos_type sp, std::ios_base::openmode which) override {
		return seekoff(sp - pos_type(off_type(0)), std::ios_base::beg, which);
	}
};
DLL_EXPORT int32_t Parse(uint32_t guid, void * data, uint64_t size, void ** parsedData, uint64_t * parsedSize)
{
	if (data != nullptr)
	{
		ObjParser::Interpreter parser;
		membuf buf((char*)data, (char*)data + size);
		istream stream(&buf);
		*parsedData = new ArfData::ArfData();
		auto& tp = (*(ArfData::ArfData*)*parsedData);
		auto res =  parser.parse(&stream);
		if (res)
			return -2;

		*parsedData = parser.GetData();

		return 0;

	}
	return -1;
}

DLL_EXPORT int32_t Destroy(uint32_t guid, void * data, uint64_t size)
{
	operator delete(data);
	return 0;
}
