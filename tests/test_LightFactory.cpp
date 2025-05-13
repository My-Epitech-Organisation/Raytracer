/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_LightFactory
*/

#include <gtest/gtest.h>
#include "../src/scene/lights/LightFactory.hpp"
#include "../src/scene/parser/SceneParser.hpp"

using namespace RayTracer;

TEST(SceneParserTest, ParseLightConfiguration) {
  libconfig::Config cfg;
  const char* configText = R"(
    lights: {
      ambient = 0.4;
      diffuse = 0.6;
      point = (
        { x = 400; y = 100; z = 500; }
      );
      directional = (
        { x = 0; y = -1; z = 0; }
      );
    };
  )";

  try {
    cfg.readString(configText);
    const auto& lightsSetting = cfg.lookup("lights");

    SceneParser parser;
    parser.parseLights(lightsSetting);

    SUCCEED();
  } catch (const std::exception& e) {
    FAIL() << "Parsing lights failed: " << e.what();
  }
}
