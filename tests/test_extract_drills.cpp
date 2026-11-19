// Extraer taladros contra el original (tests/tools/oracle_extract_drills.py -> golden_extract_drills.h).
#include <gtest/gtest.h>

#include <map>
#include <string>

#include "golden_extract_drills.h"
#include "parsers/Gerber.h"
#include "tools/ExtractDrills.h"

using namespace flatcam;

TEST(ExtractDrills, MatchesTheOriginal) {
    std::map<std::string, Gerber> gerbers;
    for (const auto& c : golden_extract_drills::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + c.mode);
        if (!gerbers.count(c.file)) ASSERT_TRUE(gerbers[c.file].parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        ExtractDrillsSettings s;
        const std::string m = c.mode;
        s.mode = m == "fixed" ? ExtractDrillsSettings::Mode::Fixed
               : m == "ring"  ? ExtractDrillsSettings::Mode::Ring
                              : ExtractDrillsSettings::Mode::Proportional;
        s.fixedDia = c.fixedDia;
        s.ringCircular = c.rings[0];
        s.ringOblong = c.rings[1];
        s.ringSquare = c.rings[2];
        s.ringRectangular = c.rings[3];
        s.ringOther = c.rings[4];
        s.factor = c.factor;
        s.circular = c.filters[0];
        s.oblong = c.filters[1];
        s.square = c.filters[2];
        s.rectangular = c.filters[3];
        s.other = c.filters[4];
        const auto r = extractDrills(gerbers.at(c.file), s);
        ASSERT_EQ(r.has_value(), c.ok);
        if (!r) continue;
        ASSERT_EQ(r->size(), c.tools.size());
        for (std::size_t i = 0; i < r->size(); ++i) {
            const auto& want = c.tools[i];
            const ExtractedTool& t = (*r)[i];
            EXPECT_EQ(t.id, want.id);
            EXPECT_NEAR(t.dia, want.dia, 1e-12) << t.id;
            ASSERT_EQ(t.drills.size(), want.drills.size()) << t.id;
            for (std::size_t k = 0; k < t.drills.size(); ++k)
                EXPECT_TRUE(Shape::point(t.drills[k]).equalsExact(Shape::fromWkt(want.drills[k]), 1e-9)) << t.id << " " << k;
        }
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

TEST(ExtractDrillsFixed, ASinglePadIsExtracted) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX46Y46*%", "%MOMM*%", "%ADD10C,1.5*%", "D10*", "X1000000Y2000000D03*", "M02*"}));
    ExtractDrillsSettings s;
    s.mode = ExtractDrillsSettings::Mode::Fixed;
    s.fixedDia = 0.8;
    {
        OriginalBugsScope o(true);
        EXPECT_FALSE(extractDrills(g, s).has_value());  // "No drills extracted": el único pad no cuenta
        s.mode = ExtractDrillsSettings::Mode::Proportional;
        EXPECT_FALSE(extractDrills(g, s).has_value());
    }
    OriginalBugsScope fixed(false);
    for (auto m : {ExtractDrillsSettings::Mode::Fixed, ExtractDrillsSettings::Mode::Proportional}) {
        s.mode = m;
        const auto r = extractDrills(g, s);
        ASSERT_TRUE(r.has_value());
        ASSERT_EQ(r->size(), 1u);
        ASSERT_EQ(r->front().drills.size(), 1u);
        EXPECT_NEAR(r->front().drills[0].x, 1.0, 1e-9);
    }
}
