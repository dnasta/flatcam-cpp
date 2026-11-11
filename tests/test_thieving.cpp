// Robo de cobre contra el original (tests/tools/oracle_thieving.py -> golden_thieving.h): las operaciones se encadenan
// como en la herramienta original (el relleno, la barra y la lista aplanada pasan de una a la siguiente).
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <string>

#include "golden_thieving.h"
#include "parsers/Gerber.h"
#include "tools/CopperThieving.h"

using namespace flatcam;

namespace {

std::string data(const std::string& f) { return std::string(FLATCAM_TEST_DATA_DIR) + "/" + f; }

int vertices(const Shape& s) {
    if (s.isEmpty()) return 0;
    const ShapeType t = s.type();
    if (t == ShapeType::MultiPolygon || t == ShapeType::GeometryCollection || t == ShapeType::MultiLineString) {
        int n = 0;
        for (const Shape& p : s.parts()) n += vertices(p);
        return n;
    }
    if (t == ShapeType::Polygon) {
        int n = static_cast<int>(s.exterior().coordinates().size());
        for (const Shape& i : s.interiors()) n += static_cast<int>(i.coordinates().size());
        return n;
    }
    return static_cast<int>(s.coordinates().size());
}

/// Error relativo (absoluto si el valor es chico).
double relErr(double a, double b) { return std::abs(a - b) / std::max(1.0, std::abs(b)); }

/// GEOS 3.11 es la del oráculo: con ella todo coincide exacto.
bool sameGeosAsOracle() { return Shape::geosVersion().rfind("3.11.", 0) == 0; }

/// Con GEOS 3.12 o más nueva cambian los vértices de los engrosados de 16 segmentos (hasta 2 %) y, cuando la
/// geometría sólida sale de engrosar un MultiPolygon inválido (las líneas superpuestas del relleno con líneas), cuántos
/// polígonos se unen: ahí solo se comparan las aperturas y lo agregado.
struct Tol {
    double verts;
    int polygons;
    bool solid;
};

Tol tolerance(bool invalidInput) {
    if (sameGeosAsOracle()) return {0.0, 0, true};
    return {0.025, 1, !invalidInput};
}

void expectGerber(const golden_thieving::Op& op, const ShapeList& solid, const std::map<std::string, Aperture>& aps,
                  const std::vector<std::string>& order, std::size_t follow, double tolArea, const Tol& tol) {
    const double tolVerts = tol.verts;
    if (tol.solid) {
        ASSERT_NEAR(static_cast<int>(solid.size()), op.polygons, tol.polygons);
        double area = 0.0;
        int verts = 0;
        for (const Shape& s : solid) {
            area += s.area();
            verts += vertices(s);
        }
        EXPECT_LT(relErr(area, op.area), tolArea);
        EXPECT_NEAR(verts, op.vertices, op.vertices * tolVerts);
    }
    const BoundingBox b = Shape::collection(solid).bounds().value_or(BoundingBox{});
    ASSERT_EQ(op.bounds.size(), 4u);
    EXPECT_NEAR(b.xmin, op.bounds[0], 1e-6);
    EXPECT_NEAR(b.ymin, op.bounds[1], 1e-6);
    EXPECT_NEAR(b.xmax, op.bounds[2], 1e-6);
    EXPECT_NEAR(b.ymax, op.bounds[3], 1e-6);
    ASSERT_EQ(order.size(), op.apertures.size());
    for (std::size_t i = 0; i < order.size(); ++i) {
        const auto& a = op.apertures[i];
        SCOPED_TRACE(a.id);
        EXPECT_EQ(order[i], a.id);
        const Aperture& ap = aps.at(order[i]);
        EXPECT_EQ(ap.type, a.type);
        EXPECT_EQ(ap.size.value_or(-1.0), a.size);
        ASSERT_EQ(static_cast<int>(ap.geometry.size()), a.count);
        double ar = 0.0;
        int v = 0;
        for (const ApertureGeometry& e : ap.geometry)
            if (e.solid) {
                ar += e.solid->area();
                v += vertices(*e.solid);
            }
        EXPECT_LT(relErr(ar, a.area), tolArea);
        EXPECT_NEAR(v, a.vertices, a.vertices * tolVerts);
    }
    EXPECT_EQ(static_cast<int>(follow), op.follow);
}

}  // namespace

