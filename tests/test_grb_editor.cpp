// Editor de Gerber contra el original (tests/tools/oracle_grb_editor.py -> golden_grb_editor.h).
#include <gtest/gtest.h>

#include <string>

#include "editors/GrbEditorModel.h"
#include "golden_grb_editor.h"
#include "parsers/Gerber.h"

using namespace flatcam;

TEST(GrbEditor, LoadPadsAndSaveMatchTheOriginal) {
    for (const auto& c : golden_grb_editor::kCases) {
        SCOPED_TRACE(c.file);
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        GrbEditorModel m;
        m.load(g);
        ASSERT_EQ(m.apertures.size(), c.loaded.size());
        for (std::size_t i = 0; i < c.loaded.size(); ++i) {
            SCOPED_TRACE(c.loaded[i].code);
            EXPECT_EQ(m.apertures[i].code, c.loaded[i].code);
            EXPECT_EQ(static_cast<int>(m.apertures[i].geometry.size()), c.loaded[i].count);
            double area = 0.0;
            for (const auto& s : m.apertures[i].geometry)
                if (s->solid) area += s->solid->area();
            EXPECT_NEAR(area, c.loaded[i].area, 1e-6 * std::max(1.0, area));
        }
        // Un pad de cada apertura C, R y O.
        std::size_t k = 0;
        for (const auto& a : m.apertures) {
            const auto e = grbdraw::pad(a.ap, {1.25, -2.5}, 64);
            if (!e) continue;
            ASSERT_LT(k, c.pads.size());
            const std::string want = c.pads[k++];
            const auto bar = want.find('|');
            EXPECT_EQ(a.code, want.substr(0, bar));
            EXPECT_TRUE(e->solid->equalsExact(Shape::fromWkt(want.substr(bar + 1)), 1e-9)) << a.code;
        }
        EXPECT_EQ(k, c.pads.size());
        // Guardar.
        const auto out = m.result();
        EXPECT_NEAR(static_cast<double>(out.solid.size()), c.solidCount, 2);
        double area = 0.0;
        for (const Shape& s : out.solid) area += s.area();
        EXPECT_NEAR(area, c.solidArea, 1e-5 * c.solidArea);
        EXPECT_EQ(static_cast<int>(out.follow.size()), c.follow);
        ASSERT_EQ(out.order.size(), c.order.size());
        for (std::size_t i = 0; i < out.order.size(); ++i) EXPECT_EQ(out.order[i], c.order[i]);
    }
}

TEST(GrbEditor, SemiDiscsMatchTheOriginal) {
    for (const auto& s : golden_grb_editor::kSemi) {
        const std::string mode = s.mode;
        SCOPED_TRACE(mode + " " + s.dir);
        const int md = mode == "c12" ? 0 : mode == "132" ? 1 : 2;
        const bool c12 = md == 0;
        const Point2D a{0, 0}, b = c12 ? Point2D{3, 0} : Point2D{4, 0}, c = c12 ? Point2D{0, 3} : Point2D{2, 1.5};
        const auto e = grbdraw::semiDisc(md, a, b, c, std::string(s.dir) == "cw", 0.2, 64);
        ASSERT_TRUE(e.has_value());
        EXPECT_TRUE(e->solid->equalsExact(Shape::fromWkt(s.wkt), 1e-9)) << e->solid->toWkt() << "\n" << s.wkt;
    }
}
