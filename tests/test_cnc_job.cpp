#include <gtest/gtest.h>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>

#include "cam/CncJob.h"
#include "cam/PathOptimizer.h"
#include "golden_drill.h"
#include "parsers/Excellon.h"

using namespace flatcam;

namespace {

/// Excellon del taladro del usuario, reflejado en X como en su proyecto.
Excellon loadDrill() {
    ExcellonOptions opt;
    opt.zeros = "T";
    opt.excellonUnits = "METRIC";
    Excellon ex(opt);
    EXPECT_TRUE(ex.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Drill.drl"));
    ex.mirror(Axis::Y, {0, 0});
    return ex;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::string cur;
    for (char c : text) {
        if (c == '\r') continue;
        if (c == '\n') {
            lines.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    lines.push_back(cur);
    return lines;
}

/// Separa el G-code en líneas "fijas" (en orden) y bloques de agujero (en un conjunto).
struct Parts {
    std::vector<std::string> fixed;
    std::vector<std::string> holes;
};

Parts splitHoles(const std::vector<std::string>& lines) {
    Parts parts;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const bool isHole = lines[i].rfind("G00 X", 0) == 0 && i + 3 < lines.size() && lines[i + 1].rfind("G01 Z", 0) == 0 &&
                            lines[i + 2] == "G01 Z0" && lines[i + 3].rfind("G00 Z", 0) == 0 &&
                            lines[i].find("                ") == std::string::npos;
        if (isHole) {
            parts.holes.push_back(lines[i] + "|" + lines[i + 1] + "|" + lines[i + 2] + "|" + lines[i + 3]);
            i += 3;
        } else {
            parts.fixed.push_back(lines[i]);
        }
    }
    std::sort(parts.holes.begin(), parts.holes.end());
    return parts;
}

}  // namespace

// ------------------------------------------------------------- contra el código original (2024.4)

TEST(CncJobDrill, FileOrderMatchesOriginalGeneratorExactly) {
    const Excellon ex = loadDrill();
    CncJob job(golden_drill::settings());
    ASSERT_TRUE(job.generateFromExcellon(ex)) << job.lastError();

    EXPECT_EQ(job.startGcode(), golden_drill::kStartGcode);
    ASSERT_EQ(job.toolGcodes().size(), golden_drill::kToolGcodes.size());
    for (std::size_t i = 0; i < job.toolGcodes().size(); ++i) {
        EXPECT_EQ(job.toolGcodes()[i], golden_drill::kToolGcodes[i]) << "herramienta #" << i + 1;
    }
    EXPECT_EQ(job.exportGcode(golden_drill::header()), golden_drill::kFullFile);
    // Distancia recorrida y tiempo estimado (ToolDrilling): los del original.
    EXPECT_NEAR(job.travelDistance(), golden_drill::kTravelDistance, 1e-9 * golden_drill::kTravelDistance);
    EXPECT_NEAR(job.routingTime(), golden_drill::kRoutingTime, 1e-9 * golden_drill::kRoutingTime);
}

TEST(CncJobDrill, EveryExcellonPreprocessorMatchesOriginalGenerator) {
    const Excellon ex = loadDrill();
    for (const auto& run : golden_drill::kOtherPreprocessors) {
        SCOPED_TRACE(run.pp);
        DrillJobSettings s = golden_drill::settings();
        s.preprocessor = run.pp;
        s.flavor = GcodeFlavor::FlatCAM2024_4;
        CncJob job(s);
        ASSERT_TRUE(job.generateFromExcellon(ex)) << job.lastError();
        EXPECT_EQ(job.startGcode(), run.start);
        ASSERT_EQ(job.toolGcodes().size(), run.tools.size());
        for (std::size_t i = 0; i < run.tools.size(); ++i)
            EXPECT_EQ(job.toolGcodes()[i], run.tools[i]) << "herramienta #" << i + 1;
    }
}

TEST(CncJobDrill, ExclusionAreasMatchOriginalGenerator) {
    // Los viajes entre agujeros esquivan las áreas (travel_coordinates original): por encima y alrededor.
    const Excellon ex = loadDrill();
    for (std::size_t r = 0; r < golden_drill::kExclusionRuns.size(); ++r) {
        const auto& run = golden_drill::kExclusionRuns[r];
        SCOPED_TRACE(r);
        DrillJobSettings s = golden_drill::settings();
        s.exclusionAreas = run.areas();
        CncJob job(s);
        ASSERT_TRUE(job.generateFromExcellon(ex)) << job.lastError();
        EXPECT_EQ(job.startGcode(), run.start);
        ASSERT_EQ(job.toolGcodes().size(), run.tools.size());
        for (std::size_t i = 0; i < run.tools.size(); ++i)
            EXPECT_EQ(job.toolGcodes()[i], run.tools[i]) << "herramienta #" << i + 1;
    }
}

// ------------------------------------------- contra el G-code real generado por FlatCAM 8.994 del usuario

TEST(CncJobDrill, OptimizedMatchesUserFileExceptHoleOrder) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/DosCau_PPal-Drill.nc", std::ios::binary);
    ASSERT_TRUE(in.good());
    std::stringstream buf;
    buf << in.rdbuf();
    const std::vector<std::string> real = splitLines(buf.str());
    ASSERT_GT(real.size(), 20u);

    GcodeHeaderInfo h = golden_drill::header();
    for (const auto& line : real) {
        if (line.rfind("(Created on ", 0) == 0) h.createdOn = line.substr(12, line.size() - 13);
    }
    DrillJobSettings s = golden_drill::settings();
    s.order = DrillOrder::Optimized;
    CncJob job(s);
    ASSERT_TRUE(job.generateFromExcellon(loadDrill())) << job.lastError();
    const std::vector<std::string> mine = splitLines(job.exportGcode(h));

    const Parts a = splitHoles(real), b = splitHoles(mine);
    EXPECT_EQ(b.holes.size(), 192u);
    EXPECT_EQ(a.holes, b.holes) << "no se taladran los mismos agujeros con las mismas profundidades";
    ASSERT_EQ(a.fixed.size(), b.fixed.size());
    for (std::size_t i = 0; i < a.fixed.size(); ++i) {
        EXPECT_EQ(a.fixed[i], b.fixed[i]) << "línea fija #" << i;
    }
}

// ------------------------------------------------------------------------------------ optimizador

TEST(PathOptimizer, ReturnsPermutationAndNeverLosesToFileOrder) {
    const Excellon ex = loadDrill();
    for (const auto& [id, tool] : ex.drillTools()) {
        const auto order = PathOptimizer::order(tool.drills, {0, 0});
        ASSERT_EQ(order.size(), tool.drills.size());
        std::vector<std::size_t> sorted = order;
        std::sort(sorted.begin(), sorted.end());
        for (std::size_t i = 0; i < sorted.size(); ++i) EXPECT_EQ(sorted[i], i) << "herramienta " << id;

        std::vector<std::size_t> fileOrder(tool.drills.size());
        for (std::size_t i = 0; i < fileOrder.size(); ++i) fileOrder[i] = i;
        EXPECT_LE(PathOptimizer::pathLength(tool.drills, order, {0, 0}),
                  PathOptimizer::pathLength(tool.drills, fileOrder, {0, 0}) + 1e-9)
            << "herramienta " << id;
    }
}

/// Longitud del recorrido de cada herramienta según el orden de los agujeros en un G-code.
std::map<int, double> toolPathLengths(const std::vector<std::string>& lines) {
    std::map<int, std::vector<Point2D>> seq;
    int tool = 0;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].size() >= 2 && lines[i][0] == 'T' && std::isdigit(static_cast<unsigned char>(lines[i][1]))) {
            tool = std::stoi(lines[i].substr(1));
        } else if (tool != 0 && lines[i].rfind("G00 X", 0) == 0 && i + 1 < lines.size() && lines[i + 1].rfind("G01 Z-", 0) == 0) {
            double x = 0, y = 0;
            std::sscanf(lines[i].c_str(), "G00 X%lf Y%lf", &x, &y);
            seq[tool].push_back({x, y});
        }
    }
    std::map<int, double> out;
    for (const auto& [t, pts] : seq) {
        std::vector<std::size_t> idx(pts.size());
        for (std::size_t i = 0; i < idx.size(); ++i) idx[i] = i;
        out[t] = PathOptimizer::pathLength(pts, idx, {0, 0});
    }
    return out;
}

