/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Light
*/

/**
 * @file test_Light.cpp
 * @brief Unit tests for Light classes to validate lighting calculations and
 * behaviors
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <sstream>
#include "../src/scene/lights/DirectionalLight.hpp"
#include "../src/scene/lights/Light.hpp"
#include "../src/scene/lights/PointLight.hpp"

using namespace RayTracer;

TEST(LightTest, BasicInitialization) {
  std::vector<Vector3D> points = {Vector3D(1, 2, 3)};
  std::vector<Vector3D> dirs = {Vector3D(0, -1, 0)};
  PointLight pointLight(points[0]);
  DirectionalLight directionalLight(dirs[0]);

  EXPECT_EQ(pointLight.getPosition(), points[0]);
  EXPECT_EQ(directionalLight.getDirection(), dirs[0]);
}

TEST(LightTest, ToStringOutput) {
  PointLight pointLight(Vector3D(1, 2, 3));
  DirectionalLight directionalLight(Vector3D(4, 5, 6));

  std::string pointStr = pointLight.toString();
  std::string dirStr = directionalLight.toString();

  EXPECT_NE(pointStr.find("PointLight"), std::string::npos);
  EXPECT_NE(dirStr.find("DirectionalLight"), std::string::npos);
}

TEST(LightTest, DefaultInitialization) {
  PointLight pointLight(Vector3D(0, 0, 0));
  DirectionalLight directionalLight(Vector3D(0, 0, 0));

  EXPECT_EQ(pointLight.getPosition(), Vector3D(0, 0, 0));
  EXPECT_EQ(directionalLight.getDirection(), Vector3D(0, 0, 0));
}

TEST(LightTest, AddLights) {
  PointLight pointLight(Vector3D(1, 2, 3));
  DirectionalLight directionalLight(Vector3D(4, 5, 6));

  EXPECT_EQ(pointLight.getPosition(), Vector3D(1, 2, 3));
  EXPECT_EQ(directionalLight.getDirection(), Vector3D(4, 5, 6));
}
