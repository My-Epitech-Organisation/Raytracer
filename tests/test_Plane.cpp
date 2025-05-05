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

// Keep the old helper for non-failing tests for brevity if needed
bool vectorsNearlyEqualPlaneTest(const Vector3D& v1, const Vector3D& v2,
                                 double epsilon = 1e-5) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  return dx < epsilon && dy < epsilon && dz < epsilon;
}

// Test constructor
TEST(PlaneTest, Constructor) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);      // Y-up plane
  Color color(255.0, 0.0, 0.0);  // Red
  Plane plane(point, normal, color);

  // Use a dummy point for getNormalAt as it's ignored for Plane
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper,
                      plane.getNormalAt({1, 1, 1}), Vector3D(0, 1, 0), 1e-5);
  EXPECT_EQ(plane.getColor(), color);
  // Check if the internal normal is normalized
  EXPECT_NEAR(plane.getNormalAt({}).getMagnitude(), 1.0, 1e-9);
}

// Test intersection with a ray hitting the plane
TEST(PlaneTest, RayIntersectionHit) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);      // XY plane at y=0
  Color color(0.0, 255.0, 0.0);  // Green
  Plane plane(point, normal, color);

  Ray ray(Vector3D(0, 10, 0),
          Vector3D(0, -1, 0));  // Ray pointing straight down along -Y
  std::optional<Intersection> intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 10.0, 1e-5);
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->point,
                      Vector3D(0, 0, 0), 1e-5);
  // Normal should point opposite to the ray direction for a hit from above
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->normal,
                      Vector3D(0, 1, 0), 1e-5);
  EXPECT_EQ(intersection->color, color);
  EXPECT_EQ(intersection->primitive, &plane);
}

// Test intersection with a ray hitting the plane from below
TEST(PlaneTest, RayIntersectionHitFromBelow) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);      // XY plane at y=0, normal points up
  Color color(0.0, 0.0, 255.0);  // Blue
  Plane plane(point, normal, color);

  Ray ray(Vector3D(0, -10, 0),
          Vector3D(0, 1, 0));  // Ray pointing straight up along +Y
  std::optional<Intersection> intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 10.0, 1e-5);
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->point,
                      Vector3D(0, 0, 0), 1e-5);
  // Normal should point opposite to the ray direction
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->normal,
                      Vector3D(0, -1, 0), 1e-5);
  EXPECT_EQ(intersection->color, color);
}

// Test intersection with a ray parallel to the plane
TEST(PlaneTest, RayIntersectionParallel) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);        // XY plane at y=0
  Color color(255.0, 255.0, 0.0);  // Yellow
  Plane plane(point, normal, color);

  Ray ray(Vector3D(0, 10, 0), Vector3D(1, 0, 0));  // Ray parallel to the plane
  std::optional<Intersection> intersection = plane.intersect(ray);

  EXPECT_FALSE(intersection.has_value());
}

// Test intersection with a ray starting on the plane (should not intersect due
// to epsilon check)
TEST(PlaneTest, RayIntersectionOnPlane) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);        // XY plane at y=0
  Color color(255.0, 0.0, 255.0);  // Magenta
  Plane plane(point, normal, color);

  Ray ray(Vector3D(1, 0, 1),
          Vector3D(1, 0, 0));  // Ray starting on the plane, moving parallel
  std::optional<Intersection> intersection = plane.intersect(ray);
  EXPECT_FALSE(
      intersection.has_value());  // t is near zero or denominator is zero

  Ray ray2(Vector3D(1, 0, 1),
           Vector3D(0, 1,
                    0));  // Ray starting on the plane, moving away along normal
  intersection = plane.intersect(ray2);
  EXPECT_FALSE(intersection.has_value());  // t is near zero
}

// Test intersection with a ray moving away from the plane
TEST(PlaneTest, RayIntersectionMovingAway) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);        // XY plane at y=0
  Color color(0.0, 255.0, 255.0);  // Cyan
  Plane plane(point, normal, color);

  Ray ray(Vector3D(0, 10, 0),
          Vector3D(0, 1, 0));  // Ray starting above, moving further up
  std::optional<Intersection> intersection = plane.intersect(ray);

  EXPECT_FALSE(intersection.has_value());  // t would be negative
}

// Test intersection with a transformed plane (rotated)
TEST(PlaneTest, TransformedPlaneIntersectionRotated) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);          // XY plane at y=0
  Color color(128.0, 128.0, 128.0);  // Gray
  Plane plane(point, normal, color);

  Transform transform;
  transform.rotateZ(45.0);  // Use degrees instead of M_PI / 4.0
  plane.setTransform(transform);

  // Ray pointing straight down along -Y
  Ray ray(Vector3D(0, 10, 0), Vector3D(0, -1, 0));
  std::optional<Intersection> intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());

  // Use helper for point comparison too, just in case
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->point,
                      Vector3D(0, 0, 0), 1e-5);
  EXPECT_NEAR(intersection->distance, 10.0, 1e-5);

  // Expected normal after rotation (rotated (0,1,0) by 45 deg around Z)
  Vector3D expectedNormal =
      Vector3D(-sin(M_PI / 4.0), cos(M_PI / 4.0), 0).normalized();

  // Check normal from getNormalAt with detailed output
  Vector3D rawWorldNormal = plane.getNormalAt({0, 0, 0});
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, rawWorldNormal,
                      expectedNormal, 1e-5);

  // Check normal from intersect method with detailed output
  // Based on previous analysis, intersection->normal should equal
  // expectedNormal here
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->normal,
                      expectedNormal, 1e-5);
}

