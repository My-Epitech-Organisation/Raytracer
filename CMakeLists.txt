cmake_minimum_required(VERSION 3.25)
project(raytracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(plugins/primitives/sample_sphere)

install(TARGETS raytracer RUNTIME DESTINATION bin)
