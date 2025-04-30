/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Interface for light sources
*/

#ifndef ILIGHT_HPP_
#define ILIGHT_HPP_

#include <memory>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Interface for all light sources
 */
class ILight {
 public:
  /**
     * @brief Virtual destructor
     */
  virtual ~ILight() = default;

  /**
     * @brief Get the direction from a point to this light
     * @param point The point in world space
     * @return The normalized direction vector from the point to the light
     */
  virtual Vector3D getDirectionFrom(const Vector3D& point) const = 0;

  /**
     * @brief Get the distance from a point to this light
     * @param point The point in world space
     * @return The distance to the light (for point lights) or infinity (for directional lights)
     */
  virtual double getDistanceFrom(const Vector3D& point) const = 0;

  /**
     * @brief Get the intensity of this light at a given point
     * @param point The point in world space
     * @return The intensity value [0.0 - 1.0]
     */
  virtual double getIntensityAt(const Vector3D& point) const = 0;

  /**
     * @brief Get the color of this light
     * @return The light color
     */
  virtual Color getColor() const = 0;

  /**
     * @brief Check if this light casts shadows
     * @return true if the light casts shadows, false otherwise
     */
  virtual bool castsShadows() const = 0;

  /**
     * @brief Create a shadow ray from a point towards this light
     * @param point The starting point in world space
     * @return A ray starting at the point and pointing towards the light
     */
  virtual Ray getShadowRay(const Vector3D& point) const = 0;

  /**
     * @brief Clone this light
     * @return A new instance of this light
     */
  virtual std::shared_ptr<ILight> clone() const = 0;

  /**
     * @brief Get the position of the light source (if applicable)
     * @return The position of the light in world space
     */
  virtual Vector3D getPosition() const = 0;
};

}  // namespace RayTracer

#endif /* !ILIGHT_HPP_ */