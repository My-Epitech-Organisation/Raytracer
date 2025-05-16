/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ParserException
*/

/**
 * @file ParserException.hpp
 * @brief Exception class for handling parsing errors in the raytracer
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef PARSEREXCEPTION_HPP_
#define PARSEREXCEPTION_HPP_

#include "RaytracerException.hpp"

namespace RayTracer {

class ParserException : public RaytracerException {
 public:
  explicit ParserException(const std::string& msg) : RaytracerException(msg) {}
};

}  // namespace RayTracer

#endif
