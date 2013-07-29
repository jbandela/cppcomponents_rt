g++ -std=c++11 ../unit_test_exe.cpp -I ..\..\..\cppcomponents -I external\googletest-read-only -I external\googletest-read-only\include external\googletest-read-only\src\gtest_main.cc external\googletest-read-only\src\gtest-all.cc

cl ..\..\cppcomponents_rt\implementation\win8_rt_implementation.cpp /LD /I ..\..\..\cppcomponents /EHsc runtimeobject.lib ole32.lib

a.exe
