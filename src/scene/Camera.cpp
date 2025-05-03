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
  // Ensure pixel coordinates are within bounds
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    throw std::runtime_error("Pixel coordinates out of bounds");
  }

  // Calculate normalized device coordinates (NDC)
  // Convert pixel coordinates to range [-1, 1]
  // For corner rays, we need exact values at the pixel centers
  double ndcX = (2.0 * (x + 0.5) / _width) - 1.0;
  double ndcY = 1.0 - (2.0 * (y + 0.5) / _height); // Flip Y to match image coordinates (0,0 at top-left)

  // Calculate the aspect ratio to maintain proper perspective
  double aspectRatio = static_cast<double>(_width) / _height;

  // Calculate the camera's field of view in radians
  double fovRadians = (_fieldOfView * M_PI) / 180.0;
  
  // Calculate the camera plane distance from the camera position
  // (assuming the camera points along negative Z and the plane is at Z=-1)
  double tanHalfFov = tan(fovRadians / 2.0);

  // Calculate the ray direction in camera space
  // The ray originates at the camera position and passes through the pixel
  // on the virtual screen at z = -1
  Vector3D rayDirection(
      ndcX * aspectRatio * tanHalfFov,
      ndcY * tanHalfFov,
      -1.0);

  // Create the ray in camera space
  Ray ray(Vector3D(0, 0, 0), rayDirection);

  // Transform the ray to world space using the camera transform
  return ray.transform(_transform);
}

void Camera::updateTransform() {
  // Start with identity transform
  _transform = Transform();

  // Apply rotation (in the order X, Y, Z)
  _transform.rotateX(_rotation.getX());
  _transform.rotateY(_rotation.getY());
  _transform.rotateZ(_rotation.getZ());
  
  // Apply translation
  _transform.translate(_position.getX(), _position.getY(), _position.getZ());
}

}  // namespace RayTracer
