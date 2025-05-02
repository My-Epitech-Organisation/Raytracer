/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere
*/

#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "core/Vector3D.hpp"
#include "core/Color.hpp"

namespace RayTracer {
class Sphere {
    public:
        Sphere(Vector3D center, float radius, Color color)
            : _center(center), _radius(radius), _color(color) {}

        const Vector3D& getCenter() const { return _center; }
        float getRadius() const { return _radius; }
        const Color& getColor() const { return _color; }

        bool operator==(const Sphere& other) const {
            return _center == other._center &&
                _radius == other._radius &&
                _color == other._color;
        }

    private:
        Vector3D _center;
        float _radius;
        Color _color;
};
}  // namespace RayTracer

#endif  /* !SPHERE_HPP_ */