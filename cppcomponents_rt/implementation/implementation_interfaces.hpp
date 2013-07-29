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
#else
			namespace detail{
				struct hstring_base_type;
			}
			typedef hstring_base_type* hstring_type;

			typedef char native_char_t;
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

				CPPCOMPONENTS_CONSTRUCT(StaticInterface, CreateString, DuplicateString, GetStringRawBuffer,
					ConcatenateStrings, CompareStrings, DeleteString, Initialize,Uninitialize,
				GetActivationFactory,CppComponentRtInitialize);


			};

			inline std::string cppcomponents_rt_implementation_id(){ return "cppcomponents_rt_implementation"; }
			typedef runtime_class < cppcomponents_rt_implementation_id, object_interfaces<InterfaceUnknown>, factory_interface<NoConstructorFactoryInterface>,
				static_interfaces < StaticInterface >> Implementation_t;



		}

	}



}
#define CPPCOMPONENTS_RT_USE_RAW_WINRT_CALLS
#ifdef CPPCOMPONENTS_RT_USE_RAW_WINRT_CALLS
#include <winstring.h>
#include <roapi.h>

namespace cppcomponents{
	namespace rt{
		namespace implementation{
		
			struct Implementation{
				static hstring_type CreateString(cross_compiler_interface::basic_cr_string < native_char_t, std::char_traits < native_char_t >> s ){
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
					return std::make_pair(p,length );

				}
				static hstring_type ConcatenateStrings(hstring_type h1, hstring_type h2){
					hstring_type ret = nullptr;
					WindowsConcatString(h1, h2,&ret);
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
					auto e = RoInitialize(static_cast < RO_INIT_TYPE>(i));
					throw_if_error(e);
				}

				static void Uninitialize(){
					RoUninitialize();
				}

				// Activation Factory
				static portable_base* GetActivationFactory(hstring_type h, uuid_base* u){
					portable_base* p = nullptr;
					auto e = RoGetActivationFactory(h, reinterpret_cast <const IID&>(*u), reinterpret_cast<void**>(&p));
					throw_if_error(e);
					return p;
				}
			};
		}
	}
}


#else
namespace cppcomponents{
	namespace rt{
		namespace implementation{
			typedef use_runtime_class<Implementation_t> Implementation;

		}
	}
}


#endif

namespace cross_compiler_interface{

	template<>
	struct cross_conversion<cppcomponents::rt::implementation::hstring_type>
		:trivial_conversion<cppcomponents::rt::implementation::hstring_type>
	{};
}