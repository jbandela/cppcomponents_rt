#include "implementation_interfaces.hpp"

#include <winstring.h>
#include <roapi.h>

using namespace cppcomponents;
using namespace cppcomponents::rt;
using namespace implementation;

#include <Windows.h>
#include <unordered_map>
#include <atomic>

#define CPPCOMPONENTS_RT_NO_CACHE


#ifndef CPPCOMPONENTS_RT_NO_CACHE
typedef std::pair<std::uint32_t,std::unordered_map<std::basic_string<implementation::native_char_t>, use<InterfaceUnknown>>> map_t;

struct factory_map{
	static std::uint32_t global_TLS_handle;

	factory_map(){
		global_TLS_handle = ::TlsAlloc();
	}
	~factory_map(){
		::TlsFree(global_TLS_handle);;
	}

	 void set_map(map_t* p){

		 if (global_TLS_handle == TLS_OUT_OF_INDEXES){
			 throw error_fail();
		 }



		 auto b = ::TlsSetValue(global_TLS_handle, p);
		 if (!b){
			 throw error_fail();
		 }

	}
	 map_t* get_map(){

		if (global_TLS_handle == TLS_OUT_OF_INDEXES){
			throw error_fail();
		}

		return static_cast<map_t*>(::TlsGetValue(global_TLS_handle));



	}

	 static factory_map& get(){
		 return cross_compiler_interface::detail::safe_static_init<factory_map, factory_map>::get();
	 }


};
	 std::uint32_t factory_map::global_TLS_handle = TLS_OUT_OF_INDEXES;
#endif

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

#ifndef CPPCOMPONENTS_RT_NO_CACHE

		auto p = factory_map::get().get_map();
		if (p){
			++(p->first);
		}
		else{
			std::unique_ptr<map_t> ptr(new map_t);
			ptr->first = 1;
			factory_map::get().set_map(ptr.get());
			ptr.release();

		}
#endif
		auto e = RoInitialize(static_cast <RO_INIT_TYPE>(i));
		throw_if_error(e);
	}

	static void Uninitialize(){
#ifndef CPPCOMPONENTS_RT_NO_CACHE
		try{
			if (auto pm = factory_map::get().get_map()){
				if (--pm->first)
				delete pm;
			}
		}
		catch (std::exception&){

		}

		RoUninitialize();
#endif

	}

	// Activation Factory
	static portable_base* GetActivationFactory(hstring_type h, const uuid_base* u){
#ifndef CPPCOMPONENTS_RT_NO_CACHE
		auto& m = factory_map::get().get_map()->second;
		std::uint32_t length;
		auto p = ::WindowsGetStringRawBuffer(h, &length);
		std::basic_string<implementation::native_char_t> key(p,p+length);
		auto iter = m.find(key);
		if (iter == m.end()){
			portable_base* p = nullptr;
			auto e = RoGetActivationFactory(h, reinterpret_cast <const IID&>(*u), reinterpret_cast<void**>(&p));
			throw_if_error(e);
			m.insert(std::make_pair(key, use<InterfaceUnknown>(reinterpret_portable_base<InterfaceUnknown::Interface>(p), false)));
			return p;
		}
		else{
			return iter->second.QueryInterfaceRaw(u);
		}
#else
			portable_base* p = nullptr;
			auto e = RoGetActivationFactory(h, reinterpret_cast <const IID&>(*u), reinterpret_cast<void**>(&p));
			throw_if_error(e);
			return p;

#endif
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

