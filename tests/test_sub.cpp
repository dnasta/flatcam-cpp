// Restar contra el original (tests/tools/oracle_sub.py -> golden_sub.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <string>

#include "golden_sub.h"
#include "parsers/Gerber.h"
#include "tools/Subtract.h"

using namespace flatcam;

TEST(Subtract, GerberMatchesTheOriginal) {
    const std::string data = FLATCAM_TEST_DATA_DIR;
    for (const auto& c : golden_sub::kGerber) {
        SCOPED_TRACE(std::string(c.target) + " - " + c.sub);
        Gerber t, s;
        ASSERT_TRUE(t.parseFile(data + "/" + c.target));
        ASSERT_TRUE(s.parseFile(data + "/" + c.sub));
        const SubtractGerberResult r = subtractGerber(t, s);
        ASSERT_EQ(r.apertures.size(), c.apertures.size());
        for (const auto& want : c.apertures) {
            ASSERT_TRUE(r.apertures.count(want.id)) << want.id;
            const auto& geo = r.apertures.at(want.id).geometry;
            ASSERT_EQ(geo.size(), want.solidAreas.size()) << want.id;
            for (std::size_t i = 0; i < geo.size(); ++i)
                EXPECT_NEAR(geo[i].solid ? geo[i].solid->area() : -1.0, want.solidAreas[i], 1e-6) << want.id << " " << i;
        }
        ASSERT_EQ(static_cast<int>(r.solid.size()), c.polygons);
        std::vector<double> areas;
        for (const Shape& p : r.solid) areas.push_back(p.area());
        std::sort(areas.begin(), areas.end());
        for (std::size_t i = 0; i < areas.size(); ++i) EXPECT_NEAR(areas[i], c.areas[i], 1e-6) << i;
    }
}

TEST(Subtract, GeometryMatchesTheOriginal) {
    const Shape sub = Shape::unaryUnion({Shape::fromWkt("POLYGON ((2 2, 6 2, 6 6, 2 6, 2 2))"),
                                         Shape::fromWkt("POLYGON ((7 7, 9 7, 9 12, 7 12, 7 7))")});
    const ShapeList geo = {Shape::fromWkt("POLYGON ((0 0, 8 0, 8 8, 0 8, 0 0), (3 3, 5 3, 5 5, 3 5, 3 3))"),
                           Shape::fromWkt("LINESTRING (-1 4, 12 4)"),
                           Shape::fromWkt("MULTILINESTRING ((1 9, 10 9), (8 1, 8 11))"),
                           Shape::fromWkt("MULTIPOLYGON (((10 0, 12 0, 12 2, 10 2, 10 0)))")};
    for (const auto& c : golden_sub::kGeo) {
        const ShapeList r = subtractGeometry(geo, sub, c.close);
        ASSERT_EQ(r.size(), c.result.size()) << c.close;
        for (std::size_t i = 0; i < r.size(); ++i) {
            const Shape want = Shape::fromWkt(c.result[i]);
            EXPECT_TRUE(r[i].equals(want)) << c.close << " " << i << " " << r[i].toWkt(6);
            EXPECT_NEAR(r[i].length(), want.length(), 1e-9);
        }
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

TEST(SubtractFixed, OverlappingSubtractorsCoveredAndSplitElements) {
    // El objetivo: una región de 10 x 2 y un pad de 1 x 1 aparte. El sustraendo: dos cuadrados de 2 x 2 superpuestos que
    // cortan la región en dos y un cuadrado que tapa el pad entero.
    Gerber target, sub;
    ASSERT_TRUE(target.parseLines({"%FSLAX46Y46*%", "%MOMM*%", "%ADD10R,1X1*%", "G36*", "X0Y0D02*", "X10000000Y0D01*",
                                   "X10000000Y2000000D01*", "X0Y2000000D01*", "X0Y0D01*", "G37*", "D10*",
                                   "X20000000Y0D03*", "M02*"}));
    ASSERT_TRUE(sub.parseLines({"%FSLAX46Y46*%", "%MOMM*%", "%ADD10R,2X3*%", "%ADD11R,2X2*%", "D10*", "X4000000Y1000000D03*",
                                "X5000000Y1000000D03*", "D11*", "X20000000Y0D03*", "M02*"}));
    auto areaOf = [](const ShapeList& l) {
        double a = 0;
        for (const Shape& s : l) a += s.area();
        return a;
    };
    OriginalBugsScope fixed(false);
    const SubtractGerberResult r = subtractGerber(target, sub);
    ASSERT_FALSE(r.failed);
    EXPECT_NEAR(areaOf(r.solid), 20.0 - 3 * 2, 1e-6);  // las dos partes de la región, sin el pad
    EXPECT_EQ(r.solid.size(), 2u);
    for (const auto& [id, ap] : r.apertures)
        for (const auto& el : ap.geometry) EXPECT_FALSE(el.solid && el.solid->isEmpty()) << id;
}
