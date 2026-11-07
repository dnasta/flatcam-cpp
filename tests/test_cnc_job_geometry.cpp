#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "golden_geometry.h"
#include "golden_geometry_modes.h"
#include "golden_geometry_multitool.h"

using namespace flatcam;

namespace {

ShapeList contourGeometry() {
    ShapeList geometry;
    for (const auto& path : golden_geometry::kPaths) geometry.push_back(Shape::lineString(path));
    return geometry;
}

std::string readFile(const std::string& name) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/" + name, std::ios::binary);
    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

std::string withoutCarriageReturns(const std::string& text) {
    std::string out;
    for (char c : text) {
        if (c != '\r') out += c;
    }
    return out;
}

}  // namespace

TEST(CncJobGeometry, MatchesOriginalGeneratorExactly) {
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(contourGeometry(), golden_geometry::settings())) << job.lastError();
    EXPECT_EQ(job.startGcode(), golden_geometry::kStartGcode);
    EXPECT_EQ(job.gcode(), golden_geometry::kToolGcode);
    EXPECT_EQ(job.exportGcode(golden_geometry::header()), golden_geometry::kFullFile);
}

TEST(CncJobGeometry, ReproducesTheUsersCutoutFileByteForByte) {
    // El .nc real lo generó FlatCAM 8.994 en Windows (saltos de línea \r\n): se compara sin los \r.
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(contourGeometry(), golden_geometry::settings())) << job.lastError();
    EXPECT_EQ(job.exportGcode(golden_geometry::header()), withoutCarriageReturns(readFile("DosCau_PPal-Edge_Cuts.nc")));
}

TEST(CncJobGeometry, OptimizedOrderKeepsAllPaths) {
    GeometryJobSettings s = golden_geometry::settings();
    s.order = DrillOrder::Optimized;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(contourGeometry(), s)) << job.lastError();
    // Mismas líneas aunque cambie el orden de los dos trazados: se comparan como conjunto de líneas.
    auto lines = [](const std::string& t) {
        std::vector<std::string> v;
        std::stringstream ss(t);
        std::string l;
        while (std::getline(ss, l)) v.push_back(l);
        std::sort(v.begin(), v.end());
        return v;
    };
    EXPECT_EQ(lines(job.gcode()), lines(golden_geometry::kToolGcode));
}

TEST(CncJobGeometry, ToolOffsetMovesThePath) {
    GeometryJobSettings s = golden_geometry::settings();
    s.toolSettings.offsetKind = "out";
    // Un cuadrado de 10 x 10 recorrido por fuera con una fresa de 2 mm queda 1 mm más grande.
    const ShapeList square = {Shape::linearRing({{0, 0}, {10, 0}, {10, 10}, {0, 10}})};
    s.toolSettings.toolDia = 2.0;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry(square, s)) << job.lastError();
    EXPECT_NE(job.gcode().find("X-1.0000 Y"), std::string::npos);
    EXPECT_NE(job.gcode().find("Y11.0000"), std::string::npos);
}

TEST(CncJobGeometry, ErrorsAreReported) {
    GeometryJobSettings s = golden_geometry::settings();
    s.preprocessor = "no_existe";
    CncJob bad;
    EXPECT_FALSE(bad.generateFromGeometry(contourGeometry(), s));
    EXPECT_FALSE(bad.lastError().empty());

    CncJob empty;
    EXPECT_FALSE(empty.generateFromGeometry({}, golden_geometry::settings()));
    EXPECT_FALSE(empty.lastError().empty());
}

// ------------------------------------------- modos: profundidad múltiple, corte extra, puntos, tolerancia...

class GeometryModes : public ::testing::TestWithParam<std::size_t> {};

TEST_P(GeometryModes, MatchesOriginalGenerator) {
    const auto& sc = golden_geometry_modes::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    CncJob job;
    GeometryJobSettings settings = sc.settings();
    settings.flavor = GcodeFlavor::FlatCAM2024_4;  // los goldens salen del código de la 2024.4
    ASSERT_TRUE(job.generateFromGeometry(sc.geometry(), settings)) << job.lastError();
    EXPECT_EQ(job.startGcode() + "\n" + job.gcode(), sc.expected);
    // travel_distance y routing_time del original (las sumas de largos pueden diferir en el último bit).
    EXPECT_NEAR(job.travelDistance(), sc.travelDistance, 1e-9 * std::max(1.0, sc.travelDistance));
    EXPECT_NEAR(job.routingTime(), sc.routingTime, 1e-9 * std::max(1.0, sc.routingTime));
}

INSTANTIATE_TEST_SUITE_P(Original, GeometryModes,
                         ::testing::Range<std::size_t>(0, golden_geometry_modes::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_geometry_modes::kScenarios.at(info.param).name);
                         });

TEST(CncJobGeometry, MultidepthUsesExactDecimalArithmetic) {
    // Con floats, -1.8 / 0.6 daría una pasada de más (0.6 * 3 = 1.7999999999999998 no llega a 1.8).
    GeometryJobSettings s;
    s.order = DrillOrder::FileOrder;
    s.toolSettings.toolDia = 1.0;
    s.toolSettings.cutZ = -1.8;
    s.toolSettings.multidepth = true;
    s.toolSettings.depthPerPass = 0.6;
    s.toolSettings.travelZ = 2.0;
    s.toolSettings.endZ = 10.0;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometry({Shape::lineString({{0, 0}, {5, 0}})}, s)) << job.lastError();
    int downs = 0;
    for (std::size_t pos = job.gcode().find("G01 Z-"); pos != std::string::npos; pos = job.gcode().find("G01 Z-", pos + 1)) ++downs;
    EXPECT_EQ(downs, 3);
    EXPECT_NE(job.gcode().find("G01 Z-1.8000"), std::string::npos);
    EXPECT_EQ(job.gcode().find("G01 Z-2.4000"), std::string::npos);
}

// ------------------------------------------- varias herramientas en un mismo trabajo (mtool_gen_cncjob)

class GeometryMultiTool : public ::testing::TestWithParam<std::size_t> {};

TEST_P(GeometryMultiTool, MatchesOriginalGenerator) {
    const auto& sc = golden_geometry_multitool::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    std::vector<GeometryToolJob> jobs = sc.jobs();
    for (auto& j : jobs) j.settings.flavor = GcodeFlavor::FlatCAM2024_4;
    CncJob job;
    ASSERT_TRUE(job.generateFromGeometryTools(jobs)) << job.lastError();
    EXPECT_EQ(job.toolGcodes().size(), jobs.size());
    EXPECT_EQ(job.startGcode() + "\n" + job.gcode(), sc.expected);
    // travel_distance y routing_time se acumulan entre las herramientas, como en el original.
    EXPECT_NEAR(job.travelDistance(), sc.travelDistance, 1e-9 * std::max(1.0, sc.travelDistance));
    EXPECT_NEAR(job.routingTime(), sc.routingTime, 1e-9 * std::max(1.0, sc.routingTime));
}

INSTANTIATE_TEST_SUITE_P(Original, GeometryMultiTool,
                         ::testing::Range<std::size_t>(0, golden_geometry_multitool::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_geometry_multitool::kScenarios.at(info.param).name);
                         });
