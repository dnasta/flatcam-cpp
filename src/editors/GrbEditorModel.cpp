#include "editors/GrbEditorModel.h"

#include <algorithm>
#include <cmath>

#include "core/ArcMath.h"
#include "core/GeometryError.h"
#include "editors/GeoEditorModel.h"

namespace flatcam {

namespace {

constexpr double kPi = 3.14159265358979323846;

/// round() de Python (al par en los empates).
double pyRound(double v) { return std::nearbyint(v); }

}  // namespace

namespace grbdraw {

std::optional<ApertureGeometry> pad(const Aperture& ap, const Point2D& p, int steps) {
    const double radius = ap.size.value_or(0.0) / 2;
    const double hw = ap.width / 2, hh = ap.height / 2;
    ApertureGeometry e;
    e.follow = Shape::point(p);
    if (ap.type == "C") {
        e.solid = Shape::point(p).buffer(radius);
        return e;
    }
    if (ap.type == "R") {
        e.solid = Shape::polygon({{p.x - hw, p.y - hh}, {p.x + hw, p.y - hh}, {p.x + hw, p.y + hh},
                                  {p.x - hw, p.y + hh}, {p.x - hw, p.y - hh}});
        return e;
    }
    if (ap.type == "O") {
        std::vector<Point2D> geo;
        if (hh > hw) {
            const auto down = arc({p.x, p.y - hh + hw}, hw, kPi, 0.0, ArcDirection::CounterClockwise, steps);
            const auto up = arc({p.x, p.y + hh - hw}, hw, 0.0, kPi, ArcDirection::CounterClockwise, steps);
            geo.push_back({p.x - hw, p.y - hh + hw});
            geo.insert(geo.end(), down.begin(), down.end());
            geo.push_back({p.x + hw, p.y - hh + hw});
            geo.push_back({p.x + hw, p.y + hh - hw});
            geo.insert(geo.end(), up.begin(), up.end());
            geo.push_back({p.x - hw, p.y + hh - hw});
        } else {
            const auto left = arc({p.x - hw + hh, p.y}, hh, kPi / 2, 1.5 * kPi, ArcDirection::CounterClockwise, steps);
            const auto right = arc({p.x + hw - hh, p.y}, hh, 1.5 * kPi, kPi / 2, ArcDirection::CounterClockwise, steps);
            geo.push_back({p.x - hw + hh, p.y - hh});
            geo.push_back({p.x + hw - hh, p.y - hh});
            geo.insert(geo.end(), right.begin(), right.end());
            geo.push_back({p.x + hw - hh, p.y + hh});
            geo.push_back({p.x - hw + hh, p.y + hh});
            geo.insert(geo.end(), left.begin(), left.end());
        }
        e.solid = Shape::polygon(geo);
        return e;
    }
    return std::nullopt;
}

std::optional<Point2D> bend(int mode, const Point2D& old, const Point2D& cur, double gx, double gy, bool later,
                            std::optional<Point2D> prev) {
    const double x = cur.x, y = cur.y, ox = old.x, oy = old.y;
    const double mx = std::abs(pyRound((x - ox) / gx)), my = std::abs(pyRound((y - oy) / gy));
    std::optional<Point2D> r = prev;
    if (mode == 1) {
        if (x > ox) {
            if (mx > my) r = Point2D{ox + gx * (mx - my), oy};
            else if (mx < my) r = y < oy ? Point2D{ox, oy - gy * (my - mx)}
                                         : Point2D{ox, later ? oy + gy * (my - mx) : oy - gy * (mx - my)};
        }
        if (x < ox) {
            if (mx > my) r = Point2D{ox - gx * (mx - my), oy};
            else if (mx < my) r = y < oy ? Point2D{ox, oy - gy * (my - mx)}
                                         : Point2D{ox, later ? oy + gy * (my - mx) : oy - gy * (mx - my)};
        }
    } else if (mode == 2) {
        if (x > ox) {
            if (mx > my) r = Point2D{ox + gx * my, y};
            if (mx < my) r = y < oy ? Point2D{x, oy - gy * mx} : Point2D{x, oy + gy * mx};
        }
        if (x < ox) {
            if (mx > my) r = Point2D{ox - gx * my, y};
            if (mx < my) r = y < oy ? Point2D{x, oy - gy * mx} : Point2D{x, oy + gy * mx};
        }
    } else if (mode == 3) {
        r = Point2D{x, oy};
    } else if (mode == 4) {
        r = Point2D{ox, y};
    }
    return r;
}

std::optional<ApertureGeometry> semiDisc(int mode, const Point2D& a, const Point2D& b, const Point2D& c, bool cw,
                                         double buf, int steps) {
    const geodraw::ArcMode m = mode == 0 ? geodraw::ArcMode::C12 : mode == 1 ? geodraw::ArcMode::P132 : geodraw::ArcMode::P12C;
    const auto center = geodraw::arcCenter(m, a, b, c);
    if (!center) return std::nullopt;
    double radius;
    ArcDirection dir = cw ? ArcDirection::Clockwise : ArcDirection::CounterClockwise;
    Point2D p1 = a, p2 = (mode == 0) ? c : b;
    if (mode == 0) {
        radius = distance(a, b) + buf / 2;
        p1 = b;
    } else if (mode == 1) {
        const auto circ = threePointCircle(a, c, b);
        radius = circ->radius + buf / 2;
        dir = circ->t > 0 ? ArcDirection::Clockwise : ArcDirection::CounterClockwise;
    } else {
        radius = std::sqrt((center->x - a.x) * (center->x - a.x) + (center->y - a.y) * (center->y - a.y)) + buf / 2;
    }
    const double start = std::atan2(p1.y - center->y, p1.x - center->x);
    const double stop = std::atan2(p2.y - center->y, p2.x - center->x);
    const Shape poly = Shape::polygon(arc(*center, radius, start, stop, dir, steps));
    ApertureGeometry e;
    e.solid = poly;
    e.follow = poly.exterior();
    return e;
}

}  // namespace grbdraw

GrbEditorModel::Ap* GrbEditorModel::aperture(const std::string& code) {
    for (Ap& a : apertures)
        if (a.code == code) return &a;
    return nullptr;
}

GrbEditorModel::Ap& GrbEditorModel::ensureZero() {
    if (Ap* z = aperture("0")) return *z;
    Ap z;
    z.code = "0";
    z.ap.type = "REG";
    z.ap.size = 0.0;
    apertures.push_back(z);
    return apertures.back();
}

std::optional<std::string> GrbEditorModel::codeOf(const GrbShapePtr& s) const {
    for (const Ap& a : apertures)
        if (std::find(a.geometry.begin(), a.geometry.end(), s) != a.geometry.end()) return a.code;
    return std::nullopt;
}

void GrbEditorModel::load(const Gerber& g) {
    *this = GrbEditorModel();
    ShapeList clears;
    for (const std::string& code : g.apertureOrder())
        for (const ApertureGeometry& e : g.apertures().at(code).geometry)
            if (e.clear) clears.push_back(*e.clear);
    ShapeList clearParts;
    if (!clears.empty()) {
        const Shape u = Shape::unaryUnion(clears);
        clearParts = u.type() == ShapeType::Polygon ? ShapeList{u} : u.parts();
    }
    for (const std::string& code : g.apertureOrder()) {
        Ap a;
        a.code = code;
        a.ap = g.apertures().at(code);
        a.ap.geometry.clear();
        for (const ApertureGeometry& e : g.apertures().at(code).geometry) {
            ApertureGeometry n = e;
            if (e.solid)
                for (const Shape& c : clearParts)
                    if (c.within(*n.solid)) n.solid = n.solid->difference(c);
            if (n.solid || n.follow || n.clear) a.geometry.push_back(std::make_shared<ApertureGeometry>(n));
        }
        apertures.push_back(std::move(a));
    }
}

GrbShapePtr GrbEditorModel::add(const std::string& code, const ApertureGeometry& e) {
    Ap* a = aperture(code);
    if (!a) return nullptr;
    auto s = std::make_shared<ApertureGeometry>(e);
    a->geometry.push_back(s);
    return s;
}

void GrbEditorModel::remove(const GrbShapePtr& s) {
    for (Ap& a : apertures) a.geometry.erase(std::remove(a.geometry.begin(), a.geometry.end(), s), a.geometry.end());
    selected.erase(std::remove(selected.begin(), selected.end(), s), selected.end());
}

void GrbEditorModel::deleteSelected() {
    const auto tmp = selected;
    for (const auto& s : tmp) remove(s);
    selected.clear();
}

void GrbEditorModel::clickSelect(const Point2D& p) {
    const Shape pt = Shape::point(p);
    for (const Ap& a : apertures) {
        for (std::size_t start = 0; start < a.geometry.size(); start += 77) {
            const std::size_t end = std::min(a.geometry.size(), start + 77);
            for (std::size_t i = start; i < end; ++i) {
                const auto& s = a.geometry[i];
                if (s->solid && pt.intersects(*s->solid)) {
                    auto it = std::find(selected.begin(), selected.end(), s);
                    if (it != selected.end()) selected.erase(it);
                    else selected.push_back(s);
                    break;
                }
            }
        }
    }
}

void GrbEditorModel::areaSelect(const Point2D& a, const Point2D& b, bool contains, bool toggle) {
    const Shape poly = Shape::polygon({a, {b.x, a.y}, b, {a.x, b.y}});
    if (!toggle) selected.clear();
    for (const Ap& ap : apertures)
        for (const auto& s : ap.geometry) {
            if (!s->solid) continue;
            if (contains ? poly.contains(*s->solid) : poly.intersects(*s->solid)) {
                auto it = std::find(selected.begin(), selected.end(), s);
                if (toggle && it != selected.end()) selected.erase(it);
                else if (it == selected.end()) selected.push_back(s);
            }
        }
}

void GrbEditorModel::buffer(const std::vector<std::string>& codes, double dist, int join) {
    const JoinStyle js = join == 1 ? JoinStyle::Round : join == 2 ? JoinStyle::Mitre : JoinStyle::Bevel;
    for (const std::string& code : codes) {
        Ap* a = aperture(code);
        if (!a) continue;
        for (auto& s : a->geometry) {
            if (std::find(selected.begin(), selected.end(), s) == selected.end()) continue;
            ApertureGeometry n;
            if (s->solid) n.solid = s->solid->buffer(dist, 16, js);
            if (s->follow) n.follow = s->follow->buffer(dist, 16, js);
            if (s->clear) n.clear = s->clear->buffer(dist, 16, js);
            s = std::make_shared<ApertureGeometry>(n);
        }
    }
}

void GrbEditorModel::scale(const std::vector<std::string>& codes, double f) {
    for (const std::string& code : codes) {
        Ap* a = aperture(code);
        if (!a) continue;
        for (auto& s : a->geometry) {
            if (std::find(selected.begin(), selected.end(), s) == selected.end()) continue;
            ApertureGeometry n;
            if (s->solid) n.solid = s->solid->scale(f, f);
            if (s->follow) n.follow = s->follow->scale(f, f);
            if (s->clear) n.clear = s->clear->scale(f, f);
            s = std::make_shared<ApertureGeometry>(n);
        }
    }
}

std::vector<GrbShapePtr> GrbEditorModel::markArea(double lower, double upper) const {
    std::vector<GrbShapePtr> out;
    for (const Ap& a : apertures)
        for (const auto& s : a.geometry)
            if (s->solid) {
                const double area = s->solid->area();
                if (upper > area && area > lower) out.push_back(s);
            }
    return out;
}

bool GrbEditorModel::poligonize(const std::string& lastAperture) {
    if (selected.empty()) return false;
    std::vector<std::string> codes;
    for (const auto& s : selected)
        if (const auto c = codeOf(s))
            if (std::find(codes.begin(), codes.end(), *c) == codes.end()) codes.push_back(*c);
    if (codes.size() > 1) return false;
    ShapeList ext;
    for (const auto& s : selected)
        if (s->solid && s->solid->type() == ShapeType::Polygon) ext.push_back(Shape::polygon(s->solid->exterior().coordinates()));
    const Shape fused = Shape::collection(ext).buffer(0.0000001);
    std::string target = lastAperture;
    auto addTo = [&](const Shape& g) {
        ApertureGeometry e;
        e.solid = g;
        e.follow = g.exterior();
        if (!aperture(target)) ensureZero();
        add(target, e);
    };
    if (fused.type() == ShapeType::MultiPolygon) {
        for (const Shape& part : fused.parts()) {
            const Shape g = part.buffer(0);
            if (g.interiors().empty()) {
                ensureZero();
                target = "0";
            }
            addTo(g);
        }
    } else {
        const Shape g = fused.buffer(0);
        if (g.interiors().empty() && ext.size() == 1) {
            ensureZero();
            target = "0";
        }
        addTo(g);
    }
    deleteSelected();
    return true;
}

void GrbEditorModel::move(const std::vector<std::string>& codes, double dx, double dy, bool copy) {
    for (const std::string& code : codes) {
        Ap* a = aperture(code);
        if (!a) continue;
        std::vector<GrbShapePtr> done;
        const auto geo = a->geometry;
        for (const auto& s : selected) {
            if (std::find(geo.begin(), geo.end(), s) == geo.end()) continue;
            ApertureGeometry n;
            if (s->solid) n.solid = s->solid->translate(dx, dy);
            if (s->follow) n.follow = s->follow->translate(dx, dy);
            if (s->clear) n.clear = s->clear->translate(dx, dy);
            if (!copy) a->geometry.erase(std::remove(a->geometry.begin(), a->geometry.end(), s), a->geometry.end());
            a->geometry.push_back(std::make_shared<ApertureGeometry>(n));
            done.push_back(s);
        }
        for (const auto& s : done) selected.erase(std::remove(selected.begin(), selected.end(), s), selected.end());
    }
}

void GrbEditorModel::erase(double dx, double dy) {
    ShapeList er;
    for (const auto& s : selected) {
        if (!s->solid) continue;
        const Shape b = s->solid->translate(dx, dy).buffer(0.0000001);
        if (b.type() != ShapeType::Polygon) throw GeometryError("erase: exterior de una figura múltiple");
        er.push_back(Shape::polygon(b.exterior().coordinates()));
    }
    const Shape eraser = Shape::unaryUnion(er);
    for (Ap& a : apertures)
        for (auto& s : a.geometry)
            if (s->solid && (eraser.within(*s->solid) || eraser.intersects(*s->solid)))
                s->solid = s->solid->difference(eraser).buffer(0);
}

GrbEditorModel::Out GrbEditorModel::result() const {
    Out o;
    ShapeList polys;
    for (const Ap& a : apertures) {
        if (a.geometry.empty()) continue;
        Aperture ap = a.ap;
        ap.geometry.clear();
        ap.hasGeometry = true;
        for (const auto& s : a.geometry) {
            ApertureGeometry n;
            if (s->solid) {
                n.solid = s->solid;
                polys.push_back(*s->solid);
            }
            if (s->follow) {
                n.follow = s->follow;
                o.follow.push_back(*s->follow);
            } else if (s->solid && s->solid->type() == ShapeType::Polygon) {
                n.follow = s->solid->exterior();
                o.follow.push_back(*n.follow);
            }
            if (s->clear) n.clear = s->clear;
            if (n.solid || n.follow || n.clear) ap.geometry.push_back(n);
        }
        o.apertures[a.code] = ap;
        o.order.push_back(a.code);
    }
    // MultiPolygon(poly_buffer).buffer(0.00000001).buffer(-0.00000001)
    if (!polys.empty()) {
        const Shape u = Shape::collection(polys).buffer(0.00000001).buffer(-0.00000001);
        if (!u.isEmpty()) o.solid = u.parts();
    }
    return o;
}

}  // namespace flatcam
