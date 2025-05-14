/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightingSettings
*/

#ifndef LIGHTING_SETTINGS_HPP_
#define LIGHTING_SETTINGS_HPP_

#include <vector>
#include "../../core/Vector3D.hpp"
#include "Light.hpp"

namespace RayTracer {

struct LightingSettings {
  float ambient = 0.0f;
  float diffuse = 0.0f;
};

}  // namespace RayTracer

#endif /* !LIGHTING_SETTINGS_HPP_ */
