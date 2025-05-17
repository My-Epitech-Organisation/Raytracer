/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight
*/

/**
 * @file AmbientLight.hpp
 * @brief Definition of the AmbientLight class for representing ambient lighting
 * in the scene
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef AMBIENTLIGHT_HPP_
#define AMBIENTLIGHT_HPP_

#include <memory>
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "Light.hpp"

namespace RayTracer {

class AmbientLight : public Light {
 public:
  explicit AmbientLight(float ambient);
  AmbientLight(float ambient, const Color& color);

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
  float getAmbient() const;
  std::string toString() const override;

 private:
  float _ambient;
  Color _color{static_cast<uint8_t>(255), static_cast<uint8_t>(255),
               static_cast<uint8_t>(255)};  // Default white light
};

}  // namespace RayTracer

#endif /* !AMBIENTLIGHT_HPP_ */
