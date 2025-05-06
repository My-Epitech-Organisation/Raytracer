/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Plane class
*/

#include <gtest/gtest.h>
#include <cmath>
#include <iomanip>  // Added for detailed output
#include <limits>
#include <sstream>  // Added for detailed output
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/Plane.hpp"

using namespace RayTracer;

// Helper function to compare Vector3D with tolerance and provide detailed
// output
::testing::AssertionResult AssertVectorsNearlyEqualHelper(
    const char* v1_expr, const char* v2_expr, const char* epsilon_expr,
    const Vector3D& v1, const Vector3D& v2, double epsilon) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  bool pass = dx < epsilon && dy < epsilon && dz < epsilon;

  if (pass) {
    return ::testing::AssertionSuccess();
  } else {
    // Try to format Vector3D nicely if possible, otherwise use basic format
    // Assuming Vector3D doesn't have a << operator or toString()
    std::stringstream ss;
    ss << std::fixed << std::setprecision(9) << "Vectors " << v1_expr << " and "
       << v2_expr << " are not nearly equal (within " << epsilon_expr << ").\n"
       << v1_expr << ": (" << v1.getX() << ", " << v1.getY() << ", "
       << v1.getZ() << ")\n"
       << v2_expr << ": (" << v2.getX() << ", " << v2.getY() << ", "
       << v2.getZ() << ")\n"
       << "Difference: (" << dx << ", " << dy << ", " << dz << ")\n"
       << "Epsilon: " << epsilon;
    return ::testing::AssertionFailure() << ss.str();
  }
}

// Macro for easier usage of the helper
#define EXPECT_VECTORS_NEARLY_EQUAL(v1, v2, epsilon) \
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, v1, v2, epsilon)

// Keep the old helper for non-failing tests for brevity if needed
bool vectorsNearlyEqualPlaneTest(const Vector3D& v1, const Vector3D& v2,
                                 double epsilon = 1e-5) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  return dx < epsilon && dy < epsilon && dz < epsilon;
}

// Helper function to compare Intersection with tolerance
bool intersectionsNearlyEqual_Plane(const Intersection& i1,
                                    const Intersection& i2,
                                    double epsilon = 1e-5) {
  return (std::abs(i1.distance - i2.distance) < epsilon &&
          vectorsNearlyEqualPlaneTest(i1.point, i2.point, epsilon) &&
          vectorsNearlyEqualPlaneTest(i1.normal, i2.normal, epsilon) &&
          i1.color == i2.color);
}

// Test constructor
TEST(PlaneTest, Constructor) {
  // Test X-axis plane
  Plane planeX(Axis::X, 10.0, Color::RED);
  EXPECT_VECTORS_NEARLY_EQUAL(planeX.getNormalAt(Vector3D(10, 5, 2)),
                              Vector3D(1, 0, 0), 1e-9);
  EXPECT_EQ(planeX.getColor(), Color::RED);

  // Test Y-axis plane
  Plane planeY(Axis::Y, -5.0, Color::GREEN);
  EXPECT_VECTORS_NEARLY_EQUAL(planeY.getNormalAt(Vector3D(1, -5, 3)),
                              Vector3D(0, 1, 0), 1e-9);
  EXPECT_EQ(planeY.getColor(), Color::GREEN);

  // Test Z-axis plane
  Plane planeZ(Axis::Z, 0.0, Color::BLUE);
  EXPECT_VECTORS_NEARLY_EQUAL(planeZ.getNormalAt(Vector3D(2, 3, 0)),
                              Vector3D(0, 0, 1), 1e-9);
  EXPECT_EQ(planeZ.getColor(), Color::BLUE);

  // Invalid axis check removed as Axis is a scoped enum
}

// Test intersection with a ray hitting the plane
TEST(PlaneTest, RayIntersectionHit) {
  Plane plane(Axis::Z, 0.0, Color::BLUE);          // XY plane at z=0
  Ray ray(Vector3D(0, 0, -5), Vector3D(0, 0, 1));  // Ray along +Z from below

  auto intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance, 5.0);
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->point, Vector3D(0, 0, 0), 1e-9);
  // Normal should point towards the ray origin
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->normal, Vector3D(0, 0, -1), 1e-9);
  EXPECT_EQ(intersection->color, Color::BLUE);
  EXPECT_EQ(intersection->primitive, &plane);
}

