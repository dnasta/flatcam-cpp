#include "tools/Subtract.h"

#include <optional>

namespace flatcam {

namespace {

/// Las restas de una figura con cada figura del sustraendo que la toca; vacío si ninguna cambió nada.
std::optional<Shape> subtractEach(const Shape& target, const ShapeList& subs) {
    ShapeList diff;
    for (const Shape& s : subs) {
        if (!target.intersects(s)) continue;
        const Shape d = target.difference(s);
        if (!d.isEmpty()) diff.push_back(d);
    }
    if (diff.empty()) return std::nullopt;
    return Shape::unaryUnion(diff);
}

}  // namespace

SubtractGerberResult subtractGerber(const Gerber& target, const Gerber& sub) {
    ShapeList subSolid, subClear;
    for (const std::string& id : sub.apertureOrder())
        for (const ApertureGeometry& el : sub.apertures().at(id).geometry) {
            if (el.solid) subSolid.push_back(*el.solid);
            if (el.clear) subClear.push_back(*el.clear);
        }
    SubtractGerberResult r;
    std::vector<std::string> order = target.apertureOrder();
    for (const std::string& id : order) {
        Aperture ap = target.apertures().at(id);
        ap.geometry.clear();
        r.apertures[id] = ap;
    }
    if (!r.apertures.count("0")) {
        Aperture reg;
        reg.type = "REG";
        reg.size = 0.0;
        r.apertures["0"] = reg;
        order.push_back("0");
    }
    std::vector<ApertureGeometry> modified;
    for (const std::string& id : target.apertureOrder()) {
        std::vector<ApertureGeometry> kept;
        for (const ApertureGeometry& el : target.apertures().at(id).geometry) {
            ApertureGeometry dest;
            bool changed = false;
            if (el.solid) {
                const auto s = subtractEach(*el.solid, subSolid);
                dest.solid = s ? *s : *el.solid;
                changed = changed || s.has_value();
            }
            if (el.clear) {
                const auto c = subtractEach(*el.clear, subClear);
                dest.clear = c ? *c : *el.clear;
                changed = changed || c.has_value();
            }
            (changed ? modified : kept).push_back(dest);
        }
        Aperture& ap = r.apertures[id];
        ap.geometry.insert(ap.geometry.end(), kept.begin(), kept.end());
        // Los modificados van a la apertura "0", en el orden de las aperturas.
        r.apertures["0"].geometry.insert(r.apertures["0"].geometry.end(), modified.begin(), modified.end());
        modified.clear();
    }
    for (auto it = r.apertures.begin(); it != r.apertures.end();) {
        it->second.hasGeometry = !it->second.geometry.empty();
        if (it->second.geometry.empty()) it = r.apertures.erase(it);
        else ++it;
    }
    // MultiPolygon(poly_buff).buffer(1e-7).buffer(-1e-7), con el MultiPolygon de Shapely 1.8: los polígonos vacíos se
    // descartan, de un MultiPolygon de la lista solo queda su primera parte, y si el primero de la lista es un
    // MultiPolygon falla ("Sequences of multi-polygons are not valid arguments").
    ShapeList flat;
    for (const std::string& id : order) {
        const auto it = r.apertures.find(id);
        if (it == r.apertures.end()) continue;
        for (const ApertureGeometry& el : it->second.geometry) {
            if (!el.solid || el.solid->isEmpty()) continue;
            if (el.solid->type() == ShapeType::MultiPolygon) {
                if (flat.empty()) {
                    r.failed = true;
                    return r;
                }
                flat.push_back(el.solid->parts().front());
            } else {
                flat.push_back(*el.solid);
            }
        }
    }
    if (!flat.empty()) {
        const Shape s = Shape::collection(flat).buffer(0.0000001).buffer(-0.0000001);
        r.solid = s.parts().empty() ? ShapeList{s} : s.parts();
    }
    return r;
}

ShapeList subtractGeometry(const ShapeList& geo, const Shape& subUnion, bool closePaths) {
    ShapeList out;
    auto add = [&](const Shape& g) {
        const Shape d = g.difference(subUnion);
        if (!d.isEmpty()) out.push_back(d);
    };
    if (closePaths) {
        if (!geo.empty()) add(Shape::unaryUnion(geo));
        return out;
    }
    auto rings = [&](const Shape& poly) {
        add(poly.exterior());
        for (const Shape& h : poly.interiors()) add(h);
    };
    for (const Shape& g : geo) {
        switch (g.type()) {
            case ShapeType::Polygon: rings(g); break;
            case ShapeType::MultiPolygon:
                for (const Shape& p : g.parts()) rings(p);
                break;
            case ShapeType::LineString: add(g); break;
            case ShapeType::MultiLineString:
                for (const Shape& l : g.parts()) add(l);
                break;
            default: break;
        }
    }
    return out;
}

}  // namespace flatcam
