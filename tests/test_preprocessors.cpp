#include <gtest/gtest.h>

#include <functional>
#include <map>
#include <string>

#include "cam/Preprocessor.h"
#include "golden_preprocessors.h"

using namespace flatcam;

TEST(PyNumber, FloatReprMatchesPython) {
    for (const auto& f : golden_pp::kFloats) {
        EXPECT_EQ(pyFloatRepr(f.value), f.repr) << "valor " << f.repr;
    }
}

TEST(PyNumber, IntegersAndFloatsPrintDifferently) {
    EXPECT_EQ(PyNumber(1500).str(), "1500");
    EXPECT_EQ(PyNumber(1500.0).str(), "1500.0");
    EXPECT_EQ(PyNumber(-4).str(), "-4");
    EXPECT_TRUE(PyNumber(3).isInt());
    EXPECT_FALSE(PyNumber(3.0).isInt());
    EXPECT_FALSE(PyNumber(0).truthy());
    EXPECT_TRUE(PyNumber(0.1).truthy());
}

TEST(Preprocessor, FactoryFindsKnownNames) {
    EXPECT_NE(makePreprocessor("default"), nullptr);
    EXPECT_EQ(makePreprocessor("GRBL_laser")->name(), "GRBL_laser");
    EXPECT_EQ(makePreprocessor("no_existe"), nullptr);
    // Los 20 postprocesadores de FlatCAM 2024.4.
    for (const char* n : {"Berta_CNC", "default", "Default_no_M6", "grbl_11", "GRBL_11_no_M6", "GRBL_laser", "hpgl",
                          "ISEL_CNC", "ISEL_ICP_CNC", "line_xyz", "Marlin", "Marlin_laser_FAN_pin",
                          "Marlin_laser_Spindle_pin", "Paste_1", "Repetier", "Roland_MDX_20", "Toolchange_Custom",
                          "Toolchange_Manual", "Toolchange_Probe_MACH3", "Z_laser"}) {
        const auto pp = makePreprocessor(n);
        ASSERT_NE(pp, nullptr) << n;
        EXPECT_EQ(pp->name(), n);
    }
}

class PreprocessorGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(PreprocessorGolden, MatchesOriginalPreprocessor) {
    static const std::map<std::string, std::function<std::string(const Preprocessor&, const PreprocessorParams&)>> calls = {
        {"startCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.startCode(p); }},
        {"startzCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.startzCode(p); }},
        {"liftCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.liftCode(p); }},
        {"downCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.downCode(p); }},
        {"toolchangeCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.toolchangeCode(p); }},
        {"upToZeroCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.upToZeroCode(p); }},
        {"rapidCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.rapidCode(p); }},
        {"linearCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.linearCode(p); }},
        {"endCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.endCode(p); }},
        {"feedrateCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.feedrateCode(p); }},
        {"zFeedrateCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.zFeedrateCode(p); }},
        {"spindleCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.spindleCode(p); }},
        {"dwellCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.dwellCode(p); }},
        {"spindleStopCode", [](const Preprocessor& pp, const PreprocessorParams& p) { return pp.spindleStopCode(p); }},
    };
    // Los métodos propios de Paste_1 (pasta de soldar).
    using PasteFn = std::string (SolderPastePreprocessor::*)(const PreprocessorParams&) const;
    static const std::map<std::string, PasteFn> paste = {
        {"downZStartCode", &SolderPastePreprocessor::downZStartCode},
        {"liftZDispenseCode", &SolderPastePreprocessor::liftZDispenseCode},
        {"downZStopCode", &SolderPastePreprocessor::downZStopCode},
        {"feedrateXYCode", &SolderPastePreprocessor::feedrateXYCode},
        {"feedrateZDispenseCode", &SolderPastePreprocessor::feedrateZDispenseCode},
        {"spindleFwdCode", &SolderPastePreprocessor::spindleFwdCode},
        {"spindleRevCode", &SolderPastePreprocessor::spindleRevCode},
        {"spindleOffCode", &SolderPastePreprocessor::spindleOffCode},
        {"dwellFwdCode", &SolderPastePreprocessor::dwellFwdCode},
        {"dwellRevCode", &SolderPastePreprocessor::dwellRevCode},
    };
    const auto& c = golden_pp::kCases.at(GetParam());
    SCOPED_TRACE(std::string(c.pp) + " / " + golden_pp::kScenarioNames[c.scenario] + " / " + c.method);
    const auto pp = makePreprocessor(c.pp, GcodeFlavor::FlatCAM2024_4);  // los goldens salen del código de la 2024.4
    ASSERT_NE(pp, nullptr);
    const auto it = paste.find(c.method);
    if (it != paste.end()) {
        const auto* sp = dynamic_cast<const SolderPastePreprocessor*>(pp.get());
        ASSERT_NE(sp, nullptr);
        EXPECT_EQ((sp->*(it->second))(golden_pp::scenario(c.scenario)), c.expected);
        return;
    }
    EXPECT_EQ(calls.at(c.method)(*pp, golden_pp::scenario(c.scenario)), c.expected);
}

INSTANTIATE_TEST_SUITE_P(Original, PreprocessorGolden, ::testing::Range<std::size_t>(0, golden_pp::kCases.size()));

// De punta a punta: un trabajo de geometría con cada postprocesador. Comprueba que el CNC Job usa la cabecera y los
// movimientos del postprocesador elegido (los textos exactos ya los comparan los casos del original).
#include "cam/CncJob.h"
#include "core/Shape.h"

TEST(Preprocessor, EveryPreprocessorDrivesAGeometryJob) {
    for (const char* n : {"Berta_CNC", "Default_no_M6", "grbl_11", "GRBL_11_no_M6", "hpgl", "ISEL_CNC", "ISEL_ICP_CNC",
                          "line_xyz", "Marlin", "Marlin_laser_FAN_pin", "Marlin_laser_Spindle_pin", "Repetier",
                          "Roland_MDX_20", "Toolchange_Custom", "Toolchange_Manual", "Toolchange_Probe_MACH3",
                          "Z_laser"}) {
        SCOPED_TRACE(n);
        GeometryJobSettings s;
        s.preprocessor = n;
        s.order = DrillOrder::FileOrder;
        s.toolSettings.toolDia = 1.0;
        s.toolSettings.cutZ = -1.0;
        s.toolSettings.travelZ = 2.0;
        s.toolSettings.feedrate = 100.0;
        s.toolSettings.feedrateZ = 50.0;
        s.toolSettings.feedrateRapid = 1000.0;
        s.toolSettings.endZ = 10.0;
        s.toolSettings.toolchangeZ = 15.0;
        CncJob job;
        ASSERT_TRUE(job.generateFromGeometry({Shape::lineString({{0, 0}, {10, 0}, {10, 5}})}, s)) << job.lastError();
        const auto pp = makePreprocessor(n);
        PreprocessorParams p;
        p.coordsDecimals = 4;
        p.units = "MM";
        p.x = 10;
        p.y = 5;
        p.zMove = PyNumber(2.0);
        p.zCut = PyNumber(-1.0);
        p.z = PyNumber(-1.0);
        p.feedrate = PyNumber(100.0);
        p.feedrateRapid = PyNumber(1000.0);
        // El último punto del trazado aparece con el formato del postprocesador.
        EXPECT_NE(job.gcode().find(pp->linearCode(p)), std::string::npos) << job.gcode();
        EXPECT_FALSE(job.startGcode().empty());
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

namespace {
/// El texto de un método con los errores del original y corregido.
template <class F>
std::pair<std::string, std::string> bothWays(F f) {
    std::string orig;
    {
        OriginalBugsScope o(true);
        orig = f();
    }
    OriginalBugsScope fixed(false);
    return {orig, f()};
}
}  // namespace

TEST(PreprocessorFixed, HeaderAndToolchangeBugs) {
    PreprocessorParams p = golden_pp::scenario(0);  // corte en mm: con X,Y final, sin X,Y de cambio
    // Marlin: ";Spindle Speed: ... RPM)" sin el paréntesis de cierre.
    auto [m0, m1] = bothWays([&] { return makePreprocessor("Marlin")->startCode(p); });
    EXPECT_NE(m0.find(" RPM)\n"), std::string::npos);
    EXPECT_EQ(m1.find(" RPM)\n"), std::string::npos);
    EXPECT_NE(m1.find(";Spindle Speed: 1000.0 RPM\n"), std::string::npos) << m1;
    // Berta_CNC: el X,Y final aunque no haya X,Y de cambio.
    ASSERT_FALSE(p.xyToolchange.has_value());
    auto [b0, b1] = bothWays([&] { return makePreprocessor("Berta_CNC")->startCode(p); });
    EXPECT_NE(b0.find("(X,Y End: None mm)"), std::string::npos) << b0;
    EXPECT_NE(b1.find("(X,Y End: 0.0000, 0.0000 mm)"), std::string::npos) << b1;
    // Toolchange_Custom: M6 también sin X,Y de cambio.
    auto [t0, t1] = bothWays([&] { return makePreprocessor("Toolchange_Custom")->toolchangeCode(p); });
    EXPECT_EQ(t0.find("M6"), std::string::npos);
    EXPECT_NE(t1.find("M6"), std::string::npos);
    // line_xyz: la Y del cambio de herramienta es la Y.
    p.xyToolchange = std::array<PyNumber, 2>{PyNumber(1.5), PyNumber(7.25)};
    auto [l0, l1] = bothWays([&] { return makePreprocessor("line_xyz")->toolchangeCode(p); });
    EXPECT_NE(l0.find("X1.5000 Y1.5000"), std::string::npos) << l0;
    EXPECT_NE(l1.find("X1.5000 Y7.2500"), std::string::npos) << l1;
    // Marlin_laser: con Z inicial va a la Z inicial, no a la de viaje.
    p.startZ = PyNumber(5.0);
    auto [z0, z1] = bothWays([&] { return makePreprocessor("Marlin_laser_FAN_pin")->startzCode(p); });
    EXPECT_EQ(z0, "G0 Z" + std::string(p.zMove.value() == 2.0 ? "2.0000" : "?"));
    EXPECT_EQ(z1, "G0 Z5.0000");
}

TEST(PreprocessorFixed, MultiDepthPassesUseTheirOwnZ) {
    // line_xyz con profundidad múltiple: el original corta a la Z final en todos los tramos de cada pasada.
    GeometryJobSettings s;
    s.preprocessor = "line_xyz";
    s.order = DrillOrder::FileOrder;
    s.toolSettings.toolDia = 1.0;
    s.toolSettings.cutZ = -1.0;
    s.toolSettings.multidepth = true;
    s.toolSettings.depthPerPass = 0.5;
    s.toolSettings.travelZ = 2.0;
    s.toolSettings.feedrate = 100.0;
    s.toolSettings.feedrateZ = 50.0;
    s.toolSettings.endZ = 10.0;
    auto [g0, g1] = bothWays([&] {
        CncJob job;
        EXPECT_TRUE(job.generateFromGeometry({Shape::lineString({{0, 0}, {10, 0}, {10, 5}})}, s)) << job.lastError();
        return job.gcode();
    });
    EXPECT_EQ(g0.find("G01 X10.0000 Y0.0000 Z-0.5000"), std::string::npos) << g0;
    EXPECT_NE(g1.find("G01 X10.0000 Y0.0000 Z-0.5000"), std::string::npos) << g1;
    EXPECT_NE(g1.find("G01 X10.0000 Y0.0000 Z-1.0000"), std::string::npos) << g1;
}
