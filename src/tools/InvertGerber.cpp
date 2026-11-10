#include "tools/InvertGerber.h"

#include <cmath>

namespace flatcam {

ShapeList invertGerber(const ShapeList& solid, const BoundingBox& b, double margin, JoinStyle join, int steps,
                       int decimals) {
    const double p = std::pow(10.0, decimals);
    if (std::round(margin * p) / p == 0.0) margin = 1e-10;
    Shape s = Shape::box(b.xmin, b.ymin, b.xmax, b.ymax).buffer(margin, steps, join);
    for (const Shape& poly : solid) s = s.difference(poly);
    const ShapeList parts = s.parts();
    return parts.empty() ? ShapeList{s} : parts;
}

}  // namespace flatcam
