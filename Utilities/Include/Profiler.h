#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <stdint.h>
#include <chrono>
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>
#include <filesystem>
#include "CompileTimeString.h"

#ifdef _P_NS
static const char* scale = "ns";
#define _P_TIMESCALE std::chrono::nanoseconds
#else
#ifdef _P_MS
static const char* scale = "ms";
#define _P_TIMESCALE std::chrono::milliseconds
#else
static const char* scale = "ms";
#define _P_TIMESCALE std::chrono::milliseconds
#endif
#endif


#ifdef __PROFILE
const std::string escapeLessGreaterSymbols(const std::string& str)
{
	std::string temp = str;
	while (auto pos = temp.find('<') != std::string::npos)
		temp.replace(pos, 1, "\\<");
	while (auto pos = temp.find('>') != std::string::npos)
		temp.replace(pos, 1, "\\>");
	return temp;
};
struct ProfilerNode {
	ProfilerNode() : timesCalled(0), timeSpent(0)
	{

	}
	ProfilerNode(ProfilerNode* parent, const char* functionName, Utilities::HashValue myHash, const char* file)
		: parent(parent), functionName(functionName), myHash(myHash), timesCalled(0), timeSpent(0),
		file(file)
	{

	}
	~ProfilerNode()
	{
		for (auto& c : children)
		{
			if (c.second)
				delete c.second;
		}
	}
	ProfilerNode* parent;
	std::string functionName;
	const char* file;
	Utilities::HashValue myHash;
	uint64_t timesCalled;
	std::chrono::nanoseconds timeSpent;
	std::chrono::high_resolution_clock::time_point timeStart;
	std::map<Utilities::HashValue, ProfilerNode*> children;

	inline std::string getHexCode(unsigned char c) {


		std::stringstream ss;

		ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex;


		ss << +c;

		return ss.str();
	}
	void makeTree(std::stringstream & out)
	{
		out << "\"" << this << "\"" << "[\n shape = none\n";
		out << "label = <<table border=\"0\" cellspacing = \"0\">\n";

		out << "<tr><td port=\"port1\" border=\"1\" bgcolor = \"#" << getHexCode(0) << getHexCode(150) << getHexCode(50) << "\"><font color=\"white\">" 
			<< file << ": " << escapeLessGreaterSymbols(functionName) << "</font></td></tr>\n";

		for (auto& c : children)
		{
			timeSpent += c.second->timeSpent;
		}
		out << "<tr><td border=\"1\">" << "Time Spent: " << std::chrono::duration_cast<_P_TIMESCALE>(timeSpent).count() << " " << scale << "</td></tr>\n";

		out << "</table>>]\n";
		for (auto& c : children)
		{
			c.second->_makeTree(out);
			out << "\"" << this << "\":port1 -> \"" << c.second << "\":port1\n";
		}
		out << "\n";
		return void();
	}
private:
	void _makeTree(std::stringstream & out)
	{
		
		out << "\"" << this << "\"" << "[\n shape = none\n";
		out << "label = <<table border=\"0\" cellspacing = \"0\">\n";
		double div = 0.0;
		if (parent)
			div = ((double)timeSpent.count() / parent->timeSpent.count());
		out << "<tr><td port=\"port1\" border=\"1\" bgcolor = \"#" 
			<< getHexCode(unsigned char(150 * div)) << getHexCode(50) << getHexCode(unsigned char(50 * (1.0 - div))) << "\"><font color=\"white\">"
			<< file << ": " << escapeLessGreaterSymbols(functionName) << "</font></td></tr>\n";

		out << "<tr><td border=\"1\">" << "Times Called: " << timesCalled << "</td></tr>\n";
		out << "<tr><td border=\"1\">" << "Time Spent(IC): " << std::chrono::duration_cast<_P_TIMESCALE>(timeSpent).count() << " " << scale;
		if (parent)
			out << " " << div * 100.0 << " % of parents.</td></tr>\n";
		else
			out << "</td></tr>\n";

		out << "<tr><td border=\"1\">" << "Time Spent(avg): " << std::chrono::duration_cast<_P_TIMESCALE>(timeSpent / timesCalled).count() << " " << scale << "</td></tr>\n";


		if (children.size())
		{
			auto temp = timeSpent;
			for (auto& c : children)
				temp -= c.second->timeSpent;
			out << "<tr><td border=\"1\">" << "Time Spent(EC): " << std::chrono::duration_cast<_P_TIMESCALE>(temp).count() << " " << scale << "</td></tr>\n";
		}

		out << "</table>>]\n";

		for (auto& c : children)
		{
			c.second->_makeTree(out);
			out << "\"" << this << "\":port1 -> \"" << c.second << "\":port1\n";
		}
		out << "\n";
		return void();
	}

};


class Profiler {
public:
	inline static Profiler& GetInstance()
	{
		static thread_local Profiler inst;
		return inst;
	}
	
	template<uint64_t functionHash>
	void startProfile(const char * funcName, const char* file)
	{
		/*if (!_profile)
		{
			std::string name = funcName;
			size_t lastindex = name.find_last_of(":");
			if (lastindex == std::string::npos)
				_profile = _current = new ProfilerNode(nullptr, "root", functionHash, "");
			else
			{
				size_t lastindex2 = name.substr(0, lastindex - 1).find_last_of(":");
				_profile = _current = new ProfilerNode(nullptr, name.substr(0, lastindex - 1).substr(lastindex2 + 1).c_str(), functionHash, "");
			}



		}

		{
			auto& child = _current->children[functionHash];
			if (!child)
				child = new ProfilerNode(_current, funcName, functionHash, file);
			_current = child;
		}
		_current->timesCalled++;
		_current->timeStart = std::chrono::high_resolution_clock::now();*/
	}

