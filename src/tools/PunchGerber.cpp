#include "tools/PunchGerber.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <vector>

namespace flatcam {

namespace {

bool isFlash(const ApertureGeometry& el) { return el.follow && el.follow->type() == ShapeType::Point; }

Shape unionOf(const ShapeList& l) { return l.empty() ? Shape() : Shape::unaryUnion(l); }

ShapeList partsOf(const Shape& s) {
    if (s.isEmpty()) return {};
    const ShapeList p = s.parts();
    return p.empty() ? ShapeList{s} : p;
}

double roundTo(double v, int dec) {
    const double p = std::pow(10.0, dec);
    return std::round(v * p) / p;
}

/// Agrega las aperturas nuevas con los agujeros, numeradas desde la mayor.
void addHoleApertures(PunchResult& r, const std::vector<std::pair<double, std::vector<Point2D>>>& holes) {
    int mx = 0;
    for (const auto& [k, v] : r.apertures) mx = std::max(mx, std::stoi(k));
    for (const auto& [size, pts] : holes) {
        Aperture ap;
        ap.type = "C";
        ap.size = size;
        ap.hasGeometry = true;
        for (const Point2D& p : pts) {
            ApertureGeometry el;
            el.clear = Shape::point(p);
            ap.geometry.push_back(el);
        }
        const std::string id = std::to_string(++mx);
        r.apertures[id] = ap;
        r.newApertures.push_back(id);
    }
}

/// Las perforaciones dentro de cada pad (sus centros), agrupadas por diámetro en el orden en que aparecen.
void collectHoles(std::vector<std::pair<double, std::vector<Point2D>>>& holes, double size, const Shape& pad,
                  const ShapeList& punches) {
    for (const Shape& p : punches) {
        if (!p.within(pad)) continue;
        auto it = std::find_if(holes.begin(), holes.end(), [&](const auto& h) { return h.first == size; });
        if (it == holes.end()) {
            holes.push_back({size, {}});
            it = holes.end() - 1;
        }
        it->second.push_back(p.centroid());
    }
}

}  // namespace

PunchResult punchGerber(const Gerber& g, const std::set<std::string>& selected, const PunchSettings& s,
                        const Excellon* exc) {
    PunchResult r;
    r.apertures = g.apertures();
    const std::vector<std::string> order = g.apertureOrder();
    std::vector<std::pair<double, std::vector<Point2D>>> holes;

    if (s.method == PunchSettings::Method::Excellon) {
        ShapeList target, other;
        for (const std::string& id : order)
            for (const ApertureGeometry& el : g.apertures().at(id).geometry)
                if (el.solid) (selected.count(id) ? target : other).push_back(*el.solid);
        ShapeList excSolid;
        if (exc)
            for (const auto& [id, t] : exc->drillTools()) excSolid.insert(excSolid.end(), t.solidGeometry.begin(), t.solidGeometry.end());
        const Shape punched = unionOf(target).difference(unionOf(excSolid));
        ShapeList all = partsOf(punched);
        all.insert(all.end(), other.begin(), other.end());
        r.solid = partsOf(unionOf(all));
        for (const std::string& id : order) {
            if (!selected.count(id)) continue;
            for (const ApertureGeometry& el : g.apertures().at(id).geometry) {
                if (!el.solid || !isFlash(el) || !exc) continue;
                for (const auto& [tid, t] : exc->drillTools()) {
                    ShapeList pts;
                    for (const Point2D& d : t.drills) pts.push_back(Shape::point(d));
                    collectHoles(holes, t.tooldia, *el.solid, pts);
                }
            }
        }
        addHoleApertures(r, holes);
        return r;
    }

    const Shape solid = unionOf(g.solidGeometry());
    if (s.method == PunchSettings::Method::Fixed) {
        const double punch = s.fixedDia;
        if (punch == 0.0) {
            r.error = PunchResult::Error::ZeroDiameter;
            return r;
        }
        ShapeList punching;
        for (const std::string& id : order) {
            if (!selected.count(id)) continue;
            const Aperture& ap = g.apertures().at(id);
            bool use = false;
            std::optional<double> limitA, limitB;  // el agujero tiene que ser menor que estas medidas
            if (ap.type == "C" && s.circular) {
                use = true;
                limitA = ap.size;
            } else if (ap.type == "R") {
                const bool sq = roundTo(ap.width, s.decimals) == roundTo(ap.height, s.decimals);
                if ((sq && s.square) || (!sq && s.rectangular)) {
                    use = true;
                    limitA = ap.width;
                    limitB = ap.height;
                }
            } else if (ap.type == "O" && s.oblong) {
                use = true;
                limitA = ap.size;
            } else if (ap.type != "C" && ap.type != "R" && ap.type != "O" && s.other) {
                use = true;
                limitA = ap.size;
            }
            if (!use) continue;
            for (const ApertureGeometry& el : ap.geometry) {
                if (!isFlash(el)) continue;
                if (!limitA) {  // float(apertures[apid]['size']) de una macro: KeyError
                    r.error = PunchResult::Error::OriginalFails;
                    return r;
                }
                if (punch >= *limitA || (limitB && punch >= *limitB)) {
                    r.error = PunchResult::Error::HoleTooBig;
                    return r;
                }
                punching.push_back(el.follow->buffer(punch / 2));
            }
        }
        if (punching.empty()) {  // la diferencia con nada da la misma geometría
            r.error = PunchResult::Error::SameGeometry;
            return r;
        }
        r.solid = partsOf(solid.difference(unionOf(punching)));
        for (const std::string& id : order)
            for (const ApertureGeometry& el : g.apertures().at(id).geometry)
                if (el.solid && isFlash(el)) collectHoles(holes, punch, *el.solid, punching);
        addHoleApertures(r, holes);
        return r;
    }

    // Anillo y proporcional.
    const bool ring = s.method == PunchSettings::Method::Ring;
    const double factor = s.factor / 100.0;
    auto size = [&](double measure, double ringValue) { return ring ? measure - 2 * ringValue : measure * factor; };
    Shape punched = solid;
    std::optional<double> dia;  // como el original, no se reinicia en cada apertura
    for (const std::string& id : order) {
        const Aperture& ap = g.apertures().at(id);
        ShapeList punching;
        if (selected.count(id)) {
            bool use = false;
            if (ap.type == "C" && s.circular) {
                dia = size(ap.size.value_or(0.0), s.ringCircular);
                use = true;
            } else if (ap.type == "O" && s.oblong) {
                dia = size(ap.width > ap.height ? ap.height : ap.width, s.ringOblong);
                use = true;
            } else if (ap.type == "R") {
                if (roundTo(ap.width, s.decimals) == roundTo(ap.height, s.decimals)) {
                    if (s.square) {
                        dia = size(ap.height, s.ringSquare);
                        use = true;
                    }
                } else if (s.rectangular) {
                    dia = size(ap.width > ap.height ? ap.height : ap.width, s.ringRectangular);
                    use = true;
                }
            } else if (s.other) {
                if (ap.size) {
                    dia = size(*ap.size, s.ringOther);
                } else if (ap.type == "AM" && !ap.geometry.empty() && ap.geometry[0].solid) {
                    if (const auto b = ap.geometry[0].solid->bounds()) {
                        const double dx = b->xmax - b->xmin, dy = b->ymax - b->ymin;
                        dia = size(dx <= dy ? dx : dy, s.ringOther);
                    }
                }
                use = true;
            }
            if (use && dia)
                for (const ApertureGeometry& el : ap.geometry)
                    if (isFlash(el)) punching.push_back(el.follow->buffer(*dia / 2));
        }
        if (!dia || punching.empty()) continue;
        punched = punched.difference(unionOf(punching));
        for (const ApertureGeometry& el : ap.geometry)
            if (el.solid && isFlash(el)) collectHoles(holes, *dia, *el.solid, punching);
    }
    r.solid = partsOf(punched);
    addHoleApertures(r, holes);
    return r;
}

}  // namespace flatcam
