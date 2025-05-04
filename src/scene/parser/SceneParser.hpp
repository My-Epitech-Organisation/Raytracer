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

 private:
};

}  // namespace RayTracer

#endif /* !SCENEPARSER_HPP_ */
