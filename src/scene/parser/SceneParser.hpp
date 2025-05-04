/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
*/

#ifndef SCENEPARSER_HPP_
#define SCENEPARSER_HPP_

#include <libconfig.h++>
#include "../Camera.hpp"

namespace RayTracer {

class SceneParser {
 public:
  SceneParser() = default;
  Camera parseCamera(const libconfig::Setting& cameraSetting);

 private:
};

}  // namespace RayTracer

#endif /* !SCENEPARSER_HPP_ */
