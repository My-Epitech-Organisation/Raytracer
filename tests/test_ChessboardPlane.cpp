/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for ChessboardPlane class
*/

#include <gtest/gtest.h>
#include <cmath>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/ChessboardPlane.hpp"

using namespace RayTracer;

// Helper function to compare Vector3D with tolerance
bool vectorsNearlyEqual_ChessboardTest(const Vector3D& v1, const Vector3D& v2,
                                      double epsilon = 1e-5) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  return dx < epsilon && dy < epsilon && dz < epsilon;
}

// Test constructor
TEST(ChessboardPlaneTest, Constructor) {
  // Test basic construction
  ChessboardPlane plane('Z', 0.0, Color::RED, Color::BLACK, 20.0);

  // Test position and normal
  EXPECT_DOUBLE_EQ(plane.getPosition(), 0.0);
  EXPECT_TRUE(vectorsNearlyEqual_ChessboardTest(plane.getNormal(), Vector3D(0, 0, 1)));

  // Test colors
  EXPECT_EQ(plane.getColor(), Color::RED);
  EXPECT_EQ(plane.getAlternateColor(), Color::BLACK);

  // Test check size
  EXPECT_DOUBLE_EQ(plane.getCheckSize(), 20.0);
}

// Test intersection with chessboard pattern
TEST(ChessboardPlaneTest, ChessboardPattern) {
  ChessboardPlane plane('Z', 0.0, Color::WHITE, Color::BLACK, 10.0);

  // Point at (5, 5, 0) should be on a white square (primary color)
  Ray ray1(Vector3D(5, 5, -5), Vector3D(0, 0, 1));
  auto intersection1 = plane.intersect(ray1);
  ASSERT_TRUE(intersection1.has_value());
  EXPECT_EQ(intersection1->color, Color::WHITE);

  // Point at (15, 5, 0) should be on a black square (alternate color)
  Ray ray2(Vector3D(15, 5, -5), Vector3D(0, 0, 1));
  auto intersection2 = plane.intersect(ray2);
  ASSERT_TRUE(intersection2.has_value());
  EXPECT_EQ(intersection2->color, Color::BLACK);

  // Point at (25, 5, 0) should be on a white square again
  Ray ray3(Vector3D(25, 5, -5), Vector3D(0, 0, 1));
  auto intersection3 = plane.intersect(ray3);
  ASSERT_TRUE(intersection3.has_value());
  EXPECT_EQ(intersection3->color, Color::WHITE);
}

// Test setAlternateColor and getAlternateColor
TEST(ChessboardPlaneTest, SetGetAlternateColor) {
  ChessboardPlane plane('Y', 0, Color::RED, Color::BLUE);
  EXPECT_EQ(plane.getAlternateColor(), Color::BLUE);

  plane.setAlternateColor(Color::GREEN);
  EXPECT_EQ(plane.getAlternateColor(), Color::GREEN);
}

// Test setCheckSize and getCheckSize
TEST(ChessboardPlaneTest, SetGetCheckSize) {
  ChessboardPlane plane('X', 0, Color::RED, Color::BLUE, 5.0);
  EXPECT_DOUBLE_EQ(plane.getCheckSize(), 5.0);

  plane.setCheckSize(15.0);
  EXPECT_DOUBLE_EQ(plane.getCheckSize(), 15.0);

  // Test invalid size
  EXPECT_THROW(plane.setCheckSize(0.0), std::invalid_argument);
  EXPECT_THROW(plane.setCheckSize(-5.0), std::invalid_argument);
}

// Test clone method
TEST(ChessboardPlaneTest, Clone) {
  double position = -5.5;
  Color color = Color::CYAN;
  Color altColor = Color::MAGENTA;
  double checkSize = 7.5;
  Transform transform;
  transform.translate(1, 2, 3);
  transform.rotateZ(30);

  ChessboardPlane original('X', position, color, altColor, checkSize);
  original.setTransform(transform);

  std::shared_ptr<IPrimitive> cloneBase = original.clone();
  ASSERT_NE(cloneBase, nullptr);

  // Check if it's actually a ChessboardPlane
  std::shared_ptr<ChessboardPlane> clone =
      std::dynamic_pointer_cast<ChessboardPlane>(cloneBase);
  ASSERT_NE(clone, nullptr);

  // Check if properties are copied
  EXPECT_EQ(clone->getColor(), color);
  EXPECT_EQ(clone->getAlternateColor(), altColor);
  EXPECT_DOUBLE_EQ(clone->getCheckSize(), checkSize);

  // Compare matrices for transform equality
  EXPECT_EQ(clone->getTransform().getMatrix(), transform.getMatrix());
}
