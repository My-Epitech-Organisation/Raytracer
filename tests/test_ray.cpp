/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Ray class
*/

#include <gtest/gtest.h>
#include "../src/core/Ray.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/core/Transform.hpp"

using namespace RayTracer;

// Test the default constructor
TEST(RayTest, DefaultConstructor)
{
    Ray ray;

    // Default ray should start at origin (0,0,0)
    Vector3D origin = ray.getOrigin();
    EXPECT_DOUBLE_EQ(origin.getX(), 0.0);
    EXPECT_DOUBLE_EQ(origin.getY(), 0.0);
    EXPECT_DOUBLE_EQ(origin.getZ(), 0.0);

    // Default ray should point along positive Z direction (0,0,1)
    Vector3D direction = ray.getDirection();
    EXPECT_DOUBLE_EQ(direction.getX(), 0.0);
    EXPECT_DOUBLE_EQ(direction.getY(), 0.0);
    EXPECT_DOUBLE_EQ(direction.getZ(), 1.0);
}

// Test the parameterized constructor
TEST(RayTest, ParameterizedConstructor)
{
    Vector3D origin(1.0, 2.0, 3.0);
    Vector3D direction(2.0, 3.0, 4.0);

    Ray ray(origin, direction);

    // Check origin is set correctly
    Vector3D rayOrigin = ray.getOrigin();
    EXPECT_DOUBLE_EQ(rayOrigin.getX(), 1.0);
    EXPECT_DOUBLE_EQ(rayOrigin.getY(), 2.0);
    EXPECT_DOUBLE_EQ(rayOrigin.getZ(), 3.0);

    // Check direction is normalized
    Vector3D normalizedDir = direction.normalized();
    Vector3D rayDirection = ray.getDirection();

    EXPECT_DOUBLE_EQ(rayDirection.getX(), normalizedDir.getX());
    EXPECT_DOUBLE_EQ(rayDirection.getY(), normalizedDir.getY());
    EXPECT_DOUBLE_EQ(rayDirection.getZ(), normalizedDir.getZ());
}

// Test copy constructor
TEST(RayTest, CopyConstructor)
{
    Vector3D origin(4.0, 5.0, 6.0);
    Vector3D direction(7.0, 8.0, 9.0);

    Ray original(origin, direction);
    Ray copy(original);

    // Check origin is copied correctly
    Vector3D copyOrigin = copy.getOrigin();
    EXPECT_DOUBLE_EQ(copyOrigin.getX(), 4.0);
    EXPECT_DOUBLE_EQ(copyOrigin.getY(), 5.0);
    EXPECT_DOUBLE_EQ(copyOrigin.getZ(), 6.0);

    // Check direction is copied correctly
    Vector3D originalDir = original.getDirection();
    Vector3D copyDir = copy.getDirection();

    EXPECT_DOUBLE_EQ(copyDir.getX(), originalDir.getX());
    EXPECT_DOUBLE_EQ(copyDir.getY(), originalDir.getY());
    EXPECT_DOUBLE_EQ(copyDir.getZ(), originalDir.getZ());
}

// Test assignment operator
TEST(RayTest, AssignmentOperator)
{
    Vector3D origin1(1.0, 2.0, 3.0);
    Vector3D direction1(4.0, 5.0, 6.0);
    Ray ray1(origin1, direction1);

    Vector3D origin2(7.0, 8.0, 9.0);
    Vector3D direction2(10.0, 11.0, 12.0);
    Ray ray2(origin2, direction2);

    ray2 = ray1;

    // Check origin is assigned correctly
    Vector3D origin = ray2.getOrigin();
    EXPECT_DOUBLE_EQ(origin.getX(), 1.0);
    EXPECT_DOUBLE_EQ(origin.getY(), 2.0);
    EXPECT_DOUBLE_EQ(origin.getZ(), 3.0);

    // Check direction is assigned correctly
    Vector3D dir1 = ray1.getDirection();
    Vector3D dir2 = ray2.getDirection();

    EXPECT_DOUBLE_EQ(dir2.getX(), dir1.getX());
    EXPECT_DOUBLE_EQ(dir2.getY(), dir1.getY());
    EXPECT_DOUBLE_EQ(dir2.getZ(), dir1.getZ());
}

// Test setOrigin method
TEST(RayTest, SetOrigin)
{
    Ray ray;
    Vector3D newOrigin(5.0, 6.0, 7.0);

    ray.setOrigin(newOrigin);

    Vector3D origin = ray.getOrigin();
    EXPECT_DOUBLE_EQ(origin.getX(), 5.0);
    EXPECT_DOUBLE_EQ(origin.getY(), 6.0);
    EXPECT_DOUBLE_EQ(origin.getZ(), 7.0);
}

// Test setDirection method
TEST(RayTest, SetDirection)
{
    Ray ray;
    Vector3D newDirection(3.0, 4.0, 0.0);

    ray.setDirection(newDirection);

    // Direction should be normalized
    Vector3D normalized = newDirection.normalized();
    Vector3D direction = ray.getDirection();

    EXPECT_DOUBLE_EQ(direction.getX(), normalized.getX());
    EXPECT_DOUBLE_EQ(direction.getY(), normalized.getY());
    EXPECT_DOUBLE_EQ(direction.getZ(), normalized.getZ());

    // Magnitude should be 1
    double magnitude = direction.getMagnitude();
    EXPECT_NEAR(magnitude, 1.0, 1e-10);
}

