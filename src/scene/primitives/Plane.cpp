/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane primitive implementation
*/

#include "Plane.hpp"
#include <cmath>
#include <iostream>
#include <limits>

namespace RayTracer {

Plane::Plane(const Vector3D& point, const Vector3D& normal, const Color& color)
    : _point(point), _normal(normal.normalized()), _color(color) {
  _transform = Transform();
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& e) {
    std::cerr << "Error initializing inverse transform for Plane: " << e.what()
              << std::endl;
    _inverseTransform = Transform();
  }
}

std::optional<Intersection> Plane::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);

  double denominator = localRay.getDirection().dot(_normal);

  if (std::abs(denominator) < EPSILON) {
    return std::nullopt;
  }

  Vector3D originToPoint = _point - localRay.getOrigin();
  double t = originToPoint.dot(_normal) / denominator;

  if (t < EPSILON) {
    return std::nullopt;
  }

  Vector3D localIntersectionPoint = localRay.pointAt(t);

  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);
  Vector3D worldNormal = _transform.applyToNormal(_normal).normalized();

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

void Plane::setTransform(const Transform& transform) {
  _transform = transform;
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& e) {
    _inverseTransform = Transform();
    std::cerr << "Warning: Plane transform is non-invertible: " << e.what()
              << ". Using identity inverse." << std::endl;
  }
}

Transform Plane::getTransform() const {
  return _transform;
}

void Plane::setColor(const Color& color) {
  _color = color;
}

Color Plane::getColor() const {
  return _color;
}

Vector3D Plane::getNormalAt(const Vector3D& point) const {
  (void)point;
  Vector3D worldNormal = _transform.applyToNormal(_normal).normalized();
  return worldNormal;
}

std::shared_ptr<IPrimitive> Plane::clone() const {
  auto clonedPlane = std::make_shared<Plane>(_point, _normal, _color);
  clonedPlane->setTransform(_transform);
  return clonedPlane;
}

}  // namespace RayTracer
