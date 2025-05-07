/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/

#include "LightFactory.hpp"
#include <stdexcept>
#include "../lights/AmbientLight.hpp"
#include "../lights/DirectionalLight.hpp"

namespace RayTracer {

std::unique_ptr<Light> LightFactory::createLight(
    const libconfig::Setting& setting) {
  if (!setting.exists("type") || !setting["type"].isScalar())
    throw std::runtime_error("Light missing valid type");

  std::string type = setting["type"];

  Vector3D color(1, 1, 1);
  if (setting.exists("color")) {
    auto& c = setting["color"];
    if (c.getLength() >= 3) {
      color.setX(static_cast<float>(c[0]));
      color.setY(static_cast<float>(c[1]));
      color.setZ(static_cast<float>(c[2]));
    } else {
      throw std::runtime_error("Color setting must have 3 components");
    }
  }

  float intensity = setting.exists("intensity")
                        ? static_cast<float>(setting["intensity"])
                        : 1.0f;

  if (type == "ambient") {
    return std::make_unique<AmbientLight>(color, intensity);
  } else if (type == "directional") {
    Vector3D direction(0, -1, 0);
    if (setting.exists("direction")) {
      auto& dir = setting["direction"];
      if (dir.getLength() >= 3) {
        direction.setX(static_cast<float>(dir[0]));
        direction.setY(static_cast<float>(dir[1]));
        direction.setZ(static_cast<float>(dir[2]));
      } else {
        throw std::runtime_error("Direction must have 3 components");
      }
    }
    return std::make_unique<DirectionalLight>(color, intensity, direction);
  }

  throw std::runtime_error("Unknown light type: " + type);
}

}  // namespace RayTracer
