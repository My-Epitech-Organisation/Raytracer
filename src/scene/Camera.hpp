/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera class header
*/

/**
 * @file Camera.hpp
 * @brief Defines the Camera class for scene viewing and ray generation
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "../core/Ray.hpp"
#include "../core/Transform.hpp"
#include "../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Camera class for ray generation
 *
 * This class handles the creation of primary rays for each pixel in the
 * rendered image based on the camera's position, rotation, and field of view.
 */
class Camera {
 public:
  /**
   * @brief Default constructor
   * Creates a camera at origin (0,0,0) looking along negative Z with default
   * parameters
   */
  Camera();

  /**
   * @brief Constructor with parameters
   * @param position The position of the camera in world space
   * @param resolution The resolution of the output image (width, height)
   * @param fieldOfView The vertical field of view in degrees
   */
  Camera(const Vector3D& position, int width, int height, double fieldOfView);

  /**
   * @brief Destructor
   */
  ~Camera();

  /**
   * @brief Set the position of the camera
   * @param position The new position in world space
   */
  void setPosition(const Vector3D& position);

  /**
   * @brief Get the position of the camera
   * @return The current position
   */
  Vector3D getPosition() const;

  /**
   * @brief Set the rotation of the camera
   * @param rotation The rotation angles in degrees (x, y, z)
   */
  void setRotation(const Vector3D& rotation);

  /**
   * @brief Get the rotation of the camera
   * @return The current rotation
   */
  Vector3D getRotation() const;

  /**
   * @brief Set the resolution of the output image
   * @param width The width in pixels
   * @param height The height in pixels
   */
  void setResolution(int width, int height);

  /**
   * @brief Get the width of the output image
   * @return The width in pixels
   */
  int getWidth() const;

  /**
   * @brief Get the height of the output image
   * @return The height in pixels
   */
  int getHeight() const;

  /**
   * @brief Set the field of view
   * @param fov The vertical field of view in degrees
   */
  void setFieldOfView(double fov);

  /**
   * @brief Get the field of view
   * @return The vertical field of view in degrees
   */
  double getFieldOfView() const;

  /**
   * @brief Generate a ray for a specific pixel
   * @param x The x-coordinate of the pixel (0 is left)
   * @param y The y-coordinate of the pixel (0 is top)
   * @return The ray passing through that pixel
   */
  Ray generateRay(int x, int y) const;

 private:
  Vector3D _position;    ///< Camera position in world space
  Vector3D _rotation;    ///< Camera rotation in degrees (x, y, z)
  int _width;            ///< Width of the output image in pixels
  int _height;           ///< Height of the output image in pixels
  double _fieldOfView;   ///< Vertical field of view in degrees
  Transform _transform;  ///< Camera transformation matrix

  /**
   * @brief Update the transformation matrix based on position and rotation
   */
  void updateTransform();
};

}  // namespace RayTracer

#endif /* !CAMERA_HPP_ */
