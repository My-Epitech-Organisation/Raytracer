/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_color
*/

#include <gtest/gtest.h>
#include "../src/core/Color.hpp"

TEST(ColorTest, Equality) {
  RayTracer::Color c1(255, 128, 64);
  RayTracer::Color c2(255, 128, 64);
  RayTracer::Color c3(255, 0, 0);

  EXPECT_EQ(c1, c2);
  EXPECT_NE(c1, c3);
}
