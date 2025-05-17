/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RaytracerException
*/

/**
 * @file RaytracerException.hpp
 * @brief Base exception class for all raytracer-specific errors
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef RAYTRACEREXCEPTION_HPP_
#define RAYTRACEREXCEPTION_HPP_

#include <exception>
#include <string>

class RaytracerException : public std::exception {
 public:
  explicit RaytracerException(const std::string& message);
  const char* what() const noexcept override;

 private:
  std::string _msg;
};

#endif /* !RAYTRACEREXCEPTION_HPP_ */
