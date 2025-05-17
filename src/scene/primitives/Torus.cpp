/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus
*/

#include "Torus.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

namespace RayTracer {

Torus::Torus(double majorRadius, double tubeRadius, const Color& color)
    : _majorRadius(majorRadius),
      _tubeRadius(tubeRadius),
      _color(color),
      _transform() {
  if (majorRadius <= 0 || tubeRadius <= 0) {
    throw std::invalid_argument("Torus radii must be positive");
  }
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error&) {
    _inverseTransform = Transform();
  }
}

void Torus::setTransform(const Transform& transform) {
  _transform = transform;
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error&) {
    _inverseTransform = Transform();
  }
}

Transform Torus::getTransform() const {
  return _transform;
}

void Torus::setColor(const Color& color) {
  _color = color;
}

Color Torus::getColor() const {
  return _color;
}

std::shared_ptr<IPrimitive> Torus::clone() const {
  return std::make_shared<Torus>(*this);
}

double Torus::getMajorRadius() const {
  return _majorRadius;
}

double Torus::getTubeRadius() const {
  return _tubeRadius;
}

std::optional<Intersection> Torus::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);
  Vector3D O = localRay.getOrigin();
  Vector3D D = localRay.getDirection();
  double R = _majorRadius;
  double r = _tubeRadius;

  double ox = O.getX(), oy = O.getY(), oz = O.getZ();
  double dx = D.getX(), dy = D.getY(), dz = D.getZ();

  double sum_d_sq = dx * dx + dy * dy + dz * dz;
  double sum_o_sq = ox * ox + oy * oy + oz * oz;
  double k = sum_o_sq + R * R - r * r;

  double a4 = sum_d_sq * sum_d_sq;
  double a3 = 4.0 * sum_d_sq * (ox * dx + oy * dy + oz * dz);
  double a2 =
      2.0 * sum_d_sq * k +
      4.0 * (ox * dx + oy * dy + oz * dz) * (ox * dx + oy * dy + oz * dz) -
      4.0 * R * R * (dx * dx + dz * dz);
  double a1 = 4.0 * k * (ox * dx + oy * dy + oz * dz) -
              8.0 * R * R * (ox * dx + oz * dz);
  double a0 = k * k - 4.0 * R * R * (ox * ox + oz * oz);

  // optimisation to find roots
  std::vector<std::pair<double, double>> intervals;
  double t_prev = 1e-4;
  double val_prev = a4 * t_prev * t_prev * t_prev * t_prev +
                    a3 * t_prev * t_prev * t_prev + a2 * t_prev * t_prev +
                    a1 * t_prev + a0;
  for (double t = 1e-2; t < 100.0; t += 1e-2) {
    double val = a4 * t * t * t * t + a3 * t * t * t + a2 * t * t + a1 * t + a0;
    if (val_prev * val < 0) {
      intervals.emplace_back(t_prev, t);
    }
    t_prev = t;
    val_prev = val;
  }

  auto quartic = [&](double t) {
    return a4 * t * t * t * t + a3 * t * t * t + a2 * t * t + a1 * t + a0;
  };
  auto quartic_deriv = [&](double t) {
    return 4 * a4 * t * t * t + 3 * a3 * t * t + 2 * a2 * t + a1;
  };

  double t_min = std::numeric_limits<double>::infinity();
  for (const auto& interval : intervals) {
    double t = (interval.first + interval.second) / 2.0;
    for (int i = 0; i < 10; ++i) {
      double f = quartic(t);
      double df = quartic_deriv(t);
      if (std::abs(df) < 1e-8)
        break;
      t = t - f / df;
      if (t < 0)
        break;
    }
    if (t > 1e-4 && t < t_min && std::abs(quartic(t)) < 1e-3) {
      t_min = t;
    }
  }
  if (t_min == std::numeric_limits<double>::infinity())
    return std::nullopt;

  Vector3D localIntersectionPoint = localRay.pointAt(t_min);
  double sumSquared =
      localIntersectionPoint.getX() * localIntersectionPoint.getX() +
      localIntersectionPoint.getZ() * localIntersectionPoint.getZ();
  double major = _majorRadius;
  Vector3D centerToTube(
      localIntersectionPoint.getX() * (1 - major / std::sqrt(sumSquared)), 0,
      localIntersectionPoint.getZ() * (1 - major / std::sqrt(sumSquared)));
  Vector3D localNormal = localIntersectionPoint - centerToTube;
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

Vector3D Torus::getNormalAt(const Vector3D& point) const {
  Vector3D localPoint = _inverseTransform.applyToPoint(point);
  double sumSquared = localPoint.getX() * localPoint.getX() +
                      localPoint.getZ() * localPoint.getZ();
  double major = _majorRadius;
  double distToAxis = std::sqrt(sumSquared);
  Vector3D centerToTube(localPoint.getX() * (1 - major / distToAxis), 0,
                        localPoint.getZ() * (1 - major / distToAxis));
  Vector3D localNormal = localPoint - centerToTube;
  if (distToAxis < major) {
    localNormal = -localNormal;
  }
  Vector3D worldNormal = _transform.applyToNormal(localNormal).normalized();
  return worldNormal;
}

}  // namespace RayTracer
