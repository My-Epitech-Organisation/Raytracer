/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Matrix class header
*/

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <array>
#include <iostream>
#include "./Vector3D.hpp" // Add this include

namespace RayTracer {

/**
 * @brief 4x4 matrix class for 3D transformations
 */
class Matrix {
 public:
  /**
   * @brief Default constructor
   * Creates an identity matrix
   */
  Matrix();

  /**
   * @brief Constructor with array initialization
   * @param values 16 values to initialize the matrix in row-major order
   */
  Matrix(const std::array<double, 16>& values);

  /**
   * @brief Copy constructor
   * @param other The matrix to copy
   */
  Matrix(const Matrix& other);

  /**
   * @brief Assignment operator
   * @param other The matrix to assign from
   * @return Reference to this matrix
   */
  Matrix& operator=(const Matrix& other);

  /**
   * @brief Equality operator
   * @param other The matrix to compare with
   * @return true if the matrices are equal, false otherwise
   */
  bool operator==(const Matrix& other) const;

  /**
   * @brief Destructor
   */
  ~Matrix();

  /**
   * @brief Access element at row, col
   * @param row Row index (0-3)
   * @param col Column index (0-3)
   * @return Reference to the element
   * @throws std::out_of_range if indices are invalid
   */
  double& at(int row, int col);

  /**
   * @brief Access element at row, col (const version)
   * @param row Row index (0-3)
   * @param col Column index (0-3)
   * @return Const reference to the element
   * @throws std::out_of_range if indices are invalid
   */
  const double& at(int row, int col) const;

  /**
   * @brief Set matrix to identity
   * @return Reference to this matrix
   */
  Matrix& setIdentity();

  /**
   * @brief Matrix multiplication
   * @param other The matrix to multiply with
   * @return The resulting matrix
   */
  Matrix operator*(const Matrix& other) const;

  /**
   * @brief In-place matrix multiplication
   * @param other The matrix to multiply with
   * @return Reference to this matrix
   */
  Matrix& operator*=(const Matrix& other);

  /**
   * @brief Calculate determinant of the matrix
   * @return The determinant value
   */
  double determinant() const;

  /**
   * @brief Calculate inverse of the matrix
   * @return The inverse matrix
   * @throws std::runtime_error if matrix is not invertible
   */
  Matrix inverse() const;

  /**
   * @brief Create a translation matrix
   * @param x Translation along x-axis
   * @param y Translation along y-axis
   * @param z Translation along z-axis
   * @return The translation matrix
   */
  static Matrix createTranslation(double x, double y, double z);

  /**
   * @brief Create a rotation matrix around X axis
   * @param angleDegrees Angle in degrees
   * @return The rotation matrix
   */
  static Matrix createRotationX(double angleDegrees);

  /**
   * @brief Create a rotation matrix around Y axis
   * @param angleDegrees Angle in degrees
   * @return The rotation matrix
   */
  static Matrix createRotationY(double angleDegrees);

  /**
   * @brief Create a rotation matrix around Z axis
   * @param angleDegrees Angle in degrees
   * @return The rotation matrix
   */
  static Matrix createRotationZ(double angleDegrees);

  /**
   * @brief Create a scaling matrix
   * @param x Scale factor along x-axis
   * @param y Scale factor along y-axis
   * @param z Scale factor along z-axis
   * @return The scaling matrix
   */
  static Matrix createScaling(double x, double y, double z);

  /**
   * @brief Get the translation components from the matrix
   * @return A Vector3D representing the translation (x, y, z)
   */
  Vector3D getTranslation() const;

 private:
  std::array<double, 16> _data;  ///< Matrix data in row-major order

  /**
   * @brief Calculate the minor (determinant of 3x3 submatrix)
   * @param row Row to exclude
   * @param col Column to exclude
   * @return The minor value
   */
  double minor(int row, int col) const;

  /**
   * @brief Calculate the cofactor
   * @param row Row of the cofactor
   * @param col Column of the cofactor
   * @return The cofactor value
   */
  double cofactor(int row, int col) const;
};

/**
 * @brief Output stream operator for Matrix
 * @param os The output stream
 * @param matrix The matrix to output
 * @return The modified output stream
 */
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

}  // namespace RayTracer

#endif /* !MATRIX_HPP_ */
