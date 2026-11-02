#include "tools/Isolation.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>

#include "core/GeometryError.h"

namespace flatcam {

namespace {

bool isMulti(ShapeType t) { return static_cast<int>(t) >= static_cast<int>(ShapeType::MultiPoint); }

/// Partes de una figura: ella misma si es simple, sus miembros si es múltiple; nada si está vacía.
ShapeList partsOf(const Shape& s) {
    if (s.isEmpty()) return {};
    if (isMulti(s.type())) return s.parts();
    return {s};
}

/// Polígono con el contorno exterior al revés y los mismos agujeros (`Polygon(p.exterior.coords[::-1], p.interiors)`).
Shape reversedPolygon(const Shape& p) {
    auto ext = p.exterior().coordinates();
    std::reverse(ext.begin(), ext.end());
    std::vector<std::vector<Point2D>> holes;
    for (const Shape& h : p.interiors()) holes.push_back(h.coordinates());
    return Shape::polygon(ext, holes);
}

/// Polígono hecho con los vértices de un anillo al revés (`Polygon(p.coords[::-1])`).
Shape reversedRingPolygon(const Shape& ring) {
    auto c = ring.coordinates();
    std::reverse(c.begin(), c.end());
    return Shape::polygon(c);
}

/// Inversión del sentido de `generate_envelope` y `generate_rest_geometry`: polígonos y anillos; lo demás se descarta.
ShapeList invertAll(const ShapeList& geometry) {
    ShapeList out;
    for (const Shape& g : geometry) {
        if (g.type() == ShapeType::Polygon) out.push_back(reversedPolygon(g));
        else if (g.type() == ShapeType::LinearRing) out.push_back(reversedRingPolygon(g));
    }
    return out;
}

/// `[poly.exterior] + poly.interiors` (`poly2rings`).
ShapeList rings(const Shape& poly) {
    ShapeList out{poly.exterior()};
    for (const Shape& h : poly.interiors()) out.push_back(h);
    return out;
}

/// Núcleo común de `area_subtraction` y `area_intersection`: aplica @p op a cada anillo o línea.
template <typename Op>
ShapeList ringwise(const ShapeList& geometry, Op op) {
    ShapeList out;
    auto keep = [&out](const Shape& g) {
        if (!g.isEmpty()) out.push_back(g);
    };
    for (const Shape& el : geometry) {
        switch (el.type()) {
            case ShapeType::Polygon:
                for (const Shape& r : rings(el)) keep(op(r));
                break;
            case ShapeType::MultiPolygon:
                for (const Shape& p : el.parts())
                    for (const Shape& r : rings(p)) keep(op(r));
                break;
            case ShapeType::LineString:
            case ShapeType::LinearRing:
                keep(op(el));
                break;
            case ShapeType::MultiLineString:
                for (const Shape& l : el.parts()) keep(op(l));
                break;
            default:
                break;  // puntos y colecciones: el original no los procesa
        }
    }
    return out;
}

/// Descarta las figuras vacías (`[geo for geo in iso_geo if not geo.is_empty]`).
ShapeList withoutEmpty(const ShapeList& geometry) {
    ShapeList out;
    for (const Shape& g : geometry)
        if (!g.isEmpty()) out.push_back(g);
    return out;
}

/// Diámetro redondeado como `float('%.*f' % (decimals, tooldia))`.
double roundedDia(double dia, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, dia);
    return std::strtod(buf, nullptr);
}

}  // namespace

double Isolation::passOffset(double toolDia, int pass, double overlap, bool negative) {
    const double offset = toolDia * ((2 * pass + 1) / 2.0000001) - (pass * overlap * toolDia);
    return negative ? -offset : offset;
}

std::optional<double> Isolation::safeToolDiameter(const Gerber& gerber, int decimals, std::string& error) {
    error.clear();
    ShapeList solids;
    for (const auto& [id, ap] : gerber.apertures())
        for (const ApertureGeometry& el : ap.geometry)
            if (el.solid && el.solid->isValid()) solids.push_back(*el.solid);
    const Shape total = Shape::collection(solids).buffer(0);
    // `iter(total_geo)`: un solo polígono no se puede recorrer.
    if (total.type() != ShapeType::MultiPolygon && total.type() != ShapeType::GeometryCollection) {
        error = "The Gerber object has one Polygon as geometry.\nThere are no distances between geometry elements to be found.";
        return std::nullopt;
    }
    const ShapeList parts = total.parts();
    auto round = [decimals](double v) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%.*f", decimals, v);
        return std::strtod(buf, nullptr);
    };
    std::optional<double> best;
    for (std::size_t i = 0; i < parts.size(); ++i)
        for (std::size_t j = i + 1; j < parts.size(); ++j) {
            const double d = round(parts[i].distance(parts[j]));
            if (!best || d < *best) best = d;
        }
    if (!best) {
        error = "No distances";  // min() de una lista vacía: el original falla y no informa nada
        return std::nullopt;
    }
    return round(*best);  // dec_format
}

