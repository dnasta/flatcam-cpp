#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "cam/EndpointStorage.h"
#include "golden_paint.h"
#include "parsers/Gerber.h"
#include "tools/Paint.h"

using namespace flatcam;

namespace {

using Lines = golden_paint::Lines;

Lines linesOf(const ShapeList& shapes) {
    Lines out;
    for (const Shape& s : shapes) out.push_back(s.coordinates());
    return out;
}

}  // namespace

class PaintGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(PaintGolden, TrajectoriesMatchTheOriginalClearPolygon) {
    const auto& sc = golden_paint::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    PaintOptions o;
    o.toolDia = sc.dia;
    o.overlap = sc.overlap;
    o.connect = sc.connect;
    o.offset = sc.offset;
    ShapeList paths;
    std::string error;
    const bool ok = Paint::paintAll(sc.input(), o, paths, error);
    if (sc.expectEmpty) {
        EXPECT_FALSE(ok);
        EXPECT_FALSE(error.empty());
        return;
    }
    ASSERT_TRUE(ok) << error;
    const Lines got = linesOf(paths);
    ASSERT_EQ(got.size(), sc.lines.size()) << "cantidad de trazados";
    double worst = 0;
    for (std::size_t i = 0; i < got.size(); ++i) {
        ASSERT_EQ(got[i].size(), sc.lines[i].size()) << "vértices del trazado " << i;
        for (std::size_t k = 0; k < got[i].size(); ++k) {
            worst = std::max({worst, std::fabs(got[i][k].x - sc.lines[i][k].x), std::fabs(got[i][k].y - sc.lines[i][k].y)});
        }
    }
    EXPECT_LT(worst, 1e-9) << "mayor diferencia de coordenadas";
}

TEST_P(PaintGolden, GcodeWithRTreeOrderMatchesTheOriginalGenerator) {
    const auto& sc = golden_paint::kScenarios.at(GetParam());
    if (sc.expectEmpty) {
        EXPECT_STREQ(sc.gcode, "") << "sin trazados no hay G-code";
        return;
    }
    SCOPED_TRACE(sc.name);
    ShapeList lines;
    for (const auto& l : sc.lines) lines.push_back(Shape::lineString(l));
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(lines, sc.settings())) << job.lastError();
    EXPECT_EQ(job.startGcode() + "\n" + job.gcode(), sc.gcode);
}

INSTANTIATE_TEST_SUITE_P(Original, PaintGolden, ::testing::Range<std::size_t>(0, golden_paint::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_paint::kScenarios.at(info.param).name);
                         });

TEST(EndpointStorage, NearestRemovesAndBreaksTiesByInsertionOrder) {
    EndpointStorage s;
    s.insert({{0, 0}, {10, 0}});    // líneas 0: extremos (0,0) y (10,0)
    s.insert({{5, 5}, {10, 10}});   // línea 1
    s.insert({{0, 0}, {-3, 4}});    // línea 2: comparte el extremo (0,0) con la línea 0
    const auto a = s.nearest({1, 0});
    ASSERT_TRUE(a);
    EXPECT_EQ(a->index, 0u);          // empate en (0,0): gana la línea que se insertó primero
    EXPECT_EQ(a->point.x, 0.0);
    s.remove(0);
    const auto b = s.nearest({1, 0});
    ASSERT_TRUE(b);
    EXPECT_EQ(b->index, 2u);
    s.remove(2);
    s.remove(1);
    EXPECT_FALSE(s.nearest({0, 0}));
    EXPECT_EQ(s.size(), 0u);
}

TEST(EndpointStorage, FindsTheTrueNearestOnLargeRandomSets) {
    EndpointStorage s;
    std::vector<Point2D> ends;
    unsigned seed = 12345;
    auto rnd = [&seed] {
        seed = seed * 1664525u + 1013904223u;
        return (seed >> 8) / 16777216.0 * 100.0;
    };
    for (int i = 0; i < 400; ++i) {
        const Point2D a{rnd(), rnd()}, b{rnd(), rnd()};
        s.insert({a, b});
        ends.push_back(a);
        ends.push_back(b);
    }
    for (int q = 0; q < 300; ++q) {
        const Point2D p{rnd() * 1.5 - 20, rnd() * 1.5 - 20};  // incluye consultas fuera de la grilla
        double best = 1e300;
        for (const auto& e : ends) best = std::min(best, std::hypot(e.x - p.x, e.y - p.y));
        const auto n = s.nearest(p);
        ASSERT_TRUE(n);
        EXPECT_DOUBLE_EQ(std::hypot(n->point.x - p.x, n->point.y - p.y), best);
    }
}

// ------------------------------------------------ de punta a punta contra el G-code real del usuario (FlatCAM 8.994)

