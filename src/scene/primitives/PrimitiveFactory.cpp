/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitiveFactory implementation
*/

/**
 * @file PrimitiveFactory.cpp
 * @brief Implementation of the factory class for creating different types of
 * primitives
 * @author Santi
 * @date 2025-05-17
 * @version 1.0
 */

#include "PrimitiveFactory.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include "../../../include/exceptions/ParserException.hpp"
#include "../../core/Transform.hpp"
#include "CheckerboardPlane.hpp"

using namespace libconfig;

namespace RayTracer {

// Initialize the mapping of primitive type names to their factory methods
std::unordered_map<std::string, PrimitiveFactory::PrimitiveCreator>
    PrimitiveFactory::primitiveCreators = {
        {"spheres",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createSphere(setting));
         }},
        {"planes",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createPlane(setting));
         }},
        {"cones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createCone(setting));
         }},
        {"limitedcones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCone(setting));
         }},
        {"cylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createCylinder(setting));
         }},
        {"limitedcylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCylinder(setting));
         }},
        {"toruses",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTorus(setting));
         }},
        {"tori",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTorus(setting));
         }},
        {"torus",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTorus(setting));
         }},
        {"triangles",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTriangle(setting));
         }},
        {"triangle", [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTriangle(setting));
         }}};

PrimitiveFactory::Result PrimitiveFactory::createPrimitives(
    const Setting& setting) {
  Result result;

  // Iterate through all groups in the primitives section
  for (int i = 0; i < setting.getLength(); ++i) {
    const Setting& primitiveGroup = setting[i];
    std::string typeName = primitiveGroup.getName();
    std::string normalizedTypeName = normalizeTypeName(typeName);

    // Check if this primitive type is supported (with case insensitivity)
    auto it = primitiveCreators.find(normalizedTypeName);

    if (it != primitiveCreators.end()) {
      try {
        // Create primitives of this type and add them to our result
        auto primitivesOfType =
            createPrimitivesOfType(primitiveGroup, it->first);
        result.primitives.insert(result.primitives.end(),
                                 primitivesOfType.begin(),
                                 primitivesOfType.end());
      } catch (const std::exception& e) {
        std::cerr << "Error creating primitives of type '" << typeName
                  << "': " << e.what() << std::endl;
      }
    } else {
      std::cerr << "Unsupported primitive type: " << typeName << std::endl;
    }
  }

  return result;
}

std::vector<std::shared_ptr<IPrimitive>>
PrimitiveFactory::createPrimitivesOfType(const Setting& setting,
                                         const std::string& type) {

  std::vector<std::shared_ptr<IPrimitive>> primitives;

  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw ParserException("Primitive setting must be a list, group, or array");
  }

  // Get the factory method for this primitive type
  auto creatorFunc = primitiveCreators.at(type);

  // Create each primitive in the list
  for (int i = 0; i < setting.getLength(); ++i) {
    try {
      auto primitive = creatorFunc(setting[i]);
      primitives.push_back(primitive);
    } catch (const std::exception& e) {
      std::cerr << "Failed to create primitive at index " << i << ": "
                << e.what() << std::endl;
    }
  }

  return primitives;
}

std::shared_ptr<Sphere> PrimitiveFactory::createSphere(const Setting& setting) {
  try {
    double x = 0.0, y = 0.0, z = 0.0, r = 1.0;
    int xInt = 0, yInt = 0, zInt = 0, rInt = 1;

    // Handle both direct float and int values for position
    bool hasX = setting.lookupValue("x", x);
    if (!hasX && setting.lookupValue("x", xInt)) {
      x = static_cast<double>(xInt);
    }

    bool hasY = setting.lookupValue("y", y);
    if (!hasY && setting.lookupValue("y", yInt)) {
      y = static_cast<double>(yInt);
    }

    bool hasZ = setting.lookupValue("z", z);
    if (!hasZ && setting.lookupValue("z", zInt)) {
      z = static_cast<double>(zInt);
    }

    // Handle both float and int values for radius
    bool hasR = setting.lookupValue("r", r);
    if (!hasR && setting.lookupValue("r", rInt)) {
      r = static_cast<double>(rInt);
    }

    Color color = Color::RED;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    auto sphere = std::make_shared<Sphere>(Vector3D(x, y, z), r, color);

    // Apply any transformation
    applyTransformIfExists(setting, sphere);

    return sphere;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in sphere: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in sphere: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating sphere: ") + e.what());
  }
}

