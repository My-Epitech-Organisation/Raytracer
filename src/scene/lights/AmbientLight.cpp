/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight
*/

#include "AmbientLight.hpp"
#include <limits>

namespace RayTracer {

AmbientLight::AmbientLight(float ambient) : _ambient(ambient) {}

AmbientLight::AmbientLight(float ambient, const Color& color)
    : _ambient(ambient), _color(color) {}

Vector3D AmbientLight::getDirectionFrom(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Ambient light comes from all directions, but let's use a default
  return Vector3D(0, 0, 1);
}

double AmbientLight::getDistanceFrom(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Ambient light has no distance
  return 0.0;
}

double AmbientLight::getIntensityAt(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Return the ambient intensity
  return _ambient;
}

Color AmbientLight::getColor() const {
  return _color;
}

bool AmbientLight::castsShadows() const {
  // Ambient light doesn't cast shadows
  return false;
}

Ray AmbientLight::getShadowRay(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Ambient light doesn't produce shadow rays, but need to implement
  return Ray(point, Vector3D(0, 0, 1));
}

std::shared_ptr<ILight> AmbientLight::clone() const {
  auto clone = std::make_shared<AmbientLight>(_ambient);
  clone->_color = _color;
  return clone;
}

Vector3D AmbientLight::getPosition() const {
  // Ambient light doesn't have a position
  return Vector3D(0, 0, 0);
}

float AmbientLight::getAmbient() const {
  return _ambient;
}

std::string AmbientLight::toString() const {
  return "AmbientLight with intensity: " + std::to_string(_ambient);
}

}  // namespace RayTracer
