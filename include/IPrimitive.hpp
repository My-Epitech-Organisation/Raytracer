/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Interface for geometric primitives
*/

/**
 * @file IPrimitive.hpp
 * @brief Interface defining the behavior of geometric primitives in the
 * raytracing engine
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef IPRIMITIVE_HPP_
#define IPRIMITIVE_HPP_

#include <memory>
#include <optional>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Represents an intersection between a ray and a primitive
 */
struct Intersection {
  double distance;        ///< Distance from ray origin to intersection point
  Vector3D point;         ///< Point of intersection in world space
  Vector3D normal;        ///< Surface normal at intersection point
  Color color;            ///< Color at intersection point
  const void* primitive;  ///< Pointer to the primitive that was intersected
};

/**
 * @brief Interface for all geometric primitives
 */
class IPrimitive {
 public:
  /**
   * @brief Virtual destructor
   */
  virtual ~IPrimitive() = default;

  /**
   * @brief Check if a ray intersects this primitive
   * @param ray The ray to check
   * @return Intersection data if hit, std::nullopt otherwise
   */
  virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;

  /**
   * @brief Set the transformation matrix for this primitive
   * @param transform The transformation to apply
   */
  virtual void setTransform(const Transform& transform) = 0;

  /**
   * @brief Get the transformation of this primitive
   * @return The current transformation
   */
  virtual Transform getTransform() const = 0;

  /**
   * @brief Set the color of this primitive
   * @param color The color to set
   */
  virtual void setColor(const Color& color) = 0;

  /**
   * @brief Get the color of this primitive
   * @return The current color
   */
  virtual Color getColor() const = 0;

  /**
   * @brief Get the normal at a specific point on the primitive
   * @param point The point in world space
   * @return The normal vector at that point
   */
  virtual Vector3D getNormalAt(const Vector3D& point) const = 0;

  /**
   * @brief Clone this primitive
   * @return A new instance of this primitive
   */
  virtual std::shared_ptr<IPrimitive> clone() const = 0;
};

}  // namespace RayTracer

#endif /* !IPRIMITIVE_HPP_ */
