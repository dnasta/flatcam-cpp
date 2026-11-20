// Editor de Geometría contra el original (tests/tools/oracle_geo_editor.py -> golden_geo_editor.h): los mismos pasos
// encadenados sobre el mismo almacén; después de cada uno, las mismas figuras y la misma elección.
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <string>

#include "core/OriginalBugs.h"
#include "editors/GeoEditorModel.h"
#include "golden_geo_editor.h"

using namespace flatcam;

namespace {

/// Con GEOS 3.11 (la del oráculo) las figuras coinciden vértice por vértice; con 3.12 y 3.15 algunos anillos que salen
/// de un engrosado o de una resta arrancan en otro vértice: ahí se compara la misma figura (mismo tipo y casi sin
/// diferencia simétrica, o los mismos puntos si es una línea).
bool same(const Shape& a, const Shape& b) {
    if (a.isEmpty() || b.isEmpty()) return a.isEmpty() == b.isEmpty();
    if (a.equalsExact(b, 1e-9)) return true;
    if (Shape::geosVersion().rfind("3.11.", 0) == 0) return false;
    if (a.type() != b.type()) return false;
    const double scale = std::max(1.0, std::max(a.area(), a.length()));
    // Una línea cerrada que arranca en otro vértice: se comparan como polígonos.
    // Lo pintado (MultiLineString): con 3.15 la semilla parte los círculos en otros puntos (algún vértice más o menos)
    // y el método de líneas pierde la primera línea, que cae justo sobre el borde del polígono achicado (5 %).
    if (a.type() == ShapeType::MultiLineString) {
        const double tol = Shape::geosVersion().rfind("3.12.", 0) == 0 ? 0.01 : 0.05;
        auto count = [](const Shape& g) {
            double c = 0;
            for (const Shape& p : g.parts()) c += static_cast<double>(p.coordinates().size());
            return c;
        };
        const double na = count(a), nb = count(b);
        return std::abs(a.length() - b.length()) < tol * scale && std::abs(na - nb) <= tol * nb + 1;
    }
    if (a.type() == ShapeType::LineString) {
        const auto ca = a.coordinates(), cb = b.coordinates();
        if (ca.size() > 3 && cb.size() > 3 && ca.front() == ca.back() && cb.front() == cb.back())
            return Shape::polygon(ca).symmetricDifference(Shape::polygon(cb)).area() < 1e-9 * scale;
    }
    return a.symmetricDifference(b).area() < 1e-9 * scale && (a.area() > 0 || a.equals(b));
}

}  // namespace

TEST(GeoEditor, MatchesTheOriginal) {
    GeoEditorModel m;
    ShapeList input;
    for (const std::string& w : golden_geo_editor::kInput) input.push_back(Shape::fromWkt(w));
    m.load(input, 1);
    bool climb = true;
    const int steps = 64;
    int n = 0;
    for (const auto& s : golden_geo_editor::kSteps) {
        const std::string op = s.op, text = s.text;
        SCOPED_TRACE(std::to_string(n++) + " " + op + " " + text);
        const auto& a = s.args;
        std::vector<Point2D> pts;
        for (std::size_t i = 0; i + 1 < a.size(); i += 2) pts.push_back({a[i], a[i + 1]});
        bool ok = true;
        ShapeList result;
        try {
            if (op == "circle") {
                m.complete({geodraw::circle(pts[0], pts[1], steps)}, false, climb);
            } else if (op == "arc") {
                const std::string mode = text.substr(0, text.find(' '));
                const auto am = mode == "c12" ? geodraw::ArcMode::C12
                                : mode == "132" ? geodraw::ArcMode::P132
                                                : geodraw::ArcMode::P12C;
                const auto dir = text.substr(text.find(' ') + 1) == "cw" ? ArcDirection::Clockwise
                                                                         : ArcDirection::CounterClockwise;
                const auto g = geodraw::arc(am, pts[0], pts[1], pts[2], dir, steps);
                ASSERT_TRUE(g.has_value());
                m.complete({*g}, false, climb);
            } else if (op == "rect") {
                m.complete({geodraw::rectangle(pts[0], pts[1])}, false, climb);
            } else if (op == "polygon") {
                m.complete({Shape::polygon(pts)}, false, climb);
            } else if (op == "path") {
                m.complete({Shape::lineString(pts)}, false, climb);
            } else if (op == "select") {
                const auto o = m.storage.objects();
                m.selected.clear();
                for (double i : a) m.selected.push_back(o.at(static_cast<std::size_t>(i)));
            } else if (op == "click") {
                m.clickSelect(pts[0], a[2] != 0);
            } else if (op == "area") {
                m.areaSelect(pts[0], pts[1], a[4] != 0, a[5] != 0);
            } else if (op == "move" || op == "copy") {
                const double dx = pts[1].x - pts[0].x, dy = pts[1].y - pts[0].y;
                ShapeList moved;
                for (const auto& sh : m.selected) moved.push_back(sh->geo.translate(dx, dy));
                if (op == "move") m.deleteSelected();
                m.complete(moved, true, climb);
            } else if (op == "erase") {
                m.erase(pts[0], pts[1]);
            } else if (op == "explode") {
                const auto lines = m.explode();
                ok = lines.has_value();
                if (ok) m.complete(*lines, true, climb);
            } else if (op == "paint") {
                const auto meth = text == "Seed"    ? GeoEditorModel::PaintMethod::Seed
                                  : text == "Lines" ? GeoEditorModel::PaintMethod::Lines
                                                    : GeoEditorModel::PaintMethod::Standard;
                ok = !m.paint(a[0], a[1], a[2], a[3] != 0, a[4] != 0, meth, steps).has_value();
            } else if (op == "rotate") {
                m.rotate(a[0], {a[1], a[2]});
            } else if (op == "mirror") {
                m.mirror(text[0], pts[0]);
            } else if (op == "skew") {
                m.skew(a[0], a[1], {a[2], a[3]});
            } else if (op == "scale") {
                m.scale(a[0], a[1], {a[2], a[3]});
            } else if (op == "offset") {
                m.offset(a[0], a[1]);
            } else if (op == "buffer") {
                m.buffer(a[0], static_cast<int>(a[1]), a[2] != 0);
            } else if (op == "milling") {
                climb = text == "cl";
            } else if (op == "delete") {
                m.deleteSelected();
            } else if (op == "result") {
                result = m.result();
            } else if (op == "union") {
                m.unite();
            } else if (op == "intersection") {
                m.intersection();
            } else if (op == "subtract") {
                m.subtract();
            } else if (op == "cutpath") {
                ok = m.cutpath();
            } else {
                FAIL() << op;
            }
        } catch (const std::exception&) {
            ok = false;
        }
        EXPECT_EQ(ok, s.ok);
        const auto o = m.storage.objects();
        ASSERT_EQ(o.size(), s.shapes.size());
        for (std::size_t i = 0; i < o.size(); ++i)
            EXPECT_TRUE(same(o[i]->geo, Shape::fromWkt(s.shapes[i]))) << i << "\n" << o[i]->geo.toWkt() << "\n" << s.shapes[i];
        std::vector<int> sel;
        for (const auto& sh : m.selected)
            for (std::size_t i = 0; i < o.size(); ++i)
                if (o[i] == sh) sel.push_back(static_cast<int>(i));
        EXPECT_EQ(sel, s.selected);
        ASSERT_EQ(result.size(), s.result.size());
        for (std::size_t i = 0; i < result.size(); ++i) EXPECT_TRUE(same(result[i], Shape::fromWkt(s.result[i]))) << i;
    }
}

