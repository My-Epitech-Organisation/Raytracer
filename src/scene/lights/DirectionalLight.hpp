/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight
*/

#ifndef DIRECTIONALLIGHT_HPP_
#define DIRECTIONALLIGHT_HPP_

#include <memory>
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "Light.hpp"

namespace RayTracer {

class DirectionalLight : public Light {
 public:
  explicit DirectionalLight(const Vector3D& direction);
  DirectionalLight(const Vector3D& direction, const Color& color);

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
  const Vector3D& getDirection() const;
  std::string toString() const override;

 private:
  Vector3D _direction;
  Color _color{static_cast<uint8_t>(255), static_cast<uint8_t>(255),
               static_cast<uint8_t>(255)};  // Default white light
};

}  // namespace RayTracer

#endif /* !DIRECTIONALLIGHT_HPP_ */
