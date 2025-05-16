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
#include <libconfig.h++>
#include "../../../include/exceptions/InvalidTypeException.hpp"
#include "../../../include/exceptions/ParserException.hpp"
#include "../lights/LightFactory.hpp"

using namespace libconfig;

namespace RayTracer {

Camera SceneParser::parseCamera(const Setting& cameraSetting) {
  try {
    const Setting& res = cameraSetting["resolution"];
    const Setting& pos = cameraSetting["position"];
    const Setting& rot = cameraSetting["rotation"];
    const Setting& fovSetting = cameraSetting["fieldOfView"];

    int width, height;
    res.lookupValue("width", width);
    res.lookupValue("height", height);

    int posX, posY, posZ;
    pos.lookupValue("x", posX);
    pos.lookupValue("y", posY);
    pos.lookupValue("z", posZ);

    int rotX, rotY, rotZ;
    rot.lookupValue("x", rotX);
    rot.lookupValue("y", rotY);
    rot.lookupValue("z", rotZ);

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

  if (transformSetting.exists("translate")) {
    const Setting& translateSetting = transformSetting["translate"];
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

std::pair<LightingSettings, std::vector<std::unique_ptr<Light>>>
SceneParser::parseLightingSettings(const libconfig::Setting& lightsSetting) {
  auto result = LightFactory::createLights(lightsSetting);
  return {result.settings, std::move(result.lights)};
}

void SceneParser::parsePrimitives(const Setting& primitivesSetting) {
  for (int i = 0; i < primitivesSetting.getLength(); ++i) {
    const Setting& primitiveGroup = primitivesSetting[i];
    std::string name = primitiveGroup.getName();

    if (name == "spheres") {
      this->parseSpheres(primitiveGroup);
    } else if (name == "planes") {
      this->parsePlanes(primitiveGroup);
    } else if (name == "cones") {
      this->parseCones(primitiveGroup);
    } else if (name == "limitedcones") {
      this->parseLimitedCones(primitiveGroup);
    } else {
      std::cerr << "Unsupported primitive type: " << name << std::endl;
    }
  }
}

}  // namespace RayTracer
