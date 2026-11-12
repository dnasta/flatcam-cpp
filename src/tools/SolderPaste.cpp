#include "tools/SolderPaste.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "cam/EndpointStorage.h"
#include "core/ArcMath.h"

namespace flatcam {

namespace {

double roundPy(double v, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return std::strtod(buf, nullptr);
}

bool isMulti(const Shape& s) {
    const ShapeType t = s.type();
    return t == ShapeType::MultiPoint || t == ShapeType::MultiLineString || t == ShapeType::MultiPolygon ||
           t == ShapeType::GeometryCollection;
}

/// flatten(pathonly=True): los polígonos dan su exterior.
void flatten(const Shape& g, ShapeList& out) {
    if (isMulti(g)) {
        for (const Shape& p : g.parts()) flatten(p, out);
        return;
    }
    out.push_back(g.type() == ShapeType::Polygon ? g.exterior() : g);
}

}  // namespace

PasteGeometry solderPasteGeometry(const ShapeList& solid, const std::vector<std::pair<int, double>>& nozzles,
                                  int decimals, bool mm) {
    PasteGeometry r;
    std::vector<double> sorted;
    for (const auto& [uid, d] : nozzles)
        if (d > 0) sorted.push_back(roundPy(d, decimals));
    std::sort(sorted.rbegin(), sorted.rend());
    if (sorted.empty()) {
        r.error = "No Nozzle tools in the tool table.";
        return r;
    }
    ShapeList work;
    for (const Shape& s : solid) flatten(s, work);
    Shape geo = Shape::fromWkt("LINESTRING EMPTY");  // geo = LineString()
    for (double tool : sorted) {
        const double offset = tool / 2;
        int uid = 1;
        for (const auto& [u, d] : nozzles)
            if (roundPy(d, decimals) == tool) {
                uid = u;
                break;
            }
        PasteTool pt{uid, tool, {}};
        ShapeList rest;
        for (const Shape& g : work) {
            const Shape poly = Shape::polygon(g.coordinates());
            const BoundingBox b = poly.bounds().value_or(BoundingBox{});
            const Shape d1 = Shape::lineString({{b.xmin, b.ymin}, {b.xmax, b.ymax}}).intersection(poly);
            const Shape d2 = Shape::lineString({{b.xmin, b.ymax}, {b.xmax, b.ymin}}).intersection(poly);
            const int nd = mm ? 1 : 2;
            const double r1 = roundPy(d1.length(), nd), r2 = roundPy(d2.length(), nd);
            if (r1 == r2) {
                const double length = distance({b.xmin, b.ymin}, {b.xmax, b.ymin});
                const double h = distance({b.xmin, b.ymin}, {b.xmin, b.ymax});
                // Si la boquilla no entra, geo queda como estaba (la del pad anterior), como en el original.
                if (!(offset >= length / 2 || offset >= h / 2)) {
                    if (length > h) geo = Shape::lineString({{b.xmin, b.ymin + h / 2}, {b.xmin + length, b.ymin + h / 2}});
                    else geo = Shape::lineString({{b.xmin + length / 2, b.ymin}, {b.xmin + length / 2, b.ymin + h}});
                }
            } else if (r1 > r2) {
                geo = d1;
            } else {
                geo = d2;
            }
            const Shape cut = geo.isEmpty() ? geo : geo.intersection(poly.buffer(-offset));
            if (!cut.isEmpty()) pt.paths.push_back(cut);
            else rest.push_back(g);
        }
        r.tools.push_back(pt);
        work = rest;
        if (work.empty()) {
            bool allEmpty = true;
            for (const PasteTool& t : r.tools)
                if (!t.paths.empty()) allEmpty = false;
            if (allEmpty) {
                r.error = "Cancelled. Empty file, it has no geometry";
                r.tools.clear();
            }
            return r;
        }
    }
    r.error = "Some or all pads have no solder due of inadequate nozzle diameters...";
    return r;
}

std::string solderPasteGcode(const ShapeList& paths, PreprocessorParams p, bool g90) {
    const SolderPastePreprocessor pp;
    ShapeList flat;
    for (const Shape& s : paths) flatten(s, flat);
    // FlatCAMRTreeStorage con los extremos de cada trazo (EndpointStorage elige el primero ante un empate).
    EndpointStorage storage;
    std::vector<Shape> geos;
    for (const Shape& g : flat) {
        if (g.isEmpty()) continue;
        storage.insert(g.coordinates(), false);
        geos.push_back(g);
    }
    std::string g;
    auto line = [&](const std::string& s) { g += s + "\n"; };
    line(pp.startCode(p));
    line(pp.spindleOffCode(p));
    line(pp.toolchangeCode(p));
    Point2D current{0, 0};
    while (const auto n = storage.nearest(current)) {
        storage.remove(n->index);
        std::vector<Point2D> path = geos[n->index].coordinates();
        if (!(n->point == path.front()) && n->point == path.back()) std::reverse(path.begin(), path.end());
        const bool isPoint = geos[n->index].type() == ShapeType::Point;
        const double fx = g90 ? path[0].x : path[0].x - current.x, fy = g90 ? path[0].y : path[0].y - current.y;
        p.x = fx;
        p.y = fy;
        if (!isPoint) {
            line(pp.rapidCode(p));
            line(pp.zFeedrateCode(p));
            line(pp.downZStartCode(p));
            line(pp.spindleFwdCode(p));
            line(pp.dwellFwdCode(p));
            line(pp.feedrateZDispenseCode(p));
            line(pp.liftZDispenseCode(p));
            line(pp.feedrateXYCode(p));
            double px = fx, py = fy;
            for (std::size_t i = 1; i < path.size(); ++i) {
                const double nx = g90 ? path[i].x : path[i].x - px, ny = g90 ? path[i].y : path[i].y - py;
                p.x = nx;
                p.y = ny;
                line(pp.linearCode(p));
                px = nx;
                py = ny;
            }
            line(pp.spindleOffCode(p));
            line(pp.spindleRevCode(p));
            line(pp.downZStopCode(p));
            line(pp.spindleOffCode(p));
            line(pp.dwellRevCode(p));
            line(pp.zFeedrateCode(p));
            line(pp.liftCode(p));
        } else {
            line(pp.linearCode(p));
            line(pp.feedrateZDispenseCode(p));
            line(pp.downZStartCode(p));
            line(pp.spindleFwdCode(p));
            line(pp.dwellFwdCode(p));
            line(pp.liftZDispenseCode(p));
            line(pp.spindleOffCode(p));
            line(pp.spindleRevCode(p));
            line(pp.spindleOffCode(p));
            line(pp.downZStopCode(p));
            line(pp.dwellRevCode(p));
            line(pp.zFeedrateCode(p));
            line(pp.liftCode(p));
        }
        current = path.back();
    }
    line(pp.liftCode(p));
    line(pp.endCode(p));
    return g;
}

}  // namespace flatcam