namespace {

std::vector<std::string> readLines(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    std::vector<std::string> lines;
    std::string l;
    while (std::getline(in, l)) {
        if (!l.empty() && l.back() == '\r') l.pop_back();
        lines.push_back(l);
    }
    return lines;
}

/// Trazados de un G-code de láser (empiezan en `G00 X.. Y..` + `G01 F..` + `M03`).
std::vector<std::vector<Point2D>> laserPaths(const std::vector<std::string>& L) {
    std::vector<std::vector<Point2D>> out;
    std::vector<Point2D> cur;
    for (std::size_t i = 0; i < L.size(); ++i) {
        double x = 0, y = 0;
        if (std::sscanf(L[i].c_str(), "G00 X%lf Y%lf", &x, &y) == 2 && i + 2 < L.size() && L[i + 1].rfind("G01 F", 0) == 0 &&
            L[i + 2].rfind("M03", 0) == 0) {
            if (cur.size() >= 2) out.push_back(cur);
            cur = {{x, y}};
        } else if (!cur.empty() && std::sscanf(L[i].c_str(), "G01 X%lf Y%lf F", &x, &y) == 2) {
            cur.push_back({x, y});
        }
    }
    if (cur.size() >= 2) out.push_back(cur);
    return out;
}

double distanceToSegment(const Point2D& p, const Point2D& a, const Point2D& b) {
    const double dx = b.x - a.x, dy = b.y - a.y, len2 = dx * dx + dy * dy;
    double t = len2 > 0 ? ((p.x - a.x) * dx + (p.y - a.y) * dy) / len2 : 0;
    t = std::max(0.0, std::min(1.0, t));
    return std::hypot(p.x - (a.x + t * dx), p.y - (a.y + t * dy));
}

double distanceToPolyline(const Point2D& p, const std::vector<Point2D>& line) {
    double d = 1e300;
    for (std::size_t i = 0; i + 1 < line.size(); ++i) d = std::min(d, distanceToSegment(p, line[i], line[i + 1]));
    return d;
}

}  // namespace

TEST(PaintEndToEnd, UserLogoFromGerberMatchesTheRealLaserFile) {
    // B_Logo.gbr -> reflejo en X (como en su proyecto) -> Paint estándar (0,1 mm, solape 80 %, conectar)
    // -> G-code de láser con el orden R y la tolerancia de simplificación (global_tolerance / 20 = 0,00025).
    // La 8.994 no se puede reproducir byte a byte (simplifica con otras decisiones de desempate), pero los trazados
    // coinciden en cantidad, orden y orientación, y se apartan menos de 4 micras.
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-B_Logo.gbr"));
    g.mirror(Axis::Y, {0, 0});
    PaintOptions o;
    o.toolDia = 0.1;
    o.overlap = 0.8;
    o.connect = true;
    ShapeList paths;
    std::string error;
    ASSERT_TRUE(Paint::paintAll(g.solidGeometry(), o, paths, error)) << error;

    GeometryJobSettings s;
    s.preprocessor = "GRBL_laser";
    s.order = DrillOrder::RTree;
    s.toolchange = false;
    s.flavor = GcodeFlavor::FlatCAM8_994;
    GeometryToolSettings& t = s.toolSettings;
    t.toolDia = 0.1;
    t.cutZ = -3.5;
    t.depthPerPass = 0.8;
    t.travelZ = 2.0;
    t.feedrate = 500.0;
    t.feedrateZ = 100.0;
    t.feedrateRapid = 1500.0;
    t.spindleSpeed = 300.0;
    t.endZ = 19.0;
    t.endXY = std::array<double, 2>{0.0, 0.0};
    t.toolchangeZ = 19.0;
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.tolerance = 0.00025;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(paths, s)) << job.lastError();

    std::vector<std::string> mineLines;
    {
        std::stringstream ss(job.exportGcode({}));
        std::string l;
        while (std::getline(ss, l)) mineLines.push_back(l);
    }
    const auto mine = laserPaths(mineLines);
    const auto real = laserPaths(readLines(std::string(FLATCAM_TEST_DATA_DIR) + "/DosCau_PPal-Logo.nc"));
    ASSERT_EQ(mine.size(), real.size());
    ASSERT_EQ(mine.size(), 38u);

    std::size_t pointsMine = 0, pointsReal = 0;
    double worstDistance = 0, worstStart = 0;
    for (std::size_t i = 0; i < real.size(); ++i) {
        pointsMine += mine[i].size();
        pointsReal += real[i].size();
        worstStart = std::max(worstStart, std::hypot(mine[i].front().x - real[i].front().x, mine[i].front().y - real[i].front().y));
        for (const Point2D& p : real[i]) worstDistance = std::max(worstDistance, distanceToPolyline(p, mine[i]));
        for (const Point2D& p : mine[i]) worstDistance = std::max(worstDistance, distanceToPolyline(p, real[i]));
    }
    EXPECT_LT(worstDistance, 0.005) << "mm entre mi trazado y el real";
    EXPECT_LT(worstStart, 0.06) << "mm entre los inicios";
    EXPECT_NEAR(static_cast<double>(pointsMine), static_cast<double>(pointsReal), pointsReal * 0.01);
}
