/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene class implementation
*/

/**
 * @file Scene.cpp
 * @brief Implementation of the Scene class that manages all 3D objects, lights
 * and rendering environment
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "Scene.hpp"
#include <algorithm>
#include <limits>

namespace RayTracer {

Scene::Scene()
    : _camera(),
      _primitives(),
      _lights(),
      _ambientIntensity(0.1),
      _diffuseMultiplier(0.9) {}

Scene::Scene(const Camera& camera)
    : _camera(camera),
      _primitives(),
      _lights(),
      _ambientIntensity(0.1),
      _diffuseMultiplier(0.9) {}

Scene::~Scene() {}

Scene::Scene(const Scene& other)
    : _camera(other._camera),
      _ambientIntensity(other._ambientIntensity),
      _diffuseMultiplier(other._diffuseMultiplier) {
  // Deep copy primitives
  for (const auto& primitive : other._primitives) {
    _primitives.push_back(primitive->clone());
  }

  // Deep copy lights
  for (const auto& light : other._lights) {
    _lights.push_back(light->clone());
  }
}

Scene& Scene::operator=(const Scene& other) {
  if (this != &other) {
    _camera = other._camera;
    _ambientIntensity = other._ambientIntensity;
    _diffuseMultiplier = other._diffuseMultiplier;

    // Clear current primitives and lights
    _primitives.clear();
    _lights.clear();

    // Deep copy primitives
    for (const auto& primitive : other._primitives) {
      _primitives.push_back(primitive->clone());
    }

    // Deep copy lights
    for (const auto& light : other._lights) {
      _lights.push_back(light->clone());
    }
  }
  return *this;
}

void Scene::addPrimitive(std::shared_ptr<IPrimitive> primitive) {
  _primitives.push_back(primitive);
}

void Scene::addLight(std::shared_ptr<ILight> light) {
  _lights.push_back(light);
}

void Scene::setAmbientLightIntensity(double intensity) {
  _ambientIntensity = std::clamp(intensity, 0.0, 1.0);
}

double Scene::getAmbientLightIntensity() const {
  return _ambientIntensity;
}

void Scene::setDiffuseMultiplier(double multiplier) {
  _diffuseMultiplier = std::clamp(multiplier, 0.0, 1.0);
}

double Scene::getDiffuseMultiplier() const {
  return _diffuseMultiplier;
}

void Scene::setCamera(const Camera& camera) {
  _camera = camera;
}

const Camera& Scene::getCamera() const {
  return _camera;
}

const std::vector<std::shared_ptr<IPrimitive>>& Scene::getPrimitives() const {
  return _primitives;
}

const std::vector<std::shared_ptr<ILight>>& Scene::getLights() const {
  return _lights;
}

std::optional<Intersection> Scene::traceRay(const Ray& ray) const {
  std::optional<Intersection> closestIntersection;
  double closestDistance = std::numeric_limits<double>::infinity();

  for (const auto& primitive : _primitives) {
    auto intersection = primitive->intersect(ray);
    if (intersection && intersection->distance > 0 &&
        intersection->distance < closestDistance) {
      closestIntersection = intersection;
      closestDistance = intersection->distance;
    }
  }

  return closestIntersection;
}

bool Scene::isInShadow(const Vector3D& point,
                       const std::shared_ptr<ILight>& light) const {
  if (!light->castsShadows()) {
    return false;
  }

  Ray shadowRay = light->getShadowRay(point);

  double lightDistance = light->getDistanceFrom(point);

  for (const auto& primitive : _primitives) {
    auto intersection = primitive->intersect(shadowRay);

    if (intersection && intersection->distance > 0.001 &&
        intersection->distance < lightDistance) {
      return true;
    }
  }

  return false;
}

void Scene::clearPrimitives() {
  _primitives.clear();
}

void Scene::clearLights() {
  _lights.clear();
}

}  // namespace RayTracer
