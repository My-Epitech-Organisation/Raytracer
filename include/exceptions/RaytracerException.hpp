/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RaytracerException
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
