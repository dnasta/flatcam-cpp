#include "tools/Ncc.h"

#include "core/GeometryError.h"

namespace flatcam {

namespace {

bool isMulti(ShapeType t) { return static_cast<int>(t) >= static_cast<int>(ShapeType::MultiPoint); }

}  // namespace

ShapeList Ncc::emptyArea(const ShapeList& solidGeometry, const NccOptions& o) {
    // calculate_bounding_box, selección "Él mismo": una lista de un solo polígono se une tal cual; en otro caso, la
    // envolvente convexa de la unión.
    Shape env;
    if (solidGeometry.size() == 1 && solidGeometry[0].type() == ShapeType::Polygon) env = Shape::unaryUnion(solidGeometry);
    else env = Shape::unaryUnion(solidGeometry).convexHull();

    // apply_margin_to_bounding_box: buffer con esquinas en ingletes.
    const Shape bbox = env.buffer(o.margin, 16, JoinStyle::Mitre);

    // get_tool_empty_area para un Gerber sin herramientas de aislamiento.
    Shape target = Shape::collection(solidGeometry);
    if (o.hasOffset) target = target.buffer(o.offset);

    // get_ncc_empty_area: la diferencia de golpe; si GEOS falla, de a una figura por vez.
    Shape empty;
    try {
        empty = bbox.difference(target);
    } catch (const GeometryError&) {
        empty = bbox;
        for (const Shape& el : solidGeometry) empty = empty.difference(el);
    }
    if (empty.isEmpty()) return {};
    ShapeList out;
    if (isMulti(empty.type())) {
        for (const Shape& p : empty.parts()) out.push_back(p);
    } else {
        out.push_back(empty);
    }
    return out;
}

bool Ncc::clearCopper(const ShapeList& solidGeometry, const NccOptions& o, ShapeList& paths, std::string& error) {
    error.clear();
    paths.clear();
    if (o.toolDia <= 0) {
        error = "El diámetro de la herramienta debe ser positivo";
        return false;
    }
    try {
        const ShapeList area = emptyArea(solidGeometry, o);
        if (area.empty()) {
            error = "No se pudo obtener la extensión del área a limpiar";
            return false;
        }
        for (const Shape& region : area) {
            const Shape p = region.buffer(0);
            if (p.isEmpty() || !p.isValid()) continue;
            ShapeList pols;
            if (isMulti(p.type())) pols = p.parts();
            else pols.push_back(p);
            for (const Shape& pol : pols) {
                if (pol.type() != ShapeType::Polygon) continue;
                const auto lines = PolygonClearing::clearPolygon(pol, o.toolDia, o.stepsPerCircle, o.overlap, o.connect);
                if (!lines) continue;  // "Polygon could not be cleared" en el original
                for (const auto& l : *lines) paths.push_back(PolygonClearing::toShape(l));
            }
        }
    } catch (const std::exception& e) {
        error = std::string("Error al limpiar el cobre: ") + e.what();
        return false;
    }
    if (paths.empty()) {
        error = "No hay geometría de NCC: la herramienta es demasiado grande para el área a limpiar";
        return false;
    }
    return true;
}

}  // namespace flatcam
