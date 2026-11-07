// Importar DXF (DxfImport) contra Geometry.import_dxf_as_geo y Gerber.import_dxf_as_gerber originales (golden_dxf_import.h).
#include <gtest/gtest.h>

#include <cmath>
#include <fstream>
#include <sstream>

#include "golden_dxf_import.h"
#include "parsers/DxfImport.h"

using namespace flatcam;

namespace {

std::string readFile(const std::string& name) {
    std::ifstream in(std::string(FLATCAM_TEST_DATA_DIR) + "/dxf/" + name, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

}  // namespace

TEST(DxfImport, MatchesOriginal) {
    for (const auto& c : golden_dxf_import::kCases) {
        SCOPED_TRACE(std::string(c.file) + (c.gerber ? " (Gerber)" : " (Geometría)"));
        const std::string text = readFile(c.file);
        const DxfImportResult r = c.gerber ? importDxfAsGerber(text) : importDxfAsGeometry(text);
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


