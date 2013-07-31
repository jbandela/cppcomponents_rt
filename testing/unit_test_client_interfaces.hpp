// Define the interfaces - taken from windows.foundation.h
#include "../cppcomponents_rt/cppcomponents_rt.hpp"
#include <cppcomponents/events.hpp>
struct InterfaceUriRuntimeClass : public cppcomponents::rt::define_rt_interface <cppcomponents::uuid< 0x9E365E57, 0x48B2, 0x4160, 0x956F, 0xC7385120BBFC>>{

	// Define a typedef for hstring
	typedef cppcomponents::rt::hstring hstring;

	//// Declare Interface so we can use it in our class
	//template<class T> struct Interface;

	// Define the UUID for the class

	hstring GetAbsoluteUri();
	hstring GetDisplayUri();
	hstring GetDomain();
	hstring GetExtension();
	hstring GetFragment();
	hstring GetHost();
	hstring GetPassword();
	hstring GetPath();
	hstring GetQuery();

	// Change so we don't have to define Iwwwformdecoder
	cppcomponents::use<cppcomponents::rt::InterfaceInspectable> GetQueryParsed();

	hstring GetRawUri();
	hstring GetSchemeName();
	hstring GetUserName();
	hstring GetPort();
	unsigned char GetSuspicious();
	unsigned char Equals(cppcomponents::use<InterfaceUriRuntimeClass>);
	cppcomponents::use<InterfaceUriRuntimeClass> CombineUri(hstring);



	CPPCOMPONENTS_CONSTRUCT(InterfaceUriRuntimeClass, GetAbsoluteUri, GetDisplayUri, GetDomain, GetExtension, GetFragment, GetHost, GetPassword, GetPath,
		GetQuery, GetQueryParsed, GetRawUri, GetSchemeName, GetUserName, GetPort, GetSuspicious, Equals, CombineUri);


};

//[uuid(758D9661-221C-480F-A339-50656673F46F)]
//[version(0x06020000)]
//[exclusiveto(Windows.Foundation.Uri)]
//interface IUriRuntimeClassWithAbsoluteCanonicalUri : IInspectable
//{
//    [propget] HRESULT AbsoluteCanonicalUri([out] [retval] HSTRING* value);
//    [propget] HRESULT DisplayIri([out] [retval] HSTRING* value);
//}

struct InterfaceUriRuntimeClassWithAbsoluteCanonicalUri : cppcomponents::rt::define_rt_interface <cppcomponents::uuid< 0x758D9661, 0x221C, 0x480F, 0xA339, 0x50656673F46F>>{
	cppcomponents::rt::hstring AbsoluteCanonicalUri();
	cppcomponents::rt::hstring DisplayIri();

	CPPCOMPONENTS_CONSTRUCT(InterfaceUriRuntimeClassWithAbsoluteCanonicalUri, AbsoluteCanonicalUri, DisplayIri);


};

//    [uuid(C1D432BA-C824-4452-A7FD-512BC3BBE9A1)]
//[exclusiveto(Windows.Foundation.Uri)]
//[version(0x06020000)]
//interface IUriEscapeStatics : IInspectable
//{
//    HRESULT UnescapeComponent([in] HSTRING toUnescape, [out] [retval] HSTRING* value);
//    HRESULT EscapeComponent([in] HSTRING toEscape, [out] [retval] HSTRING* value);
//}

struct InterfaceUriEscapeStatics : cppcomponents::rt::define_rt_interface<cppcomponents::uuid<0xC1D432BA, 0xC824, 0x4452, 0xA7FD, 0x512BC3BBE9A1>>{

	cppcomponents::rt::hstring UnescapeComponent(cppcomponents::rt::hstring toUnescape);
	cppcomponents::rt::hstring EscapeComponent(cppcomponents::rt::hstring toUnescape);

	CPPCOMPONENTS_CONSTRUCT(InterfaceUriEscapeStatics, UnescapeComponent, EscapeComponent);
};

struct InterfaceUriRuntimeClassFactory : cppcomponents::rt::define_rt_interface<cppcomponents::uuid<0x44A9796F, 0x723E, 0x4FDF, 0xA218, 0x033E75B0C084>>{

	typedef cppcomponents::rt::hstring hstring;


	cppcomponents::use<InterfaceUriRuntimeClass> CreateUri(hstring);
	cppcomponents::use< InterfaceUriRuntimeClass > CreateWithRelativeUri(hstring, hstring);


	CPPCOMPONENTS_CONSTRUCT(InterfaceUriRuntimeClassFactory, CreateUri, CreateWithRelativeUri);

};






inline  cppcomponents::rt::hstring FoundationUri(){ return L"Windows.Foundation.Uri"; }
typedef cppcomponents::rt::rt_runtime_class < FoundationUri,
	cppcomponents::object_interfaces<InterfaceUriRuntimeClass, InterfaceUriRuntimeClassWithAbsoluteCanonicalUri>,
	cppcomponents::factory_interface<InterfaceUriRuntimeClassFactory>, cppcomponents::static_interfaces<InterfaceUriEscapeStatics> > ClassUri_t;


typedef cppcomponents::rt::use_rt_runtime_class<ClassUri_t> CUri;




enum DeviceWatcherStatus
{
	DeviceWatcherStatus_Created = 0,
	DeviceWatcherStatus_Started = 1,
	DeviceWatcherStatus_EnumerationCompleted = 2,
	DeviceWatcherStatus_Stopping = 3,
	DeviceWatcherStatus_Stopped = 4,
	DeviceWatcherStatus_Aborted = 5
};


