/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Plane
*/

#include <gtest/gtest.h>
#include "../src/scene/primitives/Plane.hpp"

using namespace RayTracer;

TEST(PlaneTest, Equality) {
    Plane p1('Z', -20.0f, Color(64, 64, 255));
    Plane p2 = p1;
    Plane p3('X', -20.0f, Color(64, 64, 255));

    EXPECT_EQ(p1, p2);
    EXPECT_NE(p1, p3);
}
