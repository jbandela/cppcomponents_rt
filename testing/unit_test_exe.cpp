#include "../cppcomponents_rt/hstring.hpp"
#include "../cppcomponents_rt/cppcomponents_rt.hpp"
#include "unit_test_client_interfaces.hpp"

#include <gtest/gtest.h>

#ifdef _WIN32

#define NATIVE_TEXT(T)L ## T

#else

#define NATIVE_TEXT(T) T
#endif

#define NATIVE L

TEST(hstring_tests, default_construct){
	cppcomponents::rt::hstring h;

}

TEST(hstring_tests, construct_with_string_literal){

	cppcomponents::rt::hstring h(NATIVE_TEXT("This is a test"));

}
TEST(hstring_tests, construct_with_string){

	cppcomponents::rt::hstring h(std::string("This is a test"));

}
TEST(hstring_tests, construct_with_wstring){

	cppcomponents::rt::hstring h(std::wstring(L"This is a test"));

}

TEST(hstring_tests, concatenate){
	using cppcomponents::rt::hstring;

	hstring h1(NATIVE_TEXT("This"));
	hstring h2(std::string(" is a "));
	hstring h3(std::wstring(L"test"));

	auto h = h1 + h2 + h3;
	hstring expected(std::string("This is a test"));
	EXPECT_EQ(h, expected);
}

TEST(hstring_tests, to_string){

	using cppcomponents::rt::hstring;

	hstring h(std::string("This is a test"));

	std::string expected("This is a test");

	EXPECT_EQ(h.to_string(), expected);

}
TEST(hstring_tests, to_wstring){

	using cppcomponents::rt::hstring;

	hstring h(std::wstring(L"This is a test"));
	std::wstring expected(L"This is a test");

	EXPECT_EQ(h.to_wstring(), expected);

}

TEST(winrt_client_tests, test_client){

		cppcomponents::rt::initializer init;

		cppcomponents::rt::hstring uri_string("http://www.microsoft.com");
		CUri uri(uri_string);
		auto w = uri.GetDomain();
		std::wcout << L"Domain name: " << uri.GetDomain().c_str() << std::endl;

		std::wcout << L"Absolute Canonical Uri: " << uri.AbsoluteCanonicalUri().c_str() << std::endl;
		auto escaped = CUri::EscapeComponent(L"this is a test");
		std::wcout << escaped.c_str() << std::endl;
		auto unescaped = CUri::UnescapeComponent(escaped);
		std::wcout << unescaped.c_str() << std::endl;

}