#ifndef CONE_HPP_
#define CONE_HPP_

#include <cmath>  // For M_PI, tan, cos, sqrt, acos
#include <memory>
#include <optional>
#include <string>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Represents an infinite cone primitive.
 * The cone is defined by an apex, an axis vector, and a half-angle.
 */
class Cone : public IPrimitive {
 public:
  /**
   * @brief Constructor for a Cone.
   * @param apex The apex point of the cone.
   * @param axis The direction vector of the cone's axis (will be normalized).
   * @param angle The half-angle of the cone in degrees (must be > 0 and < 90).
   * @param color The color of the cone.
   */
  Cone(const Vector3D& apex, const Vector3D& axis, double angleDegrees,
       const Color& color);

  /**
   * @brief Destructor.
   */
  ~Cone() override = default;

  /**
   * @brief Check if a ray intersects this cone.
   * @param ray The ray to check.
   * @return Intersection data if hit, std::nullopt otherwise.
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Set the transformation matrix for this cone.
   * @param transform The transformation to apply.
   */
  void setTransform(const Transform& transform) override;

  /**
   * @brief Get the transformation of this cone.
   * @return The current transformation.
   */
  Transform getTransform() const override;

  /**
   * @brief Set the color of this cone.
   * @param color The color to set.
   */
  void setColor(const Color& color) override;

  /**
   * @brief Get the color of this cone.
   * @return The current color.
   */
  Color getColor() const override;

  /**
   * @brief Get the normal at a specific point on the cone surface.
   * @param point The point in world space.
   * @return The normal vector at that point.
   */
  Vector3D getNormalAt(const Vector3D& point) const override;

  /**
   * @brief Clone this cone.
   * @return A new instance of this cone.
   */
  std::shared_ptr<IPrimitive> clone() const override;

  // Specific getters for Cone
  Vector3D getApex() const;
  Vector3D getAxis() const;
  double getAngleDegrees() const;

 private:
  Vector3D _apex;        ///< Apex of the cone in its local coordinate system.
  Vector3D _axis;        ///< Normalized axis vector in local coordinate system.
  double _angle_rad;     ///< Half-angle of the cone in radians.
  double _cos_angle_sq;  ///< cos^2(half-angle) for intersection calculations.
  Color _color;          ///< Color of the cone.
  Transform _transform;  ///< Transformation applied to the cone.
  Transform _inverseTransform;  ///< Inverse of the transformation.

  /**
   * @brief Updates the inverse transform matrix whenever the main transform
   * changes.
   */
  void updateInverseTransform();

  /**
   * @brief Finds the smallest valid intersection distance 't' for a ray in
   * local space. This method calculates potential intersection distances,
   * checks if they are positive, and verifies they lie on the correct nappe of
   * the cone.
   * @param localRay The ray in the cone's local coordinate system.
   * @return An std::optional<double> containing the smallest valid 't' if an
   * intersection occurs, otherwise std::nullopt.
   */
  std::optional<double> findClosestValidIntersectionT(
      const Ray& localRay) const;
};

}  // namespace RayTracer

#endif /* !CONE_HPP_ */
