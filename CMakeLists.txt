cmake_minimum_required(VERSION 3.16)
project(LMMS_MAGENTA VERSION 0.1.0)

# Project-wide settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a build directory.")
endif()

# Set module path for custom Find modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Build options
option(BUILD_TESTING "Build tests" ON)
option(ENABLE_PROFILING "Enable performance profiling" OFF)
option(USE_SYSTEM_TENSORFLOW "Use system TensorFlow instead of bundled" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Find dependencies
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
# Other dependencies will be added as needed

# Add components
add_subdirectory(core)
add_subdirectory(ai)

# Testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Documentation
if(BUILD_DOCS)
    find_package(MkDocs QUIET)
    if(MKDOCS_FOUND)
        add_custom_target(docs
            COMMAND ${MKDOCS_EXECUTABLE} build
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating documentation with MkDocs"
        )
    else()
        message(STATUS "MkDocs not found. Documentation will not be built.")
    endif()
endif()

# Create a directory for CMake modules
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
endif()

# Create a Find module for LMMS
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindLMMS.cmake")
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindLMMS.cmake"
"# Find LMMS
# This module defines
# LMMS_INCLUDE_DIRS - where to find LMMS headers
# LMMS_LIBRARIES - LMMS libraries
# LMMS_FOUND - True if LMMS is found

# Try to find LMMS in standard paths
find_path(LMMS_INCLUDE_DIR
    NAMES lmms/Plugin.h
    PATHS
        /usr/include
        /usr/local/include
        ${CMAKE_SOURCE_DIR}/lmms/include
)

find_library(LMMS_LIBRARY
    NAMES lmms
    PATHS
        /usr/lib
        /usr/local/lib
        ${CMAKE_SOURCE_DIR}/lmms/build
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LMMS DEFAULT_MSG
    LMMS_INCLUDE_DIR
    LMMS_LIBRARY
)

if(LMMS_FOUND)
    set(LMMS_INCLUDE_DIRS ${LMMS_INCLUDE_DIR})
    set(LMMS_LIBRARIES ${LMMS_LIBRARY})
endif()

mark_as_advanced(LMMS_INCLUDE_DIR LMMS_LIBRARY)
")
endif()

# Create a Find module for MkDocs
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindMkDocs.cmake")
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindMkDocs.cmake"
"# Find MkDocs
# This module defines
# MKDOCS_EXECUTABLE - Path to mkdocs executable
# MKDOCS_FOUND - True if MkDocs is found

find_program(MKDOCS_EXECUTABLE
    NAMES mkdocs
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MkDocs DEFAULT_MSG
    MKDOCS_EXECUTABLE
)

mark_as_advanced(MKDOCS_EXECUTABLE)
")
endif()

# Output configuration information
message(STATUS "LMMS-Magenta configuration:")
message(STATUS "  Version: ${LMMS_MAGENTA_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build testing: ${BUILD_TESTING}")
message(STATUS "  Enable profiling: ${ENABLE_PROFILING}")
message(STATUS "  Use system TensorFlow: ${USE_SYSTEM_TENSORFLOW}")
message(STATUS "  Build documentation: ${BUILD_DOCS}")

message(STATUS "LMMS-Magenta integration structure initialized")
