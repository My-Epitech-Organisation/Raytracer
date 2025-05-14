/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Abstract Light class
*/

#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include <string>
#include <vector>
#include "../../core/Vector3D.hpp"

namespace RayTracer {

/**
 * @brief Abstract base class for all light types
 */
class Light {
  public:
    virtual ~Light() = default;
    virtual std::string toString() const = 0;
};

}  // namespace RayTracer

#endif /* !LIGHT_HPP_ */
