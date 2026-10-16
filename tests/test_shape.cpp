#include <gtest/gtest.h>

#include "core/GeometryError.h"
#include "core/Shape.h"
#include "golden_data.h"
#include "test_helpers.h"

using namespace flatcam;

TEST(Shape, DefaultIsEmpty) {
    Shape s;
    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.type(), ShapeType::GeometryCollection);
    EXPECT_FALSE(s.bounds().has_value());
}

TEST(Shape, WktRoundTrip) {
    const Shape s = Shape::fromWkt("POLYGON ((0 0, 4 0, 4 3, 0 3, 0 0))");
    EXPECT_EQ(s.type(), ShapeType::Polygon);
    EXPECT_DOUBLE_EQ(s.area(), 12.0);
    EXPECT_DOUBLE_EQ(s.length(), 14.0);
    EXPECT_EQ(s.toWkt(), "POLYGON ((0 0, 4 0, 4 3, 0 3, 0 0))");
    EXPECT_THROW(Shape::fromWkt("NO ES WKT"), GeometryError);
}

TEST(Shape, PolygonAutoCloseAndHoles) {
    const Shape p = Shape::polygon({{0, 0}, {10, 0}, {10, 10}, {0, 10}}, {{{2, 2}, {4, 2}, {4, 4}, {2, 4}}});
    EXPECT_TRUE(p.isValid());
    EXPECT_DOUBLE_EQ(p.area(), 100.0 - 4.0);
    EXPECT_EQ(p.interiors().size(), 1u);
    EXPECT_EQ(p.exterior().type(), ShapeType::LinearRing);
    EXPECT_EQ(p.coordinates().size(), 5u);  // cerrado automáticamente
}

TEST(Shape, InvalidInputsThrow) {
    EXPECT_THROW(Shape::lineString({{0, 0}}), GeometryError);
    EXPECT_THROW(Shape::polygon({{0, 0}, {1, 1}}), GeometryError);
}

TEST(Shape, BoundsAndCentroid) {
    const Shape p = Shape::polygon({{1, 2}, {5, 2}, {5, 6}, {1, 6}});
    const auto b = p.bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_DOUBLE_EQ(b->xmin, 1);
    EXPECT_DOUBLE_EQ(b->ymin, 2);
    EXPECT_DOUBLE_EQ(b->xmax, 5);
    EXPECT_DOUBLE_EQ(b->ymax, 6);
    EXPECT_EQ(p.centroid(), (Point2D{3, 4}));
}

TEST(Shape, CircleMatchesShapely) {
    const Shape c = Shape::circle({1, 2}, 1.5, 32);
    expectRelNear(c.area(), golden::kCircleArea32);
    const auto b = c.bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_NEAR(b->xmin, golden::kCircleBounds0, 1e-12);
    EXPECT_NEAR(b->ymin, golden::kCircleBounds1, 1e-12);
    EXPECT_NEAR(b->xmax, golden::kCircleBounds2, 1e-12);
    EXPECT_NEAR(b->ymax, golden::kCircleBounds3, 1e-12);
    expectRelNear(Shape::circle({0, 0}, 1.0, 8).area(), golden::kCircleArea8);
    expectRelNear(Shape::circle({0, 0}, 1.0, 128).area(), golden::kCircleArea128);
}

TEST(Shape, BufferPolygonJoinStyles) {
    const Shape rect = Shape::polygon({{0, 0}, {4, 0}, {4, 3}, {0, 3}});
    expectRelNear(rect.buffer(0.5, 16, JoinStyle::Round).area(), golden::kRectBufferRound);
    expectRelNear(rect.buffer(0.5, 16, JoinStyle::Mitre).area(), golden::kRectBufferMitre);
    expectRelNear(rect.buffer(0.5, 16, JoinStyle::Bevel).area(), golden::kRectBufferBevel);
    expectRelNear(rect.buffer(-0.3, 16).area(), golden::kRectBufferNeg);
}

TEST(Shape, BufferLineCapStyles) {
    const Shape line = Shape::lineString({{0, 0}, {10, 0}, {10, 10}});
    expectRelNear(line.buffer(1, 16, JoinStyle::Round, CapStyle::Round).area(), golden::kLineBufferRoundRound);
    expectRelNear(line.buffer(1, 16, JoinStyle::Mitre, CapStyle::Flat).area(), golden::kLineBufferFlatMitre);
    expectRelNear(line.buffer(1, 16, JoinStyle::Bevel, CapStyle::Square).area(), golden::kLineBufferSquareBevel);
}

