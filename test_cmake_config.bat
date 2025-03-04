@echo off
REM Test script to verify CMake configuration with MinGW

REM First, run the setup script to set environment variables
call setup_qt_env_fixed.bat

REM Create a test directory
mkdir qt_test_clean
cd qt_test_clean

REM Create a simple CMakeLists.txt file
echo cmake_minimum_required(VERSION 3.10) > CMakeLists.txt
echo project(QtTest) >> CMakeLists.txt
echo find_package(Qt5 COMPONENTS Widgets REQUIRED) >> CMakeLists.txt
echo add_executable(qt_test main.cpp) >> CMakeLists.txt
echo target_link_libraries(qt_test Qt5::Widgets) >> CMakeLists.txt

REM Create a simple main.cpp file
echo #include ^<QApplication^> > main.cpp
echo #include ^<QLabel^> >> main.cpp
echo int main(int argc, char *argv[]) { >> main.cpp
echo     QApplication app(argc, argv); >> main.cpp
echo     QLabel label("Hello, Qt!"); >> main.cpp
echo     label.show(); >> main.cpp
echo     return app.exec(); >> main.cpp
echo } >> main.cpp

REM Create build directory
mkdir build
cd build

REM Run CMake with explicit compiler paths
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="%MINGW_DIR%\bin\gcc.exe" -DCMAKE_CXX_COMPILER="%MINGW_DIR%\bin\g++.exe" ..

REM If CMake succeeds, try to build
if %ERRORLEVEL% EQU 0 (
    echo CMake configuration successful. Attempting to build...
    cmake --build .
) else (
    echo CMake configuration failed with error code %ERRORLEVEL%
)

REM Return to original directory
cd ..\..

echo Test completed.
