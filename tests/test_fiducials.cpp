// Fiduciales contra el original (tests/tools/oracle_fiducials.py -> golden_fiducials.h).
#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include "golden_fiducials.h"
#include "parsers/Gerber.h"
#include "tools/Fiducials.h"

using namespace flatcam;

TEST(Fiducials, MatchesTheOriginal) {
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr"));
    const BoundingBox b = g.bounds();
    for (const auto& c : golden_fiducials::kCases) {
        const std::string op = c.op, t = c.type;
        SCOPED_TRACE(op + " " + t + " " + c.second + " " + std::to_string(c.size) + " " + std::to_string(c.margin));
        std::vector<Point2D> pts;
        if (op == "geo") {
            for (std::size_t i = 0; i < c.manual.size(); i += 2) pts.push_back({c.manual[i], c.manual[i + 1]});
        } else {
            pts = fiducialAutoPoints(b, c.margin, c.second);
        }
        ASSERT_EQ(pts.size() * 2, c.points.size());
        for (std::size_t i = 0; i < pts.size(); ++i) {
            EXPECT_EQ(pts[i].x, c.points[2 * i]);
            EXPECT_EQ(pts[i].y, c.points[2 * i + 1]);
        }
        const FiducialType type = t == "circular" ? FiducialType::Circular
                                  : t == "cross"  ? FiducialType::Cross
                                                  : FiducialType::Chess;
        // La máscara lleva aberturas circulares del doble del tamaño.
        const auto r = op == "sm" ? addFiducials(g, pts, c.size * 2.0, FiducialType::Circular, c.line)
                                  : addFiducials(g, pts, c.size, type, c.line);
        ASSERT_EQ(r.has_value(), c.ok);
        if (!r) continue;
        ASSERT_EQ(static_cast<int>(r->solid.size()), c.polygons);
        double area = 0.0;
        for (const Shape& s : r->solid) area += s.area();
        EXPECT_NEAR(area, c.area, 1e-6);
        ASSERT_EQ(r->apertures.size(), c.apertures.size());
        for (const auto& a : c.apertures) {
            SCOPED_TRACE(a.id);
            const auto it = r->apertures.find(a.id);
            ASSERT_NE(it, r->apertures.end());
            const Aperture& ap = it->second;
            EXPECT_EQ(ap.type, a.type);
            EXPECT_EQ(ap.size.value_or(-1.0), a.size);
            if (a.width >= 0) EXPECT_EQ(ap.width, a.width);
            if (a.height >= 0) EXPECT_EQ(ap.height, a.height);
            ASSERT_EQ(static_cast<int>(ap.geometry.size()), a.count);
            // Los elementos agregados, vértice por vértice.
            const std::size_t first = ap.geometry.size() - a.solid.size();
            for (std::size_t i = 0; i < a.solid.size(); ++i) {
                EXPECT_TRUE(ap.geometry[first + i].solid->equalsExact(Shape::fromWkt(a.solid[i]), 1e-9)) << i;
                EXPECT_TRUE(ap.geometry[first + i].follow->equalsExact(Shape::fromWkt(a.follow[i]), 1e-9)) << i;
            }
        }
        const std::size_t base = g.followGeometry().size();
        ASSERT_EQ(r->follow.size() - base, c.followAdded.size());
        for (std::size_t i = 0; i < c.followAdded.size(); ++i)
            EXPECT_TRUE(r->follow[base + i].equalsExact(Shape::fromWkt(c.followAdded[i]), 1e-9)) << i;
    }
}
