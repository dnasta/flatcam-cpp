#include "parsers/Hpgl2.h"

#include <cmath>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>

#include "core/ArcMath.h"

namespace flatcam {

namespace {

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

double num(const std::string& s) { return std::stod(s) / 40.0; }  // parse_number: en milímetros

/// Una coordenada de la posición actual: un número, ninguno, o "rota" (después de un AA queda una tupla de puntos).
struct Coord {
    std::optional<double> v;
    bool broken{false};
    double get() const {
        if (broken) throw ParseError("la posición quedó con una tupla después de AA");
        return *v;
    }
};

}  // namespace

Hpgl2Result parseHpgl2(const std::string& text, int circleSteps) {
    static const std::regex commentRe(R"(^CO\s*["']([a-zA-Z0-9\s]*)["'];?$)");
    static const std::regex spRe(R"(SP(\d);?$)");
    static const std::regex penRe(R"(^(P[U|D]);?$)");
    static const std::regex initRe(R"(^(IN);?$)");
    static const std::regex absRe(R"(^PA\s*(-?\d+\.?\d*),?\s*(-?\d+\.?\d*)*;?$)");
    static const std::regex circRe(R"(^CI\s*(\+?\d+\.?\d+?)?\s*;?\s*$)");
    static const std::regex arcRe(R"(^AA\s*([+-]?\d+),?\s*([+-]?\d+),?\s*([+-]?\d+);?$)");
    static const std::regex arc3Re(R"(^AT\s*([+-]?\d+),?\s*([+-]?\d+),?\s*([+-]?\d+),?\s*([+-]?\d+);?$)");
    Hpgl2Result r;
    std::vector<Point2D> path;
    ShapeList buffer;
    Coord cx, cy, lx, ly;
    Point2D arcFirst;  // después de un AA, current_x es el primer punto del arco
    bool penDown = false, init = false;
    std::optional<int> tool;
    auto flush = [&]() {
        if (!tool) throw ParseError("KeyError: None");  // self.tools[current_tool] sin SP
        const Shape g = Shape::lineString(path);
        r.tools[*tool].push_back(g);
        buffer.push_back(g);
        path.clear();
    };
    auto startPath = [&]() {
        if (lx.v && ly.v) path = {{lx.get(), ly.get()}};
    };
    std::istringstream in(text);
    std::string line;
    try {
        while (std::getline(in, line)) {
            std::size_t a = 0, b = line.size();
            while (a < b && (line[a] == ' ' || line[a] == '\r' || line[a] == '\n')) ++a;
            while (b > a && (line[b - 1] == ' ' || line[b - 1] == '\r' || line[b - 1] == '\n')) --b;
            const std::string g = line.substr(a, b - a);
            std::smatch m;
            if (std::regex_search(g, m, commentRe)) continue;
            if (std::regex_search(g, m, initRe)) {
                init = true;
                continue;
            }
            if (!init) continue;
            if (std::regex_search(g, m, spRe)) {
                const int t = std::stoi(m[1]);
                r.tools[t].clear();  // la pluma se vuelve a crear vacía
                if (tool && !path.empty()) flush();
                tool = t;
                continue;
            }
            if (std::regex_search(g, m, penRe)) {
                penDown = m[1] == "PD";
                continue;
            }
            if (std::regex_search(g, m, absRe)) {
                lx = Coord{num(m[1]), false};
                cx = lx;
                if (m[2].matched && !m[2].str().empty()) {
                    ly = Coord{num(m[2]), false};
                    cy = ly;
                } else {
                    ly = cy;
                }
                if (penDown) {
                    if (cx.v && (cy.v || cy.broken)) {
                        if (path.empty()) throw ParseError("IndexError: list index out of range");  // path[-1]
                        const Point2D p{cx.get(), cy.get()};
                        if (!(path.back() == p)) path.push_back(p);
                    }
                } else {
                    if (path.size() > 1) flush();
                    if (lx.v && (ly.v || ly.broken)) path = {{lx.get(), ly.get()}};
                }
                continue;
            }
            if (std::regex_search(g, m, circRe)) {
                if (path.size() > 1) flush();
                startPath();
                if (cx.v && cy.v) {
                    const double radius = std::stod(m[1]);
                    // Point((current_x, current_y)) con dos tuplas (después de un AA): Shapely toma la primera.
                    const Point2D c = cx.broken ? arcFirst : Point2D{cx.get(), cy.get()};
                    const Shape ring = Shape::point(c).buffer(radius, circleSteps).exterior();
                    if (!tool) throw ParseError("KeyError: None");
                    r.tools[*tool].push_back(ring);
                    buffer.push_back(ring);
                    continue;
                }
            }
            if (std::regex_search(g, m, arcRe)) {
                if (path.size() > 1) flush();
                startPath();
                if (cx.v && cy.v) {
                    const Point2D center{num(m[1]), num(m[2])};
                    const double angle = std::stod(m[3]) * M_PI / 180.0;  // np.deg2rad
                    const Point2D p1{cx.get(), cy.get()};
                    const double radius = std::sqrt(std::pow(center.x - p1.x, 2) + std::pow(center.y - p1.y, 2));
                    const double start = std::atan2(p1.y - center.y, p1.x - center.x);
                    const auto pts = arc(center, radius, start, start + angle,
                                         angle >= 0.0 ? ArcDirection::CounterClockwise : ArcDirection::Clockwise, circleSteps);
                    if (!tool) throw ParseError("KeyError: None");
                    const Shape geo = Shape::lineString(pts);
                    r.tools[*tool].push_back(geo);
                    buffer.push_back(geo);
                    // current_x = line_coords[0], current_y = line_coords[1]: dos puntos, no números.
                    arcFirst = pts.front();
                    cx = Coord{0.0, true};
                    cy = Coord{0.0, true};
                    continue;
                }
            }
            if (std::regex_search(g, m, arc3Re)) {
                if (path.size() > 1) flush();
                startPath();
                // three_point_circle recibe listas de Python (p1 + p2 las concatena y / 2.0 lanza TypeError), y
                // parse_lines atrapa el TypeError con `return`: cualquier AT corta la lectura sin geometría.
                if (cx.v && cy.v) return Hpgl2Result{};
            }
            // "Line ignored"
        }
        if (buffer.empty()) return r;  // "Object is not HPGL2 file or empty"
        const Shape u = Shape::unaryUnion(buffer);
        r.solidGeometry = u.parts().empty() ? ShapeList{u} : u.parts();
        r.ok = true;
    } catch (const std::exception& e) {
        r.error = e.what();  // "HPGL2 Parser ERROR": solid_geometry queda en None
        r.ok = false;
        r.solidGeometry.clear();
    }
    return r;
}

}  // namespace flatcam
