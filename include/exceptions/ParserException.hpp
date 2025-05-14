/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ParserException
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
