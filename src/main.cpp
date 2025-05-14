/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** main
*/

#include <iostream>
#include <libconfig.h++>
#include <memory>
#include <string>
#include "display/PPMDisplay.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneBuilder.hpp"
#include "scene/lights/Light.hpp"
#include "scene/parser/SceneParser.hpp"

void usage() {
  std::cout << "USAGE: ./raytracer <SCENE_FILE>" << std::endl;
  std::cout << "SCENE_FILE: scene configuration" << std::endl;
}

int main(int argc, char** argv) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    usage();
    return 0;
  }

  if (argc != 2) {
    usage();
    return 84;
  }

  try {
    // Read the scene configuration file
    libconfig::Config cfg;
    cfg.readFile(argv[1]);

    // Create a scene builder
    RayTracer::SceneBuilder builder;
    RayTracer::SceneParser parser;

    // Parse camera settings
    const libconfig::Setting& cameraSetting = cfg.lookup("camera");
    RayTracer::Camera camera = parser.parseCamera(cameraSetting);
    builder.withCamera(camera);

    // Parse primitives
    const libconfig::Setting& primitivesSetting = cfg.lookup("primitives");

    // Parse spheres if they exist
    if (primitivesSetting.exists("spheres")) {
      const libconfig::Setting& spheresSetting = primitivesSetting["spheres"];
      auto spheres = parser.parseSpheres(spheresSetting);
      for (const auto& sphere : spheres) {
        builder.withPrimitive(std::make_shared<RayTracer::Sphere>(sphere));
      }
    }

    // Parse planes if they exist
    if (primitivesSetting.exists("planes")) {
      const libconfig::Setting& planesSetting = primitivesSetting["planes"];
      auto planes = parser.parsePlanes(planesSetting);
      for (const auto& plane : planes) {
        builder.withPrimitive(std::make_shared<RayTracer::Plane>(plane));
      }
    }

    // Parse lights
    if (cfg.exists("lights")) {
      const libconfig::Setting& lightsSetting = cfg.lookup("lights");

      // Set ambient intensity if it exists
      double ambientIntensity = 0.0;
      if (lightsSetting.exists("ambient")) {
        ambientIntensity = lightsSetting["ambient"];
        builder.withAmbientLightIntensity(ambientIntensity);
      }

      // Set diffuse multiplier if it exists
      double diffuseMultiplier = 0.0;
      if (lightsSetting.exists("diffuse")) {
        diffuseMultiplier = lightsSetting["diffuse"];
        builder.withDiffuseMultiplier(diffuseMultiplier);
      }
    }

    // Build the scene
    RayTracer::Scene scene = builder.build();

    // Render the scene to a PPM file
    std::string outputFilename =
        std::string(argv[1]).substr(0, std::string(argv[1]).find_last_of('.')) +
        ".ppm";
    RayTracer::PPMDisplay display;

    std::cout << "Rendering scene to " << outputFilename << "..." << std::endl;

    if (!display.renderToFile(scene, outputFilename)) {
      std::cerr << "Error: Failed to render scene" << std::endl;
      return 84;
    }

    std::cout << "Rendering complete! Output saved to " << outputFilename
              << std::endl;

  } catch (const libconfig::FileIOException& e) {
    std::cerr << "Error: Cannot read scene file: " << argv[1] << std::endl;
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
