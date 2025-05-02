/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera
*/

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "../core/Vector3D.hpp"
#include "Resolution.hpp"

namespace RayTracer {
class Camera {
 public:
  Camera(Resolution res, Vector3D pos, Vector3D rot, float fov)
      : _resolution(res), _position(pos), _rotation(rot), _fov(fov) {}

  const Resolution& getResolution() const { return _resolution; }
  const Vector3D& getPosition() const { return _position; }
  const Vector3D& getRotation() const { return _rotation; }
  float getFov() const { return _fov; }

  bool operator==(const Camera& other) const {
    return _resolution == other._resolution && _position == other._position &&
           _rotation == other._rotation && _fov == other._fov;
  }

 private:
  Resolution _resolution;
  Vector3D _position;
  Vector3D _rotation;
  float _fov;
};
}  // namespace RayTracer

#endif /* !CAMERA_HPP_ */
