/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Checkerboard plane primitive implementation
*/

/**
 * @file CheckerboardPlane.cpp
 * @brief Implementation of the checkerboard plane primitive
 * @author GitHub Copilot
 * @date 2025-05-18
 * @version 1.0
 */

#include "CheckerboardPlane.hpp"
#include <cmath>
#include <iostream>
#include "../../../include/exceptions/RaytracerException.hpp"

namespace RayTracer {

CheckerboardPlane::CheckerboardPlane(char axis, double position, const Color& color,
                             const Color& alternateColor, double squareSize)
    : Plane(axis, position, color),  // Call the parent constructor
      _alternateColor(alternateColor),
      _squareSize(squareSize) {
  // All initialization is done in the initializer list
}

std::optional<Intersection> CheckerboardPlane::intersect(const Ray& ray) const {
  // Get the basic intersection using the parent Plane class
  auto intersection = Plane::intersect(ray);
  
  if (!intersection) {
    return std::nullopt;
  }
  
  // Get the local intersection point to determine the color
  Ray localRay = ray.transform(getTransform().inverse());
  Vector3D localIntersectionPoint = localRay.pointAt(
      (intersection->point - ray.getOrigin()).getMagnitude() / 
      ray.getDirection().getMagnitude());
  
  // Determine which color to use based on the checkerboard pattern
  intersection->color = getColorAtPoint(localIntersectionPoint);
  
  return intersection;
}

Color CheckerboardPlane::getColorAtPoint(const Vector3D& point) const {
  // Determine the checkerboard pattern based on the point coordinates
  // We need to choose which coordinates to check based on the plane's axis
  double u, v;
  Axis axis = getAxis();
  
  // Pick the appropriate coordinates to check based on the plane's axis
  if (axis == Axis::X) {
    u = point.getY();
    v = point.getZ();
  } else if (axis == Axis::Y) {
    u = point.getX();
    v = point.getZ();
  } else { // axis == Axis::Z
    u = point.getX();
    v = point.getY();
  }
  
  // Determine if we're in an "even" or "odd" square
  // Divide by square size and see if the integer part is even or odd
  int uCheckIndex = static_cast<int>(std::floor(u / _squareSize));
  int vCheckIndex = static_cast<int>(std::floor(v / _squareSize));
  
  // If the sum is even, use the primary color, otherwise use the alternate color
  if ((uCheckIndex + vCheckIndex) % 2 == 0) {
    return getColor();
  } else {
    return _alternateColor;
  }
}

std::shared_ptr<IPrimitive> CheckerboardPlane::clone() const {
  char axisChar = getCharFromAxis(getAxis());
  auto clonedPlane = std::make_shared<CheckerboardPlane>(
      axisChar, getPosition(), getColor(), _alternateColor, _squareSize);
  clonedPlane->setTransform(getTransform());
  return clonedPlane;
}

Color CheckerboardPlane::getAlternateColor() const {
  return _alternateColor;
}

void CheckerboardPlane::setAlternateColor(const Color& color) {
  _alternateColor = color;
}

double CheckerboardPlane::getSquareSize() const {
  return _squareSize;
}

void CheckerboardPlane::setSquareSize(double size) {
  _squareSize = size;
}

}  // namespace RayTracer
