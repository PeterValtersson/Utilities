//#include "Parsers.h"
//#include "ObjParser.h"
//#include <fstream>
//
//int ParseObj(const char * filename, ArfData::Data * data, ArfData::DataPointers * dataPointers, uint32_t flags)
//{
//	ObjParser::Interpreter parser;
//	ifstream file(filename);
//
//	if (file.is_open())
//	{
//		int res = parser.parse(&file, data, dataPointers);
//		return res;
//	}
//	return PARSER_FILE_NOT_FOUND;
//}
//struct membuf : std::streambuf
//{
//	membuf(char* begin, char* end) {
//		this->setg(begin, begin, end);
//	}
//};
//
//int ParseObj(const void * rawData, size_t size, ArfData::Data * data, ArfData::DataPointers * dataPointers, uint32_t flags)
//{
//	if (rawData != nullptr)
//	{
//		ObjParser::Interpreter parser;
//		membuf buf((char*)rawData, (char*)rawData + size);
//		istream stream(&buf);
//
//		int res = parser.parse(&stream, data, dataPointers);
//		return res;
//	}
//	return PARSER_FILE_NOT_FOUND;
//}
//
//int ParseArf(const char * filename, ArfData::Data * data, ArfData::DataPointers * dataPointers, uint32_t flags)
//{
//	fstream f;
//	f.open(filename, ios::binary);
//	if (!f.is_open())
//		return PARSER_FILE_NOT_FOUND;
//
//	f.read((char*)data, sizeof(ArfData::Data));
//
//	dataPointers->buffer = operator new(data->allocated);
//	f.read((char*)dataPointers->buffer, data->allocated);
//
//	dataPointers->positions = (ArfData::Position*)dataPointers->buffer;
//	dataPointers->texCoords = (ArfData::TexCoord*)(dataPointers->positions + data->PosCap);
//	dataPointers->normals = (ArfData::Normal*)(dataPointers->texCoords + data->TexCap);
//	dataPointers->faces = (ArfData::Face*)(dataPointers->normals + data->NormCap);
//	dataPointers->subMesh = (ArfData::SubMesh*)(dataPointers->faces + data->FaceCap);
//
//	f.close();
//
//	return PARSER_SUCCESS;
//}
//
//int Interleave(ArfData::Data * data, ArfData::DataPointers * dataPointers, uint32_t flags)
//{
//	return PARSER_SUCCESS;
//}