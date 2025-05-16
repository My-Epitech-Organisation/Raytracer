/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RaytracerException
*/

/**
 * @file RaytracerException.cpp
 * @brief Implementation of the base exception class used throughout the raytracer
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "../../include/exceptions/RaytracerException.hpp"

RaytracerException::RaytracerException(const std::string& message)
    : _msg("RaytracerException: " + message) {}

const char* RaytracerException::what() const noexcept {
  return _msg.c_str();
}