TEST(Shape, BooleanOperationsMatchShapely) {
    const Shape a = Shape::circle({0, 0}, 2, 64);
    const Shape b = Shape::circle({1.5, 0}, 2, 64);
    expectRelNear(a.unite(b).area(), golden::kUnionArea);
    expectRelNear(a.difference(b).area(), golden::kDiffArea);
    expectRelNear(a.intersection(b).area(), golden::kInterArea);
    expectRelNear(a.symmetricDifference(b).area(), golden::kSymDiffArea);
    const Shape u = Shape::unaryUnion({a, b, Shape::circle({10, 10}, 1, 64)});
    expectRelNear(u.area(), golden::kUnaryUnionArea);
    EXPECT_TRUE(Shape::unaryUnion({}).isEmpty());
}

TEST(Shape, Predicates) {
    const Shape big = Shape::polygon({{0, 0}, {10, 0}, {10, 10}, {0, 10}});
    const Shape small = Shape::polygon({{2, 2}, {4, 2}, {4, 4}, {2, 4}});
    const Shape far = Shape::polygon({{20, 20}, {21, 20}, {21, 21}, {20, 21}});
    EXPECT_TRUE(big.contains(small));
    EXPECT_TRUE(small.within(big));
    EXPECT_TRUE(big.intersects(small));
    EXPECT_TRUE(big.disjoint(far));
    EXPECT_FALSE(big.intersects(far));
    EXPECT_DOUBLE_EQ(big.distance(far), std::sqrt(200.0));
    EXPECT_TRUE(big.contains(Shape::point({5, 5})));
    EXPECT_TRUE(big.equals(Shape::fromWkt("POLYGON ((10 10, 10 0, 0 0, 0 10, 10 10))")));
}

TEST(Shape, PartsOfMultiAndCollection) {
    const Shape a = Shape::polygon({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    const Shape b = Shape::polygon({{5, 5}, {6, 5}, {6, 6}, {5, 6}});
    const Shape multi = Shape::collection({a, b});
    EXPECT_EQ(multi.type(), ShapeType::MultiPolygon);
    EXPECT_EQ(multi.numParts(), 2u);
    EXPECT_EQ(multi.parts().size(), 2u);
    // Una figura simple se devuelve a sí misma.
    EXPECT_EQ(a.numParts(), 1u);
    ASSERT_EQ(a.parts().size(), 1u);
    EXPECT_TRUE(a.parts().front().equals(a));
    // Colección heterogénea.
    const Shape mixed = Shape::collection({a, Shape::lineString({{0, 0}, {1, 1}})});
    EXPECT_EQ(mixed.type(), ShapeType::GeometryCollection);
}

TEST(Shape, SimplifyConvexHullMakeValid) {
    const Shape line = Shape::lineString({{0, 0}, {5, 0.001}, {10, 0}});
    EXPECT_EQ(line.simplify(0.1).coordinates().size(), 2u);
    const Shape l = Shape::polygon({{0, 0}, {4, 0}, {4, 1}, {1, 1}, {1, 4}, {0, 4}});
    EXPECT_GT(l.convexHull().area(), l.area());
    const Shape bowtie = Shape::fromWkt("POLYGON ((0 0, 2 2, 2 0, 0 2, 0 0))");
    EXPECT_FALSE(bowtie.isValid());
    EXPECT_TRUE(bowtie.makeValid().isValid());
}

TEST(Shape, AsLineString) {
    const Shape ring = Shape::linearRing({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    EXPECT_EQ(ring.type(), ShapeType::LinearRing);
    const Shape line = ring.asLineString();
    EXPECT_EQ(line.type(), ShapeType::LineString);
    EXPECT_EQ(line.coordinates().size(), ring.coordinates().size());
}

TEST(Shape, CopiesShareImmutableData) {
    const Shape a = Shape::circle({0, 0}, 1, 16);
    const Shape b = a;  // copia barata
    EXPECT_TRUE(a.equalsExact(b));
    EXPECT_DOUBLE_EQ(a.area(), b.area());
}