std::shared_ptr<Plane> PrimitiveFactory::createPlane(const Setting& setting) {
  try {
    char axis = 'x';
    std::string axisStr;

    if (setting.lookupValue("axis", axisStr)) {
      if (!axisStr.empty()) {
        axis = axisStr[0];
      }
    }

    float pos = 0.0f;
    const Setting& posSetting = setting.lookup("position");
    pos = getFlexibleFloat(posSetting);

    Color color = Color::WHITE;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    if (setting.exists("checkerboard")) {
      Color alternateColor = Color::BLACK;  // Default alternate color
      double squareSize = 1.0;              // Default square size

      const Setting& checkerboardSetting = setting["checkerboard"];

      if (checkerboardSetting.exists("alternateColor")) {
        alternateColor = parseColor(checkerboardSetting["alternateColor"]);
      }

      if (checkerboardSetting.exists("size")) {
        squareSize = getFlexibleFloat(checkerboardSetting["size"]);
      }

      auto plane = std::make_shared<CheckerboardPlane>(
          axis, pos, color, alternateColor, squareSize);

      // Apply any transformation
      applyTransformIfExists(setting, plane);

      return plane;
    } else {
      // Create a regular plane
      auto plane = std::make_shared<Plane>(axis, pos, color);

      // Apply any transformation
      applyTransformIfExists(setting, plane);

      return plane;
    }
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in plane: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in plane: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating plane: ") + e.what());
  }
}

