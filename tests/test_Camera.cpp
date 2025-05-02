/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Camera
*/

#include <gtest/gtest.h>
#include "../src/scene/Camera.hpp"

using namespace RayTracer;

TEST(CameraTest, Equality) {
  Camera c1(Resolution(1920, 1080), Vector3D(0.0f, -100.0f, 20.0f),
            Vector3D(0.0f, 0.0f, 0.0f), 72.0f);
  Camera c2 = c1;
  Camera c3 = Camera(Resolution(1920, 1080), Vector3D(0.0f, -100.0f, 20.0f),
                     Vector3D(0.0f, 0.0f, 0.0f), 90.0f);

  EXPECT_EQ(c1, c2);
  EXPECT_NE(c1, c3);
}
