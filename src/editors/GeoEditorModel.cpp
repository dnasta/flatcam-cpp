#include "editors/GeoEditorModel.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "cam/PolygonClearing.h"
#include "core/ArcMath.h"
#include "core/GeometryError.h"

namespace flatcam {

namespace {

bool isMulti(const Shape& s) {
    const ShapeType t = s.type();
    return t == ShapeType::MultiPoint || t == ShapeType::MultiLineString || t == ShapeType::MultiPolygon ||
           t == ShapeType::GeometryCollection;
}

std::vector<Point2D> reversed(std::vector<Point2D> v) {
    std::reverse(v.begin(), v.end());
    return v;
}

/// Polygon(p.exterior.coords[::-1], p.interiors)
Shape reversedPolygon(const Shape& p) {
    std::vector<std::vector<Point2D>> holes;
    for (const Shape& i : p.interiors()) holes.push_back(i.coordinates());
    return Shape::polygon(reversed(p.exterior().coordinates()), holes);
}

/// shapely.geometry.polygon.signed_area
double signedArea(const std::vector<Point2D>& c) {
    if (c.size() < 3) return 0.0;
    std::vector<double> xs, ys;
    for (const Point2D& p : c) {
        xs.push_back(p.x);
        ys.push_back(p.y);
    }
    xs.push_back(xs[1]);
    ys.push_back(ys[1]);
    double s = 0.0;
    for (std::size_t i = 1; i < c.size(); ++i) s += xs[i] * (ys[i + 1] - ys[i - 1]);
    return s / 2.0;
}

/// shapely.geometry.polygon.orient
Shape orient(const Shape& p, double sign) {
    std::vector<Point2D> ext = p.exterior().coordinates();
    if (!(signedArea(ext) / sign >= 0.0)) ext = reversed(ext);
    std::vector<std::vector<Point2D>> holes;
    for (const Shape& i : p.interiors()) {
        std::vector<Point2D> h = i.coordinates();
        if (!(signedArea(h) / sign <= 0.0)) h = reversed(h);
        holes.push_back(h);
    }
    return Shape::polygon(ext, holes);
}

/// AppGeoEditor.flatten(orient_val): polígonos orientados; recorre listas y figuras múltiples.
void flattenOriented(const Shape& g, int orientVal, ShapeList& out) {
    if (isMulti(g)) {
        for (const Shape& p : g.parts()) flattenOriented(p, orientVal, out);
        return;
    }
    out.push_back(g.type() == ShapeType::Polygon && !g.isEmpty() ? orient(g, orientVal) : g);
}

}  // namespace

// --------------------------------------------------------------------------------------------------- geodraw

namespace geodraw {

Shape circle(const Point2D& p1, const Point2D& p2, int steps) {
    return Shape::point(p1).buffer(distance(p1, p2), steps / 4);
}

std::optional<Point2D> arcCenter(ArcMode mode, const Point2D& a, const Point2D& b, const Point2D& c) {
    if (mode == ArcMode::C12) return a;
    if (mode == ArcMode::P132) {
        const auto circ = threePointCircle(a, c, b);
        if (!circ) return std::nullopt;
        return circ->center;
    }
    // '12c': el centro sobre la mediatriz, del lado del tercer punto, a la distancia del punto medio.
    const Point2D mid{(a.x + b.x) / 2.0, (a.y + b.y) / 2.0};
    const Point2D cv{b.x - a.x, b.y - a.y};
    Point2D bv{cv.y, -cv.x};
    const double n = std::sqrt(bv.x * bv.x + bv.y * bv.y);
    bv = {bv.x / n, bv.y / n};
    double t = distance(c, mid);
    const double side = (c.x - a.x) * cv.y - (c.y - a.y) * cv.x;
    t *= side > 0 ? 1.0 : (side < 0 ? -1.0 : 0.0);
    return Point2D{mid.x + bv.x * t, mid.y + bv.y * t};
}

std::optional<Shape> arc(ArcMode mode, const Point2D& a, const Point2D& b, const Point2D& c,
                         ArcDirection direction, int steps) {
    Point2D center, p1, p2;
    double radius = 0.0;
    if (mode == ArcMode::C12) {
        center = a;
        p1 = b;
        p2 = c;
        radius = distance(center, p1);
    } else if (mode == ArcMode::P132) {
        const auto circ = threePointCircle(a, c, b);
        if (!circ) return std::nullopt;
        center = circ->center;
        radius = circ->radius;
        direction = circ->t > 0 ? ArcDirection::Clockwise : ArcDirection::CounterClockwise;
        p1 = a;
        p2 = b;
    } else {
        center = *arcCenter(mode, a, b, c);
        p1 = a;
        p2 = b;
        radius = std::sqrt((center.x - p1.x) * (center.x - p1.x) + (center.y - p1.y) * (center.y - p1.y));
    }
    const double start = std::atan2(p1.y - center.y, p1.x - center.x);
    const double stop = std::atan2(p2.y - center.y, p2.x - center.x);
    return Shape::lineString(flatcam::arc(center, radius, start, stop, direction, steps));
}

Shape rectangle(const Point2D& p1, const Point2D& p2) {
    return Shape::polygon({p1, {p2.x, p1.y}, p2, {p1.x, p2.y}});
}

}  // namespace geodraw

// --------------------------------------------------------------------------------------------------- modelo

EditorShapePtr GeoEditorModel::add(const Shape& g) {
    EditorShapePtr s = makeEditorShape(g);
    storage.insert(s);
    return s;
}

void GeoEditorModel::remove(const EditorShapePtr& s) {
    storage.remove(s);
    selected.erase(std::remove(selected.begin(), selected.end(), s), selected.end());
}

void GeoEditorModel::deleteSelected() {
    const auto tmp = selected;
    for (const auto& s : tmp) remove(s);
    selected.clear();
}

void GeoEditorModel::load(const ShapeList& geometry, int orientVal) {
    ShapeList flat;
    for (const Shape& g : geometry) flattenOriented(g, orientVal, flat);
    for (const Shape& s : flat) {
        if (s.type() == ShapeType::Polygon && !s.isEmpty()) {
            add(s.exterior());
            for (const Shape& in : s.interiors()) add(in);
        } else {
            add(s);
        }
    }
}

ShapeList GeoEditorModel::result() const {
    ShapeList out;
    for (const auto& s : storage.objects())
        out.push_back(s->geo.type() == ShapeType::MultiLineString ? Shape::lineMerge(s->geo.parts()) : s->geo);
    return out;
}

std::vector<EditorShapePtr> GeoEditorModel::complete(const ShapeList& geometry, bool isList, bool climb) {
    // geom: la lista (isList) o la figura sola.
    ShapeList geom = geometry;
    bool list = isList;
    bool multiPolygonOfList = false;  // geom = MultiPolygon(pl)
    if (climb) {
        const bool iterable = list || (!geom.empty() && isMulti(geom.front()));
        if (iterable) {
            const ShapeList items = list ? geom : geom.front().parts();
            ShapeList pl;
            bool hasLine = false;
            for (const Shape& p : items) {
                const ShapeType t = p.type();
                if (t == ShapeType::Polygon) pl.push_back(reversedPolygon(p));
                else if (t == ShapeType::LinearRing) pl.push_back(Shape::polygon(reversed(p.coordinates())));
                else if (t == ShapeType::LineString) {
                    pl.push_back(Shape::lineString(reversed(p.coordinates())));
                    hasLine = true;
                }
            }
            if (!hasLine) {
                // MultiPolygon(pl): descarta las vacías; con una línea falla (TypeError) y geom queda como estaba.
                ShapeList mp;
                for (const Shape& p : pl)
                    if (!p.isEmpty()) mp.push_back(p);
                geom = mp;
                list = true;
                multiPolygonOfList = true;
            }
        } else if (!geom.empty()) {
            const Shape& g = geom.front();
            if (g.type() == ShapeType::Polygon) geom = {reversedPolygon(g)};
            else if (g.type() == ShapeType::LinearRing) geom = {Shape::polygon(reversed(g.coordinates()))};
            else if (g.type() == ShapeType::LineString) geom = {Shape::lineString(reversed(g.coordinates()))};
        }
    }
    (void)multiPolygonOfList;
    // for geo in geom: una lista, un MultiPolygon o una figura múltiple se recorren.
    ShapeList items;
    if (list) items = geom;
    else if (!geom.empty() && isMulti(geom.front())) items = geom.front().parts();
    else items = geom;
    std::vector<EditorShapePtr> out;
    for (const Shape& g : items) out.push_back(add(g));
    return out;
}

void GeoEditorModel::clickSelect(const Point2D& p, bool toggle) {
    const auto n = storage.nearest(p);
    if (!n) {
        selected.clear();
        return;
    }
    const EditorShapePtr obj = n->second;
    if (toggle) {
        auto it = std::find(selected.begin(), selected.end(), obj);
        if (it != selected.end()) selected.erase(it);
        else selected.push_back(obj);
    } else {
        selected = {obj};
    }
}

void GeoEditorModel::areaSelect(const Point2D& a, const Point2D& b, bool contains, bool toggle) {
    const Shape poly = Shape::polygon({a, {b.x, a.y}, b, {a.x, b.y}});
    std::vector<EditorShapePtr> list;
    for (const auto& o : storage.objects())
        if (contains ? poly.contains(o->geo) : poly.intersects(o->geo)) list.push_back(o);
    if (toggle) {
        for (const auto& o : list) {
            auto it = std::find(selected.begin(), selected.end(), o);
            if (it != selected.end()) selected.erase(it);
            else selected.push_back(o);
        }
    } else {
        selected = list;
    }
}

void GeoEditorModel::unite() {
    ShapeList geos;
    for (const auto& s : selected) geos.push_back(s->geo);
    const Shape r = Shape::unaryUnion(geos);
    deleteSelected();
    add(r);
}

bool GeoEditorModel::intersection() {
    if (selected.empty()) return false;
    const Shape inter = selected.front()->geo;
    ShapeList results;
    std::vector<EditorShapePtr> intact;
    for (std::size_t i = 1; i < selected.size(); ++i) {
        if (inter.intersects(selected[i]->geo)) results.push_back(inter.intersection(selected[i]->geo));
        else intact.push_back(selected[i]);
    }
    if (!results.empty()) {
        const auto sel = selected;
        for (const auto& s : sel)
            if (std::find(intact.begin(), intact.end(), s) == intact.end()) remove(s);
        for (const Shape& g : results) add(g);
    }
    selected.clear();
    return true;
}

void GeoEditorModel::subtract() {
    if (selected.empty()) return;  // IndexError: solo queda en el registro
    ShapeList tools;
    for (std::size_t i = 1; i < selected.size(); ++i) tools.push_back(selected[i]->geo);
    const Shape toolgeo = Shape::unaryUnion(tools).buffer(0.0000001);
    const Shape target = selected.front()->geo.buffer(0.0000001);
    const Shape r = target.difference(toolgeo);
    deleteSelected();
    add(r);
}

bool GeoEditorModel::cutpath() {
    if (selected.empty()) return false;
    ShapeList tools;
    for (std::size_t i = 1; i < selected.size(); ++i) tools.push_back(selected[i]->geo);
    const Shape toolgeo = Shape::unaryUnion(tools);
    const EditorShapePtr target = selected.front();
    const ShapeType t = target->geo.type();
    if (t == ShapeType::Polygon) {
        add(target->geo.exterior().difference(toolgeo));
        for (const Shape& in : target->geo.interiors()) add(in.difference(toolgeo));
    } else if (t == ShapeType::LineString || t == ShapeType::LinearRing) {
        add(target->geo.difference(toolgeo));
    } else if (t == ShapeType::MultiLineString) {
        for (const Shape& l : target->geo.parts()) add(l.difference(toolgeo));
    } else {
        return false;
    }
    remove(target);
    return true;
}

std::optional<ShapeList> GeoEditorModel::explode() {
    ShapeList lines;
    for (const auto& s : selected) {
        if (s->geo.type() != ShapeType::Polygon) return std::nullopt;  // geo.exterior: AttributeError
        const auto ext = s->geo.exterior().coordinates();
        for (std::size_t c = 0; c + 1 < ext.size(); ++c) lines.push_back(Shape::lineString({ext[c], ext[c + 1]}));
        // El recorrido de los agujeros llega hasta c + 1 == len: IndexError.
        if (!s->geo.interiors().empty()) return std::nullopt;
    }
    const auto sel = selected;
    for (const auto& s : sel) remove(s);
    return lines;
}

void GeoEditorModel::erase(const Point2D& origin, const Point2D& destination) {
    const double dx = destination.x - origin.x, dy = destination.y - origin.y;
    ShapeList er;
    for (const auto& s : selected) {
        const Shape b = s->geo.translate(dx, dy).buffer(0.0000001);
        if (b.type() != ShapeType::Polygon) throw GeometryError("erase: exterior de una figura múltiple");
        er.push_back(Shape::polygon(b.exterior().coordinates()));
    }
    const Shape eraser = Shape::unaryUnion(er);
    for (const auto& o : storage.objects())
        if (eraser.intersects(o->geo)) o->geo = o->geo.difference(eraser);
}

std::optional<std::string> GeoEditorModel::paint(double tooldia, double overlap, double margin, bool connect,
                                                 bool contour, PaintMethod method, int steps) {
    ShapeList results;
    for (const auto& s : selected) {
        ShapeList flat;
        if (isMulti(s->geo)) flat = s->geo.parts();
        else flat = {s->geo};
        ShapeList local;
        for (const Shape& g : flat) {
            try {
                const Shape poly = g.type() == ShapeType::Polygon ? g : Shape::polygon(g.coordinates());
                const Shape buf = poly.buffer(-margin);
                std::optional<std::vector<EndpointStorage::Line>> cp;
                if (method == PaintMethod::Seed)
                    cp = PolygonClearing::clearPolygonSeed(buf, tooldia, steps, overlap, connect, contour);
                else if (method == PaintMethod::Lines)
                    cp = PolygonClearing::clearPolygonLines(buf, tooldia, steps, overlap, connect, contour);
                else
                    cp = PolygonClearing::clearPolygon(buf, tooldia, steps, overlap, connect);
                if (cp)
                    for (const auto& l : *cp)
                        local.push_back(l.linearRing ? Shape::linearRing(l.points) : Shape::lineString(l.points));
            } catch (const GeometryError& e) {
                return std::string(e.what());
            }
            results.push_back(Shape::unaryUnion(local));
        }
    }
    for (const Shape& r : results) add(r);
    return std::nullopt;
}

BoundingBox GeoEditorModel::selectionBounds() const {
    BoundingBox b{std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
                  -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};
    for (const auto& s : selected) {
        const auto sb = s->geo.bounds();
        if (!sb) continue;
        b.xmin = std::min(b.xmin, sb->xmin);
        b.ymin = std::min(b.ymin, sb->ymin);
        b.xmax = std::max(b.xmax, sb->xmax);
        b.ymax = std::max(b.ymax, sb->ymax);
    }
    return b;
}

void GeoEditorModel::rotate(double angle, const Point2D& p) {
    for (const auto& s : selected) s->geo = s->geo.rotate(angle, p);
}

void GeoEditorModel::mirror(char axis, const Point2D& p) {
    const double xs = axis == 'X' ? 1.0 : -1.0, ys = axis == 'X' ? -1.0 : 1.0;
    for (const auto& s : selected) s->geo = s->geo.scale(xs, ys, p);
}

void GeoEditorModel::skew(double ax, double ay, const Point2D& p) {
    for (const auto& s : selected) s->geo = s->geo.skew(ax, ay, p);
}

void GeoEditorModel::scale(double fx, double fy, const Point2D& p) {
    for (const auto& s : selected) s->geo = s->geo.scale(fx, fy, p);
}

void GeoEditorModel::offset(double dx, double dy) {
    for (const auto& s : selected) s->geo = s->geo.translate(dx, dy);
}

void GeoEditorModel::buffer(double value, int join, bool factor) {
    for (const auto& s : selected)
        s->geo = factor ? s->geo.scale(value, value) : s->geo.buffer(value, 32, join == 1 ? JoinStyle::Round : JoinStyle::Mitre);
}

}  // namespace flatcam
