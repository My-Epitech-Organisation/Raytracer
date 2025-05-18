/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight
*/

/**
 * @file AmbientLight.cpp
 * @brief Implementation of the ambient light class which provides constant,
 * non-directional illumination
 * @author @santiago
 * @date 2025-05-16
 * @version 1.0
 */

#include "AmbientLight.hpp"

namespace RayTracer {

AmbientLight::AmbientLight(float ambient) : _ambient(ambient) {}

AmbientLight::AmbientLight(float ambient, const Color& color)
    : _ambient(ambient), _color(color) {}

Vector3D AmbientLight::getDirectionFrom(const Vector3D& point) const {
  (void)point;
  return Vector3D(0, 0, 1);
}

double AmbientLight::getDistanceFrom(const Vector3D& point) const {
  (void)point;
  return 0.0;
}

double AmbientLight::getIntensityAt(const Vector3D& point) const {
  (void)point;
  return _ambient;
}

Color AmbientLight::getColor() const {
  return _color;
}

bool AmbientLight::castsShadows() const {
  return false;
}

Ray AmbientLight::getShadowRay(const Vector3D& point) const {
  (void)point;
  return Ray(point, Vector3D(0, 0, 1));
}

std::shared_ptr<ILight> AmbientLight::clone() const {
  auto clone = std::make_shared<AmbientLight>(_ambient);
  clone->_color = _color;
  return clone;
}

Vector3D AmbientLight::getPosition() const {
  return Vector3D(0, 0, 0);
}

float AmbientLight::getAmbient() const {
  return _ambient;
}

std::string AmbientLight::toString() const {
  return "AmbientLight with intensity: " + std::to_string(_ambient);
}

}  // namespace RayTracer
