/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Abstract Light class
*/

/**
 * @file Light.hpp
 * @brief Defines the abstract Light base class for all light sources in the ray
 * tracer
 * @author @santiago
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include <string>
#include "../../../include/ILight.hpp"

namespace RayTracer {

/**
 * @brief Abstract base class for all light types
 */
class Light : public ILight {
 public:
  virtual ~Light() = default;
  virtual std::string toString() const = 0;
};

}  // namespace RayTracer

#endif /* !LIGHT_HPP_ */