std::shared_ptr<Cone> PrimitiveFactory::createCone(const Setting& setting) {
  try {
    // Parse position or apex
    double apexX = 0.0, apexY = 0.0, apexZ = 0.0;
    int apexXInt = 0, apexYInt = 0, apexZInt = 0;

    bool hasPosition = false;

    // Check for direct x,y,z coordinates in the setting
    if (setting.exists("x") && setting.exists("y") && setting.exists("z")) {
      bool hasX = setting.lookupValue("x", apexX);
      if (!hasX && setting.lookupValue("x", apexXInt)) {
        apexX = static_cast<double>(apexXInt);
        hasX = true;
      }

      bool hasY = setting.lookupValue("y", apexY);
      if (!hasY && setting.lookupValue("y", apexYInt)) {
        apexY = static_cast<double>(apexYInt);
        hasY = true;
      }

      bool hasZ = setting.lookupValue("z", apexZ);
      if (!hasZ && setting.lookupValue("z", apexZInt)) {
        apexZ = static_cast<double>(apexZInt);
        hasZ = true;
      }

      hasPosition = hasX && hasY && hasZ;
    }
    // Check for apex as a separate group
    else if (setting.exists("apex")) {
      const Setting& apexSetting = setting["apex"];

      bool hasX = apexSetting.lookupValue("x", apexX);
      if (!hasX && apexSetting.lookupValue("x", apexXInt)) {
        apexX = static_cast<double>(apexXInt);
        hasX = true;
      }

      bool hasY = apexSetting.lookupValue("y", apexY);
      if (!hasY && apexSetting.lookupValue("y", apexYInt)) {
        apexY = static_cast<double>(apexYInt);
        hasY = true;
      }

      bool hasZ = apexSetting.lookupValue("z", apexZ);
      if (!hasZ && apexSetting.lookupValue("z", apexZInt)) {
        apexZ = static_cast<double>(apexZInt);
        hasZ = true;
      }

      hasPosition = hasX && hasY && hasZ;
    }

    if (!hasPosition) {
      throw ParserException(
          "Missing apex or position coordinates in cone definition");
    }

    // Parse axis vector - defaults to Y-axis if not specified
    double axisX = 0.0, axisY = 1.0, axisZ = 0.0;
    int axisXInt = 0, axisYInt = 1, axisZInt = 0;

    if (setting.exists("axis")) {
      const Setting& axisSetting = setting["axis"];

      if (axisSetting.isGroup()) {
        bool hasAxisX = axisSetting.lookupValue("x", axisX);
        if (!hasAxisX && axisSetting.lookupValue("x", axisXInt)) {
          axisX = static_cast<double>(axisXInt);
        }

        bool hasAxisY = axisSetting.lookupValue("y", axisY);
        if (!hasAxisY && axisSetting.lookupValue("y", axisYInt)) {
          axisY = static_cast<double>(axisYInt);
        }

        bool hasAxisZ = axisSetting.lookupValue("z", axisZ);
        if (!hasAxisZ && axisSetting.lookupValue("z", axisZInt)) {
          axisZ = static_cast<double>(axisZInt);
        }
      } else {
        throw ParserException(
            "Cone 'axis' setting must be a group (e.g., axis = { x = 0.0; y = "
            "1.0; z = 0.0; })");
      }
    }

    if (axisX == 0.0 && axisY == 0.0 && axisZ == 0.0) {
      throw ParserException("Cone axis cannot be a zero vector");
    }

    // Parse cone angle - default to using r value if angle not provided
    double angle = 0.0;
    int angleInt = 0;
    double radius = 0.0;
    int radiusInt = 0;

    bool hasAngle = false;

    if (setting.exists("angle")) {
      bool hasAngleDouble = setting.lookupValue("angle", angle);
      if (!hasAngleDouble && setting.lookupValue("angle", angleInt)) {
        angle = static_cast<double>(angleInt);
        hasAngle = true;
      } else if (hasAngleDouble) {
        hasAngle = true;
      }
    }

    // If angle not provided, try to use radius (r) to compute a reasonable
    // angle
    if (!hasAngle && setting.exists("r")) {
      bool hasRadius = setting.lookupValue("r", radius);
      if (!hasRadius && setting.lookupValue("r", radiusInt)) {
        radius = static_cast<double>(radiusInt);
        hasRadius = true;
      }

      if (hasRadius) {
        // Default to a 45-degree cone if only radius is provided
        angle = 45.0;
        hasAngle = true;
      }
    }

    // If we still don't have an angle, use a default
    if (!hasAngle) {
      angle = 45.0;  // Default angle
    }

    // Parse color
    Color color = Color::WHITE;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    auto cone =
        std::make_shared<Cone>(Vector3D(apexX, apexY, apexZ),
                               Vector3D(axisX, axisY, axisZ), angle, color);

    // Apply any transformation
    applyTransformIfExists(setting, cone);

    return cone;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in cone: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in cone: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating cone: ") + e.what());
  }
}

