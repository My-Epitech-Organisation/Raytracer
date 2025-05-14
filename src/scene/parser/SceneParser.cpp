/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
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
    } else {
      std::cerr << "Unsupported primitive type: " << name << std::endl;
    }
  }
}

}  // namespace RayTracer
