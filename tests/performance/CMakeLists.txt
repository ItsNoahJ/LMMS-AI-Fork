set(PERFORMANCE_TEST_SOURCES
    ModelLoadingBenchmark.cpp
)

add_executable(performance_tests ${PERFORMANCE_TEST_SOURCES})

target_link_libraries(performance_tests
    PRIVATE
        lmms-magenta-core
        lmms-magenta-model-serving
        lmms-magenta-plugins
        lmms-magenta-utils
        lmms-magenta-ui
        GTest::GTest
        GTest::Main
)

# Add tests to CTest
gtest_discover_tests(performance_tests)
