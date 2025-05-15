#include <cmath>
#include <stdexcept>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/Cone.hpp"
#include "gtest/gtest.h"

// Define a small epsilon for floating point comparisons
const double EPSILON_TEST = 1e-5;

// Test fixture for Cone tests
class ConeTest : public ::testing::Test {
 protected:
  RayTracer::Color defaultColor{RayTracer::Color::WHITE};
  RayTracer::Vector3D defaultApex{0, 0, 0};
  RayTracer::Vector3D yAxis{0, 1, 0};  // Cone along Y-axis
  RayTracer::Vector3D zAxis{0, 0, 1};  // Cone along Z-axis
  double defaultAngleDegrees = 30.0;
};

// Test Cone Construction
TEST_F(ConeTest, ConstructorValid) {
  ASSERT_NO_THROW(RayTracer::Cone cone(defaultApex, yAxis, defaultAngleDegrees,
                                       defaultColor));
}

TEST_F(ConeTest, ConstructorInvalidAngleTooSmall) {
  ASSERT_THROW(RayTracer::Cone cone(defaultApex, yAxis, 0.0, defaultColor),
               std::invalid_argument);
}

TEST_F(ConeTest, ConstructorInvalidAngleTooLarge) {
  ASSERT_THROW(RayTracer::Cone cone(defaultApex, yAxis, 90.0, defaultColor),
               std::invalid_argument);
}

TEST_F(ConeTest, ConstructorInvalidAngleNegative) {
  ASSERT_THROW(RayTracer::Cone cone(defaultApex, yAxis, -10.0, defaultColor),
               std::invalid_argument);
}

// Test Cone Properties
TEST_F(ConeTest, GetProperties) {
  RayTracer::Vector3D apex(1, 2, 3);
  RayTracer::Vector3D axis(0, 0, 1);
  double angle = 45.0;
  RayTracer::Color color(static_cast<unsigned char>(255),
                         static_cast<unsigned char>(0),
                         static_cast<unsigned char>(0));  // Red

  RayTracer::Cone cone(apex, axis, angle, color);

  EXPECT_EQ(cone.getApex().getX(), apex.getX());
  EXPECT_EQ(cone.getApex().getY(), apex.getY());
  EXPECT_EQ(cone.getApex().getZ(), apex.getZ());

  // Axis is normalized in constructor
  RayTracer::Vector3D normalizedAxis = axis.normalized();
  EXPECT_NEAR(cone.getAxis().getX(), normalizedAxis.getX(), EPSILON_TEST);
  EXPECT_NEAR(cone.getAxis().getY(), normalizedAxis.getY(), EPSILON_TEST);
  EXPECT_NEAR(cone.getAxis().getZ(), normalizedAxis.getZ(), EPSILON_TEST);

  EXPECT_NEAR(cone.getAngleDegrees(), angle, EPSILON_TEST);
  EXPECT_EQ(cone.getColor(),
            color);  // Changed to compare Color objects directly
}

// Test Simple Intersections

// Ray along the cone axis, starting outside and pointing towards the apex
TEST_F(ConeTest, IntersectionRayAlongAxisHitsApex) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);  // Cone opens along positive Y
  double angle = 30.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Ray ray(
      RayTracer::Vector3D(0, -10, 0),
      RayTracer::Vector3D(0, 1, 0));  // Ray along Y, towards apex
  auto intersection = cone.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  // The intersection point should be the apex
  EXPECT_NEAR(intersection->point.getX(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->point.getY(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->point.getZ(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->distance, 10.0, EPSILON_TEST);
  EXPECT_EQ(intersection->color, defaultColor);
}

// Ray parallel to cone axis, offset, and intersects the cone (e.g., at y = 10 *
// √3)
TEST_F(ConeTest, IntersectionRayParallelToAxisMisses) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);  // Cone opens along positive Y
  double angle = 30.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Ray ray(
      RayTracer::Vector3D(10, -10, 0),
      RayTracer::Vector3D(0, 1, 0));  // Parallel to Y-axis, offset
  auto intersection = cone.intersect(ray);

  ASSERT_TRUE(intersection.has_value());  // Changed from ASSERT_FALSE
  if (intersection.has_value()) {
    EXPECT_NEAR(intersection->point.getX(), 10.0, EPSILON_TEST);
    EXPECT_NEAR(intersection->point.getY(), 10.0 * std::sqrt(3.0),
                EPSILON_TEST);
    EXPECT_NEAR(intersection->point.getZ(), 0.0, EPSILON_TEST);
    EXPECT_NEAR(intersection->distance, 10.0 * std::sqrt(3.0) + 10.0,
                EPSILON_TEST);
  }
}

