/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneParser
*/

#include "SceneParser.hpp"
#include <libconfig.h++>
#include <stdexcept>

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
      throw std::runtime_error("Field of view must be a number");

    Camera camera(Vector3D(posX, posY, posZ), width, height, fov);
    camera.setRotation(Vector3D(rotX, rotY, rotZ));

    return camera;
  } catch (const SettingNotFoundException& e) {
    throw std::runtime_error(std::string("Setting not found: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw std::runtime_error(std::string("Setting type error: ") + e.what());
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error parsing camera: ") + e.what());
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
    throw std::runtime_error(std::string("Setting not found: ") + e.what());
  } catch (const SettingTypeException& e) {
    throw std::runtime_error(std::string("Setting type error: ") + e.what());
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error parsing sphere: ") + e.what());
  }
}

std::vector<Sphere> SceneParser::parseSpheres(const Setting& setting) {
  std::vector<Sphere> spheres;

  for (int i = 0; i < setting.getLength(); ++i) {
    spheres.push_back(parseSphere(setting[i]));
  }

  return spheres;
}

};  // namespace RayTracer
