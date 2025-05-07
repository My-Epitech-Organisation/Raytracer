/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Directional Light header
*/

#ifndef DIRECTIONAL_LIGHT_HPP_
#define DIRECTIONAL_LIGHT_HPP_

#include "Light.hpp"

namespace RayTracer {

class DirectionalLight : public Light {
 public:
  /**
   * @brief Constructor
   * @param color The color of the light
   * @param intensity The intensity of the light
   * @param direction The direction the light is pointing (normalized)
   */
  DirectionalLight(const Vector3D& color, float intensity,
                   const Vector3D& direction);

  /**
   * @brief Compute the light's contribution to a given point
   * (for now, just return the direction as a vector)
   */
  Vector3D illuminate(const Vector3D& point) const override;

  /**
   * @brief Get the direction of the light
   */
  Vector3D getDirection() const;

 private:
  Vector3D _direction;  ///< Direction the light is pointing
};

}  // namespace RayTracer

#endif /* !DIRECTIONAL_LIGHT_HPP_ */
