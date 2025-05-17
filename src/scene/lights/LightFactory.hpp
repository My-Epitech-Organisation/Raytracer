/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory header
*/

/**
 * @file LightFactory.hpp
 * @brief Provides a factory class for creating different types of light sources
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef LIGHT_FACTORY_HPP_
#define LIGHT_FACTORY_HPP_

#include <libconfig.h++>
#include <memory>
#include <vector>
#include "Light.hpp"
#include "LightingSettings.hpp"

namespace RayTracer {

class LightFactory {
 public:
  struct Result {
    LightingSettings settings;
    std::vector<std::unique_ptr<Light>> lights;
  };
  /**
   * @brief Create a Light object from config setting
   * @param setting libconfig setting representing a light
   * @return A unique pointer to a Light
   */
  static Result createLights(const libconfig::Setting& setting);
};

}  // namespace RayTracer

#endif /* !LIGHT_FACTORY_HPP_ */
