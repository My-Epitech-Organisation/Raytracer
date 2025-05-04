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
    float fov = cameraSetting["fieldOfView"];

    int width = res["width"];
    int height = res["height"];
    
    float posX = pos["x"];
    float posY = pos["y"];
    float posZ = pos["z"];
    
    float rotX = rot["x"];
    float rotY = rot["y"];
    float rotZ = rot["z"];
    
    // Create the camera with the proper constructor
    Camera camera(Vector3D(posX, posY, posZ), width, height, fov);
    // Set the rotation separately
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
