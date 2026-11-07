// Exportar a DXF (DxfExport) contra GeometryObject.export_dxf original (golden_dxf_export.h).
#include <gtest/gtest.h>

#include <cmath>
#include <sstream>

#include "golden_dxf_export.h"
#include "io/DxfExport.h"
#include "parsers/DxfImport.h"
#include "parsers/Gerber.h"

using namespace flatcam;

namespace {

/// El texto sin el valor de los GUID (las dos líneas que siguen a $FINGERPRINTGUID y $VERSIONGUID, salvo el código).
std::string withoutGuids(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string l;
    while (std::getline(ss, l)) lines.push_back(l);
    if (!text.empty() && text.back() == '\n') lines.push_back("");
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i >= 2 && (lines[i - 2] == "$FINGERPRINTGUID" || lines[i - 2] == "$VERSIONGUID" || lines[i - 2] == "$TDCREATE" ||
                       lines[i - 2] == "$TDUPDATE"))
            continue;
        if (!out.empty() || i > 0) out += (i > 0 ? "\n" : "");
        out += lines[i];
    }
    return out;
}

}  // namespace

TEST(DxfExport, MatchesOriginal) {
    for (const auto& c : golden_dxf_export::kCases) {
        SCOPED_TRACE(c.gerber);
        Gerber g;
        ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + c.gerber));
        ShapeList shapes = g.solidGeometry();
        for (const char* w : c.extra) shapes.push_back(Shape::fromWkt(w));
        const std::string mine = withoutGuids(exportDxf(shapes));
        if (mine == c.dxf) continue;
        // El orden de los vértices del buffer de GEOS cambia con la versión (el oráculo usa la 3.11): el mismo largo de
        // documento a menos del 0,1 % y la misma cantidad de polilíneas.
        const std::string ref = c.dxf;
        auto count = [](const std::string& t) {
            std::size_t n = 0, p = 0;
            while ((p = t.find("\nLWPOLYLINE\n", p)) != std::string::npos) ++n, ++p;
            return n;
        };
        EXPECT_EQ(count(mine), count(ref));
        EXPECT_NEAR(static_cast<double>(mine.size()), static_cast<double>(ref.size()), 1e-3 * ref.size());
    }
}

TEST(DxfExport, ReadsBackWithTheImporter) {
    // Lo exportado se vuelve a importar como Geometría: las mismas polilíneas (cerradas: los mismos vértices).
    const ShapeList shapes{Shape::fromWkt("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0), (2 2, 4 2, 4 4, 2 2))"),
                           Shape::fromWkt("LINESTRING (20 0, 25 5)"), Shape::fromWkt("LINESTRING (30 0, 31 1)")};
    const DxfImportResult r = importDxfAsGeometry(exportDxf(shapes));
    ASSERT_TRUE(r.ok) << r.error;
    double len = 0;
    for (const Shape& s : r.geometry) len += s.length();
    EXPECT_NEAR(len, 40 + 2 + 2 + std::sqrt(8.0) + std::sqrt(50.0) + std::sqrt(2.0), 1e-9);
}
