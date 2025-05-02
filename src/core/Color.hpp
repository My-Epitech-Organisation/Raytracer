/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color
*/

#ifndef COLOR_HPP_
#define COLOR_HPP_

namespace RayTracer {
class Color {
    public:
        Color(int r, int g, int b) : _r(r), _g(g), _b(b) {}

        int getR() const { return _r; }
        int getG() const { return _g; }
        int getB() const { return _b; }

        bool operator==(const Color& other) const {
            return _r == other._r && _g == other._g && _b == other._b;
        }

    private:
        int _r;
        int _g;
        int _b;
    };
}

#endif  /* !COLOR_HPP_ */