TEST(PathOptimizer, IsNotWorseThanOrToolsRouteOfTheUserFile) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/DosCau_PPal-Drill.nc", std::ios::binary);
    std::stringstream buf;
    buf << in.rdbuf();
    const auto real = toolPathLengths(splitLines(buf.str()));
    DrillJobSettings s = golden_drill::settings();
    s.order = DrillOrder::Optimized;
    CncJob job(s);
    ASSERT_TRUE(job.generateFromExcellon(loadDrill()));
    const auto mine = toolPathLengths(splitLines(job.exportGcode(golden_drill::header())));
    ASSERT_EQ(real.size(), mine.size());
    for (const auto& [t, len] : real) {
        EXPECT_LE(mine.at(t), len * 1.10) << "herramienta " << t << ": mío " << mine.at(t) << " mm, OR-Tools " << len << " mm";
    }
}

TEST(PathOptimizer, IsDeterministicAndHandlesSmallInputs) {
    EXPECT_TRUE(PathOptimizer::order({}, {0, 0}).empty());
    const std::vector<Point2D> one{{5, 5}};
    EXPECT_EQ(PathOptimizer::order(one, {0, 0}), (std::vector<std::size_t>{0}));
    const std::vector<Point2D> line{{10, 0}, {2, 0}, {6, 0}, {4, 0}};
    EXPECT_EQ(PathOptimizer::order(line, {0, 0}), (std::vector<std::size_t>{1, 3, 2, 0}));
    EXPECT_EQ(PathOptimizer::order(line, {0, 0}), PathOptimizer::order(line, {0, 0}));
}

TEST(CncJobDrill, FailsWithoutDrillsOrUnknownPreprocessor) {
    ExcellonOptions opt;
    Excellon empty(opt);
    ASSERT_TRUE(empty.parseLines({"M48", "METRIC", "T1C0.8", "%", "M30"}));
    CncJob job(golden_drill::settings());
    EXPECT_FALSE(job.generateFromExcellon(empty));
    EXPECT_FALSE(job.lastError().empty());

    DrillJobSettings s = golden_drill::settings();
    s.preprocessor = "no_existe";
    CncJob bad(s);
    EXPECT_FALSE(bad.generateFromExcellon(loadDrill()));
}
