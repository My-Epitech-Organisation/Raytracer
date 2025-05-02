/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color class implementation
*/

#include "Color.hpp"
#include <cmath>
#include <stdexcept>

namespace RayTracer {

// Define static constant colors
const Color Color::BLACK(static_cast<uint8_t>(0), static_cast<uint8_t>(0),
                         static_cast<uint8_t>(0));
const Color Color::WHITE(static_cast<uint8_t>(255), static_cast<uint8_t>(255),
                         static_cast<uint8_t>(255));
const Color Color::RED(static_cast<uint8_t>(255), static_cast<uint8_t>(0),
                       static_cast<uint8_t>(0));
const Color Color::GREEN(static_cast<uint8_t>(0), static_cast<uint8_t>(255),
                         static_cast<uint8_t>(0));
const Color Color::BLUE(static_cast<uint8_t>(0), static_cast<uint8_t>(0),
                        static_cast<uint8_t>(255));
const Color Color::YELLOW(static_cast<uint8_t>(255), static_cast<uint8_t>(255),
                          static_cast<uint8_t>(0));
const Color Color::CYAN(static_cast<uint8_t>(0), static_cast<uint8_t>(255),
                        static_cast<uint8_t>(255));
const Color Color::MAGENTA(static_cast<uint8_t>(255), static_cast<uint8_t>(0),
                           static_cast<uint8_t>(255));
const Color Color::GRAY(static_cast<uint8_t>(128), static_cast<uint8_t>(128),
                        static_cast<uint8_t>(128));

Color::Color() : _r(0), _g(0), _b(0) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b) {}

Color::Color(double r, double g, double b)
    : _r(floatToByte(clamp(r))),
      _g(floatToByte(clamp(g))),
      _b(floatToByte(clamp(b))) {}

Color::Color(const Color& other) : _r(other._r), _g(other._g), _b(other._b) {}

Color& Color::operator=(const Color& other) {
  if (this != &other) {
    _r = other._r;
    _g = other._g;
    _b = other._b;
  }
  return *this;
}

Color::~Color() {}

uint8_t Color::getR() const {
  return _r;
}

uint8_t Color::getG() const {
  return _g;
}

uint8_t Color::getB() const {
  return _b;
}

double Color::getRf() const {
  return byteToFloat(_r);
}

double Color::getGf() const {
  return byteToFloat(_g);
}

double Color::getBf() const {
  return byteToFloat(_b);
}

void Color::setR(uint8_t r) {
  _r = r;
}

void Color::setG(uint8_t g) {
  _g = g;
}

void Color::setB(uint8_t b) {
  _b = b;
}

void Color::setRf(double r) {
  _r = floatToByte(clamp(r));
}

void Color::setGf(double g) {
  _g = floatToByte(clamp(g));
}

void Color::setBf(double b) {
  _b = floatToByte(clamp(b));
}

Color Color::operator+(const Color& other) const {
  uint16_t r = static_cast<uint16_t>(_r) + static_cast<uint16_t>(other._r);
  uint16_t g = static_cast<uint16_t>(_g) + static_cast<uint16_t>(other._g);
  uint16_t b = static_cast<uint16_t>(_b) + static_cast<uint16_t>(other._b);

  return Color(static_cast<uint8_t>(std::min(r, static_cast<uint16_t>(255))),
               static_cast<uint8_t>(std::min(g, static_cast<uint16_t>(255))),
               static_cast<uint8_t>(std::min(b, static_cast<uint16_t>(255))));
}

Color& Color::operator+=(const Color& other) {
  uint16_t r = static_cast<uint16_t>(_r) + static_cast<uint16_t>(other._r);
  uint16_t g = static_cast<uint16_t>(_g) + static_cast<uint16_t>(other._g);
  uint16_t b = static_cast<uint16_t>(_b) + static_cast<uint16_t>(other._b);

  _r = static_cast<uint8_t>(std::min(r, static_cast<uint16_t>(255)));
  _g = static_cast<uint8_t>(std::min(g, static_cast<uint16_t>(255)));
  _b = static_cast<uint8_t>(std::min(b, static_cast<uint16_t>(255)));

  return *this;
}

Color Color::operator*(double scalar) const {
  scalar = clamp(scalar);
  return Color(static_cast<uint8_t>(std::round(_r * scalar)),
               static_cast<uint8_t>(std::round(_g * scalar)),
               static_cast<uint8_t>(std::round(_b * scalar)));
}

Color& Color::operator*=(double scalar) {
  scalar = clamp(scalar);
  _r = static_cast<uint8_t>(std::round(_r * scalar));
  _g = static_cast<uint8_t>(std::round(_g * scalar));
  _b = static_cast<uint8_t>(std::round(_b * scalar));

  return *this;
}

Color Color::operator*(const Color& other) const {
  return Color(floatToByte(getRf() * other.getRf()),
               floatToByte(getGf() * other.getGf()),
               floatToByte(getBf() * other.getBf()));
}

Color& Color::operator*=(const Color& other) {
  setRf(getRf() * other.getRf());
  setGf(getGf() * other.getGf());
  setBf(getBf() * other.getBf());

  return *this;
}

bool Color::operator==(const Color& other) const {
  return (_r == other._r && _g == other._g && _b == other._b);
}

bool Color::operator!=(const Color& other) const {
  return !(*this == other);
}

bool Color::isEqual(const Color& other, uint8_t tolerance) const {
  return (std::abs(static_cast<int>(_r) - static_cast<int>(other._r)) <=
              tolerance &&
          std::abs(static_cast<int>(_g) - static_cast<int>(other._g)) <=
              tolerance &&
          std::abs(static_cast<int>(_b) - static_cast<int>(other._b)) <=
              tolerance);
}

Color Color::blend(const Color& other, double t) const {
  t = clamp(t);
  double invT = 1.0 - t;

  return Color(_r * invT + other._r * t, _g * invT + other._g * t,
               _b * invT + other._b * t);
}

Color Color::toGrayscale() const {
  uint8_t gray =
      static_cast<uint8_t>(std::round(0.2126 * _r + 0.7152 * _g + 0.0722 * _b));

  return Color(gray, gray, gray);
}

uint32_t Color::toRGB() const {
  return (static_cast<uint32_t>(_r) << 16) | (static_cast<uint32_t>(_g) << 8) |
         static_cast<uint32_t>(_b);
}

Color Color::fromRGB(uint32_t rgb) {
  uint8_t r = (rgb >> 16) & 0xFF;
  uint8_t g = (rgb >> 8) & 0xFF;
  uint8_t b = rgb & 0xFF;

  return Color(r, g, b);
}

double Color::clamp(double value) {
  return std::max(0.0, std::min(1.0, value));
}

uint8_t Color::floatToByte(double value) {
  return static_cast<uint8_t>(std::round(value * 255.0));
}

double Color::byteToFloat(uint8_t value) {
  return static_cast<double>(value) / 255.0;
}

std::ostream& operator<<(std::ostream& os, const Color& color) {
  os << "Color(" << static_cast<int>(color.getR()) << ", "
     << static_cast<int>(color.getG()) << ", " << static_cast<int>(color.getB())
     << ")";
  return os;
}

}  // namespace RayTracer
