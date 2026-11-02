// Operación "Aislamiento" de NCC (get_tool_empty_area con isotooldia) contra el original (golden_ncc_isolation.h).
#include <gtest/gtest.h>

#include <cmath>
#include <sstream>

#include "golden_ncc_isolation.h"
#include "parsers/Gerber.h"
#include "tools/Ncc.h"

using namespace flatcam;

namespace {

Gerber loadGerber(const char* text, bool isPath) {
    Gerber g;
    if (isPath) {
        EXPECT_TRUE(g.parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/" + text));
    } else {
        std::vector<std::string> lines;
        std::stringstream ss(text);
        std::string l;
        while (std::getline(ss, l)) lines.push_back(l);
        EXPECT_TRUE(g.parseLines(lines));
    }
    return g;
}

/// Distancia máxima de los vértices de @p a a la figura @p b.
double maxVertexDistance(const Shape& a, const Shape& b) {
    double d = 0;
    for (const Shape& part : a.parts().empty() ? ShapeList{a} : a.parts())
        for (const Point2D& p : part.coordinates()) d = std::max(d, Shape::point(p).distance(b));
    return d;
}

}  // namespace

class NccIsolation : public ::testing::TestWithParam<std::size_t> {};

TEST_P(NccIsolation, MatchesOriginal) {
    const auto& sc = golden_ncc_isolation::kScenarios.at(GetParam());
    SCOPED_TRACE(sc.name);
    const Gerber g = loadGerber(sc.gerber, sc.isPath);
    NccOptions o;
    o.margin = sc.margin;
    o.hasOffset = sc.hasOffset;
    o.offset = sc.offset;
    o.isoToolDias = sc.isoDias;
    o.climb = sc.climb;
    const Shape box = Ncc::clearingBox(g.solidGeometry(), o);
    // El recorrido de cada herramienta de aislamiento: mismo sentido y los mismos vértices (a menos de una micra).
    for (std::size_t i = 0; i < sc.isoDias.size(); ++i) {
        bool broken = false;
        const ShapeList mine = Ncc::isolationPaths(g.solidGeometry(), sc.isoDias[i], sc.climb, box, 64, broken);
        EXPECT_EQ(broken, static_cast<bool>(sc.broken[i]));
        ASSERT_EQ(mine.size(), sc.isoPaths[i].size()) << "herramienta " << sc.isoDias[i];
        for (std::size_t k = 0; k < mine.size(); ++k) {
            const Shape ref = Shape::fromWkt(sc.isoPaths[i][k]);
            // GEOS 3.15 devuelve como una sola línea un anillo cortado por la caja cuyas dos partes se tocan en su inicio;
            // 3.11 y 3.12, como dos (MultiLineString). Misma geometría: se comprueba abajo.
            const bool lines = (mine[k].type() == ShapeType::LineString || mine[k].type() == ShapeType::MultiLineString) &&
                               (ref.type() == ShapeType::LineString || ref.type() == ShapeType::MultiLineString);
            if (!lines) EXPECT_EQ(mine[k].type(), ref.type()) << k;
            EXPECT_NEAR(mine[k].length(), ref.length(), 1e-6) << k;
            EXPECT_LT(maxVertexDistance(mine[k], ref), 1e-6) << k;
            EXPECT_LT(maxVertexDistance(ref, mine[k]), 1e-6) << k;
            // El sentido: el primer tramo de cada parte del port aparece, en el mismo sentido, en el recorrido del original
            // (así no depende de en qué vértice empieza un anillo ni de cómo se parte: GEOS 3.15 cambia las dos cosas).
            for (const Shape& part : mine[k].parts().empty() ? ShapeList{mine[k]} : mine[k].parts()) {
                const auto c = part.coordinates();
                ASSERT_GE(c.size(), 2u);
                bool found = false;
                for (const Shape& rp : ref.parts().empty() ? ShapeList{ref} : ref.parts()) {
                    const auto r = rp.coordinates();
                    for (std::size_t j = 0; !found && j + 1 < r.size(); ++j)
                        found = std::hypot(r[j].x - c[0].x, r[j].y - c[0].y) < 1e-6 &&
                                std::hypot(r[j + 1].x - c[1].x, r[j + 1].y - c[1].y) < 1e-6;
                }
                EXPECT_TRUE(found) << "recorrido " << k << " en otro sentido";
            }
        }
    }
    // El área que queda para limpiar: la misma (diferencia simétrica despreciable).
    const ShapeList empty = Ncc::emptyArea(g.solidGeometry(), o);
    const Shape mine = Shape::unaryUnion(empty);
    const Shape ref = Shape::fromWkt(sc.empty);
    EXPECT_EQ(empty.size(), ref.parts().empty() ? 1u : ref.parts().size());
    EXPECT_NEAR(mine.area(), ref.area(), 1e-9 * ref.area());
    EXPECT_LT(mine.symmetricDifference(ref).area(), 1e-6 * ref.area());
}

INSTANTIATE_TEST_SUITE_P(Original, NccIsolation,
                         ::testing::Range<std::size_t>(0, golden_ncc_isolation::kScenarios.size()),
                         [](const ::testing::TestParamInfo<std::size_t>& info) {
                             return std::string(golden_ncc_isolation::kScenarios.at(info.param).name);
                         });

// Diámetro de herramienta seguro (find_optim_mp de NCC, el mismo cálculo de Isolation) contra el original.
#include "golden_safe_tooldia.h"
#include "tools/Isolation.h"

TEST(SafeToolDiameter, MatchesOriginal) {
    for (const auto& c : golden_safe_tooldia::kCases) {
        SCOPED_TRACE(c.name);
        const Gerber g = loadGerber(c.gerber, c.isPath);
        std::string error;
        const auto d = Isolation::safeToolDiameter(g, 4, error);
        ASSERT_EQ(d.has_value(), c.ok) << error;
        if (d) EXPECT_DOUBLE_EQ(*d, c.dist);
    }
}