// Test intersection with a transformed plane (translated)
TEST(PlaneTest, TransformedPlaneIntersectionTranslated) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 1, 0);        // XY plane at y=0
  Color color(255.0, 128.0, 0.0);  // Orange
  Plane plane(point, normal, color);

  Transform transform;
  transform.translate(0, 5, 0);  // Move plane up by 5 units (now at y=5)
  plane.setTransform(transform);

  Ray ray(Vector3D(0, 10, 0),
          Vector3D(0, -1, 0));  // Ray pointing straight down
  std::optional<Intersection> intersection = plane.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 5.0, 1e-5);
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->point,
                      Vector3D(0, 5, 0), 1e-5);
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper, intersection->normal,
                      Vector3D(0, 1, 0), 1e-5);
}

// Test getNormalAt method with transformation
TEST(PlaneTest, GetNormalAtTransformed) {
  Vector3D point(0, 0, 0);
  Vector3D normal(0, 0, 1);  // XY plane at z=0
  Plane plane(point, normal, Color::WHITE);

  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper,
                      plane.getNormalAt(Vector3D(10, 20, 0)), Vector3D(0, 0, 1),
                      1e-5);

  Transform transform;
  transform.rotateY(90.0);  // Use degrees instead of M_PI / 2.0
  plane.setTransform(transform);

  // The normal (0,0,1) rotated 90 deg around Y becomes (1,0,0)
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper,
                      plane.getNormalAt(Vector3D(0, 5, 5)), Vector3D(1, 0, 0),
                      1e-5);
}

// Test setColor and getColor
TEST(PlaneTest, SetGetColor) {
  Plane plane(Vector3D(0, 0, 0), Vector3D(0, 1, 0), Color::RED);
  EXPECT_EQ(plane.getColor(), Color::RED);
  plane.setColor(Color::BLUE);
  EXPECT_EQ(plane.getColor(), Color::BLUE);
}

// Test setTransform and getTransform
TEST(PlaneTest, SetGetTransform) {
  Plane plane(Vector3D(0, 0, 0), Vector3D(0, 1, 0), Color::WHITE);
  Transform t;
  t.translate(1, 2, 3);
  t.rotateX(M_PI / 6.0);  // 30 degrees
  plane.setTransform(t);

  Transform retrieved = plane.getTransform();
  Matrix mOriginal = t.getMatrix();
  Matrix mRetrieved = retrieved.getMatrix();

  // Check if the matrices are the same
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_DOUBLE_EQ(mRetrieved.at(i, j), mOriginal.at(i, j));
    }
  }
}

// Test clone method
TEST(PlaneTest, Clone) {
  Plane original(Vector3D(1, 2, 3), Vector3D(1, 0, 0), Color::GREEN);
  Transform t;
  t.scale(2.0, 0.5, 1.0);
  t.translate(5, 0, -2);
  original.setTransform(t);

  std::shared_ptr<IPrimitive> clonedBase = original.clone();
  ASSERT_NE(clonedBase, nullptr);

  // Try casting to Plane
  std::shared_ptr<Plane> clonedPlane =
      std::dynamic_pointer_cast<Plane>(clonedBase);
  ASSERT_NE(clonedPlane, nullptr);

  // Ensure it's a different object in memory
  EXPECT_NE(clonedPlane.get(), &original);

  // Check properties are the same
  EXPECT_EQ(clonedPlane->getColor(), original.getColor());
  // Need to compare internal point and normal? No, clone copies constructor
  // args. Check transformed normal
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper,
                      clonedPlane->getNormalAt({}), original.getNormalAt({}),
                      1e-5);

  // Check transform (compare matrices)
  Matrix mOriginal = original.getTransform().getMatrix();
  Matrix mCloned = clonedPlane->getTransform().getMatrix();
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_DOUBLE_EQ(mCloned.at(i, j), mOriginal.at(i, j));
    }
  }

  // Verify intersection behavior is the same
  Ray ray(Vector3D(10, 10, 10), Vector3D(-1, -1, -1));
  std::optional<Intersection> intersectionOriginal = original.intersect(ray);
  std::optional<Intersection> intersectionCloned = clonedPlane->intersect(ray);

  if (intersectionOriginal.has_value()) {
    ASSERT_TRUE(intersectionCloned.has_value());
    EXPECT_NEAR(intersectionCloned->distance, intersectionOriginal->distance,
                1e-5);
    EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper,
                        intersectionCloned->point, intersectionOriginal->point,
                        1e-5);
    EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualHelper,
                        intersectionCloned->normal,
                        intersectionOriginal->normal, 1e-5);
    EXPECT_EQ(intersectionCloned->color, intersectionOriginal->color);
  } else {
    EXPECT_FALSE(intersectionCloned.has_value());
  }
}
