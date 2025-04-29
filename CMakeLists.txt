cmake_minimum_required(VERSION 3.25)
project(raytracer 
        VERSION 0.1.0
        DESCRIPTION "A modular raytracing engine"
        LANGUAGES CXX)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Build options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_PLUGINS "Build plugin modules" ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Fetch external dependencies
include(cmake/FetchGoogleTest.cmake)

# Add subdirectories
add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_PLUGINS)
    add_subdirectory(plugins/primitives/sample_sphere)
    add_subdirectory(plugins/lights/sample_point)
endif()

# Installation
install(TARGETS raytracer RUNTIME DESTINATION bin)
