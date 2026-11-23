#include "core/OriginalBugs.h"
#include "io/FileExport.h"

#include <algorithm>
#include <cstdio>
#include <set>
#include <string>

#include "cam/PyNumber.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam {

namespace {

/// `"{:.{dec}f}".format(v)`.
std::string fixed(double v, int dec) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", dec, v);
    return buf;
}

/// `lz_format` / ceros "L": el número con sus decimales, sin el punto.
std::string lz(double v, int fract) {
    std::string s = fixed(v, fract);
    s.erase(s.find('.'), 1);
    return s;
}

/// `tz_format` / "LZ" del Excellon: la parte entera completada con ceros a la izquierda hasta `whole`, sin el punto.
std::string tz(double v, int whole, int fract) {
    std::string s = fixed(v, fract);
    // Corregido (sección 14): el original cuenta el signo como un dígito y completa con ceros delante del signo
    // ("0-1..."): el signo va primero y se completan solo los dígitos.
    std::string sign;
    if (!reproduceOriginalBugs() && !s.empty() && s[0] == '-') {
        sign = "-";
        s.erase(0, 1);
    }
    const auto dot = s.find('.');
    std::string w = s.substr(0, dot);
    if (static_cast<int>(w.size()) < whole) w = std::string(whole - w.size(), '0') + w;
    return sign + w + s.substr(dot + 1);
}

struct GerberWriter {
    const GerberExportFormat& f;
    double factor;
    std::string out;

    std::string xy(const Point2D& p) const {
        return f.zeros == "T" ? "X" + tz(p.x * factor, f.whole, f.fract) + "Y" + tz(p.y * factor, f.whole, f.fract)
                              : "X" + lz(p.x * factor, f.fract) + "Y" + lz(p.y * factor, f.fract);
    }
    /// Recorre los puntos: el primero con D02 y los demás con D01; con @p skipDup se saltean los repetidos.
    void path(const std::vector<Point2D>& c, bool skipDup) {
        if (c.empty()) return;
        out += xy(c[0]) + "D02*\n";
        Point2D prev = c[0];
        for (std::size_t i = 1; i < c.size(); ++i) {
            if (!skipDup || !(c[i] == prev)) out += xy(c[i]) + "D01*\n";
            prev = c[i];
        }
    }
    void point(const Shape& s) { out += xy(s.coordinates().front()) + "D03*\n"; }
};

bool isLine(const Shape& s) {
    return s.type() == ShapeType::LineString || s.type() == ShapeType::LinearRing || s.type() == ShapeType::MultiLineString;
}

/// Una región (con sus agujeros) en polaridad oscura y los agujeros en clara.
void writeRegion(GerberWriter& w, const Shape& poly) {
    w.out += "G36*\n";
    w.path(poly.exterior().coordinates(), false);
    w.out += "D02*\nG37*\n";
    const ShapeList holes = poly.interiors();
    if (holes.empty()) return;
    w.out += "%LPC*%\n";
    for (const Shape& r : holes) {
        w.out += "G36*\n";
        w.path(r.coordinates(), true);
        w.out += "D02*\nG37*\n";
    }
    w.out += "%LPD*%\n";
}

/**
 * Corregido (sección 14): la apertura "0" y las macros como regiones, en un orden en que los agujeros (polaridad clara)
 * no borran lo que se dibujó antes adentro de ellos. El original escribe las regiones en el orden de la lista: una isla
 * dibujada antes que la región que la rodea desaparecía al escribir el agujero de esa región.
 */
