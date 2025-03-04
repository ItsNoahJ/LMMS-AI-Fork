set(INTEGRATION_TEST_SOURCES
    MusicVAEIntegrationTest.cpp
    GrooVAEIntegrationTest.cpp
)

add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})

target_link_libraries(integration_tests
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
gtest_discover_tests(integration_tests)
