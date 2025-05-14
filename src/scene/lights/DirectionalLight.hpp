/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight
*/

#ifndef DIRECTIONALLIGHT_HPP_
#define DIRECTIONALLIGHT_HPP_

#include "Light.hpp"

namespace RayTracer {

class DirectionalLight : public Light {
 public:
  explicit DirectionalLight(const Vector3D& direction);
  const Vector3D& getDirection() const;
  std::string toString() const override;

 private:
  Vector3D _direction;
};

}  // namespace RayTracer

#endif /* !DIRECTIONALLIGHT_HPP_ */