std::shared_ptr<LimitedCone> PrimitiveFactory::createLimitedCone(
    const Setting& setting) {
  try {
    // Parse apex coordinates
    double apexX = 0.0, apexY = 0.0, apexZ = 0.0;
    int apexXInt = 0, apexYInt = 0, apexZInt = 0;

    bool hasPosition = false;

    // Check for direct x,y,z coordinates in the setting
    if (setting.exists("x") && setting.exists("y") && setting.exists("z")) {
      bool hasX = setting.lookupValue("x", apexX);
      if (!hasX && setting.lookupValue("x", apexXInt)) {
        apexX = static_cast<double>(apexXInt);
        hasX = true;
      }

      bool hasY = setting.lookupValue("y", apexY);
      if (!hasY && setting.lookupValue("y", apexYInt)) {
        apexY = static_cast<double>(apexYInt);
        hasY = true;
      }

      bool hasZ = setting.lookupValue("z", apexZ);
      if (!hasZ && setting.lookupValue("z", apexZInt)) {
        apexZ = static_cast<double>(apexZInt);
        hasZ = true;
      }

      hasPosition = hasX && hasY && hasZ;
    }
    // Check for apex as a separate group
    else if (setting.exists("apex")) {
      const Setting& apexSetting = setting["apex"];

      bool hasX = apexSetting.lookupValue("x", apexX);
      if (!hasX && apexSetting.lookupValue("x", apexXInt)) {
        apexX = static_cast<double>(apexXInt);
        hasX = true;
      }

      bool hasY = apexSetting.lookupValue("y", apexY);
      if (!hasY && apexSetting.lookupValue("y", apexYInt)) {
        apexY = static_cast<double>(apexYInt);
        hasY = true;
      }

      bool hasZ = apexSetting.lookupValue("z", apexZ);
      if (!hasZ && apexSetting.lookupValue("z", apexZInt)) {
        apexZ = static_cast<double>(apexZInt);
        hasZ = true;
      }

      hasPosition = hasX && hasY && hasZ;
    }

    if (!hasPosition) {
      throw ParserException(
          "Missing apex or position coordinates in limited cone definition");
    }

    // Parse axis vector - support both in the setting and as a separate group
    double axisX = 0.0, axisY = 1.0, axisZ = 0.0;  // Default to Y-axis
    int axisXInt = 0, axisYInt = 1, axisZInt = 0;

    if (setting.exists("axis")) {
      const Setting& axisSetting = setting["axis"];

      if (axisSetting.isGroup()) {
        bool hasAxisX = axisSetting.lookupValue("x", axisX);
        if (!hasAxisX && axisSetting.lookupValue("x", axisXInt)) {
          axisX = static_cast<double>(axisXInt);
        }

        bool hasAxisY = axisSetting.lookupValue("y", axisY);
        if (!hasAxisY && axisSetting.lookupValue("y", axisYInt)) {
          axisY = static_cast<double>(axisYInt);
        }

        bool hasAxisZ = axisSetting.lookupValue("z", axisZ);
        if (!hasAxisZ && axisSetting.lookupValue("z", axisZInt)) {
          axisZ = static_cast<double>(axisZInt);
        }
      } else {
        throw ParserException(
            "LimitedCone 'axis' setting must be a group (e.g., axis = { x = "
            "0.0; "
            "y = 1.0; z = 0.0; })");
      }
    }

    if (axisX == 0.0 && axisY == 0.0 && axisZ == 0.0) {
      throw ParserException("LimitedCone axis cannot be a zero vector");
    }

    // Parse cone angle and radius
    double angle = 0.0;
    int angleInt = 0;
    double radius = 0.0;
    int radiusInt = 0;
    double height = 1.0;  // Default height
    int heightInt = 1;

    bool hasAngle = false;
    bool hasRadius = false;

    if (setting.exists("angle")) {
      bool hasAngleDouble = setting.lookupValue("angle", angle);
      if (!hasAngleDouble && setting.lookupValue("angle", angleInt)) {
        angle = static_cast<double>(angleInt);
        hasAngle = true;
      } else if (hasAngleDouble) {
        hasAngle = true;
      }
    }

    // Parse height
    if (setting.exists("height")) {
      bool hasHeightDouble = setting.lookupValue("height", height);
      if (!hasHeightDouble && setting.lookupValue("height", heightInt)) {
        height = static_cast<double>(heightInt);
      }
    }

    // Support minZ/maxZ to define the height
    if (setting.exists("minZ") && setting.exists("maxZ")) {
      double minZ = 0.0, maxZ = 0.0;
      int minZInt = 0, maxZInt = 0;

      bool hasMinZ = setting.lookupValue("minZ", minZ);
      if (!hasMinZ && setting.lookupValue("minZ", minZInt)) {
        minZ = static_cast<double>(minZInt);
        hasMinZ = true;
      }

      bool hasMaxZ = setting.lookupValue("maxZ", maxZ);
      if (!hasMaxZ && setting.lookupValue("maxZ", maxZInt)) {
        maxZ = static_cast<double>(maxZInt);
        hasMaxZ = true;
      }

      if (hasMinZ && hasMaxZ) {
        height = maxZ - minZ;
      }
    }

    // Try to compute angle from radius if needed
    if (!hasAngle && setting.exists("r")) {
      hasRadius = setting.lookupValue("r", radius);
      if (!hasRadius && setting.lookupValue("r", radiusInt)) {
        radius = static_cast<double>(radiusInt);
        hasRadius = true;
      }

      if (hasRadius) {
        // Calculate angle from radius and height
        if (height > 0.0) {
          angle = std::atan2(radius, height) * 180.0 / M_PI;
        } else {
          angle = 45.0;  // Default if height isn't valid
        }
        hasAngle = true;
      }
    }

    // Default angle if none provided
    if (!hasAngle) {
      angle = 45.0;
    }

    // Parse if the cone has caps
    bool hasCaps = true;  // Default to having caps
    if (setting.exists("caps")) {
      int caps = 1;
      setting.lookupValue("caps", caps);
      hasCaps = (caps != 0);
    }

    // Parse color
    Color color = Color::WHITE;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    auto limitedCone = std::make_shared<LimitedCone>(
        Vector3D(apexX, apexY, apexZ), Vector3D(axisX, axisY, axisZ), angle,
        color, height, hasCaps);

    // Apply any transformation
    applyTransformIfExists(setting, limitedCone);

    return limitedCone;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in limited cone: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in limited cone: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating limited cone: ") +
                          e.what());
  }
}