// Test pointAt method
TEST(RayTest, PointAt)
{
    Vector3D origin(1.0, 2.0, 3.0);
    Vector3D direction(1.0, 0.0, 0.0); // Unit vector along X axis

    Ray ray(origin, direction);

    // Point at t=0 should be the origin
    Vector3D point0 = ray.pointAt(0.0);
    EXPECT_DOUBLE_EQ(point0.getX(), 1.0);
    EXPECT_DOUBLE_EQ(point0.getY(), 2.0);
    EXPECT_DOUBLE_EQ(point0.getZ(), 3.0);

    // Point at t=5 should be 5 units along the direction
    Vector3D point5 = ray.pointAt(5.0);
    EXPECT_DOUBLE_EQ(point5.getX(), 6.0); // 1.0 + 5.0 * 1.0
    EXPECT_DOUBLE_EQ(point5.getY(), 2.0); // 2.0 + 5.0 * 0.0
    EXPECT_DOUBLE_EQ(point5.getZ(), 3.0); // 3.0 + 5.0 * 0.0
}

// Test transform method with translation
TEST(RayTest, TransformTranslation)
{
    Vector3D origin(1.0, 2.0, 3.0);
    Vector3D direction(0.0, 1.0, 0.0); // Unit vector along Y axis

    Ray ray(origin, direction);

    // Create a translation transform of (2,3,4)
    Transform transform;
    transform.translate(2.0, 3.0, 4.0);

    Ray transformed = ray.transform(transform);

    // Check that origin is translated
    Vector3D newOrigin = transformed.getOrigin();
    EXPECT_DOUBLE_EQ(newOrigin.getX(), 3.0); // 1.0 + 2.0
    EXPECT_DOUBLE_EQ(newOrigin.getY(), 5.0); // 2.0 + 3.0
    EXPECT_DOUBLE_EQ(newOrigin.getZ(), 7.0); // 3.0 + 4.0

    // Direction should not change with a pure translation
    Vector3D newDirection = transformed.getDirection();
    EXPECT_DOUBLE_EQ(newDirection.getX(), 0.0);
    EXPECT_DOUBLE_EQ(newDirection.getY(), 1.0);
    EXPECT_DOUBLE_EQ(newDirection.getZ(), 0.0);
}

// Test transform method with rotation
TEST(RayTest, TransformRotation)
{
    Vector3D origin(1.0, 0.0, 0.0);
    Vector3D direction(1.0, 0.0, 0.0); // Unit vector along X axis

    Ray ray(origin, direction);

    // Create a 90-degree rotation around Z axis
    Transform transform;
    transform.rotateZ(90.0); // Rotation in degrees

    Ray transformed = ray.transform(transform);

    // Check that origin is rotated (should become approximately (0, 1, 0))
    Vector3D newOrigin = transformed.getOrigin();
    EXPECT_NEAR(newOrigin.getX(), 0.0, 1e-10);
    EXPECT_NEAR(newOrigin.getY(), 1.0, 1e-10);
    EXPECT_NEAR(newOrigin.getZ(), 0.0, 1e-10);

    // Direction should also be rotated (should become approximately (0, 1, 0))
    Vector3D newDirection = transformed.getDirection();
    EXPECT_NEAR(newDirection.getX(), 0.0, 1e-10);
    EXPECT_NEAR(newDirection.getY(), 1.0, 1e-10);
    EXPECT_NEAR(newDirection.getZ(), 0.0, 1e-10);
}

// Test complex transformation (combination of rotate + translate)
TEST(RayTest, ComplexTransform)
{
    Vector3D origin(1.0, 0.0, 0.0);
    Vector3D direction(1.0, 0.0, 0.0); // Unit vector along X axis

    Ray ray(origin, direction);

    // Create a combination of transformations:
    // First rotate 90 degrees around Z, then translate by (0,2,0)
    Transform transform;
    transform.rotateZ(90.0); // Rotation in degrees
    transform.translate(0.0, 2.0, 0.0);

    Ray transformed = ray.transform(transform);

    // Check that origin is correctly transformed
    // After rotation: (0, 1, 0)
    // After translation: (0, 3, 0)
    Vector3D newOrigin = transformed.getOrigin();
    EXPECT_NEAR(newOrigin.getX(), 0.0, 1e-10);
    EXPECT_NEAR(newOrigin.getY(), 3.0, 1e-10);
    EXPECT_NEAR(newOrigin.getZ(), 0.0, 1e-10);

    // Direction should only be affected by rotation (should become (0, 1, 0))
    // since direction vectors are not affected by translation
    Vector3D newDirection = transformed.getDirection();
    EXPECT_NEAR(newDirection.getX(), 0.0, 1e-10);
    EXPECT_NEAR(newDirection.getY(), 1.0, 1e-10);
    EXPECT_NEAR(newDirection.getZ(), 0.0, 1e-10);
}