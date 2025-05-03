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
  double ndcX = (2.0 * x / (_width - 1)) - 1.0;  // Exact corner values
  double ndcY =
      1.0 -
      (2.0 * y /
       (_height - 1));  // Flip Y to match image coordinates (0,0 at top-left)

  // Calculate the aspect ratio to maintain proper perspective
  double aspectRatio = static_cast<double>(_width) / _height;

  // Calculate the camera's field of view in radians
  double fovRadians = (_fieldOfView * M_PI) / 180.0;

  // Calculate the camera plane distance from the camera position
  double tanHalfFov = tan(fovRadians / 2.0);

  // Calculate the ray direction in camera space
  Vector3D rayDirection(ndcX * aspectRatio * tanHalfFov, ndcY * tanHalfFov,
                        -1.0);

  // Normalize the direction
  rayDirection = rayDirection.normalized();

  // Apply only the rotation part of the transform to the direction
  Vector3D worldDirection = _transform.applyToVector(rayDirection);

  // The ray origin is simply the camera position
  Vector3D worldOrigin = _position;

  // Create the ray directly in world space
  return Ray(worldOrigin, worldDirection);
}

void Camera::updateTransform() {
  // Pour une caméra, nous avons besoin de créer une matrice de transformation view-to-world

  // Commencer par une transformation identité
  _transform = Transform();

  // L'ordre et la direction des rotations sont critiques
  // Pour une caméra avec rotation (φ, θ, ψ) nous devons appliquer
  // les rotations inverses (-φ, -θ, -ψ) dans l'ordre inverse (Z, Y, X)

  // Calculer la transformation de la vue (view transform)
  Transform viewTransform;

  // Appliquer les rotations en ordre inverse (Z, Y, X) et avec les angles inversés
  viewTransform.rotateX(-_rotation.getX());
  viewTransform.rotateY(-_rotation.getY());
  viewTransform.rotateZ(-_rotation.getZ());

  // Appliquer la translation inverse (-position)
  viewTransform.translate(-_position.getX(), -_position.getY(),
                          -_position.getZ());

  // La transformation de la caméra est la même que la transformation de la vue
  // puisque nous transformons de l'espace caméra vers l'espace monde
  _transform = viewTransform;
}

}  // namespace RayTracer
