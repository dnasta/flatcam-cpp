// Asistente de PCB contra el original (tests/tools/oracle_pcbwizard.py).
#include <gtest/gtest.h>

#include <cmath>

#include "golden_pcbwizard.h"
#include "parsers/Excellon.h"
#include "tools/PcbWizard.h"

using namespace flatcam;

namespace {
std::string dataPath(const std::string& f) { return std::string(FLATCAM_TEST_DATA_DIR) + "/pcbwizard/" + f; }
}  // namespace

TEST(PcbWizard, MatchesTheOriginal) {
    for (const auto& c : golden_pcbwizard::kCases) {
        SCOPED_TRACE(std::string(c.exc) + " + " + c.inf);
        const auto exc = pcbWizardReadLines(dataPath(c.exc));
        PcbWizardInf inf;
        const bool ok = pcbWizardReadInf(pcbWizardReadLines(dataPath(c.inf)), inf);
        EXPECT_EQ(!ok, c.failed);
        std::vector<double> tools;
        for (const auto& [t, d] : inf.tools) {
            tools.push_back(t);
            tools.push_back(d);
        }
        EXPECT_EQ(tools, c.infTools);
        // set_tool_ui y lo que el .INF cambia del formulario.
        const int integral = inf.integral.value_or(2), fractional = inf.fractional.value_or(4);
        const std::string units = inf.units.value_or("INCH");
        EXPECT_EQ(integral, c.integral);
        EXPECT_EQ(fractional, c.fractional);
        EXPECT_EQ(units, c.units);
        if (!ok) continue;
        const std::string zeros = std::string(c.zeros).empty() ? "LZ" : c.zeros;
        const std::string text =
            pcbWizardMerge(exc, inf, integral, fractional, units, zeros, "2024.4", "2024/04/15", golden_pcbwizard::kTime);
        EXPECT_EQ(text, c.text);
        // El Excellon que resulta, con el parser del port.
        std::vector<std::string> lines;
        std::size_t a = 0;
        while (a < text.size()) {
            const std::size_t b = text.find('\n', a);
            lines.push_back(text.substr(a, b - a));
            a = b + 1;
        }
        Excellon e;
        ASSERT_TRUE(e.parseLines(lines));
        e.createGeometry();
        std::size_t n = 0;
        for (const auto& [id, t] : e.drillTools()) {
            if (t.drills.empty() && t.tooldia == 0.0) continue;
            ++n;
        }
        for (const auto& g : c.drills) {
            SCOPED_TRACE("T" + std::to_string(g.tool));
            const auto it = e.drillTools().find(g.tool);
            ASSERT_NE(it, e.drillTools().end());
            if (g.dia >= 0) EXPECT_DOUBLE_EQ(it->second.tooldia, g.dia);
            ASSERT_EQ(it->second.drills.size() * 2, g.xy.size());
            for (std::size_t i = 0; i < it->second.drills.size(); ++i) {
                EXPECT_NEAR(it->second.drills[i].x, g.xy[2 * i], 1e-12);
                EXPECT_NEAR(it->second.drills[i].y, g.xy[2 * i + 1], 1e-12);
            }
        }
        EXPECT_EQ(n, c.drills.size());
    }
}

TEST(PcbWizard, ToolsAccumulateBetweenInfFiles) {
    // Como el original: tools_from_inf solo se vacía en set_tool_ui.
    PcbWizardInf inf;
    ASSERT_TRUE(pcbWizardReadInf(pcbWizardReadLines(dataPath("board.inf")), inf));
    ASSERT_TRUE(pcbWizardReadInf(pcbWizardReadLines(dataPath("nounits.inf")), inf));
    ASSERT_EQ(inf.tools.size(), 3u);
    EXPECT_EQ(inf.tools[2].first, 5);
    EXPECT_EQ(*inf.units, "METRIC");
}
