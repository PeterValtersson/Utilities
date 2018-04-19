#ifndef _UTILITIES_ERROR_H_
#define _UTILITIES_ERROR_H_
#include "CompileTimeString.h"
#include <string>

#define PASS_IF_ERROR(e) if(e.hash != "Success"_hash) return e

#define RETURN_SUCCESS return (Utilities::__lastError__ = Utilities::CreateError("Success", __FILE__, __LINE__))
#define RETURN_ERROR(msg) return (Utilities::__lastError__ = Utilities::CreateError(msg, __FILE__, __LINE__))
#define RETURN_ERROR_EX(msg, extra) return Utilities::CreateErrorExtra(Utilities::CreateError(msg, __FILE__, __LINE__), extra)
#define RETURN_IF_HR_ERROR(hr, msg) if(FAILED(hr)) RETURN_ERROR_EX(msg, hr)
#define THROW_ERROR(msg) throw (Utilities::__lastError__ = Utilities::CreateError(msg, __FILE__, __LINE__))
#define THROW_ERROR_EX(msg, extra) throw Utilities::CreateErrorExtra(Utilities::CreateError(msg, __FILE__, __LINE__), extra)
#define THROW_IF_HR_ERROR(hr, msg) if(FAILED(hr)) THROW_ERROR_EX(msg, hr)
#define UERROR const Utilities::Error&


namespace Utilities
{
	
	struct Error
	{
		const char* message;
		Utilities::HashValue hash;
		const char* file;
		uint32_t line;
	};
	static Error __lastError__;
	static std::string __lastErrorString__;
	constexpr Error CreateError(const StringHash::ConstexprString message, const char* file, uint32_t line)
	{
		return { message, StringHash::GetHash_ConstexprString(message), file, line };
	}
	template<typename extraType>
	static UERROR CreateErrorExtra(const Error& err, extraType extra)
	{
		__lastError__.message = (__lastErrorString__ = err.message + std::to_string(extra)).c_str();
		__lastError__.hash = StringHash::GetHash_ConstexprString(__lastErrorString__.c_str(), uint32_t(__lastErrorString__.size()));
		__lastError__.file = err.file;
		__lastError__.line = err.line;
		return __lastError__;
	}
	template<>
	static UERROR CreateErrorExtra(const Error& err,const std::string& extra)
	{
		__lastError__.message = (__lastErrorString__ = err.message + extra).c_str();
		__lastError__.hash = StringHash::GetHash_ConstexprString(__lastErrorString__.c_str(), uint32_t(__lastErrorString__.size()));
		__lastError__.file = err.file;
		__lastError__.line = err.line;
		return __lastError__;
	}
	template<>
	static UERROR CreateErrorExtra(const Error& err, const char* extra)
	{
		__lastError__.message = (__lastErrorString__ = err.message + std::string(extra)).c_str();
		__lastError__.hash = StringHash::GetHash_ConstexprString(__lastErrorString__.c_str(), uint32_t(__lastErrorString__.size()));
		__lastError__.file = err.file;
		__lastError__.line = err.line;
		return __lastError__;
	}
	template<>
	static UERROR CreateErrorExtra(const Error& err, char* extra)
	{
		__lastError__.message = (__lastErrorString__ = err.message + std::string(extra)).c_str();
		__lastError__.hash = StringHash::GetHash_ConstexprString(__lastErrorString__.c_str(), uint32_t(__lastErrorString__.size()));
		__lastError__.file = err.file;
		__lastError__.line = err.line;
		return __lastError__;
	}
}
#endif