/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_PrimitiveFactory
*/

/**
 * @file test_PrimitiveFactory.cpp
 * @brief Unit tests for the PrimitiveFactory class to validate creation of
 * primitives
 * @author Santi
 * @date 2025-05-17
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include "../src/scene/primitives/PrimitiveFactory.hpp"

using namespace RayTracer;
using namespace libconfig;

TEST(PrimitiveFactoryTest, CreateSphere) {
  Config cfg;
  const char* cfgText = R"(
  {
    x = 10; y = 20; z = 30; r = 15;
    color = { r = 255; g = 0; b = 0; };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& sphereSetting = cfg.getRoot();

    auto sphere = PrimitiveFactory::createSphere(sphereSetting);

    EXPECT_TRUE(sphere->getCenter().isEqual(Vector3D(10, 20, 30)));
    EXPECT_DOUBLE_EQ(sphere->getRadius(), 15.0);
    EXPECT_TRUE(sphere->getColor().isEqual(Color(static_cast<uint8_t>(255),
                                                 static_cast<uint8_t>(0),
                                                 static_cast<uint8_t>(0))));

  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during sphere creation: " << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreatePlane) {
  Config cfg;
  const char* cfgText = R"(
  {
    axis = "Z"; position = -20;
    color = { r = 64; g = 64; b = 255; };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& planeSetting = cfg.getRoot();

    auto plane = PrimitiveFactory::createPlane(planeSetting);

    EXPECT_EQ(plane->getAxis(), Axis::Z);
    EXPECT_FLOAT_EQ(plane->getPosition(), -20);
    EXPECT_TRUE(plane->getColor().isEqual(Color(static_cast<uint8_t>(64),
                                                static_cast<uint8_t>(64),
                                                static_cast<uint8_t>(255))));

    std::cout << "Plane created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during plane creation: " << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreateCone) {
  Config cfg;
  const char* cfgText = R"(
  {
    apex = { x = 0; y = 0; z = 0; };
    axis = { x = 0; y = 1; z = 0; };
    angle = 30;
    color = { r = 255; g = 255; b = 0; };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& coneSetting = cfg.getRoot();

    auto cone = PrimitiveFactory::createCone(coneSetting);

    EXPECT_TRUE(cone->getApex().isEqual(Vector3D(0, 0, 0)));
    EXPECT_TRUE(cone->getAxis().isEqual(Vector3D(0, 1, 0)));
    EXPECT_DOUBLE_EQ(cone->getAngleDegrees(), 30.0);
    EXPECT_TRUE(cone->getColor().isEqual(Color(static_cast<uint8_t>(255),
                                               static_cast<uint8_t>(255),
                                               static_cast<uint8_t>(0))));

    std::cout << "Cone created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during cone creation: " << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreateLimitedCone) {
  Config cfg;
  const char* cfgText = R"(
  {
    apex = { x = 0; y = 0; z = 0; };
    axis = { x = 0; y = 1; z = 0; };
    angle = 30;
    height = 10;
    caps = 1;
    color = { r = 255; g = 128; b = 0; };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& limitedConeSetting = cfg.getRoot();

    auto limitedCone = PrimitiveFactory::createLimitedCone(limitedConeSetting);

    EXPECT_TRUE(limitedCone->getApex().isEqual(Vector3D(0, 0, 0)));
    EXPECT_TRUE(limitedCone->getAxis().isEqual(Vector3D(0, 1, 0)));
    EXPECT_DOUBLE_EQ(limitedCone->getAngleDegrees(), 30.0);
    EXPECT_DOUBLE_EQ(limitedCone->getHeight(), 10.0);
    EXPECT_TRUE(limitedCone->hasCaps());
    EXPECT_TRUE(limitedCone->getColor().isEqual(
        Color(static_cast<uint8_t>(255), static_cast<uint8_t>(128),
              static_cast<uint8_t>(0))));

    std::cout << "Limited cone created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during limited cone creation: " << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreateCylinder) {
  Config cfg;
  const char* cfgText = R"(
  {
    radius = 5;
    color = { r = 0; g = 255; b = 255; };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& cylinderSetting = cfg.getRoot();

    auto cylinder = PrimitiveFactory::createCylinder(cylinderSetting);

    EXPECT_DOUBLE_EQ(cylinder->getRadius(), 5.0);
    EXPECT_TRUE(cylinder->getColor().isEqual(Color(static_cast<uint8_t>(0),
                                                   static_cast<uint8_t>(255),
                                                   static_cast<uint8_t>(255))));

    std::cout << "Cylinder created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during cylinder creation: " << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreateLimitedCylinder) {
  Config cfg;
  const char* cfgText = R"(
  {
    radius = 5;
    height = 10;
    color = { r = 128; g = 0; b = 255; };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& limitedCylinderSetting = cfg.getRoot();

    auto limitedCylinder =
        PrimitiveFactory::createLimitedCylinder(limitedCylinderSetting);

    EXPECT_DOUBLE_EQ(limitedCylinder->getRadius(), 5.0);
    EXPECT_DOUBLE_EQ(limitedCylinder->getHeight(), 10.0);
    EXPECT_TRUE(limitedCylinder->getColor().isEqual(
        Color(static_cast<uint8_t>(128), static_cast<uint8_t>(0),
              static_cast<uint8_t>(255))));

    std::cout << "Limited cylinder created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during limited cylinder creation: " << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreatePrimitivesMultipleTypes) {
  Config cfg;
  const char* cfgText = R"(
  primitives:
  {
    spheres = (
      { x = 10; y = 20; z = 30; r = 15; color = { r = 255; g = 0; b = 0; }; },
      { x = -10; y = 0; z = 5; r = 5; color = { r = 0; g = 255; b = 0; }; }
    );
    planes = (
      { axis = "Z"; position = -20; color = { r = 64; g = 64; b = 255; }; }
    );
  };
  )";

  try {
    cfg.readString(cfgText);
    const Setting& primitivesSetting = cfg.lookup("primitives");

    auto result = PrimitiveFactory::createPrimitives(primitivesSetting);

    EXPECT_EQ(result.primitives.size(), 3);  // 2 spheres + 1 plane

    std::cout << "Multiple primitives created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during multiple primitive creation: "
           << e.what();
  }
}

TEST(PrimitiveFactoryTest, CreateWithTransformation) {
  Config cfg;
  const char* cfgText = R"(
  {
    radius = 5;
    color = { r = 0; g = 255; b = 255; };
    transform = {
      translate = { x = 10; y = 5; z = 3; };
      rotate = { x = 45; y = 0; z = 90; };
      scale = { x = 2; y = 2; z = 2; };
    };
  }
  )";

  try {
    cfg.readString(cfgText);
    const Setting& cylinderSetting = cfg.getRoot();

    auto cylinder = PrimitiveFactory::createCylinder(cylinderSetting);

    EXPECT_DOUBLE_EQ(cylinder->getRadius(), 5.0);
    EXPECT_TRUE(cylinder->getColor().isEqual(Color(static_cast<uint8_t>(0),
                                                   static_cast<uint8_t>(255),
                                                   static_cast<uint8_t>(255))));

    // Verify the transform was applied by checking the transformed axis
    // The original axis is (0,1,0), but after rotation it should be different
    Vector3D transformedAxis = cylinder->getAxis();
    EXPECT_FALSE(transformedAxis.isEqual(Vector3D(0, 1, 0)));

    std::cout << "Cylinder with transformation created successfully!\n";
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during cylinder with transform creation: "
           << e.what();
  }
}
