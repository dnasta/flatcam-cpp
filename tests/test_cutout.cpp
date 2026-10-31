#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "golden_cutout.h"
#include "golden_geometry.h"
#include "parsers/Gerber.h"
#include "tools/CutOut.h"

using namespace flatcam;

namespace {

using Lines = golden_cutout::Lines;

Lines linesOf(const ShapeList& shapes) {
    Lines out;
    for (const Shape& s : shapes) {
        if (s.isEmpty()) continue;
        if (static_cast<int>(s.type()) >= static_cast<int>(ShapeType::MultiPoint)) {
            for (const Shape& p : s.parts()) {
                if (!p.isEmpty()) out.push_back(p.coordinates());
            }
        } else {
            out.push_back(s.coordinates());
        }
    }
    return out;
}

/// Compara dos conjuntos de líneas vértice por vértice, en el mismo orden.
void expectSameLines(const Lines& got, const Lines& want, const std::string& what) {
    ASSERT_EQ(got.size(), want.size()) << what << ": cantidad de líneas";
    for (std::size_t i = 0; i < want.size(); ++i) {
        ASSERT_EQ(got[i].size(), want[i].size()) << what << ": vértices de la línea " << i;
        for (std::size_t k = 0; k < want[i].size(); ++k) {
            EXPECT_NEAR(got[i][k].x, want[i][k].x, 1e-9) << what << ": línea " << i << " vértice " << k;
            EXPECT_NEAR(got[i][k].y, want[i][k].y, 1e-9) << what << ": línea " << i << " vértice " << k;
        }
    }
}

ShapeList edgeCutsSolid() {
    static Gerber g;
    static bool loaded = false;
    if (!loaded) {
        EXPECT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Edge_Cuts.gm1"));
        loaded = true;
    }
    return g.solidGeometry();
}

}  // namespace

class CutoutGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(CutoutGolden, MatchesOriginalTool) {
    const auto& sc = golden_cutout::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    ShapeList input;
    if (sc.useEdgeCuts) {
        input = edgeCutsSolid();
    } else {
        for (const auto& w : sc.wkt) input.push_back(Shape::fromWkt(w));
    }
    CutoutResult r;
    std::string error;
    const bool ok = sc.rectangular ? CutOut::rectangular(input, sc.isGerber, sc.options, r, error)
                                   : CutOut::freeform(input, sc.isGerber, sc.options, r, error);
    if (sc.expectFail) {
        EXPECT_FALSE(ok) << "el original no genera geometría en este caso";
        EXPECT_FALSE(error.empty());
        return;
    }
    ASSERT_TRUE(ok) << error;

    expectSameLines(linesOf({r.solidGeometry}), sc.solid, "contorno");
    expectSameLines(linesOf(r.gapsSolidGeometry), sc.gaps, "puentes finos");
    ASSERT_EQ(r.mouseBiteHoles.size(), sc.holes.size()) << "cantidad de agujeros";
    for (std::size_t i = 0; i < sc.holes.size(); ++i) {
        EXPECT_NEAR(r.mouseBiteHoles[i].x, sc.holes[i].x, 1e-9) << "agujero " << i;
        EXPECT_NEAR(r.mouseBiteHoles[i].y, sc.holes[i].y, 1e-9) << "agujero " << i;
    }
}

INSTANTIATE_TEST_SUITE_P(Original, CutoutGolden, ::testing::Range<std::size_t>(0, golden_cutout::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_cutout::kScenarios.at(info.param).name);
                         });

class CutoutManual : public ::testing::TestWithParam<std::size_t> {};

TEST_P(CutoutManual, MatchesOriginalTool) {
    const auto& sc = golden_cutout::kManual.at(GetParam());
    SCOPED_TRACE(sc.name);
    Shape solid = Shape::lineMerge({Shape::fromWkt(sc.wkt).exterior()});
    for (const Point2D& click : sc.clicks) solid = CutOut::manualGap(solid, click, sc.dia, sc.gap);
    expectSameLines(linesOf({solid}), sc.expected, "contorno");
}

INSTANTIATE_TEST_SUITE_P(Original, CutoutManual, ::testing::Range<std::size_t>(0, golden_cutout::kManual.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_cutout::kManual.at(info.param).name);
                         });

TEST(Cutout, ErrorsAreReported) {
    CutoutResult r;
    std::string error;
    CutoutOptions o;
    o.toolDia = 0.0;
    EXPECT_FALSE(CutOut::freeform({Shape::fromWkt("POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0))")}, true, o, r, error));
    EXPECT_FALSE(error.empty());
    o.toolDia = 1.0;
    EXPECT_FALSE(CutOut::freeform({}, true, o, r, error));
    EXPECT_FALSE(error.empty());
}

TEST(Cutout, BridgeWidthIsGapPlusToolDiameter) {
    // Contorno del usuario: fresa de 1,2 y puente de 0,4 dejan un hueco de 1,6 centrado, como en su G-code.
    CutoutOptions o;
    o.toolDia = 1.2;
    o.gaps = CutoutGaps::TB;
    o.gapSize = 0.4;
    CutoutResult r;
    std::string error;
    ASSERT_TRUE(CutOut::freeform(edgeCutsSolid(), true, o, r, error)) << error;
    const Lines lines = linesOf({r.solidGeometry});
    ASSERT_EQ(lines.size(), 2u);
    double minAbsX = 1e9;
    for (const auto& l : lines) {
        for (const Point2D& p : l) {
            if (std::fabs(std::fabs(p.y) - 39.15) < 1e-3) minAbsX = std::min(minAbsX, std::fabs(p.x));
        }
    }
    EXPECT_NEAR(minAbsX, 0.8, 1e-6);  // el hueco arranca justo en x = ±0,8
}

TEST(Cutout, GerberToGcodeReproducesTheUsersCutoutFile) {
    // Recorrido completo del usuario: Edge_Cuts.gm1 -> CutOut rectangular (fresa 1,2, puentes arriba y abajo de 0,4)
    // -> G-code del contorno. Debe dar el mismo archivo que generó su FlatCAM 8.994 (sin los \r de Windows).
    CutoutOptions o;
    o.toolDia = 1.2;
    o.margin = 0.0;
    o.gaps = CutoutGaps::TB;
    o.gapSize = 0.4;
    CutoutResult r;
    std::string error;
    ASSERT_TRUE(CutOut::rectangular(edgeCutsSolid(), true, o, r, error)) << error;

    ShapeList lines;
    for (const Shape& s : r.solidGeometry.parts().empty() ? ShapeList{r.solidGeometry} : r.solidGeometry.parts()) {
        lines.push_back(s);
    }
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(lines, golden_geometry::settings())) << job.lastError();

    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/DosCau_PPal-Edge_Cuts.nc", std::ios::binary);
    std::stringstream buf;
    buf << in.rdbuf();
    std::string real;
    for (char c : buf.str()) {
        if (c != '\r') real += c;
    }
    EXPECT_EQ(job.exportGcode(golden_geometry::header()), real);
}
