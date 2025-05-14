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
    auto parsedLight = parser.parseLights(lightsSetting);

    ASSERT_NE(parsedLight, nullptr);
    EXPECT_FLOAT_EQ(parsedLight->getAmbient(), 0.4f);
    EXPECT_FLOAT_EQ(parsedLight->getDiffuse(), 0.6f);
    EXPECT_EQ(parsedLight->getPointLights().size(), 1);
    EXPECT_EQ(parsedLight->getDirectionalLights().size(), 0);

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
  EXPECT_THROW(LightFactory::createLight(lights), RayTracer::ParserException);
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
  EXPECT_THROW(LightFactory::createLight(lights), RayTracer::ParserException);
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
  auto light = LightFactory::createLight(lights);

  ASSERT_NE(light, nullptr);
  EXPECT_EQ(light->getPointLights().size(), 0);
  EXPECT_EQ(light->getDirectionalLights().size(), 0);
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
  auto light = LightFactory::createLight(lights);

  ASSERT_EQ(light->getPointLights().size(), 2);
  EXPECT_TRUE(light->getPointLights()[0].isEqual(Vector3D(1, 1, 1)));
  EXPECT_TRUE(light->getPointLights()[1].isEqual(Vector3D(2, 2, 2)));
}
