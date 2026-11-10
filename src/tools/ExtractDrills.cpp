#include "tools/ExtractDrills.h"

#include <cmath>
#include <cstdio>
#include <set>

namespace flatcam {

namespace {

double decFormat(double v, int dec) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", dec, v);
    return std::stod(buf);
}

/// Los flashes de una apertura (los `follow` que son puntos).
std::vector<Point2D> flashes(const Aperture& ap) {
    std::vector<Point2D> out;
    for (const ApertureGeometry& el : ap.geometry)
        if (el.follow && el.follow->type() == ShapeType::Point) out.push_back(el.follow->coordinates().front());
    return out;
}

}  // namespace

std::optional<std::vector<ExtractedTool>> extractDrills(const Gerber& g, const ExtractDrillsSettings& s) {
    const double tol = std::pow(10.0, -s.decimals);
    auto squareish = [&](const Aperture& ap) {
        return std::abs(decFormat(ap.width, s.decimals) - decFormat(ap.height, s.decimals)) < tol;
    };
    std::vector<ExtractedTool> tools;
    if (s.mode == ExtractDrillsSettings::Mode::Fixed) {
        ExtractedTool t;
        t.dia = s.fixedDia;
        for (const std::string& id : g.apertureOrder()) {
            const Aperture& ap = g.apertures().at(id);
            if (ap.type == "C") {
                if (!s.circular) continue;
            } else if (ap.type == "O") {
                if (!s.oblong) continue;
            } else if (ap.type == "R") {
                // round(width, decimals) == round(height, decimals)
                const double p = std::pow(10.0, s.decimals);
                if (std::round(ap.width * p) / p == std::round(ap.height * p) / p) {
                    if (!s.square) continue;
                } else if (!s.rectangular) {
                    continue;
                }
            } else if (!s.other) {
                continue;
            }
            for (const Point2D& p : flashes(ap)) t.drills.push_back(p);
        }
        if (t.drills.size() < 2) return std::nullopt;  // el primero no entra en solid_geometry
        tools.push_back(t);
        return tools;
    }
    const bool ring = s.mode == ExtractDrillsSettings::Mode::Ring;
    const double factor = s.factor / 100.0;
    // dia = medida * factor (proporcional) o medida - 2 * anillo.
    auto size = [&](double measure, double ringValue) { return ring ? measure - 2 * ringValue : measure * factor; };
    std::set<bool> found;
    for (const std::string& id : g.apertureOrder()) {
        const Aperture& ap = g.apertures().at(id);
        std::optional<double> dia;
        if (ap.type == "C") {
            if (s.circular) dia = size(ap.size.value_or(0.0), s.ringCircular);
        } else if (ap.type == "O") {
            if (s.oblong) dia = size(ap.width > ap.height ? ap.height : ap.width, s.ringOblong);
        } else if (ap.type == "R") {
            if (squareish(ap)) {
                if (s.square) dia = size(ap.height, s.ringSquare);
            } else if (s.rectangular) {
                dia = size(ap.width > ap.height ? ap.height : ap.width, s.ringRectangular);
            }
        } else if (s.other) {
            if (ap.size) {
                dia = size(*ap.size, s.ringOther);
            } else if (ap.type == "AM" && !ap.geometry.empty() && ap.geometry[0].solid) {
                const auto b = ap.geometry[0].solid->bounds();
                if (b) {
                    const double dx = b->xmax - b->xmin, dy = b->ymax - b->ymin;
                    dia = size(dx <= dy ? dx : dy, s.ringOther);
                }
            }
        }
        if (!dia) continue;
        // La herramienta con el mismo diámetro (la última que coincide) o una nueva.
        std::optional<std::size_t> match;
        for (std::size_t i = 0; i < tools.size(); ++i)
            if (std::abs(decFormat(tools[i].dia, s.decimals) - decFormat(*dia, s.decimals)) < tol) match = i;
        const std::vector<Point2D> pts = flashes(ap);
        if (!match && !pts.empty()) {
            ExtractedTool t;
            int mx = 0;
            for (const ExtractedTool& e : tools) mx = std::max(mx, e.id);
            t.id = tools.empty() ? 1 : mx + 1;
            t.dia = *dia;
            tools.push_back(t);
            match = tools.size() - 1;
        }
        if (!match) continue;
        const std::size_t before = tools[*match].drills.size();
        for (const Point2D& p : pts) tools[*match].drills.push_back(p);
        // `solid_geometry` de la herramienta: todos sus agujeros menos el primero.
        if (before > 0 || !pts.empty()) found.insert(tools[*match].drills.size() >= 2);
    }
    if (!found.count(true)) return std::nullopt;
    return tools;
}

}  // namespace flatcam
