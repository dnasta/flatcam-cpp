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
            const auto lines =
                PolygonClearing::clear(pp, o.toolDia, o.stepsPerCircle, o.overlap, o.connect, o.contour, o.method);
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

bool Paint::paintRest(const ShapeList& geometry, const std::vector<PaintRestTool>& tools, double offset,
                      PolygonClearing::Method method, int stepsPerCircle,
                      std::vector<std::pair<std::size_t, ShapeList>>& perTool, std::string& error) {
    error.clear();
    perTool.clear();
    try {
        ShapeList bufs;
        for (const Shape& pol : flattenToPolygons(geometry)) {
            const Shape b = pol.buffer(-offset);
            if (b.isEmpty()) continue;
            if (isMulti(b.type())) {
                for (const Shape& x : b.parts()) bufs.push_back(x);
            } else {
                bufs.push_back(b);
            }
        }
        Shape polyBuf = Shape::unaryUnion(bufs);
        if (polyBuf.isEmpty()) {
            error = "No hay polígonos para pintar";
            return false;
        }
        std::vector<std::size_t> order(tools.size());
        for (std::size_t i = 0; i < order.size(); ++i) order[i] = i;
        std::stable_sort(order.begin(), order.end(),
                         [&](std::size_t a, std::size_t b) { return tools[a].toolDia > tools[b].toolDia; });
        for (std::size_t idx : order) {
            const PaintRestTool& t = tools[idx];
            const ShapeList pieces = isMulti(polyBuf.type()) ? polyBuf.parts() : ShapeList{polyBuf};
            ShapeList cleared, restList;
            bool failed = false;
            for (const Shape& pp : pieces) {
                if (pp.buffer(-t.toolDia / 2.0).isEmpty()) continue;
                const auto res =
                    PolygonClearing::clear(pp, t.toolDia, stepsPerCircle, t.overlap, t.connect, t.contour, method);
                if (!res) {
                    failed = true;  // `geo_res.get_objects()` sobre None: excepción y se pasa a la herramienta siguiente
                    break;
                }
                ShapeList elems;
                for (const auto& l : *res) elems.push_back(PolygonClearing::toShape(l));
                const Shape ppCleared = Shape::unaryUnion(elems).buffer(t.toolDia / 2.0);
                const Shape rest = pp.difference(ppCleared);
                if (!rest.isEmpty()) {
                    for (const Shape& r : isMulti(rest.type()) ? rest.parts() : ShapeList{rest})
                        if (r.isValid()) restList.push_back(r);
                }
                cleared.insert(cleared.end(), elems.begin(), elems.end());
            }
            if (failed) continue;
            if (!cleared.empty()) perTool.push_back({idx, cleared});
            const Shape buffered = Shape::unaryUnion(cleared).buffer(t.toolDia / 2.0);
            const Shape remaining = polyBuf.difference(buffered);
            ShapeList tmp;
            for (const Shape& p : isMulti(remaining.type()) ? remaining.parts() : ShapeList{remaining})
                if (!p.isEmpty() && p.isValid()) tmp.push_back(p);
            tmp.insert(tmp.end(), restList.begin(), restList.end());
            if (tmp.empty()) break;
            polyBuf = Shape::collection(tmp);
            if (!polyBuf.isValid()) polyBuf = Shape::unaryUnion(tmp);
            if (polyBuf.isEmpty() || !polyBuf.isValid()) break;
        }
    } catch (const std::exception& e) {
        error = std::string("Error al pintar: ") + e.what();
        return false;
    }
    if (perTool.empty()) {
        error = "La herramienta es demasiado grande para la geometría: no se generó ninguna trayectoria";
        return false;
    }
    return true;
}

}  // namespace flatcam
