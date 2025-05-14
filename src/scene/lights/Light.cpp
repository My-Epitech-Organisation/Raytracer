/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Light
*/

#include "Light.hpp"
#include <sstream>

namespace RayTracer {

Light::Light() : _ambient(0.0f), _diffuse(0.0f) {}

Light::Light(float ambient, float diffuse,
             const std::vector<Vector3D>& pointLights,
             const std::vector<Vector3D>& directionalLights)
    : _ambient(ambient),
      _diffuse(diffuse),
      _pointLights(pointLights),
      _directionalLights(directionalLights) {}

float Light::getAmbient() const {
  return _ambient;
}

float Light::getDiffuse() const {
  return _diffuse;
}

const std::vector<Vector3D>& Light::getPointLights() const {
  return _pointLights;
}

const std::vector<Vector3D>& Light::getDirectionalLights() const {
  return _directionalLights;
}

void Light::addPointLight(const Vector3D& light) {
  _pointLights.push_back(light);
}

void Light::addDirectionalLight(const Vector3D& light) {
  _directionalLights.push_back(light);
}

std::string Light::toString() const {
  std::ostringstream oss;
  oss << "Ambient: " << _ambient << ", Diffuse: " << _diffuse << "\n";
  oss << "Point Lights: \n";
  for (const auto& p : _pointLights) {
    oss << "  " << p << "\n";
  }
  oss << "Directional Lights: \n";
  for (const auto& d : _directionalLights) {
    oss << "  " << d << "\n";
  }
  return oss.str();
}

}  // namespace RayTracer
