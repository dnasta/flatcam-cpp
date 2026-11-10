// Panelizar contra el original (tests/tools/oracle_panelize.py -> golden_panelize.h).
#include <gtest/gtest.h>

#include <string>

#include "golden_panelize.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"
#include "tools/Panelize.h"

using namespace flatcam;

namespace {

BoundingBox boundsOf(const ShapeList& l) {
    BoundingBox b{1e300, 1e300, -1e300, -1e300};
    for (const Shape& s : l) {
        const auto o = s.bounds();
        if (!o) continue;
        b = {std::min(b.xmin, o->xmin), std::min(b.ymin, o->ymin), std::max(b.xmax, o->xmax), std::max(b.ymax, o->ymax)};
    }
    return b;
}

/// La misma figura: exactamente (vértice por vértice) o, si la unión cambió el orden, la misma geometría y largo.
void expectSame(const Shape& mine, const std::string& wkt, const std::string& what) {
    const Shape want = Shape::fromWkt(wkt);
    if (mine.equalsExact(want, 1e-9)) return;
    EXPECT_TRUE(mine.equals(want)) << what << "\n  port: " << mine.toWkt(10) << "\n  orig: " << wkt.substr(0, 300);
    EXPECT_NEAR(mine.length(), want.length(), 1e-9) << what;
    EXPECT_NEAR(mine.area(), want.area(), 1e-9) << what;
}

/// El corte de 10 x 10 con puentes del oráculo.
std::map<int, ShapeList> cutoutTools() {
    return {{1,
             {Shape::lineString({{4, 0}, {10, 0}, {10, 10}, {6, 10}}), Shape::lineString({{4, 10}, {0, 10}, {0, 0}, {2, 0}}),
              Shape::lineString({{2, 0}, {4, 0}})}},
            {9999, {Shape::lineString({{4, 10}, {6, 10}})}}};
}

}  // namespace

TEST(Panelize, MatchesTheOriginal) {
    const std::string data = FLATCAM_TEST_DATA_DIR;
    for (const auto& c : golden_panelize::kCases) {
        SCOPED_TRACE(c.name);
        PanelizeSettings s;
        s.columns = c.columns;
        s.rows = c.rows;
        s.spacingColumns = c.spacingX;
        s.spacingRows = c.spacingY;
        s.optimize = c.optimize;
        s.constrain = c.constrain;
        s.constrainX = c.cx;
        s.constrainY = c.cy;
        const std::string kind = c.kind;
        if (kind == "gerber") {
            Gerber g;
            ASSERT_TRUE(g.parseFile(data + "/" + c.file));
            const PanelLayout l = panelLayout(boundsOf(g.solidGeometry()), s);
            EXPECT_EQ(l.constrained, std::string(c.message).find("constrain") != std::string::npos || c.constrain);
            ShapeList solid;
            std::map<std::string, Aperture> aps;
            if (std::string(c.outKind) == "gerber") {
                panelizeGerber(g, l, solid, aps);
                ASSERT_EQ(aps.size(), c.apertures.size());
                for (const auto& [id, list] : c.apertures) {
                    ASSERT_TRUE(aps.count(id)) << id;
                    ASSERT_EQ(aps.at(id).geometry.size(), list.size()) << id;
                    for (std::size_t i = 0; i < list.size(); ++i) {
                        const ApertureGeometry& el = aps.at(id).geometry[i];
                        EXPECT_EQ(el.solid.has_value(), std::string(list[i].solid) != "");
                        if (el.solid && *list[i].solid) expectSame(*el.solid, list[i].solid, "solid " + id);
                        if (el.follow && *list[i].follow) expectSame(*el.follow, list[i].follow, "follow " + id);
                    }
                }
            } else {
                solid = panelizeShapes(g.solidGeometry(), l);
            }
            ASSERT_EQ(solid.size(), c.solid.size());
            for (std::size_t i = 0; i < solid.size(); ++i) expectSame(solid[i], c.solid[i], "solid " + std::to_string(i));
        } else if (kind == "excellon") {
            Excellon e;
            ASSERT_TRUE(e.parseFile(data + "/" + c.file));
            e.createGeometry();
            ShapeList all;
            for (const auto& [id, t] : e.drillTools()) all.insert(all.end(), t.solidGeometry.begin(), t.solidGeometry.end());
            const PanelLayout l = panelLayout(boundsOf(all), s);
            const auto out = panelizeExcellon(e, l);
            ASSERT_EQ(out->drillTools().size(), c.tools.size());
            for (const auto& [id, list] : c.tools) {
                const ExcellonTool& t = out->drillTools().at(id);
                std::vector<std::string> mine;
                ASSERT_EQ(t.drills.size() + t.slots.size(), list.size()) << id;
                for (std::size_t i = 0; i < t.drills.size(); ++i) expectSame(Shape::point(t.drills[i]), list[i], "drill");
                for (std::size_t i = 0; i < t.slots.size(); ++i)
                    expectSame(Shape::lineString({t.slots[i].first, t.slots[i].second}),
                               list[t.drills.size() + i].substr(5), "slot");
            }
        } else {
            const PanelLayout l = panelLayout({0, 0, 10, 10}, s);
            const auto out = panelizeGeometryTools(cutoutTools(), l, c.optimize);
            ASSERT_EQ(out.size(), c.tools.size());
            for (const auto& [id, list] : c.tools) {
                ASSERT_EQ(out.at(id).size(), list.size()) << id;
                for (std::size_t i = 0; i < list.size(); ++i) expectSame(out.at(id)[i], list[i], "tool " + std::to_string(id));
            }
        }
    }
}

TEST(Panelize, ConstrainReducesColumnsAndRows) {
    PanelizeSettings s;
    s.columns = 4;
    s.rows = 4;
    s.spacingColumns = 2;
    s.spacingRows = 2;
    s.constrain = true;
    s.constrainX = 200;
    s.constrainY = 120;
    const PanelLayout l = panelLayout({0, 0, 76, 76}, s);
    EXPECT_TRUE(l.constrained);
    EXPECT_EQ(l.columns, 2);
    EXPECT_EQ(l.rows, 1);
    EXPECT_DOUBLE_EQ(l.stepX, 78);
}
