#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "golden_ncc.h"
#include "parsers/Gerber.h"
#include "tools/Ncc.h"

using namespace flatcam;

namespace {

using Lines = golden_ncc::Lines;

/// Distancia mayor de un vértice de @p a al vértice más cercano de @p b: no depende de dónde empieza el contorno ni de
/// su orientación, que cambian entre versiones de GEOS sin cambiar la figura.
double maxDistanceToNearestVertex(const std::vector<Point2D>& a, const std::vector<Point2D>& b) {
    double worst = 0;
    for (const Point2D& p : a) {
        double best = 1e300;
        for (const Point2D& q : b) best = std::min(best, std::hypot(p.x - q.x, p.y - q.y));
        worst = std::max(worst, best);
    }
    return worst;
}

double maxCoordinateDifference(const std::vector<Point2D>& a, const std::vector<Point2D>& b) {
    double worst = 0;
    for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
        worst = std::max({worst, std::fabs(a[i].x - b[i].x), std::fabs(a[i].y - b[i].y)});
    }
    return worst;
}

}  // namespace

class NccGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(NccGolden, RegionAndTrajectoriesMatchTheOriginalTool) {
    const auto& sc = golden_ncc::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    NccOptions o;
    o.toolDia = sc.dia;
    o.overlap = sc.overlap;
    o.connect = sc.connect;
    o.margin = sc.margin;
    o.hasOffset = sc.hasOffset;
    o.offset = sc.offset;

    // La región a limpiar: misma cantidad de polígonos, mismos vértices.
    const ShapeList region = Ncc::emptyArea(sc.copper(), o);
    const ShapeList expectedRegion = sc.region();
    ASSERT_EQ(region.size(), expectedRegion.size());
    for (std::size_t i = 0; i < region.size(); ++i) {
        const auto ext = region[i].exterior().coordinates();
        const auto expExt = expectedRegion[i].exterior().coordinates();
        ASSERT_EQ(ext.size(), expExt.size()) << "vértices del contorno del polígono " << i;
        EXPECT_LT(maxDistanceToNearestVertex(ext, expExt), 1e-6) << "vértices del contorno del polígono " << i;
        EXPECT_LT(maxDistanceToNearestVertex(expExt, ext), 1e-6);
        ASSERT_EQ(region[i].interiors().size(), expectedRegion[i].interiors().size());
        EXPECT_NEAR(region[i].area(), expectedRegion[i].area(), 1e-6);
    }

    ShapeList paths;
    std::string error;
    const bool ok = Ncc::clearCopper(sc.copper(), o, paths, error);
    if (sc.expectEmpty) {
        EXPECT_FALSE(ok);
        EXPECT_FALSE(error.empty());
        return;
    }
    ASSERT_TRUE(ok) << error;

    std::size_t pointsMine = 0, pointsRef = 0;
    double lengthMine = 0, lengthRef = 0;
    bool sameStructure = paths.size() == sc.lines.size();
    for (std::size_t i = 0; i < paths.size(); ++i) {
        const auto c = paths[i].coordinates();
        pointsMine += c.size();
        for (std::size_t k = 0; k + 1 < c.size(); ++k) lengthMine += std::hypot(c[k + 1].x - c[k].x, c[k + 1].y - c[k].y);
        if (i < sc.lines.size() && c.size() != sc.lines[i].size()) sameStructure = false;
    }
    for (const auto& l : sc.lines) {
        pointsRef += l.size();
        for (std::size_t k = 0; k + 1 < l.size(); ++k) lengthRef += std::hypot(l[k + 1].x - l[k].x, l[k + 1].y - l[k].y);
    }
    // Los anillos de la espiral son siempre los mismos: la cantidad total de puntos tiene que coincidir.
    EXPECT_EQ(pointsMine, pointsRef) << "puntos en total";

    if (sameStructure) {
        // Mismos trazados con los mismos vértices: coinciden con el original al milmillonésimo. Las versiones nuevas de
        // GEOS pueden resolver distinto algún punto muy sensible, a menos de una micra.
        std::size_t total = 0, off = 0;
        double worst = 0;
        for (std::size_t i = 0; i < paths.size(); ++i) {
            const auto c = paths[i].coordinates();
            for (std::size_t k = 0; k < c.size(); ++k) {
                const double d = std::max(std::fabs(c[k].x - sc.lines[i][k].x), std::fabs(c[k].y - sc.lines[i][k].y));
                ++total;
                if (d > 1e-9) ++off;
                worst = std::max(worst, d);
            }
        }
        EXPECT_LT(worst, 1e-3) << "mayor diferencia de coordenadas (mm)";
        EXPECT_LE(off, std::max<std::size_t>(2, total / 500)) << off << " de " << total << " vértices difieren del original";
    } else {
        // GEOS 3.15 empieza algunos contornos en otro vértice, y entonces la unión de anillos vecinos cambia de a un
        // trazado (medido: +-1 trazado y hasta 1,2 % de longitud en estos casos). Se compara lo que no depende de eso.
        EXPECT_NEAR(lengthMine, lengthRef, lengthRef * 0.02) << "longitud total";
        EXPECT_LE(std::llabs(static_cast<long long>(paths.size()) - static_cast<long long>(sc.lines.size())), 1) << "trazados";
    }
}

