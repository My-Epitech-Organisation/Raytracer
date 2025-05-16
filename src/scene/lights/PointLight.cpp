/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight
*/

#include "PointLight.hpp"
#include <cmath>
#include "../../core/Vector3D.hpp"

namespace RayTracer {

PointLight::PointLight(const Vector3D& position) : _position(position) {}

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
  const double kLinear = 0.09;
  const double kQuadratic = 0.032;

  return 1.0 /
         (kConstant + kLinear * distance + kQuadratic * distance * distance);
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
  return std::make_shared<PointLight>(_position);
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
