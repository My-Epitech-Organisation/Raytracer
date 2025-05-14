/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone
*/

#include "Cone.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace RayTracer {

constexpr double CONE_EPSILON = 1e-6;

Cone::Cone(const Vector3D& apex, const Vector3D& axis, double angleDegrees,
           const Color& color)
    : _apex(apex), _axis(axis.normalized()), _color(color), _transform() {
  if (angleDegrees <= 0 || angleDegrees >= 90) {
    throw std::invalid_argument(
        "Cone angle must be strictly between 0 and 90 degrees.");
  }
  _angle_rad = angleDegrees * M_PI / 180.0;
  double cos_angle = std::cos(_angle_rad);
  _cos_angle_sq = cos_angle * cos_angle;
  updateInverseTransform();
}

void Cone::updateInverseTransform() {
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& e) {
    _inverseTransform = Transform();
    _color = Color(static_cast<unsigned char>(255), static_cast<unsigned char>(0), static_cast<unsigned char>(255));
  }
}

std::optional<double> Cone::findClosestValidIntersectionT(
    const Ray& localRay) const {
  Vector3D ro = localRay.getOrigin();
  Vector3D rd = localRay.getDirection();

  Vector3D delta_p = ro - _apex;

  double rd_dot_axis = rd.dot(_axis);
  double delta_p_dot_axis = delta_p.dot(_axis);
  double rd_dot_delta_p = rd.dot(delta_p);
  double delta_p_sq = delta_p.getSquaredMagnitude();
  double rd_sq = rd.getSquaredMagnitude();

  double a = rd_dot_axis * rd_dot_axis - rd_sq * _cos_angle_sq;
  double b =
      2.0 * (rd_dot_axis * delta_p_dot_axis - rd_dot_delta_p * _cos_angle_sq);
  double c = delta_p_dot_axis * delta_p_dot_axis - delta_p_sq * _cos_angle_sq;

  double discriminant = b * b - 4.0 * a * c;

  if (discriminant < -CONE_EPSILON) {
    return std::nullopt;
  }

  double t0, t1;

  if (std::abs(a) < CONE_EPSILON) {
    if (std::abs(b) < CONE_EPSILON) {
      return std::nullopt;
    }
    t0 = -c / b;
    t1 = t0;
  } else {
    double sqrt_discriminant = std::sqrt(std::max(0.0, discriminant));
    t0 = (-b - sqrt_discriminant) / (2.0 * a);
    t1 = (-b + sqrt_discriminant) / (2.0 * a);
  }

  double valid_t_values[2];
  int num_valid_t = 0;

  if (t0 > CONE_EPSILON) {
    Vector3D p0 = localRay.pointAt(t0);
    double nappe_check_val_0 = (p0 - _apex).dot(_axis);
    if (nappe_check_val_0 >= -CONE_EPSILON) {
      valid_t_values[num_valid_t++] = t0;
    }
  }

  if (t1 > CONE_EPSILON) {
    Vector3D p1 = localRay.pointAt(t1);
    double nappe_check_val_1 = (p1 - _apex).dot(_axis);
    if (nappe_check_val_1 >= -CONE_EPSILON) {
      valid_t_values[num_valid_t++] = t1;
    }
  }

  if (num_valid_t == 0) {
    return std::nullopt;
  } else if (num_valid_t == 1) {
    return valid_t_values[0];
  } else {
    double result_t = std::min(valid_t_values[0], valid_t_values[1]);
    return result_t;
  }
}

std::optional<Intersection> Cone::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);

  std::optional<double> t_opt = findClosestValidIntersectionT(localRay);

  if (!t_opt) {
    return std::nullopt;
  }

  double t_hit = *t_opt;
  Vector3D localIntersectionPoint = localRay.pointAt(t_hit);

  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);

  Vector3D worldNormal = getNormalAt(worldIntersectionPoint);

  Intersection intersection_data;
  intersection_data.point = worldIntersectionPoint;
  intersection_data.normal = worldNormal;
  intersection_data.distance =
      (worldIntersectionPoint - ray.getOrigin()).getMagnitude();
  intersection_data.color = _color;
  intersection_data.primitive = this;

  return intersection_data;
}

void Cone::setTransform(const Transform& transform) {
  _transform = transform;
  updateInverseTransform();
}

Transform Cone::getTransform() const {
  return _transform;
}

void Cone::setColor(const Color& color) {
  _color = color;
}

Color Cone::getColor() const {
  return _color;
}

Vector3D Cone::getNormalAt(const Vector3D& point_world) const {
  Vector3D point_local = _inverseTransform.applyToPoint(point_world);
  Vector3D PA = point_local - _apex;

  if (PA.getSquaredMagnitude() < CONE_EPSILON * CONE_EPSILON) {
    Vector3D transformedAxis = _transform.applyToNormal(_axis).normalized();
    return transformedAxis;
  }

  double PA_dot_axis = PA.dot(_axis);

  Vector3D localNormal_candidate1 = PA - _axis * (PA_dot_axis / _cos_angle_sq);
  Vector3D localNormal_candidate2 = _axis * PA_dot_axis - PA * _cos_angle_sq;

  Vector3D localNormal = localNormal_candidate1;

  Vector3D worldNormal = _transform.applyToNormal(localNormal).normalized();

  return worldNormal;
}

std::shared_ptr<IPrimitive> Cone::clone() const {
  auto clonedCone =
      std::make_shared<Cone>(_apex, _axis, _angle_rad * 180.0 / M_PI, _color);
  clonedCone->setTransform(this->_transform);
  return clonedCone;
}

Vector3D Cone::getApex() const {
  return _transform.applyToPoint(_apex);
}

Vector3D Cone::getAxis() const {
  return _transform.applyToNormal(_axis).normalized();
}

double Cone::getAngleDegrees() const {
  return _angle_rad * 180.0 / M_PI;
}

}  // namespace RayTracer
