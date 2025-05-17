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
        {"limitedcylinders", [](const Setting& setting) {
           return std::static_pointer_cast<IPrimitive>(
               PrimitiveFactory::createLimitedCylinder(setting));
         }}};

PrimitiveFactory::Result PrimitiveFactory::createPrimitives(
    const Setting& setting) {
  Result result;

  // Iterate through all groups in the primitives section
  for (int i = 0; i < setting.getLength(); ++i) {
    const Setting& primitiveGroup = setting[i];
    std::string typeName = primitiveGroup.getName();

    // Check if this primitive type is supported
    if (primitiveCreators.find(typeName) != primitiveCreators.end()) {
      try {
        // Create primitives of this type and add them to our result
        auto primitivesOfType =
            createPrimitivesOfType(primitiveGroup, typeName);
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
    int x = 0, y = 0, z = 0, r = 1;
    setting.lookupValue("x", x);
    setting.lookupValue("y", y);
    setting.lookupValue("z", z);
    setting.lookupValue("r", r);

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
    const Setting& apexSetting = setting["apex"];
    const Setting& axisSetting = setting["axis"];
    const Setting& angleSetting = setting["angle"];

    // Parse apex coordinates
    int tempApexX = 0, tempApexY = 0, tempApexZ = 0;
    apexSetting.lookupValue("x", tempApexX);
    apexSetting.lookupValue("y", tempApexY);
    apexSetting.lookupValue("z", tempApexZ);
    double apexX = static_cast<double>(tempApexX);
    double apexY = static_cast<double>(tempApexY);
    double apexZ = static_cast<double>(tempApexZ);

    // Parse axis vector
    if (!axisSetting.isGroup()) {
      throw ParserException(
          "Cone 'axis' setting must be a group (e.g., axis = { x = 0.0; y = "
          "1.0; z = 0.0; })");
    }

    double axisX = 0.0, axisY = 1.0, axisZ = 0.0;  // Default to Y-axis

    const Setting& compXSetting = axisSetting["x"];
    axisX = getFlexibleFloat(compXSetting);

    const Setting& compYSetting = axisSetting["y"];
    axisY = getFlexibleFloat(compYSetting);

    const Setting& compZSetting = axisSetting["z"];
    axisZ = getFlexibleFloat(compZSetting);

    if (axisX == 0.0 && axisY == 0.0 && axisZ == 0.0) {
      throw ParserException("Cone axis cannot be a zero vector");
    }

    // Parse cone angle
    double angle = 0.0;
    if (angleSetting.isNumber()) {
      angle = getFlexibleFloat(angleSetting);
    } else {
      throw ParserException("Cone angle must be a number");
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
    const Setting& apexSetting = setting["apex"];
    const Setting& axisSetting = setting["axis"];
    const Setting& angleSetting = setting["angle"];

    // Parse apex coordinates
    int tempApexX = 0, tempApexY = 0, tempApexZ = 0;
    apexSetting.lookupValue("x", tempApexX);
    apexSetting.lookupValue("y", tempApexY);
    apexSetting.lookupValue("z", tempApexZ);
    double apexX = static_cast<double>(tempApexX);
    double apexY = static_cast<double>(tempApexY);
    double apexZ = static_cast<double>(tempApexZ);

    // Parse axis vector
    if (!axisSetting.isGroup()) {
      throw ParserException(
          "LimitedCone 'axis' setting must be a group (e.g., axis = { x = 0.0; "
          "y = 1.0; z = 0.0; })");
    }

    double axisX = 0.0, axisY = 1.0, axisZ = 0.0;  // Default to Y-axis

    const Setting& compXSetting = axisSetting["x"];
    axisX = getFlexibleFloat(compXSetting);

    const Setting& compYSetting = axisSetting["y"];
    axisY = getFlexibleFloat(compYSetting);

    const Setting& compZSetting = axisSetting["z"];
    axisZ = getFlexibleFloat(compZSetting);

    if (axisX == 0.0 && axisY == 0.0 && axisZ == 0.0) {
      throw ParserException("LimitedCone axis cannot be a zero vector");
    }

    // Parse cone angle
    double angle = 0.0;
    if (angleSetting.isNumber()) {
      angle = getFlexibleFloat(angleSetting);
    } else {
      throw ParserException("LimitedCone angle must be a number");
    }

    // Parse height
    double height = 1.0;  // Default height
    if (setting.exists("height")) {
      height = getFlexibleFloat(setting["height"]);
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

void PrimitiveFactory::applyTransformIfExists(
    const Setting& setting, std::shared_ptr<IPrimitive> primitive) {
  if (setting.exists("transform")) {
    const Setting& transformSetting = setting["transform"];
    Transform transform;

    // Apply translation if it exists
    if (transformSetting.exists("translate")) {
      const Setting& translateSetting = transformSetting["translate"];
      double x = 0.0, y = 0.0, z = 0.0;

      if (translateSetting.exists("x")) {
        x = getFlexibleFloat(translateSetting["x"]);
      }

      if (translateSetting.exists("y")) {
        y = getFlexibleFloat(translateSetting["y"]);
      }

      if (translateSetting.exists("z")) {
        z = getFlexibleFloat(translateSetting["z"]);
      }

      transform.translate(x, y, z);
    }

    // Apply rotation if it exists
    if (transformSetting.exists("rotate")) {
      const Setting& rotateSetting = transformSetting["rotate"];
      double x = 0.0, y = 0.0, z = 0.0;

      if (rotateSetting.exists("x")) {
        x = getFlexibleFloat(rotateSetting["x"]);
      }

      if (rotateSetting.exists("y")) {
        y = getFlexibleFloat(rotateSetting["y"]);
      }

      if (rotateSetting.exists("z")) {
        z = getFlexibleFloat(rotateSetting["z"]);
      }

      transform.rotateX(x);
      transform.rotateY(y);
      transform.rotateZ(z);
    }

    // Apply scale if it exists
    if (transformSetting.exists("scale")) {
      const Setting& scaleSetting = transformSetting["scale"];
      double x = 1.0, y = 1.0, z = 1.0;

      if (scaleSetting.exists("x")) {
        x = getFlexibleFloat(scaleSetting["x"]);
      }

      if (scaleSetting.exists("y")) {
        y = getFlexibleFloat(scaleSetting["y"]);
      }

      if (scaleSetting.exists("z")) {
        z = getFlexibleFloat(scaleSetting["z"]);
      }

      transform.scale(x, y, z);
    }

    // Apply the combined transformation to the primitive
    primitive->setTransform(transform);
  }
}

Color PrimitiveFactory::parseColor(const Setting& setting) {
  int r = 255, g = 255, b = 255;  // Default to white

  setting.lookupValue("r", r);
  setting.lookupValue("g", g);
  setting.lookupValue("b", b);

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
