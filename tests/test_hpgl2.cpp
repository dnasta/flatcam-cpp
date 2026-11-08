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
        const Hpgl2Result r = parseHpgl2(ss.str(), 64, false);  // como el original
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

namespace {

Hpgl2Result parseFile(const char* name) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/hpgl2/" + name);
    std::stringstream ss;
    ss << in.rdbuf();
    return parseHpgl2(ss.str());
}

double totalLength(const ShapeList& l) {
    double s = 0;
    for (const Shape& g : l) s += g.length();
    return s;
}

}  // namespace

// Corregido: varias órdenes por línea, con coordenadas en PU/PD y varios pares en PA (40 unidades = 1 mm).
TEST(Hpgl2Fixed, SeveralCommandsPerLine) {
    const Hpgl2Result r = parseHpgl2("IN;SP1;PU0,0;PD400,0,400,400;PU;PA800,0;PD;PA1200,0;PU;\n");
    ASSERT_TRUE(r.ok) << r.error;
    ASSERT_EQ(r.tools.at(1).size(), 2u);
    EXPECT_NEAR(r.tools.at(1)[0].length(), 20.0, 1e-9);
    EXPECT_NEAR(r.tools.at(1)[1].length(), 10.0, 1e-9);
    // El original ignora esas líneas.
    EXPECT_FALSE(parseHpgl2("IN;SP1;PU0,0;PD400,0,400,400;PU;\n", 64, false).ok);
}

// Corregido: el último trazo con la pluma abajo no se pierde.
TEST(Hpgl2Fixed, LastStrokeIsKept) {
    const std::string t = "IN;\nSP1;\nPU;\nPA0,0;\nPD;\nPA400,0;\n";
    EXPECT_FALSE(parseHpgl2(t, 64, false).ok);  // el original lo pierde y no queda nada
    const Hpgl2Result r = parseHpgl2(t);
    ASSERT_TRUE(r.ok);
    EXPECT_NEAR(totalLength(r.tools.at(1)), 10.0, 1e-9);
}

// Corregido: después de un AA la posición queda en el final del arco y se puede seguir dibujando desde ahí.
TEST(Hpgl2Fixed, ArcLeavesThePositionAtItsEnd) {
    // Centro (10, 0) mm, desde (0, 0), 90° antihorario: termina en (10, -10); después una recta a (20, -10).
    const Hpgl2Result r = parseHpgl2("IN;SP1;PU0,0;AA400,0,90;PD;PA800,-400;PU;\n");
    ASSERT_TRUE(r.ok) << r.error;
    ASSERT_EQ(r.tools.at(1).size(), 2u);
    const auto arcPts = r.tools.at(1)[0].coordinates();
    EXPECT_NEAR(arcPts.back().x, 10.0, 1e-9);
    EXPECT_NEAR(arcPts.back().y, -10.0, 1e-9);
    const auto line = r.tools.at(1)[1].coordinates();
    EXPECT_NEAR(line.front().x, 10.0, 1e-9);
    EXPECT_NEAR(line.front().y, -10.0, 1e-9);
    EXPECT_NEAR(r.tools.at(1)[1].length(), 10.0, 1e-9);
    EXPECT_FALSE(parseFile("at_aborts.plt").error.size());  // y AT ya no corta la lectura
}

// Corregido: AT dibuja el arco por los tres puntos y termina en el final.
TEST(Hpgl2Fixed, ThreePointArc) {
    // Semicírculo de (0, 0) a (20, 0) pasando por (10, 10) mm: radio 10, largo pi * 10.
    const Hpgl2Result r = parseHpgl2("IN;SP1;PU0,0;AT400,400,800,0;PU;\n");
    ASSERT_TRUE(r.ok) << r.error;
    ASSERT_EQ(r.tools.at(1).size(), 1u);
    const Shape& a = r.tools.at(1)[0];
    EXPECT_NEAR(a.length(), M_PI * 10, 0.05);
    const auto pts = a.coordinates();
    EXPECT_NEAR(pts.front().x, 0.0, 1e-9);
    EXPECT_NEAR(pts.back().x, 20.0, 1e-9);
    EXPECT_NEAR(pts.back().y, 0.0, 1e-9);
    for (const Point2D& p : pts) EXPECT_GE(p.y, -1e-9);  // pasa por arriba (por el punto intermedio)
    EXPECT_TRUE(parseFile("at_aborts.plt").ok);  // el original no lo lee
}
