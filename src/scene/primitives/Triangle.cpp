/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#include "Triangle.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace RayTracer {

Triangle::Triangle(const Vector3D& a, const Vector3D& b, const Vector3D& c,
                   const Color& color)
    : _a(a), _b(b), _c(c), _color(color), _transform() {
  _normal = (b - a).cross(c - a).normalized();
  std::cerr << "[Triangle] Normale calculée : (" << _normal.getX() << ", "
            << _normal.getY() << ", " << _normal.getZ() << ")" << std::endl;
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error&) {
    _inverseTransform = Transform();
  }
}

void Triangle::setTransform(const Transform& transform) {
  _transform = transform;
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error&) {
    _inverseTransform = Transform();
  }
}

Transform Triangle::getTransform() const {
  return _transform;
}

void Triangle::setColor(const Color& color) {
  _color = color;
}

Color Triangle::getColor() const {
  return _color;
}

std::shared_ptr<IPrimitive> Triangle::clone() const {
  return std::make_shared<Triangle>(*this);
}

Vector3D Triangle::getA() const {
  return _a;
}
Vector3D Triangle::getB() const {
  return _b;
}
Vector3D Triangle::getC() const {
  return _c;
}

std::optional<Intersection> Triangle::intersect(const Ray& ray) const {
  // Möller–Trumbore intersection algorithm
  Ray localRay = ray.transform(_inverseTransform);
  const Vector3D& orig = localRay.getOrigin();
  const Vector3D& dir = localRay.getDirection();
  Vector3D edge1 = _b - _a;
  Vector3D edge2 = _c - _a;
  Vector3D h = dir.cross(edge2);
  double a = edge1.dot(h);
  if (std::abs(a) < 1e-8)
    return std::nullopt;  // Ray is parallel
  double f = 1.0 / a;
  Vector3D s = orig - _a;
  double u = f * s.dot(h);
  if (u < 0.0 || u > 1.0)
    return std::nullopt;
  Vector3D q = s.cross(edge1);
  double v = f * dir.dot(q);
  if (v < 0.0 || u + v > 1.0)
    return std::nullopt;
  double t = f * edge2.dot(q);
  if (t < 1e-4)
    return std::nullopt;
  Vector3D localIntersection = orig + dir * t;
  Vector3D worldIntersection = _transform.applyToPoint(localIntersection);
  Vector3D worldNormal = _transform.applyToNormal(_normal).normalized();
  if (worldNormal.dot(ray.getDirection()) > 0)
    worldNormal = -worldNormal;
  Intersection intersection;
  intersection.distance = (worldIntersection - ray.getOrigin()).getMagnitude();
  intersection.point = worldIntersection;
  intersection.normal = worldNormal;
  intersection.color = _color;
  intersection.primitive = this;
  return intersection;
}

Vector3D Triangle::getNormalAt(const Vector3D&) const {
  return _transform.applyToNormal(_normal).normalized();
}

}  // namespace RayTracer
