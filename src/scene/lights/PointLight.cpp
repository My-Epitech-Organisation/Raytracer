/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight
*/

/**
 * @file PointLight.cpp
 * @brief Implementation of the point light class which simulates light
 * emanating from a specific position in 3D space
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "PointLight.hpp"
#include <cmath>
#include "../../core/Vector3D.hpp"

namespace RayTracer {

PointLight::PointLight(const Vector3D& position) : _position(position) {}

PointLight::PointLight(const Vector3D& position, const Color& color)
    : _position(position), _color(color) {}

Vector3D PointLight::getDirectionFrom(const Vector3D& point) const {
  Vector3D direction = _position - point;
  return direction.normalized();
}

double PointLight::getDistanceFrom(const Vector3D& point) const {
  return (_position - point).getMagnitude();
}

double PointLight::getIntensityAt(const Vector3D& point) const {
  double distance = getDistanceFrom(point);

  // Constant, linear and quadratic (Phong attenuation model)
  const double kConstant = 1.0;
  const double kLinear = 0.007;
  const double kQuadratic = 0.0008;

  double intensity =
      2.5 / (kConstant + kLinear * distance + kQuadratic * distance * distance);

  return std::min(intensity, 3.0);
}

Color PointLight::getColor() const {
  return _color;
}

bool PointLight::castsShadows() const {
  return true;
}

Ray PointLight::getShadowRay(const Vector3D& point) const {
  Vector3D direction = getDirectionFrom(point);
  // Small epsilon to avoid self-shadowing
  return Ray(point + direction * 0.001, direction);
}

std::shared_ptr<ILight> PointLight::clone() const {
  auto clone = std::make_shared<PointLight>(_position);
  clone->_color = _color;
  return clone;
}

Vector3D PointLight::getPosition() const {
  return _position;
}

std::string PointLight::toString() const {
  return "PointLight at (" + std::to_string(_position.getX()) + ", " +
         std::to_string(_position.getY()) + ", " +
         std::to_string(_position.getZ()) + ")";
}

}  // namespace RayTracer
