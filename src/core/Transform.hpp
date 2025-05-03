/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Transform class header
*/

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "Matrix.hpp"
#include "Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Represents a 3D transformation (translation, rotation, scale)
 */
class Transform {
 public:
  /**
     * @brief Default constructor
     * Creates an identity transformation
     */
  Transform();

  /**
     * @brief Copy constructor
     * @param other The transformation to copy
     */
  Transform(const Transform& other);

  /**
     * @brief Assignment operator
     * @param other The transformation to assign from
     * @return Reference to this transformation after assignment
     */
  Transform& operator=(const Transform& other);

  /**
     * @brief Equality operator
     * @param other The transformation to compare with
     * @return true if the transformations are equal, false otherwise
     */
  bool operator==(const Transform& other) const;

  /**
     * @brief Destructor
     */
  ~Transform();

  /**
     * @brief Apply a translation
     * @param x Translation along x-axis
     * @param y Translation along y-axis
     * @param z Translation along z-axis
     * @return Reference to this transform
     */
  Transform& translate(double x, double y, double z);

  /**
     * @brief Apply a rotation around the X axis
     * @param angleDegrees Angle in degrees
     * @return Reference to this transform
     */
  Transform& rotateX(double angleDegrees);

  /**
     * @brief Apply a rotation around the Y axis
     * @param angleDegrees Angle in degrees
     * @return Reference to this transform
     */
  Transform& rotateY(double angleDegrees);

  /**
     * @brief Apply a rotation around the Z axis
     * @param angleDegrees Angle in degrees
     * @return Reference to this transform
     */
  Transform& rotateZ(double angleDegrees);

  /**
     * @brief Apply scaling
     * @param x Scale factor along x-axis
     * @param y Scale factor along y-axis
     * @param z Scale factor along z-axis
     * @return Reference to this transform
     */
  Transform& scale(double x, double y, double z);

  /**
     * @brief Apply uniform scaling
     * @param factor Scale factor for all axes
     * @return Reference to this transform
     */
  Transform& scale(double factor);

  /**
     * @brief Combine this transform with another
     * @param other The transform to combine with
     * @return Reference to this transform
     */
  Transform& combine(const Transform& other);

  /**
     * @brief Get inverse of this transformation
     * This method returns a new Transform object where the main matrix (_matrix)
     * and the inverse matrix (_inverseMatrix) are swapped. This allows the
     * resulting Transform to represent the inverse transformation efficiently.
     *
     * Note that this method does not perform a mathematical inversion of the matrix;
     * it relies on the assumption that the inverse matrix is already maintained
     * correctly during transformations via the updateInverseMatrix() method.
     *
     * @return A new Transform representing the inverse of this transformation
     * @throws std::runtime_error if the matrix cannot be inverted
     */
  Transform inverse() const;

  /**
     * @brief Apply transformation to a 3D point
     * @param point The point to transform
     * @return The transformed point
     */
  Vector3D applyToPoint(const Vector3D& point) const;

  /**
     * @brief Apply transformation to a 3D vector (direction)
     * @param vector The vector to transform
     * @return The transformed vector
     */
  Vector3D applyToVector(const Vector3D& vector) const;

  /**
     * @brief Get the transformation matrix
     * @return The 4x4 transformation matrix
     */
  Matrix getMatrix() const;

  /**
     * @brief Get the inverse transformation matrix
     * @return The 4x4 inverse transformation matrix
     */
  Matrix getInverseMatrix() const;

 private:
  Matrix _matrix;         ///< 4x4 transformation matrix
  Matrix _inverseMatrix;  ///< 4x4 inverse transformation matrix

  /**
   * @brief Update the inverse matrix
   * Called after any operation that modifies the main matrix
   */
  void updateInverseMatrix();

  /**
   * @brief Performs homogeneous coordinate division if needed
   *
   * This helper method handles perspective division for homogeneous coordinates
   * when the w component is not 1.0 (non-affine transformations).
   *
   * @param x X coordinate to normalize
   * @param y Y coordinate to normalize
   * @param z Z coordinate to normalize
   * @param w W coordinate (homogeneous component)
   * @return Vector3D The normalized 3D point
   */
  Vector3D performHomogeneousDivision(double x, double y, double z,
                                      double w) const;
};

}  // namespace RayTracer

#endif /* !TRANSFORM_HPP_ */
