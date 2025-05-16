/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight
*/

#ifndef POINTLIGHT_HPP_
#define POINTLIGHT_HPP_

#include <memory>
#include <string>
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Vector3D.hpp"
#include "Light.hpp"

namespace RayTracer {

class PointLight : public Light {
 public:
  PointLight(const Vector3D& position);
  PointLight(const Vector3D& position, const Color& color);

  // ILight interface implementation
  Vector3D getDirectionFrom(const Vector3D& point) const override;
  double getDistanceFrom(const Vector3D& point) const override;
  double getIntensityAt(const Vector3D& point) const override;
  Color getColor() const override;
  bool castsShadows() const override;
  Ray getShadowRay(const Vector3D& point) const override;
  std::shared_ptr<ILight> clone() const override;
  Vector3D getPosition() const override;

  // Additional methods
  std::string toString() const override;

 private:
  Vector3D _position;
  Color _color{static_cast<uint8_t>(255), static_cast<uint8_t>(255),
               static_cast<uint8_t>(255)};  // Default white light
};

}  // namespace RayTracer

#endif /* !POINTLIGHT_HPP_ */
