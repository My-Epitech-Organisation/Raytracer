#include "Cone.hpp"
#include <iostream>
#include <stdexcept>

namespace RayTracer {

constexpr double CONE_EPSILON = 1e-6;

Cone::Cone(const Vector3D& apex, const Vector3D& axis, double angleDegrees,
           const Color& color)
    : _apex(apex), _axis(axis.normalized()), _color(color), _transform() {
  if (angleDegrees <= 0 || angleDegrees >= 90) {
    throw std::invalid_argument("Cone angle must be between 0 and 90 degrees.");
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
    throw std::runtime_error(
        std::string("Error initializing inverse transform for Cone: ") +
        e.what());
  }
}

std::optional<double> Cone::findClosestValidIntersectionT(
    const Ray& localRay) const {
  Vector3D ro = localRay.getOrigin();
  Vector3D rd = localRay.getDirection();
  Vector3D v_minus_ro = _apex - ro;

  double rd_dot_axis = rd.dot(_axis);
  double v_minus_ro_dot_axis = v_minus_ro.dot(_axis);

  double a = rd_dot_axis * rd_dot_axis - _cos_angle_sq;
  double b = 2.0 * (rd.dot(v_minus_ro) * _cos_angle_sq -
                    rd_dot_axis * v_minus_ro_dot_axis);
  double c = v_minus_ro_dot_axis * v_minus_ro_dot_axis -
             v_minus_ro.dot(v_minus_ro) * _cos_angle_sq;

  double t_intersect = -1.0;

  if (std::abs(a) < CONE_EPSILON) {
    if (std::abs(b) > CONE_EPSILON) {
      double t_candidate = -c / b;
      if (t_candidate > CONE_EPSILON) {
        Vector3D p_local = localRay.pointAt(t_candidate);
        if ((p_local - _apex).dot(_axis) >= -CONE_EPSILON) {
          t_intersect = t_candidate;
        }
      }
    }
  } else {
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant >= -CONE_EPSILON) {
      double sqrt_discriminant = std::sqrt(std::max(0.0, discriminant));

      double t_values[2];
      t_values[0] = (-b - sqrt_discriminant) / (2.0 * a);
      t_values[1] = (-b + sqrt_discriminant) / (2.0 * a);

      double smallest_valid_t = -1.0;

      for (double current_t_val : t_values) {
        if (current_t_val > CONE_EPSILON) {
          Vector3D p_local = localRay.pointAt(current_t_val);
          if ((p_local - _apex).dot(_axis) >= -CONE_EPSILON) {
            if (smallest_valid_t < 0 || current_t_val < smallest_valid_t) {
              smallest_valid_t = current_t_val;
            }
          }
        }
      }
      t_intersect = smallest_valid_t;
    }
  }

  if (t_intersect < 0.0) {
    return std::nullopt;
  }
  return t_intersect;
}

std::optional<Intersection> Cone::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);

  std::optional<double> t_intersect_opt =
      findClosestValidIntersectionT(localRay);

  if (!t_intersect_opt) {
    return std::nullopt;
  }

  double t_intersect = *t_intersect_opt;

  Vector3D localIntersectionPoint = localRay.pointAt(t_intersect);
  Vector3D worldIntersectionPoint =
      _transform.applyToPoint(localIntersectionPoint);
  Vector3D worldNormal = getNormalAt(worldIntersectionPoint);

  if (worldNormal.dot(ray.getDirection()) > 0) {
    worldNormal =
        Vector3D(-worldNormal.getX(), -worldNormal.getY(), -worldNormal.getZ());
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

Vector3D Cone::getNormalAt(const Vector3D& point) const {
  Vector3D localPoint = _inverseTransform.applyToPoint(point);
  Vector3D C_to_P = localPoint - _apex;

  if (C_to_P.getMagnitude() < CONE_EPSILON) {
    return _transform.applyToNormal(-_axis).normalized();
  }

  double cp_dot_axis = C_to_P.dot(_axis);
  Vector3D normal_local_unnormalized =
      (C_to_P - _axis * (cp_dot_axis / _cos_angle_sq));

  if (normal_local_unnormalized.getMagnitude() < CONE_EPSILON) {
    return _transform.applyToNormal(-_axis).normalized();
  }

  Vector3D normal_local = normal_local_unnormalized.normalized();
  Vector3D worldNormal = _transform.applyToNormal(normal_local).normalized();
  return worldNormal;
}

std::shared_ptr<IPrimitive> Cone::clone() const {
  auto clonedCone =
      std::make_shared<Cone>(_apex, _axis, _angle_rad * 180.0 / M_PI, _color);
  clonedCone->setTransform(this->_transform);
  return clonedCone;
}

Vector3D Cone::getApex() const {
  return _apex;
}

Vector3D Cone::getAxis() const {
  return _axis;
}

double Cone::getAngleDegrees() const {
  return _angle_rad * 180.0 / M_PI;
}

}  // namespace RayTracer
