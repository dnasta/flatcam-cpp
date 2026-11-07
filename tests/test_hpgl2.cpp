// Importar HPGL2 (parseHpgl2) contra appParsers/ParseHPGL2.py original (golden_hpgl2.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

#include "golden_hpgl2.h"
#include "parsers/Hpgl2.h"

using namespace flatcam;

namespace {

/// Las mismas coordenadas, en el mismo orden (a menos del milmillonésimo).
bool sameCoords(const Shape& a, const Shape& b) {
    const auto x = a.coordinates(), y = b.coordinates();
    if (x.size() != y.size()) return false;
    for (std::size_t i = 0; i < x.size(); ++i)
        if (std::abs(x[i].x - y[i].x) > 1e-9 || std::abs(x[i].y - y[i].y) > 1e-9) return false;
    return true;
}

}  // namespace

TEST(Hpgl2, MatchesOriginal) {
    for (const auto& c : golden_hpgl2::kCases) {
        SCOPED_TRACE(c.file);
        std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/hpgl2/" + c.file);
        std::stringstream ss;
        ss << in.rdbuf();
        const Hpgl2Result r = parseHpgl2(ss.str());
        ASSERT_EQ(r.ok, c.ok) << r.error;
        if (!r.ok) continue;
        // Cada pluma: los mismos trazos, vértice por vértice.
        ASSERT_EQ(r.tools.size(), c.tools.size());
        for (const auto& [tool, wkts] : c.tools) {
            ASSERT_TRUE(r.tools.count(tool)) << tool;
            const ShapeList& mine = r.tools.at(tool);
            ASSERT_EQ(mine.size(), wkts.size());
            for (std::size_t i = 0; i < wkts.size(); ++i) EXPECT_TRUE(sameCoords(mine[i], Shape::fromWkt(wkts[i]))) << i;
        }
        // La unión: la misma figura (la de GEOS puede ordenar distinto las partes).
        const Shape ref = Shape::fromWkt(c.solid);
        const Shape mine = Shape::collection(r.solidGeometry);
        EXPECT_NEAR(mine.length(), ref.length(), 1e-9);
        auto maxDist = [](const Shape& a, const Shape& b) {
            double d = 0;
            for (const Shape& part : a.parts().empty() ? ShapeList{a} : a.parts())
                for (const Point2D& p : part.coordinates()) d = std::max(d, Shape::point(p).distance(b));
            return d;
        };
        EXPECT_LT(maxDist(mine, ref), 1e-9);
        EXPECT_LT(maxDist(ref, mine), 1e-9);
    }
}
