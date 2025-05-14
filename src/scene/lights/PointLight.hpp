/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight
*/

#ifndef POINTLIGHT_HPP_
#define POINTLIGHT_HPP_

#include <string>
#include "../../core/Vector3D.hpp"
#include "Light.hpp"

namespace RayTracer {

class PointLight : public Light {
  public:
    PointLight(const Vector3D& position);
    const Vector3D& getPosition() const;
    std::string toString() const override;

private:
    Vector3D _position;
};

}  // namespace RayTracer

#endif /* !POINTLIGHT_HPP_ */