/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_SceneParser
*/

#include <gtest/gtest.h>
#include <libconfig.h++>
#include "../src/scene/parser/SceneParser.hpp"

using namespace RayTracer;
using namespace libconfig;

TEST(SceneParserTest, ParseCamera) {
  Config cfg;
  const char* cfgText = R"(
        camera = {
            resolution = { width = 1920; height = 1080; };
            position = { x = 0; y = -100; z = 20; };
            rotation = { x = 0; y = 0; z = 0; };
            fieldOfView = 72.0;
        };
    )";

  try {
    cfg.readString(cfgText);
    const Setting& camera = cfg.lookup("camera");

    SceneParser parser;
    Camera result = parser.parseCamera(camera);

    EXPECT_EQ(result.getResolution(), Resolution(1920, 1080));
    EXPECT_EQ(result.getPosition(), Vector3D(0.0f, -100.0f, 20.0f));
    EXPECT_EQ(result.getRotation(), Vector3D(0.0f, 0.0f, 0.0f));
    EXPECT_FLOAT_EQ(result.getFov(), 72.0f);
  } catch (const std::exception& e) {
    std::cerr << "⚠️ Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during camera parsing: " << e.what();
  }
}
