// Perforar Gerber contra el original (tests/tools/oracle_punch.py -> golden_punch.h).
#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <string>

#include "golden_punch.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"
#include "tools/PunchGerber.h"

using namespace flatcam;

TEST(PunchGerber, MatchesTheOriginal) {
    std::map<std::string, Gerber> gerbers;
    std::map<std::string, Excellon> excellons;
    const std::string data = FLATCAM_TEST_DATA_DIR;
    for (const auto& c : golden_punch::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + c.method + " " + std::to_string(c.dia));
        if (!gerbers.count(c.file)) ASSERT_TRUE(gerbers[c.file].parseFile(data + "/" + c.file));
        const Gerber& g = gerbers.at(c.file);
        const Excellon* exc = nullptr;
        if (*c.exc) {
            if (!excellons.count(c.exc)) {
                ASSERT_TRUE(excellons[c.exc].parseFile(data + "/" + c.exc));
                excellons[c.exc].createGeometry();
            }
            exc = &excellons.at(c.exc);
        }
        std::set<std::string> sel;
        if (c.allSelected)
            for (const auto& [id, ap] : g.apertures()) sel.insert(id);
        else
            sel.insert(c.sel.begin(), c.sel.end());
        PunchSettings s;
        const std::string m = c.method;
        s.method = m == "exc" ? PunchSettings::Method::Excellon : m == "fixed" ? PunchSettings::Method::Fixed
                 : m == "ring" ? PunchSettings::Method::Ring : PunchSettings::Method::Proportional;
        s.fixedDia = c.dia;
        s.ringCircular = 0.2;
        s.ringOblong = 0.3;
        s.ringSquare = 0.25;
        s.ringRectangular = 0.35;
        s.ringOther = 0.2;
        s.factor = c.factor;
        s.circular = c.filters[0];
        s.oblong = c.filters[1];
        s.square = c.filters[2];
        s.rectangular = c.filters[3];
        s.other = c.filters[4];
        const PunchResult r = punchGerber(g, sel, s, exc);
        ASSERT_EQ(r.ok(), c.ok) << c.message;
        const std::string msg = c.message;
        if (msg.find("EXCEPTION") != std::string::npos) EXPECT_EQ(r.error, PunchResult::Error::OriginalFails);
        if (msg.find("bigger") != std::string::npos) EXPECT_EQ(r.error, PunchResult::Error::HoleTooBig);
        if (msg.find("same") != std::string::npos) EXPECT_EQ(r.error, PunchResult::Error::SameGeometry);
        if (msg.find("0.0. Aborting") != std::string::npos) EXPECT_EQ(r.error, PunchResult::Error::ZeroDiameter);
        if (!r.ok()) continue;
        // Los polígonos: la misma cantidad y las mismas áreas (la resta los puede devolver en otro orden o con otros
        // vértices iniciales según la GEOS).
        ASSERT_EQ(static_cast<int>(r.solid.size()), c.polygons);
        double area = 0;
        std::vector<double> areas;
        for (const Shape& p : r.solid) {
            area += p.area();
            areas.push_back(p.area());
        }
        EXPECT_NEAR(area, c.area, 1e-6);
        std::sort(areas.begin(), areas.end());
        for (std::size_t i = 0; i < areas.size(); ++i) EXPECT_NEAR(areas[i], c.areas[i], 1e-6) << i;
        // Las aperturas nuevas: los mismos números, diámetros y agujeros.
        ASSERT_EQ(r.newApertures.size(), c.newAps.size());
        for (std::size_t i = 0; i < c.newAps.size(); ++i) {
            const auto& want = c.newAps[i];
            EXPECT_EQ(r.newApertures[i], want.id);
            const Aperture& ap = r.apertures.at(r.newApertures[i]);
            EXPECT_EQ(ap.type, "C");
            EXPECT_DOUBLE_EQ(ap.size.value_or(-1), want.size);
            ASSERT_EQ(ap.geometry.size(), want.clear.size()) << want.id;
            for (std::size_t k = 0; k < want.clear.size(); ++k)
                EXPECT_TRUE(ap.geometry[k].clear->equalsExact(Shape::fromWkt(want.clear[k]), 1e-9)) << want.id << " " << k;
        }
    }
}
