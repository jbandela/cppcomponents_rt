#pragma once
#ifndef INCLUDE_GUARD_CPPCOMPONENT_RT_IMPLEMENTATION_INTERFACES_HPP
#define INCLUDE_GUARD_CPPCOMPONENT_RT_IMPLEMENTATION_INTERFACES_HPP
#include <cppcomponents/cppcomponents.hpp>


#ifdef _WIN32
// On Windows, mirror the HSTRING declaration
struct HSTRING__;
#endif
namespace cppcomponents{

	namespace rt{


		namespace implementation{
			// Define the HSTRING type
#ifdef _WIN32
			// hstring_type is compatible with HSTRING
			typedef HSTRING__* hstring_type;
			typedef wchar_t native_char_t;
			typedef char non_native_char_t;
#else
			namespace detail{
				struct hstring_base_type;
			}
			typedef hstring_base_type* hstring_type;

			typedef char native_char_t;
			typedef wchar_t non_native_char_t;
#endif

			struct StaticInterface : public define_interface < uuid < 0x210054e8, 0xea2a, 0x4d70, 0x88b0, 0x6f4e4677a68d>>{
				hstring_type CreateString(cross_compiler_interface::basic_cr_string<native_char_t,std::char_traits<native_char_t>>);
				hstring_type DuplicateString(hstring_type);
				std::pair<const native_char_t*,std::uint32_t> GetStringRawBuffer(hstring_type);
				hstring_type ConcatenateStrings(hstring_type, hstring_type);
				std::int32_t CompareStrings(hstring_type, hstring_type);
				void DeleteString(hstring_type);

				//
				// Call per thread
				void Initialize(std::int32_t);

				void Uninitialize();

				// Activation Factory
				portable_base* GetActivationFactory(hstring_type h, const uuid_base*);

				void CppComponentRtInitialize(cr_string config_file);
				void* AllocateMemory(std::size_t sz);
				void FreeMemory(void* p);

				CPPCOMPONENTS_CONSTRUCT(StaticInterface, CreateString, DuplicateString, GetStringRawBuffer,
					ConcatenateStrings, CompareStrings, DeleteString, Initialize, Uninitialize,
					GetActivationFactory, CppComponentRtInitialize,AllocateMemory,FreeMemory);


			};

			inline std::string cppcomponents_rt_implementation_id(){ return "cppcomponents_rt_implementation"; }
			typedef runtime_class < cppcomponents_rt_implementation_id, object_interfaces<InterfaceUnknown>, factory_interface<NoConstructorFactoryInterface>,
				static_interfaces < StaticInterface >> Implementation_t;

			typedef use_runtime_class<Implementation_t> Implementation;


		}

	}



}


namespace cross_compiler_interface{

	template<>
	struct cross_conversion<cppcomponents::rt::implementation::hstring_type>
		:trivial_conversion<cppcomponents::rt::implementation::hstring_type>
	{};
}


#endif