std::shared_ptr<Cylinder> PrimitiveFactory::createCylinder(
    const Setting& setting) {
  try {
    double radius = 1.0;  // Default radius
    if (setting.exists("radius")) {
      radius = getFlexibleFloat(setting["radius"]);
    }

    // Parse color
    Color color = Color::WHITE;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    auto cylinder = std::make_shared<Cylinder>(radius, color);

    // Apply any transformation
    applyTransformIfExists(setting, cylinder);

    return cylinder;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in cylinder: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in cylinder: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating cylinder: ") + e.what());
  }
}

std::shared_ptr<LimitedCylinder> PrimitiveFactory::createLimitedCylinder(
    const Setting& setting) {
  try {
    double radius = 1.0;  // Default radius
    if (setting.exists("radius")) {
      radius = getFlexibleFloat(setting["radius"]);
    }

    double height = 1.0;  // Default height
    if (setting.exists("height")) {
      height = getFlexibleFloat(setting["height"]);
    }

    // Parse color
    Color color = Color::WHITE;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    auto limitedCylinder =
        std::make_shared<LimitedCylinder>(radius, height, color);

    // Apply any transformation
    applyTransformIfExists(setting, limitedCylinder);

    return limitedCylinder;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(
        std::string("Setting not found in limited cylinder: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(
        std::string("Setting type error in limited cylinder: ") + e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating limited cylinder: ") +
                          e.what());
  }
}

std::shared_ptr<Torus> PrimitiveFactory::createTorus(const Setting& setting) {
  try {
    // Parse position
    double posX = 0.0, posY = 0.0, posZ = 0.0;
    int posXInt = 0, posYInt = 0, posZInt = 0;

    bool hasPosition = false;

    if (setting.exists("x") && setting.exists("y") && setting.exists("z")) {
      bool hasX = setting.lookupValue("x", posX);
      if (!hasX && setting.lookupValue("x", posXInt)) {
        posX = static_cast<double>(posXInt);
        hasX = true;
      }

      bool hasY = setting.lookupValue("y", posY);
      if (!hasY && setting.lookupValue("y", posYInt)) {
        posY = static_cast<double>(posYInt);
        hasY = true;
      }

      bool hasZ = setting.lookupValue("z", posZ);
      if (!hasZ && setting.lookupValue("z", posZInt)) {
        posZ = static_cast<double>(posZInt);
        hasZ = true;
      }

      hasPosition = hasX && hasY && hasZ;
    }

    // Parse majorRadius and tubeRadius
    double majorRadius = 1.0, tubeRadius = 0.3;  // Default values
    int majorRadiusInt = 1, tubeRadiusInt = 0;

    if (setting.exists("majorRadius")) {
      bool hasMajorRadius = setting.lookupValue("majorRadius", majorRadius);
      if (!hasMajorRadius &&
          setting.lookupValue("majorRadius", majorRadiusInt)) {
        majorRadius = static_cast<double>(majorRadiusInt);
      }
    }

    if (setting.exists("tubeRadius")) {
      bool hasTubeRadius = setting.lookupValue("tubeRadius", tubeRadius);
      if (!hasTubeRadius && setting.lookupValue("tubeRadius", tubeRadiusInt)) {
        tubeRadius = static_cast<double>(tubeRadiusInt);
      }
    }

    // Parse color
    Color color = Color::WHITE;  // Default color
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    }

    // Create the torus
    auto torus = std::make_shared<Torus>(majorRadius, tubeRadius, color);

    // If we have a position, create a transform to position it
    if (hasPosition) {
      Transform posTransform;
      posTransform.translate(posX, posY, posZ);
      torus->setTransform(posTransform);
    }

    // Apply any additional transformations
    applyTransformIfExists(setting, torus);

    return torus;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in torus: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in torus: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw ParserException(std::string("Error creating torus: ") + e.what());
  }
}

