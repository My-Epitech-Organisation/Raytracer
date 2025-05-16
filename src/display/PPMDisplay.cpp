/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPM image display class implementation
*/

#include "PPMDisplay.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include "../core/Ray.hpp"
#include "../scene/lights/AmbientLight.hpp"

namespace RayTracer {

PPMDisplay::PPMDisplay()
    : _pixelBuffer(),
      _width(0),
      _height(0),
      _threadPool(nullptr),
      _tileManager(nullptr),
      _renderingActive(true) {}

PPMDisplay::~PPMDisplay() {
  stopRendering();
}

bool PPMDisplay::render(const Scene& scene) {
  const Camera& camera = scene.getCamera();
  _width = camera.getWidth();
  _height = camera.getHeight();

  // Resize the pixel buffer to match the camera resolution
  _pixelBuffer.resize(_width * _height);

  // Create a thread pool with number of cores - 1 threads
  _threadPool = std::make_unique<ThreadPool>();
  _tileManager =
      std::make_unique<TileManager>(_width, _height, 64);  // 64x64 tiles

  _renderingActive = true;
  _startTime = std::chrono::steady_clock::now();

  // Process tiles until all are complete
  std::vector<std::future<void>> futures;
  RenderTile* tile;

  while ((tile = _tileManager->getNextTile()) != nullptr) {
    futures.push_back(_threadPool->enqueue([this, &scene, tile]() {
      this->renderTile(scene, *tile);
      this->_tileManager->tileCompleted();
      delete tile;
    }));
  }

  // Wait for all tasks to complete
  for (auto& future : futures) {
    future.wait();
  }

  return true;
}

bool PPMDisplay::renderWithProgress(
    const Scene& scene, std::function<void(double, double)> progressCallback) {
  const Camera& camera = scene.getCamera();
  _width = camera.getWidth();
  _height = camera.getHeight();

  // Resize the pixel buffer to match the camera resolution
  _pixelBuffer.resize(_width * _height);

  // Create a thread pool with number of cores - 1 threads
  _threadPool = std::make_unique<ThreadPool>();
  _tileManager =
      std::make_unique<TileManager>(_width, _height, 64);  // 64x64 tiles

  _renderingActive = true;
  _startTime = std::chrono::steady_clock::now();

  // Process tiles asynchronously
  std::vector<std::future<void>> futures;
  int totalTiles = _tileManager->getTotalTiles();

  // Queue all rendering tasks
  RenderTile* tile;
  while ((tile = _tileManager->getNextTile()) != nullptr) {
    futures.push_back(_threadPool->enqueue([this, &scene, tile]() {
      if (this->_renderingActive) {
        this->renderTile(scene, *tile);
        this->_tileManager->tileCompleted();
      }
      delete tile;
    }));
  }

  // Monitor progress in a separate thread
  if (progressCallback) {
    std::thread progressThread([this, totalTiles, progressCallback]() {
      int lastCompleted = 0;

      while (_renderingActive &&
             _tileManager->getCompletedTiles() < totalTiles) {
        int completed = _tileManager->getCompletedTiles();
        if (completed > lastCompleted) {
          auto now = std::chrono::steady_clock::now();
          auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                             now - _startTime)
                             .count() /
                         1000.0;

          double progress = 100.0 * completed / totalTiles;
          double etaSeconds = elapsed * (totalTiles - completed) / completed;

          // Report progress
          progressCallback(progress, etaSeconds);
          lastCompleted = completed;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      // Final progress update (100%)
      if (_renderingActive) {
        progressCallback(100.0, 0.0);
      }
    });

    // Wait for all rendering tasks to complete
    for (auto& future : futures) {
      future.wait();
    }

    progressThread.join();
  } else {
    // If no progress callback, just wait for rendering to complete
    for (auto& future : futures) {
      future.wait();
    }
  }

  return _renderingActive;
}

void PPMDisplay::renderTile(const Scene& scene, const RenderTile& tile) {
  if (!_renderingActive) {
    return;
  }

  // Render all pixels in this tile
  for (int y = tile.getStartY(); y < tile.getEndY(); ++y) {
    for (int x = tile.getStartX(); x < tile.getEndX(); ++x) {
      if (!_renderingActive) {
        return;
      }

      Color pixelColor = calculatePixelColor(scene, x, y);

      // Thread-safe pixel buffer update
      {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        setPixel(x, y, pixelColor);
      }
    }
  }
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
  file << "P6" << std::endl;
  file << _width << " " << _height << std::endl;
  file << "255" << std::endl;  // Max color value

  // Write pixel data in binary format
  for (int y = 0; y < _height; ++y) {
    for (int x = 0; x < _width; ++x) {
      Color color = getPixel(x, y);
      unsigned char r = static_cast<unsigned char>(color.getR());
      unsigned char g = static_cast<unsigned char>(color.getG());
      unsigned char b = static_cast<unsigned char>(color.getB());
      file.write(reinterpret_cast<char*>(&r), 1);
      file.write(reinterpret_cast<char*>(&g), 1);
      file.write(reinterpret_cast<char*>(&b), 1);
    }
  }

  file.close();
  return true;
}

bool PPMDisplay::renderToFile(const Scene& scene, const std::string& filename) {
  // Display progress in the console
  auto progressCallback = [](double progress, double etaSeconds) {
    std::stringstream ss;
    ss << "\rRendering: " << std::fixed << std::setprecision(1) << progress
       << "% complete";

    if (etaSeconds > 0) {
      int minutes = static_cast<int>(etaSeconds) / 60;
      int seconds = static_cast<int>(etaSeconds) % 60;
      ss << " | ETA: ";
      if (minutes > 0) {
        ss << minutes << "m ";
      }
      ss << seconds << "s";
    }

    // Add spaces to overwrite previous line and stay in place
    ss << "                    ";

    std::cout << ss.str() << std::flush;
  };

  if (!renderWithProgress(scene, progressCallback)) {
    std::cout << "\nRendering was interrupted." << std::endl;
    return false;
  }

  std::cout << "\nRendering complete! Saving to file..." << std::endl;
  return saveToFile(filename);
}

Color PPMDisplay::getPixel(int x, int y) const {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    return Color::BLACK;  // Return black for out-of-bounds pixels
  }
  return _pixelBuffer[y * _width + x];
}

void PPMDisplay::setPixel(int x, int y, const Color& color) {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    return;  // Ignore out-of-bounds pixels
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

void PPMDisplay::stopRendering() {
  _renderingActive = false;

  // Wait for thread pool to finish (if it exists)
  _threadPool.reset();
  _tileManager.reset();
}

Color PPMDisplay::calculatePixelColor(const Scene& scene, int x, int y) const {
  const Camera& camera = scene.getCamera();

  Ray ray = camera.generateRay(x, y);

  auto intersection = scene.traceRay(ray);

  if (!intersection) {
    return Color::BLACK;
  }

  return calculateLighting(scene, *intersection);
}

Color PPMDisplay::calculateLighting(const Scene& scene,
                                    const Intersection& intersection) const {
  double ambientIntensity = scene.getAmbientLightIntensity();
  Color baseColor = intersection.color;

  Color ambientColor = Color::WHITE;
  for (const auto& light : scene.getLights()) {
    if (auto ambient = std::dynamic_pointer_cast<AmbientLight>(light)) {
      ambientColor = ambient->getColor();
      break;
    }
  }

  Color resultColor = baseColor * ambientColor * ambientIntensity;

  Vector3D viewDir =
      (scene.getCamera().getPosition() - intersection.point).normalized();

  const double specularStrength = 1.2;
  const double shininess = 24.0;

  for (const auto& light : scene.getLights()) {
    if (scene.isInShadow(intersection.point, light)) {
      continue;
    }

    Vector3D lightDir = light->getDirectionFrom(intersection.point);
    double intensity = light->getIntensityAt(intersection.point);
    
    if (std::dynamic_pointer_cast<AmbientLight>(light)) {
        continue;
    }

    // Calculate diffuse lighting (Lambert's law)
    double diffuseFactor = std::max(0.0, intersection.normal.dot(lightDir));
    diffuseFactor *= scene.getDiffuseMultiplier() * intensity * 1.5;  // Multiplied by 1.5 for stronger diffuse

    Color lightColor = light->getColor();
    
    lightColor = Color(
        static_cast<uint8_t>(std::min(255, static_cast<int>(lightColor.getR() * 1.2))),
        static_cast<uint8_t>(std::min(255, static_cast<int>(lightColor.getG() * 1.2))),
        static_cast<uint8_t>(std::min(255, static_cast<int>(lightColor.getB() * 1.2)))
    );
    
    Color diffuseColor = baseColor * lightColor * diffuseFactor;

    // Add diffuse component to the result
    resultColor += diffuseColor;

    // Calculate specular (Phong model)
    Vector3D reflectDir = reflect(-lightDir, intersection.normal);
    double spec = std::pow(std::max(0.0, viewDir.dot(reflectDir)), shininess);

    Color specular = lightColor * (specularStrength * spec * intensity * 1.8);

    // Add specular component to the result
    resultColor += specular;
  }

  return resultColor;
}

Vector3D PPMDisplay::reflect(const Vector3D& incident,
                             const Vector3D& normal) const {
  return incident - normal * 2.0 * incident.dot(normal);
}

}  // namespace RayTracer
