cmake_minimum_required(VERSION 3.10)

# Create plugins library
add_library(lmms_magenta_plugins STATIC
    AIPlugin.cpp
    AIInstrument.cpp
    AIEffect.cpp
    MusicVAEInstrument.cpp
)

# Link libraries
target_link_libraries(lmms_magenta_plugins
    PRIVATE
    Qt5::Core
    Qt5::Widgets
    lmms_magenta_model_serving
    lmms_magenta_utils
)

# Include directories
target_include_directories(lmms_magenta_plugins
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../model_serving
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lmms/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lmms/src
)

# Install targets
install(TARGETS lmms_magenta_plugins
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(FILES
    AIPlugin.h
    AIInstrument.h
    AIEffect.h
    MusicVAEInstrument.h
    DESTINATION include/lmms_magenta/plugins
)
