/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Camera class
*/

/**
 * @file test_Camera.cpp
 * @brief Unit tests for the Camera class to validate camera positioning, ray
 * generation and transformations
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <cmath>
#include "../include/exceptions/InvalidTypeException.hpp"
#include "../include/exceptions/ParserException.hpp"
#include "../include/exceptions/RaytracerException.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/Camera.hpp"

using namespace RayTracer;

// Helper function to compare Vector3D with tolerance
bool vectorsNearlyEqual_Camera(const Vector3D& v1, const Vector3D& v2,
                               double epsilon = 1e-3) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  return dx < epsilon && dy < epsilon && dz < epsilon;
}

// Test the default constructor
TEST(CameraTest, DefaultConstructor) {
  Camera camera;

  EXPECT_EQ(camera.getWidth(), 800);
  EXPECT_EQ(camera.getHeight(), 600);
  EXPECT_EQ(camera.getFieldOfView(), 60.0);
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(camera.getPosition(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(camera.getRotation(), Vector3D(0, 0, 0)));
}

// Test the parameterized constructor
TEST(CameraTest, ParameterizedConstructor) {
  Vector3D position(10, 20, 30);
  Camera camera(position, 1920, 1080, 72.0);

  EXPECT_EQ(camera.getWidth(), 1920);
  EXPECT_EQ(camera.getHeight(), 1080);
  EXPECT_EQ(camera.getFieldOfView(), 72.0);
  EXPECT_TRUE(vectorsNearlyEqual_Camera(camera.getPosition(), position));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(camera.getRotation(), Vector3D(0, 0, 0)));
}

// Test invalid parameters
TEST(CameraTest, InvalidParameters) {
  Vector3D position(0, 0, 0);

  EXPECT_THROW(Camera(position, -1, 1080, 72.0), InvalidTypeException);
  EXPECT_THROW(Camera(position, 1920, -1, 72.0), InvalidTypeException);
  EXPECT_THROW(Camera(position, 1920, 1080, -1.0), InvalidTypeException);
  EXPECT_THROW(Camera(position, 1920, 1080, 180.0), InvalidTypeException);
}

// Test setters
TEST(CameraTest, Setters) {
  Camera camera;
  Vector3D position(10, 20, 30);
  Vector3D rotation(45, 90, 180);

  camera.setPosition(position);
  camera.setRotation(rotation);
  camera.setResolution(1920, 1080);
  camera.setFieldOfView(72.0);

  EXPECT_EQ(camera.getWidth(), 1920);
  EXPECT_EQ(camera.getHeight(), 1080);
  EXPECT_EQ(camera.getFieldOfView(), 72.0);
  EXPECT_TRUE(vectorsNearlyEqual_Camera(camera.getPosition(), position));
  EXPECT_TRUE(vectorsNearlyEqual_Camera(camera.getRotation(), rotation));

  EXPECT_THROW(camera.setResolution(-1, 1080), InvalidTypeException);
  EXPECT_THROW(camera.setResolution(1920, -1), InvalidTypeException);
  EXPECT_THROW(camera.setFieldOfView(-1.0), InvalidTypeException);
  EXPECT_THROW(camera.setFieldOfView(180.0), InvalidTypeException);
}

// Test ray generation with default camera (center pixel)
TEST(CameraTest, CenterRayGeneration) {
  Camera camera;

  // For a default camera at (0,0,0) looking along -Z:
  // The center pixel should generate a ray pointing straight down the -Z axis
  Ray centerRay =
      camera.generateRay(camera.getWidth() / 2, camera.getHeight() / 2);

  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getOrigin(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getDirection(), Vector3D(0, 0, -1)));
}

// Test ray generation with default camera (corner pixels)
TEST(CameraTest, CornerRaysGeneration) {
  Camera camera;

  // Test the four corner rays
  Ray topLeftRay = camera.generateRay(0, 0);
  Ray topRightRay = camera.generateRay(camera.getWidth() - 1, 0);
  Ray bottomLeftRay = camera.generateRay(0, camera.getHeight() - 1);
  Ray bottomRightRay =
      camera.generateRay(camera.getWidth() - 1, camera.getHeight() - 1);

  // All rays should start at the origin
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(topLeftRay.getOrigin(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(topRightRay.getOrigin(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(bottomLeftRay.getOrigin(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(bottomRightRay.getOrigin(), Vector3D(0, 0, 0)));

  // Verify that corner rays have the expected directions based on FOV
  double fovRadians = (camera.getFieldOfView() * M_PI) / 180.0;
  double aspectRatio =
      static_cast<double>(camera.getWidth()) / camera.getHeight();
  double tanHalfFov = tan(fovRadians / 2.0);

  // The expected corner directions (normalized)
  Vector3D expectedTopLeft =
      Vector3D(-aspectRatio * tanHalfFov, tanHalfFov, -1.0).normalized();
  Vector3D expectedTopRight =
      Vector3D(aspectRatio * tanHalfFov, tanHalfFov, -1.0).normalized();
  Vector3D expectedBottomLeft =
      Vector3D(-aspectRatio * tanHalfFov, -tanHalfFov, -1.0).normalized();
  Vector3D expectedBottomRight =
      Vector3D(aspectRatio * tanHalfFov, -tanHalfFov, -1.0).normalized();

  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(topLeftRay.getDirection(), expectedTopLeft));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(topRightRay.getDirection(), expectedTopRight));
  EXPECT_TRUE(vectorsNearlyEqual_Camera(bottomLeftRay.getDirection(),
                                        expectedBottomLeft));
  EXPECT_TRUE(vectorsNearlyEqual_Camera(bottomRightRay.getDirection(),
                                        expectedBottomRight));
}

// Test ray generation with offset camera position
TEST(CameraTest, OffsetPositionRayGeneration) {
  Vector3D position(10, 20, 30);
  Camera camera(position, 800, 600, 60.0);

  // The center ray should still point along -Z, but from the new position
  Ray centerRay =
      camera.generateRay(camera.getWidth() / 2, camera.getHeight() / 2);

  EXPECT_TRUE(vectorsNearlyEqual_Camera(centerRay.getOrigin(), position));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getDirection(), Vector3D(0, 0, -1)));
}

// Test ray generation with camera rotation
TEST(CameraTest, RotatedCameraRayGeneration) {
  Camera camera;

  // Rotate the camera 90 degrees around Y axis
  // This makes the camera look along the positive X axis
  camera.setRotation(Vector3D(0, 90, 0));

  // The center ray should now point along +X axis
  Ray centerRay =
      camera.generateRay(camera.getWidth() / 2, camera.getHeight() / 2);

  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getOrigin(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getDirection(), Vector3D(1, 0, 0)));

  // Rotate the camera 90 degrees around X axis (looking down)
  camera.setRotation(Vector3D(90, 0, 0));
  centerRay = camera.generateRay(camera.getWidth() / 2, camera.getHeight() / 2);

  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getOrigin(), Vector3D(0, 0, 0)));
  EXPECT_TRUE(
      vectorsNearlyEqual_Camera(centerRay.getDirection(), Vector3D(0, -1, 0)));
}

// Test ray generation with out-of-bounds pixel coordinates
TEST(CameraTest, OutOfBoundsRayGeneration) {
  Camera camera;

  EXPECT_THROW(camera.generateRay(-1, 300), RaytracerException);
  EXPECT_THROW(camera.generateRay(800, 300), RaytracerException);
  EXPECT_THROW(camera.generateRay(400, -1), RaytracerException);
  EXPECT_THROW(camera.generateRay(400, 600), RaytracerException);
}

// Test different field of view values
TEST(CameraTest, FieldOfViewTest) {
  // Create cameras with different FOVs
  Camera narrowCamera(Vector3D(0, 0, 0), 800, 600, 30.0);
  Camera wideCamera(Vector3D(0, 0, 0), 800, 600, 90.0);

  // Get rays through the same corner pixel for each camera
  Ray narrowRay = narrowCamera.generateRay(0, 0);
  Ray wideRay = wideCamera.generateRay(0, 0);

  // The wide FOV ray should have a greater angle from the center ray
  double narrowAngle = acos(narrowRay.getDirection().dot(Vector3D(0, 0, -1)));
  double wideAngle = acos(wideRay.getDirection().dot(Vector3D(0, 0, -1)));

  EXPECT_GT(wideAngle, narrowAngle);
}
