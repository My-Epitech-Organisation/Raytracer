set(RAYTRACER_SOURCES
    main.cpp
    core/Vec3.cpp
    core/Ray.cpp
    core/Mat4.cpp
    core/Transform.cpp
    core/Color.cpp
    display/PPMDisplay.cpp
    display/SFMLDisplay.cpp
    scene/Scene.cpp
    scene/SceneBuilder.cpp
    scene/Camera.cpp
    scene/primitives/Sphere.cpp
    scene/primitives/Plane.cpp
    scene/primitives/Cylinder.cpp
    scene/lights/DirectionalLight.cpp
)

# Main executable
add_executable(raytracer ${RAYTRACER_SOURCES})

# Find required packages
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
if(SFML_FOUND)
    target_link_libraries(raytracer PRIVATE sfml-graphics sfml-window sfml-system)
    target_compile_definitions(raytracer PRIVATE SFML_AVAILABLE)
else()
    message(STATUS "SFML not found - SFML display will be disabled")
endif()

# Link with dynamic loader for plugins (libdl on Linux)
if(UNIX AND NOT APPLE)
    target_link_libraries(raytracer PRIVATE dl)
endif()

# Include project header files
target_include_directories(raytracer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)