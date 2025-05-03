/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Matrix class implementation
*/

#include "Matrix.hpp"
#include <cmath>
#include <stdexcept>

namespace RayTracer {

Matrix::Matrix() {
  setIdentity();
}

Matrix::Matrix(const std::array<double, 16>& values) : _data(values) {}

Matrix::Matrix(const Matrix& other) : _data(other._data) {}

Matrix& Matrix::operator=(const Matrix& other) {
  if (this != &other) {
    _data = other._data;
  }
  return *this;
}

Matrix::~Matrix() {}

double& Matrix::at(int row, int col) {
  if (row < 0 || row > 3 || col < 0 || col > 3) {
    throw std::out_of_range("Matrix indices out of range");
  }
  return _data[row * 4 + col];
}

const double& Matrix::at(int row, int col) const {
  if (row < 0 || row > 3 || col < 0 || col > 3) {
    throw std::out_of_range("Matrix indices out of range");
  }
  return _data[row * 4 + col];
}

Matrix& Matrix::setIdentity() {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      at(row, col) = (row == col) ? 1.0 : 0.0;
    }
  }
  return *this;
}

Matrix Matrix::operator*(const Matrix& other) const {
  Matrix result;

  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      double sum = 0.0;
      for (int k = 0; k < 4; ++k) {
        sum += at(row, k) * other.at(k, col);
      }
      result.at(row, col) = sum;
    }
  }

  return result;
}

Matrix& Matrix::operator*=(const Matrix& other) {
  *this = *this * other;
  return *this;
}

bool Matrix::operator==(const Matrix& other) const {
  for (int i = 0; i < 16; ++i) {
    if (std::abs(_data[i] - other._data[i]) > 1e-10) {
      return false;
    }
  }
  return true;
}

double Matrix::minor(int row, int col) const {
  double submatrix[9];
  int index = 0;

  for (int r = 0; r < 4; ++r) {
    if (r == row)
      continue;

    for (int c = 0; c < 4; ++c) {
      if (c == col)
        continue;

      submatrix[index++] = at(r, c);
    }
  }

  return submatrix[0] *
             (submatrix[4] * submatrix[8] - submatrix[5] * submatrix[7]) -
         submatrix[1] *
             (submatrix[3] * submatrix[8] - submatrix[5] * submatrix[6]) +
         submatrix[2] *
             (submatrix[3] * submatrix[7] - submatrix[4] * submatrix[6]);
}

double Matrix::cofactor(int row, int col) const {
  double minorValue = minor(row, col);
  return ((row + col) % 2 == 0) ? minorValue : -minorValue;
}

double Matrix::determinant() const {
  double det = 0.0;

  for (int col = 0; col < 4; ++col) {
    det += at(0, col) * cofactor(0, col);
  }

  return det;
}

Matrix Matrix::inverse() const {
  double det = determinant();

  if (std::abs(det) < 1e-10) {
    throw std::runtime_error(
        "Matrix is not invertible (determinant is zero or near-zero)");
  }

  Matrix result;

  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      result.at(col, row) = cofactor(row, col) / det;
    }
  }

  return result;
}

Matrix Matrix::createTranslation(double x, double y, double z) {
  Matrix result;

  result.at(0, 3) = x;
  result.at(1, 3) = y;
  result.at(2, 3) = z;

  return result;
}

Matrix Matrix::createRotationX(double angleDegrees) {
  double angleRadians = angleDegrees * M_PI / 180.0;
  double cosAngle = std::cos(angleRadians);
  double sinAngle = std::sin(angleRadians);

  Matrix result;

  result.at(1, 1) = cosAngle;
  result.at(1, 2) = -sinAngle;
  result.at(2, 1) = sinAngle;
  result.at(2, 2) = cosAngle;

  return result;
}

Matrix Matrix::createRotationY(double angleDegrees) {
  double angleRadians = angleDegrees * M_PI / 180.0;
  double cosAngle = std::cos(angleRadians);
  double sinAngle = std::sin(angleRadians);

  Matrix result;

  result.at(0, 0) = cosAngle;
  result.at(0, 2) = sinAngle;
  result.at(2, 0) = -sinAngle;
  result.at(2, 2) = cosAngle;

  return result;
}

Matrix Matrix::createRotationZ(double angleDegrees) {
  double angleRadians = angleDegrees * M_PI / 180.0;
  double cosAngle = std::cos(angleRadians);
  double sinAngle = std::sin(angleRadians);

  Matrix result;

  result.at(0, 0) = cosAngle;
  result.at(0, 1) = -sinAngle;
  result.at(1, 0) = sinAngle;
  result.at(1, 1) = cosAngle;

  return result;
}

Matrix Matrix::createScaling(double x, double y, double z) {
  Matrix result;

  result.at(0, 0) = x;
  result.at(1, 1) = y;
  result.at(2, 2) = z;

  return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
  for (int row = 0; row < 4; ++row) {
    os << "| ";
    for (int col = 0; col < 4; ++col) {
      os << matrix.at(row, col) << " ";
    }
    os << "|" << std::endl;
  }
  return os;
}

}  // namespace RayTracer
