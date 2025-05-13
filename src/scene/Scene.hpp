/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene class header
*/

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>
#include <vector>
#include <optional>
#include "../../include/ILight.hpp"
#include "../../include/IPrimitive.hpp"
#include "Camera.hpp"

namespace RayTracer {

/**
 * @brief Class representing a 3D scene to be rendered
 * 
 * This class manages all the elements of a scene: primitives, lights, and camera.
 * It provides functionality to trace rays through the scene and find intersections.
 */
class Scene {
public:
    /**
     * @brief Default constructor
     */
    Scene();

    /**
     * @brief Constructor with camera
     * @param camera The camera to use for this scene
     */
    explicit Scene(const Camera& camera);

    /**
     * @brief Destructor
     */
    ~Scene();

    /**
     * @brief Copy constructor
     * @param other The scene to copy
     */
    Scene(const Scene& other);

    /**
     * @brief Assignment operator
     * @param other The scene to assign from
     * @return Reference to this scene after assignment
     */
    Scene& operator=(const Scene& other);

    /**
     * @brief Add a primitive to the scene
     * @param primitive The primitive to add
     */
    void addPrimitive(std::shared_ptr<IPrimitive> primitive);

    /**
     * @brief Add a light to the scene
     * @param light The light to add
     */
    void addLight(std::shared_ptr<ILight> light);

    /**
     * @brief Set the ambient light intensity
     * @param intensity The intensity value [0.0 - 1.0]
     */
    void setAmbientLightIntensity(double intensity);

    /**
     * @brief Get the ambient light intensity
     * @return The current ambient light intensity
     */
    double getAmbientLightIntensity() const;

    /**
     * @brief Set the diffuse light multiplier
     * @param multiplier The multiplier value [0.0 - 1.0]
     */
    void setDiffuseMultiplier(double multiplier);

    /**
     * @brief Get the diffuse light multiplier
     * @return The current diffuse light multiplier
     */
    double getDiffuseMultiplier() const;

    /**
     * @brief Set the camera for this scene
     * @param camera The camera to use
     */
    void setCamera(const Camera& camera);

    /**
     * @brief Get the camera of this scene
     * @return The current camera
     */
    const Camera& getCamera() const;

    /**
     * @brief Get all primitives in the scene
     * @return Vector of primitive pointers
     */
    const std::vector<std::shared_ptr<IPrimitive>>& getPrimitives() const;

    /**
     * @brief Get all lights in the scene
     * @return Vector of light pointers
     */
    const std::vector<std::shared_ptr<ILight>>& getLights() const;

    /**
     * @brief Trace a ray through the scene and find the closest intersection
     * @param ray The ray to trace
     * @return The closest intersection if any, std::nullopt otherwise
     */
    std::optional<Intersection> traceRay(const Ray& ray) const;

    /**
     * @brief Check if a point is in shadow from a specific light
     * @param point The point to check
     * @param light The light source
     * @return true if in shadow, false otherwise
     */
    bool isInShadow(const Vector3D& point, const std::shared_ptr<ILight>& light) const;

    /**
     * @brief Clear all primitives from the scene
     */
    void clearPrimitives();

    /**
     * @brief Clear all lights from the scene
     */
    void clearLights();

private:
    Camera _camera;                                      ///< The scene camera
    std::vector<std::shared_ptr<IPrimitive>> _primitives; ///< All primitives in the scene
    std::vector<std::shared_ptr<ILight>> _lights;         ///< All lights in the scene
    double _ambientIntensity;                            ///< Ambient light intensity [0.0 - 1.0]
    double _diffuseMultiplier;                           ///< Diffuse light multiplier [0.0 - 1.0]
};

} // namespace RayTracer

#endif /* !SCENE_HPP_ */