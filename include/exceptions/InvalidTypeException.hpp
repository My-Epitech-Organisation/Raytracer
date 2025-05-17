/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InvalidTypeException
*/

/**
 * @file InvalidTypeException.hpp
 * @brief Exception class for handling invalid type errors in the raytracer
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
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
