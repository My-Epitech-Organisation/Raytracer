/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera class implementation
*/

#include "Camera.hpp"
#include <cmath>
#include <stdexcept>

namespace RayTracer {

Camera::Camera()
    : _position(0, 0, 0),
      _rotation(0, 0, 0),
      _width(800),
      _height(600),
      _fieldOfView(60.0) {
  updateTransform();
}

Camera::Camera(const Vector3D& position, int width, int height,
               double fieldOfView)
    : _position(position),
      _rotation(0, 0, 0),
      _width(width),
      _height(height),
      _fieldOfView(fieldOfView) {
  if (width <= 0 || height <= 0) {
    throw std::runtime_error("Camera resolution must be positive");
  }
  if (fieldOfView <= 0 || fieldOfView >= 180) {
    throw std::runtime_error("Field of view must be between 0 and 180 degrees");
  }
  updateTransform();
}

Camera::~Camera() {}

void Camera::setPosition(const Vector3D& position) {
  _position = position;
  updateTransform();
}

Vector3D Camera::getPosition() const {
  return _position;
}

void Camera::setRotation(const Vector3D& rotation) {
  _rotation = rotation;
  updateTransform();
}

Vector3D Camera::getRotation() const {
  return _rotation;
}

void Camera::setResolution(int width, int height) {
  if (width <= 0 || height <= 0) {
    throw std::runtime_error("Camera resolution must be positive");
  }
  _width = width;
  _height = height;
}

int Camera::getWidth() const {
  return _width;
}

int Camera::getHeight() const {
  return _height;
}

void Camera::setFieldOfView(double fov) {
  if (fov <= 0 || fov >= 180) {
    throw std::runtime_error("Field of view must be between 0 and 180 degrees");
  }
  _fieldOfView = fov;
}

double Camera::getFieldOfView() const {
  return _fieldOfView;
}

Ray Camera::generateRay(int x, int y) const {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    throw std::runtime_error("Pixel coordinates out of bounds");
  }

  double ndcX = (2.0 * x / (_width - 1)) - 1.0;
  double ndcY = 1.0 - (2.0 * y / (_height - 1));

  double aspectRatio = static_cast<double>(_width) / _height;

  double fovRadians = (_fieldOfView * M_PI) / 180.0;

  double tanHalfFov = tan(fovRadians / 2.0);

  Vector3D rayDirection(ndcX * aspectRatio * tanHalfFov, 1.0, ndcY * tanHalfFov);

  rayDirection = rayDirection.normalized();

  Vector3D worldDirection = _transform.applyToVector(rayDirection);

  Vector3D worldOrigin = _position;

  return Ray(worldOrigin, worldDirection);
}

void Camera::updateTransform() {
  _transform = Transform();

  Transform viewTransform;

  viewTransform.rotateX(-_rotation.getX());
  viewTransform.rotateY(-_rotation.getY());
  viewTransform.rotateZ(-_rotation.getZ());

  viewTransform.translate(-_position.getX(), -_position.getY(),
                          -_position.getZ());

  _transform = viewTransform;
}

}  // namespace RayTracer
