/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Checkerboard plane primitive header
*/

/**
 * @file CheckerboardPlane.hpp
 * @brief Definition of the CheckerboardPlane class for representing infinite
 * planes with a checkerboard pattern
 * @author paul-antoine.salmon@epitech.eu
 * @date 2025-05-18
 * @version 1.0
 */

#ifndef CHECKERBOARD_PLANE_HPP_
#define CHECKERBOARD_PLANE_HPP_

#include <memory>
#include <optional>
#include "../../../include/IPrimitive.hpp"
#include "../../core/Color.hpp"
#include "../../core/Ray.hpp"
#include "../../core/Vector3D.hpp"
#include "Plane.hpp"

namespace RayTracer {

/**
 * @brief Represents a plane with a checkerboard pattern
 */
class CheckerboardPlane : public Plane {
 public:
  /**
   * @brief Constructor for a checkerboard plane
   * @param axis The axis the plane is perpendicular to ('X', 'Y', or 'Z')
   * @param position The position of the plane along the specified axis
   * @param color The primary color of the checkerboard
   * @param alternateColor The alternate color of the checkerboard
   * @param squareSize The size of each checkerboard square
   */
  CheckerboardPlane(char axis, double position, const Color& color,
                    const Color& alternateColor, double squareSize);

  /**
   * @brief Destructor
   */
  ~CheckerboardPlane() override = default;

  /**
   * @brief Check if a ray intersects this plane
   * @param ray The ray to check
   * @return Intersection data if hit, std::nullopt otherwise
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Clone this plane
   * @return A new instance of this plane
   */
  std::shared_ptr<IPrimitive> clone() const override;

  /**
   * @brief Get the alternate color of the checkerboard
   * @return The alternate color
   */
  Color getAlternateColor() const;

  /**
   * @brief Set the alternate color of the checkerboard
   * @param color The color to set
   */
  void setAlternateColor(const Color& color);

  /**
   * @brief Get the size of each square in the checkerboard
   * @return The square size
   */
  double getSquareSize() const;

  /**
   * @brief Set the size of each square in the checkerboard
   * @param size The size to set
   */
  void setSquareSize(double size);

 private:
  /**
   * @brief Determine the color at a point on the checkerboard
   * @param point The point in local space
   * @return The color at the point
   */
  Color getColorAtPoint(const Vector3D& point) const;

  Color _alternateColor;  ///< The alternate color of the checkerboard
  double _squareSize;     ///< The size of each square in the checkerboard
};

}  // namespace RayTracer

#endif /* !CHECKERBOARD_PLANE_HPP_ */
