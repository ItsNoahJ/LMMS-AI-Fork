set(CORE_SOURCES
    src/CoreConfig.cpp
)

set(CORE_HEADERS
    include/CoreConfig.h
)

add_library(lmms-magenta-core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(lmms-magenta-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(lmms-magenta-core
    PUBLIC
        Qt5::Core
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION include/lmms-magenta/core
    FILES_MATCHING PATTERN "*.h"
)

# Install library
install(
    TARGETS lmms-magenta-core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
