/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray class header
*/

#ifndef RAY_HPP_
#define RAY_HPP_

#include "Vector3D.hpp"

namespace RayTracer {

class Transform;

/**
 * @brief Represents a ray with an origin point and a direction vector
 */
class Ray {
 public:
  /**
   * @brief Default constructor
   * Creates a ray at origin (0,0,0) pointing towards positive Z direction
   */
  Ray();

  /**
   * @brief Constructor with origin and direction
   * @param origin The origin point of the ray
   * @param direction The direction vector of the ray (will be normalized)
   */
  Ray(const Vector3D& origin, const Vector3D& direction);

  /**
   * @brief Copy constructor
   * @param other The ray to copy
   */
  Ray(const Ray& other);

  /**
   * @brief Assignment operator
   * @param other The ray to assign from
   * @return Reference to this ray after assignment
   */
  Ray& operator=(const Ray& other);

  /**
   * @brief Destructor
   */
  ~Ray();

  /**
   * @brief Get the origin of the ray
   * @return The origin point
   */
  Vector3D getOrigin() const;

  /**
   * @brief Get the direction of the ray
   * @return The direction vector (normalized)
   */
  Vector3D getDirection() const;

  /**
   * @brief Set the origin of the ray
   * @param origin The new origin point
   */
  void setOrigin(const Vector3D& origin);

  /**
   * @brief Set the direction of the ray
   * @param direction The new direction vector (will be normalized)
   */
  void setDirection(const Vector3D& direction);

  /**
   * @brief Calculate a point along the ray at a specific distance
   * @param t The distance along the ray
   * @return The point at distance t from the origin along the ray direction
   */
  Vector3D pointAt(double t) const;

  /**
   * @brief Transform the ray using a transformation matrix
   * @param transform The transformation to apply
   * @return A new ray with the transformation applied
   */
  Ray transform(const Transform& transform) const;

 private:
  Vector3D _origin;     ///< The origin point of the ray
  Vector3D _direction;  ///< The normalized direction vector of the ray
};

}  // namespace RayTracer

#endif /* !RAY_HPP_ */
