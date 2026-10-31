#include "tools/Paint.h"

namespace flatcam {

namespace {

bool isMulti(ShapeType t) { return static_cast<int>(t) >= static_cast<int>(ShapeType::MultiPoint); }

void recurse(const Shape& g, ShapeList& out, bool topLevel) {
    if (isMulti(g.type())) {
        for (const Shape& part : g.parts()) {
            if (!part.isEmpty() && part.isValid()) recurse(part, out, false);
        }
        return;
    }
    (void)topLevel;
    if (g.type() == ShapeType::LinearRing) out.push_back(Shape::polygon(g.coordinates()));
    else out.push_back(g);
}

}  // namespace

ShapeList Paint::flattenToPolygons(const ShapeList& geometry) {
    ShapeList out;
    for (const Shape& g : geometry) {
        if (!g.isEmpty() && g.isValid()) recurse(g, out, true);
    }
    return out;
}

bool Paint::paintAll(const ShapeList& solidGeometry, const PaintOptions& o, ShapeList& paths, std::string& error) {
    error.clear();
    paths.clear();
    if (o.toolDia <= 0) {
        error = "El diámetro de la herramienta debe ser positivo";
        return false;
    }
    try {
        const ShapeList area = flattenToPolygons(solidGeometry);
        if (area.empty()) {
            error = "No hay polígonos para pintar";
            return false;
        }
        ShapeList buffered;
        for (const Shape& pol : area) {
            const Shape b = pol.buffer(-o.offset);
            if (!b.isEmpty()) buffered.push_back(b);
        }
        for (const Shape& pp : buffered) {
            if (pp.type() != ShapeType::Polygon && pp.type() != ShapeType::MultiPolygon) {
                error = "Se esperaba un polígono o un multipolígono (el original falla con otras figuras)";
                return false;
            }
            const auto lines = PolygonClearing::clearPolygon(pp, o.toolDia, o.stepsPerCircle, o.overlap, o.connect);
            if (!lines) continue;
            for (const auto& l : *lines) {
                Shape s = PolygonClearing::toShape(l);
                if (!s.isEmpty()) paths.push_back(s);
            }
        }
    } catch (const std::exception& e) {
        error = std::string("Error al pintar: ") + e.what();
        return false;
    }
    if (paths.empty()) {
        error = "La herramienta es demasiado grande para la geometría: no se generó ninguna trayectoria";
        return false;
    }
    return true;
}

}  // namespace flatcam
