/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/

#include "LightFactory.hpp"
#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "Light.hpp"
#include "LightingSettings.hpp"
#include "PointLight.hpp"
#include "../../../include/exceptions/ParserException.hpp"

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
        int xInt = 0, yInt = 0, zInt = 0;
        
        bool hasX = points[i].lookupValue("x", x);
        bool hasY = points[i].lookupValue("y", y);
        bool hasZ = points[i].lookupValue("z", z);
        
        if (!hasX) {
          hasX = points[i].lookupValue("x", xInt);
          if (hasX) x = static_cast<float>(xInt);
        }
        
        if (!hasY) {
          hasY = points[i].lookupValue("y", yInt);
          if (hasY) y = static_cast<float>(yInt);
        }
        
        if (!hasZ) {
          hasZ = points[i].lookupValue("z", zInt);
          if (hasZ) z = static_cast<float>(zInt);
        }
        
        if (!hasX || !hasY || !hasZ) {
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
        int xInt = 0, yInt = 0, zInt = 0;
        
        bool hasX = dirs[i].lookupValue("x", x);
        bool hasY = dirs[i].lookupValue("y", y);
        bool hasZ = dirs[i].lookupValue("z", z);
        
        if (!hasX) {
          hasX = dirs[i].lookupValue("x", xInt);
          if (hasX) x = static_cast<float>(xInt);
        }
        
        if (!hasY) {
          hasY = dirs[i].lookupValue("y", yInt);
          if (hasY) y = static_cast<float>(yInt);
        }
        
        if (!hasZ) {
          hasZ = dirs[i].lookupValue("z", zInt);
          if (hasZ) z = static_cast<float>(zInt);
        }
        
        if (!hasX || !hasY || !hasZ) {
          throw ParserException("Incomplete directional light definition at index " +
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
