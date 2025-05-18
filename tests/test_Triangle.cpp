#include <gtest/gtest.h>
#include "../src/core/Color.hpp"
#include "../src/core/Ray.hpp"
#include "../src/core/Transform.hpp"
#include "../src/core/Vector3D.hpp"
#include "../src/scene/primitives/Triangle.hpp"

using namespace RayTracer;

TEST(TriangleTest, NormalIsCorrectFor3DTriangle) {
  // Triangle dans le plan XZ
  Triangle tri(Vector3D(0, 0, 0), Vector3D(0, 0, 1), Vector3D(1, 1, 1),
               Color(uint8_t(255), uint8_t(255), uint8_t(0)));
  Vector3D normal = tri.getNormalAt(Vector3D(0, 0, 0));
  // Le vecteur normal attendu (calculé à la main)
  Vector3D expected = Vector3D(-0.7071, 0.7071, 0).normalized();
  EXPECT_NEAR(normal.getX(), expected.getX(), 1e-3);
  EXPECT_NEAR(normal.getY(), expected.getY(), 1e-3);
  EXPECT_NEAR(normal.getZ(), expected.getZ(), 1e-3);
}

TEST(TriangleTest, RayIntersectsTriangle) {
  Triangle tri(Vector3D(0, 0, 0), Vector3D(1, 0, 0), Vector3D(0, 1, 1),
               Color(uint8_t(255), uint8_t(255), uint8_t(0)));
  Ray ray(Vector3D(0.2, 0.2, -1), Vector3D(0, 0, 1));
  auto result = tri.intersect(ray);
  EXPECT_TRUE(result.has_value());
  if (result) {
    EXPECT_GT(result->distance, 0);
  }
}

TEST(TriangleTest, TransformAffectsNormal) {
  Triangle tri(Vector3D(0, 0, 0), Vector3D(1, 0, 0), Vector3D(0, 1, 0),
               Color(uint8_t(255), uint8_t(255), uint8_t(0)));
  Transform rot;
  rot.rotateY(90);  // Utilise la méthode rotateY si disponible
  tri.setTransform(rot);
  Vector3D normal = tri.getNormalAt(Vector3D(0, 0, 0));
  // Après rotation de 90° autour de Y, la normale (0,0,1) devient (1,0,0)
  EXPECT_NEAR(normal.getX(), 1.0, 1e-3);
  EXPECT_NEAR(normal.getY(), 0.0, 1e-3);
  EXPECT_NEAR(normal.getZ(), 0.0, 1e-3);
}
