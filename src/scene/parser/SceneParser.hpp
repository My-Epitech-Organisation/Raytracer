/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
*/

#ifndef SCENEPARSER_HPP_
#define SCENEPARSER_HPP_

#include <libconfig.h++>
#include <vector>
#include "../../core/Color.hpp"
#include "../../core/Vector3D.hpp"
#include "../Camera.hpp"
#include "../primitives/Plane.hpp"
#include "../primitives/Sphere.hpp"

namespace RayTracer {

class SceneParser {
 public:
  SceneParser() = default;
  Camera parseCamera(const libconfig::Setting& cameraSetting);
  Sphere parseSphere(const libconfig::Setting& sphereSetting);
  std::vector<Sphere> parseSpheres(const libconfig::Setting& setting);
  Plane parsePlane(const libconfig::Setting& planeSetting);
  std::vector<Plane> parsePlanes(const libconfig::Setting& setting);
  void parseLights(const libconfig::Setting& lightsSetting);
  std::shared_ptr<Light> parseLights(const libconfig::Setting& lightsSetting);
  void parsePrimitives(const libconfig::Setting& primitivesSetting);
};

}  // namespace RayTracer

#endif /* !SCENEPARSER_HPP_ */
