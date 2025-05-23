/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Resolution
*/

/**
 * @file Resolution.cpp
 * @brief Implementation of the Resolution class which handles image dimensions
 * @author @santiago
 * @date 2025-05-16
 * @version 1.0
 */

#include "Resolution.hpp"
#include "../../include/exceptions/InvalidTypeException.hpp"

namespace RayTracer {

Resolution::Resolution() : _width(0), _height(0) {}

Resolution::Resolution(int width, int height) : _width(width), _height(height) {
  if (width <= 0 || height <= 0) {
    throw InvalidTypeException("Resolution must be positive");
  }
}

int Resolution::getWidth() const {
  return _width;
}

int Resolution::getHeight() const {
  return _height;
}

bool Resolution::operator==(const Resolution& other) const {
  return _width == other._width && _height == other._height;
}

}  // namespace RayTracer
