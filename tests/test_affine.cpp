#include <gtest/gtest.h>

#include "core/AffineTransform.h"
#include "core/Shape.h"
#include "golden_data.h"
#include "test_helpers.h"

using namespace flatcam;

namespace {
const Shape kPoly = Shape::polygon({{0, 0}, {4, 0}, {4, 3}, {1, 5}, {0, 3}});
}

TEST(Affine, IdentityAndTranslation) {
    EXPECT_EQ(AffineTransform::identity().apply({3, 4}), (Point2D{3, 4}));
    EXPECT_EQ(AffineTransform::translation(2, -1).apply({3, 4}), (Point2D{5, 3}));
}

TEST(Affine, CompositionOrder) {
    const auto t = AffineTransform::translation(10, 0) * AffineTransform::scaling(2, 2);
    // Primero escala y luego traslada.
    EXPECT_EQ(t.apply({1, 1}), (Point2D{12, 2}));
}

TEST(Affine, RotationMatchesShapely) {
    expectPointsNear(kPoly.rotate(30, Point2D{1, 1}).coordinates(), golden::kPolyRot30, 1e-12);
    // Los múltiplos de 90° deben dar valores exactos (sin ruido del orden de 1e-16).
    const auto r90 = kPoly.rotate(90, Point2D{2, 2}).coordinates();
    expectPointsNear(r90, golden::kPolyRot90, 0.0);
}

TEST(Affine, SkewScaleMirrorMatchShapely) {
    expectPointsNear(kPoly.skew(10, 20, Point2D{1, 1}).coordinates(), golden::kPolySkew, 1e-12);
    expectPointsNear(kPoly.scale(2, 0.5, Point2D{1, 1}).coordinates(), golden::kPolyScale, 1e-12);
    const auto m = AffineTransform::mirror(Axis::X, {0, 1});
    expectPointsNear(kPoly.transform(m).coordinates(), golden::kPolyMirrorX, 1e-12);
}

TEST(Affine, DefaultOriginIsBoundingBoxCenter) {
    expectPointsNear(kPoly.scale(1.5, 1.5).coordinates(), golden::kPolyScaleCenter, 1e-12);
}

TEST(Affine, TranslateShape) {
    const auto b = kPoly.translate(10, 20).bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_DOUBLE_EQ(b->xmin, 10);
    EXPECT_DOUBLE_EQ(b->ymax, 25);
}
