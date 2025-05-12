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
  DirectionalLight(const Vector3D& color, const Vector3D& direction);

  /**
   * @brief Destructor
   */
  virtual ~DirectionalLight() = default;

  /**
   * @brief Get the string representation of the directional light
   * @return The string representation of the directional light
   */
  std::string toString() const override;

  /**
   * @brief Get the direction of the light
   */
  Vector3D getDirection() const;

 private:
  Vector3D _direction;  ///< Direction the light is pointing
};

}  // namespace RayTracer

#endif /* !DIRECTIONAL_LIGHT_HPP_ */
