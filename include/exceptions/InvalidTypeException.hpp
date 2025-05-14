/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InvalidTypeException
*/

#ifndef INVALIDTYPEEXCEPTION_HPP_
#define INVALIDTYPEEXCEPTION_HPP_

#include "RaytracerException.hpp"

class InvalidTypeException : public RaytracerException {
 public:
  explicit InvalidTypeException(const std::string& message)
      : RaytracerException(message) {}
};

#endif  // INVALIDTYPEEXCEPTION_HPP_
