/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane primitive header
*/

/**
 * @file Plane.hpp
 * @brief Definition of the Plane class for representing infinite planes in the raytracing engine
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef PLANE_HPP_
#define PLANE_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Transform.hpp"
#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Epsilon value for floating point comparisons
 */
constexpr double EPSILON = 1e-6;
/**
 * @brief Enum representing the axis of the plane
 */
enum class Axis { X, Y, Z };

/**
 * @brief Represents an infinite plane primitive
 */
class Plane : public IPrimitive {
 public:
  /**
   * @brief Constructor for an axis-aligned plane
   * @param axis The axis the plane is perpendicular to ('X', 'Y', or 'Z')
   * @param position The position of the plane along the specified axis
   * @param color The color of the plane
   * @param alternateColor The alternate color for checkerboard pattern (default
   * black)
   * @param checkSize The size of each square in the checkerboard pattern
   * (default 10.0)
   */
  explicit Plane(char axis, double position, const Color& color,
                 const Color& alternateColor = Color::BLACK,
                 double checkSize = 10.0);

  /**
   * @brief Destructor
   */
  ~Plane() override = default;

  /**
   * @brief Check if a ray intersects this plane
   * @param ray The ray to check
   * @return Intersection data if hit, std::nullopt otherwise
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Set the transformation matrix for this plane
   * @param transform The transformation to apply
   */
  void setTransform(const Transform& transform) override;

  /**
   * @brief Get the transformation of this plane
   * @return The current transformation
   */
  Transform getTransform() const override;

  /**
   * @brief Set the color of this plane
   * @param color The color to set
   */
  void setColor(const Color& color) override;

  /**
   * @brief Get the color of this plane
   * @return The current color
   */
  Color getColor() const override;

  /**
   * @brief Get the normal at a specific point on the plane
   * @param point The point in world space
   * @return The normal vector of the plane
   */
  Vector3D getNormalAt(const Vector3D& point) const override;

  /**
   * @brief Clone this plane
   * @return A new instance of this plane
   */
  std::shared_ptr<IPrimitive> clone() const override;

  /**
   * @brief Get the axis of the plane
   * @return The axis the plane is aligned with
   */
  Axis getAxis() const;

  /**
   * @brief Get the position of the plane along the axis
   * @return The position of the plane
   */
  double getPosition() const;

  /**
   * @brief Get the normal vector of the plane
   * @return The normal vector of the plane
   */
  Vector3D getNormal() const;

  /**
   * @brief Set the alternating color for checkerboard pattern
   * @param color The color to set
   */
  void setAlternateColor(const Color& color);

  /**
   * @brief Get the alternating color for checkerboard pattern
   * @return The current alternate color
   */
  Color getAlternateColor() const;

  /**
   * @brief Set the size of each square in the checkerboard pattern
   * @param size The size of each square
   */
  void setCheckSize(double size);

  /**
   * @brief Get the size of each square in the checkerboard pattern
   * @return The current check size
   */
  double getCheckSize() const;

  /**
   * @brief Determine if a point on the plane is on a black or white square
   * @param point The point to check
   * @return true if on a white square, false if on a black square
   */
  bool isWhiteSquare(const Vector3D& point) const;

 private:
  Axis getAxisFromChar(char axis) const;
  char getCharFromAxis(Axis axis) const;
  Vector3D _normal;      ///< The normal vector of the plane (derived from axis)
  double _position;      ///< Position along the axis
  Axis _axis;            ///< Axis the plane is perpendicular to (X, Y, Z)
  Color _color;          ///< The color of the plane
  Transform _transform;  ///< Transformation applied to the plane
  Transform _inverseTransform;  ///< Inverse transformation
  Color _alternateColor;  ///< The alternate color for checkerboard pattern
  double _checkSize;      ///< Size of each square in the checkerboard pattern
};

}  // namespace RayTracer

#endif /* !PLANE_HPP_ */
