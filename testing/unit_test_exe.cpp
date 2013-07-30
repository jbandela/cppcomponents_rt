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


TEST(winrt_client_tests, test_events){



	rt::initializer init;
	std::atomic<bool> done = { false };
	bool enumCompleted = false;

		int deviceCount = 0;
		auto watcher = DeviceInformation::CreateWatcher();

		std::int64_t addedToken;
		std::int64_t stoppedToken;
		std::int64_t enumCompletedToken;


		addedToken = watcher.Added += [&deviceCount](use<InterfaceDeviceWatcher> watcher, void*){
			// Print a message and increment the device count. 
			// When we reach 10 devices, stop enumerating devices.
			deviceCount++;
			if (deviceCount == 10)
			{
				return watcher.Stop();
			}
		};

		stoppedToken = watcher.Stopped += [&](use<InterfaceDeviceWatcher> w, use<InterfaceInspectable>)mutable{

			// Unsubscribe from the events. This is shown for demonstration. 
			// The need to remove event handlers depends on the requirements of  
			// your app. For instance, if you only need to handle an event for  
			// a short period of time, you might remove the event handler when you 
			// no longer need it. If you handle an event for the duration of the app, 
			// you might not need to explicitly remove it.
			w.Added -= addedToken;
			w.Stopped -= stoppedToken;
			w.EnumerationCompleted -= enumCompletedToken;

			// It seems there is a "bug in IDeviceWatcher in that Release calls stop which causes an error
			// When the device is not started. This is also visible in the WRL example.
			// So add a dummy handler and call start on it;
			w.Added += [](use<InterfaceDeviceWatcher> watcher, void*){};
			w.Start();

			// Set the completion event and return.
			done = true;


		};
		enumCompletedToken = watcher.EnumerationCompleted += [&](use<InterfaceDeviceWatcher> w, use<InterfaceInspectable>){
			enumCompleted = true;
			watcher.Stop();

		};


		watcher.Start();

		// Busy wait
		while (done == false);

		EXPECT_EQ(enumCompleted ? 10 : deviceCount, 10);



}