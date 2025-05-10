#include "Cylinder.hpp"
#include "../../../include/IPrimitive.hpp"
#include "../../core/Vector3D.hpp"
#include "../../core/Transform.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

namespace RayTracer {

Cylinder::Cylinder(double radius, double height, const Color& color)
    : _radius(radius), _height(height), _color(color), _transform() {
    if (radius <= 0) {
        throw std::invalid_argument("Cylinder radius must be positive");
    }
    if (height <= 0) {
        throw std::invalid_argument("Cylinder height must be positive");
    }
    try {
        _inverseTransform = _transform.inverse();
    } catch (const std::runtime_error& e) {
        _inverseTransform = Transform();
    }
}

void Cylinder::setTransform(const Transform& transform) {
    _transform = transform;
    try {
        _inverseTransform = _transform.inverse();
    } catch (const std::runtime_error& e) {
        _inverseTransform = Transform();
    }
}

Transform Cylinder::getTransform() const {
    return _transform;
}

void Cylinder::setColor(const Color& color) {
    _color = color;
}

Color Cylinder::getColor() const {
    return _color;
}

Vector3D Cylinder::getNormalAt(const Vector3D& point) const {
    Vector3D localPoint = _transform.inverse().applyToPoint(point);
    double halfHeight = _height / 2.0;
    
    // Check if point is on a cap
    if (std::abs(localPoint.getY() - halfHeight) < CYLINDER_EPSILON) {
        return _transform.applyToNormal(Vector3D(0, 1, 0)).normalized();
    }
    if (std::abs(localPoint.getY() + halfHeight) < CYLINDER_EPSILON) {
        return _transform.applyToNormal(Vector3D(0, -1, 0)).normalized();
    }
    
    // Point is on the body
    Vector3D localNormal(localPoint.getX(), 0, localPoint.getZ());
    return _transform.applyToNormal(localNormal).normalized();
}

std::shared_ptr<IPrimitive> Cylinder::clone() const {
    return std::make_shared<Cylinder>(*this);
}

double Cylinder::getRadius() const {
    return _radius;
}

double Cylinder::getHeight() const {
    return _height;
}

std::optional<Intersection> Cylinder::intersect(const Ray& ray) const {
  Ray localRay = ray.transform(_inverseTransform);
  double t_min_overall = std::numeric_limits<double>::infinity();
  std::optional<Intersection> closest_intersection = std::nullopt;

  // Intersect with body
  auto body_intersect_result = intersectBody(localRay, t_min_overall);
  if (body_intersect_result) {
    closest_intersection = body_intersect_result;
    // t_min_overall is updated within intersectBody if a closer hit is found
  }

  // Intersect with caps
  auto cap_intersect_result = intersectCaps(localRay, t_min_overall);
  if (cap_intersect_result) {
    closest_intersection = cap_intersect_result;
    // t_min_overall is updated within intersectCaps if a closer hit is found
  }

  if (closest_intersection) {
    // Transform intersection point and normal back to world space
    Vector3D worldIntersectionPoint = _transform.applyToPoint(closest_intersection->point);
    Vector3D worldNormal = _transform.applyToNormal(closest_intersection->normal).normalized();

    // Ensure normal points towards the ray origin (away from the surface)
    if (worldNormal.dot(ray.getDirection()) > 0) {
        worldNormal = -worldNormal;
    }

    closest_intersection->point = worldIntersectionPoint;
    closest_intersection->normal = worldNormal;
    // Recalculate distance in world space to be accurate
    closest_intersection->distance = (worldIntersectionPoint - ray.getOrigin()).getMagnitude();
    closest_intersection->color = _color;
    closest_intersection->primitive = this;
    return closest_intersection;
  }

  return std::nullopt;
}

std::optional<Intersection> Cylinder::intersectCaps(const Ray& localRay, double& t_min_overall) const {
    std::optional<Intersection> cap_intersection = std::nullopt;
    double halfHeight = _height / 2.0;

    // Top cap: plane Y = halfHeight
    if (std::abs(localRay.getDirection().getY()) > CYLINDER_EPSILON) { // Avoid division by zero if ray parallel to cap plane
        double t_top = (halfHeight - localRay.getOrigin().getY()) / localRay.getDirection().getY();
        if (t_top > CYLINDER_EPSILON && t_top < t_min_overall) {
            Vector3D p_top = localRay.pointAt(t_top);
            // Check if intersection point is within the radius of the cap
            if (p_top.getX() * p_top.getX() + p_top.getZ() * p_top.getZ() <= _radius * _radius + CYLINDER_EPSILON) {
                t_min_overall = t_top;
                Intersection intersection_data;
                intersection_data.distance = t_top; // Local distance for now
                intersection_data.point = p_top;    // Local point
                intersection_data.normal = Vector3D(0, 1, 0); // Local normal for top cap
                // Color and primitive will be set by the main intersect method
                cap_intersection = intersection_data;
            }
        }
    }

    // Bottom cap: plane Y = -halfHeight
    if (std::abs(localRay.getDirection().getY()) > CYLINDER_EPSILON) { // Avoid division by zero
        double t_bottom = (-halfHeight - localRay.getOrigin().getY()) / localRay.getDirection().getY();
        if (t_bottom > CYLINDER_EPSILON && t_bottom < t_min_overall) {
            Vector3D p_bottom = localRay.pointAt(t_bottom);
            // Check if intersection point is within the radius of the cap
            if (p_bottom.getX() * p_bottom.getX() + p_bottom.getZ() * p_bottom.getZ() <= _radius * _radius + CYLINDER_EPSILON) {
                t_min_overall = t_bottom;
                Intersection intersection_data;
                intersection_data.distance = t_bottom; // Local distance
                intersection_data.point = p_bottom;   // Local point
                intersection_data.normal = Vector3D(0, -1, 0); // Local normal for bottom cap
                cap_intersection = intersection_data;
            }
        }
    }
    return cap_intersection;
}

std::optional<Intersection> Cylinder::intersectBody(const Ray& localRay, double& t_min_overall) const {
    Vector3D O = localRay.getOrigin();
    Vector3D D = localRay.getDirection();

    // Cylinder aligned with Y axis: x^2 + z^2 = radius^2
    // Ray: P(t) = O + tD
    // (Ox + tDx)^2 + (Oz + tDz)^2 = radius^2

    double a = D.getX() * D.getX() + D.getZ() * D.getZ();
    double b = 2.0 * (O.getX() * D.getX() + O.getZ() * D.getZ());
    double c = O.getX() * O.getX() + O.getZ() * O.getZ() - _radius * _radius;

    if (std::abs(a) < CYLINDER_EPSILON) { // Ray is parallel to the Y-axis (cylinder's axis)
        // Check if it's inside the infinite cylinder body
        if (c <= 0) { // Ray is inside or on the boundary of the infinite cylinder
            // This case is complex as it might hit caps or be entirely within.
            // For now, we assume if it's parallel and inside, it doesn't hit the body sides in a way that this quadratic solves.
            // Cap intersections will handle cases where it enters/exits through caps.
            // Or, if the cylinder is infinitely long and ray is inside, it's an infinite number of intersections.
            // We are looking for the *first* hit on the *finite* cylinder.
            // This specific condition (parallel and inside) is better handled by cap checks for a finite cylinder.
        }
        return std::nullopt; // No intersection with the curved body if parallel
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::nullopt; // No real roots, no intersection with infinite cylinder body
    }

    double sqrt_discriminant = std::sqrt(discriminant);
    double t0 = (-b - sqrt_discriminant) / (2 * a);
    double t1 = (-b + sqrt_discriminant) / (2 * a);

    std::optional<Intersection> body_intersection = std::nullopt;
    double halfHeight = _height / 2.0;

    // Check t0
    if (t0 > CYLINDER_EPSILON && t0 < t_min_overall) {
        Vector3D p0 = localRay.pointAt(t0);
        if (p0.getY() >= -halfHeight - CYLINDER_EPSILON && p0.getY() <= halfHeight + CYLINDER_EPSILON) {
            t_min_overall = t0;
            Intersection intersection_data;
            intersection_data.distance = t0;
            intersection_data.point = p0;
            intersection_data.normal = Vector3D(p0.getX(), 0, p0.getZ()).normalized();
            body_intersection = intersection_data;
        }
    }

    // Check t1 (it must be greater than t0, but check if it's a closer valid intersection)
    if (t1 > CYLINDER_EPSILON && t1 < t_min_overall) {
        Vector3D p1 = localRay.pointAt(t1);
        if (p1.getY() >= -halfHeight - CYLINDER_EPSILON && p1.getY() <= halfHeight + CYLINDER_EPSILON) {
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

} // namespace RayTracer