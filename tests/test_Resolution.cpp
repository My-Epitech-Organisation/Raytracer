/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Resolution
*/

#include <gtest/gtest.h>
#include "../src/scene/Resolution.hpp"

TEST(ResolutionTest, Equality) {
    Resolution r1(1920, 1080);
    Resolution r2(1920, 1080);
    Resolution r3(1280, 720);

    EXPECT_EQ(r1, r2);
    EXPECT_NE(r1, r3);
}
