/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color class header
*/

/**
 * @file Color.hpp
 * @brief Provides the Color class for managing RGB color values with various
 * operations and transformations
 * @author @paul-antoine
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <cstdint>
#include <iostream>

namespace RayTracer {

/**
 * @brief Represents an RGB color
 */
class Color {
 public:
  /**
   * @brief Default constructor
   * Creates a black color (0, 0, 0)
   */
  Color();

  /**
   * @brief Constructor with RGB values (0-255)
   * @param r Red component [0-255]
   * @param g Green component [0-255]
   * @param b Blue component [0-255]
   */
  Color(uint8_t r, uint8_t g, uint8_t b);

  /**
   * @brief Constructor with RGB values as floats (0.0-1.0)
   * @param r Red component [0.0-1.0]
   * @param g Green component [0.0-1.0]
   * @param b Blue component [0.0-1.0]
   */
  Color(double r, double g, double b);

  /**
   * @brief Copy constructor
   * @param other The color to copy
   */
  Color(const Color& other);

  /**
   * @brief Assignment operator
   * @param other The color to assign from
   * @return Reference to this color after assignment
   */
  Color& operator=(const Color& other);

  /**
   * @brief Destructor
   */
  ~Color();

  /**
   * @brief Get the red component (0-255)
   * @return The red component
   */
  uint8_t getR() const;

  /**
   * @brief Get the green component (0-255)
   * @return The green component
   */
  uint8_t getG() const;

  /**
   * @brief Get the blue component (0-255)
   * @return The blue component
   */
  uint8_t getB() const;

  /**
   * @brief Get the red component as a floating point (0.0-1.0)
   * @return The red component as a float
   */
  double getRf() const;

  /**
   * @brief Get the green component as a floating point (0.0-1.0)
   * @return The green component as a float
   */
  double getGf() const;

  /**
   * @brief Get the blue component as a floating point (0.0-1.0)
   * @return The blue component as a float
   */
  double getBf() const;

  /**
   * @brief Set the red component (0-255)
   * @param r The new red component
   */
  void setR(uint8_t r);

  /**
   * @brief Set the green component (0-255)
   * @param g The new green component
   */
  void setG(uint8_t g);

  /**
   * @brief Set the blue component (0-255)
   * @param b The new blue component
   */
  void setB(uint8_t b);

  /**
   * @brief Set the red component as a floating point (0.0-1.0)
   * @param r The new red component, clamped to [0.0, 1.0]
   */
  void setRf(double r);

  /**
   * @brief Set the green component as a floating point (0.0-1.0)
   * @param g The new green component, clamped to [0.0, 1.0]
   */
  void setGf(double g);

  /**
   * @brief Set the blue component as a floating point (0.0-1.0)
   * @param b The new blue component, clamped to [0.0, 1.0]
   */
  void setBf(double b);

  /**
   * @brief Addition operator for colors
   * @param other The color to add
   * @return A new color with the sum of components (clamped to 0-255)
   */
  Color operator+(const Color& other) const;

  /**
   * @brief Addition assignment operator
   * @param other The color to add
   * @return Reference to this color after addition
   */
  Color& operator+=(const Color& other);

  /**
   * @brief Multiplication operator for scaling a color by a scalar
   * @param scalar The scalar to multiply by [0.0-1.0]
   * @return A new color with components scaled by the scalar
   */
  Color operator*(double scalar) const;

  /**
   * @brief Multiplication assignment operator with a scalar
   * @param scalar The scalar to multiply by [0.0-1.0]
   * @return Reference to this color after multiplication
   */
  Color& operator*=(double scalar);

  /**
   * @brief Multiplication operator for blending colors
   * @param other The color to multiply with (component-wise)
   * @return A new color with components multiplied
   */
  Color operator*(const Color& other) const;

  /**
   * @brief Multiplication assignment operator with another color
   * @param other The color to multiply with
   * @return Reference to this color after multiplication
   */
  Color& operator*=(const Color& other);

  /**
   * @brief Equality operator
   * @param other The color to compare with
   * @return true if the colors are identical, false otherwise
   */
  bool operator==(const Color& other) const;

  /**
   * @brief Inequality operator
   * @param other The color to compare with
   * @return true if the colors are different, false otherwise
   */
  bool operator!=(const Color& other) const;

  /**
   * @brief Check if colors are approximately equal
   * @param other The color to compare with
   * @param tolerance The maximum allowed difference (0-255)
   * @return true if the colors are approximately equal, false otherwise
   */
  bool isEqual(const Color& other, uint8_t tolerance = 1) const;

  /**
   * @brief Blend this color with another using linear interpolation
   * @param other The color to blend with
   * @param t The blend factor [0.0-1.0], 0 = this color, 1 = other color
   * @return The blended color
   */
  Color blend(const Color& other, double t) const;

  /**
   * @brief Create a grayscale version of this color
   * Use standard luminance formula: Y = 0.2126*R + 0.7152*G + 0.0722*B
   * @return A new color with equal R, G, and B values based on luminance
   */
  Color toGrayscale() const;

  /**
   * @brief Convert color to 32-bit unsigned integer format (0xRRGGBB)
   * @return The 32-bit representation of the color
   */
  uint32_t toRGB() const;

  /**
   * @brief Create a color from a 32-bit RGB value (0xRRGGBB)
   * @param rgb The RGB value
   * @return The new color
   */
  static Color fromRGB(uint32_t rgb);

  // Predefined colors
  static const Color BLACK;    ///< Black color (0, 0, 0)
  static const Color WHITE;    ///< White color (255, 255, 255)
  static const Color RED;      ///< Red color (255, 0, 0)
  static const Color GREEN;    ///< Green color (0, 255, 0)
  static const Color BLUE;     ///< Blue color (0, 0, 255)
  static const Color YELLOW;   ///< Yellow color (255, 255, 0)
  static const Color CYAN;     ///< Cyan color (0, 255, 255)
  static const Color MAGENTA;  ///< Magenta color (255, 0, 255)
  static const Color GRAY;     ///< Gray color (128, 128, 128)

 private:
  uint8_t _r;  ///< Red component (0-255)
  uint8_t _g;  ///< Green component (0-255)
  uint8_t _b;  ///< Blue component (0-255)

  /**
   * @brief Helper function to clamp a float to the range [0.0, 1.0]
   * @param value The value to clamp
   * @return The clamped value
   */
  static double clamp(double value);

  /**
   * @brief Helper function to convert a float [0.0, 1.0] to uint8_t [0, 255]
   * @param value The float value to convert
   * @return The converted uint8_t value
   */
  static uint8_t floatToByte(double value);

  /**
   * @brief Helper function to convert a uint8_t [0, 255] to float [0.0, 1.0]
   * @param value The uint8_t value to convert
   * @return The converted float value
   */
  static double byteToFloat(uint8_t value);
};

/**
 * @brief Output stream operator for Color
 * @param os The output stream
 * @param color The color to output
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& os, const Color& color);

}  // namespace RayTracer

#endif /* !COLOR_HPP_ */
