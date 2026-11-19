// Marcas de esquina contra el original (tests/tools/oracle_corners.py -> golden_corners.h).
#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include "golden_corners.h"
#include "parsers/Gerber.h"
#include "tools/CornerMarkers.h"

using namespace flatcam;

namespace {

void expectSameShape(const Shape& mine, const std::string& wkt, const std::string& what) {
    const Shape want = Shape::fromWkt(wkt);
    if (mine.equalsExact(want, 1e-9)) return;
    // La unión puede empezar los anillos en otro vértice según la GEOS: la misma figura.
    EXPECT_LT(mine.symmetricDifference(want).area(), 1e-9) << what;
    EXPECT_NEAR(mine.area(), want.area(), 1e-9) << what;
}

}  // namespace

TEST(CornerMarkers, MatchesTheOriginal) {
    for (const auto& c : golden_corners::kCases) {
        SCOPED_TRACE(std::string(c.file) + " " + c.op + " " + c.type + " " + std::to_string(c.thickness));
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.file));
        const CornerSelection sel{c.tl, c.tr, c.bl, c.br};
        if (std::string(c.op) == "drills") {
            const auto d = cornerDrills(g.bounds(), sel, c.thickness, c.margin);
            ASSERT_EQ(d.size(), c.drills.size());
            for (std::size_t i = 0; i < d.size(); ++i) expectSameShape(Shape::point(d[i]), c.drills[i], "drill");
            continue;
        }
        const ShapeList lines = cornerLines(g.bounds(), sel, std::string(c.type) == "c", c.thickness, c.length, c.margin);
        EXPECT_EQ(!lines.empty(), c.ok);
        if (!c.ok) continue;
        const CornersGerber r = addCornerMarkers(g, lines, c.thickness);
        ASSERT_EQ(lines.size(), c.followAdded.size());
        for (std::size_t i = 0; i < lines.size(); ++i) EXPECT_TRUE(lines[i].equalsExact(Shape::fromWkt(c.followAdded[i]), 1e-12));
        ASSERT_EQ(r.solid.size(), c.solid.size());
        for (std::size_t i = 0; i < r.solid.size(); ++i) {
            // Mismo conjunto de polígonos, aunque la unión los devuelva en otro orden.
            bool found = false;
            for (const std::string& w : c.solid)
                if (r.solid[i].symmetricDifference(Shape::fromWkt(w)).area() < 1e-9) found = true;
            EXPECT_TRUE(found) << i;
        }
        ASSERT_EQ(r.apertures.size(), c.apertures.size());
        for (const auto& want : c.apertures) {
            ASSERT_TRUE(r.apertures.count(want.id)) << want.id;
            const Aperture& ap = r.apertures.at(want.id);
            EXPECT_EQ(ap.type, want.type);
            EXPECT_DOUBLE_EQ(ap.size.value_or(0.0), want.size);
            ASSERT_EQ(ap.geometry.size(), want.solid.size()) << want.id;
            for (std::size_t i = 0; i < want.solid.size(); ++i)
                expectSameShape(*ap.geometry[i].solid, want.solid[i], "ap " + std::string(want.id));
        }
    }
}

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

TEST(CornerMarkersFixed, NewApertureIdAndMitreJoins) {
    std::map<std::string, Aperture> aps;
    aps["99"] = Aperture{};
    aps["100"] = Aperture{};
    {
        OriginalBugsScope o(true);
        EXPECT_EQ(Gerber::newApertureId(aps), "100");  // "99" > "100" como texto: repite una que ya existe
    }
    OriginalBugsScope fixed(false);
    EXPECT_EQ(Gerber::newApertureId(aps), "101");
    EXPECT_EQ(Gerber::newApertureId({}), "10");
    std::map<std::string, Aperture> low;
    low["0"] = Aperture{};
    EXPECT_EQ(Gerber::newApertureId(low), "10");
    // Una apertura nueva: las marcas con ingletes (el original, con biseles).
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"));
    const ShapeList lines = cornerLines(g.bounds(), CornerSelection{true, false, false, false}, false, 0.37, 3, 1);
    const CornersGerber r = addCornerMarkers(g, lines, 0.37);
    double mine = 0, want = 0;
    for (const auto& [id, ap] : r.apertures)
        if (ap.size && std::abs(*ap.size - 0.37) < 1e-12)
            for (const auto& el : ap.geometry) mine += el.solid->area();
    for (const Shape& l : lines) want += l.buffer(0.37 / 2, 64, JoinStyle::Mitre).area();
    EXPECT_NEAR(mine, want, 1e-12);
}
