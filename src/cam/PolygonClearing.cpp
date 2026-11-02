#include "cam/PolygonClearing.h"

#include <algorithm>

namespace flatcam {

namespace {

bool isMulti(ShapeType t) { return static_cast<int>(t) >= static_cast<int>(ShapeType::MultiPoint); }

/// Inserta el contorno exterior y los agujeros de cada polígono (`for p in current: insert(p.exterior)...`).
void insertRings(EndpointStorage& storage, const Shape& current) {
    auto insertPolygon = [&](const Shape& p) {
        storage.insert(p.exterior().coordinates(), true);
        for (const Shape& hole : p.interiors()) storage.insert(hole.coordinates(), true);
    };
    if (isMulti(current.type())) {
        for (const Shape& p : current.parts()) {
            if (p.type() == ShapeType::Polygon && !p.isEmpty()) insertPolygon(p);
        }
    } else if (current.type() == ShapeType::Polygon && !current.isEmpty()) {
        insertPolygon(current);
    }
}

}  // namespace

Shape PolygonClearing::toShape(const EndpointStorage::Line& line) {
    return line.linearRing ? Shape::linearRing(line.points) : Shape::lineString(line.points);
}

std::optional<std::vector<EndpointStorage::Line>> PolygonClearing::paintConnect(EndpointStorage& storage,
                                                                                const Shape& boundary, double tooldia,
                                                                                int stepsPerCircle) {
    const double maxWalk = 10 * tooldia;
    EndpointStorage optimized;
    const PreparedShape preparedBoundary(boundary);  // la misma figura se consulta miles de veces
    Point2D currentPt{0, 0};

    auto first = storage.nearest(currentPt);
    if (!first) return std::nullopt;
    storage.remove(first->index);
    std::vector<Point2D> geo = storage.line(first->index).points;  // `LineString(geo)`: deja de ser un anillo
    currentPt = geo.back();

    while (true) {
        const auto found = storage.nearest(currentPt);
        if (!found) break;  // StopIteration: no queda nada en el almacén
        storage.remove(found->index);
        std::vector<Point2D> candidate = storage.line(found->index).points;
        const Point2D pt = found->point;
        // Si el extremo más cercano es el último (y no el primero), se invierte la línea.
        if (!(pt == candidate.front()) && pt == candidate.back()) std::reverse(candidate.begin(), candidate.end());

        const Shape walkPath = Shape::lineString({currentPt, pt});
        const Shape walkCut = walkPath.buffer(tooldia / 2, stepsPerCircle);
        if (preparedBoundary.contains(walkCut) && walkPath.length() < maxWalk) {
            geo.insert(geo.end(), candidate.begin(), candidate.end());
        } else {
            optimized.insert(geo, false);
            geo = std::move(candidate);
        }
        currentPt = geo.back();
    }
    optimized.insert(geo, false);
    return optimized.objects();
}

std::optional<std::vector<EndpointStorage::Line>> PolygonClearing::clearPolygon(const Shape& polygon, double tooldia,
                                                                                 int stepsPerCircle, double overlap,
                                                                                 bool connect) {
    EndpointStorage geoms;
    Shape current = polygon.buffer(-tooldia / 1.999999, stepsPerCircle);
    if (current.area() == 0) return std::nullopt;
    insertRings(geoms, current);

    while (true) {
        current = current.buffer(-tooldia * (1 - overlap), stepsPerCircle);
        if (current.area() > 0) insertRings(geoms, current);
        else break;
    }
    if (connect) return paintConnect(geoms, polygon, tooldia, stepsPerCircle);
    return geoms.objects();
}

namespace {

/// Inserta una figura del resultado de una intersección: líneas sueltas o cada parte de una figura múltiple
/// (`for p in path: geoms.insert(p)`; si no es iterable, la figura entera).
void insertParts(EndpointStorage& storage, const Shape& s) {
    if (s.isEmpty()) return;
    if (isMulti(s.type())) {
        for (const Shape& p : s.parts()) insertParts(storage, p);
        return;
    }
    if (s.type() == ShapeType::Polygon) return;  // no aparece al cortar una línea con un polígono
    storage.insert(s.coordinates(), s.type() == ShapeType::LinearRing);
}

}  // namespace

std::optional<std::vector<EndpointStorage::Line>> PolygonClearing::clearPolygonSeed(const Shape& polygon, double tooldia,
                                                                                     int stepsPerCircle, double overlap,
                                                                                     bool connect, bool contour,
                                                                                     std::optional<Point2D> seed) {
    EndpointStorage geoms;
    double radius = tooldia / 2 * (1 - overlap);
    const Shape pathMargin = polygon.buffer(-tooldia / 2, stepsPerCircle);
    if (pathMargin.isEmpty()) return std::nullopt;
    const Point2D seedPoint = seed ? *seed : pathMargin.representativePoint();
    while (true) {
        const Shape ring = Shape::point(seedPoint).buffer(radius, stepsPerCircle).exterior();
        const Shape path = ring.intersection(pathMargin);
        if (path.isEmpty()) break;
        insertParts(geoms, path);
        radius += tooldia * (1 - overlap);
    }
    if (contour) {
        // Primero todos los contornos exteriores y después todos los agujeros.
        std::vector<Shape> polys = isMulti(pathMargin.type()) ? pathMargin.parts() : std::vector<Shape>{pathMargin};
        for (const Shape& x : polys)
            if (x.type() == ShapeType::Polygon && !x.isEmpty()) geoms.insert(x.exterior().coordinates(), true);
        for (const Shape& x : polys)
            if (x.type() == ShapeType::Polygon && !x.isEmpty())
                for (const Shape& h : x.interiors()) geoms.insert(h.coordinates(), true);
    }
    if (connect) {
        if (auto c = paintConnect(geoms, polygon, tooldia, stepsPerCircle)) return c;
    }
    return geoms.objects();
}

std::optional<std::vector<EndpointStorage::Line>> PolygonClearing::clearPolygonLines(const Shape& polygon,
                                                                                      double tooldia, int stepsPerCircle,
                                                                                      double overlap, bool connect,
                                                                                      bool contour) {
    if (polygon.type() != ShapeType::Polygon) return std::nullopt;
    EndpointStorage geoms;
    std::vector<Shape> trimmed;
    const auto b = polygon.bounds();
    if (!b) return std::nullopt;
    const double left = b->xmin, bot = b->ymin, right = b->xmax, top = b->ymax;
    const Shape margin = polygon.buffer(-tooldia / 1.99999999, stepsPerCircle);
    auto cut = [&](const Point2D& a, const Point2D& c) { return Shape::lineString({a, c}).intersection(margin); };
    if (std::abs(left - right) >= std::abs(top - bot)) {
        double y = top - tooldia / 1.99999999;
        while (y > bot + tooldia / 1.999999999) {
            trimmed.push_back(cut({left, y}, {right, y}));
            y -= tooldia * (1 - overlap);
        }
        y = bot + tooldia / 2;
        trimmed.push_back(cut({left, y}, {right, y}));
    } else {
        double x = left + tooldia / 1.99999999;
        while (x < right - tooldia / 1.999999999) {
            trimmed.push_back(cut({x, top}, {x, bot}));
            x += tooldia * (1 - overlap);
        }
        x = right + tooldia / 2;  // así en el original (queda fuera del polígono)
        trimmed.push_back(cut({x, top}, {x, bot}));
    }
    const Shape lines = Shape::unaryUnion(trimmed);
    if (isMulti(lines.type())) {
        for (const Shape& l : lines.parts())
            if ((l.type() == ShapeType::LineString || l.type() == ShapeType::LinearRing) && !l.isEmpty())
                geoms.insert(l.coordinates(), l.type() == ShapeType::LinearRing);
    } else if (!lines.isEmpty()) {
        geoms.insert(lines.coordinates(), lines.type() == ShapeType::LinearRing);
    }
    if (contour) insertRings(geoms, margin);
    if (connect) {
        if (auto c = paintConnect(geoms, polygon, tooldia, stepsPerCircle)) return c;
    }
    return geoms.objects();
}

std::optional<std::vector<EndpointStorage::Line>> PolygonClearing::clear(const Shape& polygon, double tooldia,
                                                                          int stepsPerCircle, double overlap,
                                                                          bool connect, bool contour, Method method) {
    using Result = std::optional<std::vector<EndpointStorage::Line>>;
    auto ok = [](const Result& r) { return r && !r->empty(); };  // `cp and cp.objects`
    Result r;
    switch (method) {
        case Method::Standard: r = clearPolygon(polygon, tooldia, stepsPerCircle, overlap, connect); break;
        case Method::Seed: r = clearPolygonSeed(polygon, tooldia, stepsPerCircle, overlap, connect, contour); break;
        case Method::Lines: r = clearPolygonLines(polygon, tooldia, stepsPerCircle, overlap, connect, contour); break;
        case Method::Combo:
            r = clearPolygonLines(polygon, tooldia, stepsPerCircle, overlap, connect, contour);
            if (!ok(r)) r = clearPolygonSeed(polygon, tooldia, stepsPerCircle, overlap, connect, contour);
            if (!ok(r)) r = clearPolygon(polygon, tooldia, stepsPerCircle, overlap, connect);
            break;
    }
    if (!ok(r)) return std::nullopt;
    return r;
}

}  // namespace flatcam
