/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Chessboard plane primitive header
*/

#ifndef CHESSBOARD_PLANE_HPP_
#define CHESSBOARD_PLANE_HPP_

#include <memory>
#include <optional>
#include "Plane.hpp"

namespace RayTracer {

/**
 * @brief Represents a plane with a chessboard pattern
 */
class ChessboardPlane : public Plane {
 public:
  /**
   * @brief Constructor for a chessboard plane
   * @param axis The axis the plane is perpendicular to ('X', 'Y', or 'Z')
   * @param position The position of the plane along the specified axis
   * @param color The primary color of the chessboard
   * @param alternateColor The alternate color for checkerboard pattern
   * @param checkSize The size of each square in the checkerboard pattern
   */
  explicit ChessboardPlane(char axis, double position, const Color& color,
                           const Color& alternateColor = Color::BLACK,
                           double checkSize = 10.0);

  /**
   * @brief Destructor
   */
  ~ChessboardPlane() override = default;

  /**
   * @brief Check if a ray intersects this plane
   * @param ray The ray to check
   * @return Intersection data if hit, std::nullopt otherwise
   */
  std::optional<Intersection> intersect(const Ray& ray) const override;

  /**
   * @brief Clone this chessboard plane
   * @return A new instance of this chessboard plane
   */
  std::shared_ptr<IPrimitive> clone() const override;

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

 private:
  /**
   * @brief Determine if a point on the plane is on a primary or alternate square
   * @param point The point to check
   * @return true if on a primary square, false if on an alternate square
   */
  bool isWhiteSquare(const Vector3D& point) const;

  Color _alternateColor;  ///< The alternate color for checkerboard pattern
  double _checkSize;      ///< Size of each square in the checkerboard pattern
};

}  // namespace RayTracer

#endif /* !CHESSBOARD_PLANE_HPP_ */
