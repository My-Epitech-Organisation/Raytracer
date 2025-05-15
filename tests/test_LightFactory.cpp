/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_LightFactory
*/

#include <gtest/gtest.h>
#include "../include/exceptions/ParserException.hpp"
#include "../src/scene/lights/LightFactory.hpp"
#include "../src/scene/parser/SceneParser.hpp"

using namespace RayTracer;

TEST(SceneParserTest, ParseLightConfigurationFromFile) {
  libconfig::Config cfg;

  try {
    cfg.readFile("tests/configs/test_lights.cfg");
    const auto& lightsSetting = cfg.lookup("lights");

    SceneParser parser;
    auto parsedLight = parser.parseLightingSettings(lightsSetting);

    EXPECT_FLOAT_EQ(parsedLight.first.diffuse, 0.6f);
    EXPECT_EQ(parsedLight.second.size(), 2);

    EXPECT_EQ(parsedLight.second[0]->toString(),
              "AmbientLight with intensity: 0.400000");
    EXPECT_EQ(parsedLight.second[1]->toString(),
              "PointLight at (400.000000, 100.000000, 500.000000)");

    SUCCEED();

  } catch (const std::exception& e) {
    FAIL() << "Parsing lights failed: " << e.what();
  }
}

TEST(LightFactoryTest, MissingAmbientThrows) {
  libconfig::Config cfg;
  cfg.readString(R"(
    lights: {
      diffuse = 0.6;
    };
  )");

  const auto& lights = cfg.lookup("lights");
  EXPECT_THROW(LightFactory::createLights(lights), RayTracer::ParserException);
}

TEST(LightFactoryTest, InvalidDirectionalDefinition) {
  libconfig::Config cfg;
  cfg.readString(R"(
    lights: {
      ambient = 0.3;
      diffuse = 0.5;
      directional = (
        { x = 0; y = -1; }
      );
    };
  )");

  const auto& lights = cfg.lookup("lights");
  EXPECT_THROW(LightFactory::createLights(lights), RayTracer::ParserException);
}

TEST(LightFactoryTest, AmbientDiffuseOnly) {
  libconfig::Config cfg;
  cfg.readString(R"(
    lights: {
      ambient = 0.1;
      diffuse = 0.2;
    };
  )");

  const auto& lights = cfg.lookup("lights");
  auto light = LightFactory::createLights(lights);

  EXPECT_EQ(light.lights.size(), 1);
}

TEST(LightFactoryTest, MultiplePointLights) {
  libconfig::Config cfg;
  cfg.readString(R"(
    lights: {
      ambient = 0.2;
      diffuse = 0.5;
      point = (
        { x = 1.0; y = 1.0; z = 1.0; },
        { x = 2.0; y = 2.0; z = 2.0; }
      );
    };
  )");

  const auto& lights = cfg.lookup("lights");
  auto light = LightFactory::createLights(lights);

  ASSERT_EQ(light.lights.size(), 3);
  EXPECT_EQ(light.lights[0]->toString(),
            "AmbientLight with intensity: 0.200000");
  EXPECT_EQ(light.lights[1]->toString(),
            "PointLight at (1.000000, 1.000000, 1.000000)");
  EXPECT_EQ(light.lights[2]->toString(),
            "PointLight at (2.000000, 2.000000, 2.000000)");
}

TEST(LightFactoryTest, DirectionalLightParsedCorrectly) {
  libconfig::Config cfg;
  cfg.readString(R"(
    lights: {
      ambient = 0.1;
      diffuse = 0.3;
      directional = (
        { x = -1.0; y = -0.5; z = 0.0; }
      );
    };
  )");

  const auto& lights = cfg.lookup("lights");
  auto light = LightFactory::createLights(lights);

  ASSERT_EQ(light.lights.size(), 2);
  EXPECT_EQ(light.lights[0]->toString(),
            "AmbientLight with intensity: 0.100000");
  EXPECT_EQ(light.lights[1]->toString(),
            "DirectionalLight with direction (-1.000000, -0.500000, 0.000000)");
}

TEST(LightFactoryTest, PointAndDirectionalLightsParsedCorrectly) {
  libconfig::Config cfg;
  cfg.readString(R"(
    lights: {
      ambient = 0.2;
      diffuse = 0.6;
      point = (
        { x = 0.0; y = 1.0; z = 2.0; }
      );
      directional = (
        { x = 0.0; y = -1.0; z = 0.0; }
      );
    };
  )");

  const auto& lights = cfg.lookup("lights");
  auto light = LightFactory::createLights(lights);

  ASSERT_EQ(light.lights.size(), 3);
  EXPECT_EQ(light.lights[0]->toString(),
            "AmbientLight with intensity: 0.200000");
  EXPECT_EQ(light.lights[1]->toString(),
            "PointLight at (0.000000, 1.000000, 2.000000)");
  EXPECT_EQ(light.lights[2]->toString(),
            "DirectionalLight with direction (0.000000, -1.000000, 0.000000)");
}
