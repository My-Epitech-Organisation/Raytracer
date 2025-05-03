/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere primitive class implementation
*/

#include "Sphere.hpp"
#include <cmath>
#include <stdexcept>

namespace RayTracer {

Sphere::Sphere()
    : _center(0, 0, 0), _radius(1.0), _color(Color::RED), _transform() {}

Sphere::Sphere(const Vector3D& center, double radius, const Color& color)
    : _center(center), _radius(radius), _color(color), _transform() {
  if (radius <= 0) {
    throw std::invalid_argument("Sphere radius must be positive");
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

Sphere::~Sphere() {
  // Nothing to clean up
}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const {
  // Transform the ray to the sphere's local space
  Ray localRay = ray.transform(_transform.inverse());

  // Vector from ray origin to sphere center
  Vector3D oc = localRay.getOrigin() - _center;

  // Coefficients for the quadratic equation
  double a = localRay.getDirection().dot(
      localRay.getDirection());  // Always 1 for normalized directions
  double b = 2.0 * oc.dot(localRay.getDirection());
  double c = oc.dot(oc) - _radius * _radius;

  // Calculate the discriminant
  double discriminant = b * b - 4 * a * c;

  // If discriminant is negative, there's no intersection
  if (discriminant < 0) {
    return std::nullopt;
  }

  // Calculate the two intersections (t values)
  double sqrtDiscriminant = std::sqrt(discriminant);
  double t1 = (-b - sqrtDiscriminant) / (2 * a);
  double t2 = (-b + sqrtDiscriminant) / (2 * a);

  // Find the closest intersection in front of the ray
  double t = t1;
  if (t < 0) {
    t = t2;  // If t1 is behind the ray, try t2
    if (t < 0) {
      return std::nullopt;  // Both intersections are behind the ray
    }
  }

  // Calculate the intersection point in local space
  Vector3D localIntersectionPoint = localRay.pointAt(t);

  // Calculate the normal in local space
  Vector3D localNormal = (localIntersectionPoint - _center).normalized();

  // Transform the intersection point and normal to world space
  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);

  // Pour la normale, nous devons calculer la distance du point d'intersection
  // à la caméra dans l'espace monde car la paramètre t est basé sur le rayon local
  double worldDistance = (worldIntersectionPoint - ray.getOrigin()).getMagnitude();

  Vector3D worldNormal = _transform.applyToVector(localNormal).normalized();

  // Créer et retourner les données d'intersection
  Intersection intersection;
  intersection.distance = worldDistance;  // Utiliser la distance monde
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
  // Transform the world point to local space
  Vector3D localPoint = _transform.inverse().applyToPoint(point);

  // Calculate the normal in local space (direction from center to point)
  Vector3D localNormal = (localPoint - _center).normalized();

  // Transform the normal back to world space
  Vector3D worldNormal = _transform.applyToVector(localNormal).normalized();

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
    throw std::invalid_argument("Sphere radius must be positive");
  }
  _radius = radius;
}

double Sphere::getRadius() const {
  return _radius;
}

}  // namespace RayTracer
