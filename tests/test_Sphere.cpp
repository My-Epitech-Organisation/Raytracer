/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Sphere
*/

#include <gtest/gtest.h>
#include "../src/scene/primitives/Sphere.hpp"

using namespace RayTracer;

TEST(SphereTest, Equality) {
  Sphere s1(Vector3D(0, 0, 0), 25.0f, Color(255, 0, 0));
  Sphere s2(Vector3D(0, 0, 0), 25.0f, Color(255, 0, 0));
  Sphere s3(Vector3D(0, 0, 0), 30.0f, Color(255, 0, 0));

  EXPECT_EQ(s1, s2);
  EXPECT_NE(s1, s3);
}
