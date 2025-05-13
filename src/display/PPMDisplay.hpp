/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPM image display class header
*/

#ifndef PPMDISPLAY_HPP_
#define PPMDISPLAY_HPP_

#include <string>
#include <vector>
#include "../core/Color.hpp"
#include "../scene/Scene.hpp"

namespace RayTracer {

/**
 * @brief Class for rendering a scene to a PPM file
 * 
 * This class handles the conversion of a 3D scene to a 2D image
 * and saves it in the PPM (Portable Pixmap) format.
 */
class PPMDisplay {
public:
    /**
     * @brief Default constructor
     */
    PPMDisplay();

    /**
     * @brief Destructor
     */
    ~PPMDisplay();

    /**
     * @brief Render a scene to a pixel buffer
     * @param scene The scene to render
     * @return true if rendering was successful, false otherwise
     */
    bool render(const Scene& scene);

    /**
     * @brief Save the rendered image to a PPM file
     * @param filename The output filename
     * @return true if saving was successful, false otherwise
     */
    bool saveToFile(const std::string& filename) const;

    /**
     * @brief Render a scene and save it to a PPM file
     * @param scene The scene to render
     * @param filename The output filename
     * @return true if rendering and saving was successful, false otherwise
     */
    bool renderToFile(const Scene& scene, const std::string& filename);

    /**
     * @brief Get the color at a specific pixel
     * @param x The x-coordinate of the pixel
     * @param y The y-coordinate of the pixel
     * @return The color at that pixel
     */
    Color getPixel(int x, int y) const;

    /**
     * @brief Set the color at a specific pixel
     * @param x The x-coordinate of the pixel
     * @param y The y-coordinate of the pixel
     * @param color The color to set
     */
    void setPixel(int x, int y, const Color& color);

    /**
     * @brief Get the width of the image
     * @return The width in pixels
     */
    int getWidth() const;

    /**
     * @brief Get the height of the image
     * @return The height in pixels
     */
    int getHeight() const;

    /**
     * @brief Clear the image buffer with a specified color
     * @param color The color to fill with (default is black)
     */
    void clear(const Color& color = Color::BLACK);

private:
    std::vector<Color> _pixelBuffer;  ///< Buffer holding the pixel data
    int _width;                       ///< Width of the image in pixels
    int _height;                      ///< Height of the image in pixels

    /**
     * @brief Calculate the color for a pixel by tracing a ray through the scene
     * @param scene The scene to render
     * @param x The x-coordinate of the pixel
     * @param y The y-coordinate of the pixel
     * @return The calculated color
     */
    Color calculatePixelColor(const Scene& scene, int x, int y) const;

    /**
     * @brief Calculate lighting for an intersection point
     * @param scene The scene containing the lights
     * @param intersection The intersection data
     * @return The calculated color including lighting effects
     */
    Color calculateLighting(const Scene& scene, const Intersection& intersection) const;
};

} // namespace RayTracer

#endif /* !PPMDISPLAY_HPP_ */