// Test intersection with a ray hitting the plane from below
TEST(PlaneTest, RayIntersectionHitFromBelow) {
  Plane plane(Axis::Z, 0.0, Color::BLUE);          // XY plane at z=0
  Ray ray(Vector3D(1, 2, 5), Vector3D(0, 0, -1));  // Ray along -Z from above

  auto intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance, 5.0);
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->point, Vector3D(1, 2, 0), 1e-9);
  // Normal should point towards the ray origin
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->normal, Vector3D(0, 0, 1), 1e-9);
  EXPECT_EQ(intersection->color, Color::BLUE);
}

// Test intersection with a ray parallel to the plane
TEST(PlaneTest, RayIntersectionParallel) {
  Plane plane(Axis::Z, 0.0, Color::BLUE);          // XY plane at z=0
  Ray ray(Vector3D(0, 0, -5), Vector3D(1, 0, 0));  // Ray parallel to the plane

  auto intersection = plane.intersect(ray);

  EXPECT_FALSE(intersection.has_value());
}

// Test intersection with a ray starting on the plane (should not intersect due
// to epsilon check)
TEST(PlaneTest, RayIntersectionOnPlane) {
  Plane plane(Axis::Y, 10.0, Color::GREEN);  // XZ plane at y=10
  Ray ray(Vector3D(1, 10, 2),
          Vector3D(1, 0, 0));  // Ray starts on the plane, moves parallel

  auto intersection = plane.intersect(ray);

  // Intersection at t=0 is usually ignored (or handled carefully)
  // Depending on EPSILON, it might be detected or not.
  // For simplicity, let's assume t near zero means no hit for practical
  // rendering.
  EXPECT_FALSE(intersection.has_value());

  // Test ray starting on plane but moving towards it (should not hit)
  Ray ray2(Vector3D(1, 10, 2), Vector3D(0, -1, 0));
  auto intersection2 = plane.intersect(ray2);
  EXPECT_FALSE(intersection2.has_value());
}

// Test intersection with a ray moving away from the plane
TEST(PlaneTest, RayIntersectionMovingAway) {
  Plane plane(Axis::X, -20.0, Color::RED);  // YZ plane at x=-20
  Ray ray(Vector3D(0, 0, 0),
          Vector3D(1, 0, 0));  // Ray starts at origin, moves away from plane

  auto intersection = plane.intersect(ray);

  EXPECT_FALSE(intersection.has_value());
}

// Test intersection with a transformed plane (rotated)
TEST(PlaneTest, TransformedPlaneIntersectionRotated) {
  Plane plane(Axis::Z, 0.0, Color::BLUE);  // XY plane at z=0 initially
  Transform transform;
  transform.rotateX(45.0);  // Rotate plane 45 degrees around X-axis
  plane.setTransform(transform);

  // Ray along -Y axis towards the rotated plane
  Ray ray(Vector3D(0, 5, 0), Vector3D(0, -1, 0));

  auto intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());

  // Expected intersection point:
  // The plane equation after rotation is roughly y - z = 0.
  // Ray: P(t) = (0, 5, 0) + t*(0, -1, 0) = (0, 5-t, 0)
  // Intersection: (5-t) - 0 = 0 => t = 5
  // Point: (0, 5-5, 0) = (0, 0, 0)
  EXPECT_NEAR(intersection->distance, 5.0, 1e-5);
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->point, Vector3D(0, 0, 0), 1e-5);

  // Expected normal:
  // Original normal (0, 0, 1). Rotated by 45 deg around X gives (0, -sin(45),
  // cos(45)). Ray direction is (0, -1, 0). Dot product is sin(45) > 0, so the
  // normal is flipped by intersect(). Final normal should be -(0, -sin(45),
  // cos(45)) = (0, sin45, -cos45).
  double cos45 = std::cos(M_PI / 4.0);
  double sin45 = std::sin(M_PI / 4.0);
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->normal, Vector3D(0, sin45, -cos45),
                              1e-5);
}

