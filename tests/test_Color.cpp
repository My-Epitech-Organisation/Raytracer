/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Unit tests for Color class
*/

#include <gtest/gtest.h>
#include "../src/core/Color.hpp"

using namespace RayTracer;

// Test constructors
TEST(ColorTest, DefaultConstructor) {
  Color color;
  EXPECT_EQ(color.getR(), 0);
  EXPECT_EQ(color.getG(), 0);
  EXPECT_EQ(color.getB(), 0);
}

TEST(ColorTest, IntegerConstructor) {
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  EXPECT_EQ(color.getR(), 128);
  EXPECT_EQ(color.getG(), 64);
  EXPECT_EQ(color.getB(), 32);
}

TEST(ColorTest, FloatConstructor) {
  Color color(0.5, 0.25, 0.125);
  EXPECT_EQ(color.getR(), 128);
  EXPECT_EQ(color.getG(), 64);
  EXPECT_EQ(color.getB(), 32);
}

TEST(ColorTest, CopyConstructor) {
  Color original(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  Color copy(original);
  EXPECT_EQ(copy.getR(), 128);
  EXPECT_EQ(copy.getG(), 64);
  EXPECT_EQ(copy.getB(), 32);
}

// Test getters/setters
TEST(ColorTest, Getters) {
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  EXPECT_EQ(color.getR(), 128);
  EXPECT_EQ(color.getG(), 64);
  EXPECT_EQ(color.getB(), 32);
  EXPECT_DOUBLE_EQ(color.getRf(), 128.0 / 255.0);
  EXPECT_DOUBLE_EQ(color.getGf(), 64.0 / 255.0);
  EXPECT_DOUBLE_EQ(color.getBf(), 32.0 / 255.0);
}

TEST(ColorTest, IntegerSetters) {
  Color color;
  color.setR(128);
  color.setG(64);
  color.setB(32);
  EXPECT_EQ(color.getR(), 128);
  EXPECT_EQ(color.getG(), 64);
  EXPECT_EQ(color.getB(), 32);
}

TEST(ColorTest, FloatSetters) {
  Color color;
  color.setRf(0.5);
  color.setGf(0.25);
  color.setBf(0.125);
  EXPECT_EQ(color.getR(), 128);
  EXPECT_EQ(color.getG(), 64);
  EXPECT_EQ(color.getB(), 32);
}

TEST(ColorTest, FloatSettersClamp) {
  Color color;
  color.setRf(-0.5);  // Should clamp to 0
  color.setGf(1.5);   // Should clamp to 1
  color.setBf(0.5);
  EXPECT_EQ(color.getR(), 0);
  EXPECT_EQ(color.getG(), 255);
  EXPECT_EQ(color.getB(), 128);
}

// Test operators
TEST(ColorTest, AdditionOperator) {
  Color a(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color b(static_cast<uint8_t>(50), static_cast<uint8_t>(75), static_cast<uint8_t>(100));
  Color result = a + b;
  EXPECT_EQ(result.getR(), 150);
  EXPECT_EQ(result.getG(), 225);
  EXPECT_EQ(result.getB(), 255);  // Saturated to 255
}

TEST(ColorTest, AdditionAssignmentOperator) {
  Color a(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color b(static_cast<uint8_t>(50), static_cast<uint8_t>(75), static_cast<uint8_t>(100));
  a += b;
  EXPECT_EQ(a.getR(), 150);
  EXPECT_EQ(a.getG(), 225);
  EXPECT_EQ(a.getB(), 255);  // Saturated to 255
}

TEST(ColorTest, MultiplicationOperator) {
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  Color result = color * 0.5;
  EXPECT_EQ(result.getR(), 64);
  EXPECT_EQ(result.getG(), 32);
  EXPECT_EQ(result.getB(), 16);
}

TEST(ColorTest, MultiplicationAssignmentOperator) {
  Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  color *= 0.5;
  EXPECT_EQ(color.getR(), 64);
  EXPECT_EQ(color.getG(), 32);
  EXPECT_EQ(color.getB(), 16);
}

TEST(ColorTest, ColorMultiplicationOperator) {
  Color a(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  Color b(static_cast<uint8_t>(128), static_cast<uint8_t>(128), static_cast<uint8_t>(128));
  Color result = a * b;
  // Expected: (128/255 * 128/255, 64/255 * 128/255, 32/255 * 128/255) * 255
  EXPECT_EQ(result.getR(), 64);
  EXPECT_EQ(result.getG(), 32);
  EXPECT_EQ(result.getB(), 16);
}

TEST(ColorTest, ColorMultiplicationAssignmentOperator) {
  Color a(static_cast<uint8_t>(128), static_cast<uint8_t>(64), static_cast<uint8_t>(32));
  Color b(static_cast<uint8_t>(128), static_cast<uint8_t>(128), static_cast<uint8_t>(128));
  a *= b;
  EXPECT_EQ(a.getR(), 64);
  EXPECT_EQ(a.getG(), 32);
  EXPECT_EQ(a.getB(), 16);
}

TEST(ColorTest, EqualityOperator) {
  Color a(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color b(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color c(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(201));
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

TEST(ColorTest, InequalityOperator) {
  Color a(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color b(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color c(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(201));
  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a != c);
}

TEST(ColorTest, IsEqual) {
  Color a(static_cast<uint8_t>(100), static_cast<uint8_t>(150), static_cast<uint8_t>(200));
  Color b(static_cast<uint8_t>(101), static_cast<uint8_t>(151), static_cast<uint8_t>(201));
  Color c(static_cast<uint8_t>(110), static_cast<uint8_t>(160), static_cast<uint8_t>(210));
  EXPECT_TRUE(a.isEqual(b, 1));   // Within tolerance
  EXPECT_FALSE(a.isEqual(c, 5));  // Outside tolerance
}

// Test utility functions
TEST(ColorTest, Blend) {
  Color a(static_cast<uint8_t>(100), static_cast<uint8_t>(100), static_cast<uint8_t>(100));
  Color b(static_cast<uint8_t>(200), static_cast<uint8_t>(200), static_cast<uint8_t>(200));

  // 25% blend
  Color blend1 = a.blend(b, 0.25);
  EXPECT_EQ(blend1.getR(), 125);
  EXPECT_EQ(blend1.getG(), 125);
  EXPECT_EQ(blend1.getB(), 125);

  // 75% blend
  Color blend2 = a.blend(b, 0.75);
  EXPECT_EQ(blend2.getR(), 175);
  EXPECT_EQ(blend2.getG(), 175);
  EXPECT_EQ(blend2.getB(), 175);
}

TEST(ColorTest, ToGrayscale) {
  Color color(static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(0));  // Pure red
  Color gray = color.toGrayscale();

  // Using standard luminance formula: Y = 0.2126*R + 0.7152*G + 0.0722*B
  uint8_t expected = static_cast<uint8_t>(std::round(0.2126 * 255));
  EXPECT_EQ(gray.getR(), expected);
  EXPECT_EQ(gray.getG(), expected);
  EXPECT_EQ(gray.getB(), expected);

  // Check that all components are equal (it's gray)
  EXPECT_EQ(gray.getR(), gray.getG());
  EXPECT_EQ(gray.getG(), gray.getB());
}

TEST(ColorTest, ToRGB) {
  Color color(static_cast<uint8_t>(255), static_cast<uint8_t>(128), static_cast<uint8_t>(64));
  uint32_t rgb = color.toRGB();
  EXPECT_EQ(rgb, 0xFF8040);
}

TEST(ColorTest, FromRGB) {
  Color color = Color::fromRGB(0xFF8040);
  EXPECT_EQ(color.getR(), 255);
  EXPECT_EQ(color.getG(), 128);
  EXPECT_EQ(color.getB(), 64);
}

// Test predefined colors
TEST(ColorTest, PredefinedColors) {
  EXPECT_EQ(Color::BLACK, Color(static_cast<uint8_t>(0), static_cast<uint8_t>(0), static_cast<uint8_t>(0)));
  EXPECT_EQ(Color::WHITE, Color(static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255)));
  EXPECT_EQ(Color::RED, Color(static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(0)));
  EXPECT_EQ(Color::GREEN, Color(static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(0)));
  EXPECT_EQ(Color::BLUE, Color(static_cast<uint8_t>(0), static_cast<uint8_t>(0), static_cast<uint8_t>(255)));
  EXPECT_EQ(Color::YELLOW, Color(static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(0)));
  EXPECT_EQ(Color::CYAN, Color(static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(255)));
  EXPECT_EQ(Color::MAGENTA, Color(static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(255)));
  EXPECT_EQ(Color::GRAY, Color(static_cast<uint8_t>(128), static_cast<uint8_t>(128), static_cast<uint8_t>(128)));
}
