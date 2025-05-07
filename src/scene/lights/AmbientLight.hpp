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
  AmbientLight(const Vector3D& color, float intensity);

  /**
   * @brief Illuminate a point
   * @param point The point to be illuminated (unused here)
   * @return A zero vector since ambient light has no direction
   */
  Vector3D illuminate(const Vector3D& point) const override;
};

}  // namespace RayTracer

#endif /* !AMBIENT_LIGHT_HPP_ */
