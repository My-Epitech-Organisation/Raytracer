/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneBuilder class implementation
*/

#include "SceneBuilder.hpp"

namespace RayTracer {

SceneBuilder::SceneBuilder()
    : _camera(),
      _primitives(),
      _lights(),
      _ambientIntensity(0.1),
      _diffuseMultiplier(0.9) {
}

SceneBuilder::~SceneBuilder() {
}

SceneBuilder& SceneBuilder::reset() {
    _camera = Camera();
    _primitives.clear();
    _lights.clear();
    _ambientIntensity = 0.1;
    _diffuseMultiplier = 0.9;
    return *this;
}

SceneBuilder& SceneBuilder::withCamera(const Camera& camera) {
    _camera = camera;
    return *this;
}

SceneBuilder& SceneBuilder::withCameraPosition(const Vector3D& position) {
    _camera.setPosition(position);
    return *this;
}

SceneBuilder& SceneBuilder::withCameraRotation(const Vector3D& rotation) {
    _camera.setRotation(rotation);
    return *this;
}

SceneBuilder& SceneBuilder::withCameraResolution(int width, int height) {
    _camera.setResolution(width, height);
    return *this;
}

SceneBuilder& SceneBuilder::withCameraFieldOfView(double fov) {
    _camera.setFieldOfView(fov);
    return *this;
}

SceneBuilder& SceneBuilder::withPrimitive(std::shared_ptr<IPrimitive> primitive) {
    _primitives.push_back(primitive);
    return *this;
}

SceneBuilder& SceneBuilder::withLight(std::shared_ptr<ILight> light) {
    _lights.push_back(light);
    return *this;
}

SceneBuilder& SceneBuilder::withAmbientLightIntensity(double intensity) {
    _ambientIntensity = intensity;
    return *this;
}

SceneBuilder& SceneBuilder::withDiffuseMultiplier(double multiplier) {
    _diffuseMultiplier = multiplier;
    return *this;
}

Scene SceneBuilder::build() const {
    Scene scene(_camera);
    
    // Set light properties
    scene.setAmbientLightIntensity(_ambientIntensity);
    scene.setDiffuseMultiplier(_diffuseMultiplier);
    
    // Add all primitives
    for (const auto& primitive : _primitives) {
        scene.addPrimitive(primitive);
    }
    
    // Add all lights
    for (const auto& light : _lights) {
        scene.addLight(light);
    }
    
    return scene;
}

} // namespace RayTracer