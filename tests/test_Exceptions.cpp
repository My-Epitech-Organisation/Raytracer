/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** test_Exceptions
*/

#include <gtest/gtest.h>
#include "exceptions/InvalidTypeException.hpp"
#include "exceptions/ParserException.hpp"
#include "exceptions/RaytracerException.hpp"

TEST(ExceptionsTest, ThrowParserException) {
  EXPECT_THROW(
      { throw RayTracer::ParserException("This is a test"); },
      RayTracer::ParserException);
}

TEST(ExceptionTest, ParserException) {
  try {
    throw RayTracer::ParserException("Missing 'x' field");
  } catch (const RaytracerException& e) {
    EXPECT_EQ(std::string(e.what()), "RaytracerException: Missing 'x' field");
  }
}

TEST(ExceptionTest, InvalidTypeException) {
  try {
    throw InvalidTypeException("Expected float, got string");
  } catch (const RaytracerException& e) {
    EXPECT_EQ(std::string(e.what()),
              "RaytracerException: Expected float, got string");
  }
}
