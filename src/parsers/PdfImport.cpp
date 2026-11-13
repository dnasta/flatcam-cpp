#include "parsers/PdfImport.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <regex>
#include <stdexcept>

#include "core/GeometryError.h"

namespace flatcam {

namespace {

using Bez = std::array<Point2D, 4>;

double roundPy(double v, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return std::strtod(buf, nullptr);
}

/// LineString(pts).buffer(d, resolution): vacío con la lista vacía, ValueError (nullopt) con un punto.
std::optional<Shape> lineBuffer(const std::vector<Point2D>& pts, double d, int steps) {
    if (pts.empty()) return Shape::collection({});
    if (pts.size() == 1) return std::nullopt;
    return Shape::lineString(pts).buffer(d, steps);
}

/// Polygon(pts).buffer(1e-7, resolution): vacío con la lista vacía; ValueError (nullopt) con 1 o 2 puntos.
std::optional<Shape> polyBuffer(std::vector<Point2D> pts, int steps) {
    if (pts.empty()) return Shape::collection({});
    if (pts.size() < 3) return std::nullopt;
    if (pts.front() != pts.back()) pts.push_back(pts.front());
    if (pts.size() < 4) pts.push_back(pts.front());
    try {
        return Shape::polygon(pts).buffer(0.0000001, steps);
    } catch (const GeometryError&) {
        return std::nullopt;
    }
}

/// Un trazado que el original no puede armar (un punto None, una lista vacía con pop): se corta todo.
struct ParseAbort : std::runtime_error {
    ParseAbort() : std::runtime_error("pdf") {}
};

Aperture* find(PdfApertures& aps, const std::string& key) {
    for (auto& [k, a] : aps)
        if (k == key) return &a;
    return nullptr;
}

/// apertures_dict[key] = {...}: reemplaza en su lugar o agrega al final.
Aperture& assign(PdfApertures& aps, const std::string& key, double size) {
    Aperture a;
    a.type = "C";
    a.size = size;
    a.hasGeometry = true;
    if (Aperture* old = find(aps, key)) {
        *old = a;
        return *old;
    }
    aps.push_back({key, a});
    return aps.back().second;
}

ShapeList iterParts(const Shape& g) {
    if (g.type() == ShapeType::MultiPolygon) return g.parts();
    return {g};
}

void addSolid(Aperture& ap, const Shape& g) {
    for (const Shape& p : iterParts(g)) {
        ApertureGeometry el;
        el.solid = p;
        el.follow = p.isEmpty() ? Shape::collection({}) : p.exterior();
        ap.geometry.push_back(el);
    }
}

void addClear(Aperture& ap, const Shape& g) {
    for (const Shape& p : iterParts(g)) {
        ApertureGeometry el;
        el.clear = p;
        ap.geometry.push_back(el);
    }
}

}  // namespace

std::vector<Point2D> pdfBezierToPoints(const Point2D& start, const Point2D& c1, const Point2D& c2, const Point2D& stop,
                                       int steps) {
    std::vector<Point2D> out;
    const double step = 1.0 / steps;
    // np.arange(0.0, 1.0, step): ceil(1 / step) valores, t = i * step
    const int n = static_cast<int>(std::ceil((1.0 - 0.0) / step));
    for (int i = 0; i < n; ++i) {
        const double t = 0.0 + i * step;
        const double p0 = std::pow(1 - t, 3), p1 = 3 * t * std::pow(1 - t, 2), p2 = 3 * (1 - t) * std::pow(t, 2),
                     p3 = std::pow(t, 3);
        out.push_back({start.x * p0 + c1.x * p1 + c2.x * p2 + stop.x * p3,
                       start.y * p0 + c1.y * p1 + c2.y * p2 + stop.y * p3});
    }
    return out;
}

std::map<int, PdfApertures> parsePdf(const std::string& content, const std::string& units, int steps) {
    static const std::regex strokeColor(R"(^\s*(\d+\.?\d*) (\d+\.?\d*) (\d+\.?\d*)\s*RG$)");
    static const std::regex fillColor(R"(^\s*(\d+\.?\d*) (\d+\.?\d*) (\d+\.?\d*)\s*rg$)");
    static const std::regex rectRe(R"(^(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s*re$)");
    static const std::regex moveRe(R"(^(-?\d+\.?\d*)\s(-?\d+\.?\d*)\sm$)");
    static const std::regex lineRe(R"(^(-?\d+\.?\d*)\s(-?\d+\.?\d*)\sl)");
    static const std::regex curveRe(
        R"(^(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s*c$)");
    static const std::regex vRe(R"(^(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s*v$)");
    static const std::regex yRe(R"(^(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s(-?\d+\.?\d*)\s*y$)");
    static const std::regex endRe(R"(^h$)");
    static const std::regex widthRe(R"(^(\d+\.?\d*)\s*w$)");
    static const std::regex strokeRe(R"(^S\s?[Q]?$)");
    static const std::regex fillRe(R"(^[f|F][*]?$)");
    static const std::regex fillStrokeRe(R"(^B[*]?$)");
    static const std::regex noOpRe(R"(^n$)");
    static const std::regex cmRe(
        R"(^(q)?\s*(-?\d+\.?\d*) (-?\d+\.?\d*) (-?\d+\.?\d*) (-?\d+\.?\d*) (-?\d+\.?\d*) (-?\d+\.?\d*)\s+cm$)");
    static const std::regex clipRe(R"(^W[*]? n?$)");
    static const std::regex saveRe(R"(^q.*?$)");
    static const std::regex restoreRe(R"(^.*Q.*$)");

    const double f = (units == "MM" || units == "mm") ? 25.4 / 72 : 1.0 / 72;
    struct {
        std::vector<std::vector<Point2D>> lines;
        std::vector<std::vector<Bez>> bezier;
        std::vector<std::vector<Point2D>> rectangle;
    } path;
    struct {
        std::vector<Point2D> lines;
        std::vector<Bez> bezier;
        std::vector<Point2D> rectangle;
    } sub;
    std::string current;  // current_subpath (vacío: None)
    bool closeSubpath = false;
    std::optional<Point2D> startPoint, currentPoint;
    double size = 0;
    std::array<double, 2> offset{0, 0}, scale{1, 1};
    struct Saved {
        std::array<double, 2> offset, scale;
    };
    std::vector<Saved> gsTransform;
    std::vector<double> gsWidth;
    std::map<int, PdfApertures> objects;
    int layer = 1;
    objects[layer] = {};
    PdfApertures aps;
    int aperture = 10;
    PdfApertures clear;
    assign(clear, "0", 0.0);
    std::optional<std::array<double, 3>> oldColor;
    bool flagClear = false;

    auto num = [](const std::ssub_match& m) { return std::strtod(m.str().c_str(), nullptr); };
    auto pt = [&](const std::smatch& m, int i) {
        const double x = num(m[i]) + offset[0], y = num(m[i + 1]) + offset[1];
        return Point2D{x * f * scale[0], y * f * scale[1]};
    };
    auto need = [](const std::optional<Point2D>& p) {
        if (!p) throw ParseAbort();
        return *p;
    };
    auto bezPoints = [&](const std::vector<Bez>& bs, std::vector<Point2D>& out) {
        for (const Bez& b : bs) {
            const auto p = pdfBezierToPoints(b[0], b[1], b[2], b[3], steps);
            out.insert(out.end(), p.begin(), p.end());
        }
    };
    // La apertura de un trazo: la del mismo tamaño redondeado o una nueva (el número sube una sola vez).
    auto strokeAperture = [&](double applied) -> Aperture& {
        const double r = roundPy(applied, 5);
        for (auto& [k, a] : aps)
            if (a.size && *a.size == r) return a;
        if (!aps.empty() && find(aps, std::to_string(aperture))) ++aperture;
        return assign(aps, std::to_string(aperture), r);
    };

    // splitlines()
    std::vector<std::string> lines;
    {
        std::string cur;
        for (std::size_t i = 0; i < content.size(); ++i) {
            const char c = content[i];
            if (c == '\n' || c == '\r' || c == '\v' || c == '\f' || c == '\x1c' || c == '\x1d' || c == '\x1e') {
                lines.push_back(cur);
                cur.clear();
                if (c == '\r' && i + 1 < content.size() && content[i + 1] == '\n') ++i;
            } else {
                cur += c;
            }
        }
        if (!cur.empty()) lines.push_back(cur);
    }

    try {
        for (const std::string& line : lines) {
            std::smatch m;
            if (std::regex_search(line, m, strokeColor)) {
                const std::array<double, 3> color{num(m[1]), num(m[2]), num(m[3])};
                if (oldColor && *oldColor == color) continue;
                if (!aps.empty()) {
                    objects[layer] = aps;
                    aps.clear();
                    ++layer;
                    objects[layer] = {};
                }
                oldColor = color;
                flagClear = false;
                continue;
            }
            if (std::regex_search(line, m, fillColor)) {
                flagClear = num(m[1]) == 1 && num(m[2]) == 1 && num(m[3]) == 1;
                continue;
            }
            if (std::regex_search(line, m, cmRe)) {
                if (m[1].matched && m[1].str() == "q") {
                    gsTransform.push_back({offset, scale});
                    gsWidth.push_back(size);
                }
                if (num(m[3]) == 0 && num(m[4]) == 0 && (num(m[6]) != 0 || num(m[7]) != 0)) {
                    offset[0] += num(m[6]);
                    offset[1] += num(m[7]);
                }
                if (num(m[2]) != 1 && num(m[5]) != 1) {
                    scale[0] *= num(m[2]);
                    scale[1] *= num(m[5]);
                }
                continue;
            }
            if (std::regex_search(line, saveRe)) {
                gsTransform.push_back({offset, scale});
                gsWidth.push_back(size);
            }
            if (std::regex_search(line, restoreRe)) {
                if (!gsTransform.empty()) {
                    offset = gsTransform.back().offset;
                    scale = gsTransform.back().scale;
                    gsTransform.pop_back();
                }
                if (!gsWidth.empty()) {
                    size = gsWidth.back();
                    gsWidth.pop_back();
                }
            }
            if (std::regex_search(line, m, moveRe)) {
                closeSubpath = false;
                sub.lines.clear();
                sub.bezier.clear();
                sub.rectangle.clear();
                startPoint = pt(m, 1);
                sub.lines.push_back(*startPoint);
                currentPoint = startPoint;
                continue;
            }
            if (std::regex_search(line, m, lineRe)) {
                current = "lines";
                const Point2D p = pt(m, 1);
                sub.lines.push_back(p);
                currentPoint = p;
                continue;
            }
            if (std::regex_search(line, m, curveRe)) {
                current = "bezier";
                const Point2D s = need(currentPoint), c1 = pt(m, 1), c2 = pt(m, 3), e = pt(m, 5);
                sub.bezier.push_back({s, c1, c2, e});
                currentPoint = e;
                continue;
            }
            if (std::regex_search(line, m, vRe)) {
                current = "bezier";
                const Point2D s = need(currentPoint), c2 = pt(m, 1), e = pt(m, 3);
                sub.bezier.push_back({s, s, c2, e});
                currentPoint = e;
                continue;
            }
            if (std::regex_search(line, m, yRe)) {
                // como el original: 'y' no marca el trazado como curva
                const Point2D s = need(currentPoint), c1 = pt(m, 1), e = pt(m, 3);
                sub.bezier.push_back({s, c1, e, e});
                currentPoint = e;
                continue;
            }
            if (std::regex_search(line, m, rectRe)) {
                current = "rectangle";
                // como el original, al ancho y al alto también se les suma el desplazamiento
                const double x = (num(m[1]) + offset[0]) * f * scale[0], y = (num(m[2]) + offset[1]) * f * scale[1];
                const double w = (num(m[3]) + offset[0]) * f * scale[0], h = (num(m[4]) + offset[1]) * f * scale[1];
                const Point2D p1{x, y}, p2{x + w, y}, p3{x + w, y + h}, p4{x, y + h};
                sub.rectangle.insert(sub.rectangle.end(), {p1, p2, p3, p4, p1});
                currentPoint = p1;
                continue;
            }
            if (std::regex_search(line, clipRe)) {
                sub.lines.clear();
                sub.bezier.clear();
                sub.rectangle.clear();
                if (closeSubpath) {
                    closeSubpath = false;
                    if (current == "lines") {
                        if (path.lines.empty()) throw ParseAbort();
                        path.lines.pop_back();
                    }
                    if (current == "rectangle") {
                        if (path.rectangle.empty()) throw ParseAbort();
                        path.rectangle.pop_back();
                    }
                }
                continue;
            }
            if (std::regex_search(line, endRe)) {
                closeSubpath = true;
                if (current == "lines") {
                    sub.lines.push_back(need(startPoint));
                    path.lines.push_back(sub.lines);
                    sub.lines.clear();
                } else if (current == "bezier") {
                    path.bezier.push_back(sub.bezier);
                    sub.bezier.clear();
                } else if (current == "rectangle") {
                    path.rectangle.push_back(sub.rectangle);
                    sub.rectangle.clear();
                }
                continue;
            }
            if (std::regex_search(line, m, widthRe)) {
                size = num(m[1]);
                continue;
            }
            if (std::regex_search(line, noOpRe)) {
                sub.lines.clear();
                sub.bezier.clear();
                sub.rectangle.clear();
                continue;
            }
            if (std::regex_search(line, strokeRe)) {
                const double applied = size * scale[0] * f;
                ShapeList pathGeo;
                auto stroke = [&](const std::vector<Point2D>& pts) {
                    if (auto g = lineBuffer(pts, applied / 2, steps)) pathGeo.push_back(*g);
                };
                if (current == "lines") {
                    if (!path.lines.empty()) {
                        for (const auto& s : path.lines) stroke(s);
                        path.lines.clear();
                    } else {
                        stroke(sub.lines);
                        sub.lines.clear();
                    }
                }
                if (current == "bezier") {
                    if (!path.bezier.empty()) {
                        for (const auto& s : path.bezier) {
                            std::vector<Point2D> g;
                            bezPoints(s, g);
                            stroke(g);
                        }
                        path.bezier.clear();
                    } else {
                        std::vector<Point2D> g;
                        bezPoints(sub.bezier, g);
                        stroke(g);
                        sub.bezier.clear();
                    }
                }
                if (current == "rectangle") {
                    if (!path.rectangle.empty()) {
                        for (const auto& s : path.rectangle) stroke(s);
                        path.rectangle.clear();
                    } else {
                        stroke(sub.rectangle);
                        sub.rectangle.clear();
                    }
                }
                Aperture& ap = strokeAperture(applied);
                for (const Shape& g : pathGeo) addSolid(ap, g);
                continue;
            }
            if (std::regex_search(line, fillRe)) {
                const double applied = size * scale[0] * f;
                ShapeList pathGeo;
                auto fill = [&](const std::vector<Point2D>& pts) {
                    if (auto g = polyBuffer(pts, steps)) pathGeo.push_back(*g);
                };
                if (current == "lines") {
                    if (!path.lines.empty()) {
                        for (std::vector<Point2D> g : path.lines) {
                            if (!closeSubpath) g.push_back(g.front());
                            fill(g);
                        }
                        path.lines.clear();
                    } else {
                        std::vector<Point2D> g = sub.lines;
                        if (!closeSubpath) g.push_back(need(startPoint));
                        fill(g);
                        sub.lines.clear();
                    }
                }
                if (current == "bezier") {
                    std::vector<Point2D> g;  // se acumula entre los trazados, como el original
                    if (!path.bezier.empty()) {
                        for (const auto& s : path.bezier)
                            for (const Bez& b : s) {
                                const auto p = pdfBezierToPoints(b[0], b[1], b[2], b[3], steps);
                                g.insert(g.end(), p.begin(), p.end());
                                if (!closeSubpath) g.push_back(g.front());
                                fill(g);
                            }
                        path.bezier.clear();
                    } else {
                        bezPoints(sub.bezier, g);
                        if (!closeSubpath) g.push_back(need(startPoint));
                        fill(g);
                        sub.bezier.clear();
                    }
                }
                if (current == "rectangle") {
                    if (!path.rectangle.empty()) {
                        for (const auto& s : path.rectangle) fill(s);
                        path.rectangle.clear();
                    } else {
                        fill(sub.rectangle);
                        sub.rectangle.clear();
                    }
                }
                closeSubpath = true;
                if (flagClear) {
                    if (current == "bezier")
                        for (const Shape& g : pathGeo) {
                            ApertureGeometry el;
                            el.clear = g;
                            find(clear, "0")->geometry.push_back(el);
                        }
                    if (!pathGeo.empty()) {
                        Aperture* zero = find(aps, "0");
                        if (!zero) zero = &assign(aps, "0", applied);
                        for (const Shape& g : pathGeo) addClear(*zero, g);
                    }
                } else {
                    if (!pathGeo.empty()) {
                        Aperture* zero = find(aps, "0");
                        if (!zero) zero = &assign(aps, "0", applied);
                        for (const Shape& g : pathGeo) addSolid(*zero, g);
                    }
                    continue;
                }
            }
            if (std::regex_search(line, fillStrokeRe)) {
                const double applied = size * scale[0] * f;
                ShapeList pathGeo, fillGeo;
                auto fill = [&](std::vector<Point2D> pts) {
                    if (auto g = polyBuffer(std::move(pts), steps)) fillGeo.push_back(*g);
                };
                // En 'B' el engrosado del trazo no está en un try: un trazado de un punto corta todo.
                auto stroke = [&](const std::vector<Point2D>& pts) {
                    auto g = lineBuffer(pts, applied / 2, steps);
                    if (!g) throw ParseAbort();
                    pathGeo.push_back(*g);
                };
                if (current == "lines") {
                    if (!path.lines.empty()) {
                        for (std::vector<Point2D> g : path.lines) {
                            if (!closeSubpath) g.push_back(g.front());
                            fill(g);
                        }
                        for (const auto& s : path.lines) stroke(s);
                        path.lines.clear();
                    } else {
                        std::vector<Point2D> g = sub.lines;
                        if (!closeSubpath) g.push_back(need(startPoint));
                        fill(g);
                        stroke(sub.lines);
                        sub.lines.clear();
                    }
                }
                if (current == "bezier") {
                    std::vector<Point2D> g;
                    if (!path.bezier.empty()) {
                        for (const auto& s : path.bezier)
                            for (const Bez& b : s) {
                                const auto p = pdfBezierToPoints(b[0], b[1], b[2], b[3], steps);
                                g.insert(g.end(), p.begin(), p.end());
                                if (!closeSubpath) g.push_back(g.front());
                                fill(g);
                            }
                        for (const auto& s : path.bezier) {
                            std::vector<Point2D> h;
                            bezPoints(s, h);
                            stroke(h);
                        }
                        path.bezier.clear();
                    } else {
                        bezPoints(sub.bezier, g);
                        if (!closeSubpath) g.push_back(need(startPoint));
                        fill(g);
                        std::vector<Point2D> h;
                        bezPoints(sub.bezier, h);
                        stroke(h);
                        sub.bezier.clear();
                    }
                }
                if (current == "rectangle") {
                    if (!path.rectangle.empty()) {
                        for (const auto& s : path.rectangle) fill(s);
                        for (const auto& s : path.rectangle) stroke(s);
                        path.rectangle.clear();
                    } else {
                        fill(sub.rectangle);
                        stroke(sub.rectangle);
                        sub.rectangle.clear();
                    }
                }
                closeSubpath = true;
                Aperture& ap = strokeAperture(applied);
                for (const Shape& g : pathGeo) addSolid(ap, g);
                // Con un engrosado de varias partes se agregan todos los rellenos (sin separarlos); con uno simple,
                // el engrosado mismo.
                auto raw = [&](Aperture& z, const Shape& g) {
                    ApertureGeometry el;
                    if (flagClear) {
                        el.clear = g;
                    } else {
                        el.solid = g;
                        el.follow = g.isEmpty() ? Shape::collection({}) : (g.type() == ShapeType::Polygon ? g.exterior() : g);
                    }
                    z.geometry.push_back(el);
                };
                Aperture* zero = find(aps, "0");
                if (zero) {
                    for (const Shape& g : pathGeo) {
                        if (g.type() == ShapeType::MultiPolygon) {
                            for (const Shape& p : fillGeo) raw(*zero, p);
                        } else {
                            raw(*zero, g);
                        }
                    }
                } else if (!pathGeo.empty()) {
                    // KeyError: la apertura 0 nueva, con los rellenos
                    zero = &assign(aps, "0", roundPy(applied, 5));
                    for (const Shape& g : fillGeo) flagClear ? addClear(*zero, g) : addSolid(*zero, g);
                }
                continue;
            }
        }
    } catch (const ParseAbort&) {
        return {};
    }
    if (!aps.empty()) objects[layer] = aps;
    if (!find(clear, "0")->geometry.empty()) objects[0] = clear;
    for (auto it = objects.begin(); it != objects.end();) it = it->second.empty() ? objects.erase(it) : std::next(it);
    return objects;
}

PdfGerberLayer pdfLayerAsGerber(const PdfApertures& aps) {
    ShapeList polys, follows;
    for (const auto& [k, ap] : aps)
        for (const ApertureGeometry& el : ap.geometry) {
            if (el.solid) polys.push_back(*el.solid);
            if (el.follow) follows.push_back(*el.follow);
        }
    Shape buff = Shape::unaryUnion(polys);
    const Aperture* zero = nullptr;
    for (const auto& [k, ap] : aps)
        if (k == "0") zero = &ap;
    if (zero) {
        ShapeList clears;
        for (const ApertureGeometry& el : zero->geometry)
            if (el.clear) clears.push_back(*el.clear);
        if (!clears.empty()) {
            ShapeList solid;
            for (const auto& [k, ap] : aps)
                for (const ApertureGeometry& el : ap.geometry) {
                    if (!el.solid) continue;
                    Shape s = *el.solid;
                    for (const Shape& c : clears) {
                        if (c.within(s)) s = s.difference(c);
                        if (s.isEmpty()) s = *el.solid;
                    }
                    if (s.type() == ShapeType::Polygon) solid.push_back(s);
                    else
                        for (const Shape& p : s.parts()) solid.push_back(p);
                }
            ShapeList nonEmpty;
            for (const Shape& s : solid)
                if (!s.isEmpty() && s.type() == ShapeType::Polygon) nonEmpty.push_back(s);
            buff = Shape::collection(nonEmpty);
        }
    }
    const Shape follow = Shape::unaryUnion(follows);
    buff = buff.buffer(0.0000001).buffer(-0.0000001);
    PdfGerberLayer out;
    if (!buff.isEmpty()) out.solid = buff.type() == ShapeType::Polygon ? ShapeList{buff} : buff.parts();
    if (!follow.isEmpty()) out.follow = follow.parts().empty() ? ShapeList{follow} : follow.parts();
    return out;
}

std::vector<std::pair<double, std::vector<Point2D>>> pdfLayerAsExcellon(const PdfApertures& aps) {
    std::vector<std::pair<double, std::vector<Point2D>>> byDia;  // en el orden en que aparecen
    for (const auto& [k, ap] : aps) {
        if (k != "0") continue;
        for (const ApertureGeometry& el : ap.geometry) {
            if (!el.clear) continue;
            const BoundingBox b = el.clear->bounds().value_or(BoundingBox{});
            const Point2D c{((b.xmax - b.xmin) / 2) + b.xmin, ((b.ymax - b.ymin) / 2) + b.ymin};
            const double dia = roundPy((b.xmax - b.xmin) * 0.974, 3);
            auto it = std::find_if(byDia.begin(), byDia.end(), [&](const auto& p) { return p.first == dia; });
            if (it == byDia.end()) byDia.push_back({dia, {c}});
            else it->second.push_back(c);
        }
    }
    std::sort(byDia.begin(), byDia.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    return byDia;
}

}  // namespace flatcam
