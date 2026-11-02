#include "tools/Ncc.h"

#include "core/GeometryError.h"
#include "tools/Isolation.h"

namespace flatcam {

namespace {

bool isMulti(ShapeType t) { return static_cast<int>(t) >= static_cast<int>(ShapeType::MultiPoint); }

}  // namespace

Shape Ncc::clearingBox(const ShapeList& solidGeometry, const NccOptions& o) {
    if (o.region) return *o.region;
    // calculate_bounding_box, selección "Él mismo": una lista de un solo polígono se une tal cual; en otro caso, la
    // envolvente convexa de la unión.
    Shape env;
    if (solidGeometry.size() == 1 && solidGeometry[0].type() == ShapeType::Polygon)
        env = Shape::unaryUnion(solidGeometry);
    else
        env = Shape::unaryUnion(solidGeometry).convexHull();
    // apply_margin_to_bounding_box: buffer con esquinas en ingletes.
    return env.buffer(o.margin, 16, JoinStyle::Mitre);
}

namespace {

/// `generate_envelope(tool_iso / 2, invert)` de ToolNCC: `isolation_geometry` completa sobre el cobre.
ShapeList nccEnvelope(const ShapeList& solid, double isoDia, bool climb, int steps) {
    return Isolation::generateEnvelope(solid, isoDia / 2, climb, IsolationType::Full, false, steps);
}

}  // namespace

ShapeList Ncc::isolationPaths(const ShapeList& solidGeometry, double isoDia, bool climb, const Shape& box, int steps,
                              bool& broken) {
    broken = false;
    ShapeList out;
    ShapeList env;
    try {
        env = nccEnvelope(solidGeometry, isoDia, climb, steps);
    } catch (const std::exception&) {
        return out;  // 'fail': "Isolation geometry could not be generated."
    }
    auto keep = [&](const Shape& g) {
        const Shape r = g.intersection(box);
        if (!r.isEmpty()) out.push_back(r);
    };
    for (const Shape& e : env) {
        for (const Shape& part : isMulti(e.type()) ? e.parts() : ShapeList{e}) {
            if (part.type() == ShapeType::Polygon) {
                // poly2rings: el exterior y los agujeros.
                keep(part.exterior());
                for (const Shape& in : part.interiors()) keep(in);
            } else if (part.type() == ShapeType::LineString || part.type() == ShapeType::LinearRing) {
                keep(part);
            }
        }
    }
    for (const Shape& g : out)
        if (g.type() == ShapeType::MultiLineString) broken = true;
    return out;
}

ShapeList Ncc::emptyArea(const ShapeList& solidGeometry, const NccOptions& o) {
    const Shape bbox = clearingBox(solidGeometry, o);

    // get_tool_empty_area: sin herramientas de aislamiento, el cobre; con ellas, la envolvente de la última.
    Shape target = Shape::collection(solidGeometry);
    if (!o.isoToolDias.empty()) {
        try {
            target = Shape::unaryUnion(nccEnvelope(solidGeometry, o.isoToolDias.back(), o.climb, o.stepsPerCircle));
        } catch (const std::exception&) {
            target = Shape();
        }
    }
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

Shape Ncc::areaRegion(const ShapeList& areas, double margin) {
    // env_obj = unary_union(sel_rect); después cada polígono agrandado el margen y todo unido.
    const Shape u = Shape::unaryUnion(areas);
    ShapeList parts;
    for (const Shape& p : isMulti(u.type()) ? u.parts() : ShapeList{u})
        parts.push_back(p.buffer(margin, 16, JoinStyle::Mitre));
    return Shape::unaryUnion(parts);
}

Shape Ncc::referenceRegion(const ShapeList& copper, const ShapeList& reference, bool referenceIsGerber, double margin) {
    if (referenceIsGerber) {
        const Shape box = Shape::unaryUnion(reference).convexHull();
        const Shape ncc = Shape::unaryUnion(copper).convexHull();
        return ncc.intersection(box).buffer(margin, 16, JoinStyle::Mitre);
    }
    return areaRegion(reference, margin);
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
