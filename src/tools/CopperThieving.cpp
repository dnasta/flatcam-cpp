#include "tools/CopperThieving.h"

#include <memory>

#include "core/GeometryError.h"

namespace flatcam {

namespace {

constexpr int kRes = 16;  // resolución por defecto de buffer() en Shapely

bool isMulti(const Shape& s) {
    const ShapeType t = s.type();
    return t == ShapeType::MultiPoint || t == ShapeType::MultiLineString || t == ShapeType::MultiPolygon ||
           t == ShapeType::GeometryCollection;
}

/// ToolCopperThieving.flatten: recorre las colecciones y agrega el resto (también las figuras vacías).
void flatten(const Shape& s, ShapeList& out) {
    if (isMulti(s)) {
        for (const Shape& p : s.parts()) flatten(p, out);
    } else {
        out.push_back(s);
    }
}

/// MultiPolygon(geo_list).buffer(0.0000001).buffer(-0.0000001), en partes.
ShapeList bufferedUnion(const ShapeList& list) {
    if (list.empty()) return {};
    const Shape u = Shape::collection(list).buffer(0.0000001, kRes).buffer(-0.0000001, kRes);
    if (u.isEmpty()) return {};
    return u.parts();
}

/// max(ap_keys) como texto (std::map ordena igual que Python compara las cadenas).
std::string maxKey(const std::map<std::string, Aperture>& aps) { return aps.rbegin()->first; }

}  // namespace

ThievingResult copperThieving(const Gerber& g, const ThievingParams& p, const ShapeList& area,
                              const ThievingBoxReference& box, ShapeList& flatGeometry) {
    ThievingResult r;
    const ShapeList& solid = g.solidGeometry();
    const int res = p.circleSteps / 4;
    try {
        // La separación alrededor del cobre.
        ShapeList clear;
        clear.reserve(solid.size());
        for (const Shape& pol : solid) clear.push_back(pol.buffer(p.clearance, res));
        const Shape clearance = Shape::unaryUnion(clear);

        // La zona que se rellena.
        Shape bbox;
        if (p.reference == ThievingReference::Itself) {
            if (solid.empty()) {
                r.error = "No object available.";
                return r;
            }
            if (p.minimalBox) {
                // Con un solo polígono, la caja "mínima" es el polígono (unary_union sin convex_hull).
                const Shape env = solid.size() == 1 ? Shape::unaryUnion(solid) : Shape::collection(solid).convexHull();
                bbox = env.buffer(p.margin, kRes, JoinStyle::Mitre);
            } else {
                const BoundingBox b = Shape::collection(solid).bounds().value_or(BoundingBox{});
                bbox = Shape::box(b.xmin, b.ymin, b.xmax, b.ymax).buffer(p.margin, kRes, JoinStyle::Mitre);
            }
        } else if (p.reference == ThievingReference::Area) {
            const Shape u = Shape::unaryUnion(area);
            ShapeList buffs;
            for (const Shape& poly : u.parts()) buffs.push_back(poly.buffer(p.margin, kRes, JoinStyle::Mitre));
            bbox = Shape::collection(buffs);
        } else if (box.kind == "geometry") {
            ShapeList buffs;
            for (const Shape& poly : box.solid) buffs.push_back(poly.buffer(p.margin, kRes, JoinStyle::Mitre));
            bbox = Shape::unaryUnion(buffs);
        } else if (box.kind == "gerber") {
            const Shape ref = Shape::unaryUnion(box.solid).convexHull();
            bbox = Shape::unaryUnion(solid).convexHull().intersection(ref).buffer(p.margin, kRes, JoinStyle::Mitre);
        } else {
            r.error = "The reference object type is not supported.";
            return r;
        }

        // El relleno sólido: la zona menos la separación, sin las partes chicas.
        ShapeList thief;
        for (const Shape& s : bbox.difference(clearance).parts())
            if (s.area() >= p.minArea) thief.push_back(s);
        r.thieving = thief;

        // La caja del Gerber más el margen, para las grillas.
        const Shape gridBox = Shape::collection(solid).envelope().buffer(p.margin, kRes, JoinStyle::Mitre);
        const BoundingBox gb = gridBox.bounds().value_or(BoundingBox{});
        const double x0 = gb.xmin, y0 = gb.ymin, x1 = gb.xmax, y1 = gb.ymax;

        if (p.fill == ThievingFill::Dot || p.fill == ThievingFill::Square) {
            ShapeList cells;
            const bool dot = p.fill == ThievingFill::Dot;
            const double h = dot ? p.dotDia / 2.0 : p.squareSize / 2.0;
            const double step = dot ? p.dotDia + p.dotSpacing : p.squareSize + p.squareSpacing;
            double nx = x0 + h, ny = y0 + h;
            while (nx <= x1 - h) {
                while (ny <= y1 - h) {
                    if (dot) {
                        cells.push_back(Shape::point({nx, ny}).buffer(h, 64));
                    } else {
                        const BoundingBox c = Shape::point({nx, ny}).buffer(h).bounds().value_or(BoundingBox{});
                        cells.push_back(Shape::box(c.xmin, c.ymin, c.xmax, c.ymax));
                    }
                    ny += step;
                }
                nx += step;
                ny = y0 + h;
            }
            // El centroide de un MultiPolygon vacío no tiene x: el original falla en su hilo.
            if (cells.empty()) return r;
            const Point2D bc = gridBox.centroid();
            const Point2D cc = Shape::collection(cells).centroid();
            const double dx = bc.x - cc.x, dy = bc.y - cc.y;
            std::vector<std::unique_ptr<PreparedShape>> prep;
            for (const Shape& t : thief) prep.push_back(std::make_unique<PreparedShape>(t));
            ShapeList out;
            for (const Shape& c : cells) {
                const Shape moved = c.translate(dx, dy);
                for (const auto& t : prep)
                    if (t->contains(moved)) out.push_back(moved);
            }
            thief = std::move(out);
            r.thieving = thief;
        }

        if (p.fill == ThievingFill::Line) {
            const double half = p.lineSize / 2.0;
            ShapeList outline;
            for (const Shape& pol : solid) outline.push_back(pol.buffer(p.clearance + half, res));
            const Shape outlineUnion = Shape::unaryUnion(outline);
            ShapeList rings;
            for (const Shape& o : outlineUnion.parts())
                if (!o.isEmpty()) rings.push_back(o.exterior().buffer(half, res));
            const Shape outlineGeo = Shape::unaryUnion(rings);

            const Shape boxIn = Shape::box(x0, y0, x1, y1).buffer(-half);
            const Shape boxOutline = boxIn.exterior().buffer(half, res);
            const BoundingBox bb = boxIn.bounds().value_or(BoundingBox{});
            ShapeList lines;
            double nx = bb.xmin, ny = bb.ymin;
            while (nx <= x1 - half) {
                lines.push_back(Shape::lineString({{nx, bb.ymin}, {nx, bb.ymax}}).buffer(half, res));
                nx += p.lineSize + p.lineSpacing;
            }
            while (ny <= y1 - half) {
                lines.push_back(Shape::lineString({{bb.xmin, ny}, {bb.xmax, ny}}).buffer(half, res));
                ny += p.lineSize + p.lineSpacing;
            }
            flatten(outlineGeo, flatGeometry);
            flatten(boxOutline, flatGeometry);
            for (const Shape& l : lines) flatten(l.difference(clearance), flatGeometry);
            thief = flatGeometry;
            r.thieving = thief;
        }

        // El Gerber nuevo: el relleno va a la apertura 0.
        r.apertures = g.apertures();
        Aperture& zero = r.apertures["0"];
        if (zero.type.empty()) {
            zero.type = "REG";
            zero.size = 0.0;
        }
        zero.hasGeometry = true;
        ShapeList all = solid;
        for (const Shape& poly : thief) {
            all.push_back(poly);
            zero.geometry.push_back({poly, poly.exterior(), std::nullopt});
        }
        r.solid = bufferedUnion(all);
        r.ok = true;
    } catch (const GeometryError&) {
        r.ok = false;
    }
    return r;
}

RobberBarResult robberBar(const Gerber& g, double margin, double thickness) {
    RobberBarResult r;
    const ShapeList& solid = g.solidGeometry();
    const Shape outline = Shape::collection(solid).envelope();
    r.line = outline.buffer(margin + thickness / 2.0).exterior();
    r.bar = r.line.buffer(thickness / 2.0);
    r.apertures = g.apertures();
    std::optional<std::string> found;
    for (const std::string& id : g.apertureOrder()) {
        const Aperture& a = g.apertures().at(id);
        if (a.type == "C" && a.size && *a.size == thickness) {
            found = id;
            break;
        }
    }
    std::string id;
    if (found) {
        id = *found;
    } else {
        id = r.apertures.empty() ? "10" : std::to_string(std::stoi(maxKey(r.apertures)) + 1);
        Aperture ap;
        ap.type = "C";
        ap.size = thickness;
        r.apertures[id] = ap;
        r.newAperture = id;
    }
    Aperture& ap = r.apertures[id];
    ap.hasGeometry = true;
    ap.geometry.push_back({r.bar, r.line, std::nullopt});
    ShapeList all = solid;
    all.push_back(r.bar);
    const Shape u = Shape::unaryUnion(all);
    r.solid = u.isEmpty() ? ShapeList{} : u.parts();
    r.follow = g.followGeometry();
    r.follow.push_back(r.line);
    return r;
}

namespace {
PlatingMaskResult platingMaskImpl(const Gerber& sm, double clearance, char choice, const ShapeList& thieving,
                                  const std::optional<Shape>& robber, const std::optional<Shape>& robberLine,
                                  double rbThickness);
}  // namespace

PlatingMaskResult platingMask(const Gerber& sm, double clearance, char choice, const ShapeList& thieving,
                              const std::optional<Shape>& robber, const std::optional<Shape>& robberLine,
                              double rbThickness) {
    PlatingMaskResult r;
    try {
        r = platingMaskImpl(sm, clearance, choice, thieving, robber, robberLine, rbThickness);
    } catch (const GeometryError&) {
        r = PlatingMaskResult{};  // el exterior de un relleno achicado en varias partes: AttributeError
    }
    return r;
}

namespace {

PlatingMaskResult platingMaskImpl(const Gerber& sm, double clearance, char choice, const ShapeList& thieving,
                                  const std::optional<Shape>& robber, const std::optional<Shape>& robberLine,
                                  double rbThickness) {
    PlatingMaskResult r;
    ShapeList geo = sm.solidGeometry();
    r.apertures = sm.apertures();
    std::vector<std::string> order = sm.apertureOrder();
    if (clearance < 0) {
        for (Shape& s : geo) s = s.buffer(clearance);
        for (auto& [id, ap] : r.apertures) {
            const Aperture& src = sm.apertures().at(id);
            ap.geometry.clear();
            if (!src.hasGeometry) continue;
            for (const ApertureGeometry& e : src.geometry) {
                ApertureGeometry n = e;
                if (e.solid) n.solid = e.solid->buffer(clearance);
                ap.geometry.push_back(n);
            }
        }
    }
    for (const Shape& s : geo) r.platedArea += s.area();
    r.follow = sm.followGeometry();
    auto addAperture = [&](const std::string& id) {
        order.push_back(id);
        r.added.push_back(id);
    };
    if (!thieving.empty() && (choice == 'b' || choice == 't')) {
        for (const Shape& s : thieving) r.platedArea += s.area();
        if (!r.apertures.count("0")) {
            Aperture z;
            z.type = "REG";
            z.size = 0.0;
            r.apertures["0"] = z;
            addAperture("0");
        }
        Aperture& zero = r.apertures["0"];
        zero.hasGeometry = true;
        for (const Shape& poly : thieving) {
            const Shape b = poly.buffer(clearance);
            geo.push_back(b);
            zero.geometry.push_back({b, b.exterior(), std::nullopt});
        }
    }
    if (robber && !robber->isEmpty() && (choice == 'b' || choice == 'r')) {
        r.platedArea += robber->area();
        r.follow.push_back(*robberLine);
        std::optional<std::string> found;
        for (const std::string& id : order) {
            const Aperture& a = r.apertures.at(id);
            if (a.type == "C" && a.size && *a.size == rbThickness + clearance) {
                found = id;
                break;
            }
        }
        if (found) {
            Aperture& ap = r.apertures[*found];
            ap.hasGeometry = true;
            ap.geometry.push_back({*robber, *robberLine, std::nullopt});
        } else {
            if (r.apertures.empty()) return r;  // max() de una lista vacía: ValueError
            const int maxId = std::stoi(maxKey(r.apertures));
            const std::string id = maxId != 0 ? std::to_string(maxId + 1) : "10";
            Aperture ap;
            ap.type = "C";
            ap.size = rbThickness + clearance;
            ap.hasGeometry = true;
            ap.geometry.push_back({robber->buffer(clearance), *robberLine, std::nullopt});
            r.apertures[id] = ap;
            addAperture(id);
        }
        geo.push_back(robber->buffer(clearance));
    }
    r.solid = bufferedUnion(geo);
    r.ok = true;
    return r;
}

}  // namespace

}  // namespace flatcam
