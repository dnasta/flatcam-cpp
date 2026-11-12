#include "tools/RulesCheck.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <optional>

namespace flatcam {

namespace {

double roundPy(double v, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return std::strtod(buf, nullptr);
}

ShapeList solids(const RulesGerber& g) {
    ShapeList out;
    for (const std::string& code : g.order) {
        const auto it = g.apertures.find(code);
        if (it == g.apertures.end()) continue;
        for (const ApertureGeometry& e : it->second.geometry)
            if (e.solid) out.push_back(*e.solid);
    }
    return out;
}

/// MultiPolygon(lista) de Shapely 1.8: de una figura múltiple solo queda la primera parte.
Shape multiPolygon(const ShapeList& l) {
    ShapeList polys;
    for (const Shape& s : l) {
        if (s.isEmpty()) continue;
        if (s.type() == ShapeType::Polygon) polys.push_back(s);
        else if (s.type() == ShapeType::MultiPolygon) polys.push_back(s.parts().front());
    }
    return Shape::collection(polys);
}

/// Polygon -> [Polygon]; MultiPolygon -> sus partes.
ShapeList iterParts(const Shape& s) {
    if (s.isEmpty()) return {};
    return s.type() == ShapeType::Polygon ? ShapeList{s} : s.parts();
}

void addUnique(std::vector<RulesPoint>& pts, const Point2D& p) {
    for (const auto& q : pts)
        if (q.p == p && !q.shapely) return;
    pts.push_back({p, false});
}

Point2D midpoint(const Shape& a, const Shape& b) {
    const auto n = a.nearestPoints(b);
    const Point2D l1 = n->first, l2 = n->second;
    const double dx = l1.x - l2.x, dy = l1.y - l2.y;
    return {std::min(l1.x, l2.x) + std::abs(dx) / 2, std::min(l1.y, l2.y) + std::abs(dy) / 2};
}

/// Las violaciones agrupadas por distancia en el orden de la primera aparición (min_dict) y sin repetir.
void pairs(const ShapeList& a, const ShapeList* b, double size, bool filterLater, std::vector<RulesPoint>& out) {
    std::vector<std::pair<double, std::vector<Point2D>>> byDist;
    auto record = [&](double d, const Point2D& loc) {
        for (auto& [k, v] : byDist)
            if (k == d) {
                v.push_back(loc);
                return;
            }
        byDist.push_back({d, {loc}});
    };
    for (std::size_t i = 0; i < a.size(); ++i) {
        const ShapeList& other = b ? *b : a;
        for (std::size_t j = b ? 0 : i + 1; j < other.size(); ++j) {
            const double d = a[i].distance(other[j]);
            if (filterLater || d < size) record(d, midpoint(a[i], other[j]));
        }
    }
    for (const auto& [d, locs] : byDist)
        if (!filterLater || d < size)
            for (const Point2D& p : locs) addUnique(out, p);
}

}  // namespace

std::vector<RulesResult> rulesTracesSize(const std::vector<RulesGerber>& list, double size) {
    std::vector<RulesResult> out;
    for (const RulesGerber& g : list) {
        RulesResult r;
        r.title = "Trace Size";
        r.names = {g.name};
        r.namesIsList = false;
        for (const std::string& code : g.order) {
            const Aperture& ap = g.apertures.at(code);
            if (!ap.size) continue;  // sin 'size': la excepción se ignora
            const double d = *ap.size;
            if (d < size && d != 0.0) {
                r.sizes.push_back(d);
                for (const ApertureGeometry& e : ap.geometry)
                    if (e.solid) r.points.push_back({e.solid->representativePoint(), false});
            }
        }
        out.push_back(r);
    }
    return out;
}

RulesResult rulesInsideClearance(const RulesGerber& g, double size, const std::string& title) {
    RulesResult r;
    r.title = title;
    r.names = {g.name};
    r.namesIsList = false;
    ShapeList solid, clear;
    for (const std::string& code : g.order)
        for (const ApertureGeometry& e : g.apertures.at(code).geometry) {
            if (e.solid) solid.push_back(*e.solid);
            if (e.clear) clear.push_back(*e.clear);
        }
    ShapeList total;
    if (!clear.empty()) {
        // Solo los sólidos que contienen un borrado (uno por cada par).
        for (const Shape& c : clear)
            for (const Shape& s : solid)
                if (c.within(s)) total.push_back(s.difference(c));
    } else {
        const Shape t = multiPolygon(solid).buffer(0.000001);
        if (t.type() == ShapeType::Polygon) {
            r.failed = "Failed. Only one polygon.";
            return r;
        }
        total = iterParts(t);
    }
    pairs(total, nullptr, size, false, r.points);
    return r;
}

RulesResult rulesGerberClearance(const std::vector<RulesGerber>& list, double size, const std::string& title) {
    RulesResult r;
    r.title = title;
    if (list.size() != 2 && list.size() != 3) return r;
    ShapeList a = solids(list[0]);
    if (list.size() == 3) {
        const ShapeList b = solids(list[1]);
        a.insert(a.end(), b.begin(), b.end());
    }
    const ShapeList g1 = iterParts(multiPolygon(a).buffer(0));
    const ShapeList g3 = iterParts(multiPolygon(solids(list.back())).buffer(0));
    pairs(g1, &g3, size, false, r.points);
    for (const RulesGerber& g : list) r.names.push_back(g.name);
    return r;
}

std::vector<RulesResult> rulesHolesSize(const std::vector<RulesExcellon>& list, double size) {
    std::vector<RulesResult> out;
    for (const RulesExcellon& e : list) {
        RulesResult r;
        r.title = "Hole Size";
        r.names = {e.name};
        r.namesIsList = false;
        r.hasDias = true;
        for (const auto& [dia, geo] : e.tools) {
            const double d = roundPy(dia, 4);
            if (d < size) r.dias.push_back(d);
        }
        out.push_back(r);
    }
    return out;
}

RulesResult rulesHolesClearance(const std::vector<RulesExcellon>& list, double size) {
    RulesResult r;
    r.title = "Hole to Hole Clearance";
    ShapeList total;
    for (const RulesExcellon& e : list)
        for (const auto& [dia, geo] : e.tools) total.insert(total.end(), geo.begin(), geo.end());
    pairs(total, nullptr, size, true, r.points);
    for (const RulesExcellon& e : list) r.names.push_back(e.name);
    return r;
}

RulesResult rulesAnnularRing(const RulesGerber& g, const RulesExcellon& e, double size, const std::string& title) {
    RulesResult r;
    r.title = title;
    const ShapeList grb = iterParts(multiPolygon(solids(g)).buffer(0));
    ShapeList exc;
    for (const auto& [dia, geo] : e.tools) exc.insert(exc.end(), geo.begin(), geo.end());
    std::vector<std::pair<double, std::vector<RulesPoint>>> byDist;
    auto record = [&](double d, const RulesPoint& p) {
        for (auto& [k, v] : byDist)
            if (k == d) {
                v.push_back(p);
                return;
            }
        byDist.push_back({d, {p}});
    };
    for (const Shape& gg : grb)
        for (const Shape& s : exc) {
            const double d = std::abs(gg.exterior().distance(s));
            if (d > 0) {
                if (d < size) record(d, {midpoint(gg.exterior(), s), false});
            } else {
                record(d, {s.representativePoint(), true});
            }
        }
    for (const auto& [d, pts] : byDist) r.points.insert(r.points.end(), pts.begin(), pts.end());
    r.names = {g.name, e.name};
    return r;
}

std::optional<std::string> rulesReport(const std::vector<RulesResult>& results, int decimals,
                                       const std::vector<std::string>& st) {
    // st: STATUS, FAILED, PASSED, "Violations: There are no violations for the current rule."
    std::string txt;
    char buf[256];
    auto upper = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
        return s;
    };
    auto status = [&](bool failed) {
        std::snprintf(buf, sizeof buf, "%s: <span style=\"color:white;background-color:%s\">&nbsp;%s </span>.<BR>",
                      st[0].c_str(), failed ? "red" : "green", failed ? st[1].c_str() : st[2].c_str());
        return std::string(buf);
    };
    for (const RulesResult& r : results) {
        txt += "<b>RULE NAME:</b>&nbsp;&nbsp;&nbsp;&nbsp;" + upper(r.title) + "<BR>";
        for (const std::string& n : r.names) txt += "File name: " + n + "<BR>";
        std::string pt;
        if (!r.hasDias) {
            if (!r.points.empty() || !r.failed.empty()) {
                txt += status(true);
                if (!r.failed.empty()) {
                    pt = r.failed;
                } else {
                    // 'Failed' in points[0]: con un Point de Shapely (TypeError) el informe no se crea.
                    if (r.points.front().shapely) return std::nullopt;
                    for (const RulesPoint& p : r.points) {
                        if (p.shapely) return std::nullopt;
                        std::snprintf(buf, sizeof buf, "(%.*f, %.*f), ", decimals, p.p.x, decimals, p.p.y);
                        pt += buf;
                    }
                }
                txt += "Violations: " + pt + "<BR>";
            } else {
                txt += status(false) + st[3] + "<BR>";
            }
        } else {
            if (!r.dias.empty()) {
                txt += status(true);
                for (double d : r.dias) {
                    std::snprintf(buf, sizeof buf, "%.*f, ", decimals, d);
                    pt += buf;
                }
                txt += "Violations: " + pt + "<BR>";
            } else {
                txt += status(false) + st[3] + "<BR>";
            }
        }
        txt += "<BR><BR>";
    }
    return txt;
}

}  // namespace flatcam
