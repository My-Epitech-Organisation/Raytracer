/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
*/

#ifndef SCENEPARSER_HPP_
#define SCENEPARSER_HPP_

#include <libconfig.h++>
#include <memory>
#include <vector>
#include "../../core/Color.hpp"
#include "../../core/Vector3D.hpp"
#include "../Camera.hpp"
#include "../lights/Light.hpp"
#include "../primitives/Cone.hpp"
#include "../primitives/Cylinder.hpp"
#include "../primitives/LimitedCone.hpp"
#include "../primitives/LimitedCylinder.hpp"
#include "../primitives/Plane.hpp"
#include "../primitives/Sphere.hpp"
#include "../primitives/Torus.hpp"

namespace RayTracer {

class SceneParser {
 public:
  SceneParser() = default;
  Camera parseCamera(const libconfig::Setting& cameraSetting);
  Sphere parseSphere(const libconfig::Setting& sphereSetting);
  std::vector<Sphere> parseSpheres(const libconfig::Setting& setting);
  Plane parsePlane(const libconfig::Setting& planeSetting);
  std::vector<Plane> parsePlanes(const libconfig::Setting& setting);
  Cone parseCone(const libconfig::Setting& coneSetting);
  std::vector<Cone> parseCones(const libconfig::Setting& setting);
  std::shared_ptr<Light> parseLights(const libconfig::Setting& lightsSetting);
  void parsePrimitives(const libconfig::Setting& primitivesSetting);
  Transform parseTransform(const libconfig::Setting& transformSetting);
  LimitedCone parseLimitedCone(const libconfig::Setting& coneSetting);
  std::vector<LimitedCone> parseLimitedCones(const libconfig::Setting& setting);
  LimitedCylinder parseLimitedCylinder(
      const libconfig::Setting& cylinderSetting);
  std::vector<LimitedCylinder> parseLimitedCylinders(
      const libconfig::Setting& setting);
  Cylinder parseInfiniteCylinder(const libconfig::Setting& cylinderSetting);
  std::vector<Cylinder> parseInfiniteCylinders(
      const libconfig::Setting& setting);
  Torus parseTorus(const libconfig::Setting& torusSetting);
  std::vector<Torus> parseTori(const libconfig::Setting& setting);
};

}  // namespace RayTracer

#endif /* !SCENEPARSER_HPP_ */
