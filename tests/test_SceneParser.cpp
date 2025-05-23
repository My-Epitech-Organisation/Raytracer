/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_SceneParser
*/

/**
 * @file test_SceneParser.cpp
 * @brief Unit tests for the SceneParser class to validate parsing of scene
 * configuration files
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <libconfig.h++>
#include "../src/core/Color.hpp"
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

    EXPECT_EQ(result.getWidth(), 1920);
    EXPECT_EQ(result.getHeight(), 1080);

    EXPECT_TRUE(result.getPosition().isEqual(Vector3D(0, -100, 20)));
    EXPECT_TRUE(result.getRotation().isEqual(Vector3D(0, 0, 0)));

    EXPECT_FLOAT_EQ(result.getFieldOfView(), 72.0f);

  } catch (const std::exception& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during camera parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParseOneSphere) {
  Config cfg;

  try {
    cfg.readFile("tests/configs/test_primitives.cfg");
    const auto& sphereSetting = cfg.lookup("primitives.spheres")[0];

    SceneParser parser;
    Sphere result = parser.parseSphere(sphereSetting);

    EXPECT_TRUE(result.getCenter().isEqual(Vector3D(60, 5, 40)));
    EXPECT_DOUBLE_EQ(result.getRadius(), 25.0);
    EXPECT_TRUE(result.getColor().isEqual(Color(static_cast<uint8_t>(255),
                                                static_cast<uint8_t>(64),
                                                static_cast<uint8_t>(64))));

    std::cout << "Sphere parsed successfully!\n";
  } catch (const std::exception& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during sphere parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParseMultipleSpheres) {
  Config cfg;

  try {
    cfg.readFile("tests/configs/test_spheres.cfg");
    const auto& spheresSetting = cfg.lookup("spheres");

    SceneParser parser;
    std::vector<Sphere> spheres = parser.parseSpheres(spheresSetting);

    ASSERT_EQ(spheres.size(), 2);

    EXPECT_TRUE(spheres[0].getCenter().isEqual(Vector3D(60, 5, 40)));
    EXPECT_DOUBLE_EQ(spheres[0].getRadius(), 25.0);
    EXPECT_TRUE(spheres[0].getColor().isEqual(
        Color((uint8_t)255, (uint8_t)64, (uint8_t)64)));

    EXPECT_TRUE(spheres[1].getCenter().isEqual(Vector3D(-40, 20, -10)));
    EXPECT_DOUBLE_EQ(spheres[1].getRadius(), 35.0);
    EXPECT_TRUE(spheres[1].getColor().isEqual(
        Color((uint8_t)64, (uint8_t)255, (uint8_t)64)));

    std::cout << "All spheres parsed successfully!\n";
  } catch (const std::exception& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during multiple sphere parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParseOnePlane) {
  Config cfg;
  const char* cfgText = R"(
  planes = (
    { axis = "Z"; position = -20; color = { r = 64; g = 64; b = 255; }; }
  );
  )";

  try {
    cfg.readString(cfgText);
    const Setting& planeSetting = cfg.lookup("planes")[0];

    SceneParser parser;
    Plane result = parser.parsePlane(planeSetting);

    EXPECT_EQ(result.getAxis(), Axis::Z);
    EXPECT_FLOAT_EQ(result.getPosition(), -20);
    EXPECT_TRUE(result.getColor().isEqual(Color(static_cast<uint8_t>(64),
                                                static_cast<uint8_t>(64),
                                                static_cast<uint8_t>(255))));

    std::cout << "Plane parsed successfully!\n";

  } catch (const SettingTypeException& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Setting type error during plane parsing: " << e.what();
  } catch (const std::exception& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during plane parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParseMultiplePlanes) {
  Config cfg;
  const char* cfgText = R"(
  planes = (
    { axis = "X"; position = 10; color = { r = 255; g = 255; b = 0; }; },
    { axis = "Y"; position = -5; color = { r = 0; g = 255; b = 255; }; }
  );
  )";

  try {
    cfg.readString(cfgText);
    const Setting& planesSetting = cfg.lookup("planes");

    SceneParser parser;
    std::vector<Plane> planes = parser.parsePlanes(planesSetting);

    ASSERT_EQ(planes.size(), 2);

    EXPECT_EQ(planes[0].getAxis(), Axis::X);
    EXPECT_FLOAT_EQ(planes[0].getPosition(), 10);
    EXPECT_TRUE(planes[0].getColor().isEqual(Color(static_cast<uint8_t>(255),
                                                   static_cast<uint8_t>(255),
                                                   static_cast<uint8_t>(0))));

    EXPECT_EQ(planes[1].getAxis(), Axis::Y);
    EXPECT_FLOAT_EQ(planes[1].getPosition(), -5);
    EXPECT_TRUE(planes[1].getColor().isEqual(Color(static_cast<uint8_t>(0),
                                                   static_cast<uint8_t>(255),
                                                   static_cast<uint8_t>(255))));

    std::cout << "All planes parsed successfully!\n";
  } catch (const std::exception& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during multiple plane parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParseLights) {
  Config cfg;

  try {
    cfg.readFile("tests/configs/test_lights.cfg");
    const auto& lightsSetting = cfg.lookup("lights");

    SceneParser parser;
    parser.parseLightingSettings(lightsSetting);

    std::cout << "Light is parsed successfully!\n";
  } catch (const std::exception& e) {
    std::cerr << "[WARNING] Error loading config: " << e.what() << "\n";
    FAIL() << "Exception thrown during lights parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParsePrimitivesDispatch) {
  Config cfg;

  try {
    cfg.readFile("tests/configs/test_primitives.cfg");
    const auto& primitivesSetting = cfg.lookup("primitives");

    SceneParser parser;
    parser.parsePrimitives(primitivesSetting);

    std::cout << "Primitives parsed.\n";

  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during primitive parsing: " << e.what();
  }
}

TEST(SceneParserTest, ParseUnknownPrimitiveType) {
  Config cfg;

  try {
    cfg.readFile("tests/configs/test_primitives.cfg");
    const auto& primitivesSetting = cfg.lookup("primitives");

    SceneParser parser;
    parser.parsePrimitives(primitivesSetting);

    std::cout << "Unknown primitive types were ignored gracefully.\n";

  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during parsing: " << e.what();
  }
}
