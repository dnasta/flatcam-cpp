#include "tools/DoubleSided.h"

namespace flatcam {

Point2D DoubleSided::boxCenter(const BoundingBox& box) { return {0.5 * (box.xmin + box.xmax), 0.5 * (box.ymin + box.ymax)}; }

std::vector<Point2D> DoubleSided::alignmentDrills(const std::vector<Point2D>& holes, Axis axis, const Point2D& pivot) {
    // xscale, yscale = {"X": (1.0, -1.0), "Y": (-1.0, 1.0)}[axis]
    const double xscale = (axis == Axis::X) ? 1.0 : -1.0;
    const double yscale = (axis == Axis::X) ? -1.0 : 1.0;
    std::vector<Point2D> out;
    out.reserve(holes.size() * 2);
    for (const Point2D& h : holes) {
        out.push_back(h);
        // affinity.scale(point, xscale, yscale, origin=(px, py)) arma la matriz [xscale, 0, 0, yscale] con el desplazamiento
        // (px - px * xscale, py - py * yscale) y calcula `factor * x + desplazamiento`; esa es la aritmética que da el mismo
        // último bit que Shapely (no `px + (x - px) * factor`).
        const double xoff = pivot.x - pivot.x * xscale;
        const double yoff = pivot.y - pivot.y * yscale;
        out.push_back({xscale * h.x + xoff, yscale * h.y + yoff});
    }
    return out;
}

Excellon DoubleSided::alignmentExcellon(const std::vector<Point2D>& holes, Axis axis, const Point2D& pivot, double diameter,
                                        const ExcellonOptions& options) {
    Excellon ex(options);
    ex.addDrillTool(1, diameter, alignmentDrills(holes, axis, pivot));
    ex.createGeometry();
    return ex;
}

}  // namespace flatcam
