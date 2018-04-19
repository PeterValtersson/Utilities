#include "stdafx.h"
#include "CppUnitTest.h"
#include"../Include/Error.h"
#include "../Include/GUID.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilitiesTests
{
	TEST_CLASS(ERRORTest)
	{
	public:
		UERROR Function(int i)
		{
			if (i == 0)
				RETURN_ERROR("i == 0");
			RETURN_SUCCESS;
		}
		UERROR Foo()
		{
			PASS_IF_ERROR(Function(0));
			RETURN_SUCCESS;
		}
		UERROR Foo_int(int i)
		{
			RETURN_ERROR_EX("Error ", i);
		}
		UERROR Foo_char()
		{
			RETURN_ERROR_EX("Error ", "char");
		}
		UERROR Foo_str()
		{
			RETURN_ERROR_EX("Error ", std::string("char"));
		}
		UERROR Foo_GUID(Utilities::GUID guid)
		{
			RETURN_ERROR_EX("Error GUID: ", guid);
		}
		TEST_METHOD(ERRORTest_Basic)
		{
			Assert::AreEqual("Success"_hash, Function(1).hash);
			Assert::AreEqual(16u, Function(1).line);
			Assert::AreEqual("i == 0"_hash, Foo().hash);
			Assert::AreEqual(15u, Foo().line);
		}
		TEST_METHOD(ERRORTest_Extra)
		{
			Assert::AreEqual("Error 1"_hash, Foo_int(1).hash);
			Assert::AreEqual("Error 2"_hash, Foo_int(2).hash);
			Assert::AreEqual("Error char"_hash, Foo_char().hash);
			Assert::AreEqual("Error char"_hash, Foo_str().hash);
			std::string str = "Error GUID: " + std::to_string("test"_hash);
			Assert::AreEqual(Utilities::StringHash::GetHash_ConstexprString(str.c_str(), uint32_t(str.size())), Foo_GUID("test").hash);
		}
	};
}