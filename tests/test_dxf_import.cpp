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



// ---------------------------------------------------------------- errores del original corregidos (sección 14)
#include "core/OriginalBugs.h"

namespace {
/// Un DXF mínimo con sus bloques y entidades, escritos como pares código / valor.
std::string dxf(const std::string& blocks, const std::string& entities) {
    std::string s;
    if (!blocks.empty()) s += "0\nSECTION\n2\nBLOCKS\n" + blocks + "0\nENDSEC\n";
    return s + "0\nSECTION\n2\nENTITIES\n" + entities + "0\nENDSEC\n0\nEOF\n";
}
double areaOf(const ShapeList& l) {
    double a = 0;
    for (const Shape& s : l) a += s.area();
    return a;
}
double lengthOf(const ShapeList& l) {
    double a = 0;
    for (const Shape& s : l) a += s.length();
    return a;
}
}  // namespace

TEST(DxfImportFixed, BulgesSolidsSplinesInsertsAndSingleLines) {
    const std::string line = "0\nLINE\n8\n0\n10\n0\n20\n0\n30\n0\n11\n10\n21\n0\n31\n0\n";
    {
        OriginalBugsScope o(true);
        EXPECT_FALSE(importDxfAsGeometry(dxf("", line)).ok);  // una sola línea: el original falla
    }
    OriginalBugsScope fixed(false);
    // Una sola línea.
    {
        const DxfImportResult r = importDxfAsGeometry(dxf("", line));
        ASSERT_TRUE(r.ok) << r.error;
        EXPECT_NEAR(lengthOf(r.geometry), 10.0, 1e-9);
    }
    // Una polilínea cerrada de dos vértices con curva 1 en cada uno: un círculo de radio 5 (largo 10 pi).
    {
        const std::string pl = "0\nLWPOLYLINE\n8\n0\n90\n2\n70\n1\n10\n0\n20\n0\n42\n1\n10\n10\n20\n0\n42\n1\n";
        const DxfImportResult r = importDxfAsGeometry(dxf("", pl));
        ASSERT_TRUE(r.ok) << r.error;
        EXPECT_NEAR(lengthOf(r.geometry), 10 * 3.14159265358979, 0.05);
        // Como Gerber, una polilínea cerrada es una región con área (antes quedaba una línea sin área).
        const DxfImportResult g = importDxfAsGerber(dxf("", pl));
        ASSERT_TRUE(g.ok) << g.error;
        EXPECT_NEAR(areaOf(g.geometry), 25 * 3.14159265358979, 0.5);
    }
    // Un SOLID: el cuadrado relleno (los puntos 1, 2, 4, 3).
    {
        const std::string so = "0\nSOLID\n8\n0\n10\n0\n20\n0\n11\n4\n21\n0\n12\n0\n22\n4\n13\n4\n23\n4\n";
        const DxfImportResult r = importDxfAsGeometry(dxf("", so + line));
        ASSERT_TRUE(r.ok) << r.error;
        EXPECT_NEAR(areaOf(r.geometry), 16.0, 1e-9);
        // Como Gerber, el polígono se conserva (el original falla) y sin el polígono vacío inicial.
        const DxfImportResult g = importDxfAsGerber(dxf("", so + line));
        ASSERT_TRUE(g.ok) << g.error;
        EXPECT_NEAR(areaOf(g.geometry), 16.0, 1e-9);
        for (const Shape& s : g.geometry) EXPECT_FALSE(s.isEmpty());
    }
    // Un spline por puntos de ajuste.
    {
        const std::string sp = "0\nSPLINE\n8\n0\n70\n8\n71\n3\n74\n3\n11\n0\n21\n0\n11\n5\n21\n5\n11\n10\n21\n0\n";
        const DxfImportResult r = importDxfAsGeometry(dxf("", sp));
        ASSERT_TRUE(r.ok) << r.error;
        EXPECT_GT(lengthOf(r.geometry), 14.0);
    }
    // Una inserción escalada por 2 y girada 90 grados, en una grilla de 2 columnas separadas 100.
    {
        const std::string block = "0\nBLOCK\n8\n0\n2\nB\n70\n0\n10\n0\n20\n0\n30\n0\n" + line + "0\nENDBLK\n";
        const std::string ins = "0\nINSERT\n8\n0\n2\nB\n10\n50\n20\n50\n30\n0\n41\n2\n42\n2\n50\n90\n70\n2\n44\n100\n";
        const DxfImportResult r = importDxfAsGeometry(dxf(block, ins));
        ASSERT_TRUE(r.ok) << r.error;
        ASSERT_EQ(r.geometry.size(), 2u);
        // La línea de (0,0)-(10,0), escalada a 20 y girada: de (50,50) a (50,70); la segunda columna, girada: x 50, y 150.
        double ymin = 1e9, ymax = -1e9;
        for (const Shape& s : r.geometry) {
            EXPECT_NEAR(s.length(), 20.0, 1e-9);
            const auto b = s.bounds();
            ASSERT_TRUE(b);
            EXPECT_NEAR(b->xmin, 50.0, 1e-9);
            ymin = std::min(ymin, b->ymin);
            ymax = std::max(ymax, b->ymax);
        }
        EXPECT_NEAR(ymin, 50.0, 1e-9);
        EXPECT_NEAR(ymax, 170.0, 1e-9);
    }
}

