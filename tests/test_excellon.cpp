#include <gtest/gtest.h>

#include <cmath>

#include "golden_excellon.h"
#include "parsers/Excellon.h"
#include "test_helpers.h"

using namespace flatcam;

namespace {

/// Opciones iguales a las preferencias del usuario (y al oráculo): ceros T, métrico, formatos 3.3 y 2.4.
ExcellonOptions userOptions() {
    ExcellonOptions opt;
    opt.zeros = "T";
    opt.excellonUnits = "METRIC";
    opt.formatUpperMm = 3;
    opt.formatLowerMm = 3;
    opt.formatUpperIn = 2;
    opt.formatLowerIn = 4;
    opt.circleSteps = 64;
    return opt;
}

}  // namespace

// ---------------------------------------------------------------------------------- casos chicos

TEST(ExcellonSmall, ToolsAndDrillsWithDecimalPoint) {
    Excellon e(userOptions());
    ASSERT_TRUE(e.parseLines({"M48", "METRIC", "T1C0.8", "T2C1.0", "%", "T1", "X1.5Y2.5", "X3.0Y4.0", "T2", "X-1.0Y0.5", "M30"}));
    ASSERT_TRUE(e.createGeometry());
    ASSERT_EQ(e.drillTools().size(), 2u);
    EXPECT_DOUBLE_EQ(e.drillTools().at(1).tooldia, 0.8);
    ASSERT_EQ(e.drillTools().at(1).drills.size(), 2u);
    EXPECT_EQ(e.drillTools().at(1).drills[1], (Point2D{3.0, 4.0}));
    ASSERT_EQ(e.drillTools().at(2).drills.size(), 1u);
    EXPECT_EQ(e.solidGeometry().size(), 3u);
    EXPECT_NEAR(e.solidGeometry()[0].area(), 3.14159 * 0.4 * 0.4, 0.002);
    EXPECT_EQ(e.units(), Units::Millimeters);
}

TEST(ExcellonSmall, GcodeFileIsRejected) {
    Excellon e(userOptions());
    EXPECT_FALSE(e.parseLines({"G21", "G01 X1 Y1"}));
}

TEST(ExcellonSmall, MissingFileFails) {
    Excellon e(userOptions());
    EXPECT_FALSE(e.parseFile("/no/existe.drl"));
}

TEST(ExcellonSmall, TransformationsMoveDrills) {
    Excellon e(userOptions());
    ASSERT_TRUE(e.parseLines({"M48", "METRIC", "T1C0.8", "%", "T1", "X10.0Y5.0", "M30"}));
    ASSERT_TRUE(e.createGeometry());
    e.mirror(Axis::Y, {0, 0});  // x -> -x
    EXPECT_EQ(e.drillTools().at(1).drills[0], (Point2D{-10.0, 5.0}));
    e.mirror(Axis::X, {0, 0});  // y -> -y
    EXPECT_EQ(e.drillTools().at(1).drills[0], (Point2D{-10.0, -5.0}));
    e.offset({1, 1});
    EXPECT_EQ(e.drillTools().at(1).drills[0], (Point2D{-9.0, -4.0}));
    e.rotate(90, {0, 0});
    EXPECT_NEAR(e.drillTools().at(1).drills[0].x, 4.0, 1e-12);
    EXPECT_NEAR(e.drillTools().at(1).drills[0].y, -9.0, 1e-12);
    const auto b = e.bounds();
    EXPECT_NEAR(b.xmin, 4.0 - 0.4, 1e-6);
}

TEST(ExcellonSmall, ConvertUnitsScalesDrillsAndDiameters) {
    Excellon e(userOptions());
    ASSERT_TRUE(e.parseLines({"M48", "METRIC", "T1C1.0", "%", "T1", "X25.4Y50.8", "M30"}));
    ASSERT_TRUE(e.createGeometry());
    EXPECT_NEAR(e.convertUnits(Units::Inches), 1 / 25.4, 1e-15);
    EXPECT_EQ(e.units(), Units::Inches);
    EXPECT_NEAR(e.drillTools().at(1).drills[0].x, 1.0, 1e-12);
    EXPECT_NEAR(e.drillTools().at(1).drills[0].y, 2.0, 1e-12);
    EXPECT_NEAR(e.drillTools().at(1).tooldia, 1 / 25.4, 1e-12);
}

