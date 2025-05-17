/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LimitedCone class
*/

/**
 * @file LimitedCone.hpp
 * @brief Definition of the LimitedCone class for representing cones with a
 * limited height
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef LIMITEDCONE_HPP_
#define LIMITEDCONE_HPP_

#include "ICone.hpp"

namespace RayTracer {

/**
 * @brief Represents a cone primitive with a limited height.
 * The cone is defined by an apex, an axis vector, a half-angle, and a maximum
 * height.
 */
class LimitedCone : public ICone {
 public:
  /**
   * @brief Constructor for a LimitedCone.
   * @param apex The apex point of the cone.
   * @param axis The direction vector of the cone's axis (will be normalized).
   * @param angleDegrees The half-angle of the cone in degrees (must be > 0 and
   * < 90).
   * @param color The color of the cone.
   * @param height The maximum height of the cone along its axis from the apex.
   * @param hasCaps Boolean indicating if the cone has end caps (base and
   * potentially top if truncated by height).
   */
  LimitedCone(const Vector3D& apex, const Vector3D& axis, double angleDegrees,
              const Color& color, double height, bool hasCaps = true);

  /**
   * @brief Destructor.
   */
  ~LimitedCone() override = default;

  std::optional<Intersection> intersect(const Ray& ray) const override;
  void setTransform(const Transform& transform) override;
  Transform getTransform() const override;
  void setColor(const Color& color) override;
  Color getColor() const override;
  Vector3D getNormalAt(const Vector3D& point) const override;
  std::shared_ptr<IPrimitive> clone() const override;

  void setHeight(double height);
  void setHasCaps(bool hasCaps);

  // ICone specific getters
  Vector3D getApex() const override;
  Vector3D getAxis() const override;
  double getAngleDegrees() const override;

  // LimitedCone specific getters
  double getHeight() const;
  bool hasCaps() const;

 private:
  Vector3D _apex;        ///< Apex of the cone in its local coordinate system.
  Vector3D _axis;        ///< Normalized axis vector in local coordinate system.
  double _angle_rad;     ///< Half-angle of the cone in radians.
  double _cos_angle_sq;  ///< cos^2(half-angle) for intersection calculations.
  Color _color;          ///< Color of the cone.
  Transform _transform;  ///< Transformation applied to the cone.
  Transform _inverseTransform;  ///< Inverse of the transformation.
  double _height;               ///< Maximum height of the cone.
  bool _has_caps;               ///< Whether the cone has end caps.

  /**
   * @brief Updates the inverse transform matrix whenever the main transform
   * changes.
   */
  void updateInverseTransform();

  /**
   * @brief Finds the smallest valid intersection distance 't' for a ray in
   * local space, considering height limits and caps.
   * @param localRay The ray in the cone's local coordinate system.
   * @return An std::optional<double> containing the smallest valid 't' if an
   * intersection occurs, otherwise std::nullopt.
   */
  std::optional<double> findClosestValidIntersectionT(
      const Ray& localRay, bool& isCapIntersection) const;

  /**
   * @brief Helper to check intersection with the base cap of the cone.
   * @param localRay The ray in local coordinates.
   * @return std::optional<double> containing 't' if intersection occurs, else
   * std::nullopt.
   */
  std::optional<double> intersectBaseCap(const Ray& localRay) const;
};

}  // namespace RayTracer

#endif /* !LIMITEDCONE_HPP_ */
