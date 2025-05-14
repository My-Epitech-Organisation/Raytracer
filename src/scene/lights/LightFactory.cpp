/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/

#include "LightFactory.hpp"
#include "exceptions/ParserException.hpp"

namespace RayTracer {

std::unique_ptr<Light> LightFactory::createLight(
    const libconfig::Setting& setting) {
  float ambient = 0.0f;
  float diffuse = 0.0f;
  std::vector<Vector3D> pointLights;
  std::vector<Vector3D> directionalLights;

  if (!setting.lookupValue("ambient", ambient))
    throw ParserException("Missing or invalid 'ambient' field in Light config");
  if (!setting.lookupValue("diffuse", diffuse))
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
        pointLights.emplace_back(x, y, z);
      }
    } else if (points.isGroup()) {
      float x = 0, y = 0, z = 0;
      if (!points.lookupValue("x", x) || !points.lookupValue("y", y) ||
          !points.lookupValue("z", z)) {
        throw ParserException("Incomplete single point light definition");
      }
      pointLights.emplace_back(x, y, z);
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
        directionalLights.emplace_back(x, y, z);
      }
    } else if (dirs.isGroup()) {
      float x = 0, y = 0, z = 0;
      if (!dirs.lookupValue("x", x) || !dirs.lookupValue("y", y) ||
          !dirs.lookupValue("z", z)) {
        throw ParserException("Incomplete single directional light definition");
      }
      directionalLights.emplace_back(x, y, z);
    }
  }

  return std::make_unique<Light>(ambient, diffuse, pointLights,
                                 directionalLights);
}

}  // namespace RayTracer
