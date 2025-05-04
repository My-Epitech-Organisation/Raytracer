set(TEST_SOURCES
    test_Camera.cpp
    test_SceneParser.cpp
    test_Color.cpp
    test_parser.cpp
    test_Ray.cpp
    test_Sphere.cpp
    test_Transform.cpp
    test_Vector3D.cpp
)

# Test executable
add_executable(raytracer_tests ${TEST_SOURCES})

find_path(LIBCONFIG_INCLUDE_DIR libconfig.h++ /opt/homebrew/include)
find_library(LIBCONFIG_LIBRARY NAMES config++ PATHS /opt/homebrew/lib)

if(NOT LIBCONFIG_INCLUDE_DIR OR NOT LIBCONFIG_LIBRARY)
    message(FATAL_ERROR "libconfig++ not found. Install it with brew install libconfig")
endif()

target_include_directories(raytracer_tests PRIVATE ${LIBCONFIG_INCLUDE_DIR})
target_link_libraries(raytracer_tests PRIVATE ${LIBCONFIG_LIBRARY})

target_link_libraries(raytracer_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    raytracer_core
)

# Include project header files
target_include_directories(raytracer_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(raytracer_tests)