void writeRegionsFixed(const Gerber& g, const std::vector<std::string>& order, GerberWriter& w) {
    struct Item {
        std::optional<Shape> region;  // polígono (región)
        std::optional<Shape> other;   // línea o punto de la apertura "0"
        std::optional<Shape> clear;
        BoundingBox box;
    };
    std::vector<Item> items;
    auto addSolid = [&](const Shape& geo, const std::optional<Shape>& clear) {
        bool first = true;
        for (const Shape& part : geo.parts()) {
            if (part.isEmpty()) continue;
            Item it;
            if (part.type() == ShapeType::Polygon) it.region = part;
            else it.other = part;
            it.box = part.bounds().value_or(BoundingBox{});
            if (first) it.clear = clear;
            first = false;
            items.push_back(std::move(it));
        }
        if (first && clear) items.push_back(Item{std::nullopt, std::nullopt, clear, {}});
    };
    for (const std::string& id : order) {
        const Aperture& ap = g.apertures().at(id);
        if (id != "0" && ap.type != "AM") continue;
        for (const ApertureGeometry& el : ap.geometry) {
            if (el.solid && !el.solid->isEmpty()) addSolid(*el.solid, el.clear && !el.clear->isEmpty() ? el.clear : std::nullopt);
            else if (el.clear && !el.clear->isEmpty()) items.push_back(Item{std::nullopt, std::nullopt, el.clear, {}});
        }
    }
    // i va antes que j si un agujero de i toca a j (orden topológico estable; con ciclos, el orden de la lista).
    const std::size_t n = items.size();
    std::vector<std::vector<std::size_t>> after(n);
    std::vector<int> indeg(n, 0);
    auto overlaps = [](const BoundingBox& a, const BoundingBox& b) {
        return a.xmin <= b.xmax && b.xmin <= a.xmax && a.ymin <= b.ymax && b.ymin <= a.ymax;
    };
    for (std::size_t i = 0; i < n; ++i) {
        if (!items[i].region) continue;
        for (const Shape& hole : items[i].region->interiors()) {
            const Shape hp = Shape::polygon(hole.coordinates());
            const BoundingBox hb = hp.bounds().value_or(BoundingBox{});
            const PreparedShape prep(hp);
            for (std::size_t j = 0; j < n; ++j) {
                if (j == i) continue;
                const Shape* s = items[j].region ? &*items[j].region : items[j].other ? &*items[j].other : nullptr;
                if (!s || !overlaps(hb, items[j].box) || !prep.intersects(*s)) continue;
                // Una región que solo toca el borde del agujero no se borra (y no arma un ciclo con su vecina).
                if (items[j].region && hp.intersection(*s).area() <= 1e-12) continue;
                if (std::find(after[i].begin(), after[i].end(), j) == after[i].end()) {
                    after[i].push_back(j);
                    ++indeg[j];
                }
            }
        }
    }
    std::vector<std::size_t> sorted;
    sorted.reserve(n);
    std::set<std::size_t> ready;
    for (std::size_t i = 0; i < n; ++i)
        if (indeg[i] == 0) ready.insert(i);
    while (!ready.empty()) {
        const std::size_t i = *ready.begin();
        ready.erase(ready.begin());
        sorted.push_back(i);
        for (std::size_t j : after[i])
            if (--indeg[j] == 0) ready.insert(j);
    }
    if (sorted.size() != n) {  // ciclo: lo que queda, en el orden de la lista
        std::vector<bool> done(n, false);
        for (std::size_t i : sorted) done[i] = true;
        for (std::size_t i = 0; i < n; ++i)
            if (!done[i]) sorted.push_back(i);
    }
    for (std::size_t i : sorted) {
        const Item& it = items[i];
        if (it.region) writeRegion(w, *it.region);
        else if (it.other && it.other->type() == ShapeType::Point) w.point(*it.other);
        else if (it.other) w.path(it.other->coordinates(), true);
        if (it.clear) {
            for (const Shape& c : it.clear->parts()) {
                if (c.type() != ShapeType::Polygon || c.isEmpty()) continue;
                w.out += "%LPC*%\nG36*\n";
                w.path(c.exterior().coordinates(), true);
                w.out += "D02*\nG37*\n%LPD*%\n";
            }
        }
    }
}

}  // namespace

