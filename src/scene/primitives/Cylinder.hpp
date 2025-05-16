/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder
*/

#ifndef CYLINDER_HPP_
#define CYLINDER_HPP_

#include <memory>
#include <optional>
#include <vector>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"
#include "ICylinder.hpp"

namespace RayTracer {

/**
 * @brief Represents an infinite cylinder primitive (no caps, infinite height).
 * The cylinder is aligned with the Y-axis in its local coordinate system,
 * centered at (0,0,0), and extends infinitely along Y.
 */
class Cylinder : public ICylinder {
 public:
  Cylinder(double radius, const Color& color);
  ~Cylinder() override = default;

  std::optional<Intersection> intersect(const Ray& ray) const override;
  void setTransform(const Transform& transform) override;
  Transform getTransform() const override;
  void setColor(const Color& color) override;
  Color getColor() const override;
  Vector3D getNormalAt(const Vector3D& point) const override;
  std::shared_ptr<IPrimitive> clone() const override;

  // ICylinder interface
  Vector3D getBaseCenter() const override;
  Vector3D getAxis() const override;
  double getRadius() const override;
  double getHeight() const override;

 private:
  double _radius;
  Color _color;
  Transform _transform;
  Transform _inverseTransform;
};

}  // namespace RayTracer

#endif /* CYLINDER_HPP_ */
