#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "golden_dblsided.h"
#include "tools/DoubleSided.h"

using namespace flatcam;

class DoubleSidedGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(DoubleSidedGolden, AlignmentDrillsMatchTheOriginalTool) {
    const auto& sc = golden_dblsided::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    const std::vector<Point2D> got = DoubleSided::alignmentDrills(sc.holes, sc.axis, sc.pivot);
    ASSERT_EQ(got.size(), sc.drills.size());
    for (std::size_t i = 0; i < got.size(); ++i) {
        EXPECT_EQ(got[i].x, sc.drills[i].x) << "agujero " << i;  // igualdad exacta: es la misma aritmética que `affinity.scale`
        EXPECT_EQ(got[i].y, sc.drills[i].y) << "agujero " << i;
    }
}

INSTANTIATE_TEST_SUITE_P(Original, DoubleSidedGolden, ::testing::Range<std::size_t>(0, golden_dblsided::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_dblsided::kScenarios.at(info.param).name);
                         });

TEST(DoubleSided, BoxCenterIsTheMiddleOfTheBounds) {
    const Point2D c = DoubleSided::boxCenter({-4.0, 2.0, 10.0, 8.0});
    EXPECT_EQ(c.x, 3.0);
    EXPECT_EQ(c.y, 5.0);
}

TEST(DoubleSided, AlignmentExcellonHasOneToolWithAllTheHoles) {
    const Excellon ex = DoubleSided::alignmentExcellon({{-38, 39.5}, {38, 39.5}}, Axis::X, {0, 0}, 0.4);
    ASSERT_EQ(ex.drillTools().size(), 1u);
    EXPECT_EQ(ex.drillTools().at(1).tooldia, 0.4);
    EXPECT_EQ(ex.drillTools().at(1).drills.size(), 4u);
    EXPECT_EQ(ex.drillTools().at(1).solidGeometry.size(), 4u);
}

// ------------------------------------------------ contra el G-code real del usuario (FlatCAM 8.994)

namespace {

std::vector<std::string> nonEmptyLines(const std::string& text) {
    std::vector<std::string> out;
    std::string cur;
    auto push = [&] {
        if (!cur.empty()) out.push_back(cur);
        cur.clear();
    };
    for (char c : text) {
        if (c == '\r') continue;
        if (c == '\n') push();
        else cur += c;
    }
    push();
    return out;
}

}  // namespace

TEST(DoubleSided, AlignmentGcodeMatchesTheUsersFileExceptTheOriginDrillAndBlankLines) {
    // Tu `Alinea.nc` salió de otro camino de código de la 8.994 (el de la pestaña del propio objeto Excellon) que ya no
    // existe en la 2024.4: deja una línea en blanco después de cada agujero y taladra además en el origen (0, 0), aunque
    // el objeto tiene 4 agujeros ("Total drills = 4"). El port genera el formato de la 2024.4 sin esas dos cosas.
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/DosCau_PPal-Alinea.nc", std::ios::binary);
    ASSERT_TRUE(in.good());
    std::stringstream buf;
    buf << in.rdbuf();
    std::vector<std::string> real = nonEmptyLines(buf.str());

    // Se quita del real el taladro del origen: 4 líneas seguidas que empiezan en `G00 X0.0000 Y0.0000` sin los espacios
    // finales del cambio de herramienta.
    bool removed = false;
    for (std::size_t i = 0; i + 3 < real.size(); ++i) {
        if (real[i] == "G00 X0.0000 Y0.0000" && real[i + 1] == "G01 Z-3.5000" && real[i + 2] == "G01 Z0") {
            real.erase(real.begin() + static_cast<long>(i), real.begin() + static_cast<long>(i) + 4);
            removed = true;
            break;
        }
    }
    ASSERT_TRUE(removed) << "el archivo real debería taladrar en el origen";

    GcodeHeaderInfo h;
    h.name = "PCB ppal-Alinea.drl_cnc";
    h.type = "Geometry";
    h.units = "MM";
    for (const auto& line : nonEmptyLines(buf.str())) {
        if (line.rfind("(Created on ", 0) == 0) h.createdOn = line.substr(12, line.size() - 13);
    }

    DrillJobSettings s;
    s.units = "MM";
    s.order = DrillOrder::Optimized;
    DrillToolSettings t;
    t.cutZ = PyNumber(-3.5);
    t.feedrateZ = PyNumber(50.0);
    t.feedrateRapid = PyNumber(1500);
    t.travelZ = PyNumber(2);
    t.depthPerPass = PyNumber(0.7);
    t.offsetZ = PyNumber(0.0);
    t.dwellTime = PyNumber(1);
    t.toolchangeZ = PyNumber(19.0);
    t.endZ = PyNumber(19.0);
    t.spindleDir = "CW";
    t.spindleSpeed = PyNumber(1000);
    t.toolchangeXY = std::array<double, 2>{0.0, 0.0};
    t.endXY = std::array<double, 2>{0.0, 0.0};
    s.tools[1] = t;

    // El usuario pidió dos agujeros y la herramienta agregó sus reflejos respecto del eje X que pasa por (0, 0).
    const Excellon ex = DoubleSided::alignmentExcellon({{-38, 39.5}, {38, 39.5}}, Axis::X, {0, 0}, 0.4);
    CncJob job(s);
    ASSERT_TRUE(job.generateFromExcellon(ex)) << job.lastError();
    const std::vector<std::string> mine = nonEmptyLines(job.exportGcode(h));

    ASSERT_EQ(mine.size(), real.size());
    for (std::size_t i = 0; i < mine.size(); ++i) EXPECT_EQ(mine[i], real[i]) << "línea " << i;
}
