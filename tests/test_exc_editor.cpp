// Editor de Excellon contra el original (tests/tools/oracle_exc_editor.py -> golden_exc_editor.h).
#include <gtest/gtest.h>

#include <cmath>
#include <set>
#include <string>

#include "editors/ExcEditorModel.h"
#include "golden_exc_editor.h"
#include "parsers/Excellon.h"

using namespace flatcam;

namespace {

/// Con GEOS 3.11 (la del oráculo) coinciden vértice por vértice; con las más nuevas un engrosado puede empezar en
/// otro vértice: la misma figura.
bool same(const Shape& a, const Shape& b) {
    if (a.equalsExact(b, 1e-9)) return true;
    if (Shape::geosVersion().rfind("3.11.", 0) == 0) return false;
    return a.type() == b.type() && a.symmetricDifference(b).area() < 1e-9 * std::max(1.0, a.area());
}

void expectStorages(const ExcEditorModel& m, const std::vector<golden_exc_editor::Storage>& g) {
    ASSERT_EQ(m.storages.size(), g.size());
    for (std::size_t i = 0; i < g.size(); ++i) {
        SCOPED_TRACE(g[i].dia);
        EXPECT_EQ(m.storages[i].dia, g[i].dia);
        const auto o = m.storages[i].storage.objects();
        ASSERT_EQ(o.size(), g[i].shapes.size());
        for (std::size_t k = 0; k < o.size(); ++k) EXPECT_TRUE(same(o[k]->geo, Shape::fromWkt(g[i].shapes[k]))) << k;
    }
}

}  // namespace

TEST(ExcEditor, LoadSelectResizeAndSaveMatchTheOriginal) {
    for (const auto& c : golden_exc_editor::kCases) {
        SCOPED_TRACE(c.file);
        Excellon e;
        ASSERT_TRUE(e.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        e.createGeometry();
        ExcEditorModel m;
        m.load(e, 4);
        expectStorages(m, c.loaded);
        std::vector<EditorShapePtr> all;
        for (const auto& t : m.storages)
            for (const auto& s : t.storage.objects()) all.push_back(s);
        m.clickSelect({c.click[0], c.click[1]}, false);
        std::vector<int> sel;
        for (const auto& s : m.selected) sel.push_back(static_cast<int>(std::find(all.begin(), all.end(), s) - all.begin()));
        EXPECT_EQ(sel, c.clickSelected);
        m.selected.clear();
        for (int i : c.resizeSel) m.selected.push_back(all.at(static_cast<std::size_t>(i)));
        const auto r = m.resize({m.storages.front().dia}, c.resizeDia);
        EXPECT_EQ(r.has_value(), c.resized);
        expectStorages(m, c.afterResize);
        const auto out = m.result();
        ASSERT_EQ(out.size(), c.saved.size());
        for (std::size_t i = 0; i < out.size(); ++i) {
            SCOPED_TRACE(i);
            EXPECT_EQ(out[i].dia, c.saved[i].dia);
            ASSERT_EQ(out[i].drills.size() * 2, c.saved[i].drills.size());
            for (std::size_t k = 0; k < out[i].drills.size(); ++k) {
                EXPECT_NEAR(out[i].drills[k].x, c.saved[i].drills[2 * k], 1e-9);
                EXPECT_NEAR(out[i].drills[k].y, c.saved[i].drills[2 * k + 1], 1e-9);
            }
            ASSERT_EQ(out[i].slots.size() * 4, c.saved[i].slots.size());
            for (std::size_t k = 0; k < out[i].slots.size(); ++k) {
                EXPECT_NEAR(out[i].slots[k].first.x, c.saved[i].slots[4 * k], 1e-6);
                EXPECT_NEAR(out[i].slots[k].first.y, c.saved[i].slots[4 * k + 1], 1e-6);
                EXPECT_NEAR(out[i].slots[k].second.x, c.saved[i].slots[4 * k + 2], 1e-6);
                EXPECT_NEAR(out[i].slots[k].second.y, c.saved[i].slots[4 * k + 3], 1e-6);
            }
        }
    }
}

TEST(ExcEditor, DrawingShapesMatchTheOriginal) {
    for (const auto& s : golden_exc_editor::kShapes) {
        const std::string what = s.what;
        SCOPED_TRACE(what);
        ShapeList got;
        if (what == "slot" || what == "slotArray") {
            got = {excdraw::slot({1.25, -2.5}, 0.8, s.args[1], static_cast<char>(s.args[0]), s.args[2], 64,
                                 what == "slotArray")};
        } else if (what == "drill") {
            got = {excdraw::drill({1.25, -2.5}, 0.8)};
        } else if (what == "drillLinear") {
            for (const Point2D& p : excdraw::linearArray({1.0, 2.0}, 4, static_cast<char>(s.args[0]), 2.5, s.args[1]))
                got.push_back(excdraw::drill(p, 0.8));
        } else {
            const auto items = excdraw::circularArray({0, 0}, {-3, 4}, 5, 45.0, s.args[0] != 0);
            ASSERT_TRUE(items.has_value());
            for (const auto& it : *items) {
                if (what == "drillCircular") {
                    got.push_back(excdraw::drill(it.p, 0.8));
                } else {
                    const Shape sl = excdraw::slot(it.p, 0.8, 2.0, 'X', 0.0, 64, true);
                    got.push_back(sl.rotate(it.rotation * 180.0 / 3.14159265358979323846));
                }
            }
        }
        ASSERT_EQ(got.size(), s.wkt.size());
        for (std::size_t i = 0; i < got.size(); ++i)
            EXPECT_TRUE(got[i].equalsExact(Shape::fromWkt(s.wkt[i]), 1e-9)) << i << "\n" << got[i].toWkt() << "\n" << s.wkt[i];
    }
}
