/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight
*/

#include "AmbientLight.hpp"

namespace RayTracer {

AmbientLight::AmbientLight(float ambient) : _ambient(ambient) {}

float AmbientLight::getAmbient() const {
  return _ambient;
}

std::string AmbientLight::toString() const {
  return "AmbientLight with intensity: " + std::to_string(_ambient);
}

}  // namespace RayTracer
