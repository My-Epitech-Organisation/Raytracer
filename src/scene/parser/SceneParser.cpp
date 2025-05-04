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

};  // namespace RayTracer
