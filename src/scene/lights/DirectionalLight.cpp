/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight
*/

#include "DirectionalLight.hpp"

namespace RayTracer {

DirectionalLight::DirectionalLight(const Vector3D& direction)
    : _direction(direction) {}

const Vector3D& DirectionalLight::getDirection() const {
  return _direction;
}

std::string DirectionalLight::toString() const {
  return "DirectionalLight with direction (" +
         std::to_string(_direction.getX()) + ", " +
         std::to_string(_direction.getY()) + ", " +
         std::to_string(_direction.getZ()) + ")";
}

}  // namespace RayTracer
