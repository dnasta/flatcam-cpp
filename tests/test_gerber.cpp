#include <gtest/gtest.h>

#include <cmath>
#include <set>

#include "golden_gerber.h"
#include "parsers/Gerber.h"
#include "test_helpers.h"

using namespace flatcam;

namespace {

/// Lee un Gerber de `tests/data` con las opciones por defecto de FlatCAM.
Gerber parse(const std::string& file, bool* ok = nullptr) {
    Gerber g;
    const bool result = g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + file);
    if (ok) *ok = result;
    return g;
}

/// Todos los polígonos no vacíos de la geometría sólida (equivale al aplanado del oráculo).
ShapeList polygonsOf(const Gerber& g) {
    ShapeList out;
    for (const Shape& s : g.solidGeometry()) {
        for (const Shape& p : s.parts()) {
            if (p.type() == ShapeType::Polygon && !p.isEmpty()) out.push_back(p);
        }
    }
    return out;
}

}  // namespace

// ----------------------------------------------------------------------------- números y formato

TEST(GerberNumbers, LeadingZerosOmitted) {
    EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("-26000000", 4, 6, 'L'), -26.0);
    EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("718380", 4, 6, 'L'), 0.71838);
    EXPECT_DOUBLE_EQ(Gerber::parseGerberNumber("1500", 2, 3, 'D'), 1.5);
}

TEST(GerberNumbers, TrailingZerosOmitted) {
    // 2.4 con ceros finales omitidos: "15" significa 1.5000
    EXPECT_NEAR(Gerber::parseGerberNumber("15", 2, 4, 'T'), 15.0, 1e-12);
    EXPECT_NEAR(Gerber::parseGerberNumber("1500", 2, 4, 'T'), 15.0, 1e-12);
}

// ------------------------------------------------------------------------------- casos chicos

TEST(GerberSmall, CircleFlash) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10C,2.0*%", "D10*", "X10000Y20000D03*", "M02*"}));
    EXPECT_EQ(g.intDigits(), 2);
    EXPECT_EQ(g.fracDigits(), 4);
    EXPECT_EQ(g.units(), Units::Millimeters);
    const auto polys = polygonsOf(g);
    ASSERT_EQ(polys.size(), 1u);
    EXPECT_NEAR(polys[0].area(), 3.14159, 0.01);
    EXPECT_NEAR(polys[0].centroid().x, 1.0, 1e-6);
    EXPECT_NEAR(polys[0].centroid().y, 2.0, 1e-6);
}

TEST(GerberSmall, StrokeUsesApertureWidth) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10C,0.5*%", "D10*", "X0Y0D02*", "X100000Y0D01*", "M02*"}));
    const auto polys = polygonsOf(g);
    ASSERT_EQ(polys.size(), 1u);
    // Línea de 10 mm con ancho 0,5 y extremos redondeados: ~ 10*0.5 + pi*0.25^2
    EXPECT_NEAR(polys[0].area(), 10 * 0.5 + 3.14159 * 0.25 * 0.25, 0.01);
}

TEST(GerberSmall, RectangleAndRegion) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10R,2.0X1.0*%", "D10*", "X0Y0D03*",
                              "G36*", "X100000Y100000D02*", "X200000Y100000D01*", "X200000Y200000D01*",
                              "X100000Y200000D01*", "X100000Y100000D01*", "G37*", "M02*"}));
    const auto polys = polygonsOf(g);
    ASSERT_EQ(polys.size(), 2u);
    double total = 0;
    for (const Shape& p : polys) total += p.area();
    EXPECT_NEAR(total, 2.0 + 100.0, 1e-3);  // rectángulo 2x1 más región 10x10
}

TEST(GerberSmall, MultiQuadrantArc) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10C,0.1*%", "G75*", "D10*", "X100000Y0D02*",
                              "G03*", "X0Y100000I-100000J0D01*", "M02*"}));
    // Cuarto de circunferencia de radio 10: longitud ~ 15.7, con trazo de 0,1.
    ASSERT_EQ(g.followGeometry().size(), 1u);
    EXPECT_NEAR(g.followGeometry()[0].length(), 3.14159265 * 10 / 2, 0.01);
}

TEST(GerberSmall, ApertureMacroWithVariables) {
    Gerber g;
    // Macro: rectángulo centrado de ancho $1 y alto $1x0.5, más un círculo restado.
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%AMBOX*", "0 comentario*", "$2=$1x0.5*",
                              "21,1,$1,$2,0,0,0*", "1,0,0.5,0,0*%", "%ADD10BOX,4.0*%", "D10*", "X0Y0D03*", "M02*"}));
    const auto polys = polygonsOf(g);
    ASSERT_EQ(polys.size(), 1u);
    EXPECT_NEAR(polys[0].area(), 4.0 * 2.0 - 3.14159 * 0.25 * 0.25, 0.01);
    EXPECT_EQ(polys[0].interiors().size(), 1u);
    ASSERT_TRUE(g.apertureMacros().count("BOX"));
}

