/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Transform class
*/

#include <gtest/gtest.h>
#include <stdexcept>
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"

using namespace RayTracer;

// Test the default constructor creates an identity transform
TEST(TransformTest, DefaultConstructor) {
  Transform transform;

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D transformedPoint = transform.applyToPoint(point);

  // An identity transform shouldn't change the point
  EXPECT_DOUBLE_EQ(transformedPoint.getX(), 1.0);
  EXPECT_DOUBLE_EQ(transformedPoint.getY(), 2.0);
  EXPECT_DOUBLE_EQ(transformedPoint.getZ(), 3.0);
}

// Test the copy constructor
TEST(TransformTest, CopyConstructor) {
  Transform original;
  original.translate(5.0, 10.0, 15.0);

  Transform copy(original);

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D transformedByOriginal = original.applyToPoint(point);
  Vector3D transformedByCopy = copy.applyToPoint(point);

  // Both transforms should produce the same result
  EXPECT_DOUBLE_EQ(transformedByCopy.getX(), transformedByOriginal.getX());
  EXPECT_DOUBLE_EQ(transformedByCopy.getY(), transformedByOriginal.getY());
  EXPECT_DOUBLE_EQ(transformedByCopy.getZ(), transformedByOriginal.getZ());
}

// Test assignment operator
TEST(TransformTest, AssignmentOperator) {
  Transform original;
  original.translate(5.0, 10.0, 15.0);

  Transform assigned;
  assigned = original;

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D transformedByOriginal = original.applyToPoint(point);
  Vector3D transformedByAssigned = assigned.applyToPoint(point);

  // Both transforms should produce the same result
  EXPECT_DOUBLE_EQ(transformedByAssigned.getX(), transformedByOriginal.getX());
  EXPECT_DOUBLE_EQ(transformedByAssigned.getY(), transformedByOriginal.getY());
  EXPECT_DOUBLE_EQ(transformedByAssigned.getZ(), transformedByOriginal.getZ());
}

// Test translation transformation
TEST(TransformTest, Translation) {
  Transform transform;
  transform.translate(2.0, 3.0, 4.0);

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D transformedPoint = transform.applyToPoint(point);

  // Point should be translated by (2,3,4)
  EXPECT_DOUBLE_EQ(transformedPoint.getX(), 3.0);  // 1.0 + 2.0
  EXPECT_DOUBLE_EQ(transformedPoint.getY(), 5.0);  // 2.0 + 3.0
  EXPECT_DOUBLE_EQ(transformedPoint.getZ(), 7.0);  // 3.0 + 4.0

  // Test that vectors are not affected by translation
  Vector3D vector(1.0, 0.0, 0.0);  // unit x vector
  Vector3D transformedVector = transform.applyToVector(vector);

  // Vector should remain unchanged by translation
  EXPECT_DOUBLE_EQ(transformedVector.getX(), 1.0);
  EXPECT_DOUBLE_EQ(transformedVector.getY(), 0.0);
  EXPECT_DOUBLE_EQ(transformedVector.getZ(), 0.0);
}

// Test X-axis rotation
TEST(TransformTest, RotationX) {
  Transform transform;
  transform.rotateX(90.0);  // 90 degrees around X

  Vector3D point(0.0, 1.0, 0.0);  // point on y-axis
  Vector3D transformedPoint = transform.applyToPoint(point);

  // A 90-degree rotation around X takes (0,1,0) to approximately (0,0,1)
  EXPECT_NEAR(transformedPoint.getX(), 0.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getY(), 0.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getZ(), 1.0, 1e-10);

  // Vectors should be transformed the same way as points
  Vector3D vector(0.0, 1.0, 0.0);  // unit y vector
  Vector3D transformedVector = transform.applyToVector(vector);

  EXPECT_NEAR(transformedVector.getX(), 0.0, 1e-10);
  EXPECT_NEAR(transformedVector.getY(), 0.0, 1e-10);
  EXPECT_NEAR(transformedVector.getZ(), 1.0, 1e-10);
}

// Test Y-axis rotation
TEST(TransformTest, RotationY) {
  Transform transform;
  transform.rotateY(90.0);  // 90 degrees around Y

  Vector3D point(0.0, 0.0, 1.0);  // point on z-axis
  Vector3D transformedPoint = transform.applyToPoint(point);

  // A 90-degree rotation around Y takes (0,0,1) to approximately (1,0,0)
  EXPECT_NEAR(transformedPoint.getX(), 1.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getY(), 0.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getZ(), 0.0, 1e-10);
}

// Test Z-axis rotation
TEST(TransformTest, RotationZ) {
  Transform transform;
  transform.rotateZ(90.0);  // 90 degrees around Z

  Vector3D point(1.0, 0.0, 0.0);  // point on x-axis
  Vector3D transformedPoint = transform.applyToPoint(point);

  // A 90-degree rotation around Z takes (1,0,0) to approximately (0,1,0)
  EXPECT_NEAR(transformedPoint.getX(), 0.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getY(), 1.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getZ(), 0.0, 1e-10);
}

