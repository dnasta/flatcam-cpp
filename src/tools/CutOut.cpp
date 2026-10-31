#include "tools/CutOut.h"

#include <cmath>

namespace flatcam {

namespace {

bool isMulti(ShapeType t) { return static_cast<int>(t) >= static_cast<int>(ShapeType::MultiPoint); }

void flattenInto(const Shape& g, ShapeList& out) {
    if (g.isEmpty()) return;
    const ShapeType t = g.type();
    if (isMulti(t)) {
        for (const Shape& part : g.parts()) flattenInto(part, out);
    } else if (t == ShapeType::Polygon) {
        out.push_back(g.exterior());  // los agujeros se descartan, como en el original
    } else {
        out.push_back(g);
    }
}

/// Agrega las figuras de un resultado de unión: si es múltiple se itera por partes (como `for g in geom`).
void emitParts(const Shape& g, ShapeList& out) {
    if (g.isEmpty()) return;
    if (isMulti(g.type())) {
        for (const Shape& p : g.parts()) {
            if (!p.isEmpty()) out.push_back(p);
        }
    } else {
        out.push_back(g);
    }
}

Shape boxPolygon(double x0, double y0, double x1, double y1) {
    return Shape::polygon({{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}});
}

/// Gestiona la lista de la que se parte: una lista de figuras, o una sola figura.
struct HandlerResult {
    ShapeList proc;
    ShapeList rest;
};

/// `cutout_handler` de `on_freeform_cutout`: abre los puentes en `geom` y devuelve el contorno y los tramos de puente.
HandlerResult cutoutHandler(const ShapeList& geom, double gapsize, CutoutGaps gaps, double margin) {
    HandlerResult out;
    const ShapeList initial = geom;
    const auto bounds = CutOut::recursiveBounds(geom);
    if (!bounds) return out;
    const double xxmin = bounds->xmin, yymin = bounds->ymin, xxmax = bounds->xmax, yymax = bounds->ymax;
    const double px = 0.5 * (xxmin + xxmax) + margin;
    const double py = 0.5 * (yymin + yymax) + margin;
    const double lenx = (xxmax - xxmin) + (margin * 2);
    const double leny = (yymax - yymin) + (margin * 2);

    ShapeList current = geom;
    bool fromUnion = false;
    std::vector<ShapeList> restTemp;

    auto cut = [&](double x0, double y0, double x1, double y1) {
        current = {CutOut::subtractBoxFromGeo(current, x0, y0, x1, y1)};
        fromUnion = true;
        restTemp.push_back(CutOut::intersectGeo(initial, boxPolygon(x0, y0, x1, y1)));
    };

    if (gaps != CutoutGaps::None) {
        if (gaps == CutoutGaps::Eight || gaps == CutoutGaps::TwoLR) {
            cut(xxmin - gapsize, py - gapsize + leny / 4, xxmax + gapsize, py + gapsize + leny / 4);
            cut(xxmin - gapsize, py - gapsize - leny / 4, xxmax + gapsize, py + gapsize - leny / 4);
        }
        if (gaps == CutoutGaps::Eight || gaps == CutoutGaps::TwoTB) {
            cut(px - gapsize + lenx / 4, yymin - gapsize, px + gapsize + lenx / 4, yymax + gapsize);
            cut(px - gapsize - lenx / 4, yymin - gapsize, px + gapsize - lenx / 4, yymax + gapsize);
        }
        if (gaps == CutoutGaps::Four || gaps == CutoutGaps::LR) {
            cut(xxmin - gapsize, py - gapsize, xxmax + gapsize, py + gapsize);
        }
        if (gaps == CutoutGaps::Four || gaps == CutoutGaps::TB) {
            cut(px - gapsize, yymin - gapsize, px + gapsize, yymax + gapsize);
        }
    }

    for (const Shape& g : current) {
        if (fromUnion) emitParts(g, out.proc);
        else if (!g.isEmpty()) out.proc.push_back(g);
    }
    for (const ShapeList& list : restTemp) {
        for (const Shape& g : CutOut::flatten(list)) {
            if (!g.isEmpty()) out.rest.push_back(g);
        }
    }
    return out;
}

/// Contorno exterior de un polígono; falla si la figura no es un polígono (el original lanza una excepción).
bool exteriorOf(const Shape& g, Shape& ring, std::string& error) {
    if (g.type() != ShapeType::Polygon) {
        error = "El contorno desplazado no es un polígono único: no se puede tomar su borde exterior";
        return false;
    }
    ring = g.exterior();
    return true;
}

}  // namespace

ShapeList CutOut::flatten(const ShapeList& geometry) {
    ShapeList out;
    for (const Shape& g : geometry) flattenInto(g, out);
    return out;
}

std::optional<BoundingBox> CutOut::recursiveBounds(const ShapeList& geometry) {
    std::optional<BoundingBox> acc;
    for (const Shape& s : geometry) {
        if (s.isEmpty()) continue;
        const auto b = s.bounds();
        if (!b) continue;
        acc = acc ? acc->merged(*b) : *b;
    }
    return acc;
}

Shape CutOut::subtractBoxFromGeo(const ShapeList& geometry, double xmin, double ymin, double xmax, double ymax) {
    const Shape tool = boxPolygon(xmin, ymin, xmax, ymax);
    ShapeList diffs;
    for (const Shape& target : flatten(geometry)) {
        if (target.type() == ShapeType::LineString || target.type() == ShapeType::LinearRing) {
            diffs.push_back(target.difference(tool));
        }
    }
    return Shape::unaryUnion(diffs);
}

Shape CutOut::subtractGeo(const ShapeList& target, const Shape& subtractor) {
    ShapeList diffs;
    for (const Shape& t : flatten(target)) {
        if (t.type() == ShapeType::LineString || t.type() == ShapeType::LinearRing ||
            t.type() == ShapeType::MultiLineString) {
            diffs.push_back(t.difference(subtractor));
        }
    }
    return Shape::unaryUnion(diffs);
}

ShapeList CutOut::intersectGeo(const ShapeList& target, const Shape& second) {
    ShapeList results;
    for (const Shape& geo : target) {
        if (second.intersects(geo)) results.push_back(second.intersection(geo));
    }
    return flatten(results);
}

Shape CutOut::cuttingBox(const Point2D& pos, double toolDia, double gapSize) {
    const double offset = toolDia / 2 + gapSize / 2;
    return Shape::box(pos.x - offset, pos.y - offset, pos.x + offset, pos.y + offset);
}

Shape CutOut::manualGap(const Shape& solidGeometry, const Point2D& pos, double toolDia, double gapSize) {
    const Shape cutPoly = cuttingBox(pos, toolDia, gapSize);
    const Shape remaining = subtractGeo({solidGeometry}, cutPoly);
    ShapeList lines;
    emitParts(remaining, lines);
    return Shape::lineMerge(lines);
}

bool CutOut::freeform(const ShapeList& objectGeometry, bool isGerber, const CutoutOptions& o, CutoutResult& result,
                      std::string& error) {
    error.clear();
    result = CutoutResult{};
    if (o.toolDia == 0.0) {
        error = "El diámetro de la herramienta es cero: cambialo por un número positivo";
        return false;
    }
    try {
        double gapsize = o.gapSize / 2 + (o.toolDia / 2);
        const double mbBuff = o.mbDia / 2.0;
        ShapeList solid, gapsSolid, mouseBitesGeo;

        // Objeto de origen: con la envolvente convexa el objeto pasa a ser un único polígono.
        ShapeList objectGeo = objectGeometry;
        if (isGerber && o.convexBox && !objectGeo.empty()) objectGeo = {Shape::unaryUnion(objectGeo).convexHull()};

        const bool single = o.kind == CutoutKind::Single;
        const bool thin = o.thinBridges && o.thinDepth != 0.0;

        // Convierte el objeto en la línea a recortar, para `single`.
        auto singleRing = [&](double bufferDist, bool plainBuffer, Shape& ring) -> bool {
            Shape u = Shape::unaryUnion(objectGeo);
            if (u.type() == ShapeType::MultiPolygon) {
                const auto b = u.bounds();
                if (!b) return false;
                u = Shape::box(b->xmin, b->ymin, b->xmax, b->ymax);
            }
            const Shape buf = u.buffer(plainBuffer ? 0.0 : bufferDist);
            return exteriorOf(buf, ring, error);
        };

        if (single) {
            Shape ring;
            double dist = o.margin >= 0 ? o.margin + std::fabs(o.toolDia / 2) : o.margin - std::fabs(o.toolDia / 2);
            if (!singleRing(dist, !isGerber, ring)) return false;
            HandlerResult h = cutoutHandler({ring}, gapsize, o.gaps, o.margin);
            solid = h.proc;
            if (thin) gapsSolid = h.rest;
        } else {
            for (const Shape& geomStruct : objectGeo) {
                ShapeList geomList;
                if (isGerber) {
                    if (o.margin >= 0) {
                        Shape ring;
                        if (!exteriorOf(geomStruct.buffer(o.margin + std::fabs(o.toolDia / 2)), ring, error)) return false;
                        geomList = {ring};
                    } else {
                        const Shape buf = geomStruct.buffer(-o.margin + std::fabs(o.toolDia / 2));
                        if (buf.type() != ShapeType::Polygon) {
                            error = "El contorno desplazado no es un polígono único";
                            return false;
                        }
                        geomList = buf.interiors();
                    }
                } else {
                    geomList = {geomStruct};
                }
                HandlerResult h = cutoutHandler(geomList, gapsize, o.gaps, o.margin);
                solid.insert(solid.end(), h.proc.begin(), h.proc.end());
                if (thin) gapsSolid.insert(gapsSolid.end(), h.rest.begin(), h.rest.end());
            }
        }
        if (solid.empty()) {
            error = "No quedó ninguna línea de corte";
            return false;
        }
        result.solidGeometry = Shape::lineMerge(solid);

        if (o.mouseBites) {
            gapsize -= o.toolDia / 2;
            if (single) {
                Shape ring;
                double dist = o.margin >= 0 ? o.margin + mbBuff : o.margin - mbBuff;
                if (!singleRing(dist, !isGerber, ring)) return false;
                mouseBitesGeo = cutoutHandler({ring}, gapsize, o.gaps, o.margin).rest;
            } else {
                for (const Shape& geomStruct : objectGeo) {
                    ShapeList geomList;
                    if (isGerber) {
                        if (o.margin >= 0) {
                            Shape ring;
                            if (!exteriorOf(geomStruct.buffer(o.margin + mbBuff), ring, error)) return false;
                            geomList = {ring};
                        } else {
                            const Shape buf = geomStruct.buffer(-o.margin + mbBuff);
                            if (buf.type() != ShapeType::Polygon) {
                                error = "El contorno desplazado no es un polígono único";
                                return false;
                            }
                            geomList = buf.interiors();
                        }
                    } else {
                        geomList = {geomStruct};
                    }
                    const HandlerResult h = cutoutHandler(geomList, gapsize, o.gaps, o.margin);
                    mouseBitesGeo.insert(mouseBitesGeo.end(), h.rest.begin(), h.rest.end());
                }
            }
            for (const Shape& line : mouseBitesGeo) {
                double len = 0;
                const double total = line.length();
                while (len < total) {
                    const auto c = line.interpolate(len).coordinates();
                    result.mouseBiteHoles.push_back(c.front());
                    len += o.mbDia + o.mbSpacing;
                }
            }
        }
        result.gapsSolidGeometry = gapsSolid;
    } catch (const std::exception& e) {
        error = std::string("Error en el corte de contorno: ") + e.what();
        return false;
    }
    return true;
}


namespace {

/// `cutout_rect_handler`: abre los puentes en un rectángulo; devuelve las líneas restantes.
ShapeList rectHandler(const Shape& geom, double gapsize, CutoutGaps gaps, double margin, double xmin, double ymin,
                      double xmax, double ymax) {
    const double px = 0.5 * (xmin + xmax) + margin;
    const double py = 0.5 * (ymin + ymax) + margin;
    const double lenx = (xmax - xmin) + (margin * 2);
    const double leny = (ymax - ymin) + (margin * 2);
    ShapeList current{geom};
    bool fromUnion = false;
    auto cut = [&](double x0, double y0, double x1, double y1) {
        current = {CutOut::subtractBoxFromGeo(current, x0, y0, x1, y1)};
        fromUnion = true;
    };
    if (gaps != CutoutGaps::None) {
        if (gaps == CutoutGaps::Eight || gaps == CutoutGaps::TwoLR) {
            cut(xmin - gapsize, py - gapsize + leny / 4, xmax + gapsize, py + gapsize + leny / 4);
            cut(xmin - gapsize, py - gapsize - leny / 4, xmax + gapsize, py + gapsize - leny / 4);
        }
        if (gaps == CutoutGaps::Eight || gaps == CutoutGaps::TwoTB) {
            cut(px - gapsize + lenx / 4, ymin - gapsize, px + gapsize + lenx / 4, ymax + gapsize);
            cut(px - gapsize - lenx / 4, ymin - gapsize, px + gapsize - lenx / 4, ymax + gapsize);
        }
        if (gaps == CutoutGaps::Four || gaps == CutoutGaps::LR) cut(xmin - gapsize, py - gapsize, xmax + gapsize, py + gapsize);
        if (gaps == CutoutGaps::Four || gaps == CutoutGaps::TB) cut(px - gapsize, ymin - gapsize, px + gapsize, ymax + gapsize);
    }
    ShapeList out;
    for (const Shape& g : current) {
        if (fromUnion) emitParts(g, out);
        else out.push_back(g);
    }
    return out;
}

/// Líneas de una figura resultado de una resta (como `list += geometry` del original).
void appendParts(ShapeList& list, const Shape& g) { emitParts(g, list); }

}  // namespace

bool CutOut::rectangular(const ShapeList& objectGeometry, bool isGerber, const CutoutOptions& o, CutoutResult& result,
                         std::string& error) {
    error.clear();
    result = CutoutResult{};
    if (o.toolDia == 0.0) {
        error = "El diámetro de la herramienta es cero: cambialo por un número positivo";
        return false;
    }
    if (o.gaps == CutoutGaps::None) {
        error = "Sin puentes el corte rectangular no genera líneas (el original también falla)";
        return false;
    }
    try {
        const double gapsize = o.gapSize / 2 + (o.toolDia / 2);
        const double mbBuff = o.mbDia / 2.0;
        const bool single = o.kind == CutoutKind::Single;
        const bool thin = o.thinBridges && o.thinDepth != 0.0;
        ShapeList solid, gapsSolid, mouseBitesGeo;

        auto boundsOfShape = [](const Shape& s, double& x0, double& y0, double& x1, double& y1) {
            const auto b = s.bounds();
            if (!b) return false;
            x0 = b->xmin;
            y0 = b->ymin;
            x1 = b->xmax;
            y1 = b->ymax;
            return true;
        };
        auto grow = [&](const Shape& box, double extra) {
            return box.buffer(o.margin >= 0 ? o.margin + extra : o.margin - extra);
        };

        if (single) {
            const Shape u = Shape::unaryUnion(objectGeometry);
            double xmin = 0, ymin = 0, xmax = 0, ymax = 0;
            if (!boundsOfShape(u, xmin, ymin, xmax, ymax)) {
                error = "El objeto no tiene límites";
                return false;
            }
            Shape geo = Shape::box(xmin, ymin, xmax, ymax);
            if (isGerber) geo = grow(geo, std::fabs(o.toolDia / 2));
            solid = rectHandler(geo, gapsize, o.gaps, o.margin, xmin, ymin, xmax, ymax);
            if (thin) appendParts(gapsSolid, subtractGeo({geo}, Shape::unaryUnion(solid)));
        } else {
            if (isGerber && o.margin < 0) {
                error = "El corte rectangular con margen negativo no es posible";
                return false;
            }
            for (const Shape& item : objectGeometry) {
                const Shape u = Shape::unaryUnion({item});
                double xmin = 0, ymin = 0, xmax = 0, ymax = 0;
                if (!boundsOfShape(u, xmin, ymin, xmax, ymax)) continue;
                Shape geomStruct = Shape::box(xmin, ymin, xmax, ymax);
                if (isGerber) geomStruct = geomStruct.buffer(o.margin + std::fabs(o.toolDia / 2));
                const ShapeList c = rectHandler(geomStruct, gapsize, o.gaps, o.margin, xmin, ymin, xmax, ymax);
                solid.insert(solid.end(), c.begin(), c.end());
                if (thin) appendParts(gapsSolid, subtractGeo({geomStruct}, Shape::unaryUnion(c)));
            }
        }
        if (solid.empty()) {
            error = "No quedó ninguna línea de corte";
            return false;
        }
        result.solidGeometry = Shape::lineMerge(solid);

        if (o.mouseBites) {
            const double gs = gapsize - o.toolDia / 2;
            if (single) {
                const Shape u = Shape::unaryUnion(objectGeometry);
                double xmin = 0, ymin = 0, xmax = 0, ymax = 0;
                if (!boundsOfShape(u, xmin, ymin, xmax, ymax)) {
                    error = "El objeto no tiene límites";
                    return false;
                }
                Shape mbGeo = Shape::box(xmin, ymin, xmax, ymax);
                mbGeo = isGerber ? grow(mbGeo, mbBuff) : mbGeo.buffer(0);
                const ShapeList mbSolid = rectHandler(mbGeo, gs, o.gaps, o.margin, xmin, ymin, xmax, ymax);
                appendParts(mouseBitesGeo, subtractGeo({mbGeo}, Shape::unaryUnion(mbSolid)));
            } else {
                // El original suma líneas a una geometría ya unida (`solid_geo += c_geo`) y lanza un TypeError.
                error = "Los mouse bites del corte rectangular con varias figuras fallan en el original (TypeError)";
                return false;
            }
            for (const Shape& line : mouseBitesGeo) {
                double len = 0;
                const double total = line.length();
                while (len < total) {
                    result.mouseBiteHoles.push_back(line.interpolate(len).coordinates().front());
                    len += o.mbDia + o.mbSpacing;
                }
            }
        }
        result.gapsSolidGeometry = gapsSolid;
    } catch (const std::exception& e) {
        error = std::string("Error en el corte rectangular: ") + e.what();
        return false;
    }
    return true;
}

}  // namespace flatcam