TEST(CopperThieving, MatchesTheOriginal) {
    Gerber cu, logo, mask, edge;
    ASSERT_TRUE(cu.parseFile(data("luz_de_noche/luz de noche Pascual-B_Cu.gbr")));
    ASSERT_TRUE(logo.parseFile(data("luz_de_noche/luz de noche Pascual-Logo.gbr")));
    ASSERT_TRUE(edge.parseFile(data("luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr")));
    ASSERT_TRUE(mask.parseFile(data("PCB principal-B_Mask.gbs")));
    // El estado de la herramienta.
    ShapeList thieving, flat;
    std::optional<Shape> bar, line;
    double rbThickness = 0.0;
    int i = 0;
    for (const auto& op : golden_thieving::kOps) {
        const std::string kind = op.kind, src = op.src;
        SCOPED_TRACE(std::to_string(i++) + " " + kind + " " + src + " " + op.ref + " " + op.fill);
        const Gerber& g = src == "logo" ? logo : cu;
        if (kind == "thief") {
            ThievingParams p;
            p.clearance = op.clearance;
            p.margin = op.margin;
            p.minArea = op.minArea;
            const std::string ref = op.ref, fill = op.fill;
            p.reference = ref == "itself" ? ThievingReference::Itself
                          : ref == "area" ? ThievingReference::Area
                                          : ThievingReference::Box;
            p.minimalBox = std::string(op.bbox) == "min";
            p.fill = fill == "solid" ? ThievingFill::Solid
                     : fill == "dot" ? ThievingFill::Dot
                     : fill == "square" ? ThievingFill::Square
                                        : ThievingFill::Line;
            p.dotDia = op.dotDia;
            p.dotSpacing = op.dotSpacing;
            p.squareSize = op.squareSize;
            p.squareSpacing = op.squareSpacing;
            p.lineSize = op.lineSize;
            p.lineSpacing = op.lineSpacing;
            ShapeList area;
            for (std::size_t k = 0; k + 3 < op.clicks.size(); k += 4) {
                const Point2D a{op.clicks[k], op.clicks[k + 1]}, b{op.clicks[k + 2], op.clicks[k + 3]};
                area.push_back(Shape::polygon({a, {b.x, a.y}, b, {a.x, b.y}}));
            }
            ThievingBoxReference box{op.refKind, std::string(op.refFile).find("Edge") != std::string::npos
                                                     ? edge.solidGeometry()
                                                     : logo.solidGeometry()};
            const ThievingResult r = copperThieving(g, p, area, box, flat);
            if (r.thieving) thieving = *r.thieving;
            ASSERT_EQ(r.ok, op.created);
            if (r.ok) {
                std::vector<std::string> order = g.apertureOrder();
                if (!g.apertures().count("0")) order.push_back("0");
                expectGerber(op, r.solid, r.apertures, order, g.followGeometry().size(), 1e-6,
                             tolerance(p.fill == ThievingFill::Line));
            }
        } else if (kind == "robber") {
            const RobberBarResult r = robberBar(g, op.rbMargin, op.rbThickness);
            bar = r.bar;
            line = r.line;
            rbThickness = op.rbThickness;
            std::vector<std::string> order = g.apertureOrder();
            if (!r.newAperture.empty()) order.push_back(r.newAperture);
            expectGerber(op, r.solid, r.apertures, order, r.follow.size(), 1e-9, tolerance(false));
        } else {
            const PlatingMaskResult r =
                platingMask(mask, op.ppmClearance, op.ppmChoice[0], thieving, bar, line, rbThickness);
            ASSERT_EQ(r.ok, op.created);
            if (!r.ok) continue;
            ASSERT_NE(op.platedArea, nullptr);
            EXPECT_LT(relErr(r.platedArea, std::stod(op.platedArea)), 1e-9);
            std::vector<std::string> order = mask.apertureOrder();
            order.insert(order.end(), r.added.begin(), r.added.end());
            expectGerber(op, r.solid, r.apertures, order, r.follow.size(), 1e-6, tolerance(false));
        }
        // Lo agregado al relleno, que usa la máscara.
        double ta = 0.0;
        int tv = 0;
        for (const Shape& t : thieving) {
            ta += t.area();
            tv += vertices(t);
        }
        EXPECT_EQ(static_cast<int>(thieving.size()), op.thiefCount);
        EXPECT_LT(relErr(ta, op.thiefArea), 1e-6);
        EXPECT_NEAR(tv, op.thiefVertices, op.thiefVertices * tolerance(false).verts);
    }
}