ShapeList Isolation::isolationGeometry(const ShapeList& geometry, double offset, IsolationType type,
                                       int stepsPerCircle) {
    ShapeList buffered;
    buffered.reserve(geometry.size());
    for (const Shape& pol : geometry) {
        buffered.push_back(offset == 0 ? pol : pol.buffer(offset, stepsPerCircle, JoinStyle::Round));
    }
    const ShapeList united = partsOf(Shape::unaryUnion(buffered));
    if (type == IsolationType::Full) return united;
    ShapeList out;
    for (const Shape& p : united) {
        if (p.type() != ShapeType::Polygon) continue;
        if (type == IsolationType::Exterior) {
            out.push_back(p.exterior());
        } else {
            for (const Shape& h : p.interiors()) out.push_back(h);
        }
    }
    return out;
}

ShapeList Isolation::generateEnvelope(const ShapeList& geometry, double offset, bool invert, IsolationType type,
                                      bool follow, int stepsPerCircle) {
    if (follow) return geometry;
    const ShapeList geom = isolationGeometry(geometry, offset, type, stepsPerCircle);
    return invert ? invertAll(geom) : geom;
}

ShapeList Isolation::areaSubtraction(const ShapeList& geometry, const ShapeList& subtractor) {
    const Shape sub = Shape::unaryUnion(subtractor);
    return ringwise(geometry, [&sub](const Shape& g) { return g.difference(sub); });
}

ShapeList Isolation::areaIntersection(const ShapeList& geometry, const ShapeList& area) {
    const Shape inter = Shape::unaryUnion(area);
    return ringwise(geometry, [&inter](const Shape& g) { return g.intersection(inter); });
}

ShapeList Isolation::generateRestGeometry(const ShapeList& geometry, double toolDia, int passes, double overlap,
                                          bool invert, IsolationType type, bool negativeDia, bool forcedRest,
                                          ShapeList& notIsolated) {
    notIsolated.clear();
    ShapeList workGeo;
    for (std::size_t idx = 0; idx < geometry.size(); ++idx) {
        const Shape& geo = geometry[idx];
        ShapeList goodPass;
        for (int nrPass = 0; nrPass < passes; ++nrPass) {
            // Ojo: acá el original divide por 2.0 (no por 2.0000001) y usa la resolución por defecto del buffer (16).
            double isoOffset = toolDia * ((2 * nrPass + 1) / 2.0) - (nrPass * overlap * toolDia);
            if (negativeDia) isoOffset = -isoOffset;

            // La pasada vale si el buffer al doble de la distancia no toca a ningún otro polígono.
            const PreparedShape check(geo.buffer(isoOffset * 2));
            bool intersect = false;
            for (std::size_t j = 0; j < geometry.size() && !intersect; ++j) {
                if (j != idx && check.intersects(geometry[j])) intersect = true;
            }
            if (intersect) continue;

            const Shape temp = geo.buffer(isoOffset);
            // Solo en la primera pasada: si el polígono tiene más de un agujero y el buffer no conserva todos, alguno
            // no se pudo aislar con esta herramienta y se abandona el polígono entero.
            if (nrPass == 0 && forcedRest) {
                const std::size_t holes = geo.interiors().size();
                if (holes > 1) {
                    const std::size_t expected = 1 + holes;
                    std::size_t got = 1;
                    if (temp.type() == ShapeType::Polygon) got = 1 + temp.interiors().size();
                    else if (isMulti(temp.type())) got = temp.numParts();
                    if (got != expected) break;
                }
            }
            goodPass.push_back(temp);
        }
        if (!goodPass.empty()) {
            for (const Shape& g : goodPass)
                for (const Shape& p : partsOf(g)) workGeo.push_back(p);  // desvío: el original pierde las partes
        } else {
            notIsolated.push_back(geo);
        }
    }

    if (invert) workGeo = invertAll(workGeo);

    ShapeList isolated;
    for (const Shape& g : workGeo) {
        if (g.type() != ShapeType::Polygon) continue;
        if (type != IsolationType::Interior) isolated.push_back(g.exterior());
        if (type != IsolationType::Exterior)
            for (const Shape& h : g.interiors()) isolated.push_back(h);
    }
    return isolated;
}

ShapeList Isolation::referenceGeometry(const ShapeList& solidGeometry, const ShapeList& reference) {
    return partsOf(Shape::unaryUnion(solidGeometry).difference(Shape::unaryUnion(reference)));
}

std::string Isolation::objectName(const std::string& baseName, double toolDia, IsolationType type, int pass,
                                  int passes, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "_%.*f", decimals, toolDia);
    std::string name = baseName + buf;
    if (type == IsolationType::Exterior) name += "_ext";
    else if (type == IsolationType::Interior) name += "_int";
    name += "_iso";
    if (passes > 1) name += std::to_string(pass + 1);
    return name;
}

