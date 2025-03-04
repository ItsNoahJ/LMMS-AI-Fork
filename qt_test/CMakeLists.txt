cmake_minimum_required(VERSION 3.10)
project(QtTest)

# Find Qt5 components
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(qt_test main.cpp)

# Link Qt5 libraries
target_link_libraries(qt_test PRIVATE Qt5::Core Qt5::Widgets)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