// Test intersection with a transformed plane (translated)
TEST(PlaneTest, TransformedPlaneIntersectionTranslated) {
  Plane plane(Axis::Z, 0.0, Color::BLUE);  // XY plane at z=0 initially
  Transform transform;
  transform.translate(0, 0, 10);  // Move plane up to z=10
  plane.setTransform(transform);

  Ray ray(Vector3D(0, 0, 0), Vector3D(0, 0, 1));  // Ray along +Z from origin

  auto intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_DOUBLE_EQ(intersection->distance, 10.0);
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->point, Vector3D(0, 0, 10), 1e-9);
  // Normal should point towards ray origin (-Z direction)
  EXPECT_VECTORS_NEARLY_EQUAL(intersection->normal, Vector3D(0, 0, -1), 1e-9);
}

// Test getNormalAt method with transformation
TEST(PlaneTest, GetNormalAtTransformed) {
  Plane plane(Axis::Z, 0.0, Color::WHITE);  // XY plane at z=0
  Transform transform;
  transform.rotateX(90.0);  // Rotate plane 90 degrees around X-axis -> becomes
                            // XZ plane at y=0
  plane.setTransform(transform);

  // Original normal (0,0,1). Transformed normal should be (0,-1,0).
  Vector3D expectedNormal(0, -1, 0);

  // Test at a point that would be on the transformed plane (e.g., origin)
  EXPECT_VECTORS_NEARLY_EQUAL(plane.getNormalAt(Vector3D(0, 0, 0)),
                              expectedNormal, 1e-5);
  // Test at another point
  EXPECT_VECTORS_NEARLY_EQUAL(plane.getNormalAt(Vector3D(10, 0, 5)),
                              expectedNormal, 1e-5);
}

// Test setColor and getColor
TEST(PlaneTest, SetGetColor) {
  Plane plane(Axis::Y, 0, Color::RED);
  EXPECT_EQ(plane.getColor(), Color::RED);

  plane.setColor(Color::GREEN);
  EXPECT_EQ(plane.getColor(), Color::GREEN);
}

// Test setTransform and getTransform
TEST(PlaneTest, SetGetTransform) {
  Plane plane(Axis::Z, 10, Color::WHITE);
  Transform t1;
  t1.translate(1, 2, 3);
  plane.setTransform(t1);
  // Compare matrices as Transform might not have operator==
  EXPECT_EQ(plane.getTransform().getMatrix(), t1.getMatrix());

  Transform t2;
  t2.rotateY(45);
  plane.setTransform(t2);
  EXPECT_EQ(plane.getTransform().getMatrix(), t2.getMatrix());
  EXPECT_NE(plane.getTransform().getMatrix(),
            t1.getMatrix());  // Ensure it actually changed
}

// Test clone method
TEST(PlaneTest, Clone) {
  Axis axis = Axis::X;
  double position = -5.5;
  Color color = Color::CYAN;
  Transform transform;
  transform.translate(1, 2, 3);
  transform.rotateZ(30);

  Plane original(axis, position, color);
  original.setTransform(transform);

  std::shared_ptr<IPrimitive> cloneBase = original.clone();
  ASSERT_NE(cloneBase, nullptr);

  // Check if it's actually a Plane
  std::shared_ptr<Plane> clone = std::dynamic_pointer_cast<Plane>(cloneBase);
  ASSERT_NE(clone, nullptr);

  // Check if properties are copied
  EXPECT_EQ(clone->getColor(), color);
  // Compare matrices for transform equality
  EXPECT_EQ(clone->getTransform().getMatrix(), transform.getMatrix());

  // Check intersection behavior (indirectly checks axis/position)
  Ray ray(Vector3D(0, 2, 3), Vector3D(-1, 0, 0));  // Ray towards the plane
  auto originalIntersection = original.intersect(ray);
  auto cloneIntersection = clone->intersect(ray);

  ASSERT_EQ(originalIntersection.has_value(), cloneIntersection.has_value());
  if (originalIntersection.has_value()) {
    EXPECT_TRUE(intersectionsNearlyEqual_Plane(*originalIntersection,
                                               *cloneIntersection, 1e-5));
  }

  // Ensure it's a deep copy (modifying clone doesn't affect original)
  clone->setColor(Color::MAGENTA);
  EXPECT_NE(original.getColor(), clone->getColor());

  Transform t2;
  clone->setTransform(t2);  // Reset transform
  EXPECT_NE(original.getTransform().getMatrix(),
            clone->getTransform().getMatrix());
}