void PrimitiveFactory::applyTransformIfExists(
    const Setting& setting, std::shared_ptr<IPrimitive> primitive) {
  if (!setting.exists("transform")) {
    return;  // No transformation to apply
  }

  const Setting& transformSetting = setting["transform"];
  Transform transform;

  // Handle translation (support both "translate" and "translation")
  const Setting* translateSetting = nullptr;
  if (transformSetting.exists("translate")) {
    translateSetting = &transformSetting["translate"];
  } else if (transformSetting.exists("translation")) {
    translateSetting = &transformSetting["translation"];
  }

  if (translateSetting) {
    double x = 0.0, y = 0.0, z = 0.0;

    // Get x component
    if (translateSetting->exists("x")) {
      x = getFlexibleFloat((*translateSetting)["x"]);
    }

    // Get y component
    if (translateSetting->exists("y")) {
      y = getFlexibleFloat((*translateSetting)["y"]);
    }

    // Get z component
    if (translateSetting->exists("z")) {
      z = getFlexibleFloat((*translateSetting)["z"]);
    }

    transform.translate(x, y, z);
  }

  // Handle rotation (support both "rotate" and "rotation")
  const Setting* rotateSetting = nullptr;
  if (transformSetting.exists("rotate")) {
    rotateSetting = &transformSetting["rotate"];
  } else if (transformSetting.exists("rotation")) {
    rotateSetting = &transformSetting["rotation"];
  }

  if (rotateSetting) {
    double x = 0.0, y = 0.0, z = 0.0;

    // Get x rotation
    if (rotateSetting->exists("x")) {
      x = getFlexibleFloat((*rotateSetting)["x"]);
    }

    // Get y rotation
    if (rotateSetting->exists("y")) {
      y = getFlexibleFloat((*rotateSetting)["y"]);
    }

    // Get z rotation
    if (rotateSetting->exists("z")) {
      z = getFlexibleFloat((*rotateSetting)["z"]);
    }

    transform.rotateX(x);
    transform.rotateY(y);
    transform.rotateZ(z);
  }

  // Handle scaling
  if (transformSetting.exists("scale")) {
    const Setting& scaleSetting = transformSetting["scale"];
    double x = 1.0, y = 1.0, z = 1.0;

    // Get x scale
    if (scaleSetting.exists("x")) {
      x = getFlexibleFloat(scaleSetting["x"]);
    }

    // Get y scale
    if (scaleSetting.exists("y")) {
      y = getFlexibleFloat(scaleSetting["y"]);
    }

    // Get z scale
    if (scaleSetting.exists("z")) {
      z = getFlexibleFloat(scaleSetting["z"]);
    }

    transform.scale(x, y, z);
  }

  // Apply the combined transformation to the primitive
  primitive->setTransform(transform);
}

