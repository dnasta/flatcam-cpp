#include "tools/OptimalDistance.h"

#include <cstdio>

#include "cam/PyNumber.h"

namespace flatcam {

namespace {

double decFormat(double v, int dec) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", dec, v);
    return std::stod(buf);
}

}  // namespace

OptimalResult findMinimumDistances(const Gerber& g, int decimals) {
    OptimalResult r;
    ShapeList total;
    for (const std::string& id : g.apertureOrder())
        for (const ApertureGeometry& el : g.apertures().at(id).geometry)
            if (el.solid && el.solid->isValid()) total.push_back(*el.solid);
    const Shape u = Shape::collection(total).buffer(0);
    const ShapeList polys = u.parts();
    if (polys.empty()) {  // un Polygon no se puede recorrer
        r.onePolygon = true;
        return r;
    }
    for (std::size_t i = 0; i < polys.size(); ++i) {
        for (std::size_t j = i + 1; j < polys.size(); ++j) {
            const double d = decFormat(polys[i].distance(polys[j]), decimals);
            const auto np = polys[i].nearestPoints(polys[j]);
            if (!np) continue;
            const Point2D a{decFormat(np->first.x, decimals), decFormat(np->first.y, decimals)};
            const Point2D b{decFormat(np->second.x, decimals), decFormat(np->second.y, decimals)};
            r.distances[d].push_back({a, b});
        }
    }
    return r;
}

std::string pyPointPair(const std::pair<Point2D, Point2D>& p) {
    auto pt = [](const Point2D& q) { return "(" + pyFloatRepr(q.x) + ", " + pyFloatRepr(q.y) + ")"; };
    return pt(p.first) + ", " + pt(p.second);
}

}  // namespace flatcam
