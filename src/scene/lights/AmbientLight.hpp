/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ambient Light header
*/

#ifndef AMBIENT_LIGHT_HPP_
#define AMBIENT_LIGHT_HPP_

#include "Light.hpp"

namespace RayTracer {

class AmbientLight : public Light {
 public:
  /**
   * @brief Constructor
   * @param color The color of the ambient light
   * @param intensity The intensity of the light
   */
  AmbientLight(const Vector3D& color);

  /**
   * @brief Destructor
   */
  virtual ~AmbientLight() = default;

  /**
   * @brief Get the string representation of the ambient light
   * @return The string representation of the ambient light
   */
  std::string toString() const override;
};

}  // namespace RayTracer

#endif /* !AMBIENT_LIGHT_HPP_ */
