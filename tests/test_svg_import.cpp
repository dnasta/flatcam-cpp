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

// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

namespace {
/// Un SVG de 100 x 100 mm con viewBox de 50 (factor 2) y el contenido dado.
std::string svgDoc(const std::string& body) {
    return "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100mm\" height=\"100mm\" viewBox=\"0 0 50 50\">" + body +
           "</svg>";
}
ShapeList geometryOf(const std::string& body) {
    const SvgImportResult r = importSvgAsGeometry(svgDoc(body), 64);
    EXPECT_TRUE(r.ok) << r.error;
    return r.geometry;
}
double totalArea(const ShapeList& l) {
    double a = 0;
    for (const Shape& s : l) a += s.area();
    return a;
}
}  // namespace

TEST(SvgImportFixed, ViewBoxFactorRectsMatrixPathsAndDefs) {
    OriginalBugsScope fixed(false);
    // Un trayecto de líneas: el inicio también con el factor (cuadrado de 10 -> 20 x 20).
    EXPECT_NEAR(totalArea(geometryOf("<path d=\"M 5 5 L 15 5 L 15 15 L 5 15 Z\"/>")), 400.0, 1e-6);
    // Un trayecto abierto es una línea.
    {
        const ShapeList g = geometryOf("<path d=\"M 0 0 L 10 0 L 10 10\"/>");
        ASSERT_EQ(g.size(), 1u);
        EXPECT_EQ(g[0].type(), ShapeType::LineString);
        EXPECT_NEAR(g[0].length(), 40.0, 1e-9);
    }
    // El rect con el ancho y el alto escalados; con esquinas redondeadas.
    EXPECT_NEAR(totalArea(geometryOf("<rect x=\"0\" y=\"0\" width=\"10\" height=\"5\"/>")), 200.0, 1e-6);
    {
        const double a = totalArea(geometryOf("<rect x=\"0\" y=\"0\" width=\"10\" height=\"10\" rx=\"2\"/>"));
        const double expected = 400.0 - (4 - 3.14159265358979) * 16;  // radio 4 tras el factor
        EXPECT_NEAR(a, expected, 0.2);
    }
    // matrix(a b c d e f): x' = a x + c y + e. Un inclinado en X de un cuadrado: el área no cambia y la X crece con la Y.
    {
        const ShapeList g = geometryOf("<rect x=\"0\" y=\"0\" width=\"10\" height=\"10\" transform=\"matrix(1 0 1 1 0 0)\"/>");
        ASSERT_EQ(g.size(), 1u);
        const auto b = g[0].bounds();
        ASSERT_TRUE(b);
        EXPECT_NEAR(b->xmax - b->xmin, 40.0, 1e-6);  // 20 de ancho más 20 de inclinación
        EXPECT_NEAR(b->ymax - b->ymin, 20.0, 1e-6);
    }
    // Una lista de puntos con espacios a los costados.
    EXPECT_NEAR(totalArea(geometryOf("<polygon points=\"  0,0 10,0 10,10 0,10  \"/>")), 400.0, 1e-6);
    // Lo de defs no se dibuja salvo que un use lo pida.
    EXPECT_TRUE(geometryOf("<defs><rect id=\"r\" x=\"0\" y=\"0\" width=\"10\" height=\"10\"/></defs>").empty());
    EXPECT_NEAR(totalArea(geometryOf("<defs><rect id=\"r\" x=\"0\" y=\"0\" width=\"10\" height=\"10\"/></defs>"
                                     "<use href=\"#r\"/>")),
                400.0, 1e-6);
    // Un texto sin style ya no hace fallar la importación.
    EXPECT_EQ(geometryOf("<text><tspan>hola</tspan></text><rect x=\"0\" y=\"0\" width=\"10\" height=\"10\"/>").size(), 1u);
    // Como Gerber: una figura con agujero lo conserva (el área es la del anillo) y sin el Polygon() vacío al final.
    const SvgImportResult grb = importSvgAsGerber(
        svgDoc("<path d=\"M 0 0 L 20 0 L 20 20 L 0 20 Z M 5 5 L 15 5 L 15 15 L 5 15 Z\"/><line x1=\"0\" y1=\"0\" x2=\"5\" y2=\"5\"/>"),
        64);
    ASSERT_TRUE(grb.ok) << grb.error;
    ASSERT_EQ(grb.geometry.size(), 1u);
    EXPECT_NEAR(grb.geometry[0].area(), 1600.0 - 400.0, 1e-6);
}
