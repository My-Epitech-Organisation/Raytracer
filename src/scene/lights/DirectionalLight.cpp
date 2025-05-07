/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Directional Light implementation
*/

#include "DirectionalLight.hpp"

namespace RayTracer {

DirectionalLight::DirectionalLight(const Vector3D& color, float intensity,
                                   const Vector3D& direction)
    : Light(color, intensity), _direction(direction.normalized()) {}

Vector3D DirectionalLight::illuminate(const Vector3D& /*point*/) const {
  // This method could later simulate shadows or other effects
  return _direction;
}

Vector3D DirectionalLight::getDirection() const {
  return _direction;
}

}  // namespace RayTracer