	inline void stopProfile()
	{
		std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds diff = time - _current->timeStart;
		_current->timeSpent += diff;
		_current = _current->parent;
	}

private:

	Profiler()
	{

	}
	inline ~Profiler()
	{
		_dumpToFile();

		if (_profile)
		{
			delete _profile;
		}

	}
	const void _dumpToFile()
	{
		std::stringstream ss;

		ss << "digraph \"" << std::this_thread::get_id() << "\"{\n";
		ss << " rankdir = LR;\n";

		if (_profile)
		{
			_profile->makeTree(ss);
		}

		ss << "\n}\n";

		

		std::experimental::filesystem::create_directory("Profiler");
		std::experimental::filesystem::create_directory("Profiler\\" + _profile->functionName);
		std::ofstream bfile;
		bfile.open("Profiler\\ConvertDotsToPdf.bat", std::ios::trunc);
		if (bfile.is_open())
			bfile << R"(@if (@X)==(@Y) @end /* JScript comment
    @echo off

    set "extension=dot"

    setlocal enableDelayedExpansion
    for /R %%a in (*%extension%) do (
        for /f %%# in ('cscript //E:JScript //nologo "%~f0" %%a') do set "cdate=%%#"
       echo "%%~a"
	   echo "%%~dpa%%~na_!cdate!.pdf"
	   dot -Tpdf "%%~a" -o "%%~dpa%%~na_!cdate!.pdf"
	   del "%%~a"
    )

    rem cscript //E:JScript //nologo "%~f0" %*
    exit /b %errorlevel%
@if (@X)==(@Y) @end JScript comment */


FSOObj = new ActiveXObject("Scripting.FileSystemObject");
var ARGS = WScript.Arguments;
var file=ARGS.Item(0);

var d1=FSOObj.GetFile(file).DateCreated;

d2=new Date(d1);
var year=d2.getFullYear();
var mon=d2.getMonth();
var day=d2.getDate();
var h=d2.getHours();
var m=d2.getMinutes();
var s=d2.getSeconds();
var ms=d2.getMilliseconds();

if (mon<10){mon="0"+mon;}
if (day<10){day="0"+day;}
if (h<10){h="0"+h;}
if (m<10){m="0"+m;}
if (s<10){s="0"+s;}
if (ms<10){ms="00"+ms;}else if(ms<100){ms="0"+ms;}

WScript.Echo(""+year+mon+day+h+m+s+ms);)";


		bfile.close();


		std::ofstream rf;
		rf.open("Profiler\\BatchInstructions.txt", std::ios::trunc);
		if (rf.is_open())
			rf << R"(Download Graphviz
http://www.graphviz.org/pub/graphviz/stable/windows/graphviz-2.38.msi
Add C:\Program Files (x86)\Graphviz2.38\bin (or equivalent) to Path Environment variable

)";

		rf.close();
		std::ofstream out;
		std::stringstream fn;
		fn <<  "Profiler\\" << escapeLessGreaterSymbols(_profile->functionName) << "\\profile_" << std::this_thread::get_id() << ".dot";
		out.open(fn.str(), std::ios::out | std::ios::trunc);
		if (!out.is_open())
			throw std::exception("Profile file could not be opened");
		out.write(ss.str().c_str(), ss.str().size());

		out.close();

		return void();
	}

	ProfilerNode* _profile = nullptr;
	ProfilerNode* _current = nullptr;

};

namespace Basename
{
	static const char* fileName(const char* file)
	{
		const char* f2 = nullptr;
		const char* f = nullptr;
		const char* c = file;
		while (*c != '\0')
		{
			if (*c == '/' || *c == '\\')
			{
				f2 = f;
				f = c;
			}

			++c;
		}
		if (f2)
			return ++f2;
		else if (f)
			return ++f;
		else
			return file;
	}
	static const char* functionName(const char* file)
	{
		const char* f2 = nullptr;
		const char* f = nullptr;
		const char* c = file;
		while (*c != '\0')
		{
			if (*c == ':')
			{
				++c;
				f2 = f;
				f = c;

			}

			++c;
		}
		if (f2)
			return ++f2;
		else if (f)
			return ++f;
		else
			return file;
	}
}
struct Prolifer
{
	Profiler&p;
	Prolifer(Profiler& p) : p(p)
	{

	}
	inline ~Prolifer()
	{
		p.stopProfile();
	}
};

#define StartProfile Prolifer pl(Profiler::GetInstance()); pl.p.startProfile<Utilities::StringHash::GetHash_ConstexprString(__FUNCTION__)> (Basename::functionName(__FUNCTION__), Basename::fileName( __FILE__));
#define StartProfileC(x) Prolifer pl(Profiler::GetInstance()); pl.p.startProfile<Utilities::StringHash::GetHash_ConstexprString(x)>(x, Basename::fileName( __FILE__) );

#else
#define StartProfile 
#define StartProfileC(x)
#endif


#endif