/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LimitedCylinder
*/

/**
 * @file LimitedCylinder.cpp
 * @brief Implementation of the LimitedCylinder class which provides limited
 * cylinder primitive functionality for ray tracing
 * @author @mael
 * @date 2025-05-16
 * @version 1.0
 */

#include "LimitedCylinder.hpp"
#include <cmath>
#include <limits>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"
#include "ICylinder.hpp"

namespace RayTracer {

LimitedCylinder::LimitedCylinder(double radius, double height,
                                 const Color& color)
    : _radius(radius), _height(height), _color(color), _transform() {
  if (radius <= 0) {
    throw std::invalid_argument("Cylinder radius must be positive");
  }
  if (height <= 0) {
    throw std::invalid_argument("Cylinder height must be positive");
  }
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& /*e*/) {
    _inverseTransform = Transform();
  }
}

void LimitedCylinder::setTransform(const Transform& transform) {
  _transform = transform;
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& /*e*/) {
    _inverseTransform = Transform();
  }
}

Transform LimitedCylinder::getTransform() const {
  return _transform;
}

void LimitedCylinder::setColor(const Color& color) {
  _color = color;
}

Color LimitedCylinder::getColor() const {
  return _color;
}

Vector3D LimitedCylinder::getNormalAt(const Vector3D& point) const {
  Vector3D localPoint = _inverseTransform.applyToPoint(point);
  double halfHeight = _height / 2.0;

  if (std::abs(localPoint.getY() - halfHeight) < CYLINDER_EPSILON) {
    return _transform.applyToNormal(Vector3D(0, 1, 0)).normalized();
  }
  if (std::abs(localPoint.getY() + halfHeight) < CYLINDER_EPSILON) {
    return _transform.applyToNormal(Vector3D(0, -1, 0)).normalized();
  }

  Vector3D localNormal(localPoint.getX(), 0, localPoint.getZ());
  return _transform.applyToNormal(localNormal).normalized();
}

std::shared_ptr<IPrimitive> LimitedCylinder::clone() const {
  return std::make_shared<LimitedCylinder>(*this);
}

double LimitedCylinder::getRadius() const {
  return _radius;
}

double LimitedCylinder::getHeight() const {
  return _height;
}

Vector3D LimitedCylinder::getBaseCenter() const {
  // Base center is at (0, -height/2, 0) in local, transformed to world
  return _transform.applyToPoint(Vector3D(0, -_height / 2.0, 0));
}

Vector3D LimitedCylinder::getAxis() const {
  // Axis is Y in local, transformed to world
  return _transform.applyToNormal(Vector3D(0, 1, 0)).normalized();
}

std::optional<Intersection> LimitedCylinder::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);
  double t_min_overall = std::numeric_limits<double>::infinity();
  std::optional<Intersection> closest_intersection = std::nullopt;

  auto body_intersect_result = intersectBody(localRay, t_min_overall);
  if (body_intersect_result) {
    closest_intersection = body_intersect_result;
  }

  auto cap_intersect_result = intersectCaps(localRay, t_min_overall);
  if (cap_intersect_result) {
    closest_intersection = cap_intersect_result;
  }

  if (closest_intersection) {
    Vector3D worldIntersectionPoint =
        _transform.applyToPoint(closest_intersection->point);
    Vector3D worldNormal =
        _transform.applyToNormal(closest_intersection->normal).normalized();

    if (worldNormal.dot(ray.getDirection()) > 0) {
      worldNormal = -worldNormal;
    }

    closest_intersection->point = worldIntersectionPoint;
    closest_intersection->normal = worldNormal;
    closest_intersection->distance =
        (worldIntersectionPoint - ray.getOrigin()).getMagnitude();
    closest_intersection->color = _color;
    closest_intersection->primitive = this;
    return closest_intersection;
  }

  return std::nullopt;
}

std::optional<Intersection> LimitedCylinder::intersectCaps(
    const Ray& localRay, double& t_min_overall) const {
  std::optional<Intersection> closest_cap_intersection = std::nullopt;
  double halfHeight = _height / 2.0;

  auto top_cap_hit =
      checkCap(localRay, t_min_overall, halfHeight, Vector3D(0, 1, 0));
  if (top_cap_hit) {
    closest_cap_intersection = top_cap_hit;
  }

  auto bottom_cap_hit =
      checkCap(localRay, t_min_overall, -halfHeight, Vector3D(0, -1, 0));
  if (bottom_cap_hit) {
    closest_cap_intersection = bottom_cap_hit;
  }

  return closest_cap_intersection;
}

std::optional<Intersection> LimitedCylinder::checkCap(
    const Ray& localRay, double& t_min_overall, double cap_y_position,
    const Vector3D& cap_normal_param) const {
  double directionY = localRay.getDirection().getY();

  if (std::abs(directionY) < CYLINDER_EPSILON) {
    return std::nullopt;
  }

  double t = (cap_y_position - localRay.getOrigin().getY()) / directionY;

  if (t <= CYLINDER_EPSILON || t >= t_min_overall) {
    return std::nullopt;
  }

  Vector3D p = localRay.pointAt(t);

  if (p.getX() * p.getX() + p.getZ() * p.getZ() >
      _radius * _radius + CYLINDER_EPSILON) {
    return std::nullopt;
  }

  t_min_overall = t;
  Intersection intersection_data;
  intersection_data.distance = t;
  intersection_data.point = p;
  intersection_data.normal = cap_normal_param;
  return intersection_data;
}

std::optional<Intersection> LimitedCylinder::intersectBody(
    const Ray& localRay, double& t_min_overall) const {
  Vector3D O = localRay.getOrigin();
  Vector3D D = localRay.getDirection();

  double a = D.getX() * D.getX() + D.getZ() * D.getZ();
  double b = 2.0 * (O.getX() * D.getX() + O.getZ() * D.getZ());
  double c_quad = O.getX() * O.getX() + O.getZ() * O.getZ() - _radius * _radius;

  if (std::abs(a) < CYLINDER_EPSILON) {
    return std::nullopt;
  }

  double discriminant = b * b - 4 * a * c_quad;

  if (discriminant < 0) {
    return std::nullopt;
  }

  double sqrt_discriminant = std::sqrt(discriminant);
  double t0 = (-b - sqrt_discriminant) / (2 * a);
  double t1 = (-b + sqrt_discriminant) / (2 * a);

  std::optional<Intersection> body_intersection = std::nullopt;
  double halfHeight = _height / 2.0;

  if (t0 > CYLINDER_EPSILON && t0 < t_min_overall) {
    Vector3D p0 = localRay.pointAt(t0);
    if (p0.getY() >= -halfHeight - CYLINDER_EPSILON &&
        p0.getY() <= halfHeight + CYLINDER_EPSILON) {
      t_min_overall = t0;
      Intersection intersection_data;
      intersection_data.distance = t0;
      intersection_data.point = p0;
      intersection_data.normal = Vector3D(p0.getX(), 0, p0.getZ()).normalized();
      body_intersection = intersection_data;
    }
  }

  if (t1 > CYLINDER_EPSILON && t1 < t_min_overall) {
    Vector3D p1 = localRay.pointAt(t1);
    if (p1.getY() >= -halfHeight - CYLINDER_EPSILON &&
        p1.getY() <= halfHeight + CYLINDER_EPSILON) {
      t_min_overall = t1;
      Intersection intersection_data;
      intersection_data.distance = t1;
      intersection_data.point = p1;
      intersection_data.normal = Vector3D(p1.getX(), 0, p1.getZ()).normalized();
      body_intersection = intersection_data;
    }
  }

  return body_intersection;
}

}  // namespace RayTracer
