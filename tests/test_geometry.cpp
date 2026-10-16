#include <gtest/gtest.h>

#include "core/Geometry.h"
#include "golden_data.h"
#include "test_helpers.h"

using namespace flatcam;

namespace {

/// Geometría de prueba: un rectángulo y un círculo (mismos datos que gen_golden.py).
Geometry makeGeometry() {
    Geometry g(32);
    EXPECT_TRUE(g.addPolygon({{0, 0}, {10, 0}, {10, 5}, {0, 5}}));
    EXPECT_TRUE(g.addCircle({20, 2.5}, 2));
    return g;
}

void expectBounds(const BoundingBox& b, double x0, double y0, double x1, double y1) {
    EXPECT_NEAR(b.xmin, x0, 1e-9);
    EXPECT_NEAR(b.ymin, y0, 1e-9);
    EXPECT_NEAR(b.xmax, x1, 1e-9);
    EXPECT_NEAR(b.ymax, y1, 1e-9);
}

}  // namespace

TEST(Geometry, EmptyGeometry) {
    Geometry g;
    EXPECT_TRUE(g.isEmpty());
    expectBounds(g.bounds(), 0, 0, 0, 0);
    EXPECT_EQ(g.size(), (std::pair<double, double>{0.0, 0.0}));
}

TEST(Geometry, AddShapesAndBounds) {
    Geometry g = makeGeometry();
    EXPECT_FALSE(g.isEmpty());
    EXPECT_EQ(g.solidGeometry().size(), 2u);
    expectBounds(g.bounds(), golden::kGeomBounds0, golden::kGeomBounds1, golden::kGeomBounds2,
                 golden::kGeomBounds3);
    const auto s = g.size();
    EXPECT_NEAR(s.first, golden::kGeomBounds2 - golden::kGeomBounds0, 1e-9);
    EXPECT_NEAR(s.second, golden::kGeomBounds3 - golden::kGeomBounds1, 1e-9);
}

TEST(Geometry, InvalidShapesAreRejected) {
    Geometry g;
    EXPECT_FALSE(g.addPolygon({{0, 0}, {1, 1}}));               // anillo con muy pocos puntos
    EXPECT_FALSE(g.addPolygon({{0, 0}, {2, 2}, {2, 0}, {0, 2}}));  // polígono "moño": inválido
    EXPECT_FALSE(g.addPolyline({{0, 0}}));
    EXPECT_TRUE(g.addPolyline({{0, 0}, {1, 1}}));
    EXPECT_EQ(g.solidGeometry().size(), 1u);
}

TEST(Geometry, ToolsFallbackToToolOne) {
    Geometry g;
    g.addCircle({0, 0}, 1, 5);          // la herramienta 5 no existe: va a la 1
    g.tools()[7];                       // se crea la herramienta 7
    g.addCircle({3, 0}, 1, 7);          // ahora sí existe
    EXPECT_EQ(g.tools().at(1).solidGeometry.size(), 1u);
    EXPECT_EQ(g.tools().at(7).solidGeometry.size(), 1u);
    EXPECT_EQ(g.tools().count(5), 0u);
}

TEST(Geometry, FlattenExpandsMultiShapes) {
    Geometry g;
    g.solidGeometry().push_back(Shape::collection({Shape::polygon({{0, 0}, {1, 0}, {1, 1}}),
                                                   Shape::polygon({{5, 5}, {6, 5}, {6, 6}})}));
    g.solidGeometry().push_back(Shape::lineString({{0, 0}, {1, 1}}));
    EXPECT_EQ(g.flatten().size(), 3u);
    // reset = false acumula.
    EXPECT_EQ(g.flatten(nullptr, false).size(), 6u);
}

TEST(Geometry, FlattenPathOnlyExpandsPolygonsIntoRings) {
    Geometry g;
    g.solidGeometry().push_back(
        Shape::polygon({{0, 0}, {10, 0}, {10, 10}, {0, 10}}, {{{2, 2}, {4, 2}, {4, 4}, {2, 4}}}));
    const ShapeList& flat = g.flatten(nullptr, true, true);
    ASSERT_EQ(flat.size(), 2u);  // exterior + 1 interior
    EXPECT_EQ(flat[0].type(), ShapeType::LinearRing);
    EXPECT_EQ(flat[1].type(), ShapeType::LinearRing);
}

TEST(Geometry, BoundsWithFlattenReplacesSolidList) {
    Geometry g;
    g.solidGeometry().push_back(Shape::collection({Shape::polygon({{0, 0}, {1, 0}, {1, 1}}),
                                                   Shape::polygon({{5, 5}, {6, 5}, {6, 6}})}));
    expectBounds(g.bounds(true), 0, 0, 6, 6);
    EXPECT_EQ(g.solidGeometry().size(), 2u);  // quedó aplanada
}

