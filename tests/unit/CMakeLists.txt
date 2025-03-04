# Define core test sources (no Qt dependencies)
set(CORE_TEST_SOURCES
    MidiUtilsTest.cpp
    ModelServerTest.cpp
    TensorFlowLiteModelTest.cpp
)

# Define Qt-dependent test sources
set(QT_TEST_SOURCES
    MusicVAEModelTest.cpp
    GrooVAEModelTest.cpp
    AIPluginTest.cpp
)

# Find Qt5
find_package(Qt5 COMPONENTS Core Widgets QUIET)

# Always build core tests
add_executable(unit_tests_core ${CORE_TEST_SOURCES})
target_link_libraries(unit_tests_core
    PRIVATE
        lmms-magenta-core
        lmms-magenta-model-serving
        lmms-magenta-utils
        GTest::GTest
        GTest::Main
)

# Add tests to CTest
gtest_discover_tests(unit_tests_core)

# Build Qt-dependent tests if Qt is available
if(Qt5_FOUND)
    message(STATUS "Qt5 found, building Qt-dependent tests")
    
    # Add Qt-dependent executable
    add_executable(unit_tests_qt ${QT_TEST_SOURCES})
    target_link_libraries(unit_tests_qt
        PRIVATE
            lmms-magenta-core
            lmms-magenta-model-serving
            lmms-magenta-plugins
            lmms-magenta-utils
            lmms-magenta-ui
            Qt5::Core
            Qt5::Widgets
            GTest::GTest
            GTest::Main
    )
    
    # Add to CTest
    gtest_discover_tests(unit_tests_qt)
    
    # For backward compatibility, create a combined test executable
    add_executable(unit_tests ${CORE_TEST_SOURCES} ${QT_TEST_SOURCES})
    target_link_libraries(unit_tests
        PRIVATE
            lmms-magenta-core
            lmms-magenta-model-serving
            lmms-magenta-plugins
            lmms-magenta-utils
            lmms-magenta-ui
            Qt5::Core
            Qt5::Widgets
            GTest::GTest
            GTest::Main
    )
    
    # Add to CTest
    gtest_discover_tests(unit_tests)
else()
    message(WARNING "Qt5 not found, skipping Qt-dependent tests")
    
    # For backward compatibility, create a unit_tests target that only includes core tests
    add_executable(unit_tests ${CORE_TEST_SOURCES})
    target_link_libraries(unit_tests
        PRIVATE
            lmms-magenta-core
            lmms-magenta-model-serving
            lmms-magenta-utils
            GTest::GTest
            GTest::Main
    )
    
    # Add to CTest
    gtest_discover_tests(unit_tests)
endif()
