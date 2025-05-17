/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Vector3D class
*/

/**
 * @file test_Vector3D.cpp
 * @brief Unit tests for the Vector3D class to verify its functionality and
 * correctness
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <gtest/gtest.h>
#include "../src/core/Vector3D.hpp"

using namespace RayTracer;

// Test the default constructor
TEST(Vector3DTest, DefaultConstructor) {
  Vector3D vec;

  EXPECT_DOUBLE_EQ(vec.getX(), 0.0);
  EXPECT_DOUBLE_EQ(vec.getY(), 0.0);
  EXPECT_DOUBLE_EQ(vec.getZ(), 0.0);
}

// Test the parameterized constructor
TEST(Vector3DTest, ParameterizedConstructor) {
  Vector3D vec(1.5, -2.3, 3.7);

  EXPECT_DOUBLE_EQ(vec.getX(), 1.5);
  EXPECT_DOUBLE_EQ(vec.getY(), -2.3);
  EXPECT_DOUBLE_EQ(vec.getZ(), 3.7);
}

// Test the copy constructor
TEST(Vector3DTest, CopyConstructor) {
  Vector3D original(1.5, -2.3, 3.7);
  Vector3D copy(original);

  EXPECT_DOUBLE_EQ(copy.getX(), 1.5);
  EXPECT_DOUBLE_EQ(copy.getY(), -2.3);
  EXPECT_DOUBLE_EQ(copy.getZ(), 3.7);
}

// Test assignment operator
TEST(Vector3DTest, AssignmentOperator) {
  Vector3D original(1.5, -2.3, 3.7);
  Vector3D assigned;

  assigned = original;

  EXPECT_DOUBLE_EQ(assigned.getX(), 1.5);
  EXPECT_DOUBLE_EQ(assigned.getY(), -2.3);
  EXPECT_DOUBLE_EQ(assigned.getZ(), 3.7);
}

// Test getter and setter methods
TEST(Vector3DTest, GetterSetterMethods) {
  Vector3D vec;

  vec.setX(4.2);
  vec.setY(-5.6);
  vec.setZ(7.1);

  EXPECT_DOUBLE_EQ(vec.getX(), 4.2);
  EXPECT_DOUBLE_EQ(vec.getY(), -5.6);
  EXPECT_DOUBLE_EQ(vec.getZ(), 7.1);
}

// Test addition operator
TEST(Vector3DTest, AdditionOperator) {
  Vector3D vec1(1.0, 2.0, 3.0);
  Vector3D vec2(4.0, 5.0, 6.0);

  Vector3D result = vec1 + vec2;

  EXPECT_DOUBLE_EQ(result.getX(), 5.0);
  EXPECT_DOUBLE_EQ(result.getY(), 7.0);
  EXPECT_DOUBLE_EQ(result.getZ(), 9.0);
}

// Test subtraction operator
TEST(Vector3DTest, SubtractionOperator) {
  Vector3D vec1(4.0, 5.0, 6.0);
  Vector3D vec2(1.0, 2.0, 3.0);

  Vector3D result = vec1 - vec2;

  EXPECT_DOUBLE_EQ(result.getX(), 3.0);
  EXPECT_DOUBLE_EQ(result.getY(), 3.0);
  EXPECT_DOUBLE_EQ(result.getZ(), 3.0);
}

// Test scalar multiplication operator
TEST(Vector3DTest, ScalarMultiplicationOperator) {
  Vector3D vec(1.0, 2.0, 3.0);

  Vector3D result = vec * 2.5;

  EXPECT_DOUBLE_EQ(result.getX(), 2.5);
  EXPECT_DOUBLE_EQ(result.getY(), 5.0);
  EXPECT_DOUBLE_EQ(result.getZ(), 7.5);
}

// Test scalar division operator
TEST(Vector3DTest, ScalarDivisionOperator) {
  Vector3D vec(2.5, 5.0, 7.5);

  Vector3D result = vec / 2.5;

  EXPECT_DOUBLE_EQ(result.getX(), 1.0);
  EXPECT_DOUBLE_EQ(result.getY(), 2.0);
  EXPECT_DOUBLE_EQ(result.getZ(), 3.0);
}

// Test division by zero exception
TEST(Vector3DTest, DivisionByZeroException) {
  Vector3D vec(1.0, 2.0, 3.0);

  EXPECT_THROW(vec / 0.0, std::runtime_error);
}

// Test negation operator
TEST(Vector3DTest, NegationOperator) {
  Vector3D vec(1.5, -2.3, 3.7);

  Vector3D result = -vec;

  EXPECT_DOUBLE_EQ(result.getX(), -1.5);
  EXPECT_DOUBLE_EQ(result.getY(), 2.3);
  EXPECT_DOUBLE_EQ(result.getZ(), -3.7);
}

// Test dot product
TEST(Vector3DTest, DotProduct) {
  Vector3D vec1(1.0, 2.0, 3.0);
  Vector3D vec2(4.0, 5.0, 6.0);

  double result = vec1.dot(vec2);

  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
  EXPECT_DOUBLE_EQ(result, 32.0);
}

// Test cross product
TEST(Vector3DTest, CrossProduct) {
  Vector3D vec1(1.0, 0.0, 0.0);  // unit x-axis
  Vector3D vec2(0.0, 1.0, 0.0);  // unit y-axis

  Vector3D result = vec1.cross(vec2);

  // Should give unit z-axis
  EXPECT_DOUBLE_EQ(result.getX(), 0.0);
  EXPECT_DOUBLE_EQ(result.getY(), 0.0);
  EXPECT_DOUBLE_EQ(result.getZ(), 1.0);
}

// Test cross product (reverse order)
TEST(Vector3DTest, CrossProductReverse) {
  Vector3D vec1(0.0, 1.0, 0.0);  // unit y-axis
  Vector3D vec2(1.0, 0.0, 0.0);  // unit x-axis

  Vector3D result = vec1.cross(vec2);

  // Should give negative unit z-axis
  EXPECT_DOUBLE_EQ(result.getX(), 0.0);
  EXPECT_DOUBLE_EQ(result.getY(), 0.0);
  EXPECT_DOUBLE_EQ(result.getZ(), -1.0);
}

// Test squared magnitude
TEST(Vector3DTest, SquaredMagnitude) {
  Vector3D vec(3.0, 4.0, 0.0);

  double result = vec.getSquaredMagnitude();

  // 3² + 4² + 0² = 9 + 16 = 25
  EXPECT_DOUBLE_EQ(result, 25.0);
}

// Test magnitude
TEST(Vector3DTest, Magnitude) {
  Vector3D vec(3.0, 4.0, 0.0);

  double result = vec.getMagnitude();

  // sqrt(3² + 4² + 0²) = sqrt(25) = 5
  EXPECT_DOUBLE_EQ(result, 5.0);
}

// Test vector normalization
TEST(Vector3DTest, Normalization) {
  Vector3D vec(3.0, 4.0, 0.0);  // length = 5

  Vector3D normalized = vec.normalized();

  // Should be (3/5, 4/5, 0)
  EXPECT_DOUBLE_EQ(normalized.getX(), 0.6);
  EXPECT_DOUBLE_EQ(normalized.getY(), 0.8);
  EXPECT_DOUBLE_EQ(normalized.getZ(), 0.0);

  // Magnitude should be 1
  EXPECT_NEAR(normalized.getMagnitude(), 1.0, 1e-10);
}

// Test normalization of zero vector
TEST(Vector3DTest, NormalizeZeroVector) {
  Vector3D vec(0.0, 0.0, 0.0);

  EXPECT_THROW(vec.normalized(), std::runtime_error);
}

// Test isEqual method
TEST(Vector3DTest, IsEqual) {
  Vector3D vec1(1.0, 2.0, 3.0);
  Vector3D vec2(1.0, 2.0, 3.0);
  Vector3D vec3(1.001, 2.0, 3.0);

  EXPECT_TRUE(vec1.isEqual(vec2));
  EXPECT_FALSE(vec1.isEqual(vec3));
  EXPECT_TRUE(vec1.isEqual(vec3, 0.01));  // with larger epsilon
}

// Test complex calculations - check orthogonality of cross product
TEST(Vector3DTest, CrossProductOrthogonality) {
  Vector3D vec1(2.0, 3.0, 4.0);
  Vector3D vec2(5.0, 6.0, 7.0);

  Vector3D crossProduct = vec1.cross(vec2);

  // Cross product should be orthogonal to both inputs
  EXPECT_NEAR(crossProduct.dot(vec1), 0.0, 1e-10);
  EXPECT_NEAR(crossProduct.dot(vec2), 0.0, 1e-10);
}

// Test for preservation of direction in normalization
TEST(Vector3DTest, NormalizationPreservesDirection) {
  Vector3D original(2.0, -3.0, 4.0);
  Vector3D normalized = original.normalized();

  // The normalized vector should point in the same direction as the original
  // This can be checked by ensuring they are scalar multiples of each other
  double magnitude = original.getMagnitude();
  Vector3D scaledNormalized = normalized * magnitude;

  EXPECT_TRUE(original.isEqual(scaledNormalized, 1e-10));
}

// Test equality operator
TEST(Vector3DTest, EqualityOperator) {
  Vector3D vec1(1.0, 2.0, 3.0);
  Vector3D vec2(1.0, 2.0, 3.0);
  Vector3D vec3(1.001, 2.0, 3.0);

  EXPECT_TRUE(vec1 == vec2);
  EXPECT_FALSE(vec1 == vec3);

  // Test that the behavior matches isEqual with default epsilon
  EXPECT_EQ(vec1 == vec2, vec1.isEqual(vec2));
  EXPECT_EQ(vec1 == vec3, vec1.isEqual(vec3));
}

// Test inequality operator
TEST(Vector3DTest, InequalityOperator) {
  Vector3D vec1(1.0, 2.0, 3.0);
  Vector3D vec2(1.0, 2.0, 3.0);
  Vector3D vec3(1.001, 2.0, 3.0);

  EXPECT_FALSE(vec1 != vec2);
  EXPECT_TRUE(vec1 != vec3);

  // Test that the behavior is the opposite of equality
  EXPECT_EQ(vec1 != vec2, !(vec1 == vec2));
  EXPECT_EQ(vec1 != vec3, !(vec1 == vec3));
}
