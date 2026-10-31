#include <gtest/gtest.h>

#include <functional>
#include <map>
#include <string>

#include "golden_gerber_ops.h"
#include "parsers/Gerber.h"
#include "test_helpers.h"

using namespace flatcam;

namespace {

/// Operaciones iguales a las del oráculo (tests/tools/oracle_gerber_ops.py).
const std::map<std::string, std::function<void(Gerber&)>> kOps = {
    {"scale", [](Gerber& g) { g.scale(1.5, 0.8, Point2D{1, 2}); }},
    {"offset", [](Gerber& g) { g.offset({3, -4}); }},
    {"mirror_y", [](Gerber& g) { g.mirror(Axis::Y, {0, 0}); }},
    {"mirror_x", [](Gerber& g) { g.mirror(Axis::X, {1, 1}); }},
    {"rotate", [](Gerber& g) { g.rotate(30, {2, 3}); }},
    {"skew", [](Gerber& g) { g.skew(10, 20, {0, 0}); }},
    {"buffer", [](Gerber& g) { g.buffer(0.2, JoinStyle::Round, false); }},
    {"buffer_factor", [](Gerber& g) { g.buffer(1.5, JoinStyle::Round, true); }},
};

ShapeList polygonsOf(const Gerber& g) {
    ShapeList out;
    for (const Shape& s : g.solidGeometry()) {
        for (const Shape& p : s.parts()) {
            if (p.type() == ShapeType::Polygon && !p.isEmpty()) out.push_back(p);
        }
    }
    return out;
}

}  // namespace

class GerberOps : public ::testing::TestWithParam<std::size_t> {};

TEST_P(GerberOps, MatchesOriginalParser) {
    const auto& c = golden_gerber_ops::kCases.at(GetParam());
    SCOPED_TRACE(std::string(c.file) + " / " + c.op);
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
    kOps.at(c.op)(g);

    // La cantidad de polígonos se compara tal cual, sin unirlos: el golden sale del FlatCAM original con Shapely 1.8,
    // que aplica el buffer polígono por polígono igual que este port. El área y los límites se comparan sobre la unión.
    const ShapeList polys = polygonsOf(g);
    EXPECT_EQ(static_cast<int>(polys.size()), c.nPolygons);
    const Shape all = Shape::unaryUnion(polys);
    expectRelNear(all.area(), c.area, 1e-6);
    const auto b = all.bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_NEAR(b->xmin, c.bounds[0], 1e-5);
    EXPECT_NEAR(b->ymin, c.bounds[1], 1e-5);
    EXPECT_NEAR(b->xmax, c.bounds[2], 1e-5);
    EXPECT_NEAR(b->ymax, c.bounds[3], 1e-5);

    ASSERT_EQ(g.apertures().size(), c.apertures.size());
    for (const auto& a : c.apertures) {
        const auto it = g.apertures().find(a.id);
        ASSERT_NE(it, g.apertures().end()) << "apertura " << a.id;
        const Aperture& ap = it->second;
        EXPECT_NEAR(ap.size.value_or(0.0), a.size, 1e-9) << "apertura " << a.id;
        EXPECT_NEAR(ap.width, a.width, 1e-9) << "apertura " << a.id;
        EXPECT_NEAR(ap.height, a.height, 1e-9) << "apertura " << a.id;
        EXPECT_NEAR(ap.diam, a.diam, 1e-9) << "apertura " << a.id;
        int nSolid = 0;
        for (const ApertureGeometry& e : ap.geometry) nSolid += e.solid ? 1 : 0;
        EXPECT_EQ(nSolid, a.nSolid) << "apertura " << a.id;
    }
}

INSTANTIATE_TEST_SUITE_P(TestData, GerberOps, ::testing::Range<std::size_t>(0, golden_gerber_ops::kCases.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             const auto& c = golden_gerber_ops::kCases.at(info.param);
                             std::string name = std::string(c.file) + "_" + c.op;
                             for (char& ch : name) if (!std::isalnum(static_cast<unsigned char>(ch))) ch = '_';
                             return name;
                         });

TEST(GerberOpsSmall, MirrorMovesFollowGeometryAndApertures) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10C,1.0*%", "D10*", "X20000Y10000D03*", "M02*"}));
    g.mirror(Axis::Y, {0, 0});
    ASSERT_EQ(g.followGeometry().size(), 1u);
    EXPECT_NEAR(g.followGeometry()[0].coordinates().front().x, -2.0, 1e-9);
    EXPECT_NEAR(g.apertures().at("10").geometry.at(0).solid->centroid().x, -2.0, 1e-6);
}

TEST(GerberOpsSmall, ConvertUnitsScalesApertures) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOIN*%", "%ADD10C,0.1*%", "D10*", "X10000Y10000D03*", "M02*"}));
    EXPECT_EQ(g.units(), Units::Inches);
    EXPECT_NEAR(g.convertUnits(Units::Millimeters), 25.4, 1e-12);
    EXPECT_NEAR(*g.apertures().at("10").size, 2.54, 1e-9);
    EXPECT_NEAR(g.followGeometry()[0].coordinates().front().x, 25.4, 1e-9);
}

TEST(GerberOpsSmall, BoundingBox) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10R,2.0X1.0*%", "D10*", "X0Y0D03*", "M02*"}));
    const auto b = g.getBoundingBox(0.5, false).bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_NEAR(b->xmin, -1.5, 1e-6);
    EXPECT_NEAR(b->ymax, 1.0, 1e-6);
    EXPECT_TRUE(g.getBoundingBox(0.5, true).area() < g.getBoundingBox(0.5, false).area());
}
