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
    // Un trayecto abierto sin relleno es una línea; con relleno (el de fábrica de SVG es negro) se rellena cerrado.
    {
        const ShapeList g = geometryOf("<path fill=\"none\" d=\"M 0 0 L 10 0 L 10 10\"/>");
        ASSERT_EQ(g.size(), 1u);
        EXPECT_EQ(g[0].type(), ShapeType::LineString);
        EXPECT_NEAR(g[0].length(), 40.0, 1e-9);
        EXPECT_NEAR(totalArea(geometryOf("<path d=\"M 0 0 L 10 0 L 10 10\"/>")), 200.0, 1e-6);
        // El relleno se hereda del grupo (atributo o style) y el elemento lo puede volver a pedir.
        const ShapeList h = geometryOf("<g style=\"fill:none\"><path d=\"M 0 0 L 10 0 L 10 10 M 20 0 L 30 0\"/></g>");
        ASSERT_EQ(h.size(), 2u);
        EXPECT_EQ(h[1].type(), ShapeType::LineString);
        EXPECT_NEAR(totalArea(geometryOf("<g fill=\"none\"><path fill=\"red\" d=\"M 0 0 L 10 0 L 10 10\"/></g>")),
                    200.0, 1e-6);
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

#include "parsers/FontParser.h"

TEST(SvgImportFixed, TextsAreWrittenWithTheirFont) {
    if (!FontParser::available()) GTEST_SKIP() << "sin FreeType";
    FontParser fonts;
    fonts.loadFonts(FontParser::fontFiles({std::string(FLATCAM_TEST_DATA_DIR) + "/fonts"}));
    const std::string body =
        "<text x=\"5\" y=\"20\" style=\"font-family:'DejaVu Sans';font-size:10px\">Ho<tspan>la</tspan></text>"
        "<text x=\"5\" y=\"40\" font-family=\"No Existe\">x</text>";
    {
        OriginalBugsScope o(true);
        const SvgImportResult r = importSvgAsGeometry(svgDoc(body), 64, &fonts);
        ASSERT_TRUE(r.ok);
        EXPECT_TRUE(r.geometry.empty());  // el original no importa los textos
    }
    OriginalBugsScope fixed(false);
    const SvgImportResult r = importSvgAsGeometry(svgDoc(body), 64, &fonts);
    ASSERT_TRUE(r.ok) << r.error;
    EXPECT_EQ(r.textsSkipped, 1);  // la fuente que no está
    ASSERT_FALSE(r.geometry.empty());
    // Factor 2: el texto empieza en x = 10 mm, con la línea de base en y = 100 - 40 = 60 mm y el em de 20 mm.
    double xmin = 1e9, ymin = 1e9, ymax = -1e9;
    for (const Shape& s : r.geometry)
        if (auto b = s.bounds()) {
            xmin = std::min(xmin, b->xmin);
            ymin = std::min(ymin, b->ymin);
            ymax = std::max(ymax, b->ymax);
        }
    EXPECT_NEAR(xmin, 10.0, 2.0);
    EXPECT_NEAR(ymin, 60.0, 1.0);          // "Hola" no baja de la línea de base
    EXPECT_GT(ymax - ymin, 10.0);          // la "H" y la "l": casi tres cuartos del em
    EXPECT_LT(ymax - ymin, 20.0);
    // "o" y "a" con su agujero: algún polígono con interiores.
    bool hole = false;
    for (const Shape& s : r.geometry) hole = hole || !s.interiors().empty();
    EXPECT_TRUE(hole);
}

TEST(SvgImportFixed, TransformsViewBoxOriginAndPhysicalUnits) {
    auto bounds = [](const std::string& svg) {
        const SvgImportResult r = importSvgAsGeometry(svg);
        EXPECT_TRUE(r.ok) << r.error;
        BoundingBox b{1e9, 1e9, -1e9, -1e9};
        for (const Shape& s : r.geometry)
            if (auto sb = s.bounds()) b = b.merged(*sb);
        return b;
    };
    const std::string rect = "<rect x=\"0\" y=\"0\" width=\"5\" height=\"5\"/>";
    // viewBox 0 0 50 50 en 100 mm: factor 2. translate(10, 0) son 20 mm.
    const std::string tr = "<svg width=\"100mm\" height=\"100mm\" viewBox=\"0 0 50 50\"><g transform=\"translate(10,0)\">" +
                           rect + "</g></svg>";
    {
        OriginalBugsScope scope(true);
        EXPECT_NEAR(bounds(tr).xmin, 10.0, 1e-9);
    }
    OriginalBugsScope scope(false);
    EXPECT_NEAR(bounds(tr).xmin, 20.0, 1e-9);
    // matrix(1 0 0 1 10 0) igual; rotate(90, 5, 5) alrededor del centro del cuadrado lo deja en su lugar.
    EXPECT_NEAR(bounds("<svg width=\"100mm\" height=\"100mm\" viewBox=\"0 0 50 50\"><g transform=\"matrix(1 0 0 1 10 0)\">" +
                       rect + "</g></svg>").xmin, 20.0, 1e-9);
    const BoundingBox rot = bounds("<svg width=\"100mm\" height=\"100mm\" viewBox=\"0 0 50 50\"><g "
                                   "transform=\"rotate(90 2.5 2.5)\">" + rect + "</g></svg>");
    EXPECT_NEAR(rot.xmin, 0.0, 1e-9);
    EXPECT_NEAR(rot.xmax, 10.0, 1e-9);
    // El origen del viewBox (con comas) queda en (0, 0).
    const BoundingBox o = bounds("<svg width=\"100mm\" height=\"100mm\" viewBox=\"10,10,50,50\">"
                                 "<rect x=\"10\" y=\"55\" width=\"5\" height=\"5\"/></svg>");
    EXPECT_NEAR(o.xmin, 0.0, 1e-9);
    EXPECT_NEAR(o.ymin, 0.0, 1e-9);
    // Unidades físicas: 1 pulgada de ancho son 25,4 mm.
    const std::string in = "<svg width=\"1in\" height=\"1in\" viewBox=\"0 0 10 10\"><rect x=\"0\" y=\"0\" width=\"10\" "
                           "height=\"10\"/></svg>";
    const SvgImportResult r = importSvgAsGeometry(in);
    EXPECT_TRUE(r.millimeters);
    EXPECT_NEAR(bounds(in).xmax, 25.4, 1e-9);
    EXPECT_NEAR(bounds(in).ymax, 25.4, 1e-9);
    // Sin unidades, como antes.
    EXPECT_FALSE(importSvgAsGeometry("<svg width=\"10\" height=\"10\">" + rect + "</svg>").millimeters);
}

TEST(SvgImportFixed, FilledSubpathsUseTheEvenOddRule) {
    OriginalBugsScope scope(false);
    // Dos cuadrados separados (abiertos): dos polígonos de 20 x 20 con el factor 2.
    EXPECT_NEAR(totalArea(geometryOf("<path d=\"M 0 0 L 10 0 L 10 10 L 0 10 M 20 0 L 30 0 L 30 10 L 20 10\"/>")),
                800.0, 1e-6);
    // Uno dentro del otro: un agujero.
    EXPECT_NEAR(totalArea(geometryOf("<path d=\"M 0 0 L 20 0 L 20 20 L 0 20 Z M 5 5 L 15 5 L 15 15 L 5 15 Z\"/>")),
                4 * (400.0 - 100.0), 1e-6);
}
