/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/

/**
 * @file LightFactory.cpp
 * @brief Implementation of the factory class for creating different types of light sources
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "LightFactory.hpp"
#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "Light.hpp"
#include "LightingSettings.hpp"
#include "PointLight.hpp"
#include "exceptions/ParserException.hpp"

namespace RayTracer {

LightFactory::Result LightFactory::createLights(
    const libconfig::Setting& setting) {
  Result result;

  float ambient = 0.0f;
  if (setting.lookupValue("ambient", ambient)) {
    result.lights.emplace_back(std::make_unique<AmbientLight>(ambient));
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
        if (!points[i].lookupValue("x", x) || !points[i].lookupValue("y", y) ||
            !points[i].lookupValue("z", z)) {
          throw ParserException("Incomplete point light definition at index " +
                                std::to_string(i));
        }
        result.lights.emplace_back(
            std::make_unique<PointLight>(Vector3D(x, y, z)));
      }
    }
  }

  if (setting.exists("directional")) {
    const libconfig::Setting& dirs = setting["directional"];

    if (dirs.isList() || dirs.isArray()) {
      for (int i = 0; i < dirs.getLength(); ++i) {
        float x = 0, y = 0, z = 0;
        if (!dirs[i].lookupValue("x", x) || !dirs[i].lookupValue("y", y) ||
            !dirs[i].lookupValue("z", z)) {
          throw ParserException(
              "Incomplete directional light definition at index " +
              std::to_string(i));
        }
        result.lights.emplace_back(
            std::make_unique<DirectionalLight>(Vector3D(x, y, z)));
      }
    }
  }

  return result;
}

}  // namespace RayTracer
