// Autonivelación contra el original (tests/tools/oracle_autolevel.py).
#include <gtest/gtest.h>

#include <map>

#include "cam/Autolevel.h"
#include "golden_autolevel.h"

using namespace flatcam;

namespace {

const BoundingBox kCuts{-3.21, 5.5, 40.123456, 31.7};  // las líneas de corte del oráculo

ProbingGcodeParams params(const std::string& controller, const std::string& units) {
    ProbingGcodeParams p;
    p.controller = controller;
    p.units = units;
    p.name = "B_Cu_cnc";
    p.version = "2024.4";
    p.versionDate = "2024/04/15";
    p.timeStr = golden_autolevel::kTime;
    p.probeFeedrate = 120.0;
    return p;
}

void check(const std::vector<ProbePoint>& pts, const golden_autolevel::Case& c) {
    SCOPED_TRACE(c.name);
    ASSERT_EQ(pts.size() * 3, c.points.size());
    for (std::size_t i = 0; i < pts.size(); ++i) {
        EXPECT_EQ(pts[i].point.x, c.points[3 * i]);
        EXPECT_EQ(pts[i].point.y, c.points[3 * i + 1]);
        EXPECT_EQ(pts[i].height, c.points[3 * i + 2]);
        const double* cell = &c.cells[5 * i];
        if (cell[0] < 0) {
            EXPECT_FALSE(pts[i].cell.has_value());
            continue;
        }
        ASSERT_TRUE(pts[i].cell.has_value());
        EXPECT_NEAR(pts[i].cell->area(), cell[0], 1e-9);
        const BoundingBox b = *pts[i].cell->bounds();
        EXPECT_NEAR(b.xmin, cell[1], 1e-9);
        EXPECT_NEAR(b.ymin, cell[2], 1e-9);
        EXPECT_NEAR(b.xmax, cell[3], 1e-9);
        EXPECT_NEAR(b.ymax, cell[4], 1e-9);
    }
}

std::vector<ProbePoint> fromPoints(const std::vector<Point2D>& in) {
    std::vector<ProbePoint> out;
    for (const Point2D& p : in) out.push_back({p, std::nullopt, 0.0});
    return out;
}

std::vector<Point2D> justPoints(const std::vector<ProbePoint>& in) {
    std::vector<Point2D> out;
    for (const ProbePoint& p : in) out.push_back(p.point);
    return out;
}

}  // namespace

TEST(Autolevel, MatchesTheOriginal) {
    std::map<std::string, const golden_autolevel::Case*> g;
    for (const auto& c : golden_autolevel::kCases) g[c.name] = &c;
    struct Grid { const char* name; int rows, cols; bool voronoi; const char* controller; const char* units; };
    for (const Grid& k : {Grid{"grid4x4_v_mach3", 4, 4, true, "MACH3", "MM"},
                          Grid{"grid3x2_v_in_linuxcnc", 3, 2, true, "LinuxCNC", "IN"},
                          Grid{"grid1x3_b_mach4", 1, 3, false, "MACH4", "MM"},
                          Grid{"grid2x2_v_grbl", 2, 2, true, "GRBL", "MM"}}) {
        std::vector<ProbePoint> pts = fromPoints(autolevelGrid(kCuts, k.rows, k.cols, 4));
        if (k.voronoi) ASSERT_TRUE(autolevelVoronoi(pts, kCuts, std::string(k.units) == "MM"));
        check(pts, *g.at(k.name));
        EXPECT_EQ(autolevelProbingGcode(justPoints(pts), params(k.controller, k.units)), g.at(k.name)->gcode) << k.name;
    }
    // A mano: el primer punto queda fuera de la caja (sin celda).
    std::vector<ProbePoint> pts = fromPoints({{-3.21, -3.21}, {10, 10}, {30.5, 20.25}});
    ASSERT_TRUE(autolevelVoronoi(pts, kCuts, true));
    check(pts, *g.at("manual_v"));
    EXPECT_EQ(autolevelProbingGcode(justPoints(pts), params("MACH3", "MM")), g.at("manual_v")->gcode);
    ASSERT_TRUE(autolevelImportHeightMap({"1.5 2.5 -0.125\n", "3,4,0.25\n", "5 6 0.5\n"}, pts));
    check(pts, *g.at("heightmap"));
    // Un mapa más largo agrega los puntos que faltan.
    std::vector<ProbePoint> empty;
    ASSERT_TRUE(autolevelImportHeightMap({"1.5 2.5 -0.125\n", "3,4,0.25\n"}, empty));
    ASSERT_EQ(empty.size(), 2u);
    EXPECT_EQ(empty[1].point.x, 3.0);
    EXPECT_FALSE(autolevelImportHeightMap({"1.5 x\n"}, empty));
    // GRBL: el avance manual con la tupla del original.
    const char* dirs[] = {"xplus", "xminus", "yplus", "yminus", "zplus", "zminus", "origin"};
    for (int i = 0; i < 7; ++i) {
        std::string joined;
        for (const std::string& c : grblJogCommands(dirs[i], 5.0, 1500.0, "MM", 15.0))
            joined += (joined.empty() ? "" : "|") + c;
        EXPECT_EQ(joined, golden_autolevel::kJog[i]);
    }
}
