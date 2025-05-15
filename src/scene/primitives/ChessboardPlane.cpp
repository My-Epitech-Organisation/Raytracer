/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Chessboard plane primitive implementation
*/

#include "ChessboardPlane.hpp"
#include <cmath>
#include <iostream>
#include "../../../include/exceptions/RaytracerException.hpp"

namespace RayTracer {

ChessboardPlane::ChessboardPlane(char axis, double position, const Color& color,
                                 const Color& alternateColor, double checkSize)
    : Plane(axis, position, color),
      _alternateColor(alternateColor),
      _checkSize(checkSize) {
  if (checkSize <= 0.0) {
    throw std::invalid_argument("Checkerboard square size must be positive");
  }
}

std::optional<Intersection> ChessboardPlane::intersect(const Ray& ray) const {
  // First get the basic plane intersection
  auto intersection = Plane::intersect(ray);

  if (!intersection.has_value()) {
    return std::nullopt;
  }

  // Transform the intersection point back to local space for pattern calculation
  Ray localRay = ray.transform(getTransform().inverse());
  double t = (intersection->distance) / localRay.getDirection().getMagnitude();
  Vector3D localIntersectionPoint = localRay.pointAt(t);

  // Apply the checkerboard pattern
  if (isWhiteSquare(localIntersectionPoint)) {
    intersection->color = getColor();
  } else {
    intersection->color = _alternateColor;
  }

  return intersection;
}

std::shared_ptr<IPrimitive> ChessboardPlane::clone() const {
  char axisChar = 'Z';
  Axis axis = getAxis();

  if (axis == Axis::X) axisChar = 'X';
  else if (axis == Axis::Y) axisChar = 'Y';

  auto cloned = std::make_shared<ChessboardPlane>(
      axisChar, getPosition(), getColor(), _alternateColor, _checkSize);
  cloned->setTransform(getTransform());
  return cloned;
}

void ChessboardPlane::setAlternateColor(const Color& color) {
  _alternateColor = color;
}

Color ChessboardPlane::getAlternateColor() const {
  return _alternateColor;
}

void ChessboardPlane::setCheckSize(double size) {
  if (size <= 0.0) {
    throw std::invalid_argument("Checkerboard square size must be positive");
  }
  _checkSize = size;
}

double ChessboardPlane::getCheckSize() const {
  return _checkSize;
}

bool ChessboardPlane::isWhiteSquare(const Vector3D& point) const {
  double u, v;
  Axis axis = getAxis();

  if (axis == Axis::X) {
    u = point.getY();
    v = point.getZ();
  } else if (axis == Axis::Y) {
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
