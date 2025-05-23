/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/

/**
 * @file LightFactory.cpp
 * @brief Implementation of the factory class for creating different types of
 * light sources
 * @author @santiago
 * @date 2025-05-16
 * @version 1.0
 */

#include "LightFactory.hpp"
#include "../../../include/exceptions/ParserException.hpp"
#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "LightingSettings.hpp"
#include "PointLight.hpp"

namespace RayTracer {

LightFactory::Result LightFactory::createLights(
    const libconfig::Setting& setting) {
  Result result;

  float ambient = 0.0f;
  if (setting.lookupValue("ambient", ambient)) {
    Color ambientColor =
        Color(static_cast<uint8_t>(255), static_cast<uint8_t>(255),
              static_cast<uint8_t>(255));
    if (setting.exists("ambientColor")) {
      const libconfig::Setting& colorSetting = setting["ambientColor"];
      int r = 255, g = 255, b = 255;
      if (colorSetting.lookupValue("r", r) &&
          colorSetting.lookupValue("g", g) &&
          colorSetting.lookupValue("b", b)) {
        ambientColor = Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                             static_cast<uint8_t>(b));
      }
    }
    result.lights.emplace_back(
        std::make_unique<AmbientLight>(ambient, ambientColor));
  } else {
    throw ParserException("Missing or invalid 'ambient' field in Light config");
  }
  if (!setting.lookupValue("diffuse", result.settings.diffuse))
    throw ParserException("Missing or invalid 'diffuse' field in Light config");

  if (setting.exists("point")) {
    const libconfig::Setting& points = setting["point"];

    if (points.isList() || points.isArray()) {
      for (int i = 0; i < points.getLength(); ++i) {
        float x = 0, y = 0, z = 0;
        int xInt = 0, yInt = 0, zInt = 0;

        bool hasX = points[i].lookupValue("x", x);
        bool hasY = points[i].lookupValue("y", y);
        bool hasZ = points[i].lookupValue("z", z);

        if (!hasX) {
          hasX = points[i].lookupValue("x", xInt);
          if (hasX)
            x = static_cast<float>(xInt);
        }

        if (!hasY) {
          hasY = points[i].lookupValue("y", yInt);
          if (hasY)
            y = static_cast<float>(yInt);
        }

        if (!hasZ) {
          hasZ = points[i].lookupValue("z", zInt);
          if (hasZ)
            z = static_cast<float>(zInt);
        }

        if (!hasX || !hasY || !hasZ) {
          throw ParserException("Incomplete point light definition at index " +
                                std::to_string(i));
        }

        Color lightColor =
            Color(static_cast<uint8_t>(255), static_cast<uint8_t>(255),
                  static_cast<uint8_t>(255));  // Par défaut blanc
        if (points[i].exists("color")) {
          const libconfig::Setting& colorSetting = points[i]["color"];
          int r = 255, g = 255, b = 255;
          if (colorSetting.lookupValue("r", r) &&
              colorSetting.lookupValue("g", g) &&
              colorSetting.lookupValue("b", b)) {
            lightColor = Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                               static_cast<uint8_t>(b));
          }
        }

        result.lights.emplace_back(
            std::make_unique<PointLight>(Vector3D(x, y, z), lightColor));
      }
    }
  }

  if (setting.exists("directional")) {
    const libconfig::Setting& dirs = setting["directional"];

    if (dirs.isList() || dirs.isArray()) {
      for (int i = 0; i < dirs.getLength(); ++i) {
        float x = 0, y = 0, z = 0;
        int xInt = 0, yInt = 0, zInt = 0;

        bool hasX = false, hasY = false, hasZ = false;

        // Check for direction vector format
        if (dirs[i].exists("direction")) {
          const libconfig::Setting& direction = dirs[i]["direction"];

          hasX = direction.lookupValue("x", x);
          if (!hasX && direction.lookupValue("x", xInt)) {
            x = static_cast<float>(xInt);
            hasX = true;
          }

          hasY = direction.lookupValue("y", y);
          if (!hasY && direction.lookupValue("y", yInt)) {
            y = static_cast<float>(yInt);
            hasY = true;
          }

          hasZ = direction.lookupValue("z", z);
          if (!hasZ && direction.lookupValue("z", zInt)) {
            z = static_cast<float>(zInt);
            hasZ = true;
          }
        } else {
          // Direct x, y, z format
          hasX = dirs[i].lookupValue("x", x);
          if (!hasX && dirs[i].lookupValue("x", xInt)) {
            x = static_cast<float>(xInt);
            hasX = true;
          }

          hasY = dirs[i].lookupValue("y", y);
          if (!hasY && dirs[i].lookupValue("y", yInt)) {
            y = static_cast<float>(yInt);
            hasY = true;
          }

          hasZ = dirs[i].lookupValue("z", z);
          if (!hasZ && dirs[i].lookupValue("z", zInt)) {
            z = static_cast<float>(zInt);
            hasZ = true;
          }
        }

        if (!hasX || !hasY || !hasZ) {
          throw ParserException(
              "Incomplete directional light definition at index " +
              std::to_string(i));
        }

        Color lightColor(static_cast<uint8_t>(255), static_cast<uint8_t>(255),
                         static_cast<uint8_t>(255));  // Default white
        if (dirs[i].exists("color")) {
          const libconfig::Setting& colorSetting = dirs[i]["color"];
          int r = 255, g = 255, b = 255;
          if (colorSetting.lookupValue("r", r) &&
              colorSetting.lookupValue("g", g) &&
              colorSetting.lookupValue("b", b)) {
            lightColor = Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                               static_cast<uint8_t>(b));
          }
        }

        // Intensity is optional for directional lights
        float intensity = 1.0f;
        int intensityInt = 0;
        if (dirs[i].lookupValue("intensity", intensity)) {
          // intensity already set
        } else if (dirs[i].lookupValue("intensity", intensityInt)) {
          intensity = static_cast<float>(intensityInt);
        }

        result.lights.emplace_back(
            std::make_unique<DirectionalLight>(Vector3D(x, y, z), lightColor));
      }
    }
  }

  return result;
}

}  // namespace RayTracer
