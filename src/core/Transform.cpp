/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Transform class implementation
*/

#include "Transform.hpp"
#include <stdexcept>

namespace RayTracer {

Transform::Transform() : _matrix(), _inverseMatrix() {
  _matrix.setIdentity();
  _inverseMatrix.setIdentity();
}

Transform::Transform(const Transform& other)
    : _matrix(other._matrix), _inverseMatrix(other._inverseMatrix) {}

Transform& Transform::operator=(const Transform& other) {
  if (this != &other) {
    _matrix = other._matrix;
    _inverseMatrix = other._inverseMatrix;
  }
  return *this;
}

bool Transform::operator==(const Transform& other) const {
  return _matrix == other._matrix && _inverseMatrix == other._inverseMatrix;
}

Transform::~Transform() {}

Transform& Transform::translate(double x, double y, double z) {
  Matrix translationMatrix = Matrix::createTranslation(x, y, z);
  _matrix = translationMatrix * _matrix;
  updateInverseMatrix();
  return *this;
}

Transform& Transform::rotateX(double angleDegrees) {
  Matrix rotationMatrix = Matrix::createRotationX(angleDegrees);
  _matrix = rotationMatrix * _matrix;
  updateInverseMatrix();
  return *this;
}

Transform& Transform::rotateY(double angleDegrees) {
  Matrix rotationMatrix = Matrix::createRotationY(angleDegrees);
  _matrix = rotationMatrix * _matrix;
  updateInverseMatrix();
  return *this;
}

Transform& Transform::rotateZ(double angleDegrees) {
  Matrix rotationMatrix = Matrix::createRotationZ(angleDegrees);
  _matrix = rotationMatrix * _matrix;
  updateInverseMatrix();
  return *this;
}

Transform& Transform::scale(double x, double y, double z) {
  Matrix scaleMatrix = Matrix::createScaling(x, y, z);
  _matrix = scaleMatrix * _matrix;
  updateInverseMatrix();
  return *this;
}

Transform& Transform::scale(double factor) {
  return scale(factor, factor, factor);
}

Transform& Transform::combine(const Transform& other) {
  _matrix = other._matrix * _matrix;
  updateInverseMatrix();
  return *this;
}

Transform Transform::inverse() const {
  Transform result;
  result._matrix = _inverseMatrix;
  result._inverseMatrix = _matrix;
  return result;
}

Vector3D Transform::performHomogeneousDivision(double x, double y, double z,
                                               double w) const {
  if (std::abs(w - 1.0) > 1e-10 && std::abs(w) > 1e-10) {
    return Vector3D(x / w, y / w, z / w);
  }
  return Vector3D(x, y, z);
}

Vector3D Transform::applyToPoint(const Vector3D& point) const {
  double x = point.getX();
  double y = point.getY();
  double z = point.getZ();
  double w = 1.0;

  double newX = _matrix.at(0, 0) * x + _matrix.at(0, 1) * y +
                _matrix.at(0, 2) * z + _matrix.at(0, 3) * w;
  double newY = _matrix.at(1, 0) * x + _matrix.at(1, 1) * y +
                _matrix.at(1, 2) * z + _matrix.at(1, 3) * w;
  double newZ = _matrix.at(2, 0) * x + _matrix.at(2, 1) * y +
                _matrix.at(2, 2) * z + _matrix.at(2, 3) * w;
  double newW = _matrix.at(3, 0) * x + _matrix.at(3, 1) * y +
                _matrix.at(3, 2) * z + _matrix.at(3, 3) * w;

  return performHomogeneousDivision(newX, newY, newZ, newW);
}

Vector3D Transform::applyToVector(const Vector3D& vector) const {
  double x = vector.getX();
  double y = vector.getY();
  double z = vector.getZ();

  double newX =
      _matrix.at(0, 0) * x + _matrix.at(0, 1) * y + _matrix.at(0, 2) * z;
  double newY =
      _matrix.at(1, 0) * x + _matrix.at(1, 1) * y + _matrix.at(1, 2) * z;
  double newZ =
      _matrix.at(2, 0) * x + _matrix.at(2, 1) * y + _matrix.at(2, 2) * z;

  return Vector3D(newX, newY, newZ);
}

Matrix Transform::getMatrix() const {
  return _matrix;
}

Matrix Transform::getInverseMatrix() const {
  return _inverseMatrix;
}

void Transform::updateInverseMatrix() {
  try {
    _inverseMatrix = _matrix.inverse();
  } catch (const std::runtime_error& e) {
    throw std::runtime_error(
        std::string("Transform has become non-invertible: ") + e.what());
  }
}

}  // namespace RayTracer
