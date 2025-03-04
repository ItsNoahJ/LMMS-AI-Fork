enable_testing()

# Find GTest
find_package(GTest REQUIRED)

# Find Qt5 (optional)
find_package(Qt5 COMPONENTS Core Widgets QUIET)
if(Qt5_FOUND)
    message(STATUS "Qt5 found at ${Qt5_DIR}")
else()
    message(WARNING "Qt5 not found. Some tests will be disabled.")
    message(STATUS "Set Qt5_DIR environment variable to your Qt installation, e.g.")
    message(STATUS "  Windows: set Qt5_DIR=C:/Qt/5.15.2/mingw81_64/lib/cmake/Qt5")
endif()

# Add subdirectories for test categories
add_subdirectory(unit)
add_subdirectory(integration)
add_subdirectory(performance)

# Create a custom target to run all tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running all tests"
)

# Create a custom target to run only core tests (no Qt dependency)
add_custom_target(run_core_tests
    COMMAND ${CMAKE_CTEST_COMMAND} -R "unit_tests_core" --output-on-failure
    COMMENT "Running core tests (no Qt dependency)"
)