std::string exportGerber(const Gerber& g, const GerberExportFormat& fmt, const ExportHeader& h) {
    if (g.apertures().empty()) return {};
    // Como el original, según las unidades de la aplicación; de mm a pulgadas usa 0,03937 (corregido: 1 / 25,4).
    const double factor = fmt.appUnits == "IN" ? (fmt.units == "MM" ? 25.4 : 1)
                                                : (fmt.units == "MM" ? 1 : (reproduceOriginalBugs() ? 0.03937 : 1.0 / 25.4));
    std::string head = "G04*\n";
    head += "G04 RS-274X GERBER GENERATED BY FLATCAM v" + h.version + " - www.flatcam.org - Version Date: " +
            h.versionDate + "*\n";
    head += "G04 Filename: " + h.name + "*\n";
    head += "G04 Created on : " + h.createdOn + "*\n";
    head += "%FS" + fmt.zeros + "AX" + std::to_string(fmt.whole) + std::to_string(fmt.fract) + "Y" +
            std::to_string(fmt.whole) + std::to_string(fmt.fract) + "*%\n";
    head += "%MO" + fmt.units + "*%\n";
    const std::vector<std::string> order = g.apertureOrder();
    for (const std::string& id : order) {
        const Aperture& ap = g.apertures().at(id);
        if (ap.type == "C") head += "%ADD" + id + "C," + pyFloatRepr(factor * ap.size.value_or(0)) + "*%\n";
        else if (ap.type == "R" || ap.type == "O")
            head += "%ADD" + id + ap.type + "," + pyFloatRepr(factor * ap.width) + "X" + pyFloatRepr(factor * ap.height) +
                    "*%\n";
        else if (ap.type == "P" && !reproduceOriginalBugs())
            // Corregido: el original no declara las aperturas P (ni las AM) y el archivo no se puede volver a abrir.
            head += "%ADD" + id + "P," + pyFloatRepr(factor * ap.diam) + "X" + std::to_string(ap.nVertices) +
                    (ap.rotation ? "X" + pyFloatRepr(*ap.rotation) : std::string()) + "*%\n";
    }
    head += "\n";
    head += fmt.units == "IN" ? "G70*\n" : "G71*\n";
    head += "G90*\nG01*\n%LPD*%\n";

    GerberWriter w{fmt, factor, {}};
    if (!reproduceOriginalBugs()) {
        writeRegionsFixed(g, order, w);
    } else
    // La apertura "0": regiones (G36/G37) con sus agujeros en polaridad clara, y líneas o puntos sueltos.
    if (const auto it = g.apertures().find("0"); it != g.apertures().end()) {
        for (const ApertureGeometry& el : it->second.geometry) {
            if (el.solid) {
                const Shape& geo = *el.solid;
                if (!geo.isEmpty() && !isLine(geo) && geo.type() != ShapeType::Point) {
                    w.out += "G36*\n";
                    w.path(geo.exterior().coordinates(), false);
                    w.out += "D02*\nG37*\n";
                    const ShapeList holes = geo.interiors();
                    if (!holes.empty()) {
                        w.out += "%LPC*%\n";
                        for (const Shape& r : holes) {
                            w.out += "G36*\n";
                            w.path(r.coordinates(), true);
                            w.out += "D02*\nG37*\n";
                        }
                        w.out += "%LPD*%\n";
                    }
                } else if (!geo.isEmpty()) {
                    if (geo.type() == ShapeType::Point) w.point(geo);
                    // geo.coords de un MultiLineString falla y el original sigue con el próximo elemento.
                    else if (geo.type() != ShapeType::MultiLineString) w.path(geo.coordinates(), true);
                }
            }
            if (el.clear && !el.clear->isEmpty()) {
                w.out += "%LPC*%\nG36*\n";
                w.path(el.clear->exterior().coordinates(), true);
                w.out += "D02*\nG37*\n%LPD*%\n";
            }
        }
    }
    for (const std::string& id : order) {
        if (id == "0") continue;
        // Corregido: las macros (AM) ya se escribieron como regiones (su figura, en las unidades de salida).
        if (!reproduceOriginalBugs() && g.apertures().at(id).type == "AM") continue;
        w.out += "D" + id + "*\n";
        for (const ApertureGeometry& el : g.apertures().at(id).geometry) {
            if (el.follow && !el.follow->isEmpty() && el.follow->type() != ShapeType::MultiLineString) {
                if (el.follow->type() == ShapeType::Point) w.point(*el.follow);
                else w.path(el.follow->coordinates(), true);
            }
            if (el.clear) {
                w.out += "%LPC*%\n";
                const Shape& geo = *el.clear;
                if (!geo.isEmpty()) {
                    if (geo.type() == ShapeType::Point) {
                        w.point(geo);
                    } else if (geo.type() == ShapeType::Polygon) {
                        w.path(geo.exterior().coordinates(), true);
                        for (const Shape& r : geo.interiors()) w.path(r.coordinates(), true);
                    } else {
                        w.path(geo.coordinates(), true);
                    }
                    w.out += "%LPD*%\n";
                }
            }
        }
    }
    return head + w.out + "M02*\n";
}

