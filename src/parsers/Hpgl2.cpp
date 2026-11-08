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

/// Una coordenada de la posición actual: un número, ninguno, o "rota" (en el original, después de un AA queda una tupla
/// de puntos).
struct Coord {
    std::optional<double> v;
    bool broken{false};
    double get() const {
        if (broken) throw ParseError("la posición quedó con una tupla después de AA");
        return *v;
    }
};

std::string trimSpaces(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\r' || s[a] == '\n' || s[a] == '\t')) ++a;
    while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\r' || s[b - 1] == '\n' || s[b - 1] == '\t')) --b;
    return s.substr(a, b - a);
}

/// Los números de una lista de coordenadas "x1,y1,x2,y2..." (comas o espacios).
std::vector<double> numberList(const std::string& s) {
    std::vector<double> out;
    static const std::regex numRe(R"([+-]?\d+\.?\d*)");
    for (auto it = std::sregex_iterator(s.begin(), s.end(), numRe); it != std::sregex_iterator(); ++it)
        out.push_back(num(it->str()));
    return out;
}

}  // namespace

Hpgl2Result parseHpgl2(const std::string& text, int circleSteps, bool fixOriginalBugs) {
    static const std::regex commentRe(R"(^CO\s*["']([a-zA-Z0-9\s]*)["'];?$)");
    static const std::regex spRe(R"(SP(\d);?$)");
    static const std::regex penRe(R"(^(P[U|D]);?$)");
    static const std::regex penCoordsRe(R"(^(P[UD])\s*([-+\d.,\s]+);?$)");  // PU/PD con coordenadas (corregido)
    static const std::regex paListRe(R"(^PA\s*([-+\d.,\s]+);?$)");          // PA con varios pares (corregido)
    static const std::regex initRe(R"(^(IN);?$)");
    static const std::regex absRe(R"(^PA\s*(-?\d+\.?\d*),?\s*(-?\d+\.?\d*)*;?$)");
    static const std::regex circRe(R"(^CI\s*(\+?\d+\.?\d+?)?\s*;?\s*$)");
    static const std::regex arcRe(R"(^AA\s*([+-]?\d+),?\s*([+-]?\d+),?\s*([+-]?\d+);?$)");
    static const std::regex arc3Re(R"(^AT\s*([+-]?\d+),?\s*([+-]?\d+),?\s*([+-]?\d+),?\s*([+-]?\d+);?$)");
    Hpgl2Result r;
    std::vector<Point2D> path;
    ShapeList buffer;
    Coord cx, cy, lx, ly;
    Point2D arcFirst;  // después de un AA (en el original), current_x es el primer punto del arco
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
    auto addShape = [&](const Shape& g) {
        if (!tool) throw ParseError("KeyError: None");
        r.tools[*tool].push_back(g);
        buffer.push_back(g);
    };
    // La rama de PA del original para un movimiento a (x, y) (o solo x, si y falta).
    auto moveTo = [&](double x, std::optional<double> y) {
        lx = Coord{x, false};
        cx = lx;
        if (y) {
            ly = Coord{*y, false};
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
    };
    auto moveList = [&](const std::string& coords) {
        const std::vector<double> v = numberList(coords);
        for (std::size_t k = 0; k + 1 < v.size(); k += 2) moveTo(v[k], v[k + 1]);
    };

    // Las órdenes: una por línea en el original; con la corrección, cada línea se parte en los ';'.
    std::vector<std::string> commands;
    {
        std::istringstream in(text);
        std::string line;
        while (std::getline(in, line)) {
            if (!fixOriginalBugs) {
                commands.push_back(trimSpaces(line));
                continue;
            }
            std::size_t start = 0;
            while (start <= line.size()) {
                const std::size_t semi = line.find(';', start);
                const std::string cmd = trimSpaces(line.substr(start, semi == std::string::npos ? std::string::npos
                                                                                            : semi - start));
                if (!cmd.empty()) commands.push_back(cmd);
                if (semi == std::string::npos) break;
                start = semi + 1;
            }
        }
    }
    try {
        for (const std::string& g : commands) {
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
            if (fixOriginalBugs && std::regex_search(g, m, penCoordsRe)) {
                penDown = m[1] == "PD";
                moveList(m[2]);
                continue;
            }
            if (std::regex_search(g, m, absRe)) {
                moveTo(num(m[1]), m[2].matched && !m[2].str().empty() ? std::optional<double>(num(m[2])) : std::nullopt);
                continue;
            }
            if (fixOriginalBugs && std::regex_search(g, m, paListRe)) {
                moveList(m[1]);
                continue;
            }
            if (std::regex_search(g, m, circRe)) {
                if (path.size() > 1) flush();
                startPath();
                if (cx.v && cy.v) {
                    const double radius = std::stod(m[1]);
                    // Point((current_x, current_y)) con dos tuplas (después de un AA): Shapely toma la primera.
                    const Point2D c = cx.broken ? arcFirst : Point2D{cx.get(), cy.get()};
                    addShape(Shape::point(c).buffer(radius, circleSteps).exterior());
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
                                         angle >= 0.0 ? ArcDirection::CounterClockwise : ArcDirection::Clockwise,
                                         circleSteps);
                    addShape(Shape::lineString(pts));
                    if (fixOriginalBugs) {
                        // Corregido: la posición queda en el final del arco (y el trazo siguiente empieza ahí).
                        cx = lx = Coord{pts.back().x, false};
                        cy = ly = Coord{pts.back().y, false};
                        path = {pts.back()};
                    } else {
                        // current_x = line_coords[0], current_y = line_coords[1]: dos puntos, no números.
                        arcFirst = pts.front();
                        cx = Coord{0.0, true};
                        cy = Coord{0.0, true};
                    }
                    continue;
                }
            }
            if (std::regex_search(g, m, arc3Re)) {
                if (path.size() > 1) flush();
                startPath();
                if (cx.v && cy.v) {
                    // El original le pasa listas de Python a three_point_circle (p1 + p2 las concatena y / 2.0 lanza
                    // TypeError) y parse_lines lo atrapa con `return`: cualquier AT corta la lectura sin geometría.
                    if (!fixOriginalBugs) return Hpgl2Result{};
                    // Corregido: AT x_intermedio,y_intermedio,x_final,y_final, de la posición actual al final pasando
                    // por el intermedio (el original además terminaba el arco en el punto intermedio).
                    const Point2D p1{cx.get(), cy.get()}, mid{num(m[1]), num(m[2])}, end{num(m[3]), num(m[4])};
                    const double cross = (mid.x - p1.x) * (end.y - p1.y) - (mid.y - p1.y) * (end.x - p1.x);
                    const auto c = threePointCircle(p1, mid, end);
                    if (!c || cross == 0) {
                        addShape(Shape::lineString({p1, end}));  // tres puntos alineados: una recta
                    } else {
                        const double start = std::atan2(p1.y - c->center.y, p1.x - c->center.x);
                        const double stop = std::atan2(end.y - c->center.y, end.x - c->center.x);
                        addShape(Shape::lineString(arc(c->center, c->radius, start, stop,
                                                       cross > 0 ? ArcDirection::CounterClockwise : ArcDirection::Clockwise,
                                                       circleSteps)));
                    }
                    cx = lx = Coord{end.x, false};
                    cy = ly = Coord{end.y, false};
                    path = {end};
                    continue;
                }
            }
            // "Line ignored"
        }
        // Corregido: el último trazo con la pluma abajo no se pierde al terminar el archivo.
        if (fixOriginalBugs && path.size() > 1 && tool) flush();
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
