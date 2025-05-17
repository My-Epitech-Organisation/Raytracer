// Unit tests for Torus primitive
#include <gtest/gtest.h>
#include <cmath>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/Torus.hpp"

using namespace RayTracer;

constexpr double TORUS_EPSILON = 1e-4;

::testing::AssertionResult AssertVectorsNearlyEqualTorusTest(
    const char* v1_expr, const char* v2_expr, const char* epsilon_expr,
    const Vector3D& v1, const Vector3D& v2, double epsilon) {
  double dx = std::abs(v1.getX() - v2.getX());
  double dy = std::abs(v1.getY() - v2.getY());
  double dz = std::abs(v1.getZ() - v2.getZ());
  bool pass = dx < epsilon && dy < epsilon && dz < epsilon;
  if (pass) {
    return ::testing::AssertionSuccess();
  } else {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(9) << "Vectors " << v1_expr << " and "
       << v2_expr << " are not nearly equal (within " << epsilon_expr << ").\n"
       << v1_expr << ": (" << v1.getX() << ", " << v1.getY() << ", "
       << v1.getZ() << ")\n"
       << v2_expr << ": (" << v2.getX() << ", " << v2.getY() << ", "
       << v2.getZ() << ")\n"
       << "Difference: (" << dx << ", " << dy << ", " << dz << ")\n"
       << "Epsilon: " << epsilon;
    return ::testing::AssertionFailure() << ss.str();
  }
}
#define EXPECT_VECTORS_NEARLY_EQUAL_TORUS(v1, v2, epsilon) \
  EXPECT_PRED_FORMAT3(AssertVectorsNearlyEqualTorusTest, v1, v2, epsilon)

TEST(TorusTest, ConstructorValid) {
  Torus torus(10.0, 2.0, Color::RED);
  EXPECT_EQ(torus.getMajorRadius(), 10.0);
  EXPECT_EQ(torus.getTubeRadius(), 2.0);
  EXPECT_EQ(torus.getColor(), Color::RED);
}

TEST(TorusTest, ConstructorInvalid) {
  EXPECT_THROW(Torus(0.0, 2.0, Color::RED), std::invalid_argument);
  EXPECT_THROW(Torus(10.0, 0.0, Color::RED), std::invalid_argument);
  EXPECT_THROW(Torus(-1.0, 2.0, Color::RED), std::invalid_argument);
  EXPECT_THROW(Torus(10.0, -2.0, Color::RED), std::invalid_argument);
}

TEST(TorusTest, RayIntersectionMiss) {
  Torus torus(10.0, 2.0, Color::BLUE);
  Ray ray(Vector3D(0, 0, 30), Vector3D(0, 1, 0));  // Ray far from torus
  auto intersection = torus.intersect(ray);
  EXPECT_FALSE(intersection.has_value());
}

TEST(TorusTest, RayIntersectionHit) {
  Torus torus(10.0, 2.0, Color::GREEN);
  Ray ray(Vector3D(0, 0, 0), Vector3D(1, 0, 0));  // Ray along X axis
  auto intersection = torus.intersect(ray);
  ASSERT_TRUE(intersection.has_value());
  // The intersection should be near (8,0,0) or (12,0,0) (outer/inner tube)
  double t = intersection->distance;
  EXPECT_GT(t, 0.0);
  // The intersection point should be on the torus surface
  double x = intersection->point.getX();
  EXPECT_NEAR(std::abs(std::sqrt(x * x + intersection->point.getZ() *
                                             intersection->point.getZ()) -
                       10.0),
              2.0, 0.1);
  EXPECT_EQ(intersection->color, Color::GREEN);
}

TEST(TorusTest, GetNormalAt) {
  Torus torus(10.0, 2.0, Color::WHITE);
  // Point on the outer tube at (12,0,0)
  Vector3D p(12, 0, 0);
  Vector3D normal = torus.getNormalAt(p);
  EXPECT_NEAR(normal.getY(), 0.0, TORUS_EPSILON);
  EXPECT_GT(normal.getX(), 0.0);
  EXPECT_NEAR(normal.getZ(), 0.0, TORUS_EPSILON);
  // Point on the inner tube at (8,0,0)
  Vector3D p2(8, 0, 0);
  Vector3D normal2 = torus.getNormalAt(p2);
  EXPECT_NEAR(normal2.getY(), 0.0, TORUS_EPSILON);
  EXPECT_LT(normal2.getX(), 0.0);
}

TEST(TorusTest, Clone) {
  Torus original(10.0, 2.0, Color::MAGENTA);
  Transform t;
  t.translate(1, 2, 3);
  t.rotateX(45);
  original.setTransform(t);
  std::shared_ptr<IPrimitive> cloneBase = original.clone();
  ASSERT_NE(cloneBase, nullptr);
  std::shared_ptr<Torus> clone = std::dynamic_pointer_cast<Torus>(cloneBase);
  ASSERT_NE(clone, nullptr);
  EXPECT_EQ(clone->getMajorRadius(), 10.0);
  EXPECT_EQ(clone->getTubeRadius(), 2.0);
  EXPECT_EQ(clone->getColor(), Color::MAGENTA);
}
