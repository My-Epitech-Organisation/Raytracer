/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Directional Light implementation
*/

#include "DirectionalLight.hpp"
#include <sstream>

namespace RayTracer {

DirectionalLight::DirectionalLight(const Vector3D& color,
                                   const Vector3D& direction)
    : Light(color), _direction(direction.normalized()) {}

std::string DirectionalLight::toString() const {
  std::ostringstream oss;
  oss << "DirectionalLight(Color: " << getColor()
      << ", Direction: " << _direction << ")";
  return oss.str();
}

Vector3D DirectionalLight::getDirection() const {
  return _direction;
}

}  // namespace RayTracer
