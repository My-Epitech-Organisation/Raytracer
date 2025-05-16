/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane primitive implementation
*/

/**
 * @file Plane.cpp
 * @brief Implementation of the infinite plane primitive with checkerboard
 * pattern support
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "Plane.hpp"
#include <cmath>
#include <iostream>
#include "../../../include/exceptions/RaytracerException.hpp"

namespace RayTracer {

Plane::Plane(char axis, double position, const Color& color,
             const Color& alternateColor, double checkSize)
    : _position(position),
      _color(color),
      _transform(),
      _alternateColor(alternateColor),
      _checkSize(checkSize) {
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

  if (isWhiteSquare(localIntersectionPoint)) {
    intersection.color = _color;
  } else {
    intersection.color = _alternateColor;
  }

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
  auto clonedPlane = std::make_shared<Plane>(axisEnum, _position, _color,
                                             _alternateColor, _checkSize);
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

void Plane::setAlternateColor(const Color& color) {
  _alternateColor = color;
}

Color Plane::getAlternateColor() const {
  return _alternateColor;
}

void Plane::setCheckSize(double size) {
  if (size <= 0.0) {
    throw std::invalid_argument("Checkerboard square size must be positive");
  }
  _checkSize = size;
}

double Plane::getCheckSize() const {
  return _checkSize;
}

bool Plane::isWhiteSquare(const Vector3D& point) const {
  double u, v;

  if (_axis == Axis::X) {
    u = point.getY();
    v = point.getZ();
  } else if (_axis == Axis::Y) {
    u = point.getX();
    v = point.getZ();
  } else {  // Axis::Z
    u = point.getX();
    v = point.getY();
  }

  int ui = static_cast<int>(std::floor(u / _checkSize));
  int vi = static_cast<int>(std::floor(v / _checkSize));

  return (ui + vi) % 2 == 0;
}

}  // namespace RayTracer
