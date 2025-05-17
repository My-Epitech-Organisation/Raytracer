/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LimitedCone
*/

/**
 * @file LimitedCone.cpp
 * @brief Implementation of a cone primitive with height constraints and
 * optional cap for ray tracing
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "LimitedCone.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace RayTracer {

constexpr double LIMITED_CONE_EPSILON = 1e-6;

LimitedCone::LimitedCone(const Vector3D& apex, const Vector3D& axis,
                         double angleDegrees, const Color& color, double height,
                         bool hasCaps)
    : _apex(apex),
      _axis(axis.normalized()),
      _color(color),
      _transform(),
      _height(height),
      _has_caps(hasCaps) {
  if (angleDegrees <= 0 || angleDegrees >= 90) {
    throw std::invalid_argument(
        "Cone angle must be strictly between 0 and 90 degrees.");
  }
  if (height <= LIMITED_CONE_EPSILON) {
    throw std::invalid_argument("Cone height must be positive.");
  }
  _angle_rad = angleDegrees * M_PI / 180.0;
  double cos_angle = std::cos(_angle_rad);
  _cos_angle_sq = cos_angle * cos_angle;
  updateInverseTransform();
}

void LimitedCone::updateInverseTransform() {
  try {
    _inverseTransform = _transform.inverse();
  } catch (const std::runtime_error& e) {
    _inverseTransform = Transform();
    _color =
        Color(static_cast<unsigned char>(255), static_cast<unsigned char>(0),
              static_cast<unsigned char>(255));
  }
}

std::optional<double> LimitedCone::intersectBaseCap(const Ray& localRay) const {
  if (!_has_caps)
    return std::nullopt;

  Vector3D baseCenter = _apex + _axis * _height;
  double t = (baseCenter - localRay.getOrigin()).dot(_axis) /
             localRay.getDirection().dot(_axis);

  if (t < LIMITED_CONE_EPSILON)
    return std::nullopt;

  Vector3D intersectionPoint = localRay.pointAt(t);
  double baseRadius = _height * std::tan(_angle_rad);
  if ((intersectionPoint - baseCenter).getMagnitude() > baseRadius) {
    return std::nullopt;
  }
  return t;
}

std::optional<double> LimitedCone::findClosestValidIntersectionT(
    const Ray& localRay, bool& isCapIntersection) const {
  Vector3D ro = localRay.getOrigin();
  Vector3D rd = localRay.getDirection();
  Vector3D delta_p = ro - _apex;

  double rd_dot_axis = rd.dot(_axis);
  double delta_p_dot_axis = delta_p.dot(_axis);
  double rd_dot_delta_p = rd.dot(delta_p);
  double delta_p_sq = delta_p.getSquaredMagnitude();

  double a =
      rd_dot_axis * rd_dot_axis - rd.getSquaredMagnitude() * _cos_angle_sq;
  double b =
      2.0 * (rd_dot_axis * delta_p_dot_axis - rd_dot_delta_p * _cos_angle_sq);
  double c = delta_p_dot_axis * delta_p_dot_axis - delta_p_sq * _cos_angle_sq;

  double discriminant = b * b - 4.0 * a * c;

  std::optional<double> t_cone_surface = std::nullopt;

  if (discriminant >= -LIMITED_CONE_EPSILON) {
    double sqrt_discriminant = std::sqrt(std::max(0.0, discriminant));
    double t0 = (-b - sqrt_discriminant) / (2.0 * a);
    double t1 = (-b + sqrt_discriminant) / (2.0 * a);

    double valid_ts[2];
    int count = 0;

    if (t0 > LIMITED_CONE_EPSILON) {
      Vector3D p0 = localRay.pointAt(t0);
      double height_check_0 = (p0 - _apex).dot(_axis);
      if (height_check_0 >= -LIMITED_CONE_EPSILON &&
          height_check_0 <= _height + LIMITED_CONE_EPSILON) {
        if ((p0 - _apex).dot(_axis) >= -LIMITED_CONE_EPSILON) {
          valid_ts[count++] = t0;
        }
      }
    }
    if (t1 > LIMITED_CONE_EPSILON) {
      Vector3D p1 = localRay.pointAt(t1);
      double height_check_1 = (p1 - _apex).dot(_axis);
      if (height_check_1 >= -LIMITED_CONE_EPSILON &&
          height_check_1 <= _height + LIMITED_CONE_EPSILON) {
        if ((p1 - _apex).dot(_axis) >= -LIMITED_CONE_EPSILON) {
          valid_ts[count++] = t1;
        }
      }
    }
    if (count == 1)
      t_cone_surface = valid_ts[0];
    else if (count == 2)
      t_cone_surface = std::min(valid_ts[0], valid_ts[1]);
  }

  std::optional<double> t_cap = std::nullopt;
  if (_has_caps) {
    t_cap = intersectBaseCap(localRay);
  }

  if (t_cone_surface && t_cap) {
    if (*t_cone_surface < *t_cap) {
      isCapIntersection = false;
      return t_cone_surface;
    } else {
      isCapIntersection = true;
      return t_cap;
    }
  } else if (t_cone_surface) {
    isCapIntersection = false;
    return t_cone_surface;
  } else if (t_cap) {
    isCapIntersection = true;
    return t_cap;
  }
  return std::nullopt;
}

std::optional<Intersection> LimitedCone::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);
  Vector3D normal_at_intersection_local_dummy;
  bool cap_intersection = false;

  std::optional<double> t_opt =
      findClosestValidIntersectionT(localRay, cap_intersection);

  if (!t_opt) {
    return std::nullopt;
  }

  double t_hit = *t_opt;
  Vector3D localIntersectionPoint = localRay.pointAt(t_hit);
  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);

  Vector3D localNormal;
  if (cap_intersection) {
    localNormal = _axis;
  } else {
    Vector3D PA = localIntersectionPoint - _apex;
    if (PA.getSquaredMagnitude() <
            LIMITED_CONE_EPSILON * LIMITED_CONE_EPSILON &&
        !(PA.dot(_axis) > _height - LIMITED_CONE_EPSILON)) {}
    double PA_dot_axis = PA.dot(_axis);
    localNormal = PA - _axis * (PA_dot_axis / _cos_angle_sq);
  }

  Vector3D worldNormal = _transform.applyToNormal(localNormal).normalized();

  Intersection intersection_data;
  intersection_data.point = worldIntersectionPoint;
  intersection_data.normal = worldNormal;
  intersection_data.distance =
      (worldIntersectionPoint - ray.getOrigin()).getMagnitude();
  intersection_data.color = _color;
  intersection_data.primitive = this;

  return intersection_data;
}

void LimitedCone::setTransform(const Transform& transform) {
  _transform = transform;
  updateInverseTransform();
}

Transform LimitedCone::getTransform() const {
  return _transform;
}

void LimitedCone::setColor(const Color& color) {
  _color = color;
}

Color LimitedCone::getColor() const {
  return _color;
}

Vector3D LimitedCone::getNormalAt(const Vector3D& point_world) const {
  Vector3D point_local = _inverseTransform.applyToPoint(point_world);
  Vector3D localNormal;

  bool on_cap = false;
  if (_has_caps) {
    Vector3D base_center_local = _apex + _axis * _height;
    if (std::abs((point_local - base_center_local).dot(_axis)) <
        LIMITED_CONE_EPSILON) {
      double base_radius = _height * std::tan(_angle_rad);
      if ((point_local - base_center_local).getMagnitude() <=
          base_radius + LIMITED_CONE_EPSILON) {
        on_cap = true;
      }
    }
  }

  if (on_cap) {
    localNormal = _axis;
  } else {
    Vector3D PA = point_local - _apex;
    if (PA.getSquaredMagnitude() <
        LIMITED_CONE_EPSILON * LIMITED_CONE_EPSILON) {
      return _transform.applyToNormal(_axis).normalized();
    }
    double PA_dot_axis = PA.dot(_axis);
    if (PA_dot_axis < -LIMITED_CONE_EPSILON ||
        PA_dot_axis > _height + LIMITED_CONE_EPSILON) {}
    localNormal = PA - _axis * (PA_dot_axis / _cos_angle_sq);
  }

  return _transform.applyToNormal(localNormal).normalized();
}

std::shared_ptr<IPrimitive> LimitedCone::clone() const {
  auto clonedCone = std::make_shared<LimitedCone>(
      _apex, _axis, _angle_rad * 180.0 / M_PI, _color, _height, _has_caps);
  clonedCone->setTransform(this->_transform);
  return clonedCone;
}

Vector3D LimitedCone::getApex() const {
  return _transform.applyToPoint(_apex);
}

Vector3D LimitedCone::getAxis() const {
  return _transform.applyToNormal(_axis).normalized();
}

double LimitedCone::getAngleDegrees() const {
  return _angle_rad * 180.0 / M_PI;
}

double LimitedCone::getHeight() const {
  return _height;
}
bool LimitedCone::hasCaps() const {
  return _has_caps;
}

void LimitedCone::setHeight(double height) {
  if (height <= LIMITED_CONE_EPSILON) {
    throw std::invalid_argument("Cone height must be positive.");
  }
  _height = height;
}

void LimitedCone::setHasCaps(bool hasCaps) {
  _has_caps = hasCaps;
}

}  // namespace RayTracer
