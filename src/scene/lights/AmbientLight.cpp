/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ambient Light implementation
*/

#include "AmbientLight.hpp"
#include <sstream>

namespace RayTracer {

AmbientLight::AmbientLight(const Vector3D& color) : Light(color) {}

std::string AmbientLight::toString() const {
  std::ostringstream oss;
  oss << "AmbientLight(Color: " << getColor() << ")";
  return oss.str();
}

}  // namespace RayTracer