Color PrimitiveFactory::parseColor(const Setting& setting) {
  // Default to white
  uint8_t r = 255, g = 255, b = 255;

  // Get each color component, supporting both int and float formats
  if (setting.exists("r")) {
    r = static_cast<uint8_t>(
        std::min(255.0f, std::max(0.0f, getFlexibleFloat(setting["r"]))));
  }

  if (setting.exists("g")) {
    g = static_cast<uint8_t>(
        std::min(255.0f, std::max(0.0f, getFlexibleFloat(setting["g"]))));
  }

  if (setting.exists("b")) {
    b = static_cast<uint8_t>(
        std::min(255.0f, std::max(0.0f, getFlexibleFloat(setting["b"]))));
  }

  return Color(r, g, b);
}

float PrimitiveFactory::getFlexibleFloat(const Setting& setting) {
  try {
    switch (setting.getType()) {
      case Setting::TypeInt:
        return static_cast<float>(static_cast<int>(setting));
      case Setting::TypeInt64:
        return static_cast<float>(static_cast<long long>(setting));
      case Setting::TypeFloat:
        return static_cast<float>(setting);
      default:
        throw ParserException("Expected a numeric type (int or float)");
    }
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error: ") + e.what());
  }
}

std::string PrimitiveFactory::normalizeTypeName(const std::string& typeName) {
  std::string result = typeName;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return result;
}

std::shared_ptr<Triangle> PrimitiveFactory::createTriangle(
    const Setting& setting) {
  try {
    // Parse vertices a, b, c
    double ax = 0, ay = 0, az = 0, bx = 0, by = 0, bz = 0, cx = 0, cy = 0,
           cz = 0;

    // Parse vertex a
    if (setting.exists("a")) {
      const Setting& aSet = setting["a"];
      if (!aSet.exists("x") || !aSet.exists("y") || !aSet.exists("z")) {
        throw ParserException("Triangle: missing or invalid 'a' coordinates");
      }
      ax = getFlexibleFloat(aSet["x"]);
      ay = getFlexibleFloat(aSet["y"]);
      az = getFlexibleFloat(aSet["z"]);
    } else {
      throw ParserException("Triangle: missing 'a' vertex");
    }

    // Parse vertex b
    if (setting.exists("b")) {
      const Setting& bSet = setting["b"];
      if (!bSet.exists("x") || !bSet.exists("y") || !bSet.exists("z")) {
        throw ParserException("Triangle: missing or invalid 'b' coordinates");
      }
      bx = getFlexibleFloat(bSet["x"]);
      by = getFlexibleFloat(bSet["y"]);
      bz = getFlexibleFloat(bSet["z"]);
    } else {
      throw ParserException("Triangle: missing 'b' vertex");
    }

    // Parse vertex c
    if (setting.exists("c")) {
      const Setting& cSet = setting["c"];
      if (!cSet.exists("x") || !cSet.exists("y") || !cSet.exists("z")) {
        throw ParserException("Triangle: missing or invalid 'c' coordinates");
      }
      cx = getFlexibleFloat(cSet["x"]);
      cy = getFlexibleFloat(cSet["y"]);
      cz = getFlexibleFloat(cSet["z"]);
    } else {
      throw ParserException("Triangle: missing 'c' vertex");
    }

    // Parse color
    Color color;
    if (setting.exists("color")) {
      color = parseColor(setting["color"]);
    } else {
      // Default to white if no color specified
      color = Color(uint8_t(255), uint8_t(255), uint8_t(255));
    }

    // Create the triangle
    auto triangle =
        std::make_shared<Triangle>(Vector3D(ax, ay, az), Vector3D(bx, by, bz),
                                   Vector3D(cx, cy, cz), color);

    // Apply transform if it exists
    if (setting.exists("transform")) {
      applyTransformIfExists(setting, triangle);
    }

    return triangle;
  } catch (const std::exception& e) {
    throw ParserException(std::string("Failed to create Triangle: ") +
                          e.what());
  }
}

}  // namespace RayTracer