TEST(GerberSmall, ClearPolarityRemovesCopper) {
    Gerber g;
    ASSERT_TRUE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "%ADD10R,10.0X10.0*%", "%ADD11C,2.0*%", "%LPD*%", "D10*",
                              "X0Y0D03*", "%LPC*%", "D11*", "X0Y0D03*", "%LPD*%", "M02*"}));
    double total = 0;
    for (const Shape& p : polygonsOf(g)) total += p.area();
    EXPECT_NEAR(total, 100.0 - 3.14159, 0.05);
}

TEST(GerberSmall, EmptyFileFails) {
    Gerber g;
    EXPECT_FALSE(g.parseLines({"%FSLAX24Y24*%", "%MOMM*%", "M02*"}));
    EXPECT_FALSE(g.lastError().empty());
}

TEST(GerberSmall, MissingFileFails) {
    Gerber g;
    EXPECT_FALSE(g.parseFile("/no/existe.gbr"));
}

// ---------------------------------------------------------- archivos reales contra el parser original

class GerberRealFile : public ::testing::TestWithParam<std::size_t> {};

TEST_P(GerberRealFile, MatchesOriginalParser) {
    const auto& expected = golden_gerber::kFiles.at(GetParam());
    SCOPED_TRACE(expected.file);
    bool ok = false;
    const Gerber g = parse(expected.file, &ok);
    ASSERT_TRUE(ok) << g.lastError();

    // Formato leído del archivo
    EXPECT_EQ(g.units(), std::string(expected.units) == "MM" ? Units::Millimeters : Units::Inches);
    EXPECT_EQ(g.intDigits(), expected.intDigits);
    EXPECT_EQ(g.fracDigits(), expected.fracDigits);
    EXPECT_EQ(g.zeros(), expected.zeros);

    std::set<std::string> macros;
    for (const auto& kv : g.apertureMacros()) macros.insert(kv.first);
    EXPECT_EQ(macros, std::set<std::string>(expected.macros.begin(), expected.macros.end()));

    // Aperturas con geometría
    ASSERT_EQ(g.apertures().size(), expected.apertures.size());
    for (const auto& e : expected.apertures) {
        const auto it = g.apertures().find(e.id);
        ASSERT_NE(it, g.apertures().end()) << "apertura " << e.id;
        EXPECT_EQ(it->second.type, e.type) << "apertura " << e.id;
        EXPECT_EQ(static_cast<int>(it->second.geometry.size()), e.nGeometry) << "apertura " << e.id;
        if (it->second.size) expectRelNear(*it->second.size, e.size, 1e-12);
    }

    // Geometría sólida
    const ShapeList polys = polygonsOf(g);
    EXPECT_EQ(static_cast<int>(polys.size()), expected.nPolygons);
    const Shape all = Shape::unaryUnion(polys);
    expectRelNear(all.area(), expected.area, 1e-6);
    const auto b = all.bounds();
    ASSERT_TRUE(b.has_value());
    EXPECT_NEAR(b->xmin, expected.bounds[0], 1e-5);
    EXPECT_NEAR(b->ymin, expected.bounds[1], 1e-5);
    EXPECT_NEAR(b->xmax, expected.bounds[2], 1e-5);
    EXPECT_NEAR(b->ymax, expected.bounds[3], 1e-5);
    int holes = 0;
    for (const Shape& p : polys) holes += static_cast<int>(p.interiors().size());
    EXPECT_EQ(holes, expected.nHoles);

    // Geometría de seguimiento
    int lines = 0, points = 0;
    double length = 0;
    for (const Shape& f : g.followGeometry()) {
        if (f.type() == ShapeType::Point) ++points;
        else if (f.type() == ShapeType::LineString || f.type() == ShapeType::LinearRing) {
            ++lines;
            length += f.length();
        }
    }
    EXPECT_EQ(lines, expected.followLines);
    EXPECT_EQ(points, expected.followPoints);
    expectRelNear(length, expected.followLength, 1e-9);

    // Puntos de muestra: adentro y afuera, comparados con el parser original. Para ir rápido solo se prueba
    // cada punto contra los polígonos cuya caja lo contiene.
    std::vector<std::pair<BoundingBox, const Shape*>> boxes;
    for (const Shape& p : polys) boxes.push_back({*p.bounds(), &p});
    auto inside = [&](double x, double y) {
        const Shape pt = Shape::point({x, y});
        for (const auto& [box, poly] : boxes) {
            if (x >= box.xmin && x <= box.xmax && y >= box.ymin && y <= box.ymax && poly->contains(pt)) return true;
        }
        return false;
    };
    int wrong = 0;
    for (const auto& s : expected.inside) wrong += inside(s.x, s.y) ? 0 : 1;
    for (const auto& s : expected.outside) wrong += inside(s.x, s.y) ? 1 : 0;
    EXPECT_EQ(wrong, 0) << "puntos de muestra que difieren del parser original";
}

INSTANTIATE_TEST_SUITE_P(TestData, GerberRealFile, ::testing::Range<std::size_t>(0, golden_gerber::kFiles.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             std::string name = golden_gerber::kFiles.at(info.param).file;
                             for (char& c : name) if (!std::isalnum(static_cast<unsigned char>(c))) c = '_';
                             return name;
                         });
