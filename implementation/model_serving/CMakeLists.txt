cmake_minimum_required(VERSION 3.10)

# Create model_serving library
add_library(lmms_magenta_model_serving STATIC
    ModelServer.cpp
    TensorFlowLiteModel.cpp
    MusicVAEModel.cpp
)

# Link libraries
target_link_libraries(lmms_magenta_model_serving
    PRIVATE
    Qt5::Core
    ${TensorFlowLite_LIBRARIES}
    lmms_magenta_utils
)

# Include directories
target_include_directories(lmms_magenta_model_serving
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TensorFlowLite_INCLUDE_DIRS}
)

# Install targets
install(TARGETS lmms_magenta_model_serving
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(FILES
    ModelServer.h
    TensorFlowLiteModel.h
    MusicVAEModel.h
    DESTINATION include/lmms_magenta/model_serving
)