namespace cross_compiler_interface{
	template<>
	struct cross_conversion<DeviceWatcherStatus>:public trivial_conversion<DeviceWatcherStatus>{};
}

using namespace cppcomponents;
using namespace rt;

struct InterfaceDeviceWatcher : public define_rt_interface <cppcomponents::uuid<0xC9EAB97D, 0x8F6B, 0x4F96, 0xA9F4, 0xABC814E22271>>{

	// Our delegates
	typedef cppcomponents::event_delegate < void(use<InterfaceDeviceWatcher>, void*), cppcomponents::uuid<0x03c5a07b, 0x990c, 0x5d09, 0xb0b8, 0x5734eaa38222>> AddHandler;
	typedef cppcomponents::event_delegate < void(use<InterfaceDeviceWatcher>, void*), cppcomponents::uuid<0x906f1254, 0x79ad, 0x54fc, 0x93c4, 0xcdb99b437899>> UpdateHandler;
	typedef cppcomponents::event_delegate < void(use<InterfaceDeviceWatcher>, use<InterfaceInspectable>), cppcomponents::uuid<0x9234630f, 0x1ff4, 0x54f6, 0x9e3f, 0xac20369b7725>> CompletedHandler;
	typedef cppcomponents::event_delegate < void(use<InterfaceDeviceWatcher>, use<InterfaceInspectable>), cppcomponents::uuid<0x9234630f, 0x1ff4, 0x54f6, 0x9e3f, 0xac20369b7725>> StoppedHandler;


	std::int64_t add_Added(use<AddHandler>);
	void remove_Added(std::int64_t);

	std::int64_t add_Updated(use<UpdateHandler>);
	void remove_Updated(std::int64_t);

	// Not use removed handler so just accept IUnknown
	std::int64_t add_Removed(use<cppcomponents::InterfaceUnknown>);
	void remove_Removed(std::int64_t);

	std::int64_t add_EnumerationCompleted(use<CompletedHandler>);
	void remove_EnumerationCompleted(std::int64_t);

	std::int64_t add_Stopped(use<StoppedHandler>);
	void remove_Stopped(std::int64_t);


	DeviceWatcherStatus get_Status();

	void Start();

	void Stop();

	CPPCOMPONENTS_CONSTRUCT(InterfaceDeviceWatcher, add_Added, remove_Added, add_Updated, remove_Updated, add_Removed, remove_Removed, add_EnumerationCompleted, remove_EnumerationCompleted,
		add_Stopped, remove_Stopped, get_Status, Start, Stop);

	// Define the properties and events
	CPPCOMPONENTS_INTERFACE_EXTRAS{

		CPPCOMPONENTS_R_PROPERTY(get_Status) Status;

		CPPCOMPONENTS_EVENT(AddHandler, add_Added, remove_Added) Added;
		CPPCOMPONENTS_EVENT(UpdateHandler, add_Updated, remove_Updated) Updated;
		CPPCOMPONENTS_EVENT(CompletedHandler, add_EnumerationCompleted, remove_EnumerationCompleted) EnumerationCompleted;
		CPPCOMPONENTS_EVENT(StoppedHandler, add_Stopped, remove_Stopped) Stopped;

		CPPCOMPONENTS_INTERFACE_EXTRAS_CONSTRUCTOR(Status, Added, Updated, EnumerationCompleted, Stopped);

	};

};

struct InterfaceDeviceInformationStatics : define_rt_interface < cppcomponents::uuid<0xC17F100E, 0x3A46, 0x4A78, 0x8013, 0x769DC9B97390>>{


	use<InterfaceInspectable> CreateFromIdAsync(hstring);
	use<InterfaceInspectable> CreateFromIdAsyncAdditionalProperties(hstring, use<InterfaceInspectable>);
	use<InterfaceInspectable> FindAllAsync();
	use<InterfaceInspectable> FindAllAsyncDeviceClass(std::int32_t);
	use<InterfaceInspectable> FindAllAsyncAqsFilter(hstring);
	use<InterfaceInspectable> FindAllAsyncAqsFilterAndAdditionalProperties(hstring, use<InterfaceInspectable>);
	use<InterfaceDeviceWatcher> CreateWatcher();
	use<InterfaceInspectable> CreateWatcherDeviceClass(std::int32_t);
	use<InterfaceInspectable> CreateWatcherAqsFilter(hstring);
	use<InterfaceInspectable> CreateWatcherAqsFilterAndAdditionalProperties(hstring, use<InterfaceInspectable>);


	CPPCOMPONENTS_CONSTRUCT(InterfaceDeviceInformationStatics, CreateFromIdAsync, CreateFromIdAsyncAdditionalProperties, FindAllAsync, FindAllAsyncDeviceClass,
		FindAllAsyncAqsFilter, FindAllAsyncAqsFilterAndAdditionalProperties, CreateWatcher, CreateWatcherDeviceClass, CreateWatcherAqsFilter, CreateWatcherAqsFilterAndAdditionalProperties);


};








inline hstring DeviceStatusId(){ return L"Windows.Devices.Enumeration.DeviceInformation"; };

typedef rt::rt_runtime_class < DeviceStatusId,
	cppcomponents::static_interfaces < InterfaceDeviceInformationStatics >> DeviceInformation_t;

typedef rt::use_rt_runtime_class<DeviceInformation_t> DeviceInformation;

