/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight
*/

#include "../../core/Vector3D.hpp"
#include "PointLight.hpp"

namespace RayTracer {

PointLight::PointLight(const Vector3D& position) 
: _position(position) {
}

const Vector3D& PointLight::getPosition() const {
    return _position;
}

std::string PointLight::toString() const {
    return "PointLight at (" + std::to_string(_position.getX()) + ", " +
           std::to_string(_position.getY()) + ", " +
           std::to_string(_position.getZ()) + ")";
}

}  // namespace RayTracer
