// Métodos de Semilla (clear_polygon2) y Líneas (clear_polygon3) contra el original (golden_clearing_methods.h).
#include <gtest/gtest.h>

#include <cmath>

#include "cam/PolygonClearing.h"
#include "golden_clearing_methods.h"

using namespace flatcam;

class ClearingMethods : public ::testing::TestWithParam<std::size_t> {};

TEST_P(ClearingMethods, MatchesOriginal) {
    const auto& sc = golden_clearing_methods::kScenarios.at(GetParam());
    SCOPED_TRACE(std::string(sc.name) + (sc.method == 2 ? " semilla" : " líneas"));
    const Shape poly = sc.polygon();
    const auto res = sc.method == 2
                         ? PolygonClearing::clearPolygonSeed(poly, sc.dia, 64, sc.overlap, sc.connect, sc.contour)
                         : PolygonClearing::clearPolygonLines(poly, sc.dia, 64, sc.overlap, sc.connect, sc.contour);
    ASSERT_EQ(res.has_value(), sc.hasResult);
    if (!res) return;
    // Nivel estricto: los mismos trazados, vértice por vértice (GEOS 3.11 y 3.12).
    bool same = res->size() == sc.lines.size();
    for (std::size_t i = 0; same && i < res->size(); ++i) {
        const auto& a = (*res)[i].points;
        const auto& b = sc.lines[i];
        same = a.size() == b.size();
        for (std::size_t k = 0; same && k < a.size(); ++k)
            same = std::abs(a[k].x - b[k].x) < 1e-9 && std::abs(a[k].y - b[k].y) < 1e-9;
    }
    if (same) return;
    // Nivel relajado (GEOS 3.15 corta los círculos de la semilla en otros puntos): el mismo recorrido, partido distinto.
    // Mismo largo total y cada vértice a menos de una micra del recorrido del original.
    std::vector<Shape> golden;
    double goldenLength = 0, myLength = 0;
    for (const auto& l : sc.lines) {
        if (l.size() < 2) continue;
        golden.push_back(Shape::lineString(l));
        goldenLength += golden.back().length();
    }
    const Shape reference = Shape::collection(golden);
    for (const auto& line : *res) {
        if (line.points.size() >= 2) myLength += Shape::lineString(line.points).length();
        for (const Point2D& p : line.points) ASSERT_LT(Shape::point(p).distance(reference), 1e-6);
    }
    EXPECT_NEAR(myLength, goldenLength, 1e-6 * goldenLength);
}

INSTANTIATE_TEST_SUITE_P(Original, ClearingMethods,
                         ::testing::Range<std::size_t>(0, golden_clearing_methods::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             const auto& s = golden_clearing_methods::kScenarios.at(info.param);
                             return std::string(s.name) + (s.method == 2 ? "_seed_" : "_lines_") + std::to_string(info.param);
                         });
