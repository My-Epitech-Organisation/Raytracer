/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser aliases
*/

/**
 * @file SceneParserAliases.cpp
 * @brief Implementation of alias methods for the SceneParser class
 * @author Santi
 * @date 2025-05-17
 * @version 1.0
 */

#include "../primitives/Cylinder.hpp"
#include "../primitives/Torus.hpp"
#include "SceneParser.hpp"

using namespace libconfig;

namespace RayTracer {

// Add alias for the Tori parser
std::vector<Torus> SceneParser::parseToruses(
    const libconfig::Setting& setting) {
  return parseTori(setting);
}

// Add alias for the Cylinders parser
std::vector<Cylinder> SceneParser::parseCylinders(
    const libconfig::Setting& setting) {
  return parseInfiniteCylinders(setting);
}

}  // namespace RayTracer
