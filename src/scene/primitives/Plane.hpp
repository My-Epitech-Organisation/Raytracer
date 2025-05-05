/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane primitive header
*/

#ifndef PLANE_HPP_
#define PLANE_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

constexpr double EPSILON = 1e-6;

/**
 * @brief Represents an infinite plane primitive
 */
class Plane : public IPrimitive {
 public:
  /**
   * @brief Constructor for a plane
   * @param point A point on the plane
   * @param normal The normal vector of the plane
   * @param color The color of the plane
   */
  Plane(const Vector3D& point, const Vector3D& normal, const Color& color);

  /**
   * @brief Destructor
   */
  ~Plane() override = default;

  /**
   * @brief Check if a ray intersects this plane
   * @param ray The ray to check
   * @return Intersection data if hit, std::nullopt otherwise
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Set the transformation matrix for this plane
   * @param transform The transformation to apply
   */
  void setTransform(const Transform& transform) override;

  /**
   * @brief Get the transformation of this plane
   * @return The current transformation
   */
  Transform getTransform() const override;

  /**
   * @brief Set the color of this plane
   * @param color The color to set
   */
  void setColor(const Color& color) override;

  /**
   * @brief Get the color of this plane
   * @return The current color
   */
  Color getColor() const override;

  /**
   * @brief Get the normal at a specific point on the plane
   * @param point The point in world space
   * @return The normal vector of the plane
   */
  Vector3D getNormalAt(const Vector3D& point) const override;

  /**
   * @brief Clone this plane
   * @return A new instance of this plane
   */
  std::shared_ptr<IPrimitive> clone() const override;

 private:
  Vector3D _point;              ///< A point on the plane
  Vector3D _normal;             ///< The normal vector of the plane
  Color _color;                 ///< The color of the plane
  Transform _transform;         ///< Transformation applied to the plane
  Transform _inverseTransform;  ///< Inverse transformation
};

}  // namespace RayTracer

#endif /* !PLANE_HPP_ */
