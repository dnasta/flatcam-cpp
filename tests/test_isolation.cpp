#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>

#include "cam/CncJob.h"
#include "golden_isolation.h"
#include "parsers/Gerber.h"
#include "tools/Isolation.h"

using namespace flatcam;

namespace {

/// Secuencias de vértices de una figura: los anillos de un polígono, las partes de una figura múltiple o ella misma.
std::vector<std::vector<Point2D>> sequences(const Shape& s) {
    std::vector<std::vector<Point2D>> out;
    if (s.type() == ShapeType::Polygon) {
        out.push_back(s.exterior().coordinates());
        for (const Shape& h : s.interiors()) out.push_back(h.coordinates());
    } else if (static_cast<int>(s.type()) >= static_cast<int>(ShapeType::MultiPoint)) {
        for (const Shape& p : s.parts()) out.push_back(p.coordinates());
    } else {
        out.push_back(s.coordinates());
    }
    return out;
}

/// Doble del área con signo (positiva en sentido antihorario): dice hacia dónde recorre la fresa.
double signedArea2(const std::vector<Point2D>& c) {
    double a = 0;
    for (std::size_t i = 0; i + 1 < c.size(); ++i) a += c[i].x * c[i + 1].y - c[i + 1].x * c[i].y;
    return a;
}

double maxDistanceToNearestVertex(const std::vector<Point2D>& a, const std::vector<Point2D>& b) {
    double worst = 0;
    for (const Point2D& p : a) {
        double best = 1e300;
        for (const Point2D& q : b) best = std::min(best, std::hypot(p.x - q.x, p.y - q.y));
        worst = std::max(worst, best);
    }
    return worst;
}

double maxCoordinateDifference(const std::vector<Point2D>& a, const std::vector<Point2D>& b) {
    double worst = 0;
    for (std::size_t i = 0; i < a.size() && i < b.size(); ++i)
        worst = std::max({worst, std::fabs(a[i].x - b[i].x), std::fabs(a[i].y - b[i].y)});
    return worst;
}

/// Compara dos figuras: mismo tipo, mismas secuencias, mismo sentido y los mismos vértices. Si el vértice inicial cambió
/// (pasa con versiones nuevas de GEOS) se acepta la misma figura empezada en otro lado, a menos de una micra.
void expectSameShape(const Shape& mine, const Shape& ref, const std::string& where) {
    SCOPED_TRACE(where);
    ASSERT_EQ(mine.type(), ref.type());
    const auto a = sequences(mine), b = sequences(ref);
    ASSERT_EQ(a.size(), b.size()) << "anillos o partes";
    for (std::size_t k = 0; k < a.size(); ++k) {
        ASSERT_EQ(a[k].size(), b[k].size()) << "vértices de la secuencia " << k;
        if (maxCoordinateDifference(a[k], b[k]) < 1e-9) continue;
        EXPECT_LT(maxDistanceToNearestVertex(a[k], b[k]), 1e-6) << "secuencia " << k;
        EXPECT_LT(maxDistanceToNearestVertex(b[k], a[k]), 1e-6) << "secuencia " << k;
        const double sa = signedArea2(a[k]), sb = signedArea2(b[k]);
        EXPECT_NEAR(sa, sb, 1e-6 * std::max(1.0, std::fabs(sb))) << "sentido o área de la secuencia " << k;
    }
}

IsolationOptions optionsFor(const golden_isolation::Scenario& sc) {
    IsolationOptions o;
    o.combine = sc.combine;
    o.rest = sc.rest;
    o.forcedRest = sc.forced;
    o.order = sc.order;
    o.negativeDia = sc.negative;
    if (sc.exceptGeo) {
        o.useExcept = true;
        o.exceptGeometry = sc.exceptGeo();
    }
    if (sc.limited) o.limitedArea = sc.limited();
    return o;
}

}  // namespace

class IsolationGolden : public ::testing::TestWithParam<std::size_t> {};

