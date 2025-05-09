set(TEST_SOURCES
    test_Camera.cpp
    test_Color.cpp
    test_parser.cpp
    test_Ray.cpp
    test_Sphere.cpp
    test_Transform.cpp
    test_Vector3D.cpp
    test_Plane.cpp
)

# Test executable
add_executable(raytracer_tests ${TEST_SOURCES})

# Link with GoogleTest
target_link_libraries(raytracer_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Link with the existing core raytracer library
target_link_libraries(raytracer_tests PRIVATE raytracer_core)

# Include project header files
target_include_directories(raytracer_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(raytracer_tests)