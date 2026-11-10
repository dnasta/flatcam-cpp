// "Geometría de fresado" del Excellon contra el original (tests/tools/oracle_milling.py -> golden_milling.h).
#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include "golden_milling.h"
#include "parsers/Excellon.h"
#include "tools/ExcellonMilling.h"

using namespace flatcam;

TEST(ExcellonMilling, MatchesTheOriginal) {
    for (const auto& c : golden_milling::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + c.op + " " + std::to_string(c.tooldia));
        Excellon exc;
        ASSERT_TRUE(exc.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        exc.createGeometry();
        const MillingResult r = std::string(c.op) == "drills" ? millDrills(exc, c.tools, c.tooldia)
                                                               : millSlots(exc, c.tools, c.tooldia);
        EXPECT_EQ(r.ok(), c.ok);
        const std::string msg = c.message;
        if (msg.find("larger") != std::string::npos) EXPECT_EQ(r.error, MillingResult::Error::ToolTooLarge);
        if (msg.find("select one") != std::string::npos) EXPECT_EQ(r.error, MillingResult::Error::NoTools);
        ASSERT_EQ(r.rings.size(), c.rings.size());
        for (std::size_t i = 0; i < r.rings.size(); ++i) {
            const Shape want = Shape::fromWkt(c.rings[i]);
            const auto a = r.rings[i].coordinates(), b = want.coordinates();
            ASSERT_EQ(a.size(), b.size()) << i;
            for (std::size_t k = 0; k < a.size(); ++k) {
                EXPECT_NEAR(a[k].x, b[k].x, 1e-9);
                EXPECT_NEAR(a[k].y, b[k].y, 1e-9);
            }
        }
    }
}
