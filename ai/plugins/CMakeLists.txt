set(PLUGINS_SOURCES
    src/AIPlugin.cpp
    src/AIInstrument.cpp
    src/AIEffect.cpp
    src/MusicVAEInstrument.cpp
)

set(PLUGINS_HEADERS
    include/AIPlugin.h
    include/AIInstrument.h
    include/AIEffect.h
    include/MusicVAEInstrument.h
)

add_library(lmms-magenta-plugins STATIC 
    ${PLUGINS_SOURCES} 
    ${PLUGINS_HEADERS}
)

target_include_directories(lmms-magenta-plugins
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find LMMS
find_package(LMMS REQUIRED)

target_include_directories(lmms-magenta-plugins
    PUBLIC
        ${LMMS_INCLUDE_DIRS}
)

target_link_libraries(lmms-magenta-plugins
    PUBLIC
        lmms-magenta-core
        lmms-magenta-model-serving
        lmms-magenta-utils
        lmms-magenta-ui
        ${LMMS_LIBRARIES}
        Qt5::Core
        Qt5::Widgets
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION include/lmms-magenta/plugins
    FILES_MATCHING PATTERN "*.h"
)

# Install library
install(
    TARGETS lmms-magenta-plugins
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
