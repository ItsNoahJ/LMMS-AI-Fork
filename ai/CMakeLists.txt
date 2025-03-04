# Add subdirectories for AI components
add_subdirectory(model_serving)
add_subdirectory(plugins)
add_subdirectory(utils)
add_subdirectory(ui)

# Create an umbrella target for all AI components
add_library(lmms-magenta-ai INTERFACE)

target_link_libraries(lmms-magenta-ai
    INTERFACE
        lmms-magenta-model-serving
        lmms-magenta-plugins
        lmms-magenta-utils
        lmms-magenta-ui
)
