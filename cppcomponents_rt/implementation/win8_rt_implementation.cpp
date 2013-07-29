#include "implementation_interfaces.hpp"

#include <winstring.h>
#include <roapi.h>

using namespace cppcomponents;
using namespace cppcomponents::rt;
using namespace implementation;


struct Win8Implementation : implement_runtime_class<Win8Implementation, implementation::Implementation_t>{
	static hstring_type CreateString(cross_compiler_interface::basic_cr_string < native_char_t, std::char_traits < native_char_t >> s){
		hstring_type ret;
		auto e = WindowsCreateString(s.data(), s.size(), &ret);
		throw_if_error(e);
		return ret;

	}
	static hstring_type DuplicateString(hstring_type h){
		hstring_type ret;
		auto e = WindowsDuplicateString(h, &ret);
		return h;

	}
	static std::pair<const native_char_t*, std::uint32_t> GetStringRawBuffer(hstring_type h){
		std::uint32_t length;
		auto p = WindowsGetStringRawBuffer(h, &length);
		return std::make_pair(p, length);

	}
	static hstring_type ConcatenateStrings(hstring_type h1, hstring_type h2){
		hstring_type ret = nullptr;
		WindowsConcatString(h1, h2, &ret);
		return ret;
	}
	static std::int32_t CompareStrings(hstring_type h1, hstring_type h2){
		std::int32_t ret = 0;
		return WindowsCompareStringOrdinal(h1, h2, &ret);
	}

	static void DeleteString(hstring_type h){
		WindowsDeleteString(h);
	}

	//
	// Call per thread
	static void Initialize(std::int32_t i){
		auto e = RoInitialize(static_cast <RO_INIT_TYPE>(i));
		throw_if_error(e);
	}

	static void Uninitialize(){
		RoUninitialize();
	}

	// Activation Factory
	static portable_base* GetActivationFactory(hstring_type h, const uuid_base* u){
		portable_base* p = nullptr;
		auto e = RoGetActivationFactory(h, reinterpret_cast <const IID&>(*u), reinterpret_cast<void**>(&p));
		throw_if_error(e);
		return p;
	}


	static void CppComponentRtInitialize(cppcomponents::cr_string config_file){

	}
	static void* AllocateMemory(std::size_t sz){
		return ::CoTaskMemAlloc(sz);
	}
	static void FreeMemory(void* p){
		::CoTaskMemFree(p);
	}

};

CPPCOMPONENTS_DEFINE_FACTORY(Win8Implementation);

