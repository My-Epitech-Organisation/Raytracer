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
        {"Spheres",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createSphere(setting));
         }},
        {"planes",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createPlane(setting));
         }},
        {"Planes",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createPlane(setting));
         }},
        {"cones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createCone(setting));
         }},
        {"Cones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createCone(setting));
         }},
        {"limitedcones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCone(setting));
         }},
        {"limitedCones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCone(setting));
         }},
        {"LimitedCones",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCone(setting));
         }},
        {"cylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createCylinder(setting));
         }},
        {"Cylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createCylinder(setting));
         }},
        {"limitedcylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCylinder(setting));
         }},
        {"limitedCylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCylinder(setting));
         }},
        {"LimitedCylinders",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCylinder(setting));
         }},
        {"toruses",
         [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTorus(setting));
         }},
        {"Toruses", [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createTorus(setting));
         }}};

PrimitiveFactory::Result PrimitiveFactory::createPrimitives(
    const Setting& setting) {
  Result result;

  // Iterate through all groups in the primitives section
  for (int i = 0; i < setting.getLength(); ++i) {
    const Setting& primitiveGroup = setting[i];
    std::string typeName = primitiveGroup.getName();
    std::string typeNameLower = typeName;

    // Convert to lowercase for case-insensitive comparison
    std::transform(typeNameLower.begin(), typeNameLower.end(),
                   typeNameLower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Check if this primitive type is supported (with case insensitivity)
    auto it = std::find_if(
        primitiveCreators.begin(), primitiveCreators.end(),
        [&typeNameLower](const auto& entry) {
          std::string key = entry.first;
          std::transform(key.begin(), key.end(), key.begin(),
                         [](unsigned char c) { return std::tolower(c); });
          return key == typeNameLower;
        });

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

    Color alternateColor = Color::BLACK;
    double checkSize = 10.0;

    if (setting.exists("checkerboard")) {
      const Setting& checkerboardSetting = setting["checkerboard"];

      if (checkerboardSetting.exists("alternateColor")) {
        alternateColor = parseColor(checkerboardSetting["alternateColor"]);
      }

      if (checkerboardSetting.exists("size")) {
        const Setting& sizeSetting = checkerboardSetting["size"];
        checkSize = getFlexibleFloat(sizeSetting);
        if (checkSize <= 0.0) {
          checkSize = 1.0;  // Clamp to a minimum positive value
        }
      }
    }

    auto plane =
        std::make_shared<Plane>(axis, pos, color, alternateColor, checkSize);

    // Apply any transformation
    applyTransformIfExists(setting, plane);

    return plane;
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
  if (setting.exists("transform")) {
    const Setting& transformSetting = setting["transform"];
    Transform transform;

    // Apply translation if it exists (support both "translate" and
    // "translation")
    if (transformSetting.exists("translate")) {
      const Setting& translateSetting = transformSetting["translate"];
      double x = 0.0, y = 0.0, z = 0.0;
      int xInt = 0, yInt = 0, zInt = 0;

      if (translateSetting.exists("x")) {
        bool hasX = translateSetting.lookupValue("x", x);
        if (!hasX && translateSetting.lookupValue("x", xInt)) {
          x = static_cast<double>(xInt);
        }
      }

      if (translateSetting.exists("y")) {
        bool hasY = translateSetting.lookupValue("y", y);
        if (!hasY && translateSetting.lookupValue("y", yInt)) {
          y = static_cast<double>(yInt);
        }
      }

      if (translateSetting.exists("z")) {
        bool hasZ = translateSetting.lookupValue("z", z);
        if (!hasZ && translateSetting.lookupValue("z", zInt)) {
          z = static_cast<double>(zInt);
        }
      }

      transform.translate(x, y, z);
    } else if (transformSetting.exists("translation")) {
      const Setting& translateSetting = transformSetting["translation"];
      double x = 0.0, y = 0.0, z = 0.0;
      int xInt = 0, yInt = 0, zInt = 0;

      if (translateSetting.exists("x")) {
        bool hasX = translateSetting.lookupValue("x", x);
        if (!hasX && translateSetting.lookupValue("x", xInt)) {
          x = static_cast<double>(xInt);
        }
      }

      if (translateSetting.exists("y")) {
        bool hasY = translateSetting.lookupValue("y", y);
        if (!hasY && translateSetting.lookupValue("y", yInt)) {
          y = static_cast<double>(yInt);
        }
      }

      if (translateSetting.exists("z")) {
        bool hasZ = translateSetting.lookupValue("z", z);
        if (!hasZ && translateSetting.lookupValue("z", zInt)) {
          z = static_cast<double>(zInt);
        }
      }

      transform.translate(x, y, z);
    }

    // Apply rotation if it exists (support both "rotate" and "rotation")
    if (transformSetting.exists("rotate")) {
      const Setting& rotateSetting = transformSetting["rotate"];
      double x = 0.0, y = 0.0, z = 0.0;
      int xInt = 0, yInt = 0, zInt = 0;

      if (rotateSetting.exists("x")) {
        bool hasX = rotateSetting.lookupValue("x", x);
        if (!hasX && rotateSetting.lookupValue("x", xInt)) {
          x = static_cast<double>(xInt);
        }
      }

      if (rotateSetting.exists("y")) {
        bool hasY = rotateSetting.lookupValue("y", y);
        if (!hasY && rotateSetting.lookupValue("y", yInt)) {
          y = static_cast<double>(yInt);
        }
      }

      if (rotateSetting.exists("z")) {
        bool hasZ = rotateSetting.lookupValue("z", z);
        if (!hasZ && rotateSetting.lookupValue("z", zInt)) {
          z = static_cast<double>(zInt);
        }
      }

      transform.rotateX(x);
      transform.rotateY(y);
      transform.rotateZ(z);
    } else if (transformSetting.exists("rotation")) {
      const Setting& rotateSetting = transformSetting["rotation"];
      double x = 0.0, y = 0.0, z = 0.0;
      int xInt = 0, yInt = 0, zInt = 0;

      if (rotateSetting.exists("x")) {
        bool hasX = rotateSetting.lookupValue("x", x);
        if (!hasX && rotateSetting.lookupValue("x", xInt)) {
          x = static_cast<double>(xInt);
        }
      }

      if (rotateSetting.exists("y")) {
        bool hasY = rotateSetting.lookupValue("y", y);
        if (!hasY && rotateSetting.lookupValue("y", yInt)) {
          y = static_cast<double>(yInt);
        }
      }

      if (rotateSetting.exists("z")) {
        bool hasZ = rotateSetting.lookupValue("z", z);
        if (!hasZ && rotateSetting.lookupValue("z", zInt)) {
          z = static_cast<double>(zInt);
        }
      }

      transform.rotateX(x);
      transform.rotateY(y);
      transform.rotateZ(z);
    }

    // Apply scale if it exists
    if (transformSetting.exists("scale")) {
      const Setting& scaleSetting = transformSetting["scale"];
      double x = 1.0, y = 1.0, z = 1.0;
      int xInt = 1, yInt = 1, zInt = 1;

      if (scaleSetting.exists("x")) {
        bool hasX = scaleSetting.lookupValue("x", x);
        if (!hasX && scaleSetting.lookupValue("x", xInt)) {
          x = static_cast<double>(xInt);
        }
      }

      if (scaleSetting.exists("y")) {
        bool hasY = scaleSetting.lookupValue("y", y);
        if (!hasY && scaleSetting.lookupValue("y", yInt)) {
          y = static_cast<double>(yInt);
        }
      }

      if (scaleSetting.exists("z")) {
        bool hasZ = scaleSetting.lookupValue("z", z);
        if (!hasZ && scaleSetting.lookupValue("z", zInt)) {
          z = static_cast<double>(zInt);
        }
      }

      transform.scale(x, y, z);
    }

    // Apply the combined transformation to the primitive
    primitive->setTransform(transform);
  }
}

Color PrimitiveFactory::parseColor(const Setting& setting) {
  int r = 255, g = 255, b = 255;  // Default to white
  double rDouble = 255.0, gDouble = 255.0, bDouble = 255.0;

  // Try to get values as integers first
  bool hasR = setting.lookupValue("r", r);
  bool hasG = setting.lookupValue("g", g);
  bool hasB = setting.lookupValue("b", b);

  // If not available as integers, try as floats
  if (!hasR && setting.lookupValue("r", rDouble)) {
    r = static_cast<int>(rDouble);
  }

  if (!hasG && setting.lookupValue("g", gDouble)) {
    g = static_cast<int>(gDouble);
  }

  if (!hasB && setting.lookupValue("b", bDouble)) {
    b = static_cast<int>(bDouble);
  }

  // Ensure RGB values are within valid range [0, 255]
  r = std::min(255, std::max(0, r));
  g = std::min(255, std::max(0, g));
  b = std::min(255, std::max(0, b));

  return Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
               static_cast<uint8_t>(b));
}

float PrimitiveFactory::getFlexibleFloat(const Setting& setting) {
  switch (setting.getType()) {
    case Setting::TypeInt:
      return static_cast<float>(static_cast<int>(setting));
    case Setting::TypeFloat:
      return static_cast<float>(setting);
    default:
      throw ParserException("Expected a numeric type (int or float)");
  }
}

}  // namespace RayTracer