INSTANTIATE_TEST_SUITE_P(Original, NccGolden, ::testing::Range<std::size_t>(0, golden_ncc::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_ncc::kScenarios.at(info.param).name);
                         });

TEST(Ncc, RejectsANonPositiveTool) {
    NccOptions o;
    o.toolDia = 0;
    ShapeList paths;
    std::string error;
    EXPECT_FALSE(Ncc::clearCopper({Shape::polygon({{0, 0}, {1, 0}, {1, 1}, {0, 1}})}, o, paths, error));
    EXPECT_FALSE(error.empty());
}

// ------------------------------------------------ de punta a punta contra el G-code real del usuario (FlatCAM 8.994)

namespace {

struct LaserFile {
    std::size_t paths{0}, points{0};
    double length{0};
    std::string xRange, yRange;
};

LaserFile analyse(const std::string& text) {
    LaserFile f;
    std::stringstream ss(text);
    std::string l;
    std::vector<Point2D> cur;
    std::vector<std::string> L;
    while (std::getline(ss, l)) {
        if (!l.empty() && l.back() == '\r') l.pop_back();
        L.push_back(l);
    }
    auto flush = [&] {
        if (cur.size() >= 2) {
            ++f.paths;
            f.points += cur.size();
            for (std::size_t i = 0; i + 1 < cur.size(); ++i) f.length += std::hypot(cur[i + 1].x - cur[i].x, cur[i + 1].y - cur[i].y);
        }
        cur.clear();
    };
    for (std::size_t i = 0; i < L.size(); ++i) {
        double x = 0, y = 0;
        if (L[i].rfind("(X range:", 0) == 0) f.xRange = L[i];
        if (L[i].rfind("(Y range:", 0) == 0) f.yRange = L[i];
        if (std::sscanf(L[i].c_str(), "G00 X%lf Y%lf", &x, &y) == 2 && i + 2 < L.size() && L[i + 1].rfind("G01 F", 0) == 0 &&
            L[i + 2].rfind("M03", 0) == 0) {
            flush();
            cur = {{x, y}};
        } else if (!cur.empty() && std::sscanf(L[i].c_str(), "G01 X%lf Y%lf F", &x, &y) == 2) {
            cur.push_back({x, y});
        }
    }
    flush();
    return f;
}

}  // namespace

// Tarda unos 100 s con optimización (26 millones de puntos antes de simplificar): solo corre con FLATCAM_SLOW_TESTS=1.
TEST(NccEndToEnd, UserBottomCopperFromGerberMatchesTheRealLaserFile) {
    if (!std::getenv("FLATCAM_SLOW_TESTS")) GTEST_SKIP() << "prueba lenta: definir FLATCAM_SLOW_TESTS=1";
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-B_Cu.gbl"));
    g.mirror(Axis::Y, {0, 0});  // el cobre inferior está reflejado en X en el proyecto del usuario
    NccOptions o;
    o.toolDia = 0.1;
    o.overlap = 0.8;
    o.connect = true;
    ShapeList paths;
    std::string error;
    ASSERT_TRUE(Ncc::clearCopper(g.solidGeometry(), o, paths, error)) << error;

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

    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/DosCau_PPal-B_Cu.nc", std::ios::binary);
    std::stringstream realText;
    realText << in.rdbuf();
    const LaserFile real = analyse(realText.str());
    const LaserFile mine = analyse(job.exportGcode({}));

    // Los rangos de la cabecera coinciden exactamente: confirma el reflejo en X y la envolvente convexa del cobre.
    EXPECT_EQ(mine.xRange, real.xRange);
    EXPECT_EQ(mine.yRange, real.yRange);
    // La 8.994 une unos 23 tramos más que la 2024.4 (723 trazados contra 746): lo demás es equivalente.
    EXPECT_NEAR(static_cast<double>(mine.points), static_cast<double>(real.points), real.points * 0.01);
    EXPECT_NEAR(mine.length, real.length, real.length * 0.005);
    EXPECT_NEAR(static_cast<double>(mine.paths), static_cast<double>(real.paths), real.paths * 0.05);
}
