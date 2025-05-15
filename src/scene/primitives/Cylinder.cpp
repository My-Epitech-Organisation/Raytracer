/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder
*/

#include "Cylinder.hpp"
#include <cmath>
#include <limits>

namespace RayTracer {

Cylinder::Cylinder(double radius, const Color& color)
    : _radius(radius), _color(color), _transform() {
  if (radius <= 0) {
    throw std::invalid_argument("Cylinder radius must be positive");
  }
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& /*e*/) {
    _inverseTransform = Transform();
  }
}

void Cylinder::setTransform(const Transform& transform) {
  _transform = transform;
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& /*e*/) {
    _inverseTransform = Transform();
  }
}

Transform Cylinder::getTransform() const {
  return _transform;
}

void Cylinder::setColor(const Color& color) {
  _color = color;
}

Color Cylinder::getColor() const {
  return _color;
}

Vector3D Cylinder::getNormalAt(const Vector3D& point) const {
  Vector3D localPoint = _inverseTransform.applyToPoint(point);
  Vector3D localNormal(localPoint.getX(), 0, localPoint.getZ());
  return _transform.applyToNormal(localNormal).normalized();
}

std::shared_ptr<IPrimitive> Cylinder::clone() const {
  return std::make_shared<Cylinder>(*this);
}

Vector3D Cylinder::getBaseCenter() const {
  // For infinite cylinder, base is arbitrary: use (0,0,0) transformed
  return _transform.applyToPoint(Vector3D(0, 0, 0));
}

Vector3D Cylinder::getAxis() const {
  // Axis is Y in local, transformed to world
  return _transform.applyToNormal(Vector3D(0, 1, 0)).normalized();
}

double Cylinder::getRadius() const {
  return _radius;
}

double Cylinder::getHeight() const {
  // Infinite height
  return std::numeric_limits<double>::infinity();
}

std::optional<Intersection> Cylinder::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);
  Vector3D O = localRay.getOrigin();
  Vector3D D = localRay.getDirection();

  double a = D.getX() * D.getX() + D.getZ() * D.getZ();
  double b = 2.0 * (O.getX() * D.getX() + O.getZ() * D.getZ());
  double c = O.getX() * O.getX() + O.getZ() * O.getZ() - _radius * _radius;

  if (std::abs(a) < CYLINDER_EPSILON) {
    return std::nullopt;
  }

  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return std::nullopt;
  }

  double sqrt_discriminant = std::sqrt(discriminant);
  double t0 = (-b - sqrt_discriminant) / (2 * a);
  double t1 = (-b + sqrt_discriminant) / (2 * a);

  double t = std::numeric_limits<double>::infinity();
  if (t0 > CYLINDER_EPSILON && t1 > CYLINDER_EPSILON) {
    t = std::min(t0, t1);
  } else if (t0 > CYLINDER_EPSILON) {
    t = t0;
  } else if (t1 > CYLINDER_EPSILON) {
    t = t1;
  } else {
    return std::nullopt;
  }

  Vector3D localIntersectionPoint = localRay.pointAt(t);
  Vector3D localNormal(localIntersectionPoint.getX(), 0,
                       localIntersectionPoint.getZ());
  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);
  Vector3D worldNormal = _transform.applyToNormal(localNormal).normalized();

  if (worldNormal.dot(ray.getDirection()) > 0) {
    worldNormal = -worldNormal;
  }

  Intersection intersection;
  intersection.distance =
      (worldIntersectionPoint - ray.getOrigin()).getMagnitude();
  intersection.point = worldIntersectionPoint;
  intersection.normal = worldNormal;
  intersection.color = _color;
  intersection.primitive = this;
  return intersection;
}

}  // namespace RayTracer
