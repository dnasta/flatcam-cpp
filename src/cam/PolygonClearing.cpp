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
        if (walkCut.within(boundary) && walkPath.length() < maxWalk) {
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

}  // namespace flatcam
