#include "tools/CornerMarkers.h"

#include <algorithm>

namespace flatcam {

ShapeList cornerLines(const BoundingBox& b, const CornerSelection& sel, bool cross, double t, double length,
                      double margin) {
    const double l = length / 2.0;
    const double off = margin + t / 2.0;
    ShapeList out;
    // dx, dy: hacia dónde va la semicruz (adentro de la caja) en cada esquina.
    auto add = [&](double x, double y, double dx, double dy) {
        if (!cross) {
            out.push_back(Shape::lineString({{x, y}, {x + dx * l, y}}));
            out.push_back(Shape::lineString({{x, y}, {x, y + dy * l}}));
        } else {
            out.push_back(Shape::lineString({{x - dx * l, y}, {x + dx * l, y}}));
            out.push_back(Shape::lineString({{x, y - dy * l}, {x, y + dy * l}}));
        }
    };
    if (sel.topLeft) add(b.xmin - off, b.ymax + off, 1, -1);
    if (sel.topRight) add(b.xmax + off, b.ymax + off, -1, -1);
    if (sel.bottomLeft) add(b.xmin - off, b.ymin - off, 1, 1);
    if (sel.bottomRight) add(b.xmax + off, b.ymin - off, -1, 1);
    return out;
}

CornersGerber addCornerMarkers(const Gerber& g, const ShapeList& lines, double thickness, int steps) {
    CornersGerber r;
    r.apertures = g.apertures();
    std::optional<std::string> found;
    for (const std::string& id : g.apertureOrder()) {
        const Aperture& ap = g.apertures().at(id);
        if (ap.type == "C" && ap.size && *ap.size == thickness) {
            found = id;
            break;
        }
    }
    std::string apid;
    JoinStyle join = JoinStyle::Mitre;
    if (found) {
        apid = *found;
    } else {
        // str(int(max(ap_keys)) + 1): el máximo como texto ("9" > "10").
        if (r.apertures.empty()) {
            apid = "10";
        } else {
            std::string mx;
            for (const auto& [k, v] : r.apertures) mx = std::max(mx, k);
            apid = std::to_string(std::stoi(mx) + 1);
        }
        Aperture ap;
        ap.type = "C";
        ap.size = thickness;
        r.apertures[apid] = ap;
        join = JoinStyle::Bevel;
    }
    Aperture& ap = r.apertures[apid];
    ap.hasGeometry = true;
    ShapeList buffs;
    for (const Shape& line : lines) {
        const Shape b = line.buffer(thickness / 2.0, steps, join);
        buffs.push_back(b);
        ApertureGeometry el;
        el.follow = line;
        el.solid = b;
        ap.geometry.push_back(el);
    }
    // MultiPolygon(geo_buff_list).buffer(0) y unary_union con el cobre.
    ShapeList all = g.solidGeometry();
    const Shape marks = Shape::unaryUnion(buffs);
    const ShapeList mp = marks.parts();
    all.insert(all.end(), mp.begin(), mp.end());
    if (mp.empty()) all.push_back(marks);
    const Shape u = Shape::unaryUnion(all);
    r.solid = u.parts().empty() ? ShapeList{u} : u.parts();
    r.follow = g.followGeometry();
    r.follow.insert(r.follow.end(), lines.begin(), lines.end());
    return r;
}

std::vector<Point2D> cornerDrills(const BoundingBox& b, const CornerSelection& sel, double t, double margin) {
    const double off = margin + t / 2.0;
    std::vector<Point2D> out;
    if (sel.topLeft) out.push_back({b.xmin - off, b.ymax + off});
    if (sel.topRight) out.push_back({b.xmax + off, b.ymax + off});
    if (sel.bottomLeft) out.push_back({b.xmin - off, b.ymin - off});
    if (sel.bottomRight) out.push_back({b.xmax + off, b.ymin - off});
    return out;
}

}  // namespace flatcam