bool Isolation::isolate(const ShapeList& solidGeometry, const ShapeList& followGeometry,
                        const std::vector<IsolationTool>& tools, const IsolationOptions& o,
                        IsolationOutcome& outcome, std::string& error, const ShapeList* geometry) {
    error.clear();
    outcome = IsolationOutcome{};
    if (tools.empty()) {
        error = "No hay herramientas seleccionadas en la tabla";
        return false;
    }
    for (const IsolationTool& t : tools) {
        if (t.toolDia <= 0) {
            error = "El diámetro de la herramienta debe ser positivo";
            return false;
        }
    }
    const bool hasLimit = !o.limitedArea.empty();

    // Recorte común: resta del objeto elegido y recorte al área.
    auto postProcess = [&](ShapeList g) {
        if (o.useExcept) g = areaSubtraction(g, o.exceptGeometry);
        if (hasLimit) g = areaIntersection(g, o.limitedArea);
        return withoutEmpty(g);
    };
    // Figuras que recibe `isolation_geometry`: con una lista vacía el original vuelve al cobre del Gerber.
    auto workFor = [&](const IsolationTool& t) -> const ShapeList& {
        const ShapeList& work = geometry ? *geometry : (t.follow ? followGeometry : solidGeometry);
        return (!t.follow && work.empty()) ? solidGeometry : work;
    };

    try {
        if (o.combine && o.rest) {
            // combined_rest: herramientas ordenadas por diámetro (redondeado), cada una aísla lo que quedó sin aislar.
            std::vector<double> sorted;
            for (const IsolationTool& t : tools) sorted.push_back(roundedDia(t.toolDia, o.decimals));
            if (o.order == IsolationToolOrder::Forward) std::stable_sort(sorted.begin(), sorted.end());
            else if (o.order == IsolationToolOrder::Reverse)
                std::stable_sort(sorted.begin(), sorted.end(), [](double a, double b) { return a > b; });

            ShapeList work = geometry ? *geometry : solidGeometry;
            std::map<std::size_t, ShapeList> perTool;  // el diccionario de herramientas conserva el orden de la tabla
            for (double dia : sorted) {
                for (std::size_t i = 0; i < tools.size(); ++i) {
                    if (roundedDia(tools[i].toolDia, o.decimals) != dia) continue;
                    const IsolationTool& t = tools[i];
                    ShapeList remaining;
                    const ShapeList solid = generateRestGeometry(work, t.toolDia, t.passes, t.overlap,
                                                                 t.milling == MillingDirection::Climb, t.type,
                                                                 o.negativeDia, o.forcedRest, remaining);
                    work = remaining;
                    perTool[i] = postProcess(solid);
                    if (work.empty()) break;
                }
            }
            for (auto& [i, g] : perTool) {
                if (!g.empty()) outcome.sets.push_back({i, -1, std::move(g)});
            }
            outcome.notIsolated = work;
        } else if (o.combine) {
            // combined_normal: todas las pasadas de cada herramienta en una sola geometría.
            for (std::size_t i = 0; i < tools.size(); ++i) {
                const IsolationTool& t = tools[i];
                ShapeList solid;
                for (int p = 0; p < t.passes; ++p) {
                    ShapeList env;
                    try {
                        env = generateEnvelope(workFor(t), passOffset(t.toolDia, p, t.overlap, o.negativeDia),
                                               t.milling == MillingDirection::Climb, t.type, t.follow,
                                               o.stepsPerCircle);
                    } catch (const GeometryError&) {
                        continue;  // "Isolation geometry could not be generated."
                    }
                    solid.insert(solid.end(), env.begin(), env.end());
                }
                ShapeList g = postProcess(solid);
                if (!g.empty()) outcome.sets.push_back({i, -1, std::move(g)});
            }
        } else {
            // Un objeto por pasada. El original resta el objeto elegido dos veces (antes y dentro de `iso_init`).
            for (std::size_t i = 0; i < tools.size(); ++i) {
                const IsolationTool& t = tools[i];
                for (int p = 0; p < t.passes; ++p) {
                    ShapeList env;
                    try {
                        env = generateEnvelope(workFor(t), passOffset(t.toolDia, p, t.overlap, o.negativeDia),
                                               t.milling == MillingDirection::Climb, t.type, t.follow,
                                               o.stepsPerCircle);
                    } catch (const GeometryError&) {
                        continue;
                    }
                    ShapeList g = postProcess(env);
                    if (o.useExcept) g = areaSubtraction(g, o.exceptGeometry);
                    g = withoutEmpty(g);
                    if (!g.empty()) outcome.sets.push_back({i, p, std::move(g)});
                }
            }
        }
    } catch (const std::exception& e) {
        error = std::string("Error al aislar: ") + e.what();
        outcome.sets.clear();
        return false;
    }
    if (outcome.sets.empty()) {
        error = "La geometría de aislamiento quedó vacía";
        return false;
    }
    return true;
}

}  // namespace flatcam
