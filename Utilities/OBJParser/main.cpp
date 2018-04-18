//
//#include <iostream>
//#include <fstream>
//#include "Parsers.h"
//#pragma warning(disable:4146)
//
//
//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
////https://github.com/ezaquarii/bison-flex-cpp-example
//
//#endif  // _DEBUG
//#include <string>
//
//using namespace std;
//
//string GetFilenameWithoutExtension(const string& str)
//{
//	size_t lastindex = str.find_last_of(".");
//	string rawname = str.substr(0, lastindex);
//	return rawname;
//}
//
//
//int main(int argc, char **argv) {
//#ifdef _DEBUG
//	_CrtDumpMemoryLeaks();
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//#endif // _DEBUG
//
//	string filename = "";
//	if (argc > 1)
//	{
//		filename = argv[1];
//	}
//	else
//	{
//		cout << "Enter filename: ";
//		cin >> filename;
//	}
//	ArfData::Data data;
//	ArfData::DataPointers dp;
//
//	cout << "Parsing " << filename << "..." << endl << "\t";
//	int ret = ParseObj(filename.c_str(), &data, &dp);
//	if (ret)
//		return ret;
//
//	string name = "";
//	if (argc > 2)
//		name = argv[2];
//	else
//		name = GetFilenameWithoutExtension(filename) + ".arf";
//
//	cout << "Saving as " << name << "." << endl << endl;
//	ofstream file(name, ios::binary | ios::trunc);
//	file.write((char*)&data, sizeof(ArfData::Data));
//	file.write((char*)dp.buffer, data.allocated);
//
//
//
//	return 0;
//}
