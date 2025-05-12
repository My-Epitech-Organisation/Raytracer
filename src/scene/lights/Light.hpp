/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Abstract Light class
*/

#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include <string>
#include <vector>
#include "../../core/Vector3D.hpp"

namespace RayTracer {

class Light {
 public:
  Light();
  Light(float ambient, float diffuse, const std::vector<Vector3D>& pointLights,
        const std::vector<Vector3D>& directionalLights);

  float getAmbient() const;
  float getDiffuse() const;

  const std::vector<Vector3D>& getPointLights() const;
  const std::vector<Vector3D>& getDirectionalLights() const;

  void addPointLight(const Vector3D& light);
  void addDirectionalLight(const Vector3D& light);

  std::string toString() const;

 private:
  float _ambient;
  float _diffuse;
  std::vector<Vector3D> _pointLights;
  std::vector<Vector3D> _directionalLights;
};

}  // namespace RayTracer

#endif /* !LIGHT_HPP_ */