// Ray pointing away from the cone
TEST_F(ConeTest, IntersectionRayPointsAway) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);
  double angle = 30.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Ray ray(RayTracer::Vector3D(0, -5, 0),
                     RayTracer::Vector3D(0, -1, 0));  // Pointing away from apex
  auto intersection = cone.intersect(ray);
  ASSERT_FALSE(intersection.has_value());
}

// Test intersection with a ray known to hit the cone surface
// Cone with apex at (0,0,0), axis (0,1,0) (upwards), angle 45 degrees.
// This means at y=1, the radius of the cone slice is also 1.
// A ray from (0,1,2) pointing towards (0,1,0) should hit at (0,1,1).
TEST_F(ConeTest, IntersectionKnownHit) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);  // Cone opens along positive Y
  double angle = 45.0;                // tan(45deg) = 1
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Ray ray(RayTracer::Vector3D(0, 1, 2),
                     RayTracer::Vector3D(0, 0, -1));  // Ray along Z, at y=1
  auto intersection = cone.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->point.getX(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->point.getY(), 1.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->point.getZ(), 1.0,
              EPSILON_TEST);  // Hits at (0,1,1)
  EXPECT_NEAR(intersection->distance, 1.0,
              EPSILON_TEST);  // Distance from (0,1,2) to (0,1,1) is 1
}

// Test normal calculation at a known point on the cone surface
// For a cone with apex (0,0,0), axis (0,1,0), angle 45 deg.
// Point (0,1,1) is on the surface.
// The normal at this point should be perpendicular to the surface.
// The vector from apex to point is (0,1,1).
// The tangent to the cone surface in the y-z plane at (0,1,1) has slope 1.
// The normal should have slope -1 in the y-z plane, pointing outwards.
// Normal components: N_y should be positive, N_z should be negative (if
// pointing "out" from y-axis direction) Or, more simply, for a point P on the
// cone, the vector from (Apex + (P-Apex).dot(Axis)*Axis) to P, then adjusted.
// For point (0,1,1) on cone (apex 0,0,0, axis 0,1,0, angle 45 deg):
// Vector from apex to point P-V = (0,1,1)
// (P-V).dot(Axis) = (0,1,1).dot(0,1,0) = 1
// cos^2(angle) = cos^2(45) = (1/sqrt(2))^2 = 0.5
// Normal_local = ( (P-V) - Axis * ( (P-V).dot(Axis) / cos^2(angle) )
// ).normalized() Normal_local = ( (0,1,1) - (0,1,0) * (1 / 0.5) ).normalized()
// Normal_local = ( (0,1,1) - (0,2,0) ).normalized()
// Normal_local = (0, -1, 1).normalized()
// This normal points "inwards" towards the axis if we consider the +Y direction
// as "up". The getNormalAt method should return a normal pointing away from the
// surface. The intersection logic flips the normal if it points in the same
// direction as the ray. Here we test getNormalAt directly.
TEST_F(ConeTest, GetNormalAtKnownPoint) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);  // Cone opens along positive Y
  double angle = 45.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Vector3D pointOnSurface(
      0, 1, 1);  // x=0, y=1, z=1. (y/z = 1/1 = tan(45))
  RayTracer::Vector3D normal = cone.getNormalAt(pointOnSurface);

  // Expected normal: (0, -1/sqrt(2), 1/sqrt(2)) or (0, 1/sqrt(2), -1/sqrt(2))
  // The formula used in Cone.cpp: (C_to_P - _axis * cp_dot_axis * (1.0 /
  // _cos_angle_sq)).normalized(); C_to_P = (0,1,1) - (0,0,0) = (0,1,1) _axis =
  // (0,1,0) cp_dot_axis = (0,1,1).dot(0,1,0) = 1 _cos_angle_sq = cos(45)^2 =
  // 0.5 normal_local = ( (0,1,1) - (0,1,0) * (1 / 0.5) ).normalized()
  // normal_local = ( (0,1,1) - (0,2,0) ).normalized()
  // normal_local = (0, -1, 1).normalized() = (0, -0.7071, 0.7071)
  // This normal points "inward" relative to the y-axis if the cone opens
  // towards +y. A common convention is for normals to point "outward" from the
  // volume. Let's check the magnitude and its general direction. The normal
  // should be perpendicular to the cone axis projected onto the tangent plane.
  // And perpendicular to the slant vector.
  // Slant vector from apex to point is (0,1,1).
  // Tangent vector along circumference at (0,1,1) could be (1,0,0) (if axis is
  // Y). Normal should be perpendicular to (1,0,0). So Nx=0. Normal should also
  // be perpendicular to the tangent along the slant. Tangent along slant:
  // derivative of (0, y, y*tan(angle)) wrt y is (0, 1, tan(angle)) = (0,1,1)
  // for 45 deg. So normal.dot(0,1,1) should be 0. (0, -0.7071,
  // 0.7071).dot(0,1,1) = -0.7071 + 0.7071 = 0. Correct.

  EXPECT_NEAR(normal.getX(), 0.0, EPSILON_TEST);
  // The sign of Y and Z depends on the "outward" definition.
  // If cone opens towards +Y, an outward normal at (0,1,1) would have Y
  // component > 0 and Z component > 0 or Y < 0 and Z < 0. The calculated (0,
  // -0.7071, 0.7071) has Ny < 0 and Nz > 0. This means it points somewhat
  // "down" (against axis) and "out" (along Z). This is consistent if the
  // "outside" is where z > y for a fixed y. Let's verify the dot product with
  // (pointOnSurface - apex) and axis. Vector M = pointOnSurface - (apex + axis
  // * ( (pointOnSurface-apex).dot(axis) ) ) M = (0,1,1) - ( (0,0,0) + (0,1,0) *
  // ( (0,1,1).dot(0,1,0) ) ) M = (0,1,1) - ( (0,0,0) + (0,1,0) * 1 ) = (0,1,1)
  // - (0,1,0) = (0,0,1) The normal should be in the direction of M - axis *
  // M.magnitude() * tan(angle) Or (P-V) - dot(A, P-V)*A / cos^2(theta) The
  // normal should be (0, -sqrt(2)/2, sqrt(2)/2) or (0, sqrt(2)/2, -sqrt(2)/2)
  // The current implementation gives (0, -0.70710678118, 0.70710678118)
  EXPECT_NEAR(normal.getY(), -std::sqrt(0.5), EPSILON_TEST);
  EXPECT_NEAR(normal.getZ(), std::sqrt(0.5), EPSILON_TEST);
  EXPECT_NEAR(normal.getMagnitude(), 1.0,
              EPSILON_TEST);  // Should be normalized
}

