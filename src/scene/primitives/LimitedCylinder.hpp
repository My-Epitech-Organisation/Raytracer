/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder primitive class header
*/

/**
 * @file LimitedCylinder.hpp
 * @brief Header file for the LimitedCylinder class which provides limited
 * cylinder primitive functionality for ray tracing
 * @author @mael
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef LIMITEDCYLINDER_HPP_
#define LIMITEDCYLINDER_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"
#include "ICylinder.hpp"

namespace RayTracer {

/**
 * @brief Represents a cylinder primitive.
 * This cylinder is defined by its radius and height.
 * It is aligned with the Y-axis in its local coordinate system,
 * centered at (0,0,0), extending from -height/2 to +height/2 on the Y-axis.
 */
class LimitedCylinder : public ICylinder {
 public:
  /**
   * @brief Constructor for a cylinder.
   * @param radius The radius of the cylinder. Must be positive.
   * @param height The total height of the cylinder. Must be positive.
   * @param color The color of the cylinder.
   */
  LimitedCylinder(double radius, double height, const Color& color);

  /**
   * @brief Destructor.
   */
  ~LimitedCylinder() override = default;

  // --- IPrimitive Interface Implementation ---

  /**
   * @brief Check if a ray intersects this cylinder.
   * Considers intersections with the body and the end caps.
   * @param ray The ray to check for intersection.
   * @return An std::optional<Intersection> containing intersection data if a
   * hit occurs within the cylinder's bounds, std::nullopt otherwise.
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Set the transformation matrix for this cylinder.
   * @param transform The transformation to apply.
   */
  void setTransform(const Transform& transform) override;

  /**
   * @brief Get the transformation of this cylinder.
   * @return The current transformation.
   */
  Transform getTransform() const override;

  /**
   * @brief Set the color of this cylinder.
   * @param color The color to set.
   */
  void setColor(const Color& color) override;

  /**
   * @brief Get the color of this cylinder.
   * @return The current color.
   */
  Color getColor() const override;

  /**
   * @brief Get the normal vector at a specific point on the cylinder's surface.
   * The point is assumed to be on the surface.
   * @param point The point in world space.
   * @return The normal vector at that point.
   */
  Vector3D getNormalAt(const Vector3D& point) const override;

  /**
   * @brief Clone this cylinder.
   * @return A std::shared_ptr to a new instance of this cylinder with the same
   * properties.
   */
  std::shared_ptr<IPrimitive> clone() const override;

  // ICylinder interface
  /**
   * @brief Get the base center of the cylinder.
   * @return The base center point.
   */
  Vector3D getBaseCenter() const override;

  /**
   * @brief Get the axis of the cylinder.
   * @return The axis vector.
   */
  Vector3D getAxis() const override;

  /**
   * @brief Get the radius of the cylinder.
   * @return The radius.
   */
  double getRadius() const override;

  /**
   * @brief Get the height of the cylinder.
   * @return The height.
   */
  double getHeight() const override;

 private:
  double _radius;               ///< The radius of the cylinder.
  double _height;               ///< The total height of the cylinder.
  Color _color;                 ///< The color of the cylinder.
  Transform _transform;         ///< Transformation applied to the cylinder.
  Transform _inverseTransform;  ///< Cached inverse of _transform for
                                ///< intersection calculations.

  /**
   * @brief Helper function to check intersection with a single cylinder cap.
   * @param localRay Ray in the cylinder's local coordinate system.
   * @param t_min_overall Reference to the closest intersection distance found
   * so far. Updated if a closer hit is found.
   * @param cap_y_position The Y coordinate of the cap plane.
   * @param cap_normal The normal vector of the cap plane (in local space).
   * @return An optional Intersection data if a valid cap intersection is found
   * closer than t_min_overall, otherwise std::nullopt. Intersection data is in
   * local space.
   */
  std::optional<Intersection> checkCap(const Ray& localRay,
                                       double& t_min_overall,
                                       double cap_y_position,
                                       const Vector3D& cap_normal) const;

  /**
   * @brief Calculates intersection with the cylinder's circular end caps.
   * @param localRay Ray in the cylinder's local coordinate system.
   * @param t_min_overall Reference to the closest intersection distance found
   * so far. This method will update it if a closer cap intersection is found.
   * @return An optional Intersection data if a valid cap intersection is found
   * closer than t_min_overall, otherwise std::nullopt. Intersection data is in
   * local space.
   */
  std::optional<Intersection> intersectCaps(const Ray& localRay,
                                            double& t_min_overall) const;

  /**
   * @brief Calculates intersection with the cylinder's curved body.
   * @param localRay Ray in the cylinder's local coordinate system.
   * @param t_min_overall Reference to the closest intersection distance found
   * so far. This method will update it if a closer body intersection is found.
   * @return An optional Intersection data if a valid body intersection is found
   * closer than t_min_overall, otherwise std::nullopt. Intersection data is in
   * local space.
   */
  std::optional<Intersection> intersectBody(const Ray& localRay,
                                            double& t_min_overall) const;
};

}  // namespace RayTracer

#endif /* !LIMITEDCYLINDER_HPP_ */
