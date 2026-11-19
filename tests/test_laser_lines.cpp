// "Líneas láser" de Paint (paint_polygon_worker, método 3, y fill_with_lines) contra el original (golden_laser_lines.h).
#include <gtest/gtest.h>

#include <cmath>
#include <sstream>

#include "golden_laser_lines.h"
#include "core/OriginalBugs.h"
#include "tools/Paint.h"

using namespace flatcam;

namespace {

Gerber loadGerber(const golden_laser_lines::Scenario& sc) {
    Gerber g;
    if (sc.isPath) {
        EXPECT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + sc.gerber));
    } else {
        std::vector<std::string> lines;
        std::stringstream ss(sc.gerber);
        std::string l;
        while (std::getline(ss, l)) lines.push_back(l);
        EXPECT_TRUE(g.parseLines(lines));
    }
    return g;
}

}  // namespace

class LaserLines : public ::testing::TestWithParam<std::size_t> {};

TEST_P(LaserLines, MatchesOriginal) {
    const auto& sc = golden_laser_lines::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    const Gerber g = loadGerber(sc);
    PaintOptions o;
    o.toolDia = sc.dia;
    o.overlap = sc.overlap;
    o.connect = sc.connect;
    o.contour = sc.contour;
    o.method = PolygonClearing::Method::LaserLines;
    // Los polígonos de paint_poly_all (margen 0) y lo que pinta cada uno.
    const ShapeList polys = Paint::flattenToPolygons(g.solidGeometry());
    ASSERT_EQ(static_cast<int>(polys.size()), sc.polygons);
    ShapeList mine;
    int painted = 0;
    for (const Shape& p : polys) {
        const auto res = Paint::laserLines(p.buffer(-0.0), g, o);
        if (!res) continue;
        ++painted;
        for (const Shape& s : *res)
            if (!s.isEmpty()) mine.push_back(s);
    }
    EXPECT_EQ(painted, sc.painted);
    // paintAll da lo mismo, concatenado.
    ShapeList all;
    std::string err;
    Paint::paintAll(g.solidGeometry(), o, all, err, &g);
    EXPECT_EQ(all.size(), mine.size());

    std::size_t points = 0;
    double length = 0;
    for (const Shape& s : mine) {
        points += s.coordinates().size();
        length += s.length();
    }
    if (!sc.hasLines) {
        // Solo totales (el Logo del usuario, 36.576 trazados): 1 % de trazados y puntos, 0,1 % de largo.
        EXPECT_NEAR(static_cast<double>(mine.size()), sc.nLines, 0.01 * sc.nLines);
        EXPECT_NEAR(static_cast<double>(points), sc.nPoints, 0.01 * sc.nPoints);
        EXPECT_NEAR(length, sc.length, 1e-3 * sc.length);
        return;
    }
    // Nivel estricto: los mismos trazados, vértice por vértice.
    bool same = mine.size() == sc.lines.size();
    for (std::size_t i = 0; same && i < mine.size(); ++i) {
        const auto a = mine[i].coordinates();
        const auto& b = sc.lines[i];
        same = a.size() == b.size();
        for (std::size_t k = 0; same && k < a.size(); ++k)
            same = std::abs(a[k].x - b[k].x) < 1e-9 && std::abs(a[k].y - b[k].y) < 1e-9;
    }
    if (same) return;
    // Nivel relajado (GEOS 3.15 corta los círculos de la Semilla de los pads en otros puntos y `paint_connect` une
    // distinto): cada vértice a menos de una micra del recorrido del original y el largo total a menos del 1 %.
    std::vector<Shape> golden;
    for (const auto& l : sc.lines)
        if (l.size() >= 2) golden.push_back(Shape::lineString(l));
    const Shape reference = Shape::collection(golden);
    for (const Shape& s : mine)
        for (const Point2D& p : s.coordinates()) ASSERT_LT(Shape::point(p).distance(reference), 1e-6);
    EXPECT_NEAR(length, sc.length, 0.01 * sc.length);
}

INSTANTIATE_TEST_SUITE_P(Original, LaserLines, ::testing::Range<std::size_t>(0, golden_laser_lines::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_laser_lines::kScenarios.at(info.param).name);
                         });


// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

namespace {
/// Trazados de "Líneas láser" de todos los polígonos de un Gerber (y cuántos polígonos se pintaron).
std::pair<int, ShapeList> paintLaser(const std::string& text, double dia = 0.05, double overlap = 0.5,
                                     bool connect = true, bool contour = true) {
    Gerber g;
    std::vector<std::string> lines;
    std::stringstream ss(text);
    for (std::string l; std::getline(ss, l);) lines.push_back(l);
    EXPECT_TRUE(g.parseLines(lines));
    PaintOptions o;
    o.toolDia = dia;
    o.overlap = overlap;
    o.connect = connect;
    o.contour = contour;
    o.method = PolygonClearing::Method::LaserLines;
    int painted = 0;
    ShapeList out;
    for (const Shape& p : Paint::flattenToPolygons(g.solidGeometry()))
        if (auto r = Paint::laserLines(p.buffer(-0.0), g, o)) {
            ++painted;
            out.insert(out.end(), r->begin(), r->end());
        }
    return {painted, out};
}
}  // namespace

TEST(LaserLinesFixed, MacrosRegionsAndASingleTraceArePainted) {
    const char* header = "%FSLAX46Y46*%\n%MOMM*%\n%LPD*%\n";
    const std::string macro = std::string(header) +
                              "%AMBOX*\n21,1,1.0,0.6,0,0,0*%\n%ADD10C,0.400000*%\n%ADD20BOX*%\nD10*\nX0Y0D02*\n"
                              "X5000000Y0D01*\nD20*\nX5000000Y0D03*\nM02*\n";
    const std::string single = std::string(header) + "%ADD10C,0.150000*%\nD10*\nX0Y0D02*\nX4000000Y0D01*\nM02*\n";
    // Una región (tamaño 0) y, aparte, un trazo.
    const std::string region = std::string(header) +
                               "%ADD10C,0.300000*%\nG36*\nX0Y0D02*\nX2000000Y0D01*\nX2000000Y1000000D01*\n"
                               "X0Y1000000D01*\nX0Y0D01*\nG37*\nD10*\nX5000000Y0D02*\nX9000000Y0D01*\nM02*\n";
    // Un trazo de 0,15 con una fresa de 0,1 y sin contorno: todas sus líneas se unen en una sola y linemerge falla.
    struct Case { const std::string* text; double dia, overlap; bool connect, contour; };
    for (const Case& c : {Case{&macro, 0.05, 0.5, true, true}, Case{&single, 0.1, 0.2, true, false},
                          Case{&region, 0.05, 0.5, true, true}}) {
        std::pair<int, ShapeList> orig;
        {
            OriginalBugsScope o(true);
            orig = paintLaser(*c.text, c.dia, c.overlap, c.connect, c.contour);
        }
        OriginalBugsScope fixed(false);
        const auto [painted, lines] = paintLaser(*c.text, c.dia, c.overlap, c.connect, c.contour);
        EXPECT_GT(lines.size(), orig.second.size()) << *c.text << " pintados " << painted << " contra " << orig.first;
        double len = 0;
        for (const Shape& sh : lines) len += sh.length();
        EXPECT_GT(len, 1.0) << *c.text;
        EXPECT_GE(painted, orig.first);
    }
}
