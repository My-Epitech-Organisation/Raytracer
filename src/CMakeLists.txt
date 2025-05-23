set(RAYTRACER_SOURCES
    core/Vector3D.cpp
    core/Ray.cpp
    core/Matrix.cpp
    core/Transform.cpp
    core/Color.cpp
    core/ThreadPool.cpp
    core/RenderTile.cpp
    display/PPMDisplay.cpp
    display/SFMLDisplay.cpp
    scene/Scene.cpp
    scene/SceneBuilder.cpp
    scene/Camera.cpp
    scene/primitives/Cylinder.cpp
    scene/primitives/Sphere.cpp
    scene/primitives/Plane.cpp
    scene/primitives/CheckerboardPlane.cpp
    scene/primitives/Cone.cpp
    scene/primitives/LimitedCone.cpp
    scene/primitives/LimitedCylinder.cpp
    scene/primitives/PrimitiveFactory.cpp
    scene/primitives/Torus.cpp
    scene/primitives/Triangle.cpp
    scene/lights/LightFactory.cpp
    scene/lights/PointLight.cpp
    scene/lights/DirectionalLight.cpp
    scene/lights/AmbientLight.cpp
    scene/parser/SceneParser.cpp
    exceptions/RaytracerException.cpp
)

# Create a library with the core functionality
add_library(raytracer_core STATIC ${RAYTRACER_SOURCES})

# Include project header files for the library
target_include_directories(raytracer_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Find required packages for the library
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
if(SFML_FOUND)
    target_link_libraries(raytracer_core PUBLIC sfml-graphics sfml-window sfml-system)
    target_compile_definitions(raytracer_core PUBLIC SFML_AVAILABLE)
else()
    message(STATUS "SFML not found - SFML display will be disabled")
endif()

# Main executable
add_executable(raytracer main.cpp)
target_link_libraries(raytracer PRIVATE raytracer_core)

# Link with dynamic loader for plugins (libdl on Linux)
if(UNIX AND NOT APPLE)
    target_link_libraries(raytracer PRIVATE dl)
endif()

# Find libconfig++ for configuration file parsing
find_path(LIBCONFIG_INCLUDE_DIR libconfig.h++ /opt/homebrew/include)
find_library(LIBCONFIG_LIBRARY NAMES config++ PATHS /opt/homebrew/lib)

# Check if libconfig++ is found
if(NOT LIBCONFIG_INCLUDE_DIR OR NOT LIBCONFIG_LIBRARY)
    message(FATAL_ERROR "libconfig++ not found. Install it with brew install libconfig")
endif()

# Link libconfig++ to the raytracer executable
target_include_directories(raytracer PRIVATE ${LIBCONFIG_INCLUDE_DIR})
target_link_libraries(raytracer PRIVATE ${LIBCONFIG_LIBRARY})

# Include project header files
target_include_directories(raytracer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
