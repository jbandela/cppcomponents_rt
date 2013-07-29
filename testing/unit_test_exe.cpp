#include "../cppcomponents_rt/hstring.hpp"
#include "../cppcomponents_rt/cppcomponents_rt.hpp"
#include "unit_test_client_interfaces.hpp"

#include <gtest/gtest.h>
#include <atomic>
#include <thread>

#ifdef _WIN32

#define NATIVE_TEXT(T)L ## T

#else

#define NATIVE_TEXT(T) T
#endif

#define NATIVE L


struct InitOnce{
	InitOnce(){
		cppcomponents::runtime_classes_map().add("cppcomponents_rt_implementation", "win8_rt_implementation");
		cppcomponents::runtime_classes_map().finalize();
	}
} init_;


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

		EXPECT_EQ(uri.GetDomain(), cppcomponents::rt::hstring("www.microsoft.com"));
		EXPECT_EQ(uri.AbsoluteCanonicalUri(), cppcomponents::rt::hstring("http://www.microsoft.com"));


		auto escaped = CUri::EscapeComponent(L"this is a test");
		EXPECT_EQ(CUri::UnescapeComponent(escaped),"this is a test");

}

#include <Windows.h>

TEST(winrt_client_tests, test_events){
	//cppcomponents::rt::initializer init;
	//{
	//	auto enumerationCompleted = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, WRITE_OWNER | EVENT_ALL_ACCESS);

	//std::atomic_bool bdone = {false};


	//int deviceCount = 0;
	//bool completedCalled = false;
	//auto watcher = DeviceInformation::CreateWatcher();

	//std::int64_t addedToken;
	//std::int64_t stoppedToken;
	//std::int64_t enumCompletedToken;


	//addedToken = watcher.Added += [&deviceCount](use<InterfaceDeviceWatcher> watcher, void*){
	//	// When we reach 10 devices, stop enumerating devices.
	//	deviceCount++;
	//	if (deviceCount == 10)
	//	{
	//		return watcher.Stop();
	//	}
	//};

	//stoppedToken = watcher.Stopped += [&](use<InterfaceDeviceWatcher> watcher, use<InterfaceInspectable>){

	//	// Unsubscribe from the events. This is shown for demonstration. 
	//	// The need to remove event handlers depends on the requirements of  
	//	// your app. For instance, if you only need to handle an event for  
	//	// a short period of time, you might remove the event handler when you 
	//	// no longer need it. If you handle an event for the duration of the app, 
	//	// you might not need to explicitly remove it.
	//	watcher.Added -= addedToken;
	//	watcher.Stopped -= stoppedToken;
	//	watcher.EnumerationCompleted -= enumCompletedToken;


	//	// Set the completion event and return.
	//	SetEvent(enumerationCompleted);


	//};
	//enumCompletedToken = watcher.EnumerationCompleted += [&](use<InterfaceDeviceWatcher> watcher, use<InterfaceInspectable>){
	//	completedCalled = true;
	//	return watcher.Stop();

	//};


	//watcher.Start();
	//WaitForSingleObjectEx(enumerationCompleted, INFINITE, FALSE);

	//
	//bool success = (completedCalled) ? true : deviceCount == 10;
	//EXPECT_EQ(success, true);
	//}
	//bool b = true;
	//EXPECT_EQ(b, true);

	auto enumerationCompleted = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, WRITE_OWNER | EVENT_ALL_ACCESS);
	auto hr = enumerationCompleted ? S_OK : HRESULT_FROM_WIN32(GetLastError());
	if (FAILED(hr))
	{
		
	}

	rt::initializer init;

	int deviceCount = 0;
	auto watcher = DeviceInformation::CreateWatcher();

	std::int64_t addedToken;
	std::int64_t stoppedToken;
	std::int64_t enumCompletedToken;


	addedToken = watcher.Added += [&deviceCount](use<InterfaceDeviceWatcher> watcher, void*){
		// Print a message and increment the device count. 
		// When we reach 10 devices, stop enumerating devices.
		wprintf_s(L"Added device...\n");
		deviceCount++;
		if (deviceCount == 10)
		{
			return watcher.Stop();
		}
	};

	stoppedToken = watcher.Stopped += [&](use<InterfaceDeviceWatcher> watcher, use<InterfaceInspectable>){
		wprintf_s(L"Device enumeration stopped.\nRemoving event handlers...\n");

		// Unsubscribe from the events. This is shown for demonstration. 
		// The need to remove event handlers depends on the requirements of  
		// your app. For instance, if you only need to handle an event for  
		// a short period of time, you might remove the event handler when you 
		// no longer need it. If you handle an event for the duration of the app, 
		// you might not need to explicitly remove it.
		watcher.Added -= addedToken;
		watcher.Stopped -= stoppedToken;
		watcher.EnumerationCompleted -= enumCompletedToken;

		// Set the completion event and return.
		SetEvent(enumerationCompleted);

	};
	enumCompletedToken = watcher.EnumerationCompleted += [](use<InterfaceDeviceWatcher> watcher, use<InterfaceInspectable>){
		wprintf_s(L"Enumeration completed.\n");

		return watcher.Stop();

	};


	watcher.Start();

	WaitForSingleObjectEx(enumerationCompleted, INFINITE, FALSE);
	wprintf_s(L"Enumerated %u devices.\n", deviceCount);


}