// Test cloning a cone
TEST_F(ConeTest, CloneCone) {
  RayTracer::Vector3D apex(1, 2, 3);
  RayTracer::Vector3D axis(0, 1, 0);
  double angle = 25.0;
  RayTracer::Color color(static_cast<unsigned char>(10),
                         static_cast<unsigned char>(20),
                         static_cast<unsigned char>(30));
  RayTracer::Cone originalCone(apex, axis, angle, color);

  RayTracer::Transform t;
  t.translate(10, 0, 0);
  originalCone.setTransform(t);

  // Get world properties of the original cone AFTER transformation
  RayTracer::Vector3D originalWorldApex = originalCone.getApex();
  RayTracer::Vector3D originalWorldAxis = originalCone.getAxis();

  std::shared_ptr<RayTracer::IPrimitive> clonedPrimitive = originalCone.clone();
  ASSERT_NE(clonedPrimitive, nullptr);

  RayTracer::Cone* clonedCone =
      dynamic_cast<RayTracer::Cone*>(clonedPrimitive.get());
  ASSERT_NE(clonedCone, nullptr);

  // Compare world properties of the cloned cone with world properties of the
  // original cone
  EXPECT_NEAR(clonedCone->getApex().getX(), originalWorldApex.getX(),
              EPSILON_TEST);
  EXPECT_NEAR(clonedCone->getApex().getY(), originalWorldApex.getY(),
              EPSILON_TEST);
  EXPECT_NEAR(clonedCone->getApex().getZ(), originalWorldApex.getZ(),
              EPSILON_TEST);

  EXPECT_NEAR(clonedCone->getAxis().getX(), originalWorldAxis.getX(),
              EPSILON_TEST);
  EXPECT_NEAR(clonedCone->getAxis().getY(), originalWorldAxis.getY(),
              EPSILON_TEST);
  EXPECT_NEAR(clonedCone->getAxis().getZ(), originalWorldAxis.getZ(),
              EPSILON_TEST);

  EXPECT_NEAR(clonedCone->getAngleDegrees(), angle, EPSILON_TEST);
  EXPECT_EQ(clonedCone->getColor(), color);

  // Check if transform was also cloned
  RayTracer::Transform clonedTransform = clonedCone->getTransform();
  RayTracer::Vector3D testVec(1, 1, 1);
  RayTracer::Vector3D originalTransformed = t.applyToPoint(testVec);
  RayTracer::Vector3D clonedTransformed = clonedTransform.applyToPoint(testVec);
  EXPECT_NEAR(originalTransformed.getX(), clonedTransformed.getX(),
              EPSILON_TEST);
  EXPECT_NEAR(originalTransformed.getY(), clonedTransformed.getY(),
              EPSILON_TEST);
  EXPECT_NEAR(originalTransformed.getZ(), clonedTransformed.getZ(),
              EPSILON_TEST);

  // Ensure it's a deep copy (modifying original doesn't change clone)
  RayTracer::Color whiteColor(static_cast<unsigned char>(255),
                              static_cast<unsigned char>(255),
                              static_cast<unsigned char>(255));
  originalCone.setColor(whiteColor);
  EXPECT_NE(clonedCone->getColor(), whiteColor);
}

