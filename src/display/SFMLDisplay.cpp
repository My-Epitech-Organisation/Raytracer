/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFML display class implementation
*/

#include "SFMLDisplay.hpp"

// Only compile SFML implementation if available
#ifdef SFML_AVAILABLE

#include <iostream>
#include "../core/Ray.hpp"

namespace RayTracer {

SFMLDisplay::SFMLDisplay() : _windowTitle("RayTracer") {}

SFMLDisplay::~SFMLDisplay() {
  if (_window.isOpen()) {
    _window.close();
  }
}

bool SFMLDisplay::render(const Scene& scene) {
  const Camera& camera = scene.getCamera();
  int width = camera.getWidth();
  int height = camera.getHeight();

  // Create the window if it doesn't exist or has different dimensions
  if (!_window.isOpen() || static_cast<int>(_image.getSize().x) != width ||
      static_cast<int>(_image.getSize().y) != height) {
    _window.create(sf::VideoMode(width, height), _windowTitle);
    _image.create(width, height, sf::Color::Black);
    _texture.create(width, height);
    _sprite.setTexture(_texture, true);
  }

  // Render each pixel directly to the SFML image
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // Generate a ray for this pixel
      Ray ray = camera.generateRay(x, y);

      // Trace the ray through the scene
      auto intersection = scene.traceRay(ray);

      // Calculate color for this pixel
      Color pixelColor = Color::BLACK;
      if (intersection) {
        // This is simplified - normally we'd calculate full lighting here
        pixelColor = intersection->color;
      }

      _image.setPixel(x, y, convertColor(pixelColor));
    }

    // Update every few scanlines to show progress
    if (y % 20 == 0) {
      update();
      handleEvents();
    }
  }

  return update();
}

bool SFMLDisplay::renderWithPPM(const Scene& scene, PPMDisplay& ppmDisplay,
                                bool saveToFile, const std::string& filename) {
  const Camera& camera = scene.getCamera();
  int width = camera.getWidth();
  int height = camera.getHeight();

  // Render using PPM display
  if (!ppmDisplay.render(scene)) {
    std::cerr << "Failed to render with PPM display" << std::endl;
    return false;
  }

  // Save to file if requested
  if (saveToFile && !filename.empty()) {
    if (!ppmDisplay.saveToFile(filename)) {
      std::cerr << "Failed to save PPM file" << std::endl;
      return false;
    }
  }

  // Create window if needed
  if (!_window.isOpen() || static_cast<int>(_image.getSize().x) != width ||
      static_cast<int>(_image.getSize().y) != height) {
    _window.create(sf::VideoMode(width, height), _windowTitle);
    _image.create(width, height, sf::Color::Black);
    _texture.create(width, height);
    _sprite.setTexture(_texture, true);
  }

  // Copy PPM pixel data to SFML image
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Color pixelColor = ppmDisplay.getPixel(x, y);
      _image.setPixel(x, y, convertColor(pixelColor));
    }
  }

  return update();
}

bool SFMLDisplay::update() {
  if (!_window.isOpen()) {
    return false;
  }

  _texture.update(_image);
  _window.clear();
  _window.draw(_sprite);
  _window.display();

  return true;
}

void SFMLDisplay::close() {
  if (_window.isOpen()) {
    _window.close();
  }
}

bool SFMLDisplay::isOpen() const {
  return _window.isOpen();
}

bool SFMLDisplay::handleEvents() {
  sf::Event event;
  while (_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      _window.close();
      return false;
    }
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
      _window.close();
      return false;
    }
  }
  return true;
}

sf::Color SFMLDisplay::convertColor(const Color& color) const {
  // Convert from 0-255 range using the proper accessors
  return sf::Color(color.getR(), color.getG(), color.getB());
}

}  // namespace RayTracer

#endif  // SFML_AVAILABLE
