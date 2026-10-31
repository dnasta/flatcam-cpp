#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "cam/CncJob.h"
#include "golden_geometry.h"

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

TEST(CncJobGeometry, UnsupportedOptionsAndErrorsAreReported) {
    GeometryJobSettings s = golden_geometry::settings();
    s.toolSettings.multidepth = true;
    CncJob multi;
    EXPECT_FALSE(multi.generateFromGeometry(contourGeometry(), s));
    EXPECT_FALSE(multi.lastError().empty());

    s = golden_geometry::settings();
    s.preprocessor = "no_existe";
    CncJob bad;
    EXPECT_FALSE(bad.generateFromGeometry(contourGeometry(), s));

    CncJob empty;
    EXPECT_FALSE(empty.generateFromGeometry({}, golden_geometry::settings()));
}
