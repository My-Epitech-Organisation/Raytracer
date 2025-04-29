set(SPHERE_PLUGIN_SOURCES
    SpherePlugin.cpp
)

# Create a shared library for the plugin
add_library(sphere_plugin MODULE ${SPHERE_PLUGIN_SOURCES})

# Set output directory and naming convention
set_target_properties(sphere_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

# Include project header files
target_include_directories(sphere_plugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Link with required libraries
target_link_libraries(sphere_plugin PRIVATE raytracer_core)