set(POINT_LIGHT_PLUGIN_SOURCES
    PointLightPlugin.cpp
)

# Create a shared library for the plugin
add_library(point_light_plugin MODULE ${POINT_LIGHT_PLUGIN_SOURCES})

# Set output directory and naming convention
set_target_properties(point_light_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Include project header files
target_include_directories(point_light_plugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Link with required libraries
target_link_libraries(point_light_plugin PRIVATE raytracer_core)