TEST(ExcellonSmall, ParseNumberUsesFormatAndZeros) {
    ExcellonOptions opt = userOptions();
    opt.zeros = "L";
    Excellon e(opt);
    EXPECT_NEAR(*e.parseNumber("015000"), 15.0, 1e-12);   // 3.3, ceros iniciales omitidos
    // Igual que FlatCAM: con "L" divide por 10^(cantidad de dígitos - dígitos enteros), así que "-25" vale -250.
    EXPECT_NEAR(*e.parseNumber("-25"), -250.0, 1e-9);
    opt.zeros = "T";
    Excellon t(opt);
    EXPECT_NEAR(*t.parseNumber("15000"), 15.0, 1e-12);    // divide por 10^3
}

// -------------------------------------------------------- archivos reales y sintéticos vs parser original

class ExcellonGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(ExcellonGolden, MatchesOriginalParser) {
    const auto& g = golden_excellon::kFiles.at(GetParam());
    SCOPED_TRACE(g.file);
    Excellon e(userOptions());
    const bool ok = e.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + g.file);
    ASSERT_EQ(!ok, g.failed) << e.lastError();

    EXPECT_EQ(e.units(), std::string(g.units) == "MM" ? Units::Millimeters : Units::Inches);
    EXPECT_EQ(e.zeros(), g.zeros);
    EXPECT_EQ(e.formatUpperMm(), g.upperMm);
    EXPECT_EQ(e.formatLowerMm(), g.lowerMm);
    EXPECT_EQ(e.formatUpperIn(), g.upperIn);
    EXPECT_EQ(e.formatLowerIn(), g.lowerIn);
    EXPECT_EQ(static_cast<int>(e.solidGeometry().size()), g.nSolid);

    ASSERT_EQ(e.drillTools().size(), g.tools.size());
    for (const auto& et : g.tools) {
        SCOPED_TRACE("herramienta " + std::to_string(et.id));
        const auto it = e.drillTools().find(et.id);
        ASSERT_NE(it, e.drillTools().end());
        const ExcellonTool& t = it->second;
        EXPECT_NEAR(t.tooldia, et.dia, 1e-12);
        ASSERT_EQ(t.drills.size(), et.drills.size());
        for (std::size_t i = 0; i < et.drills.size(); ++i) {
            EXPECT_NEAR(t.drills[i].x, et.drills[i].x, 1e-12) << "agujero " << i;
            EXPECT_NEAR(t.drills[i].y, et.drills[i].y, 1e-12) << "agujero " << i;
        }
        ASSERT_EQ(t.slots.size(), et.slots.size());
        for (std::size_t i = 0; i < et.slots.size(); ++i) {
            EXPECT_NEAR(t.slots[i].first.x, et.slots[i].x0, 1e-12);
            EXPECT_NEAR(t.slots[i].first.y, et.slots[i].y0, 1e-12);
            EXPECT_NEAR(t.slots[i].second.x, et.slots[i].x1, 1e-12);
            EXPECT_NEAR(t.slots[i].second.y, et.slots[i].y1, 1e-12);
        }
        expectRelNear(Shape::unaryUnion(t.solidGeometry).area(), et.area, 1e-9);
    }
}

INSTANTIATE_TEST_SUITE_P(TestData, ExcellonGolden, ::testing::Range<std::size_t>(0, golden_excellon::kFiles.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             std::string name = golden_excellon::kFiles.at(info.param).file;
                             for (char& c : name) if (!std::isalnum(static_cast<unsigned char>(c))) c = '_';
                             return name;
                         });
