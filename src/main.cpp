/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** main
*/

#include "scene/Scene.hpp"
#include "display/PPMDisplay.hpp"
#include "scene/SceneBuilder.hpp"
#include <iostream>

void usage()
{
    std::cout << "USAGE" << std::endl;
    std::cout << "\t./raytracer <scene_file>" << std::endl;
    std::cout << "DESCRIPTION" << std::endl;
    std::cout << "\t<scene_file>\tPath to the scene file to be rendered." << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage();
        return 84;
    }
    try {
        // launch core
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl; return 84;
    }
    return 0;
}