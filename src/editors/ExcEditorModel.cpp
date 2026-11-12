#include "editors/ExcEditorModel.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "core/ArcMath.h"

namespace flatcam {

namespace {

constexpr double kPi = 3.14159265358979323846;

double roundPy(double v, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return std::strtod(buf, nullptr);
}

/// La ranura engrosada a otro diámetro (ResizeEditorExc.make / on_tool_edit): la cuerda más larga entre las 359
/// diagonales giradas de a un grado, recortada en sus puntas y engrosada.
Shape resizedSlot(const Shape& poly, double newDia) {
    const BoundingBox b = poly.bounds().value_or(BoundingBox{});
    const Shape diag = Shape::lineString({{b.xmin, b.ymin}, {b.xmax, b.ymax}});
    const Point2D c = poly.centroid();
    double best = -1.0;
    Shape cut;
    for (int angle = 0; angle < 359; ++angle) {
        const Shape l = diag.rotate(angle, c).intersection(poly);
        const double len = l.length();
        if (len > best) {  // max(): el primero de los iguales
            best = len;
            cut = l;
        }
    }
    const auto coords = cut.coordinates();
    const Shape startCut = Shape::point(coords.at(0)).buffer(newDia / 2);
    const Shape stopCut = Shape::point(coords.at(1)).buffer(newDia / 2);
    return cut.difference(startCut).difference(stopCut).buffer(newDia / 2);
}

}  // namespace

// --------------------------------------------------------------------------------------------------- excdraw

namespace excdraw {

Shape drill(const Point2D& p, double dia) {
    return Shape::collection({Shape::lineString({{p.x - dia / 2, p.y}, {p.x + dia / 2, p.y}}),
                              Shape::lineString({{p.x, p.y - dia / 2}, {p.x, p.y + dia / 2}})});
}

Shape slot(const Point2D& p, double dia, double length, char axis, double angle, int steps, bool fromArray) {
    const double radius = dia / 2.0;
    double hw, hh;
    if (axis == 'X') {
        hw = length / 2.0;
        hh = radius;
    } else {
        hw = radius;
        hh = length / 2.0;
    }
    std::vector<Point2D> geo;
    const bool tall = hh > hw;
    if (tall) {
        const Point2D p1{p.x - hw, p.y - hh + hw}, p2{p.x + hw, p.y - hh + hw};
        const Point2D p3{p.x + hw, p.y + hh - hw}, p4{p.x - hw, p.y + hh - hw};
        const auto down = arc({p.x, p.y - hh + hw}, hw, kPi, 0.0, ArcDirection::CounterClockwise, steps);
        const auto up = arc({p.x, p.y + hh - hw}, hw, 0.0, kPi, ArcDirection::CounterClockwise, steps);
        geo.push_back(p1);
        geo.insert(geo.end(), down.begin(), down.end());
        geo.push_back(p2);
        geo.push_back(p3);
        geo.insert(geo.end(), up.begin(), up.end());
        geo.push_back(p4);
    } else {
        const Point2D p1{p.x - hw + hh, p.y - hh}, p2{p.x + hw - hh, p.y - hh};
        const Point2D p3{p.x + hw - hh, p.y + hh}, p4{p.x - hw + hh, p.y + hh};
        const auto left = arc({p.x - hw + hh, p.y}, hh, kPi / 2, 1.5 * kPi, ArcDirection::CounterClockwise, steps);
        const auto right = arc({p.x + hw - hh, p.y}, hh, 1.5 * kPi, kPi / 2, ArcDirection::CounterClockwise, steps);
        geo.push_back(p1);
        geo.push_back(p2);
        geo.insert(geo.end(), right.begin(), right.end());
        geo.push_back(p3);
        geo.push_back(p4);
        geo.insert(geo.end(), left.begin(), left.end());
    }
    const Shape poly = Shape::polygon(geo);
    if (axis == 'A' && (fromArray || tall)) return poly.rotate(-angle);
    return poly;
}

std::vector<Point2D> linearArray(const Point2D& p, int n, char axis, double pitch, double angle) {
    std::vector<Point2D> out;
    for (int i = 0; i < n; ++i) {
        if (axis == 'X') out.push_back({p.x + pitch * i, p.y});
        else if (axis == 'Y') out.push_back({p.x, p.y + pitch * i});
        else {
            const double xa = pitch * std::cos(angle * kPi / 180.0), ya = pitch * std::sin(angle * kPi / 180.0);
            out.push_back({p.x + xa * i, p.y + ya * i});
        }
    }
    return out;
}

std::optional<std::vector<CircularItem>> circularArray(const Point2D& origin, const Point2D& destination, int n,
                                                       double step, bool cw) {
    double radius = distance(destination, origin);
    if (radius == 0) return std::nullopt;
    if (destination.x < origin.x) radius = -radius;
    const double initial = std::asin((destination.y - origin.y) / radius);
    std::vector<CircularItem> out;
    for (int i = 0; i < n; ++i) {
        const double a = step * i * kPi / 180.0;  // math.radians
        if (cw) {
            out.push_back({{origin.x + radius * std::cos(-a + initial), origin.y + radius * std::sin(-a + initial)},
                           kPi - a + initial});
        } else {
            out.push_back({{origin.x + radius * std::cos(a + initial), origin.y + radius * std::sin(a + initial)},
                           a + initial - kPi});
        }
    }
    return out;
}

}  // namespace excdraw

// --------------------------------------------------------------------------------------------------- modelo

EditorStorage* ExcEditorModel::storage(double dia) {
    for (Tool& t : storages)
        if (t.dia == dia) return &t.storage;
    return nullptr;
}

EditorStorage& ExcEditorModel::ensure(double dia) {
    if (EditorStorage* s = storage(dia)) return *s;
    storages.push_back({dia, EditorStorage()});
    return storages.back().storage;
}

void ExcEditorModel::drop(double dia) {
    storages.erase(std::remove_if(storages.begin(), storages.end(), [&](const Tool& t) { return t.dia == dia; }),
                   storages.end());
}

std::optional<double> ExcEditorModel::diaOf(const EditorShapePtr& s) const {
    for (const Tool& t : storages) {
        const auto o = t.storage.objects();
        if (std::find(o.begin(), o.end(), s) != o.end()) return t.dia;
    }
    return std::nullopt;
}

void ExcEditorModel::load(const Excellon& e, int decimals) {
    *this = ExcEditorModel();
    // points_edit (con dec_format) y slot_points_edit, en el orden de las herramientas.
    std::vector<std::pair<double, std::vector<Point2D>>> drills;
    std::vector<std::pair<double, std::vector<std::pair<Point2D, Point2D>>>> slots;
    auto find = [](auto& v, double d) -> decltype(&v.front().second) {
        for (auto& [k, list] : v)
            if (k == d) return &list;
        return nullptr;
    };
    for (const auto& [id, t] : e.drillTools()) {
        const double d = roundPy(t.tooldia, decimals);
        if (!t.drills.empty()) {
            if (auto* l = find(drills, d)) l->insert(l->end(), t.drills.begin(), t.drills.end());
            else drills.push_back({d, t.drills});
        }
    }
    for (const auto& [id, t] : e.drillTools()) {
        const double d = roundPy(t.tooldia, decimals);
        if (!t.slots.empty()) {
            if (auto* l = find(slots, d)) l->insert(l->end(), t.slots.begin(), t.slots.end());
            else slots.push_back({d, t.slots});
        }
    }
    for (const auto& [d, list] : drills) {
        pointsEdit[d] = static_cast<int>(list.size());
        EditorStorage& st = ensure(d);
        for (const Point2D& p : list) st.insert(makeEditorShape(excdraw::drill(p, d)));
    }
    for (const auto& [d, list] : slots) {
        slotPointsEdit[d] = static_cast<int>(list.size());
        EditorStorage& st = ensure(d);
        for (const auto& [a, b] : list) st.insert(makeEditorShape(Shape::lineString({a, b}).buffer(d / 2)));
    }
    for (const Tool& t : storages) olddiaNewdia[t.dia] = t.dia;
}

EditorShapePtr ExcEditorModel::add(double dia, const Shape& g) {
    EditorShapePtr s = makeEditorShape(g);
    ensure(dia).insert(s);
    return s;
}

void ExcEditorModel::remove(const EditorShapePtr& s) {
    for (Tool& t : storages) t.storage.remove(s);
    selected.erase(std::remove(selected.begin(), selected.end(), s), selected.end());
}

void ExcEditorModel::clickSelect(const Point2D& pos, bool toggle) {
    // La más cercana de cada almacén y después la más cercana de esas; un almacén vacío (StopIteration) corta todo.
    EditorStorage sel;
    for (const Tool& t : storages) {
        const auto n = t.storage.nearest(pos);
        if (!n) return;
        sel.insert(n->second);
    }
    const auto n = sel.nearest(pos);
    if (!n) return;
    const EditorShapePtr closest = n->second;
    double xmin = 0, ymin = 0, xmax = 0, ymax = 0;
    const ShapeType t = closest->geo.type();
    if (t == ShapeType::MultiLineString) {
        const auto c = closest->geo.parts().front().coordinates();
        const double delta = c[1].x - c[0].x;
        xmin = c[0].x - 0.7 * delta;
        xmax = c[0].x + 1.7 * delta;
        ymin = c[0].y - 0.7 * delta;
        ymax = c[0].y + 1.7 * delta;
    } else if (t == ShapeType::Polygon) {
        const BoundingBox b = closest->geo.bounds().value_or(BoundingBox{});
        const double dx = b.xmax - b.xmin, dy = b.ymax - b.ymin;
        const double delta = dx > dy ? dx : dy;
        xmin = b.xmin - 0.7 * delta;
        xmax = b.xmax + 0.7 * delta;
        ymin = b.ymin - 0.7 * delta;
        ymax = b.ymax + 0.7 * delta;
    }
    if (pos.x < xmin || pos.x > xmax || pos.y < ymin || pos.y > ymax) {
        selected.clear();
        return;
    }
    if (toggle) {
        auto it = std::find(selected.begin(), selected.end(), closest);
        if (it != selected.end()) selected.erase(it);
        else selected.push_back(closest);
    } else {
        selected = {closest};
    }
}

void ExcEditorModel::areaSelect(const Point2D& a, const Point2D& b, bool contains, bool toggle) {
    const Shape poly = Shape::polygon({a, {b.x, a.y}, b, {a.x, b.y}});
    if (!toggle) selected.clear();
    for (const Tool& t : storages)
        for (const auto& o : t.storage.objects())
            if (contains ? poly.contains(o->geo) : poly.intersects(o->geo)) {
                auto it = std::find(selected.begin(), selected.end(), o);
                if (toggle && it != selected.end()) selected.erase(it);
                else selected.push_back(o);
            }
}

std::optional<std::vector<double>> ExcEditorModel::resize(const std::set<double>& dias, double newDia) {
    const bool existed = olddiaNewdia.count(newDia) > 0;
    if (!existed) {
        ensure(newDia);
        olddiaNewdia[newDia] = newDia;
    }
    ShapeList geometry;
    std::vector<EditorShapePtr> toDelete;
    for (double selDia : dias) {
        EditorStorage* cur = storage(selDia);
        if (!cur) continue;
        const auto objs = cur->objects();
        for (const auto& s : selected) {
            if (std::find(objs.begin(), objs.end(), s) == objs.end()) continue;
            const ShapeType t = s->geo.type();
            if (t == ShapeType::MultiLineString) {
                const double f = newDia / selDia;
                geometry.push_back(s->geo.scale(f, f));
            } else if (t == ShapeType::Polygon) {
                geometry.push_back(resizedSlot(s->geo, newDia));
            } else {
                return std::nullopt;
            }
            cur->remove(s);
            if (t == ShapeType::MultiLineString) {
                if (pointsEdit.count(selDia) && pointsEdit[selDia] > 0) --pointsEdit[selDia];
                ++pointsEdit[newDia];
            } else {
                if (slotPointsEdit.count(selDia) && slotPointsEdit[selDia] > 0) --slotPointsEdit[selDia];
                ++slotPointsEdit[newDia];
            }
            toDelete.push_back(s);
        }
    }
    // Los diámetros que quedan sin agujeros (o sin ranuras, si no tienen agujeros) se borran; uno que no tiene
    // ninguno de los dos cancela todo (KeyError) y lo cambiado se pierde.
    std::vector<double> deleted;
    std::vector<double> keys;
    for (const Tool& t : storages) keys.push_back(t.dia);
    for (double k : keys) {
        if (pointsEdit.count(k)) {
            if (pointsEdit[k] == 0) deleted.push_back(k);
        } else if (slotPointsEdit.count(k)) {
            if (slotPointsEdit[k] == 0) deleted.push_back(k);
        } else {
            for (const auto& s : toDelete) selected.erase(std::remove(selected.begin(), selected.end(), s), selected.end());
            return std::nullopt;
        }
    }
    for (double k : deleted) {
        drop(k);
        olddiaNewdia.erase(k);
        pointsEdit[k] = 0;
    }
    for (const auto& s : toDelete) selected.erase(std::remove(selected.begin(), selected.end(), s), selected.end());
    if (EditorStorage* dst = storage(newDia))
        for (const Shape& g : geometry) dst->insert(makeEditorShape(g));
    return deleted;
}

void ExcEditorModel::changeDia(double oldDia, double newDia) {
    EditorStorage* src = storage(oldDia);
    if (!src) return;
    const auto objs = src->objects();
    if (!olddiaNewdia.count(newDia)) {
        ensure(newDia);
        olddiaNewdia[newDia] = newDia;
    }
    const double f = newDia / oldDia;
    for (const auto& s : objs) {
        Shape g;
        if (s->geo.type() == ShapeType::MultiLineString) {
            ShapeList lines;
            for (const Shape& l : s->geo.parts()) lines.push_back(l.scale(f, f));
            g = Shape::collection(lines);
        } else if (s->geo.type() == ShapeType::Polygon) {
            g = resizedSlot(s->geo, newDia);
        } else {
            g = Shape::fromWkt("POLYGON EMPTY");  // new_geo = Polygon()
        }
        // points_edit.pop(old_dia, None) nunca falla, así que slot_points_edit no se toca.
        pointsEdit.erase(oldDia);
        if (s->geo.type() == ShapeType::MultiLineString) ++pointsEdit[newDia];
        if (s->geo.type() == ShapeType::Polygon) ++slotPointsEdit[newDia];
        storage(newDia)->insert(makeEditorShape(g));
    }
    // on_tool_delete(old_dia)
    drop(oldDia);
    olddiaNewdia.erase(oldDia);
    pointsEdit[oldDia] = 0;
}

std::vector<ExcEditorModel::OutTool> ExcEditorModel::result() const {
    std::map<double, std::vector<Point2D>> drills;  // sorted(zip(...)): por diámetro
    std::map<double, std::vector<std::pair<Point2D, Point2D>>> slots;
    for (const Tool& t : storages) {
        for (const auto& x : t.storage.objects()) {
            if (x->geo.type() == ShapeType::MultiLineString) {
                const Shape first = x->geo.parts().front();
                const auto c = first.coordinates();
                drills[t.dia].push_back({c[0].x + first.length() / 2, c[0].y});
            } else if (x->geo.type() == ShapeType::Polygon) {
                const double radius = t.dia / 2 - 0.0000001;
                const Shape poly = x->geo.buffer(-radius);
                if (!poly.isValid() || poly.isEmpty()) continue;
                const BoundingBox b = poly.bounds().value_or(BoundingBox{});
                const double one = Shape::lineString({{b.xmin, b.ymin}, {b.xmax, b.ymax}}).intersection(poly).length();
                const double two = Shape::lineString({{b.xmin, b.ymax}, {b.xmax, b.ymin}}).intersection(poly).length();
                if (one < two) slots[t.dia].push_back({{b.xmin, b.ymax}, {b.xmax, b.ymin}});
                else slots[t.dia].push_back({{b.xmin, b.ymin}, {b.xmax, b.ymax}});
            }
        }
    }
    std::vector<OutTool> out;
    for (const auto& [d, pts] : drills) out.push_back({d, pts, {}});
    for (const auto& entry : slots) {
        const double d = entry.first;
        const auto& sl = entry.second;
        auto it = std::find_if(out.begin(), out.end(), [d](const OutTool& o) { return o.dia == d; });
        if (it == out.end()) {
            out.push_back({d, {}, sl});
        } else {
            it->slots.insert(it->slots.end(), sl.begin(), sl.end());
        }
    }
    return out;
}

}  // namespace flatcam