// Test intersection when ray origin is at the apex
TEST_F(ConeTest, IntersectionRayOriginAtApex) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);  // Cone opens along positive Y
  double angle = 30.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  // Ray starting at apex, pointing along the surface
  double angle_rad = angle * M_PI / 180.0;
  RayTracer::Vector3D direction_on_surface(std::sin(angle_rad),
                                           std::cos(angle_rad), 0);
  RayTracer::Ray ray_along_surface(apex, direction_on_surface.normalized());
  auto intersection_surface = cone.intersect(ray_along_surface);
  ASSERT_FALSE(intersection_surface.has_value());

  // Ray starting at apex, pointing slightly inside the cone (relative to axis +
  // small perturbation)
  RayTracer::Ray ray_slightly_inside(
      apex, (axis + RayTracer::Vector3D(0.01, 0, 0.01)).normalized());
  auto intersection_slightly_inside = cone.intersect(ray_slightly_inside);
  ASSERT_FALSE(intersection_slightly_inside.has_value());

  // Ray starting at apex, pointing slightly outside the cone (perpendicular to
  // axis)
  RayTracer::Ray ray_slightly_outside(
      apex, (RayTracer::Vector3D(1, 0, 0)).normalized());
  auto intersection_slightly_outside = cone.intersect(ray_slightly_outside);
  ASSERT_FALSE(intersection_slightly_outside.has_value());

  // Ray from apex pointing into the cone body (not along axis, not along
  // surface)
  RayTracer::Ray ray_into_body(apex,
                               RayTracer::Vector3D(0.1, 1, 0).normalized());
  auto intersection_into_body = cone.intersect(ray_into_body);
  ASSERT_FALSE(intersection_into_body.has_value());
}

// Test intersection with transformed cone
TEST_F(ConeTest, IntersectionTransformedCone) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);  // Upwards
  double angle = 30.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Transform t;
  t.translate(0, 5, 0);  // Move cone up by 5 units. Apex is now (0,5,0)
  cone.setTransform(t);

  // Ray that would hit the original cone's apex
  RayTracer::Ray ray(RayTracer::Vector3D(0, -2, 0),
                     RayTracer::Vector3D(0, 1, 0));
  auto intersection = cone.intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection->point.getX(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->point.getY(), 5.0,
              EPSILON_TEST);  // Hits translated apex
  EXPECT_NEAR(intersection->point.getZ(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(intersection->distance, 7.0,
              EPSILON_TEST);  // Distance from (0,-2,0) to (0,5,0)
}

// Test normal with transformed cone
TEST_F(ConeTest, NormalTransformedCone) {
  RayTracer::Vector3D apex(0, 0, 0);
  RayTracer::Vector3D axis(0, 1, 0);
  double angle = 45.0;
  RayTracer::Cone cone(apex, axis, angle, defaultColor);

  RayTracer::Transform t;
  t.rotateZ(90);  // Rotate 90 degrees around Z. Y-axis becomes -X axis.
                  // New axis should be (-1, 0, 0)
  cone.setTransform(t);

  // Original point (0,1,1) on the cone.
  // After rotation around Z by 90 deg: (x,y,z) -> (-y,x,z)
  // So (0,1,1) -> (-1,0,1)
  RayTracer::Vector3D pointOnTransformedSurface(-1, 0, 1);
  RayTracer::Vector3D normal = cone.getNormalAt(pointOnTransformedSurface);

  // Original normal at (0,1,1) was (0, -sqrt(0.5), sqrt(0.5))
  // After rotation: N'x = -Ny, N'y = Nx, N'z = Nz
  // N' = (sqrt(0.5), 0, sqrt(0.5))
  EXPECT_NEAR(normal.getX(), std::sqrt(0.5), EPSILON_TEST);
  EXPECT_NEAR(normal.getY(), 0.0, EPSILON_TEST);
  EXPECT_NEAR(normal.getZ(), std::sqrt(0.5), EPSILON_TEST);
  EXPECT_NEAR(normal.getMagnitude(), 1.0, EPSILON_TEST);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
