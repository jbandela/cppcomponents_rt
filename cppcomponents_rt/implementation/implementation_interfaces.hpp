#include <cppcomponents/cppcomponents.hpp>


#ifdef _WIN32
// On Windows, mirror the HSTRING declaration
struct HSTRING__;
#endif
namespace cppcomponents{

	namespace rt{

		// Define the HSTRING type
#if defined(_WIN32)
		// hstring_type is compatible with HSTRING
		typedef HSTRING__* hstring_type;
#else
		namespace detail{
			struct hstring_base_type;
		}
		typedef hstring_base_type* hstring;
#endif
		namespace implementation{
			struct I

		}

	}



}