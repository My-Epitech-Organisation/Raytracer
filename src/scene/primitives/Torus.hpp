/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus
*/

/**
 * @file Torus.cpp
 * @brief Implementation of the Torus class which provides toroidal primitive
 * functionality for ray tracing
 * @author @mael
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef TORUS_HPP_
#define TORUS_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

constexpr double ROOT_FINDING_THRESHOLD = 1e-4;

class Torus : public IPrimitive {
 public:
  Torus(double majorRadius, double tubeRadius, const Color& color);
  ~Torus() override = default;

  std::optional<Intersection> intersect(const Ray& ray) const override;
  void setTransform(const Transform& transform) override;
  Transform getTransform() const override;
  void setColor(const Color& color) override;
  Color getColor() const override;
  Vector3D getNormalAt(const Vector3D& point) const override;
  std::shared_ptr<IPrimitive> clone() const override;

  double getMajorRadius() const;
  double getTubeRadius() const;

 private:
  double _majorRadius;
  double _tubeRadius;
  Color _color;
  Transform _transform;
  Transform _inverseTransform;
};

}  // namespace RayTracer

#endif /* !TORUS_HPP_ */
