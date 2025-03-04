set(UI_SOURCES
    src/AIControlPanel.cpp
    src/PatternVisualizer.cpp
    src/ModelSelector.cpp
)

set(UI_HEADERS
    include/AIControlPanel.h
    include/PatternVisualizer.h
    include/ModelSelector.h
)

add_library(lmms-magenta-ui STATIC 
    ${UI_SOURCES} 
    ${UI_HEADERS}
)

target_include_directories(lmms-magenta-ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find LMMS
find_package(LMMS REQUIRED)

target_include_directories(lmms-magenta-ui
    PUBLIC
        ${LMMS_INCLUDE_DIRS}
)

target_link_libraries(lmms-magenta-ui
    PUBLIC
        lmms-magenta-core
        ${LMMS_LIBRARIES}
        Qt5::Core
        Qt5::Widgets
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION include/lmms-magenta/ui
    FILES_MATCHING PATTERN "*.h"
)

# Install library
install(
    TARGETS lmms-magenta-ui
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
