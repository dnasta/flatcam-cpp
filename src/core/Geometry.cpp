#include "Geometry.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "GeometryError.h"
#include "Logger.h"

namespace flatcam {

namespace {

/// `true` si la figura es una colección que se debe expandir al iterar (Multi* o GeometryCollection).
bool isMulti(const Shape& s) {
    return static_cast<int>(s.type()) >= static_cast<int>(ShapeType::MultiPoint);
}

/// Expande recursivamente @p s en figuras simples, siguiendo la semántica de `Geometry.flatten`.
void flattenInto(const Shape& s, ShapeList& out, bool pathOnly) {
    if (isMulti(s)) {
        for (const Shape& p : s.parts()) flattenInto(p, out, pathOnly);
    } else if (pathOnly && s.type() == ShapeType::Polygon) {
        out.push_back(s.exterior());
        for (const Shape& r : s.interiors()) out.push_back(r);
    } else {
        out.push_back(s);
    }
}

/// Caja delimitadora de una lista, ignorando las figuras vacías.
std::optional<BoundingBox> boundsOfList(const ShapeList& list) {
    std::optional<BoundingBox> acc;
    for (const Shape& s : list) {
        const auto b = s.bounds();
        if (!b) continue;
        acc = acc ? acc->merged(*b) : *b;
    }
    return acc;
}

/// Busca recursivamente la figura simple que contiene a @p pt.
std::optional<Shape> findIn(const Shape& s, const Shape& pt) {
    if (isMulti(s)) {
        for (const Shape& p : s.parts()) {
            if (auto r = findIn(p, pt)) return r;
        }
        return std::nullopt;
    }
    Shape candidate = s;
    if (s.type() == ShapeType::LinearRing) candidate = Shape::polygon(s.coordinates());
    if (candidate.contains(pt)) return candidate;
    return std::nullopt;
}

void collectInteriors(const Shape& s, ShapeList& out) {
    if (isMulti(s)) {
        for (const Shape& p : s.parts()) collectInteriors(p, out);
    } else if (s.type() == ShapeType::Polygon) {
        for (const Shape& r : s.interiors()) out.push_back(r);
    }
}

void collectExteriors(const Shape& s, ShapeList& out) {
    if (isMulti(s)) {
        for (const Shape& p : s.parts()) collectExteriors(p, out);
    } else if (s.type() == ShapeType::Polygon) {
        out.push_back(s.exterior());
    }
}

}  // namespace

Geometry::Geometry(int stepsPerCircle, Units units) : units_(units), stepsPerCircle_(stepsPerCircle) {}

void Geometry::addToTool(const Shape& shape, std::optional<int> tool) {
    const int key = (tool && tools_.count(*tool)) ? *tool : 1;
    tools_[key].solidGeometry.push_back(shape);
}

bool Geometry::addCircle(const Point2D& origin, double radius, std::optional<int> tool) {
    const Shape circle = Shape::circle(origin, radius, stepsPerCircle_);
    if (!circle.isValid()) return false;
    solid_.push_back(circle);
    addToTool(circle, tool);
    return true;
}

bool Geometry::addPolygon(const std::vector<Point2D>& points, std::optional<int> tool) {
    Shape poly;
    try {
        poly = Shape::polygon(points);
    } catch (const GeometryError&) {
        return false;
    }
    if (!poly.isValid()) return false;
    solid_.push_back(poly);
    addToTool(poly, tool);
    return true;
}

bool Geometry::addPolyline(const std::vector<Point2D>& points, std::optional<int> tool) {
    Shape line;
    try {
        line = Shape::lineString(points);
    } catch (const GeometryError&) {
        return false;
    }
    if (!line.isValid()) return false;
    solid_.push_back(line);
    addToTool(line, tool);
    return true;
}

void Geometry::subtractPolygon(const std::vector<Point2D>& points) {
    const ShapeList paths = flatten(nullptr, true, true);
    Logger::debug(std::to_string(paths.size()) + " paths");

    const Shape tool = Shape::unaryUnion({Shape::polygon(points)});
    ShapeList diffs;
    for (const Shape& target : paths) {
        const ShapeType t = target.type();
        if (t == ShapeType::LineString || t == ShapeType::LinearRing || t == ShapeType::MultiLineString) {
            diffs.push_back(target.difference(tool));
        } else {
            Logger::warning("Not implemented.");
        }
    }
    solid_ = {Shape::unaryUnion(diffs)};
}

BoundingBox Geometry::bounds(bool flatten) {
    if (multigeo_) {
        std::optional<BoundingBox> acc;
        for (auto& [id, entry] : tools_) {
            ShapeList working = entry.solidGeometry;
            if (flatten) {
                this->flatten(&working, true, false);
                working = flat_;
            }
            if (const auto b = boundsOfList(working)) acc = acc ? acc->merged(*b) : *b;
        }
        return acc.value_or(BoundingBox{});
    }

    if (flatten) {
        this->flatten(nullptr, true, false);
        solid_ = flat_;
    }
    return boundsOfList(solid_).value_or(BoundingBox{});
}

std::pair<double, double> Geometry::size() {
    if (solid_.empty()) {
        Logger::warning("Solid_geometry not computed yet.");
        return {0.0, 0.0};
    }
    const BoundingBox b = bounds();
    return {b.width(), b.height()};
}

std::optional<Shape> Geometry::findPolygon(const Point2D& point, const ShapeList* geoset) const {
    const ShapeList& list = geoset ? *geoset : solid_;
    const Shape pt = Shape::point(point);
    for (const Shape& s : list) {
        if (auto r = findIn(s, pt)) return r;
    }
    return std::nullopt;
}

ShapeList Geometry::getInteriors(const ShapeList* geometry) const {
    ShapeList out;
    for (const Shape& s : (geometry ? *geometry : solid_)) collectInteriors(s, out);
    return out;
}

ShapeList Geometry::getExteriors(const ShapeList* geometry) const {
    ShapeList out;
    for (const Shape& s : (geometry ? *geometry : solid_)) collectExteriors(s, out);
    return out;
}

const ShapeList& Geometry::flatten(const ShapeList* geometry, bool reset, bool pathOnly) {
    // Se copia la entrada porque puede ser la propia lista sólida o la de aplanado.
    const ShapeList source = geometry ? *geometry : solid_;
    if (reset) flat_.clear();
    for (const Shape& s : source) flattenInto(s, flat_, pathOnly);
    return flat_;
}

void Geometry::unionAll() { solid_ = {Shape::unaryUnion(solid_)}; }

void Geometry::scale(double, double, const std::optional<Point2D>&) {}

void Geometry::offset(const Point2D&) {}

double Geometry::convertUnits(Units objUnits) {
    if (objUnits == units_) return 1.0;

    const double factor = (objUnits == Units::Millimeters) ? 25.4 : 1.0 / 25.4;
    units_ = objUnits;
    scale(factor, factor);
    fileUnitsFactor_ = factor;
    return factor;
}

template <typename Fn>
void Geometry::applyToAll(Fn fn) {
    if (multigeo_) {
        for (auto& [id, entry] : tools_) {
            for (Shape& s : entry.solidGeometry) s = fn(s);
        }
    } else {
        for (Shape& s : solid_) s = fn(s);
    }
}

void Geometry::mirror(Axis axis, const Point2D& point) {
    const double xs = (axis == Axis::X) ? 1.0 : -1.0;
    const double ys = (axis == Axis::X) ? -1.0 : 1.0;
    applyToAll([&](const Shape& s) { return s.scale(xs, ys, point); });
}

void Geometry::rotate(double angle, const Point2D& point) {
    applyToAll([&](const Shape& s) { return s.rotate(angle, point); });
}

void Geometry::skew(double angleX, double angleY, const Point2D& point) {
    applyToAll([&](const Shape& s) { return s.skew(angleX, angleY, point); });
}

void Geometry::buffer(double distance, JoinStyle join, bool factor) {
    if (distance == 0.0) return;

    auto op = [&](const Shape& s) {
        return factor ? s.scale(distance, distance) : s.buffer(distance, stepsPerCircle_, join);
    };

    // Igual que en camlib: en modo multigeo se procesan las herramientas y, además, siempre la
    // geometría sólida.
    if (multigeo_) {
        for (auto& [id, entry] : tools_) {
            for (Shape& s : entry.solidGeometry) s = op(s);
        }
    }
    for (Shape& s : solid_) s = op(s);
}

}  // namespace flatcam
