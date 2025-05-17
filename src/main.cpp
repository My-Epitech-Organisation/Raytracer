/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main entry point for the raytracer
*/

/**
 * @file main.cpp
 * @brief Main entry point for the Raytracer application, handling command-line
 * arguments, scene loading, and rendering
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include <signal.h>
#include <iostream>
#include <libconfig.h++>
#include <memory>
#include <string>
#include "display/PPMDisplay.hpp"
#include "display/SFMLDisplay.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "scene/lights/LightFactory.hpp"
#include "scene/parser/SceneParser.hpp"
#include "scene/primitives/PrimitiveFactory.hpp"

void usage() {
  std::cout << "USAGE: ./raytracer <SCENE_FILE> [OPTIONS]" << std::endl;
  std::cout << "SCENE_FILE: scene configuration" << std::endl;
  std::cout << "OPTIONS:" << std::endl;
  std::cout << "  --display, -d    Display render in SFML window" << std::endl;
}

bool hasDisplayFlag(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--display" || arg == "-d") {
      return true;
    }
  }
  return false;
}

std::string getSceneFilePath(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg != "--display" && arg != "-d" && arg != "--help") {
      return arg;
    }
  }
  return "";
}

RayTracer::Scene buildSceneFromFile(const std::string& filePath) {
  libconfig::Config cfg;
  cfg.readFile(filePath.c_str());

  // Create a scene builder
  RayTracer::SceneBuilder builder;
  RayTracer::SceneParser parser;

  // Parse camera settings
  const libconfig::Setting& cameraSetting = cfg.lookup("camera");
  RayTracer::Camera camera = parser.parseCamera(cameraSetting);
  builder.withCamera(camera);

  // Parse primitives using the PrimitiveFactory
  const libconfig::Setting& primitivesSetting = cfg.lookup("primitives");
  try {
    auto primitiveResult =
        RayTracer::PrimitiveFactory::createPrimitives(primitivesSetting);

    // Add all created primitives to the scene
    for (const auto& primitive : primitiveResult.primitives) {
      builder.withPrimitive(primitive);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error creating primitives: " << e.what() << std::endl;
    throw;
  }

  // Parse tori if they exist
  if (primitivesSetting.exists("torus")) {
    const libconfig::Setting& toriSetting = primitivesSetting["torus"];
    auto tori = parser.parseTori(toriSetting);
    for (const auto& torus : tori) {
      builder.withPrimitive(std::make_shared<RayTracer::Torus>(torus));
    }
  }

  // Parse lights
  if (cfg.exists("lights")) {
    const libconfig::Setting& lightsSetting = cfg.lookup("lights");

    try {
      // Use LightFactory to create all lights
      auto lightResult = RayTracer::LightFactory::createLights(lightsSetting);

      // Set the diffuse multiplier from the light factory result
      builder.withDiffuseMultiplier(lightResult.settings.diffuse);

      // Add all created lights to the scene
      for (auto& light : lightResult.lights) {
        builder.withLight(std::shared_ptr<RayTracer::Light>(light.release()));
      }
    } catch (const std::exception& e) {
      std::cerr << "Error parsing lights: " << e.what() << std::endl;
      throw;
    }
  }

  return builder.build();
}

std::string generateOutputFilename(const std::string& inputFile) {
  return inputFile.substr(0, inputFile.find_last_of('.')) + ".ppm";
}

bool renderToPPM(const RayTracer::Scene& scene,
                 const std::string& outputFilename) {
  std::cout << "Rendering scene to " << outputFilename << "..." << std::endl;

  RayTracer::PPMDisplay ppmDisplay;
  if (!ppmDisplay.renderToFile(scene, outputFilename)) {
    std::cerr << "Error: Failed to render scene" << std::endl;
    return false;
  }

  std::cout << "Rendering complete! Output saved to " << outputFilename
            << std::endl;
  return true;
}

bool renderScene(const RayTracer::Scene& scene,
                 const std::string& outputFilename, bool useDisplay) {
#ifdef SFML_AVAILABLE
  if (useDisplay) {
    std::cout << "Rendering scene with SFML display..." << std::endl;

    RayTracer::SFMLDisplay sfmlDisplay;
    RayTracer::PPMDisplay ppmDisplay;

    if (!sfmlDisplay.renderWithPPM(scene, ppmDisplay, true, outputFilename)) {
      std::cerr << "Error: Failed to render scene" << std::endl;
      return false;
    }

    std::cout << "Rendering complete! Output saved to " << outputFilename
              << std::endl;

    while (sfmlDisplay.isOpen()) {
      if (!sfmlDisplay.handleEvents()) {
        break;
      }
    }

    return true;
  }
#else
  if (useDisplay) {
    std::cerr << "Warning: SFML display requested but SFML is not available. "
              << "Falling back to PPM output only." << std::endl;
  }
#endif

  return renderToPPM(scene, outputFilename);
}

int main(int argc, char** argv) {
  // Check if we have enough arguments
  if (argc < 2) {
    usage();
    return 84;
  }

  // Handle --help flag
  if (std::string(argv[1]) == "--help") {
    usage();
    return 0;
  }

  // Get scene file and check display flag
  std::string sceneFile = getSceneFilePath(argc, argv);
  bool useDisplay = hasDisplayFlag(argc, argv);

  if (sceneFile.empty()) {
    std::cerr << "Error: No scene file provided" << std::endl;
    usage();
    return 84;
  }

  // Handle interruption signals
  signal(SIGINT, [](int) {
    std::cout << "\nRendering interrupted by user." << std::endl;
    exit(0);
  });
  signal(SIGTERM, [](int) {
    std::cout << "\nRendering interrupted." << std::endl;
    exit(0);
  });

  try {
    // Build scene from file
    RayTracer::Scene scene = buildSceneFromFile(sceneFile);

    // Generate output filename
    std::string outputFilename = generateOutputFilename(sceneFile);

    // Render the scene
    if (!renderScene(scene, outputFilename, useDisplay)) {
      return 84;
    }

  } catch (const libconfig::FileIOException& e) {
    std::cerr << "Error: Cannot read scene file: " << sceneFile << std::endl;
    return 84;
  } catch (const libconfig::ParseException& e) {
    std::cerr << "Error: Parse error in scene file " << e.getFile()
              << " at line " << e.getLine() << ": " << e.getError()
              << std::endl;
    return 84;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 84;
  }

  return 0;
}