TEST(DxfImportFixed, ArcWithEqualAnglesAndRotatedEllipses) {
    OriginalBugsScope fixed(false);
    // ARC con el mismo ángulo inicial y final: la circunferencia (el original no terminaba nunca).
    const std::string arc = "0\nARC\n8\n0\n10\n0\n20\n0\n30\n0\n40\n1\n50\n10\n51\n10\n";
    const DxfImportResult a = importDxfAsGeometry(dxf("", arc));
    ASSERT_TRUE(a.ok) << a.error;
    EXPECT_NEAR(lengthOf(a.geometry), 2 * 3.14159265358979, 0.01);
    // ELLIPSE con el eje mayor (1, 1) y razón 0,5: una elipse de semiejes sqrt(2) y sqrt(2)/2, no una recta.
    const std::string el = "0\nELLIPSE\n8\n0\n10\n0\n20\n0\n30\n0\n11\n1\n21\n1\n31\n0\n40\n0.5\n41\n0\n42\n6.283185307179586\n";
    const DxfImportResult e = importDxfAsGeometry(dxf("", el));
    ASSERT_TRUE(e.ok) << e.error;
    double maxOff = 0;  // distancia a la recta x = y
    for (const Shape& s : e.geometry)
        for (const Point2D& q : s.coordinates()) maxOff = std::max(maxOff, std::abs(q.x - q.y) / std::sqrt(2.0));
    EXPECT_NEAR(maxOff, std::sqrt(2.0) / 2, 1e-3);
    // Con el eje mayor en Y y medio recorrido (0 a pi), la mitad del lado de las X negativas, como en DXF.
    const std::string half = "0\nELLIPSE\n8\n0\n10\n0\n20\n0\n30\n0\n11\n0\n21\n2\n31\n0\n40\n0.5\n41\n0\n42\n3.141592653589793\n";
    const DxfImportResult h = importDxfAsGeometry(dxf("", half));
    ASSERT_TRUE(h.ok) << h.error;
    double minX = 1e9;
    for (const Shape& s : h.geometry)
        for (const Point2D& q : s.coordinates()) minX = std::min(minX, q.x);
    EXPECT_NEAR(minX, -1.0, 1e-3);
    // Un parámetro final enorme no cuelga.
    const std::string big = "0\nELLIPSE\n8\n0\n10\n0\n20\n0\n30\n0\n11\n1\n21\n0\n31\n0\n40\n0.5\n41\n0\n42\n1e12\n";
    EXPECT_TRUE(importDxfAsGeometry(dxf("", big)).ok);
}
