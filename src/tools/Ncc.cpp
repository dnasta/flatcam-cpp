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
                const auto lines =
                    PolygonClearing::clear(pol, o.toolDia, o.stepsPerCircle, o.overlap, o.connect, o.contour, o.method);
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

bool Ncc::clearCopperRest(const ShapeList& solidGeometry, const std::vector<NccRestTool>& tools,
                          const NccOptions& common, std::vector<std::pair<std::size_t, ShapeList>>& perTool,
                          ShapeList& notCleared, std::string& error) {
    error.clear();
    perTool.clear();
    notCleared.clear();
    try {
        ShapeList area = emptyArea(solidGeometry, common);
        if (area.empty()) {
            error = "No se pudo obtener la extensión del área a limpiar";
            return false;
        }
        std::vector<std::size_t> order(tools.size());
        for (std::size_t i = 0; i < order.size(); ++i) order[i] = i;
        std::stable_sort(order.begin(), order.end(),
                         [&](std::size_t a, std::size_t b) { return tools[a].toolDia > tools[b].toolDia; });
        for (std::size_t idx : order) {
            const NccRestTool& t = tools[idx];
            ShapeList cleared;
            for (const Shape& p : area) {
                if (p.isEmpty() || !p.isValid()) continue;
                if (p.buffer(-t.toolDia / 2, common.stepsPerCircle).isEmpty()) continue;
                if (p.type() != ShapeType::Polygon) continue;
                const auto res = PolygonClearing::clear(p, t.toolDia, common.stepsPerCircle, t.overlap, common.connect,
                                                        common.contour, t.method);
                if (!res) continue;  // "Polygon could not be cleared"
                for (const auto& l : *res) cleared.push_back(PolygonClearing::toShape(l));
            }
            if (!cleared.empty()) perTool.push_back({idx, cleared});
            // new_area = MultiPolygon([line.buffer(tool / 1.9999999) ...]).buffer(0.0000001)
            ShapeList swept;
            for (const Shape& l : cleared) swept.push_back(l.buffer(t.toolDia / 1.9999999));
            Shape remaining = Shape::collection(area);
            if (!swept.empty()) remaining = remaining.difference(Shape::collection(swept).buffer(0.0000001));
            area.clear();
            for (const Shape& pol : isMulti(remaining.type()) ? remaining.parts() : ShapeList{remaining})
                if (pol.isValid() && !pol.isEmpty()) area.push_back(pol);
            if (area.empty()) break;
        }
        notCleared = area;
    } catch (const std::exception& e) {
        error = std::string("Error al limpiar el cobre: ") + e.what();
        return false;
    }
    if (perTool.empty()) {
        error = "No hay geometría de NCC: las herramientas son demasiado grandes para el área a limpiar";
        return false;
    }
    return true;
}

}  // namespace flatcam
