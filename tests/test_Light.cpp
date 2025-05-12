/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Light
*/

#include <gtest/gtest.h>
#include <sstream>
#include "../src/scene/lights/Light.hpp"

using namespace RayTracer;

TEST(LightTest, BasicInitialization) {
  std::vector<Vector3D> points = {Vector3D(1, 2, 3)};
  std::vector<Vector3D> dirs = {Vector3D(0, -1, 0)};
  Light light(0.5f, 0.8f, points, dirs);

  EXPECT_FLOAT_EQ(light.getAmbient(), 0.5f);
  EXPECT_FLOAT_EQ(light.getDiffuse(), 0.8f);
  EXPECT_EQ(light.getPointLights().size(), 1);
  EXPECT_EQ(light.getDirectionalLights().size(), 1);
}

TEST(LightTest, ToStringOutput) {
  Light light(0.2f, 0.3f, {Vector3D(1, 2, 3)}, {Vector3D(4, 5, 6)});
  std::string str = light.toString();

  EXPECT_NE(str.find("Ambient: 0.2"), std::string::npos);
  EXPECT_NE(str.find("Diffuse: 0.3"), std::string::npos);
  EXPECT_NE(str.find("Vector3D(1, 2, 3)"), std::string::npos);
  EXPECT_NE(str.find("Vector3D(4, 5, 6)"), std::string::npos);
}
