cmake_minimum_required(VERSION 3.10)
project(LMMS_Magenta VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find packages
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(TensorFlowLite REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/model_serving
    ${CMAKE_CURRENT_SOURCE_DIR}/plugins
    ${CMAKE_CURRENT_SOURCE_DIR}/ui_components
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/../lmms/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../lmms/src
    ${TensorFlowLite_INCLUDE_DIRS}
)

# Add subdirectories
add_subdirectory(model_serving)
add_subdirectory(plugins)
add_subdirectory(ui_components)
add_subdirectory(utils)

# Create main library
add_library(lmms_magenta SHARED
    # Add source files here
)

# Link libraries
target_link_libraries(lmms_magenta
    PRIVATE
    Qt5::Core
    Qt5::Widgets
    ${TensorFlowLite_LIBRARIES}
    lmms_magenta_model_serving
    lmms_magenta_plugins
    lmms_magenta_ui_components
    lmms_magenta_utils
)

# Install targets
install(TARGETS lmms_magenta
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/lmms_magenta
    FILES_MATCHING PATTERN "*.h"
)

# Create package
include(CPack)
