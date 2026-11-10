#include "tools/AlignObjects.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace flatcam {

namespace {

/// float('%.*f' % (decimals, v)).
double roundPy(double v, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return std::strtod(buf, nullptr);
}

Point2D roundedCentroid(const Shape& s, int decimals) {
    const Point2D c = s.centroid();
    return {roundPy(c.x, decimals), roundPy(c.y, decimals)};
}

}  // namespace

std::vector<Point2D> alignPickGerber(const Gerber& g, const Point2D& click, int decimals) {
    std::vector<Point2D> out;
    const Shape pt = Shape::point(click);
    for (const std::string& id : g.apertureOrder()) {
        const auto it = g.apertures().find(id);
        if (it == g.apertures().end()) continue;
        for (const ApertureGeometry& el : it->second.geometry)
            if (el.solid && pt.within(*el.solid) && el.follow && el.follow->type() == ShapeType::Point)
                out.push_back(roundedCentroid(*el.solid, decimals));
    }
    return out;
}

std::vector<Point2D> alignPickExcellon(const Excellon& e, const Point2D& click, int decimals) {
    std::vector<Point2D> out;
    const Shape pt = Shape::point(click);
    for (const auto& [n, tool] : e.drillTools())
        for (const Shape& geo : tool.solidGeometry)
            if (pt.within(geo)) out.push_back(roundedCentroid(geo, decimals));
    return out;
}

AlignMove alignMove(const std::vector<Point2D>& p, int decimals) {
    AlignMove m;
    const double dx = p[1].x - p[0].x;
    const double dy = p[1].y - p[0].y;
    m.offset = {dx, dy};
    m.origin = p[1];
    if (p.size() < 4) return m;
    const Point2D start{p[2].x + dx, p[2].y + dy};
    const Point2D dest = p[3];
    const double dxd = dest.x - m.origin.x;
    const double dyd = dest.y - m.origin.y;
    const double dxs = start.x - m.origin.x;
    const double dys = start.y - m.origin.y;
    const double tol = std::pow(10.0, -decimals);
    const bool notNeeded = std::abs(start.x - dest.x) <= tol || std::abs(start.y - dest.y) <= tol;
    if (notNeeded) return m;
    if (dxd == 0.0 || dxs == 0.0) {
        m.fails = true;  // ZeroDivisionError en dyd / dxd o dys / dxs
        return m;
    }
    const double radToDeg = 180.0 / M_PI;
    m.angle = std::atan(dyd / dxd) * radToDeg - std::atan(dys / dxs) * radToDeg;
    return m;
}

}  // namespace flatcam