TEST(GeoEditorFixed, BufferExplodeIndexUnionClickAndClimb) {
    flatcam::OriginalBugsScope fixed(false);
    using flatcam::GeoEditorModel;
    using flatcam::Point2D;
    using flatcam::Shape;
    GeoEditorModel m;
    const Shape sq = Shape::polygon({{0, 0}, {10, 0}, {10, 10}, {0, 10}}, {{{3, 3}, {6, 3}, {6, 6}, {3, 6}}});
    const Shape line = Shape::lineString({{20, 0}, {25, 0}, {25, 5}});
    auto a = m.add(sq);
    auto b = m.add(line);
    // Unir sin nada elegido no agrega nada.
    m.selected.clear();
    m.unite();
    EXPECT_EQ(m.storage.size(), 2u);
    // Clic: la figura bajo el mouse; lejos de todo, no elige nada.
    m.clickSelect({10, 5}, false, 0.1);
    ASSERT_EQ(m.selected.size(), 1u);
    EXPECT_EQ(m.selected.front(), a);
    m.clickSelect({100, 100}, false, 0.1);
    EXPECT_TRUE(m.selected.empty());
    // Buffer: exterior agrega un polígono más grande; interior uno más chico.
    m.selected = {a};
    ASSERT_FALSE(m.bufferTool(GeoEditorModel::BufferKind::Exterior, 1.0, 1, 64));
    EXPECT_EQ(m.storage.size(), 3u);
    m.selected = {a};
    EXPECT_TRUE(m.bufferTool(GeoEditorModel::BufferKind::Interior, -1.0, 1, 64).has_value());  // negativo
    // Mover actualiza el índice: después de desplazar, el clic en el lugar nuevo elige la línea.
    m.selected = {b};
    m.offset(0, 100);
    m.clickSelect({25, 105}, false, 0.1);
    ASSERT_EQ(m.selected.size(), 1u);
    EXPECT_EQ(m.selected.front(), b);
    EXPECT_EQ(m.storage.nearest({25, 105})->second, b);
    // Explotar una línea y un polígono con agujero: 2 + 4 + 4 segmentos.
    m.selected = {a, b};
    const auto segs = m.explode();
    ASSERT_TRUE(segs.has_value());
    EXPECT_EQ(segs->size(), 10u);
    // Fresado por ascenso: los anillos siguen siendo anillos.
    const auto out = m.complete({Shape::linearRing({{0, 0}, {1, 0}, {1, 1}, {0, 0}}), line}, true, true);
    ASSERT_EQ(out.size(), 2u);
    EXPECT_EQ(out[0]->geo.type(), flatcam::ShapeType::LinearRing);
    EXPECT_EQ(out[1]->geo.type(), flatcam::ShapeType::LineString);
    EXPECT_EQ(out[1]->geo.coordinates().front().x, 25);
}
