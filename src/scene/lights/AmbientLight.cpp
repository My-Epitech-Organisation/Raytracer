/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ambient Light implementation
*/

#include "AmbientLight.hpp"

namespace RayTracer {

AmbientLight::AmbientLight(const Vector3D& color, float intensity)
    : Light(color, intensity) {}

Vector3D AmbientLight::illuminate(const Vector3D& /*point*/) const {
  return Vector3D(0, 0, 0);  // No direction
}

}  // namespace RayTracer
