/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D class header
*/

#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <iostream>

namespace RayTracer {

/**
 * @brief Represents a 3D vector or point in space
 */
class Vector3D {
 public:
  /**
     * @brief Default constructor
     * Creates a vector at origin (0,0,0)
     */
  Vector3D();

  /**
     * @brief Constructor with x, y, z coordinates
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
  Vector3D(double x, double y, double z);

  /**
     * @brief Copy constructor
     * @param other The vector to copy
     */
  Vector3D(const Vector3D& other);

  /**
     * @brief Assignment operator
     * @param other The vector to assign from
     * @return Reference to this vector after assignment
     */
  Vector3D& operator=(const Vector3D& other);

  /**
    * @brief Equality operator
    * @param other The vector to compare with
    * @return True if vectors are equal
    */
  bool operator==(const Vector3D& other) const;

  /**
     * @brief Destructor
     */
  ~Vector3D();

  /**
     * @brief Get the X component
     * @return The X coordinate
     */
  double getX() const;

  /**
     * @brief Get the Y component
     * @return The Y coordinate
     */
  double getY() const;

  /**
     * @brief Get the Z component
     * @return The Z coordinate
     */
  double getZ() const;

  /**
     * @brief Set the X component
     * @param x The new X coordinate
     */
  void setX(double x);

  /**
     * @brief Set the Y component
     * @param y The new Y coordinate
     */
  void setY(double y);

  /**
     * @brief Set the Z component
     * @param z The new Z coordinate
     */
  void setZ(double z);

  /**
     * @brief Addition operator
     * @param other The vector to add
     * @return The resulting vector
     */
  Vector3D operator+(const Vector3D& other) const;

  /**
     * @brief Subtraction operator
     * @param other The vector to subtract
     * @return The resulting vector
     */
  Vector3D operator-(const Vector3D& other) const;

  /**
     * @brief Scalar multiplication operator
     * @param scalar The scalar value to multiply by
     * @return The resulting vector
     */
  Vector3D operator*(double scalar) const;

  /**
     * @brief Scalar division operator
     * @param scalar The scalar value to divide by
     * @return The resulting vector
     * @throws std::runtime_error if scalar is zero
     */
  Vector3D operator/(double scalar) const;

  /**
     * @brief Negation operator
     * @return The negated vector
     */
  Vector3D operator-() const;

  /**
     * @brief Dot product
     * @param other The vector to dot product with
     * @return The dot product result
     */
  double dot(const Vector3D& other) const;

  /**
     * @brief Cross product
     * @param other The vector to cross product with
     * @return The cross product result
     */
  Vector3D cross(const Vector3D& other) const;

  /**
     * @brief Get the magnitude (length) of the vector
     * @return The magnitude
     */
  double getMagnitude() const;

  /**
     * @brief Get the squared magnitude of the vector
     * @return The squared magnitude
     */
  double getSquaredMagnitude() const;

  /**
     * @brief Normalize the vector
     * @return A normalized copy of this vector
     * @throws std::runtime_error if the vector has zero magnitude
     */
  Vector3D normalized() const;

  /**
     * @brief Check if two vectors are approximately equal
     * @param other The vector to compare with
     * @param epsilon The maximum allowed difference
     * @return True if vectors are approximately equal
     */
  bool isEqual(const Vector3D& other, double epsilon = 1e-10) const;

 private:
  double _x;  ///< The X coordinate
  double _y;  ///< The Y coordinate
  double _z;  ///< The Z coordinate
};

/**
 * @brief Output stream operator for Vector3D
 * @param os The output stream
 * @param vector The vector to output
 * @return The modified output stream
 */
std::ostream& operator<<(std::ostream& os, const Vector3D& vector);

}  // namespace RayTracer

#endif /* !VECTOR3D_HPP_ */
