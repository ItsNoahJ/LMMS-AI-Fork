cmake_minimum_required(VERSION 3.10)

# Create ui_components library
add_library(lmms_magenta_ui_components STATIC
    # Add source files here as they are created
)

# Link libraries
target_link_libraries(lmms_magenta_ui_components
    PRIVATE
    Qt5::Core
    Qt5::Widgets
)

# Include directories
target_include_directories(lmms_magenta_ui_components
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lmms/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lmms/src
)

# Install targets
install(TARGETS lmms_magenta_ui_components
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/lmms_magenta/ui_components
    FILES_MATCHING PATTERN "*.h"
)
