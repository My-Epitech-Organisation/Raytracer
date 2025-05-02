/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "../../core/Color.hpp"

namespace RayTracer {
class Plane {
 public:
  Plane(char axis, float position, Color color)
      : _axis(axis), _position(position), _color(color) {}

  char getAxis() const { return _axis; }
  float getPosition() const { return _position; }
  const Color& getColor() const { return _color; }

  bool operator==(const Plane& other) const {
    return _axis == other._axis && _position == other._position &&
           _color == other._color;
  }

 private:
  char _axis;
  float _position;
  Color _color;
};
}  // namespace RayTracer

#endif /* !PLANE_HPP_ */
