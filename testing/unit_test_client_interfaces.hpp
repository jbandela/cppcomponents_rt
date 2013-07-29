// Define the interfaces - taken from windows.foundation.h
#include "../cppcomponents_rt/cppcomponents_rt.hpp"

struct InterfaceUriRuntimeClass : public cppcomponents::rt::define_rt_interface <cppcomponents::uuid< 0x9E365E57, 0x48B2, 0x4160, 0x956F, 0xC7385120BBFC>>{

	// Define a typedef for hstring
	typedef cppcomponents::rt::hstring hstring;

	// Declare Interface so we can use it in our class
	template<class T> struct Interface;

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
