/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Cylinder class
*/

/**
 * @file test_Cylinder.cpp
 * @brief Unit tests for the Cylinder primitive class to verify its geometry and
 * intersection calculations
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/Cylinder.hpp"

using namespace RayTracer;

// Helper to compare Vector3D with tolerance (from test_Plane.cpp)
::testing::AssertionResult AssertVectorsNearlyEqualCylinderTest(
    const char* v1_expr, const char* v2_expr, const char* epsilon_expr,
    const Vector3D& v1, const Vector3D& v2, double epsilon) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  bool pass = dx < epsilon && dy < epsilon && dz < epsilon;
  if (pass) {
    return ::testing::AssertionSuccess();
  } else {
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
#define EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(v1, v2, epsilon) \
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualCylinderTest, v1, v2, epsilon)

TEST(CylinderTest, ConstructorValid) {
  Cylinder cyl(1.0, 2.0, Color::RED);
  EXPECT_EQ(cyl.getRadius(), 1.0);
  EXPECT_EQ(cyl.getHeight(), 2.0);
  EXPECT_EQ(cyl.getColor(), Color::RED);
}

TEST(CylinderTest, ConstructorInvalidRadius) {
  EXPECT_THROW(Cylinder(0.0, 2.0, Color::RED), std::invalid_argument);
  EXPECT_THROW(Cylinder(-1.0, 2.0, Color::RED), std::invalid_argument);
}

TEST(CylinderTest, ConstructorInvalidHeight) {
  EXPECT_THROW(Cylinder(1.0, 0.0, Color::RED), std::invalid_argument);
  EXPECT_THROW(Cylinder(1.0, -2.0, Color::RED), std::invalid_argument);
}

TEST(CylinderTest, RayIntersectionMiss) {
  Cylinder cyl(1.0, 2.0, Color::BLUE);
  Ray ray(Vector3D(0, 0, -5),
          Vector3D(0, 1, 0));  // Ray parallel to Y, outside cylinder body
  auto intersection = cyl.intersect(ray);
  EXPECT_FALSE(intersection.has_value());

  Ray ray2(Vector3D(5, 0, 0),
           Vector3D(0, 0, 1));  // Ray along Z, outside cylinder body
  auto intersection2 = cyl.intersect(ray2);
  EXPECT_FALSE(intersection2.has_value());
}

TEST(CylinderTest, RayIntersectionBody) {
  Cylinder cyl(
      1.0, 2.0,
      Color::GREEN);  // Centered at origin, radius 1, height 2 (-1 to 1 on Y)
  Ray ray(Vector3D(0, 0, -5), Vector3D(0, 0, 1));  // Ray along +Z, hits body

  auto intersection = cyl.intersect(ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 4.0, CYLINDER_EPSILON);  // Hits at z=-1
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->point, Vector3D(0, 0, -1),
                                       CYLINDER_EPSILON);
  // Normal should point towards ray origin (-Z direction)
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->normal, Vector3D(0, 0, -1),
                                       CYLINDER_EPSILON);
  EXPECT_EQ(intersection->color, Color::GREEN);
}

TEST(CylinderTest, RayIntersectionTopCap) {
  Cylinder cyl(1.0, 2.0, Color::RED);              // Height from -1 to 1 on Y
  Ray ray(Vector3D(0, 5, 0), Vector3D(0, -1, 0));  // Ray along -Y, hits top cap

  auto intersection = cyl.intersect(ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 4.0, CYLINDER_EPSILON);  // Hits at y=1
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->point, Vector3D(0, 1, 0),
                                       CYLINDER_EPSILON);
  // Normal should point towards ray origin (+Y direction)
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->normal, Vector3D(0, 1, 0),
                                       CYLINDER_EPSILON);
}

TEST(CylinderTest, RayIntersectionBottomCap) {
  Cylinder cyl(1.0, 2.0, Color::BLUE);
  Ray ray(Vector3D(0.5, -5, 0.5),
          Vector3D(0, 1, 0));  // Ray along +Y, hits bottom cap

  auto intersection = cyl.intersect(ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 4.0, CYLINDER_EPSILON);  // Hits at y=-1
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(
      intersection->point, Vector3D(0.5, -1, 0.5), CYLINDER_EPSILON);
  // Normal should point towards ray origin (-Y direction)
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->normal, Vector3D(0, -1, 0),
                                       CYLINDER_EPSILON);
}

TEST(CylinderTest, RayParallelInsideNoHitBody) {
  Cylinder cyl(1.0, 2.0, Color::YELLOW);
  // Ray parallel to Y-axis, starting inside the cylinder's radius, pointing
  // upwards
  Ray ray(Vector3D(0.5, -5, 0), Vector3D(0, 1, 0));
  auto intersection = cyl.intersect(ray);
  // Should hit the bottom cap first
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->distance, 4.0,
              CYLINDER_EPSILON);  // Hits bottom cap at y=-1
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(
      intersection->point, Vector3D(0.5, -1.0, 0), CYLINDER_EPSILON);
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->normal, Vector3D(0, -1, 0),
                                       CYLINDER_EPSILON);
}

TEST(CylinderTest, RayGrazingEdgeShouldHitCap) {
  Cylinder cyl(1.0, 2.0, Color::CYAN);
  // Ray grazes the top edge, should hit the cap, not the body at y=1
  Ray ray(Vector3D(1.0 - CYLINDER_EPSILON, 5.0, 0.0), Vector3D(0, -1, 0));
  auto intersection = cyl.intersect(ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->point.getY(), 1.0, CYLINDER_EPSILON);
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(intersection->normal, Vector3D(0, 1, 0),
                                       CYLINDER_EPSILON);
}

TEST(CylinderTest, GetNormalAt) {
  Cylinder cyl(1.0, 2.0, Color::WHITE);
  // Point on top cap
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(cyl.getNormalAt(Vector3D(0, 1, 0)),
                                       Vector3D(0, 1, 0), CYLINDER_EPSILON);
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(cyl.getNormalAt(Vector3D(0.5, 1, 0.5)),
                                       Vector3D(0, 1, 0), CYLINDER_EPSILON);
  // Point on bottom cap
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(cyl.getNormalAt(Vector3D(0, -1, 0)),
                                       Vector3D(0, -1, 0), CYLINDER_EPSILON);
  // Point on body
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(cyl.getNormalAt(Vector3D(1, 0, 0)),
                                       Vector3D(1, 0, 0), CYLINDER_EPSILON);
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(cyl.getNormalAt(Vector3D(0, 0.5, -1)),
                                       Vector3D(0, 0, -1), CYLINDER_EPSILON);
  double invSqrt2 = 1.0 / std::sqrt(2.0);
  EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(
      cyl.getNormalAt(Vector3D(invSqrt2, 0.2, invSqrt2)),
      Vector3D(invSqrt2, 0, invSqrt2), CYLINDER_EPSILON);
}

TEST(CylinderTest, Clone) {
  Cylinder original(1.5, 3.0, Color::MAGENTA);
  Transform t;
  t.translate(1, 2, 3);
  t.rotateX(45);
  original.setTransform(t);

  std::shared_ptr<IPrimitive> cloneBase = original.clone();
  ASSERT_NE(cloneBase, nullptr);

  std::shared_ptr<Cylinder> clone =
      std::dynamic_pointer_cast<Cylinder>(cloneBase);
  ASSERT_NE(clone, nullptr);

  EXPECT_EQ(clone->getRadius(), 1.5);
  EXPECT_EQ(clone->getHeight(), 3.0);
  EXPECT_EQ(clone->getColor(), Color::MAGENTA);
  // EXPECT_EQ(clone->getTransform(), t); // Assuming Transform has operator==
  // If not, compare matrices or individual components

  // Test intersection with a ray to ensure clone behaves the same
  Ray ray(Vector3D(1, 0, -5), Vector3D(0, 0, 1));
  auto originalIntersection = original.intersect(ray);
  auto cloneIntersection = clone->intersect(ray);

  ASSERT_EQ(originalIntersection.has_value(), cloneIntersection.has_value());
  if (originalIntersection.has_value()) {
    EXPECT_NEAR(originalIntersection->distance, cloneIntersection->distance,
                CYLINDER_EPSILON);
    EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(originalIntersection->point,
                                         cloneIntersection->point,
                                         CYLINDER_EPSILON);
    EXPECT_VECTORS_NEARLY_EQUAL_CYLINDER(originalIntersection->normal,
                                         cloneIntersection->normal,
                                         CYLINDER_EPSILON);
  }
}

// TODO: Add tests for transformed cylinders
// TODO: Add tests for rays originating inside the cylinder
// TODO: Add tests for rays grazing the cylinder edges more thoroughly
