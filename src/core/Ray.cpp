/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray class implementation
*/

/**
 * @file Ray.cpp
 * @brief Implementation of the Ray class which represents a ray with origin and
 * direction for ray tracing
 * @author @paul-antoine
 * @date 2025-05-16
 * @version 1.0
 */

#include "Ray.hpp"
#include "Transform.hpp"
#include "Vector3D.hpp"

namespace RayTracer {

Ray::Ray() : _origin(0, 0, 0), _direction(0, 0, 1) {}

Ray::Ray(const Vector3D& origin, const Vector3D& direction)
    : _origin(origin), _direction(direction.normalized()) {}

Ray::Ray(const Ray& other)
    : _origin(other._origin), _direction(other._direction) {}

Ray& Ray::operator=(const Ray& other) {
  if (this != &other) {
    _origin = other._origin;
    _direction = other._direction;
  }
  return *this;
}

Ray::~Ray() {}

Vector3D Ray::getOrigin() const {
  return _origin;
}

Vector3D Ray::getDirection() const {
  return _direction;
}

void Ray::setOrigin(const Vector3D& origin) {
  _origin = origin;
}

void Ray::setDirection(const Vector3D& direction) {
  _direction = direction.normalized();
}

Vector3D Ray::pointAt(double t) const {
  return _origin + (_direction * t);
}

Ray Ray::transform(const Transform& transform) const {
  Vector3D newOrigin = transform.applyToPoint(_origin);
  Vector3D newDirection = transform.applyToVector(_direction);

  return Ray(newOrigin, newDirection);
}

}  // namespace RayTracer
