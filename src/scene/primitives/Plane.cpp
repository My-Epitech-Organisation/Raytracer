/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane primitive implementation
*/

#include "Plane.hpp"
#include <cmath>
#include <iostream>
#include "../../../include/exceptions/RaytracerException.hpp"

namespace RayTracer {

Plane::Plane(char axis, double position, const Color& color)
    : _position(position),
      _color(color),
      _transform() {
  _axis = getAxisFromChar(axis);
  if (_axis == Axis::X) {
    _normal = Vector3D(1, 0, 0);
  } else if (_axis == Axis::Y) {
    _normal = Vector3D(0, 1, 0);
  } else {
    _normal = Vector3D(0, 0, 1);
  }

  try {
    _inverseTransform = _transform.inverse();
  } catch (const RaytracerException& e) {
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

  Vector3D planePointOnAxis;
  if (_axis == Axis::X) {
    planePointOnAxis = Vector3D(_position, 0, 0);
  } else if (_axis == Axis::Y) {
    planePointOnAxis = Vector3D(0, _position, 0);
  } else {
    planePointOnAxis = Vector3D(0, 0, _position);
  }

  Vector3D originToPoint = planePointOnAxis - localRay.getOrigin();
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
  char axisEnum = getCharFromAxis(_axis);
  auto clonedPlane = std::make_shared<Plane>(axisEnum, _position, _color);
  clonedPlane->setTransform(_transform);
  return clonedPlane;
}

Axis Plane::getAxisFromChar(char axis) const {
  if (axis == 'X' || axis == 'x') {
    return Axis::X;
  } else if (axis == 'Y' || axis == 'y') {
    return Axis::Y;
  } else if (axis == 'Z' || axis == 'z') {
    return Axis::Z;
  } else {
    throw std::invalid_argument("Invalid axis character");
  }
}

char Plane::getCharFromAxis(Axis axis) const {
  switch (axis) {
    case Axis::X:
      return 'X';
    case Axis::Y:
      return 'Y';
    case Axis::Z:
      return 'Z';
    default:
      throw std::invalid_argument("Invalid axis enum");
  }
}

double Plane::getPosition() const {
  return _position;
}

Vector3D Plane::getNormal() const {
  return _normal;
}

Axis Plane::getAxis() const {
  return _axis;
}

}  // namespace RayTracer
