set(MODEL_SERVING_SOURCES
    src/ModelServer.cpp
    src/TensorFlowLiteModel.cpp
    src/MusicVAEModel.cpp
)

set(MODEL_SERVING_HEADERS
    include/ModelServer.h
    include/TensorFlowLiteModel.h
    include/MusicVAEModel.h
)

add_library(lmms-magenta-model-serving STATIC 
    ${MODEL_SERVING_SOURCES} 
    ${MODEL_SERVING_HEADERS}
)

target_include_directories(lmms-magenta-model-serving
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find TensorFlow Lite
if(USE_SYSTEM_TENSORFLOW)
    find_package(TensorFlowLite REQUIRED)
else()
    # Use bundled TensorFlow Lite
    set(TFLITE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/tensorflow/tensorflow/lite/c)
    set(TFLITE_LIB ${CMAKE_SOURCE_DIR}/external/tensorflow/bazel-bin/tensorflow/lite/c/libtensorflowlite_c.so)
endif()

target_include_directories(lmms-magenta-model-serving
    PUBLIC
        ${TFLITE_INCLUDE_DIR}
)

target_link_libraries(lmms-magenta-model-serving
    PUBLIC
        lmms-magenta-core
        lmms-magenta-utils
        ${TFLITE_LIB}
        Qt5::Core
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION include/lmms-magenta/model_serving
    FILES_MATCHING PATTERN "*.h"
)

# Install library
install(
    TARGETS lmms-magenta-model-serving
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
