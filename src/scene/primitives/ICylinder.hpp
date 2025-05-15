#ifndef ICYLINDER_HPP_
#define ICYLINDER_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Interface for cylinder primitives.
 * Defines the common behavior for all types of cylinders (e.g., infinite, limited).
 */
class ICylinder : public IPrimitive {
 public:
  /**
   * @brief Virtual destructor.
   */
  ~ICylinder() override = default;

  // Methods inherited from IPrimitive that must be implemented by concrete cylinders
  std::optional<Intersection> intersect(const Ray& ray) const override = 0;
  void setTransform(const Transform& transform) override = 0;
  Transform getTransform() const override = 0;
  void setColor(const Color& color) override = 0;
  Color getColor() const override = 0;
  Vector3D getNormalAt(const Vector3D& point) const override = 0;
  std::shared_ptr<IPrimitive> clone() const override = 0;

  // Cylinder-specific getters
  /**
   * @brief Get the base center of the cylinder in world space.
   * @return The base center point.
   */
  virtual Vector3D getBaseCenter() const = 0;

  /**
   * @brief Get the axis direction of the cylinder in world space (normalized).
   * @return The axis vector.
   */
  virtual Vector3D getAxis() const = 0;

  /**
   * @brief Get the radius of the cylinder.
   * @return The radius.
   */
  virtual double getRadius() const = 0;

  /**
   * @brief Get the height of the cylinder (for limited cylinders).
   * For infinite cylinders, this can return std::numeric_limits<double>::infinity().
   * @return The height.
   */
  virtual double getHeight() const = 0;
};

}  // namespace RayTracer

#endif /* !ICYLINDER_HPP_ */
