/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
*/

/**
 * @file SceneParser.cpp
 * @brief Implementation of the configuration file parser for loading 3D scenes
 * from cfg files
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "SceneParser.hpp"
#include <algorithm>
#include <cctype>
#include <libconfig.h++>
#include "../../../include/exceptions/InvalidTypeException.hpp"
#include "../../../include/exceptions/ParserException.hpp"
#include "../lights/LightFactory.hpp"
#include "../primitives/Cylinder.hpp"
#include "../primitives/PrimitiveFactory.hpp"
#include "../primitives/Torus.hpp"
#include "../primitives/Triangle.hpp"

using namespace libconfig;
namespace RayTracer {

Camera SceneParser::parseCamera(const Setting& cameraSetting) {
  try {
    const Setting& res = cameraSetting["resolution"];
    const Setting& pos = cameraSetting["position"];
    const Setting& rot = cameraSetting["rotation"];
    const Setting& fovSetting = cameraSetting["fieldOfView"];

    int width, height;
    double tempWidth, tempHeight;
    if (res.lookupValue("width", tempWidth)) {
      width = static_cast<int>(tempWidth);
    } else {
      res.lookupValue("width", width);
    }

    if (res.lookupValue("height", tempHeight)) {
      height = static_cast<int>(tempHeight);
    } else {
      res.lookupValue("height", height);
    }

    double posX = 0, posY = 0, posZ = 0;
    try {
      if (pos.exists("x")) {
        if (pos["x"].getType() == Setting::TypeInt) {
          posX = static_cast<double>(static_cast<int>(pos["x"]));
        } else if (pos["x"].getType() == Setting::TypeFloat) {
          posX = static_cast<double>(pos["x"]);
        }
      }

      if (pos.exists("y")) {
        if (pos["y"].getType() == Setting::TypeInt) {
          posY = static_cast<double>(static_cast<int>(pos["y"]));
        } else if (pos["y"].getType() == Setting::TypeFloat) {
          posY = static_cast<double>(pos["y"]);
        }
      }

      if (pos.exists("z")) {
        if (pos["z"].getType() == Setting::TypeInt) {
          posZ = static_cast<double>(static_cast<int>(pos["z"]));
        } else if (pos["z"].getType() == Setting::TypeFloat) {
          posZ = static_cast<double>(pos["z"]);
        }
      }
    } catch (const std::exception& e) {
      throw ParserException(std::string("Error parsing camera position: ") +
                            e.what());
    }

    double rotX = 0, rotY = 0, rotZ = 0;
    try {
      if (rot.exists("x")) {
        if (rot["x"].getType() == Setting::TypeInt) {
          rotX = static_cast<double>(static_cast<int>(rot["x"]));
        } else if (rot["x"].getType() == Setting::TypeFloat) {
          rotX = static_cast<double>(rot["x"]);
        }
      }

      if (rot.exists("y")) {
        if (rot["y"].getType() == Setting::TypeInt) {
          rotY = static_cast<double>(static_cast<int>(rot["y"]));
        } else if (rot["y"].getType() == Setting::TypeFloat) {
          rotY = static_cast<double>(rot["y"]);
        }
      }

      if (rot.exists("z")) {
        if (rot["z"].getType() == Setting::TypeInt) {
          rotZ = static_cast<double>(static_cast<int>(rot["z"]));
        } else if (rot["z"].getType() == Setting::TypeFloat) {
          rotZ = static_cast<double>(rot["z"]);
        }
      }
    } catch (const std::exception& e) {
      throw ParserException(std::string("Error parsing camera rotation: ") +
                            e.what());
    }

    float fov;
    if (fovSetting.isNumber())
      fov = static_cast<float>(fovSetting);
    else
      throw RaytracerException(
          "Field of view has invalid type (expected int or float)");

    Camera camera(Vector3D(posX, posY, posZ), width, height, fov);
    camera.setRotation(Vector3D(rotX, rotY, rotZ));

    return camera;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error: ") + e.what());
  } catch (const std::exception& e) {
    throw RaytracerException(std::string("Error parsing camera: ") + e.what());
  }
}

Sphere SceneParser::parseSphere(const Setting& sphereSetting) {
  try {
    int x, y, z, r;
    sphereSetting.lookupValue("x", x);
    sphereSetting.lookupValue("y", y);
    sphereSetting.lookupValue("z", z);
    sphereSetting.lookupValue("r", r);

    const Setting& colorSetting = sphereSetting["color"];
    int red, green, blue;
    colorSetting.lookupValue("r", red);
    colorSetting.lookupValue("g", green);
    colorSetting.lookupValue("b", blue);

    Color color(static_cast<uint8_t>(red), static_cast<uint8_t>(green),
                static_cast<uint8_t>(blue));
    Sphere sphere(Vector3D(x, y, z), r, color);

    return sphere;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error: ") + e.what());
  } catch (const std::exception& e) {
    throw RaytracerException(std::string("Error parsing sphere: ") + e.what());
  }
}

std::vector<Sphere> SceneParser::parseSpheres(const Setting& setting) {
  std::vector<Sphere> spheres;

  for (int i = 0; i < setting.getLength(); ++i) {
    spheres.push_back(parseSphere(setting[i]));
  }

  return spheres;
}

Plane SceneParser::parsePlane(const Setting& planeSetting) {
  try {
    char axis = 'x';
    std::string axisStr;

    if (planeSetting.lookupValue("axis", axisStr)) {
      if (!axisStr.empty()) {
        axis = axisStr[0];
      }
    }

    float pos = 0.0f;
    const Setting& posSetting = planeSetting.lookup("position");

    if (posSetting.getType() == Setting::TypeFloat) {
      pos = posSetting;
    } else if (posSetting.getType() == Setting::TypeInt) {
      pos = static_cast<float>(int(posSetting));
    } else {
      throw RaytracerException(
          "Position has invalid type (expected int or float)");
    }

    const Setting& colorSetting = planeSetting["color"];
    int red, green, blue;
    colorSetting.lookupValue("r", red);
    colorSetting.lookupValue("g", green);
    colorSetting.lookupValue("b", blue);

    Color color(static_cast<uint8_t>(red), static_cast<uint8_t>(green),
                static_cast<uint8_t>(blue));

    Color alternateColor = Color::BLACK;
    double checkSize = 10.0;

    if (planeSetting.exists("checkerboard")) {
      const Setting& checkerboardSetting = planeSetting["checkerboard"];

      if (checkerboardSetting.exists("alternateColor")) {
        const Setting& altColorSetting = checkerboardSetting["alternateColor"];
        int altRed = 0, altGreen = 0, altBlue = 0;
        altColorSetting.lookupValue("r", altRed);
        altColorSetting.lookupValue("g", altGreen);
        altColorSetting.lookupValue("b", altBlue);
        alternateColor =
            Color(static_cast<uint8_t>(altRed), static_cast<uint8_t>(altGreen),
                  static_cast<uint8_t>(altBlue));
      }

      if (checkerboardSetting.exists("size")) {
        const Setting& sizeSetting = checkerboardSetting["size"];
        if (sizeSetting.isNumber()) {
          checkSize = static_cast<double>(sizeSetting);
          if (checkSize <= 0.0) {
            checkSize = 1.0;  // Clamp to a minimum positive value
          }
        }
      }
    }

    Plane plane(axis, pos, color, alternateColor, checkSize);

    return plane;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error: ") + e.what());
  } catch (const std::exception& e) {
    throw RaytracerException(std::string("Error parsing plane: ") + e.what());
  }
}

std::vector<Plane> SceneParser::parsePlanes(const Setting& setting) {
  std::vector<Plane> planes;

  for (int i = 0; i < setting.getLength(); ++i) {
    planes.push_back(parsePlane(setting[i]));
  }

  return planes;
}

float getFlexibleFloat(const Setting& setting) {
  switch (setting.getType()) {
    case Setting::TypeInt:
      return static_cast<float>(int(setting));
    case Setting::TypeFloat:
      return float(setting);
    default:
      throw InvalidTypeException("Expected a numeric type (int or float)");
  }
}

Transform SceneParser::parseTransform(const Setting& transformSetting) {
  Transform transform;

  // Handle both "translate" and "translation" attributes
  if (transformSetting.exists("translate") ||
      transformSetting.exists("translation")) {
    const Setting& translateSetting = transformSetting.exists("translate")
                                          ? transformSetting["translate"]
                                          : transformSetting["translation"];
    double x, y, z;
    translateSetting.lookupValue("x", x);
    translateSetting.lookupValue("y", y);
    translateSetting.lookupValue("z", z);
    transform.translate(x, y, z);
  }

  if (transformSetting.exists("rotate")) {
    const Setting& rotateSetting = transformSetting["rotate"];
    double x, y, z;
    rotateSetting.lookupValue("x", x);
    rotateSetting.lookupValue("y", y);
    rotateSetting.lookupValue("z", z);
    transform.rotateX(x);
    transform.rotateY(y);
    transform.rotateZ(z);
  }

  if (transformSetting.exists("scale")) {
    const Setting& scaleSetting = transformSetting["scale"];
    double x, y, z;
    scaleSetting.lookupValue("x", x);
    scaleSetting.lookupValue("y", y);
    scaleSetting.lookupValue("z", z);
    transform.scale(x, y, z);
  }

  return transform;
}

Cone SceneParser::parseCone(const Setting& coneSetting) {
  try {
    const Setting& apexSetting = coneSetting["apex"];
    const Setting& axisSetting = coneSetting["axis"];
    const Setting& angleSetting = coneSetting["angle"];

    int tempApexX, tempApexY, tempApexZ;
    apexSetting.lookupValue("x", tempApexX);
    apexSetting.lookupValue("y", tempApexY);
    apexSetting.lookupValue("z", tempApexZ);
    double apexX = static_cast<double>(tempApexX);
    double apexY = static_cast<double>(tempApexY);
    double apexZ = static_cast<double>(tempApexZ);

    std::cout << "DEBUG SceneParser::parseCone: Read apex from config: x="
              << apexX << " (int: " << tempApexX << ")" << ", y=" << apexY
              << " (int: " << tempApexY << ")" << ", z=" << apexZ
              << " (int: " << tempApexZ << ")" << std::endl;

    if (!axisSetting.isGroup()) {
      throw std::runtime_error(
          "Cone 'axis' setting must be a group (e.g., axis = { x = 0.0; y = "
          "1.0; z = 0.0; }). Path: " +
          axisSetting.getPath());
    }

    double axisX, axisY, axisZ;

    const Setting& compXSetting = axisSetting["x"];
    if (!compXSetting.isNumber()) {
      throw SettingTypeException(compXSetting);
    }
    axisX = static_cast<double>(compXSetting);

    const Setting& compYSetting = axisSetting["y"];
    if (!compYSetting.isNumber()) {
      throw SettingTypeException(compYSetting);
    }
    axisY = static_cast<double>(compYSetting);

    const Setting& compZSetting = axisSetting["z"];
    if (!compZSetting.isNumber()) {
      throw SettingTypeException(compZSetting);
    }
    axisZ = static_cast<double>(compZSetting);

    if (axisX == 0.0 && axisY == 0.0 && axisZ == 0.0) {
      throw std::runtime_error("Cone axis cannot be a zero vector.");
    }

    double angle;
    if (angleSetting.isNumber()) {
      angle = static_cast<double>(angleSetting);
    } else {
      throw std::runtime_error("Cone angle must be a number.");
    }
    const Setting& colorSetting = coneSetting["color"];
    int red, green, blue;
    colorSetting.lookupValue("r", red);
    colorSetting.lookupValue("g", green);
    colorSetting.lookupValue("b", blue);

    Color color(static_cast<uint8_t>(red), static_cast<uint8_t>(green),
                static_cast<uint8_t>(blue));

    Cone cone(Vector3D(apexX, apexY, apexZ), Vector3D(axisX, axisY, axisZ),
              angle, color);

    if (coneSetting.exists("transform")) {
      Transform transform = parseTransform(coneSetting["transform"]);
      cone.setTransform(transform);
    }

    return cone;
  } catch (const SettingNotFoundException& e) {
    throw std::runtime_error(std::string("Setting not found in cone: ") +
                             e.what());
  } catch (const SettingTypeException& e) {
    std::string error_msg = "Setting type error in cone. Path: ";
    if (e.getPath() != nullptr) {
      error_msg += e.getPath();
    } else {
      error_msg += "N/A";
    }
    error_msg += ". Error: ";
    error_msg += e.what();
    throw std::runtime_error(error_msg);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error parsing cone: ") + e.what());
  }
}

std::vector<Cone> SceneParser::parseCones(const Setting& setting) {
  std::vector<Cone> cones;
  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw std::runtime_error("Cones setting must be a list or group.");
  }
  for (int i = 0; i < setting.getLength(); ++i) {
    try {
      cones.push_back(parseCone(setting[i]));
    } catch (const std::exception& e) {
      std::cerr << "Failed to parse a cone: " << e.what() << std::endl;
    }
  }
  return cones;
}

LimitedCone SceneParser::parseLimitedCone(const Setting& coneSetting) {
  try {
    const Setting& apexSetting = coneSetting["apex"];
    const Setting& axisSetting = coneSetting["axis"];
    const Setting& angleSetting = coneSetting["angle"];

    int tempApexX, tempApexY, tempApexZ;
    apexSetting.lookupValue("x", tempApexX);
    apexSetting.lookupValue("y", tempApexY);
    apexSetting.lookupValue("z", tempApexZ);
    double apexX = static_cast<double>(tempApexX);
    double apexY = static_cast<double>(tempApexY);
    double apexZ = static_cast<double>(tempApexZ);

    if (!axisSetting.isGroup()) {
      throw std::runtime_error(
          "LimitedCone 'axis' setting must be a group (e.g., axis = { x = 0.0; "
          "y = 1.0; z = 0.0; }). Path: " +
          axisSetting.getPath());
    }

    double axisX, axisY, axisZ;

    const Setting& compXSetting = axisSetting["x"];
    if (!compXSetting.isNumber()) {
      throw SettingTypeException(compXSetting);
    }
    axisX = static_cast<double>(compXSetting);

    const Setting& compYSetting = axisSetting["y"];
    if (!compYSetting.isNumber()) {
      throw SettingTypeException(compYSetting);
    }
    axisY = static_cast<double>(compYSetting);

    const Setting& compZSetting = axisSetting["z"];
    if (!compZSetting.isNumber()) {
      throw SettingTypeException(compZSetting);
    }
    axisZ = static_cast<double>(compZSetting);

    if (axisX == 0.0 && axisY == 0.0 && axisZ == 0.0) {
      throw std::runtime_error("LimitedCone axis cannot be a zero vector.");
    }

    double angle;
    if (angleSetting.isNumber()) {
      angle = static_cast<double>(angleSetting);
    } else {
      throw std::runtime_error("LimitedCone angle must be a number.");
    }

    // Parse the color
    const Setting& colorSetting = coneSetting["color"];
    int red, green, blue;
    colorSetting.lookupValue("r", red);
    colorSetting.lookupValue("g", green);
    colorSetting.lookupValue("b", blue);
    Color color(static_cast<uint8_t>(red), static_cast<uint8_t>(green),
                static_cast<uint8_t>(blue));

    // Parse height (mandatory for LimitedCone constructor)
    double height_val;
    if (coneSetting.exists("height")) {
      const Setting& heightSetting = coneSetting["height"];
      if (heightSetting.isNumber()) {
        height_val = getFlexibleFloat(heightSetting);
      } else {
        throw std::runtime_error(
            "LimitedCone 'height' must be a number. Path: " +
            heightSetting.getPath());
      }
    } else {
      throw std::runtime_error(
          "LimitedCone 'height' is a required field. Path: " +
          coneSetting.getPath() + ".height");
    }

    bool hasCaps_val = true;
    if (coneSetting.exists("hasCaps")) {
      const Setting& hasCapsSetting = coneSetting["hasCaps"];
      if (hasCapsSetting.getType() == Setting::TypeBoolean) {
        hasCaps_val = static_cast<bool>(hasCapsSetting);
      } else {
        throw std::runtime_error(
            "LimitedCone 'hasCaps' must be a boolean. Path: " +
            hasCapsSetting.getPath());
      }
    }

    LimitedCone limitedCone(Vector3D(apexX, apexY, apexZ),
                            Vector3D(axisX, axisY, axisZ), angle, color,
                            height_val, hasCaps_val);

    // Set transform if it exists
    if (coneSetting.exists("transform")) {
      Transform transform = parseTransform(coneSetting["transform"]);
      limitedCone.setTransform(transform);
    }
    return limitedCone;
  } catch (const SettingNotFoundException& e) {
    throw std::runtime_error(
        std::string("Setting not found in limited cone: ") + e.what());
  } catch (const SettingTypeException& e) {
    std::string error_msg = "Setting type error in limited cone. Path: ";
    if (e.getPath() != nullptr) {
      error_msg += e.getPath();
    } else {
      error_msg += "N/A";
    }
    error_msg += ". Error: ";
    error_msg += e.what();
    throw std::runtime_error(error_msg);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error parsing limited cone: ") +
                             e.what());
  }
}

std::vector<LimitedCone> SceneParser::parseLimitedCones(
    const Setting& setting) {
  std::vector<LimitedCone> limitedCones;
  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw std::runtime_error("Limited cones setting must be a list or group.");
  }
  for (int i = 0; i < setting.getLength(); ++i) {
    try {
      limitedCones.push_back(parseLimitedCone(setting[i]));
    } catch (const std::exception& e) {
      std::cerr << "Failed to parse a limited cone: " << e.what() << std::endl;
    }
  }
  return limitedCones;
}

LimitedCylinder SceneParser::parseLimitedCylinder(
    const Setting& cylinderSetting) {
  try {
    double radius = 1.0, height = 1.0;
    int r = 255, g = 255, b = 255;
    if (cylinderSetting.exists("radius"))
      radius = getFlexibleFloat(cylinderSetting["radius"]);
    if (cylinderSetting.exists("height"))
      height = getFlexibleFloat(cylinderSetting["height"]);
    if (cylinderSetting.exists("color")) {
      const Setting& colorSetting = cylinderSetting["color"];
      colorSetting.lookupValue("r", r);
      colorSetting.lookupValue("g", g);
      colorSetting.lookupValue("b", b);
    }
    Color color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                static_cast<uint8_t>(b));
    LimitedCylinder cylinder(radius, height, color);
    if (cylinderSetting.exists("transform")) {
      Transform transform = parseTransform(cylinderSetting["transform"]);
      cylinder.setTransform(transform);
    }
    return cylinder;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in cylinder: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in cylinder: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw RaytracerException(std::string("Error parsing cylinder: ") +
                             e.what());
  }
}

std::vector<LimitedCylinder> SceneParser::parseLimitedCylinders(
    const Setting& setting) {
  std::vector<LimitedCylinder> cylinders;
  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw std::runtime_error("Cylinders setting must be a list or group.");
  }
  for (int i = 0; i < setting.getLength(); ++i) {
    try {
      cylinders.push_back(parseLimitedCylinder(setting[i]));
    } catch (const std::exception& e) {
      std::cerr << "Failed to parse a cylinder: " << e.what() << std::endl;
    }
  }
  return cylinders;
}

Cylinder SceneParser::parseInfiniteCylinder(const Setting& cylinderSetting) {
  try {
    double radius = 1.0;
    int r = 255, g = 255, b = 255;
    if (cylinderSetting.exists("radius"))
      radius = getFlexibleFloat(cylinderSetting["radius"]);
    if (cylinderSetting.exists("color")) {
      const Setting& colorSetting = cylinderSetting["color"];
      colorSetting.lookupValue("r", r);
      colorSetting.lookupValue("g", g);
      colorSetting.lookupValue("b", b);
    }
    Color color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                static_cast<uint8_t>(b));
    Cylinder cylinder(radius, color);
    if (cylinderSetting.exists("transform")) {
      Transform transform = parseTransform(cylinderSetting["transform"]);
      cylinder.setTransform(transform);
    }
    return cylinder;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(
        std::string("Setting not found in infinite cylinder: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(
        std::string("Setting type error in infinite cylinder: ") + e.what());
  } catch (const std::exception& e) {
    throw RaytracerException(std::string("Error parsing infinite cylinder: ") +
                             e.what());
  }
}

std::vector<Cylinder> SceneParser::parseInfiniteCylinders(
    const Setting& setting) {
  std::vector<Cylinder> cylinders;
  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw std::runtime_error(
        "Infinite cylinders setting must be a list or group.");
  }
  for (int i = 0; i < setting.getLength(); ++i) {
    try {
      cylinders.push_back(parseInfiniteCylinder(setting[i]));
    } catch (const std::exception& e) {
      std::cerr << "Failed to parse an infinite cylinder: " << e.what()
                << std::endl;
    }
  }
  return cylinders;
}

std::pair<LightingSettings, std::vector<std::unique_ptr<Light>>>
SceneParser::parseLightingSettings(const libconfig::Setting& lightsSetting) {
  auto result = LightFactory::createLights(lightsSetting);
  return {result.settings, std::move(result.lights)};
}

void SceneParser::parsePrimitives(const Setting& primitivesSetting) {
  try {
    auto primitiveResult =
        PrimitiveFactory::createPrimitives(primitivesSetting);
    for (const auto& primitive : primitiveResult.primitives) {
      _primitives.push_back(primitive);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error creating primitives: " << e.what() << std::endl;
  }
}

Torus SceneParser::parseTorus(const Setting& torusSetting) {
  try {
    double majorRadius = 1.0, tubeRadius = 0.3;
    int r = 255, g = 255, b = 255;
    if (torusSetting.exists("majorRadius"))
      majorRadius = getFlexibleFloat(torusSetting["majorRadius"]);
    if (torusSetting.exists("tubeRadius"))
      tubeRadius = getFlexibleFloat(torusSetting["tubeRadius"]);
    if (torusSetting.exists("color")) {
      const Setting& colorSetting = torusSetting["color"];
      colorSetting.lookupValue("r", r);
      colorSetting.lookupValue("g", g);
      colorSetting.lookupValue("b", b);
    }
    Color color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                static_cast<uint8_t>(b));
    Torus torus(majorRadius, tubeRadius, color);
    if (torusSetting.exists("transform")) {
      Transform transform = parseTransform(torusSetting["transform"]);
      torus.setTransform(transform);
    }
    return torus;
  } catch (const SettingNotFoundException& e) {
    throw ParserException(std::string("Setting not found in torus: ") +
                          e.what());
  } catch (const SettingTypeException& e) {
    throw ParserException(std::string("Setting type error in torus: ") +
                          e.what());
  } catch (const std::exception& e) {
    throw RaytracerException(std::string("Error parsing torus: ") + e.what());
  }
}

std::vector<Torus> SceneParser::parseTori(const Setting& setting) {
  std::vector<Torus> tori;
  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw std::runtime_error("Tori setting must be a list or group.");
  }
  for (int i = 0; i < setting.getLength(); ++i) {
    try {
      tori.push_back(parseTorus(setting[i]));
    } catch (const std::exception& e) {
      std::cerr << "Failed to parse a torus: " << e.what() << std::endl;
    }
  }
  return tori;
}

Triangle SceneParser::parseTriangle(const Setting& triangleSetting) {
  double ax = 0, ay = 0, az = 0, bx = 0, by = 0, bz = 0, cx = 0, cy = 0, cz = 0;
  int r = 255, g = 255, b_col = 255;
  if (triangleSetting.exists("a")) {
    const Setting& aSet = triangleSetting["a"];
    try {
      if (!aSet.exists("x") || !aSet.exists("y") || !aSet.exists("z")) {
        throw ParserException("Triangle: missing or invalid 'a' coordinates");
      }
      ax = getFlexibleFloat(aSet["x"]);
      ay = getFlexibleFloat(aSet["y"]);
      az = getFlexibleFloat(aSet["z"]);
    } catch (const std::exception& e) {
      std::cerr << "Triangle: Exception for 'a': " << e.what() << std::endl;
      throw ParserException(
          "Triangle: missing or invalid 'a' coordinates (type exception)");
    }
  } else {
    throw ParserException("Triangle: missing 'a' vertex");
  }
  if (triangleSetting.exists("b")) {
    const Setting& bSet = triangleSetting["b"];
    try {
      if (!bSet.exists("x") || !bSet.exists("y") || !bSet.exists("z")) {
        throw ParserException("Triangle: missing or invalid 'b' coordinates");
      }
      bx = getFlexibleFloat(bSet["x"]);
      by = getFlexibleFloat(bSet["y"]);
      bz = getFlexibleFloat(bSet["z"]);
    } catch (const std::exception& e) {
      std::cerr << "Triangle: Exception for 'b': " << e.what() << std::endl;
      throw ParserException(
          "Triangle: missing or invalid 'b' coordinates (type exception)");
    }
  } else {
    throw ParserException("Triangle: missing 'b' vertex");
  }
  if (triangleSetting.exists("c")) {
    const Setting& cSet = triangleSetting["c"];
    try {
      if (!cSet.exists("x") || !cSet.exists("y") || !cSet.exists("z")) {
        throw ParserException("Triangle: missing or invalid 'c' coordinates");
      }
      cx = getFlexibleFloat(cSet["x"]);
      cy = getFlexibleFloat(cSet["y"]);
      cz = getFlexibleFloat(cSet["z"]);
    } catch (const std::exception& e) {
      std::cerr << "Triangle: Exception for 'c': " << e.what() << std::endl;
      throw ParserException(
          "Triangle: missing or invalid 'c' coordinates (type exception)");
    }
  } else {
    throw ParserException("Triangle: missing 'c' vertex");
  }
  if (triangleSetting.exists("color")) {
    const Setting& colorSet = triangleSetting["color"];
    colorSet.lookupValue("r", r);
    colorSet.lookupValue("g", g);
    colorSet.lookupValue("b", b_col);
  }
  Color color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
              static_cast<uint8_t>(b_col));
  Triangle tri(Vector3D(ax, ay, az), Vector3D(bx, by, bz), Vector3D(cx, cy, cz),
               color);
  if (triangleSetting.exists("transform")) {
    tri.setTransform(parseTransform(triangleSetting["transform"]));
  }
  return tri;
}

std::vector<Triangle> SceneParser::parseTriangles(const Setting& setting) {
  std::vector<Triangle> triangles;
  if (!setting.isList() && !setting.isGroup() && !setting.isArray()) {
    throw std::runtime_error("Triangles setting must be a list or group.");
  }
  for (int i = 0; i < setting.getLength(); ++i) {
    triangles.push_back(parseTriangle(setting[i]));
  }
  return triangles;
}

}  // namespace RayTracer
