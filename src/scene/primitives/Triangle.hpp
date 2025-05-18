/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#pragma once

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

class Triangle : public IPrimitive {
 public:
  Triangle(const Vector3D& a, const Vector3D& b, const Vector3D& c,
           const Color& color);
  ~Triangle() override = default;

  std::optional<Intersection> intersect(const Ray& ray) const override;
  void setTransform(const Transform& transform) override;
  Transform getTransform() const override;
  void setColor(const Color& color) override;
  Color getColor() const override;
  Vector3D getNormalAt(const Vector3D& point) const override;
  std::shared_ptr<IPrimitive> clone() const override;

  Vector3D getA() const;
  Vector3D getB() const;
  Vector3D getC() const;

 private:
  Vector3D _a, _b, _c;
  Color _color;
  Transform _transform;
  Transform _inverseTransform;
  Vector3D _normal;  // cached normal
};

}  // namespace RayTracer
