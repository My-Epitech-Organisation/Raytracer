/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFML display class header
*/

/**
 * @file SFMLDisplay.hpp
 * @brief Defines the SFMLDisplay class which handles real-time rendering using
 * the SFML library
 * @author @paul-antoine
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef SFMLDISPLAY_HPP_
#define SFMLDISPLAY_HPP_

#include <atomic>
#include <string>
#include <thread>
#include "../core/Color.hpp"
#include "../core/RenderTile.hpp"
#include "../scene/Scene.hpp"
#include "PPMDisplay.hpp"

// Check if SFML is available
#ifdef SFML_AVAILABLE

#include <SFML/Graphics.hpp>

namespace RayTracer {

/**
 * @brief Class for rendering a scene to an SFML window
 *
 * This class handles the conversion of a 3D scene to a 2D image
 * and displays it in an SFML window.
 */
class SFMLDisplay {
 public:
  /**
   * @brief Default constructor
   */
  SFMLDisplay();

  /**
   * @brief Destructor
   */
  ~SFMLDisplay();

  /**
   * @brief Render a scene to the SFML window
   * @param scene The scene to render
   * @return true if rendering was successful, false otherwise
   */
  bool render(const Scene& scene);

  /**
   * @brief Render a scene using PPM display and show it in SFML window
   * @param scene The scene to render
   * @param ppmDisplay The PPM display to use for rendering
   * @param saveToFile Whether to save the result to a PPM file as well
   * @param filename The filename to save to if saveToFile is true
   * @return true if rendering was successful, false otherwise
   */
  bool renderWithPPM(const Scene& scene, PPMDisplay& ppmDisplay,
                     bool saveToFile = false, const std::string& filename = "");

  /**
   * @brief Update a specific tile in the window
   * @param ppmDisplay The PPM display with the pixel data
   * @param tile The tile to update
   */
  void updateTile(const PPMDisplay& ppmDisplay, const RenderTile& tile);

  /**
   * @brief Update the window to show the current pixel buffer
   * @return true if update was successful, false otherwise
   */
  bool update();

  /**
   * @brief Close the SFML window
   */
  void close();

  /**
   * @brief Check if the SFML window is open
   * @return true if the window is open, false otherwise
   */
  bool isOpen() const;

  /**
   * @brief Handle window events (close button, etc.)
   * @return true if the window should stay open, false if it should close
   */
  bool handleEvents();

 private:
  sf::RenderWindow _window;  ///< SFML render window
  sf::Texture _texture;      ///< SFML texture for the rendered image
  sf::Sprite _sprite;        ///< SFML sprite to display the texture
  sf::Image _image;          ///< SFML image to hold pixel data
  std::string _windowTitle;  ///< Window title
  std::atomic<bool>
      _isRendering;  ///< Flag to indicate if rendering is in progress
  std::thread
      _updateThread;  ///< Thread for updating the display during rendering

  /**
   * @brief Convert a RayTracer::Color to sf::Color
   * @param color The RayTracer color to convert
   * @return The equivalent SFML color
   */
  sf::Color convertColor(const Color& color) const;

  /**
   * @brief Background thread for updating the display during rendering
   * @param ppmDisplay The PPM display to monitor for updates
   */
  void displayUpdateThread(const PPMDisplay* ppmDisplay);
};

}  // namespace RayTracer

#else  // SFML_AVAILABLE

// Define an empty SFMLDisplay class to avoid compilation errors
namespace RayTracer {
class SFMLDisplay {
 public:
  SFMLDisplay() {}
  ~SFMLDisplay() {}
  bool render(const Scene&) { return false; }
  bool renderWithPPM(const Scene&, PPMDisplay&, bool = false,
                     const std::string& = "") {
    return false;
  }
  void updateTile(const PPMDisplay&, const RenderTile&) {}
  bool update() { return false; }
  void close() {}
  bool isOpen() const { return false; }
  bool handleEvents() { return false; }
};
}  // namespace RayTracer

#endif  // SFML_AVAILABLE

#endif  // SFMLDISPLAY_HPP_
