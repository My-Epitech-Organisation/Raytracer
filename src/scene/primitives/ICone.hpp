/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ICone interface
*/

/**
 * @file ICone.hpp
 * @brief Definition of the ICone interface for cone primitives in the
 * raytracing engine
 * @author @mael
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef ICONE_HPP_
#define ICONE_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Interface for cone primitives.
 * Defines the common behavior for all types of cones (e.g., infinite, limited).
 */
class ICone : public IPrimitive {
 public:
  /**
   * @brief Virtual destructor.
   */
  ~ICone() override = default;

  // Methods inherited from IPrimitive that must be implemented by concrete
  // cones
  std::optional<Intersection> intersect(const Ray& ray) const override = 0;
  void setTransform(const Transform& transform) override = 0;
  Transform getTransform() const override = 0;
  void setColor(const Color& color) override = 0;
  Color getColor() const override = 0;
  Vector3D getNormalAt(const Vector3D& point) const override = 0;
  std::shared_ptr<IPrimitive> clone() const override = 0;

  // Cone-specific getters
  /**
   * @brief Get the apex point of the cone in world space.
   * @return The apex point.
   */
  virtual Vector3D getApex() const = 0;

  /**
   * @brief Get the direction vector of the cone's axis in world space
   * (normalized).
   * @return The axis vector.
   */
  virtual Vector3D getAxis() const = 0;

  /**
   * @brief Get the half-angle of the cone in degrees.
   * @return The half-angle in degrees.
   */
  virtual double getAngleDegrees() const = 0;
};

}  // namespace RayTracer

#endif /* !ICONE_HPP_ */
