cmake_minimum_required(VERSION 3.10)

# Create utils library
add_library(lmms_magenta_utils STATIC
    # Add source files here as they are created
    MidiUtils.cpp
)

# Link libraries
target_link_libraries(lmms_magenta_utils
    PRIVATE
    Qt5::Core
)

# Include directories
target_include_directories(lmms_magenta_utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Install targets
install(TARGETS lmms_magenta_utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/lmms_magenta/utils
    FILES_MATCHING PATTERN "*.h"
)
