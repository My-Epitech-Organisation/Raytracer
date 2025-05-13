/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/

#include "LightFactory.hpp"
#include <stdexcept>

namespace RayTracer {

std::unique_ptr<Light> LightFactory::createLight(
    const libconfig::Setting& setting) {
  float ambient = 0.0f;
  float diffuse = 0.0f;
  std::vector<Vector3D> pointLights;
  std::vector<Vector3D> directionalLights;

  setting.lookupValue("ambient", ambient);
  setting.lookupValue("diffuse", diffuse);

  if (setting.exists("point")) {
    const libconfig::Setting& points = setting["point"];
    for (int i = 0; i < points.getLength(); ++i) {
      float x = 0, y = 0, z = 0;
      points[i].lookupValue("x", x);
      points[i].lookupValue("y", y);
      points[i].lookupValue("z", z);
      pointLights.emplace_back(x, y, z);
    }
  }

  if (setting.exists("directional")) {
    const libconfig::Setting& dirs = setting["directional"];
    for (int i = 0; i < dirs.getLength(); ++i) {
      float x = 0, y = 0, z = 0;
      dirs[i].lookupValue("x", x);
      dirs[i].lookupValue("y", y);
      dirs[i].lookupValue("z", z);
      directionalLights.emplace_back(x, y, z);
    }
  }

  return std::make_unique<Light>(ambient, diffuse, pointLights,
                                 directionalLights);
}

}  // namespace RayTracer
