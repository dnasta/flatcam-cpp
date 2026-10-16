#include <gtest/gtest.h>

#include "core/ArcMath.h"
#include "golden_data.h"
#include "test_helpers.h"

using namespace flatcam;

TEST(ArcMath, ArcMatchesCamlib) {
    expectPointsNear(arc({1, 2}, 3, 0.3, 2.0, ArcDirection::CounterClockwise, 64), golden::kArcCcw, 1e-12);
    expectPointsNear(arc({0, 0}, 2, 2.0, 0.3, ArcDirection::Clockwise, 50), golden::kArcCw, 1e-12);
}

TEST(ArcMath, ArcWrapsAroundWhenStopBeforeStart) {
    expectPointsNear(arc({0, 0}, 1, 3.0, 1.0, ArcDirection::CounterClockwise, 40), golden::kArcWrapCcw, 1e-12);
    expectPointsNear(arc({0, 0}, 1, 1.0, 3.0, ArcDirection::Clockwise, 40), golden::kArcWrapCw, 1e-12);
}

TEST(ArcMath, MinimumTwoSteps) {
    const auto pts = arc({0, 0}, 1, 0.0, 0.0001, ArcDirection::CounterClockwise, 4);
    // Con un ángulo diminuto igual se generan 2 segmentos (3 puntos)... salvo que stop <= start.
    EXPECT_EQ(pts.size(), 3u);
}

TEST(ArcMath, Arc2MatchesCamlib) {
    expectPointsNear(arc2({3, 0}, {0, 3}, {0, 0}, ArcDirection::CounterClockwise, 32), golden::kArc2, 1e-12);
}

TEST(ArcMath, ArcAngle) {
    EXPECT_NEAR(arcAngle(0.3, 2.0, ArcDirection::CounterClockwise), golden::kArcAngleCcw, 1e-14);
    EXPECT_NEAR(arcAngle(2.0, 0.3, ArcDirection::Clockwise), golden::kArcAngleCw, 1e-14);
    EXPECT_NEAR(arcAngle(3.0, 1.0, ArcDirection::CounterClockwise), golden::kArcAngleWrap, 1e-14);
}

TEST(ArcMath, ThreePointCircle) {
    auto c = threePointCircle({0, 0}, {2, 0}, {1, 1});
    ASSERT_TRUE(c.has_value());
    EXPECT_NEAR(c->center.x, golden::kCircle3Cx, 1e-12);
    EXPECT_NEAR(c->center.y, golden::kCircle3Cy, 1e-12);
    EXPECT_NEAR(c->radius, golden::kCircle3R, 1e-12);
    EXPECT_NEAR(c->t, golden::kCircle3T, 1e-12);

    c = threePointCircle({1, 2}, {4, 6}, {7, 1});
    ASSERT_TRUE(c.has_value());
    EXPECT_NEAR(c->center.x, golden::kCircle3bCx, 1e-9);
    EXPECT_NEAR(c->center.y, golden::kCircle3bCy, 1e-9);
    EXPECT_NEAR(c->radius, golden::kCircle3bR, 1e-9);
    EXPECT_NEAR(c->t, golden::kCircle3bT, 1e-9);
}

TEST(ArcMath, CollinearPointsHaveNoCircle) {
    EXPECT_FALSE(threePointCircle({0, 0}, {1, 1}, {2, 2}).has_value());
}

TEST(ArcMath, Distances) {
    EXPECT_DOUBLE_EQ(distance({0, 0}, {3, 4}), 5.0);
    EXPECT_DOUBLE_EQ(distanceEuclidian(0, 0, 6, 8), 10.0);
}
