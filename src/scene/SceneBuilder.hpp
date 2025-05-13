/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneBuilder class header
*/

#ifndef SCENEBUILDER_HPP_
#define SCENEBUILDER_HPP_

#include <memory>
#include <string>
#include "Scene.hpp"
#include "Camera.hpp"
#include "../../include/IPrimitive.hpp"
#include "../../include/ILight.hpp"

namespace RayTracer {

/**
 * @brief Builder class for creating Scene objects
 * 
 * This class implements the Builder design pattern to simplify the creation
 * of complex Scene objects.
 */
class SceneBuilder {
public:
    /**
     * @brief Default constructor
     */
    SceneBuilder();

    /**
     * @brief Destructor
     */
    ~SceneBuilder();

    /**
     * @brief Reset the builder to its initial state
     * @return Reference to this builder
     */
    SceneBuilder& reset();

    /**
     * @brief Set the camera for the scene
     * @param camera The camera to use
     * @return Reference to this builder
     */
    SceneBuilder& withCamera(const Camera& camera);

    /**
     * @brief Set the camera position
     * @param position The position vector
     * @return Reference to this builder
     */
    SceneBuilder& withCameraPosition(const Vector3D& position);

    /**
     * @brief Set the camera rotation
     * @param rotation The rotation vector (in degrees)
     * @return Reference to this builder
     */
    SceneBuilder& withCameraRotation(const Vector3D& rotation);

    /**
     * @brief Set the camera resolution
     * @param width Width in pixels
     * @param height Height in pixels
     * @return Reference to this builder
     */
    SceneBuilder& withCameraResolution(int width, int height);

    /**
     * @brief Set the camera field of view
     * @param fov Field of view in degrees
     * @return Reference to this builder
     */
    SceneBuilder& withCameraFieldOfView(double fov);

    /**
     * @brief Add a primitive to the scene
     * @param primitive The primitive to add
     * @return Reference to this builder
     */
    SceneBuilder& withPrimitive(std::shared_ptr<IPrimitive> primitive);

    /**
     * @brief Add a light to the scene
     * @param light The light to add
     * @return Reference to this builder
     */
    SceneBuilder& withLight(std::shared_ptr<ILight> light);

    /**
     * @brief Set the ambient light intensity
     * @param intensity The intensity value [0.0 - 1.0]
     * @return Reference to this builder
     */
    SceneBuilder& withAmbientLightIntensity(double intensity);

    /**
     * @brief Set the diffuse light multiplier
     * @param multiplier The multiplier value [0.0 - 1.0]
     * @return Reference to this builder
     */
    SceneBuilder& withDiffuseMultiplier(double multiplier);

    /**
     * @brief Build and return the Scene object
     * @return The constructed Scene
     */
    Scene build() const;

private:
    Camera _camera;                                      ///< The scene camera
    std::vector<std::shared_ptr<IPrimitive>> _primitives; ///< Primitives to add
    std::vector<std::shared_ptr<ILight>> _lights;         ///< Lights to add
    double _ambientIntensity;                            ///< Ambient light intensity
    double _diffuseMultiplier;                           ///< Diffuse light multiplier
};

} // namespace RayTracer

#endif /* !SCENEBUILDER_HPP_ */