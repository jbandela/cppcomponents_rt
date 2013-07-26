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
				std::pair<native_char_t*,std::int32_t> GetStringRawBuffer(hstring_type);
				void DeleteString(hstring_type);
				// Call per thread
				void Initialize(std::int32_t);

				// Call only once per app
				void CppComponentRtInitialize();
				CPPCOMPONENTS_CONSTRUCT(StaticInterface, CreateString, DuplicateString, GetStringRawBuffer, DeleteString, Initialize);


			};

			inline std::string cppcomponents_rt_implementation_id(){ return "cppcomponents_rt_implementation"; }
			typedef runtime_class < cppcomponents_rt_implementation_id, object_interfaces<InterfaceUnknown>, factory_interface<NoConstructorFactoryInterface>,
				static_interfaces < StaticInterface >> Implementation_t;

			typedef use_runtime_class<Implementation_t> Implementation;


		}

	}



}