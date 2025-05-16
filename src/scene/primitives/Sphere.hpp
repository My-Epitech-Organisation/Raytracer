/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere primitive class header
*/

/**
 * @file Sphere.hpp
 * @brief Definition of the Sphere class for representing spheres in the raytracing engine
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Represents a sphere primitive
 */
class Sphere : public IPrimitive {
 public:
  /**
   * @brief Default constructor
   * Creates a unit sphere at origin (0,0,0) with radius 1
   */
  Sphere();

  /**
   * @brief Constructor with center and radius
   * @param center The center position of the sphere
   * @param radius The radius of the sphere
   * @param color The color of the sphere
   */
  Sphere(const Vector3D& center, double radius, const Color& color);

  /**
   * @brief Copy constructor
   * @param other The sphere to copy
   */
  Sphere(const Sphere& other);

  /**
   * @brief Assignment operator
   * @param other The sphere to assign from
   * @return Reference to this sphere after assignment
   */
  Sphere& operator=(const Sphere& other);

  /**
   * @brief Destructor
   */
  ~Sphere() override;

  /**
   * @brief Check if a ray intersects this sphere
   * @param ray The ray to check
   * @return Intersection data if hit, std::nullopt otherwise
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Set the transformation matrix for this sphere
   * @param transform The transformation to apply
   */
  void setTransform(const Transform& transform) override;

  /**
   * @brief Get the transformation of this sphere
   * @return The current transformation
   */
  Transform getTransform() const override;

  /**
   * @brief Set the color of this sphere
   * @param color The color to set
   */
  void setColor(const Color& color) override;

  /**
   * @brief Get the color of this sphere
   * @return The current color
   */
  Color getColor() const override;

  /**
   * @brief Get the normal at a specific point on the sphere
   * @param point The point in world space
   * @return The normal vector at that point
   */
  Vector3D getNormalAt(const Vector3D& point) const override;

  /**
   * @brief Clone this sphere
   * @return A new instance of this sphere
   */
  std::shared_ptr<IPrimitive> clone() const override;

  /**
   * @brief Set the center of the sphere
   * @param center The new center position
   */
  void setCenter(const Vector3D& center);

  /**
   * @brief Get the center of the sphere
   * @return The current center position
   */
  Vector3D getCenter() const;

  /**
   * @brief Set the radius of the sphere
   * @param radius The new radius
   */
  void setRadius(double radius);

  /**
   * @brief Get the radius of the sphere
   * @return The current radius
   */
  double getRadius() const;

 private:
  Vector3D _center;      ///< The center position of the sphere
  double _radius;        ///< The radius of the sphere
  Color _color;          ///< The color of the sphere
  Transform _transform;  ///< The transformation applied to the sphere
};

}  // namespace RayTracer

#endif /* !SPHERE_HPP_ */