std::string exportExcellon(const Excellon& e, const ExcellonExportFormat& fmt, const ExportHeader& h) {
    const double factor = fmt.appUnits == "IN" ? (fmt.units == "METRIC" ? 25.4 : 1)
                                                : (fmt.units == "METRIC" ? 1 : (reproduceOriginalBugs() ? 0.03937 : 1.0 / 25.4));
    bool hasDrills = false, hasSlots = false;
    for (const auto& [id, t] : e.drillTools()) {
        hasDrills = hasDrills || !t.drills.empty();
        hasSlots = hasSlots || !t.slots.empty();
    }
    if (!hasDrills && !hasSlots) return {};
    const bool dec = fmt.format == "dec";
    auto num = [&](double v) {
        if (dec) return fixed(v, fmt.fract);
        return fmt.zeros == "LZ" ? tz(v, fmt.whole, fmt.fract) : lz(v, fmt.fract);
    };
    auto toolName = [](int id) { return (id < 10 ? "T0" : "T") + std::to_string(id) + "\n"; };
    std::string code;
    if (hasDrills) {
        for (const auto& [id, t] : e.drillTools()) {
            code += toolName(id);
            for (const Point2D& d : t.drills) code += "X" + num(d.x * factor) + "Y" + num(d.y * factor) + "\n";
        }
    }
    if (hasSlots && !reproduceOriginalBugs()) {
        // Corregido: el original falla con slot.x sobre la tupla (inicio, fin) y, aun sin eso, escribe el inicio como
        // fin y formatea mal los ceros. Cada ranura con el mismo formato que los agujeros: "routing" (G00 + M15, G01 +
        // M16) o "drilling" (G85).
        for (const auto& [id, t] : e.drillTools()) {
            code += "G05\n" + toolName(id);
            for (const auto& [a, b] : t.slots) {
                const std::string sx = num(a.x * factor), sy = num(a.y * factor);
                const std::string ex = num(b.x * factor), ey = num(b.y * factor);
                if (fmt.slotType == "drilling")
                    code += "X" + sx + "Y" + sy + "G85X" + ex + "Y" + ey + "\nG05\n";
                else
                    code += "G00X" + sx + "Y" + sy + "\nM15\nG01X" + ex + "Y" + ey + "\nM16\n";
            }
        }
    } else if (hasSlots) {
        // slot.x sobre una tupla: falla con la primera ranura y corta el resto.
        for (const auto& [id, t] : e.drillTools()) {
            code += "G05\n" + toolName(id);
            if (!t.slots.empty()) break;
        }
    }
    std::string head = "M48\n";
    head += ";EXCELLON GENERATED BY FLATCAM v" + h.version + " - www.flatcam.org - Version Date: " + h.versionDate + "\n";
    head += ";Filename: " + h.name + "\n";
    head += ";Created on : " + h.createdOn + "\n";
    if (dec) {
        head += fmt.units + "\n";
    } else {
        head += fmt.units + "," + fmt.zeros + "\n";
        head += ";FILE_FORMAT=" + std::to_string(fmt.whole) + ":" + std::to_string(fmt.fract) + "\n";
    }
    for (const auto& [id, t] : e.drillTools())
        head += "T" + std::to_string(id) + "F00S00C" + fixed(t.tooldia * factor, fmt.units == "METRIC" ? 2 : 4) + "\n";
    head += "%\n";
    return head + code + "M30\n";
}

}  // namespace flatcam
