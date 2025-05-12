/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Light
*/

#include <gtest/gtest.h>
#include "../src/scene/lights/AmbientLight.hpp"
#include "../src/scene/lights/DirectionalLight.hpp"
#include "../src/scene/lights/Light.hpp"

using namespace RayTracer;

TEST(PointLightTest, Initialization) {
  Vector3D position(1.0, 2.0, 3.0);
  Vector3D color(0.5, 0.5, 0.5);
  PointLight light(position, color);

  EXPECT_EQ(light.getType(), LightType::POINT);
  EXPECT_EQ(light.getPosition(), position);
  EXPECT_EQ(light.getColor(), color);
}

TEST(DirectionalLightTest, Initialization) {
  Vector3D direction(1.0, 0.0, 0.0);
  Vector3D color(0.8, 0.6, 0.4);
  DirectionalLight light(direction, color);

  EXPECT_EQ(light.getType(), LightType::DIRECTIONAL);
  EXPECT_EQ(light.getDirection(), direction);
  EXPECT_EQ(light.getColor(), color);
}

TEST(PointLightTest, ToString) {
  PointLight light(Vector3D(1, 2, 3), Vector3D(0.1, 0.2, 0.3));
  std::ostringstream oss;
  oss << light;
  std::string str = oss.str();
  EXPECT_TRUE(str.find("PointLight") != std::string::npos);
}

TEST(DirectionalLightTest, ToString) {
  DirectionalLight light(Vector3D(0, 1, 0), Vector3D(1, 1, 1));
  std::ostringstream oss;
  oss << light;
  std::string str = oss.str();
  EXPECT_TRUE(str.find("DirectionalLight") != std::string::npos);
}