TEST_P(IsolationGolden, MatchesTheOriginalTool) {
    const auto& sc = golden_isolation::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    const ShapeList geometry = sc.geometry ? sc.geometry() : ShapeList{};
    IsolationOutcome out;
    std::string error;
    const bool ok = Isolation::isolate(sc.copper(), sc.follow(), sc.tools, optionsFor(sc), out, error,
                                       sc.geometry ? &geometry : nullptr);
    ASSERT_TRUE(ok) << error;

    const auto objects = sc.objects();
    if (sc.combine) {
        // Un solo objeto Geometry con una entrada por herramienta que produjo recorridos, en el orden de la tabla.
        ASSERT_EQ(objects.size(), 1u);
        const auto& obj = objects[0];
        EXPECT_EQ(obj.name, sc.rest ? "PCB_iso_rest" : "PCB_iso_combined");
        ASSERT_EQ(out.sets.size(), obj.toolIndex.size());
        for (std::size_t i = 0; i < out.sets.size(); ++i) {
            EXPECT_EQ(out.sets[i].toolIndex, obj.toolIndex[i]);
            EXPECT_EQ(out.sets[i].pass, -1);
            ASSERT_EQ(out.sets[i].geometry.size(), obj.geometry[i].size()) << "figuras de la herramienta " << i;
            for (std::size_t k = 0; k < obj.geometry[i].size(); ++k)
                expectSameShape(out.sets[i].geometry[k], obj.geometry[i][k],
                                "herramienta " + std::to_string(i) + ", figura " + std::to_string(k));
        }
    } else {
        // Un objeto por pasada, con el nombre que le pone FlatCAM.
        ASSERT_EQ(out.sets.size(), objects.size());
        for (std::size_t i = 0; i < objects.size(); ++i) {
            const auto& set = out.sets[i];
            const auto& tool = sc.tools.at(set.toolIndex);
            EXPECT_EQ(Isolation::objectName("PCB", tool.toolDia, tool.type, set.pass, tool.passes), objects[i].name);
            ASSERT_EQ(objects[i].toolIndex.size(), 1u);
            EXPECT_EQ(set.toolIndex, objects[i].toolIndex[0]);
            ASSERT_EQ(set.geometry.size(), objects[i].geometry[0].size()) << "figuras del objeto " << objects[i].name;
            for (std::size_t k = 0; k < set.geometry.size(); ++k)
                expectSameShape(set.geometry[k], objects[i].geometry[0][k], objects[i].name + ", figura " + std::to_string(k));
        }
    }
}

INSTANTIATE_TEST_SUITE_P(Original, IsolationGolden,
                         ::testing::Range<std::size_t>(0, golden_isolation::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_isolation::kScenarios[info.param].name);
                         });

TEST(Isolation, PassOffsetFollowsTheOriginalFormula) {
    EXPECT_DOUBLE_EQ(Isolation::passOffset(0.2, 0, 0.1, false), 0.2 * (1 / 2.0000001));
    EXPECT_DOUBLE_EQ(Isolation::passOffset(0.2, 2, 0.1, false), 0.2 * (5 / 2.0000001) - 2 * 0.1 * 0.2);
    EXPECT_DOUBLE_EQ(Isolation::passOffset(0.2, 1, 0.3, true), -(0.2 * (3 / 2.0000001) - 0.3 * 0.2));
}

TEST(Isolation, ObjectNamesLikeFlatCam) {
    EXPECT_EQ(Isolation::objectName("B_Cu", 0.1, IsolationType::Full, 0, 1), "B_Cu_0.1000_iso");
    EXPECT_EQ(Isolation::objectName("B_Cu", 0.25, IsolationType::Exterior, 1, 3), "B_Cu_0.2500_ext_iso2");
    EXPECT_EQ(Isolation::objectName("B_Cu", 0.2, IsolationType::Interior, 0, 2, 2), "B_Cu_0.20_int_iso1");
}

TEST(Isolation, RestReportsWhatCouldNotBeIsolated) {
    // Las dos primeras pistas están a 0,3 mm: la fresa de 0,4 no pasa entre ellas.
    const ShapeList tracks{Shape::box(1, 1, 1.4, 9), Shape::box(1.7, 1, 2.1, 9), Shape::box(4, 1, 4.4, 9)};
    IsolationOptions o;
    o.rest = true;
    IsolationOutcome out;
    std::string error;
    ASSERT_TRUE(Isolation::isolate(tracks, {}, {IsolationTool{0.4}}, o, out, error)) << error;
    EXPECT_EQ(out.notIsolated.size(), 2u);
    ASSERT_TRUE(Isolation::isolate(tracks, {}, {IsolationTool{0.4}, IsolationTool{0.1}}, o, out, error)) << error;
    EXPECT_TRUE(out.notIsolated.empty());
    ASSERT_EQ(out.sets.size(), 2u);
    EXPECT_EQ(out.sets[0].geometry.size(), 1u);  // la de 0,4 aísla solo la pista suelta
    EXPECT_EQ(out.sets[1].geometry.size(), 2u);  // la de 0,1, las dos juntas
}

