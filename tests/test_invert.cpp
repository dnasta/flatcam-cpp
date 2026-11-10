// Invertir Gerber contra el original (tests/tools/oracle_invert.py -> golden_invert.h).
#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include "golden_invert.h"
#include "parsers/Gerber.h"
#include "tools/InvertGerber.h"

using namespace flatcam;

TEST(InvertGerber, MatchesTheOriginal) {
    for (const auto& c : golden_invert::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + std::to_string(c.margin) + " " + c.join);
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        const std::string j = c.join;
        const JoinStyle join = j == "r" ? JoinStyle::Round : j == "b" ? JoinStyle::Bevel : JoinStyle::Mitre;
        const ShapeList r = invertGerber(g.solidGeometry(), g.bounds(), c.margin, join);
        ASSERT_EQ(r.size(), c.solid.size());
        int exact = 0;
        for (std::size_t i = 0; i < r.size(); ++i) {
            // La misma figura, en el mismo lugar de la lista o, si GEOS devolvió las partes en otro orden, en otro.
            const Shape want = Shape::fromWkt(c.solid[i]);
            if (r[i].equalsExact(want, 1e-9)) {
                ++exact;
                continue;
            }
            bool found = false;
            for (const std::string& w : c.solid) {
                const Shape o = Shape::fromWkt(w);
                // Hasta media micra cuadrada de área: la resta da otros vértices según la versión de GEOS.
                if (std::abs(o.area() - r[i].area()) < 5e-7 && r[i].symmetricDifference(o).area() < 5e-7) found = true;
            }
            EXPECT_TRUE(found) << i << " " << r[i].area();
        }
        RecordProperty("exact", exact);
    }
}
