/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight
*/

#include "DirectionalLight.hpp"
#include <cmath>
#include <limits>

namespace RayTracer {

DirectionalLight::DirectionalLight(const Vector3D& direction)
    : _direction(direction) {}

DirectionalLight::DirectionalLight(const Vector3D& direction,
                                   const Color& color)
    : _direction(direction), _color(color) {}

Vector3D DirectionalLight::getDirectionFrom(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Directional light has the same direction regardless of point
  return -_direction;  // Negative because we want direction FROM point TO light
}

double DirectionalLight::getDistanceFrom(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Directional lights are infinitely far away
  return std::numeric_limits<double>::infinity();
}

double DirectionalLight::getIntensityAt(const Vector3D& point) const {
  (void)point;  // Silence unused parameter warning
  // Directional lights have constant intensity
  return 1.0;
}

Color DirectionalLight::getColor() const {
  return _color;
}

bool DirectionalLight::castsShadows() const {
  return true;
}

Ray DirectionalLight::getShadowRay(const Vector3D& point) const {
  Vector3D direction = -_direction;  // Direction from point to light
  // Small epsilon to avoid self-shadowing
  return Ray(point + direction * 0.001, direction);
}

std::shared_ptr<ILight> DirectionalLight::clone() const {
  auto clone = std::make_shared<DirectionalLight>(_direction);
  clone->_color = _color;
  return clone;
}

Vector3D DirectionalLight::getPosition() const {
  // Directional lights don't have a position, return a position infinitely far
  // in the direction
  return Vector3D(std::numeric_limits<double>::infinity() * _direction.getX(),
                  std::numeric_limits<double>::infinity() * _direction.getY(),
                  std::numeric_limits<double>::infinity() * _direction.getZ());
}

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
