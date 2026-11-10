// Alinear objetos contra el original (tests/tools/oracle_align.py -> golden_align.h).
#include <gtest/gtest.h>

#include <string>

#include "golden_align.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"
#include "tools/AlignObjects.h"

using namespace flatcam;

TEST(AlignObjects, PicksLikeTheOriginal) {
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/";
    Gerber g;
    ASSERT_TRUE(g.parseFile(dir + "luz de noche Pascual-B_Cu.gbr"));
    Excellon e;
    ASSERT_TRUE(e.parseFile(dir + "luz de noche Pascual-PTH.drl"));
    for (const auto& c : golden_align::kPicks) {
        SCOPED_TRACE(std::string(c.target) + " " + std::to_string(c.x) + " " + std::to_string(c.y));
        const std::vector<Point2D> r = std::string(c.target) == "gerber" ? alignPickGerber(g, {c.x, c.y})
                                                                         : alignPickExcellon(e, {c.x, c.y});
        ASSERT_EQ(r.size() * 2, c.points.size());
        for (std::size_t i = 0; i < r.size(); ++i) {
            EXPECT_EQ(r[i].x, c.points[2 * i]);
            EXPECT_EQ(r[i].y, c.points[2 * i + 1]);
        }
    }
}

TEST(AlignObjects, MovesLikeTheOriginal) {
    for (const auto& c : golden_align::kTransforms) {
        std::vector<Point2D> pts;
        for (std::size_t i = 0; i < c.points.size(); i += 2) pts.push_back({c.points[i], c.points[i + 1]});
        SCOPED_TRACE(pts.size());
        const AlignMove m = alignMove(pts);
        EXPECT_EQ(m.offset.x, c.dx);
        EXPECT_EQ(m.offset.y, c.dy);
        EXPECT_EQ(m.fails, c.fails);
        ASSERT_EQ(m.angle.has_value(), c.rotates);
        if (c.rotates) {
            EXPECT_NEAR(*m.angle, c.angle, 1e-12);
            EXPECT_EQ(m.origin.x, c.ox);
            EXPECT_EQ(m.origin.y, c.oy);
        }
    }
}
