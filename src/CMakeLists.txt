set(RAYTRACER_SOURCES
    core/Vector3D.cpp
    core/Ray.cpp
    core/Matrix.cpp
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
    scene/parser/SceneParser.cpp
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

# Rechercher libconfig++ (utilisé pour le parsing de scène)
find_path(LIBCONFIG_INCLUDE_DIR libconfig.h++ /opt/homebrew/include)
find_library(LIBCONFIG_LIBRARY NAMES config++ PATHS /opt/homebrew/lib)

# Vérifie qu'on a bien trouvé
if(NOT LIBCONFIG_INCLUDE_DIR OR NOT LIBCONFIG_LIBRARY)
    message(FATAL_ERROR "libconfig++ not found. Install it with brew install libconfig")
endif()

# Inclure les headers et linker la lib
target_include_directories(raytracer PRIVATE ${LIBCONFIG_INCLUDE_DIR})
target_link_libraries(raytracer PRIVATE ${LIBCONFIG_LIBRARY})

# Include project header files
target_include_directories(raytracer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