// Test scaling (non-uniform)
TEST(TransformTest, ScaleNonUniform) {
  Transform transform;
  transform.scale(2.0, 3.0, 4.0);

  Vector3D point(1.0, 1.0, 1.0);
  Vector3D transformedPoint = transform.applyToPoint(point);

  // Point should be scaled by (2,3,4)
  EXPECT_DOUBLE_EQ(transformedPoint.getX(), 2.0);
  EXPECT_DOUBLE_EQ(transformedPoint.getY(), 3.0);
  EXPECT_DOUBLE_EQ(transformedPoint.getZ(), 4.0);

  // Vectors should be scaled the same way
  Vector3D vector(1.0, 1.0, 1.0);
  Vector3D transformedVector = transform.applyToVector(vector);

  EXPECT_DOUBLE_EQ(transformedVector.getX(), 2.0);
  EXPECT_DOUBLE_EQ(transformedVector.getY(), 3.0);
  EXPECT_DOUBLE_EQ(transformedVector.getZ(), 4.0);
}

// Test uniform scaling
TEST(TransformTest, ScaleUniform) {
  Transform transform;
  transform.scale(2.0);  // Uniform scale

  Vector3D point(1.0, 1.0, 1.0);
  Vector3D transformedPoint = transform.applyToPoint(point);

  // Point should be scaled uniformly by 2
  EXPECT_DOUBLE_EQ(transformedPoint.getX(), 2.0);
  EXPECT_DOUBLE_EQ(transformedPoint.getY(), 2.0);
  EXPECT_DOUBLE_EQ(transformedPoint.getZ(), 2.0);
}

// Test combining transformations
TEST(TransformTest, CombineTransformations) {
  // Test sequence: rotate 90 degrees around Z, then translate by (1,0,0)
  Transform transform;
  transform.rotateZ(90.0);
  transform.translate(1.0, 0.0, 0.0);

  Vector3D point(1.0, 0.0, 0.0);  // point on x-axis
  Vector3D transformedPoint = transform.applyToPoint(point);

  // After rotation: (0,1,0)
  // After translation: (1,1,0)
  EXPECT_NEAR(transformedPoint.getX(), 1.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getY(), 1.0, 1e-10);
  EXPECT_NEAR(transformedPoint.getZ(), 0.0, 1e-10);
}

// Test order of transformations
TEST(TransformTest, TransformationOrder) {
  // Test different sequences of transformations
  // Sequence 1: rotate then translate
  Transform rotateFirst;
  rotateFirst.rotateZ(90.0);
  rotateFirst.translate(1.0, 0.0, 0.0);

  // Sequence 2: translate then rotate
  Transform translateFirst;
  translateFirst.translate(1.0, 0.0, 0.0);
  translateFirst.rotateZ(90.0);

  Vector3D point(0.0, 0.0, 0.0);  // origin
  Vector3D result1 = rotateFirst.applyToPoint(point);
  Vector3D result2 = translateFirst.applyToPoint(point);

  // Results should be different due to order of operations
  // rotateFirst: origin -> (0,0,0) -> rotate -> (0,0,0) -> translate -> (1,0,0)
  // translateFirst: origin -> (0,0,0) -> translate -> (1,0,0) -> rotate -> (0,1,0)
  EXPECT_NEAR(result1.getX(), 1.0, 1e-10);
  EXPECT_NEAR(result1.getY(), 0.0, 1e-10);

  EXPECT_NEAR(result2.getX(), 0.0, 1e-10);
  EXPECT_NEAR(result2.getY(), 1.0, 1e-10);
}

// Test the combine method
// TEST(TransformTest, CombineMethod) {
//   Transform transformA;
//   transformA.translate(1.0, 2.0, 3.0);

//   Transform transformB;
//   transformB.rotateZ(90.0);

//   // Combine A with B (B is applied first, then A)
//   Transform combined = transformA;
//   combined.combine(transformB);

//   Vector3D point(1.0, 0.0, 0.0);

//   // Apply each transform sequentially
//   Vector3D expected = transformA.applyToPoint(transformB.applyToPoint(point));

//   // Apply combined transform
//   Vector3D result = combined.applyToPoint(point);

//   // Results should match
//   EXPECT_NEAR(result.getX(), expected.getX(), 1e-10);
//   EXPECT_NEAR(result.getY(), expected.getY(), 1e-10);
//   EXPECT_NEAR(result.getZ(), expected.getZ(), 1e-10);
// }

// Test the inverse transformation
TEST(TransformTest, Inverse) {
  Transform transform;
  transform.translate(3.0, 4.0, 5.0).rotateZ(30.0).scale(2.0);

  Vector3D point(1.0, 2.0, 3.0);
  Vector3D transformed = transform.applyToPoint(point);

  // Apply inverse transform to get back the original point
  Transform inverse = transform.inverse();
  Vector3D restored = inverse.applyToPoint(transformed);

  // Check that we get back the original point (allowing for floating-point errors)
  EXPECT_NEAR(restored.getX(), point.getX(), 1e-10);
  EXPECT_NEAR(restored.getY(), point.getY(), 1e-10);
  EXPECT_NEAR(restored.getZ(), point.getZ(), 1e-10);
}

// Test handling of degenerate transform (non-invertible)
TEST(TransformTest, DegenerateTransform) {
  Transform transform;

  // Zero scaling makes matrix non-invertible
  EXPECT_THROW(transform.scale(0.0, 1.0, 1.0), std::runtime_error);
}
