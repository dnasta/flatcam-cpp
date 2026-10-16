#include <gtest/gtest.h>

#include <stdexcept>

#include "core/Types.h"

using namespace flatcam;

TEST(Types, PointArithmetic) {
    const Point2D a{1, 2}, b{3, 5};
    EXPECT_EQ(a + b, (Point2D{4, 7}));
    EXPECT_EQ(b - a, (Point2D{2, 3}));
    EXPECT_EQ(a * 2.0, (Point2D{2, 4}));
    EXPECT_EQ(b / 2.0, (Point2D{1.5, 2.5}));
    EXPECT_DOUBLE_EQ((Point2D{3, 4}).norm(), 5.0);
}

TEST(Types, BoundingBox) {
    const BoundingBox a{0, 0, 2, 4}, b{-1, 1, 1, 6};
    EXPECT_DOUBLE_EQ(a.width(), 2);
    EXPECT_DOUBLE_EQ(a.height(), 4);
    EXPECT_EQ(a.center(), (Point2D{1, 2}));
    const BoundingBox m = a.merged(b);
    EXPECT_DOUBLE_EQ(m.xmin, -1);
    EXPECT_DOUBLE_EQ(m.ymax, 6);
}

TEST(Types, UnitsFromString) {
    EXPECT_EQ(unitsFromString("MM"), Units::Millimeters);
    EXPECT_EQ(unitsFromString("mm"), Units::Millimeters);
    EXPECT_EQ(unitsFromString("IN"), Units::Inches);
    EXPECT_EQ(unitsFromString("in"), Units::Inches);
    EXPECT_THROW(unitsFromString("cm"), std::invalid_argument);
    EXPECT_EQ(toString(Units::Millimeters), "MM");
    EXPECT_EQ(toString(Units::Inches), "IN");
}
