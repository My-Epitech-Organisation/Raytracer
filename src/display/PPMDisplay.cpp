/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPM image display class implementation
*/

#include "PPMDisplay.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "../core/Ray.hpp"

namespace RayTracer {

PPMDisplay::PPMDisplay()
    : _pixelBuffer(),
      _width(0),
      _height(0) {
}

PPMDisplay::~PPMDisplay() {
}

bool PPMDisplay::render(const Scene& scene) {
    const Camera& camera = scene.getCamera();
    _width = camera.getWidth();
    _height = camera.getHeight();
    
    // Resize the pixel buffer to match the camera resolution
    _pixelBuffer.resize(_width * _height);
    
    // Render each pixel
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            Color pixelColor = calculatePixelColor(scene, x, y);
            setPixel(x, y, pixelColor);
        }
    }
    
    return true;
}

bool PPMDisplay::saveToFile(const std::string& filename) const {
    // Check if we have pixel data to save
    if (_pixelBuffer.empty() || _width <= 0 || _height <= 0) {
        std::cerr << "No image data to save." << std::endl;
        return false;
    }
    
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return false;
    }
    
    // Write PPM header
    // P3 format is ASCII, P6 is binary
    file << "P3" << std::endl;
    file << _width << " " << _height << std::endl;
    file << "255" << std::endl; // Max color value
    
    // Write pixel data
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            Color color = getPixel(x, y);
            file << static_cast<int>(color.getR()) << " "
                 << static_cast<int>(color.getG()) << " "
                 << static_cast<int>(color.getB()) << " ";
        }
        file << std::endl;
    }
    
    file.close();
    return true;
}

bool PPMDisplay::renderToFile(const Scene& scene, const std::string& filename) {
    if (!render(scene)) {
        return false;
    }
    return saveToFile(filename);
}

Color PPMDisplay::getPixel(int x, int y) const {
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        return Color::BLACK; // Return black for out-of-bounds pixels
    }
    return _pixelBuffer[y * _width + x];
}

void PPMDisplay::setPixel(int x, int y, const Color& color) {
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        return; // Ignore out-of-bounds pixels
    }
    _pixelBuffer[y * _width + x] = color;
}

int PPMDisplay::getWidth() const {
    return _width;
}

int PPMDisplay::getHeight() const {
    return _height;
}

void PPMDisplay::clear(const Color& color) {
    std::fill(_pixelBuffer.begin(), _pixelBuffer.end(), color);
}

Color PPMDisplay::calculatePixelColor(const Scene& scene, int x, int y) const {
    const Camera& camera = scene.getCamera();
    
    // Generate a ray for this pixel
    Ray ray = camera.generateRay(x, y);
    
    // Trace the ray through the scene
    auto intersection = scene.traceRay(ray);
    
    // If we didn't hit anything, return black (or background color)
    if (!intersection) {
        return Color::BLACK;
    }
    
    // Calculate lighting for the intersection point
    return calculateLighting(scene, *intersection);
}

Color PPMDisplay::calculateLighting(const Scene& scene, const Intersection& intersection) const {
    // Start with ambient light
    double ambientIntensity = scene.getAmbientLightIntensity();
    Color baseColor = intersection.color;
    Color resultColor = baseColor * ambientIntensity;
    
    // Add contribution from each light source
    for (const auto& light : scene.getLights()) {
        // Skip if the point is in shadow from this light
        if (scene.isInShadow(intersection.point, light)) {
            continue;
        }
        
        // Get the light direction and intensity
        Vector3D lightDir = light->getDirectionFrom(intersection.point);
        double intensity = light->getIntensityAt(intersection.point);
        
        // Calculate diffuse lighting (Lambert's law)
        double diffuseFactor = std::max(0.0, intersection.normal.dot(lightDir));
        diffuseFactor *= scene.getDiffuseMultiplier() * intensity;
        
        // Add diffuse component to the result
        resultColor += baseColor * diffuseFactor;
    }
    
    // Clamp the result to valid color range (the Color class should handle this)
    return resultColor;
}

} // namespace RayTracer