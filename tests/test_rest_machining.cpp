// Mecanizado de resto de NCC y de Paint contra el original (golden_rest_machining.h).
#include <gtest/gtest.h>

#include <cmath>

#include "golden_rest_machining.h"
#include "tools/Ncc.h"
#include "tools/Paint.h"

using namespace flatcam;

namespace {

PolygonClearing::Method methodOf(int m) {
    switch (m) {
        case 1: return PolygonClearing::Method::Seed;
        case 2: return PolygonClearing::Method::Lines;
        case 3: return PolygonClearing::Method::Combo;
        default: return PolygonClearing::Method::Standard;
    }
}

/// Estricto: mismos trazados vértice por vértice. Relajado (otras versiones de GEOS): mismo largo y mismos lugares.
void expectSameLines(const ShapeList& mine, const golden_rest_machining::Lines& golden) {
    bool same = mine.size() == golden.size();
    for (std::size_t i = 0; same && i < mine.size(); ++i) {
        const auto c = mine[i].coordinates();
        same = c.size() == golden[i].size();
        for (std::size_t k = 0; same && k < c.size(); ++k)
            same = std::abs(c[k].x - golden[i][k].x) < 1e-9 && std::abs(c[k].y - golden[i][k].y) < 1e-9;
    }
    if (same) return;
    std::vector<Shape> ref;
    double lg = 0, lm = 0;
    for (const auto& l : golden) {
        if (l.size() < 2) continue;
        ref.push_back(Shape::lineString(l));
        lg += ref.back().length();
    }
    const Shape reference = Shape::collection(ref);
    double worst = 0;
    std::size_t pm = 0, pg = 0;
    for (const Shape& s : mine) {
        lm += s.length();
        pm += s.coordinates().size();
        for (const Point2D& p : s.coordinates()) worst = std::max(worst, Shape::point(p).distance(reference));
    }
    for (const auto& l : golden) pg += l.size();
    // GEOS 3.15 empieza algunos contornos en otro vértice: cambia alguna unión de pasadas (como en el NCC estándar).
    EXPECT_LT(worst, 1e-3) << "mm de los vértices al recorrido del original";
    EXPECT_NEAR(static_cast<double>(pm), static_cast<double>(pg), 0.06 * pg) << "puntos";
    EXPECT_NEAR(lm, lg, 0.08 * lg) << "largo total (sin las uniones que cambian)";
}

}  // namespace

class RestMachining : public ::testing::TestWithParam<std::size_t> {};

TEST_P(RestMachining, MatchesOriginal) {
    const auto& sc = golden_rest_machining::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    std::vector<std::pair<std::size_t, ShapeList>> perTool;
    std::string err;
    if (std::string(sc.kind) == "ncc") {
        std::vector<NccRestTool> tools;
        for (const auto& t : sc.tools) tools.push_back({t.dia, t.overlap, methodOf(t.method)});
        NccOptions o;
        o.margin = sc.margin;
        o.connect = sc.connect;
        o.contour = sc.contour;
        ShapeList left;
        ASSERT_TRUE(Ncc::clearCopperRest(sc.shapes(), tools, o, perTool, left, err)) << err;
    } else {
        std::vector<PaintRestTool> tools;
        for (const auto& t : sc.tools) tools.push_back({t.dia, t.overlap, true, true});
        ASSERT_TRUE(Paint::paintRest(sc.shapes(), tools, sc.offset, methodOf(sc.method), 64, perTool, err)) << err;
    }
    ASSERT_EQ(perTool.size(), sc.expected.size()) << "herramientas que limpiaron algo";
    for (std::size_t i = 0; i < perTool.size(); ++i) {
        SCOPED_TRACE("herramienta " + std::to_string(sc.expected[i].dia));
        EXPECT_DOUBLE_EQ(sc.tools[perTool[i].first].dia, sc.expected[i].dia);
        expectSameLines(perTool[i].second, sc.expected[i].lines);
    }
}

INSTANTIATE_TEST_SUITE_P(Original, RestMachining,
                         ::testing::Range<std::size_t>(0, golden_rest_machining::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             const auto& s = golden_rest_machining::kScenarios.at(info.param);
                             return std::string(s.kind) + "_" + s.name;
                         });
