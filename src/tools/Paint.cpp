#include "tools/Paint.h"

#include <stdexcept>

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

bool Paint::paintAll(const ShapeList& solidGeometry, const PaintOptions& o, ShapeList& paths, std::string& error,
                     const Gerber* gerber) {
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
            if (o.method == PolygonClearing::Method::LaserLines) {
                // Solo para Gerber: con otra geometría el original no ofrece el método.
                if (!gerber) {
                    error = "El método Líneas láser solo sirve para objetos Gerber";
                    return false;
                }
                if (const auto laser = laserLines(pp, *gerber, o))
                    for (const Shape& sh : *laser)
                        if (!sh.isEmpty()) paths.push_back(sh);
                continue;
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

namespace {

/// Inserta las líneas de una figura en el almacén como el original (`for x in geom: insert(x)` o, si no se puede
/// iterar, la figura entera): solo líneas y anillos.
void insertLines(EndpointStorage& storage, const Shape& g) {
    const ShapeType t = g.type();
    if (!isMulti(t)) {
        // No se puede iterar: `geoms.insert(lines_trimmed)`. Una figura vacía hace fallar a `get_pts` (IndexError),
        // y la excepción sale de fill_with_lines (pasa con las regiones, de tamaño 0).
        if (g.isEmpty()) throw std::runtime_error("index out of range");
        storage.insert(g.coordinates(), t == ShapeType::LinearRing);
    } else {
        for (const Shape& p : g.parts()) {
            if (p.type() == ShapeType::LineString) storage.insert(p.coordinates(), false);
            else if (p.type() == ShapeType::LinearRing) storage.insert(p.coordinates(), true);
            // "Not a line": se descarta
        }
    }
}

/// `for ll in geom: lista.append(ll)` (figuras múltiples por partes) o, si no se puede iterar, la figura entera.
void appendParts(ShapeList& list, const Shape& g) {
    if (isMulti(g.type())) {
        for (const Shape& p : g.parts()) list.push_back(p);
    } else {
        list.push_back(g);
    }
}

}  // namespace

std::optional<std::vector<EndpointStorage::Line>> Paint::fillWithLines(const Shape& line, double apertureSize,
                                                                      double tooldia, int steps, double overlap,
                                                                      bool connect, bool contour) {
    // isinstance(line, LineString): también los anillos (LinearRing deriva de LineString en Shapely).
    if (line.type() != ShapeType::LineString && line.type() != ShapeType::LinearRing) return std::nullopt;
    EndpointStorage geoms;
    ShapeList trimmed;
    const Shape polygon = line.buffer(apertureSize / 2.0, steps);
    const Shape margin = polygon.buffer(-tooldia / 2.0, steps);
    double delta = 0;
    while (delta < apertureSize / 2) {
        trimmed.push_back(line.parallelOffset(delta, false, steps).intersection(margin));
        trimmed.push_back(line.parallelOffset(delta, true, steps).intersection(margin));
        delta += tooldia * (1 - overlap);
    }
    delta = (apertureSize / 2) - (tooldia / 2.00000001);
    appendParts(trimmed, line.parallelOffset(delta, false, steps).intersection(margin));
    appendParts(trimmed, line.parallelOffset(delta, true, steps).intersection(margin));
    insertLines(geoms, Shape::unaryUnion(trimmed));

    if (contour) {
        const ShapeList polys = isMulti(margin.type()) ? margin.parts() : ShapeList{margin};
        for (const Shape& poly : polys) {
            if (poly.type() != ShapeType::Polygon || poly.isEmpty()) continue;
            geoms.insert(poly.exterior().coordinates(), true);
            for (const Shape& in : poly.interiors()) geoms.insert(in.coordinates(), true);
        }
    }
    if (connect) {
        if (auto conn = PolygonClearing::paintConnect(geoms, polygon, tooldia, steps)) return conn;
    }
    return geoms.objects();
}

std::optional<ShapeList> Paint::laserLines(const Shape& polyg, const Gerber& gerber, const PaintOptions& o) {
    try {
        // Los destellos por tipo (C, O, R) y los trazos por ancho, en el orden de los diccionarios del original.
        std::vector<std::pair<std::string, std::vector<const ApertureGeometry*>>> flashes;
        std::vector<std::pair<double, std::vector<const ApertureGeometry*>>> traces;
        auto addTo = [](auto& dict, const auto& key, const ApertureGeometry* g) {
            for (auto& entry : dict)
                if (entry.first == key) {
                    entry.second.push_back(g);
                    return;
                }
            dict.push_back({key, {g}});
        };
        const auto& aps = gerber.apertures();
        for (const std::string& id : gerber.apertureOrder()) {
            const Aperture& ap = aps.at(id);
            for (const ApertureGeometry& el : ap.geometry) {
                if (!ap.size) return std::nullopt;  // apval["size"] de una macro: KeyError
                if (*ap.size == 0.0) addTo(traces, 0.0, &el);
                if (el.follow && el.follow->within(polyg)) {
                    if (el.follow->type() == ShapeType::Point) {
                        if (ap.type == "C" || ap.type == "O" || ap.type == "R") addTo(flashes, ap.type, &el);
                    } else {
                        addTo(traces, *ap.size, &el);
                    }
                }
            }
        }
        ShapeList out;
        // Los destellos: Semilla (C, O) o Líneas (R), siempre con contorno.
        for (const auto& [type, elems] : flashes) {
            for (const ApertureGeometry* el : elems) {
                if (!el->solid) continue;
                const auto f = type == "R" ? PolygonClearing::clearPolygonLines(*el->solid, o.toolDia, o.stepsPerCircle,
                                                                                  o.overlap, o.connect, true)
                                           : PolygonClearing::clearPolygonSeed(*el->solid, o.toolDia, o.stepsPerCircle,
                                                                                 o.overlap, o.connect, true);
                if (!f) return std::nullopt;  // f_o.get_objects() sobre None
                for (const auto& l : *f)
                    if (!l.points.empty()) out.push_back(PolygonClearing::toShape(l));
            }
        }
        // Desde acá, si algo falla el almacén ya tiene los pads: el original los devuelve (`if cpoly and cpoly.objects`).
        auto partial = [&]() -> std::optional<ShapeList> {
            if (out.empty()) return std::nullopt;
            return out;
        };
        try {
        // Los trazos: líneas paralelas a su línea central; después se unen.
        ShapeList copper;
        for (const auto& [size, elems] : traces) {
            for (const ApertureGeometry* el : elems) {
                if (!el->follow || el->follow->isEmpty()) continue;  // `if line:`
                const auto t =
                    fillWithLines(*el->follow, size, o.toolDia, o.stepsPerCircle, o.overlap, o.connect, o.contour);
                if (!t) return partial();  // t_o.get_objects() sobre None
                for (const auto& l : *t) {
                    const Shape sh = PolygonClearing::toShape(l);
                    if (!sh.isEmpty()) copper.push_back(sh);
                }
            }
        }
        const Shape united = Shape::unaryUnion(copper);
        if (united.type() == ShapeType::LineString) return partial();  // linemerge: "Cannot linemerge"
        if (!united.isEmpty()) {
            const Shape merged = Shape::lineMerge(isMulti(united.type()) ? united.parts() : ShapeList{united});
            ShapeList parts;
            appendParts(parts, merged);
            for (const Shape& p : parts)
                if (!p.isEmpty()) out.push_back(p);
        }
        } catch (const std::exception&) {
            return partial();
        }
        return partial();  // vacío: "Geometry could not be painted completely"
    } catch (const std::exception&) {
        return std::nullopt;  // el original registra la excepción y no pinta el polígono
    }
}

bool Paint::paintRest(const ShapeList& geometry, const std::vector<PaintRestTool>& tools, double offset,
                      PolygonClearing::Method method, int stepsPerCircle,
                      std::vector<std::pair<std::size_t, ShapeList>>& perTool, std::string& error,
                      const Gerber* gerber) {
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
                ShapeList elems;
                if (method == PolygonClearing::Method::LaserLines) {
                    PaintOptions lo;
                    lo.toolDia = t.toolDia;
                    lo.overlap = t.overlap;
                    lo.connect = t.connect;
                    lo.contour = t.contour;
                    lo.stepsPerCircle = stepsPerCircle;
                    const auto laser = gerber ? laserLines(pp, *gerber, lo) : std::nullopt;
                    if (!laser) {
                        failed = true;
                        break;
                    }
                    elems = *laser;
                } else {
                    const auto res =
                        PolygonClearing::clear(pp, t.toolDia, stepsPerCircle, t.overlap, t.connect, t.contour, method);
                    if (!res) {
                        failed = true;  // `geo_res.get_objects()` sobre None: excepción y se pasa a la herramienta siguiente
                        break;
                    }
                    for (const auto& l : *res) elems.push_back(PolygonClearing::toShape(l));
                }
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
