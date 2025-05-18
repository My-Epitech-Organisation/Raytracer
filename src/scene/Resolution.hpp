/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Resolution
*/

/**
 * @file Resolution.hpp
 * @brief Defines the Resolution class for managing image dimensions in the ray
 * tracer
 * @author @santiago
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef RESOLUTION_HPP_
#define RESOLUTION_HPP_

namespace RayTracer {

/**
 * @brief Represents the resolution of an image
 */

class Resolution {
 public:
  /**
   * @brief Default constructor
   * Initializes resolution to 0x0
   */
  Resolution();

  /**
   * @brief Constructor with width and height
   * @param width The width of the image
   * @param height The height of the image
   */
  Resolution(int width, int height);

  /**
   * @brief Set the width of the image
   * @param width The new width of the image
   */

  int getWidth() const;
  /**
   * @brief Get the height of the image
   * @return The height of the image
   */
  int getHeight() const;

  /**
   * @brief Assignment operator
   * @param other The resolution to assign from
   * @return Reference to this resolution after assignment
   */
  bool operator==(const Resolution& other) const;

 private:
  int _width;
  int _height;
};

}  // namespace RayTracer

#endif /* !RESOLUTION_HPP_ */
