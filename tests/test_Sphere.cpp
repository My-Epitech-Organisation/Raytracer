/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Sphere primitive
*/

/**
 * @file test_Sphere.cpp
 * @brief Unit tests for the Sphere primitive class to validate geometry and ray
 * intersections
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include "../include/exceptions/InvalidTypeException.hpp"
#include "../include/exceptions/ParserException.hpp"
#include "../include/exceptions/RaytracerException.hpp"
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/Sphere.hpp"

using namespace RayTracer;

// Helper function to compare Vector3D with tolerance
bool vectorsNearlyEqual_Sphere(const Vector3D& v1, const Vector3D& v2,
                               double epsilon = 1e-5) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  return dx < epsilon && dy < epsilon && dz < epsilon;
}

// Test constructors
TEST(SphereTest, DefaultConstructor) {
  Sphere sphere;

  EXPECT_TRUE(vectorsNearlyEqual_Sphere(sphere.getCenter(), Vector3D(0, 0, 0)));
  EXPECT_DOUBLE_EQ(sphere.getRadius(), 1.0);
  EXPECT_EQ(sphere.getColor(), Color::RED);
}

TEST(SphereTest, ParameterizedConstructor) {
  Vector3D center(1, 2, 3);
  double radius = 2.5;
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64),
              static_cast<uint8_t>(32));

  Sphere sphere(center, radius, color);

  EXPECT_TRUE(vectorsNearlyEqual_Sphere(sphere.getCenter(), center));
  EXPECT_DOUBLE_EQ(sphere.getRadius(), radius);
  EXPECT_EQ(sphere.getColor(), color);
}

TEST(SphereTest, InvalidRadius) {
  Vector3D center(1, 2, 3);
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64),
              static_cast<uint8_t>(32));

  EXPECT_THROW(Sphere(center, -1.0, color), InvalidTypeException);
  EXPECT_THROW(Sphere(center, 0.0, color), InvalidTypeException);
}

// Test getters and setters
TEST(SphereTest, SetCenter) {
  Sphere sphere;
  Vector3D newCenter(5, 6, 7);

  sphere.setCenter(newCenter);
  EXPECT_TRUE(vectorsNearlyEqual_Sphere(sphere.getCenter(), newCenter));
}

TEST(SphereTest, SetRadius) {
  Sphere sphere;
  double newRadius = 3.5;

  sphere.setRadius(newRadius);
  EXPECT_DOUBLE_EQ(sphere.getRadius(), newRadius);
}

TEST(SphereTest, SetInvalidRadius) {
  Sphere sphere;

  EXPECT_THROW(sphere.setRadius(-1.0), InvalidTypeException);
  EXPECT_THROW(sphere.setRadius(0.0), InvalidTypeException);
}

TEST(SphereTest, SetColor) {
  Sphere sphere;
  Color newColor(static_cast<uint8_t>(50), static_cast<uint8_t>(100),
                 static_cast<uint8_t>(150));

  sphere.setColor(newColor);
  EXPECT_EQ(sphere.getColor(), newColor);
}

TEST(SphereTest, SetTransform) {
  Sphere sphere;
  Transform transform;
  transform.translate(1, 2, 3);
  transform.rotateX(45);

  sphere.setTransform(transform);
  EXPECT_EQ(sphere.getTransform(), transform);
}

// Test clone method
TEST(SphereTest, Clone) {
  Vector3D center(1, 2, 3);
  double radius = 2.5;
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64),
              static_cast<uint8_t>(32));
  Transform transform;
  transform.translate(4, 5, 6);

  Sphere original(center, radius, color);
  original.setTransform(transform);

  std::shared_ptr<IPrimitive> clone = original.clone();
  std::shared_ptr<Sphere> sphereClone =
      std::dynamic_pointer_cast<Sphere>(clone);

  ASSERT_NE(sphereClone, nullptr);
  EXPECT_TRUE(vectorsNearlyEqual_Sphere(sphereClone->getCenter(), center));
  EXPECT_DOUBLE_EQ(sphereClone->getRadius(), radius);
  EXPECT_EQ(sphereClone->getColor(), color);
  EXPECT_EQ(sphereClone->getTransform(), transform);
}

// Test ray intersection with sphere at origin
TEST(SphereTest, RayIntersectionFromOutside) {
  Sphere sphere;  // Unit sphere at origin

  // Ray pointing directly at the sphere from outside
  Ray ray(Vector3D(0, 0, -5), Vector3D(0, 0, 1));

  auto intersection = sphere.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance,
                   4.0);  // Ray starts at z=-5, sphere surface is at z=-1
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->point, Vector3D(0, 0, -1)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->normal, Vector3D(0, 0, -1)));
  EXPECT_EQ(intersection->color, Color::RED);
  EXPECT_EQ(intersection->primitive, &sphere);
}

TEST(SphereTest, RayIntersectionFromInside) {
  Sphere sphere;  // Unit sphere at origin

  // Ray pointing outward from inside the sphere
  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, 1));

  auto intersection = sphere.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance,
                   1.0);  // Ray starts at center, sphere surface is 1 unit away
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->point, Vector3D(0, 0, 1)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->normal, Vector3D(0, 0, 1)));
  EXPECT_EQ(intersection->color, Color::RED);
  EXPECT_EQ(intersection->primitive, &sphere);
}

TEST(SphereTest, RayMissesSphere) {
  Sphere sphere;  // Unit sphere at origin

  // Ray passing to the side of the sphere
  Ray ray(Vector3D(0, 2, -5), Vector3D(0, 0, 1));

  auto intersection = sphere.intersect(ray);

  EXPECT_FALSE(intersection.has_value());
}

TEST(SphereTest, RayStartsBehindSphere) {
  Sphere sphere;  // Unit sphere at origin

  // Ray pointing away from the sphere, starting behind it
  Ray ray(Vector3D(0, 0, 5), Vector3D(0, 0, 1));

  auto intersection = sphere.intersect(ray);

  EXPECT_FALSE(intersection.has_value());
}

// Test ray intersection with transformed sphere
TEST(SphereTest, IntersectWithTranslatedSphere) {
  Sphere sphere;  // Unit sphere at origin

  // Translate the sphere to (0, 0, 5)
  Transform transform;
  transform.translate(0, 0, 5);
  sphere.setTransform(transform);

  // Ray pointing along positive z-axis from origin
  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, 1));

  auto intersection = sphere.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance,
                   4.0);  // Sphere surface is 4 units from ray origin
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->point, Vector3D(0, 0, 4)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->normal, Vector3D(0, 0, -1)));
}

TEST(SphereTest, IntersectWithScaledSphere) {
  Sphere sphere;  // Unit sphere at origin

  // Scale the sphere to twice its size
  Transform transform;
  transform.scale(2, 2, 2);
  sphere.setTransform(transform);

  // Ray pointing directly at the sphere
  Ray ray(Vector3D(0, 0, -5), Vector3D(0, 0, 1));

  auto intersection = sphere.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance,
                   3.0);  // Sphere surface is now at z=-2
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->point, Vector3D(0, 0, -2)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(intersection->normal, Vector3D(0, 0, -1)));
}

// Test normals
TEST(SphereTest, NormalAtPointOnAxisX) {
  Sphere sphere;  // Unit sphere at origin

  Vector3D normal = sphere.getNormalAt(Vector3D(1, 0, 0));
  EXPECT_TRUE(vectorsNearlyEqual_Sphere(normal, Vector3D(1, 0, 0)));
}

TEST(SphereTest, NormalAtPointOnAxisY) {
  Sphere sphere;  // Unit sphere at origin

  Vector3D normal = sphere.getNormalAt(Vector3D(0, 1, 0));
  EXPECT_TRUE(vectorsNearlyEqual_Sphere(normal, Vector3D(0, 1, 0)));
}

TEST(SphereTest, NormalAtPointOnAxisZ) {
  Sphere sphere;  // Unit sphere at origin

  Vector3D normal = sphere.getNormalAt(Vector3D(0, 0, 1));
  EXPECT_TRUE(vectorsNearlyEqual_Sphere(normal, Vector3D(0, 0, 1)));
}

TEST(SphereTest, NormalAtNonAxisPoint) {
  Sphere sphere;  // Unit sphere at origin

  // Point at (√3/3, √3/3, √3/3) - a point on the unit sphere
  double sqrt3_3 = std::sqrt(3.0) / 3.0;
  Vector3D point(sqrt3_3, sqrt3_3, sqrt3_3);

  Vector3D normal = sphere.getNormalAt(point);
  EXPECT_TRUE(vectorsNearlyEqual_Sphere(
      normal, point));  // Normal should be the same as the point (normalized)
}

TEST(SphereTest, NormalOnTransformedSphere) {
  Sphere sphere;  // Unit sphere at origin

  // Apply translation and scaling
  Transform transform;
  transform.translate(0, 1, 0);
  transform.scale(1, 0.5, 1);
  sphere.setTransform(transform);

  // Test point on the transformed sphere
  Vector3D point = transform.applyToPoint(Vector3D(0, 0, 1));
  Vector3D normal = sphere.getNormalAt(point);

  // Expected normal after transformation
  // For a point on the z-axis (0,0,1), scaling by (1,0.5,1) would make the
  // normal (0,0,1) When transformed by the transpose of the inverse of the
  // transform
  EXPECT_TRUE(
      vectorsNearlyEqual_Sphere(normal.normalized(), Vector3D(0, 0, 1)));
}
