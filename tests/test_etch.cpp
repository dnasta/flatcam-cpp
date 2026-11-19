// Compensación de grabado contra el original (tests/tools/oracle_etch.py -> golden_etch.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <string>

#include "golden_etch.h"
#include "parsers/Gerber.h"
#include "tools/EtchCompensation.h"

using namespace flatcam;

TEST(EtchCompensation, OffsetLikeTheOriginal) {
    EXPECT_DOUBLE_EQ(*etchOffset(EtchRatio::Factor, 18.0, 2.0, "", 0.0), 0.018 / 0.5);
    EXPECT_DOUBLE_EQ(*etchOffset(EtchRatio::EtchList, 35.0, 1.0, "CuCl2", 0.0), 0.035 / 0.33);
    EXPECT_DOUBLE_EQ(*etchOffset(EtchRatio::EtchList, 18.0, 1.0, "Fe3Cl", 0.0), 0.018 / 0.25);
    EXPECT_DOUBLE_EQ(*etchOffset(EtchRatio::Manual, 18.0, 1.0, "", 25.0), 0.025);
    EXPECT_FALSE(etchOffset(EtchRatio::Factor, 18.0, 0.0, "", 0.0).has_value());
}

TEST(EtchCompensation, MatchesTheOriginal) {
    for (const auto& c : golden_etch::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + c.type);
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        const std::string t = c.type;
        const EtchRatio ratio = t == "factor" ? EtchRatio::Factor : t == "etch_list" ? EtchRatio::EtchList
                                                                                    : EtchRatio::Manual;
        const double off = *etchOffset(ratio, c.thickness, c.factor, c.etchant, c.offset);
        const EtchResult r = etchCompensate(g, off, 64);
        // Los mismos polígonos tras la unión, con las mismas áreas (hasta una micra cuadrada).
        ASSERT_EQ(static_cast<int>(r.solid.size()), c.polygons);
        std::vector<double> areas;
        for (const Shape& s : r.solid) areas.push_back(s.area());
        std::sort(areas.begin(), areas.end());
        for (std::size_t i = 0; i < areas.size(); ++i) EXPECT_NEAR(areas[i], c.areas[i], 1e-6) << i;
        // Las mismas aperturas, con sus tamaños y el área de sus figuras.
        ASSERT_EQ(r.apertures.size(), c.apertures.size());
        for (const auto& a : c.apertures) {
            SCOPED_TRACE(a.id);
            const auto it = r.apertures.find(a.id);
            ASSERT_NE(it, r.apertures.end());
            const Aperture& ap = it->second;
            if (a.size < 0) EXPECT_FALSE(ap.size.has_value());
            else EXPECT_NEAR(ap.size.value_or(0.0), a.size, 1e-12);
            if (a.width >= 0) EXPECT_NEAR(ap.width, a.width, 1e-12);
            if (a.height >= 0) EXPECT_NEAR(ap.height, a.height, 1e-12);
            double area = 0.0;
            for (const ApertureGeometry& el : ap.geometry)
                if (el.solid) area += el.solid->area();
            EXPECT_NEAR(area, a.area, 1e-6 * std::max(1.0, a.area));
        }
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

TEST(EtchCompensationFixed, SizesGrowTwiceTheOffsetAndFollowIsKept) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX46Y46*%", "%MOMM*%", "%ADD10C,1*%", "%ADD11R,2X1*%", "%ADD12P,2X6*%", "D10*",
                              "X0Y0D02*", "X5000000Y0D01*", "D11*", "X10000000Y0D03*", "D12*", "X15000000Y0D03*",
                              "G36*", "X0Y5000000D02*", "X1000000Y5000000D01*", "X1000000Y6000000D01*",
                              "X0Y5000000D01*", "G37*", "M02*"}));
    OriginalBugsScope fixed(false);
    const EtchResult r = etchCompensate(g, 0.1, 64);
    EXPECT_NEAR(*r.apertures.at("10").size, 1.2, 1e-12);
    EXPECT_NEAR(r.apertures.at("11").width, 2.2, 1e-12);
    EXPECT_NEAR(r.apertures.at("11").height, 1.2, 1e-12);
    EXPECT_NEAR(r.apertures.at("12").diam, 2.2, 1e-12);
    for (const auto& [id, ap] : r.apertures)
        if (ap.type == "REG") EXPECT_NEAR(ap.size.value_or(0.0), 0.0, 1e-12) << id;  // las regiones no cambian
    EXPECT_EQ(r.follow.size(), g.followGeometry().size());
    EXPECT_FALSE(r.follow.empty());
}
