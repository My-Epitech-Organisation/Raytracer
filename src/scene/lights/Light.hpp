/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Abstract Light class
*/

#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Base class for all lights in the raytracer
 */
class Light {
 public:
  /**
   * @brief Constructor
   * @param color RGB color of the light
   * @param intensity Brightness of the light
   */
  Light(const Vector3D& color, float intensity)
      : _color(color), _intensity(intensity) {}

  /**
   * @brief Virtual destructor
   */
  virtual ~Light() = default;

  /**
   * @brief Get the color of the light
   */
  Vector3D getColor() const { return _color; }

  /**
   * @brief Get the intensity of the light
   */
  float getIntensity() const { return _intensity; }

  /**
   * @brief Optional: compute light contribution (pure virtual)
   */
  virtual Vector3D illuminate(const Vector3D& point) const = 0;

 protected:
  Vector3D _color;
  float _intensity;
};

}  // namespace RayTracer

#endif /* !LIGHT_HPP_ */
