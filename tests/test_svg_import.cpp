// Importar SVG (SvgImport) contra Geometry.import_svg y Gerber.import_svg originales (golden_svg_import.h).
#include <gtest/gtest.h>

#include <cmath>
#include <fstream>
#include <sstream>

#include "golden_svg_import.h"
#include "parsers/SvgImport.h"

using namespace flatcam;

namespace {

std::string readFile(const std::string& name) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/svg/" + name, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

}  // namespace

TEST(SvgImport, MatchesOriginal) {
    for (const auto& c : golden_svg_import::kCases) {
        SCOPED_TRACE(std::string(c.file) + (c.gerber ? " (Gerber)" : " (Geometría)"));
        const std::string text = readFile(c.file);
        const SvgImportResult r = c.gerber ? importSvgAsGerber(text) : importSvgAsGeometry(text);
        ASSERT_EQ(r.ok, c.ok) << r.error;
        if (!r.ok) continue;
        ASSERT_EQ(r.geometry.size(), c.wkt.size());
        for (std::size_t i = 0; i < c.wkt.size(); ++i) {
            SCOPED_TRACE(i);
            const Shape ref = Shape::fromWkt(c.wkt[i]);
            const Shape& mine = r.geometry[i];
            if (ref.isEmpty()) {  // el Polygon() vacío de Shapely 1.8 es un GEOMETRYCOLLECTION EMPTY
                EXPECT_TRUE(mine.isEmpty());
                continue;
            }
            EXPECT_EQ(mine.type(), ref.type());
            // Las curvas se muestrean igual: los mismos vértices, en el mismo orden (a menos del milmillonésimo).
            const auto a = mine.coordinates(), b = ref.coordinates();
            bool same = a.size() == b.size();
            for (std::size_t k = 0; same && k < a.size(); ++k)
                same = std::abs(a[k].x - b[k].x) < 1e-9 && std::abs(a[k].y - b[k].y) < 1e-9;
            if (same) continue;
            // Lo que pasó por buffer(0) puede empezar en otro vértice según la GEOS: la misma figura (micras).
            const Shape m0 = mine.buffer(0), r0 = ref.buffer(0);
            EXPECT_NEAR(m0.area(), r0.area(), 1e-9 * r0.area() + 1e-12);
            EXPECT_LT(m0.symmetricDifference(r0).area(), 1e-6 * r0.area() + 1e-12);
            EXPECT_EQ(mine.coordinates().size(), ref.coordinates().size());
        }
    }
}

TEST(SvgImport, PathParserCommands) {
    std::string err;
    // Comandos implícitos después de M, relativos, y un carácter no válido tomado como comentario.
    const auto p = parseSvgPath("M0 0 10 0 10 10 z m5 5 l1 0", err);
    ASSERT_TRUE(p) << err;
    EXPECT_EQ(p->size(), 6u);
    EXPECT_EQ((*p)[1].kind, SvgSegment::Kind::Line);
    EXPECT_EQ((*p)[5].end, (Point2D{6, 5}));
    EXPECT_FALSE(parseSvgPath("10 0 L5 5", err));  // no empieza con un comando
}
