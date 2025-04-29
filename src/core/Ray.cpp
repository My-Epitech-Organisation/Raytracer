/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray class implementation
*/

#include "Ray.hpp"
#include "Transform.hpp"

namespace RayTracer {

    Ray::Ray() : _origin(0, 0, 0), _direction(0, 0, 1)
    {
        // Default constructor initializes origin at (0,0,0) and direction towards positive Z
    }

    Ray::Ray(const Vector3D &origin, const Vector3D &direction)
        : _origin(origin), _direction(direction.normalized())
    {
        // Constructor with origin and direction normalizes the direction vector
    }

    Ray::Ray(const Ray &other)
        : _origin(other._origin), _direction(other._direction)
    {
        // Copy constructor
    }

    Ray &Ray::operator=(const Ray &other)
    {
        if (this != &other) {
            _origin = other._origin;
            _direction = other._direction;
        }
        return *this;
    }

    Ray::~Ray()
    {
        // Destructor
    }

    Vector3D Ray::getOrigin() const
    {
        return _origin;
    }

    Vector3D Ray::getDirection() const
    {
        return _direction;
    }

    void Ray::setOrigin(const Vector3D &origin)
    {
        _origin = origin;
    }

    void Ray::setDirection(const Vector3D &direction)
    {
        _direction = direction.normalized();
    }

    Vector3D Ray::pointAt(double t) const
    {
        // Calculate a point along the ray at distance t
        return _origin + (_direction * t);
    }

    Ray Ray::transform(const Transform &transform) const
    {
        // Create a new ray with transformed origin and direction
        Vector3D newOrigin = transform.applyToPoint(_origin);
        Vector3D newDirection = transform.applyToVector(_direction);

        return Ray(newOrigin, newDirection);
    }

} // namespace RayTracer