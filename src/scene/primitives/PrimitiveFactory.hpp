/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitiveFactory header
*/

/**
 * @file PrimitiveFactory.hpp
 * @brief Provides a factory class for creating different types of primitive
 * objects
 * @author Santi
 * @date 2025-05-17
 * @version 1.0
 */

#ifndef PRIMITIVE_FACTORY_HPP_
#define PRIMITIVE_FACTORY_HPP_

#include <functional>
#include <libconfig.h++>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../../include/IPrimitive.hpp"
#include "Cone.hpp"
#include "Cylinder.hpp"
#include "LimitedCone.hpp"
#include "LimitedCylinder.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Torus.hpp"

namespace RayTracer {

/**
 * @class PrimitiveFactory
 * @brief Factory class to create primitives of various types
 *
 * This class implements the Factory design pattern to create geometric
 * primitive objects based on configuration settings.
 */
class PrimitiveFactory {
 public:
  /**
   * @brief Result struct containing all primitives created from a config
   */
  struct Result {
    std::vector<std::shared_ptr<IPrimitive>> primitives;
  };

  /**
   * @brief Create all primitives from a configuration section
   * @param setting libconfig setting containing all primitive definitions
   * @return A Result structure containing all created primitives
   */
  static Result createPrimitives(const libconfig::Setting& setting);

  /**
   * @brief Create a sphere primitive from configuration
   * @param setting libconfig setting containing sphere parameters
   * @return A shared pointer to the created sphere
   */
  static std::shared_ptr<Sphere> createSphere(
      const libconfig::Setting& setting);

  /**
   * @brief Create a plane primitive from configuration
   * @param setting libconfig setting containing plane parameters
   * @return A shared pointer to the created plane
   */
  static std::shared_ptr<Plane> createPlane(const libconfig::Setting& setting);

  /**
   * @brief Create an infinite cone primitive from configuration
   * @param setting libconfig setting containing cone parameters
   * @return A shared pointer to the created cone
   */
  static std::shared_ptr<Cone> createCone(const libconfig::Setting& setting);

  /**
   * @brief Create a limited cone primitive from configuration
   * @param setting libconfig setting containing limited cone parameters
   * @return A shared pointer to the created limited cone
   */
  static std::shared_ptr<LimitedCone> createLimitedCone(
      const libconfig::Setting& setting);

  /**
   * @brief Create an infinite cylinder primitive from configuration
   * @param setting libconfig setting containing cylinder parameters
   * @return A shared pointer to the created cylinder
   */
  static std::shared_ptr<Cylinder> createCylinder(
      const libconfig::Setting& setting);

  /**
   * @brief Create a limited cylinder primitive from configuration
   * @param setting libconfig setting containing limited cylinder parameters
   * @return A shared pointer to the created limited cylinder
   */
  static std::shared_ptr<LimitedCylinder> createLimitedCylinder(
      const libconfig::Setting& setting);

  /**
   * @brief Create a torus primitive from configuration
   * @param setting libconfig setting containing torus parameters
   * @return A shared pointer to the created torus
   */
  static std::shared_ptr<Torus> createTorus(const libconfig::Setting& setting);

  /**
   * @brief Create all primitives of a specific type from configuration
   * @param setting libconfig setting containing primitive list
   * @param type The type of primitives to create (e.g., "sphere", "plane")
   * @return Vector of shared pointers to created primitives
   */
  static std::vector<std::shared_ptr<IPrimitive>> createPrimitivesOfType(
      const libconfig::Setting& setting, const std::string& type);

 private:
  /**
   * @brief Parse a transform from configuration and apply it to a primitive
   * @param setting libconfig setting containing transform data
   * @param primitive The primitive to apply the transform to
   */
  static void applyTransformIfExists(const libconfig::Setting& setting,
                                     std::shared_ptr<IPrimitive> primitive);

  /**
   * @brief Parse a color from configuration
   * @param setting libconfig setting containing color data
   * @return Color object created from the configuration
   */
  static Color parseColor(const libconfig::Setting& setting);

  /**
   * @brief Helper function to get a float value from either an int or float
   * setting
   * @param setting The setting to extract the value from
   * @return The extracted float value
   */
  static float getFlexibleFloat(const libconfig::Setting& setting);

  /**
   * @brief Normalize a primitive type name to lowercase for consistent lookup
   * @param typeName The type name to normalize
   * @return The normalized type name
   */
  static std::string normalizeTypeName(const std::string& typeName);

  // Type to factory method mapping for primitive creation
  using PrimitiveCreator =
      std::function<std::shared_ptr<IPrimitive>(const libconfig::Setting&)>;
  static std::unordered_map<std::string, PrimitiveCreator> primitiveCreators;
};

}  // namespace RayTracer

#endif /* !PRIMITIVE_FACTORY_HPP_ */
