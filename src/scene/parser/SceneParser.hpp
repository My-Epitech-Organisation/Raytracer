/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
*/

/**
 * @file SceneParser.hpp
 * @brief Definition of the SceneParser class for parsing scene configuration
 * files
 * @author @santiago
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef SCENEPARSER_HPP_
#define SCENEPARSER_HPP_

#include <libconfig.h++>
#include <memory>
#include <vector>
#include "../Camera.hpp"
#include "../lights/Light.hpp"
#include "../lights/LightingSettings.hpp"
#include "../primitives/Cone.hpp"
#include "../primitives/Cylinder.hpp"
#include "../primitives/LimitedCone.hpp"
#include "../primitives/LimitedCylinder.hpp"
#include "../primitives/Plane.hpp"
#include "../primitives/Sphere.hpp"
#include "../primitives/Torus.hpp"
#include "../primitives/Triangle.hpp"

namespace RayTracer {

/**
 * @class SceneParser
 * @brief Utility class for parsing scene elements from a configuration file.
 *
 * Uses the libconfig library to read parameters for 3D objects, camera,
 * lights, and transformations.
 */
class SceneParser {
 public:
  /// Default constructor
  SceneParser() = default;

  /**
   * @brief Parse the camera from a configuration block.
   * @param cameraSetting Reference to the "camera" block in the configuration
   * file.
   * @return An initialized Camera instance.
   */
  Camera parseCamera(const libconfig::Setting& cameraSetting);

  /**
   * @brief Parse a single sphere.
   * @param sphereSetting Reference to the sphere parameters.
   * @return A Sphere instance.
   */
  Sphere parseSphere(const libconfig::Setting& sphereSetting);

  /**
   * @brief Parse multiple spheres.
   * @param setting Reference to a list of spheres.
   * @return Vector of spheres.
   */
  std::vector<Sphere> parseSpheres(const libconfig::Setting& setting);

  /**
   * @brief Parse a single plane.
   * @param planeSetting Reference to the plane parameters.
   * @return A Plane instance.
   */
  Plane parsePlane(const libconfig::Setting& planeSetting);

  /**
   * @brief Parse multiple planes.
   * @param setting Reference to a list of planes.
   * @return Vector of planes.
   */
  std::vector<Plane> parsePlanes(const libconfig::Setting& setting);

  /**
   * @brief Parse the scene lighting parameters.
   * @param lightsSetting Reference to the lights block.
   * @return A pair containing the general lighting parameters
   *         and the list of lights.
   */
  std::pair<LightingSettings, std::vector<std::unique_ptr<Light>>>
  parseLightingSettings(const libconfig::Setting& lightsSetting);

  /**
   * @brief Parse a single cone.
   * @param coneSetting Reference to the cone parameters.
   * @return A Cone instance.
   */
  Cone parseCone(const libconfig::Setting& coneSetting);

  /**
   * @brief Parse multiple cones.
   * @param setting Reference to a list of cones.
   * @return Vector of cones.
   */
  std::vector<Cone> parseCones(const libconfig::Setting& setting);

  /**
   * @brief Parse all primitives present in the scene.
   * @param primitivesSetting Reference to the "primitives" block.
   */
  void parsePrimitives(const libconfig::Setting& primitivesSetting);

  /**
   * @brief Get the parsed primitives
   * @return Vector of primitives
   */
  const std::vector<std::shared_ptr<IPrimitive>>& getPrimitives() const {
    return _primitives;
  }

  /**
   * @brief Parse a geometric transformation.
   * @param transformSetting Reference to the transformation parameters.
   * @return A Transform instance.
   */
  Transform parseTransform(const libconfig::Setting& transformSetting);

  /**
   * @brief Parse a limited cone.
   * @param coneSetting Reference to the limited cone parameters.
   * @return A LimitedCone instance.
   */
  LimitedCone parseLimitedCone(const libconfig::Setting& coneSetting);

  /**
   * @brief Parse multiple limited cones.
   * @param setting Reference to a list of limited cones.
   * @return Vector of limited cones.
   */
  std::vector<LimitedCone> parseLimitedCones(const libconfig::Setting& setting);
  LimitedCylinder parseLimitedCylinder(
      const libconfig::Setting& cylinderSetting);
  std::vector<LimitedCylinder> parseLimitedCylinders(
      const libconfig::Setting& setting);
  Cylinder parseInfiniteCylinder(const libconfig::Setting& cylinderSetting);
  std::vector<Cylinder> parseInfiniteCylinders(
      const libconfig::Setting& setting);
  std::vector<Cylinder> parseCylinders(const libconfig::Setting& setting);
  Torus parseTorus(const libconfig::Setting& torusSetting);
  std::vector<Torus> parseTori(const libconfig::Setting& setting);
  std::vector<Torus> parseToruses(const libconfig::Setting& setting);
  Triangle parseTriangle(const libconfig::Setting& triangleSetting);
  std::vector<Triangle> parseTriangles(const libconfig::Setting& setting);

 private:
  // Store parsed primitives
  std::vector<std::shared_ptr<IPrimitive>> _primitives;
};

}  // namespace RayTracer

#endif /* !SCENEPARSER_HPP_ */
