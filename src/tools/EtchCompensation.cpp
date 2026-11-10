#include "tools/EtchCompensation.h"

#include <cmath>

namespace flatcam {

std::optional<double> etchOffset(EtchRatio ratio, double thicknessUm, double factor, const std::string& etchant,
                                 double offsetUm) {
    const double thickness = thicknessUm / 1000.0;
    switch (ratio) {
        case EtchRatio::Factor:
            // etch_factor = 1 / factor; offset = thickness / etch_factor (con 0, ZeroDivisionError).
            if (factor == 0.0) return std::nullopt;
            return thickness / (1.0 / factor);
        case EtchRatio::EtchList:
            return thickness / (etchant == "CuCl2" ? 0.33 : 0.25);
        case EtchRatio::Manual:
            break;
    }
    return offsetUm / 1000.0;
}

EtchResult etchCompensate(const Gerber& g, double offset, int steps) {
    EtchResult r;
    ShapeList grown;
    for (const Shape& poly : g.solidGeometry()) grown.push_back(poly.buffer(offset, steps));
    const Shape u = Shape::unaryUnion(grown);
    r.solid = u.parts();
    r.apertures = g.apertures();
    for (auto& [id, ap] : r.apertures) {
        if (ap.type == "R" || ap.type == "O") {
            ap.width += offset;
            ap.height += offset;
            if (ap.size) ap.size = std::sqrt(ap.width * ap.width + ap.height * ap.height);
        } else if (ap.size) {
            *ap.size += offset;
        }
        for (ApertureGeometry& el : ap.geometry)
            if (el.solid) el.solid = el.solid->buffer(offset, steps);
    }
    return r;
}

}  // namespace flatcam
