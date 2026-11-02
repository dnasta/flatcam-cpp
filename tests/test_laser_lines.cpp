// "Líneas láser" de Paint (paint_polygon_worker, método 3, y fill_with_lines) contra el original (golden_laser_lines.h).
#include <gtest/gtest.h>

#include <cmath>
#include <sstream>

#include "golden_laser_lines.h"
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

