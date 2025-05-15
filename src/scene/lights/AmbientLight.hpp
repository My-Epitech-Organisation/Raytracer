/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight
*/

#ifndef AMBIENTLIGHT_HPP_
#define AMBIENTLIGHT_HPP_

#include "Light.hpp"

namespace RayTracer {

class AmbientLight : public Light {
 public:
  explicit AmbientLight(float ambient);
  float getAmbient() const;
  std::string toString() const override;

 private:
  float _ambient;
};

}  // namespace RayTracer

#endif /* !AMBIENTLIGHT_HPP_ */
