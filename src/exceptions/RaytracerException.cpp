/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RaytracerException
*/

#include "../../include/exceptions/RaytracerException.hpp"

RaytracerException::RaytracerException(const std::string& message)
    : _msg("RaytracerException: " + message) {}

const char* RaytracerException::what() const noexcept {
  return _msg.c_str();
}
