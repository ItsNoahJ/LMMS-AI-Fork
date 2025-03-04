set(UTILS_SOURCES
    src/MidiUtils.cpp
    src/ConfigUtils.cpp
    src/PerformanceMonitor.cpp
)

set(UTILS_HEADERS
    include/MidiUtils.h
    include/ConfigUtils.h
    include/PerformanceMonitor.h
)

add_library(lmms-magenta-utils STATIC 
    ${UTILS_SOURCES} 
    ${UTILS_HEADERS}
)

target_include_directories(lmms-magenta-utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(lmms-magenta-utils
    PUBLIC
        lmms-magenta-core
        Qt5::Core
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION include/lmms-magenta/utils
    FILES_MATCHING PATTERN "*.h"
)

# Install library
install(
    TARGETS lmms-magenta-utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