TEST(Isolation, RejectsBadInput) {
    IsolationOutcome out;
    std::string error;
    EXPECT_FALSE(Isolation::isolate({Shape::box(0, 0, 1, 1)}, {}, {}, IsolationOptions{}, out, error));
    EXPECT_FALSE(error.empty());
    EXPECT_FALSE(Isolation::isolate({Shape::box(0, 0, 1, 1)}, {}, {IsolationTool{0.0}}, IsolationOptions{}, out, error));
    EXPECT_FALSE(error.empty());
    // Todo el recorrido queda fuera del área elegida: no hay geometría.
    IsolationOptions o;
    o.limitedArea = {Shape::box(10, 10, 11, 11)};
    EXPECT_FALSE(Isolation::isolate({Shape::box(0, 0, 1, 1)}, {}, {IsolationTool{0.1}}, o, out, error));
    EXPECT_FALSE(error.empty());
}

TEST(Isolation, SinglePolygonWithSeparatePassesIsReturned) {
    // Desvío consciente: el original no puede iterar un único polígono con las pasadas separadas.
    IsolationOptions o;
    o.combine = false;
    IsolationOutcome out;
    std::string error;
    ASSERT_TRUE(Isolation::isolate({Shape::box(0, 0, 2, 1)}, {}, {IsolationTool{0.2}}, o, out, error)) << error;
    ASSERT_EQ(out.sets.size(), 1u);
    ASSERT_EQ(out.sets[0].geometry.size(), 1u);
    const Shape& p = out.sets[0].geometry[0];
    ASSERT_EQ(p.type(), ShapeType::Polygon);
    // Fresado en concordancia: el contorno exterior queda al revés del que da GEOS (horario).
    EXPECT_GT(signedArea2(p.exterior().coordinates()), 0);
    EXPECT_NEAR(p.area(), 2 + 2 * 3 * 0.1 + M_PI * 0.01, 1e-3);
}

TEST(Isolation, BottomCopperMatchesTheOriginalTool) {
    // El cobre inferior real con el parser propio: mismas figuras, puntos y longitud que la herramienta original.
    // Medido: con GEOS 3.11 (la del oráculo) coincide figura por figura y vértice por vértice. Con GEOS 3.12 y 3.15 el
    // buffer de 16 segmentos del resto da algunos vértices menos (15.513 contra 15.568 en la fresa de 0,2: 0,35 %) con
    // la misma longitud a menos de una micra, y GEOS 3.15 además devuelve los polígonos del Gerber en otro orden.
    Gerber g;
    ASSERT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-B_Cu.gbl"));
    for (const auto& rc : golden_isolation::kBottomCopper) {
        SCOPED_TRACE(rc.name);
        IsolationOptions o;
        o.rest = rc.rest;
        IsolationOutcome out;
        std::string error;
        ASSERT_TRUE(Isolation::isolate(g.solidGeometry(), g.followGeometry(), rc.tools, o, out, error)) << error;
        ASSERT_EQ(out.sets.size(), rc.result.size());
        for (std::size_t i = 0; i < rc.result.size(); ++i) {
            const auto& ref = rc.result[i];
            EXPECT_EQ(out.sets[i].toolIndex, ref.toolIndex);
            std::size_t points = 0;
            double length = 0;
            for (const Shape& s : out.sets[i].geometry) {
                for (const auto& seq : sequences(s)) {
                    points += seq.size();
                    for (std::size_t k = 0; k + 1 < seq.size(); ++k)
                        length += std::hypot(seq[k + 1].x - seq[k].x, seq[k + 1].y - seq[k].y);
                }
            }
            EXPECT_EQ(out.sets[i].geometry.size(), ref.shapes) << "figuras de la herramienta " << i;
            EXPECT_NEAR(static_cast<double>(points), static_cast<double>(ref.points), ref.points * 0.005)
                << "puntos de la herramienta " << i;
            EXPECT_NEAR(length, ref.length, 1e-3) << "longitud (mm) de la herramienta " << i;
        }
    }
}

TEST(Isolation, ResultFeedsTheGcodeGenerator) {
    // La geometría de aislamiento va directo a CncJob (polígonos y anillos se recorren por sus contornos).
    IsolationOutcome out;
    std::string error;
    ASSERT_TRUE(Isolation::isolate({Shape::box(0, 0, 2, 1), Shape::box(3, 0, 4, 1)}, {}, {IsolationTool{0.2}},
                                   IsolationOptions{}, out, error))
        << error;
    CncJob job;
    GeometryJobSettings s;
    s.toolSettings.toolDia = 0.2;
    ASSERT_TRUE(job.generateFromGeometry(out.sets[0].geometry, s));
    EXPECT_NE(job.gcode().find("G01"), std::string::npos);
}
