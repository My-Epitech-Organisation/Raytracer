/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D class implementation
*/

#include "Vector3D.hpp"
#include <cmath>
#include <stdexcept>

namespace RayTracer {

Vector3D::Vector3D() : _x(0.0), _y(0.0), _z(0.0) {
  // Default constructor initializes vector to (0,0,0)
}

Vector3D::Vector3D(double x, double y, double z) : _x(x), _y(y), _z(z) {
  // Constructor with coordinates
}

Vector3D::Vector3D(const Vector3D& other)
    : _x(other._x), _y(other._y), _z(other._z) {
  // Copy constructor
}

Vector3D& Vector3D::operator=(const Vector3D& other) {
  if (this != &other) {
    _x = other._x;
    _y = other._y;
    _z = other._z;
  }
  return *this;
}

Vector3D::~Vector3D() {
  // Destructor (nothing to clean up for simple POD types)
}

double Vector3D::getX() const {
  return _x;
}

double Vector3D::getY() const {
  return _y;
}

double Vector3D::getZ() const {
  return _z;
}

void Vector3D::setX(double x) {
  _x = x;
}

void Vector3D::setY(double y) {
  _y = y;
}

void Vector3D::setZ(double z) {
  _z = z;
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
  return Vector3D(_x + other._x, _y + other._y, _z + other._z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
  return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}

Vector3D Vector3D::operator*(double scalar) const {
  return Vector3D(_x * scalar, _y * scalar, _z * scalar);
}

Vector3D Vector3D::operator/(double scalar) const {
  if (std::abs(scalar) < 1e-10) {
    throw std::runtime_error("Division by zero or near-zero in Vector3D");
  }
  return Vector3D(_x / scalar, _y / scalar, _z / scalar);
}

Vector3D Vector3D::operator-() const {
  return Vector3D(-_x, -_y, -_z);
}

double Vector3D::dot(const Vector3D& other) const {
  return _x * other._x + _y * other._y + _z * other._z;
}

Vector3D Vector3D::cross(const Vector3D& other) const {
  return Vector3D(_y * other._z - _z * other._y, _z * other._x - _x * other._z,
                  _x * other._y - _y * other._x);
}

double Vector3D::getSquaredMagnitude() const {
  return _x * _x + _y * _y + _z * _z;
}

double Vector3D::getMagnitude() const {
  return std::sqrt(getSquaredMagnitude());
}

Vector3D Vector3D::normalized() const {
  double magnitude = getMagnitude();

  if (magnitude < 1e-10) {
    throw std::runtime_error("Cannot normalize a vector with zero magnitude");
  }

  return *this / magnitude;
}

bool Vector3D::isEqual(const Vector3D& other, double epsilon) const {
  return (std::abs(_x - other._x) < epsilon &&
          std::abs(_y - other._y) < epsilon &&
          std::abs(_z - other._z) < epsilon);
}

std::ostream& operator<<(std::ostream& os, const Vector3D& vector) {
  os << "Vector3D(" << vector.getX() << ", " << vector.getY() << ", "
     << vector.getZ() << ")";
  return os;
}

}  // namespace RayTracer