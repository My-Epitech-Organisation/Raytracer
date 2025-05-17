/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere primitive class implementation
*/

/**
 * @file Sphere.cpp
 * @brief Implementation of the sphere primitive with ray-sphere intersection
 * calculation
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "Sphere.hpp"
#include <cmath>
#include "../../../include/exceptions/InvalidTypeException.hpp"

namespace RayTracer {

Sphere::Sphere()
    : _center(0, 0, 0), _radius(1.0), _color(Color::RED), _transform() {}

Sphere::Sphere(const Vector3D& center, double radius, const Color& color)
    : _center(center), _radius(radius), _color(color), _transform() {
  if (radius <= 0) {
    throw InvalidTypeException("Sphere radius must be positive");
  }
}

Sphere::Sphere(const Sphere& other)
    : _center(other._center),
      _radius(other._radius),
      _color(other._color),
      _transform(other._transform) {}

Sphere& Sphere::operator=(const Sphere& other) {
  if (this != &other) {
    _center = other._center;
    _radius = other._radius;
    _color = other._color;
    _transform = other._transform;
  }
  return *this;
}

Sphere::~Sphere() {}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_transform.inverse());

  Vector3D oc = localRay.getOrigin() - _center;

  double a = localRay.getDirection().dot(localRay.getDirection());
  double b = 2.0 * oc.dot(localRay.getDirection());
  double c = oc.dot(oc) - _radius * _radius;

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return std::nullopt;
  }

  double sqrtDiscriminant = std::sqrt(discriminant);
  double t1 = (-b - sqrtDiscriminant) / (2 * a);
  double t2 = (-b + sqrtDiscriminant) / (2 * a);

  double t = t1;
  if (t < 0) {
    t = t2;
    if (t < 0) {
      return std::nullopt;
    }
  }

  Vector3D localIntersectionPoint = localRay.pointAt(t);

  Vector3D localNormal = (localIntersectionPoint - _center).normalized();

  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);

  double worldDistance =
      (worldIntersectionPoint - ray.getOrigin()).getMagnitude();

  Vector3D worldNormal = _transform.applyToNormal(localNormal).normalized();

  if (worldNormal.dot(ray.getDirection()) > 0) {
    worldNormal = -worldNormal;
  }

  Intersection intersection;
  intersection.distance = worldDistance;
  intersection.point = worldIntersectionPoint;
  intersection.normal = worldNormal;
  intersection.color = _color;
  intersection.primitive = this;

  return intersection;
}

void Sphere::setTransform(const Transform& transform) {
  _transform = transform;
}

Transform Sphere::getTransform() const {
  return _transform;
}

void Sphere::setColor(const Color& color) {
  _color = color;
}

Color Sphere::getColor() const {
  return _color;
}

Vector3D Sphere::getNormalAt(const Vector3D& point) const {
  Vector3D localPoint = _transform.inverse().applyToPoint(point);

  Vector3D localNormal = (localPoint - _center).normalized();

  // Use the new applyToNormal method which properly handles non-uniform scaling
  Vector3D worldNormal = _transform.applyToNormal(localNormal).normalized();

  return worldNormal;
}

std::shared_ptr<IPrimitive> Sphere::clone() const {
  return std::make_shared<Sphere>(*this);
}

void Sphere::setCenter(const Vector3D& center) {
  _center = center;
}

Vector3D Sphere::getCenter() const {
  return _center;
}

void Sphere::setRadius(double radius) {
  if (radius <= 0) {
    throw InvalidTypeException("Sphere radius must be positive");
  }
  _radius = radius;
}

double Sphere::getRadius() const {
  return _radius;
}

}  // namespace RayTracer
