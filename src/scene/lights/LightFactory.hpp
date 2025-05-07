/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory header
*/

#ifndef LIGHT_FACTORY_HPP_
#define LIGHT_FACTORY_HPP_

#include <libconfig.h++>
#include <memory>
#include "../lights/Light.hpp"

namespace RayTracer {

class LightFactory {
 public:
  /**
   * @brief Create a Light object from config setting
   * @param setting libconfig setting representing a light
   * @return A unique pointer to a Light
   */
  static std::unique_ptr<Light> createLight(const libconfig::Setting& setting);
};

}  // namespace RayTracer

#endif /* !LIGHT_FACTORY_HPP_ */