TEST(Geometry, InteriorsExteriorsAndFindPolygon) {
    Geometry g;
    g.solidGeometry().push_back(
        Shape::polygon({{0, 0}, {10, 0}, {10, 10}, {0, 10}}, {{{2, 2}, {4, 2}, {4, 4}, {2, 4}}}));
    g.solidGeometry().push_back(Shape::polygon({{20, 0}, {30, 0}, {30, 10}, {20, 10}}));
    EXPECT_EQ(g.getInteriors().size(), 1u);
    EXPECT_EQ(g.getExteriors().size(), 2u);
    ASSERT_TRUE(g.findPolygon({25, 5}).has_value());
    EXPECT_NEAR(g.findPolygon({25, 5})->area(), 100.0, 1e-9);
    EXPECT_FALSE(g.findPolygon({3, 3}).has_value());  // dentro del agujero
    EXPECT_FALSE(g.findPolygon({100, 100}).has_value());
}

TEST(Geometry, MirrorRotateMatchShapely) {
    Geometry m = makeGeometry();
    m.mirror(Axis::Y, {5, 0});
    expectBounds(m.bounds(), golden::kGeomMirrorYBounds0, golden::kGeomMirrorYBounds1,
                 golden::kGeomMirrorYBounds2, golden::kGeomMirrorYBounds3);

    Geometry r = makeGeometry();
    r.rotate(90, {0, 0});
    expectBounds(r.bounds(), golden::kGeomRot90Bounds0, golden::kGeomRot90Bounds1,
                 golden::kGeomRot90Bounds2, golden::kGeomRot90Bounds3);
}

TEST(Geometry, SkewKeepsArea) {
    Geometry g = makeGeometry();
    const double before = g.solidGeometry()[0].area();
    g.skew(10, 0, {0, 0});
    EXPECT_NEAR(g.solidGeometry()[0].area(), before, 1e-9);  // la cizalla conserva el área
}

TEST(Geometry, BufferAndFactorMatchShapely) {
    Geometry b = makeGeometry();
    b.buffer(0.25, JoinStyle::Round, false);
    expectRelNear(b.solidGeometry()[0].area(), golden::kGeomBufferAreaRect);
    expectRelNear(b.solidGeometry()[1].area(), golden::kGeomBufferAreaCirc);

    Geometry f = makeGeometry();
    f.buffer(1.2, JoinStyle::Round, true);  // factor: escala cada figura respecto de su centro
    expectRelNear(f.solidGeometry()[0].area(), golden::kGeomFactorAreaRect);
    expectRelNear(f.solidGeometry()[1].area(), golden::kGeomFactorAreaCirc);

    Geometry z = makeGeometry();
    const double before = z.solidGeometry()[0].area();
    z.buffer(0.0, JoinStyle::Round, false);  // distancia 0: no hace nada
    EXPECT_DOUBLE_EQ(z.solidGeometry()[0].area(), before);
}

TEST(Geometry, MultigeoOperatesOnTools) {
    Geometry g(32);
    g.setMultigeo(true);
    g.tools()[1].solidGeometry.push_back(Shape::polygon({{0, 0}, {2, 0}, {2, 2}, {0, 2}}));
    g.tools()[2].solidGeometry.push_back(Shape::polygon({{10, 10}, {12, 10}, {12, 12}, {10, 12}}));
    expectBounds(g.bounds(), 0, 0, 12, 12);
    g.mirror(Axis::Y, {0, 0});  // refleja X alrededor de x = 0
    expectBounds(g.bounds(), -12, 0, 0, 12);
    EXPECT_TRUE(g.solidGeometry().empty());  // en multigeo la lista sólida no se toca
}

TEST(Geometry, UnionAllMergesOverlaps) {
    Geometry g;
    g.addPolygon({{0, 0}, {2, 0}, {2, 2}, {0, 2}});
    g.addPolygon({{1, 1}, {3, 1}, {3, 3}, {1, 3}});
    g.unionAll();
    ASSERT_EQ(g.solidGeometry().size(), 1u);
    EXPECT_NEAR(g.solidGeometry()[0].area(), 4.0 + 4.0 - 1.0, 1e-9);
}

TEST(Geometry, SubtractPolygonCutsPaths) {
    Geometry g;
    g.solidGeometry().push_back(Shape::lineString({{0, 0}, {10, 0}}));
    g.subtractPolygon({{4, -1}, {6, -1}, {6, 1}, {4, 1}});
    ASSERT_EQ(g.solidGeometry().size(), 1u);
    EXPECT_NEAR(g.solidGeometry()[0].length(), 8.0, 1e-9);  // se quitó el tramo 4..6
}

TEST(Geometry, ConvertUnitsCallsScaleOverride) {
    // La base no escala nada; una subclase sí (como FlatCAMGerber en el programa original).
    struct Scaled : Geometry {
        double lastX = 0, lastY = 0;
        void scale(double x, double y, const std::optional<Point2D>&) override { lastX = x; lastY = y; }
    };
    Scaled g;
    EXPECT_DOUBLE_EQ(g.convertUnits(Units::Millimeters), 1.0);  // ya estaba en MM
    EXPECT_DOUBLE_EQ(g.convertUnits(Units::Inches), 1.0 / 25.4);
    EXPECT_EQ(g.units(), Units::Inches);
    EXPECT_DOUBLE_EQ(g.lastX, 1.0 / 25.4);
    EXPECT_DOUBLE_EQ(g.fileUnitsFactor(), 1.0 / 25.4);
    EXPECT_DOUBLE_EQ(g.convertUnits(Units::Millimeters), 25.4);
}
