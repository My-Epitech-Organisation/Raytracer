/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_LightFactory
*/

#include <gtest/gtest.h>
#include "../src/scene/lights/LightFactory.hpp"
#include "../src/scene/lights/AmbientLight.hpp"
#include "../src/scene/lights/DirectionalLight.hpp"

using namespace RayTracer;

TEST(LightFactoryTests, CreateAmbientLight) {
  libconfig::Config cfg;
  cfg.readString(R"(
    light: {
      type = "ambient";
      color = [0.2, 0.3, 0.4];
      intensity = 0.6;
    };
  )");

  const auto& setting = cfg.lookup("light");
  std::unique_ptr<Light> light = LightFactory::createLight(setting);

  ASSERT_NE(dynamic_cast<AmbientLight*>(light.get()), nullptr);
}

TEST(LightFactoryTests, CreateDirectionalLight) {
  libconfig::Config cfg;
  cfg.readString(R"(
    light: {
      type = "directional";
      color = [1.0, 0.9, 0.8];
      intensity = 1.2;
      direction = [0, -1, 0];
    };
  )");

  const auto& setting = cfg.lookup("light");
  std::unique_ptr<Light> light = LightFactory::createLight(setting);

  ASSERT_NE(dynamic_cast<DirectionalLight*>(light.get()), nullptr);
}

TEST(LightFactoryTests, UnknownTypeThrows) {
  libconfig::Config cfg;
  cfg.readString(R"(
    light: {
      type = "mystic";
      color = [1, 1, 1];
      intensity = 1;
    };
  )");

  const auto& setting = cfg.lookup("light");
  ASSERT_THROW(LightFactory::createLight(setting), std::runtime_error);
}
