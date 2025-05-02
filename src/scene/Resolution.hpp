/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Resolution
*/

#ifndef RESOLUTION_HPP_
#define RESOLUTION_HPP_

class Resolution {
 public:
  Resolution(int width, int height) : _width(width), _height(height) {}

  int getWidth() const { return _width; }
  int getHeight() const { return _height; }

  bool operator==(const Resolution& other) const {
    return _width == other._width && _height == other._height;
  }

 private:
  int _width;
  int _height;
};

#endif /* !RESOLUTION_HPP_ */
