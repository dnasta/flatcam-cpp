#include "parsers/SvgImport.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <regex>
#include <stdexcept>

namespace flatcam {

namespace {

constexpr double kPi = 3.14159265358979323846;

// Python: radians(x) = x * (pi / 180); degrees(x) = x * (180 / pi); a % b con el signo del divisor.
double radians(double d) { return d * (kPi / 180.0); }
double degrees(double r) { return r * (180.0 / kPi); }
double pyMod(double a, double b) {
    double m = std::fmod(a, b);
    if (m != 0 && ((m < 0) != (b < 0))) m += b;
    return m;
}

/// Error de la importación (equivale a una excepción del original que hace fallar `import_svg`).
struct SvgError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

// ------------------------------------------------------------------------------------------------ XML

struct XmlReader {
    const std::string& s;
    std::size_t i{0};

    [[noreturn]] void fail(const std::string& what) const {
        throw SvgError("XML mal formado (" + what + ") en la posición " + std::to_string(i));
    }
    bool starts(const char* p) const { return s.compare(i, std::char_traits<char>::length(p), p) == 0; }
    void skipSpace() {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    }
    static std::string decode(const std::string& v) {
        std::string o;
        for (std::size_t k = 0; k < v.size(); ++k) {
            if (v[k] != '&') {
                o += v[k];
                continue;
            }
            const std::size_t semi = v.find(';', k);
            if (semi == std::string::npos) {
                o += v[k];
                continue;
            }
            const std::string e = v.substr(k + 1, semi - k - 1);
            if (e == "amp") o += '&';
            else if (e == "lt") o += '<';
            else if (e == "gt") o += '>';
            else if (e == "quot") o += '"';
            else if (e == "apos") o += '\'';
            else if (!e.empty() && e[0] == '#') {
                const long code = e.size() > 1 && (e[1] == 'x' || e[1] == 'X') ? std::strtol(e.c_str() + 2, nullptr, 16)
                                                                              : std::strtol(e.c_str() + 1, nullptr, 10);
                if (code < 0x80) o += static_cast<char>(code);
                else if (code < 0x800) {
                    o += static_cast<char>(0xC0 | (code >> 6));
                    o += static_cast<char>(0x80 | (code & 0x3F));
                } else {
                    o += static_cast<char>(0xE0 | (code >> 12));
                    o += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                    o += static_cast<char>(0x80 | (code & 0x3F));
                }
            } else {
                o += "&" + e + ";";
            }
            k = semi;
        }
        return o;
    }
    /// Saltea declaraciones, comentarios, CDATA e instrucciones.
    bool skipMisc() {
        if (starts("<?")) {
            const auto e = s.find("?>", i);
            if (e == std::string::npos) fail("instrucción sin cerrar");
            i = e + 2;
            return true;
        }
        if (starts("<!--")) {
            const auto e = s.find("-->", i);
            if (e == std::string::npos) fail("comentario sin cerrar");
            i = e + 3;
            return true;
        }
        if (starts("<![CDATA[")) {
            const auto e = s.find("]]>", i);
            if (e == std::string::npos) fail("CDATA sin cerrar");
            i = e + 3;
            return true;
        }
        if (starts("<!")) {  // DOCTYPE, con un posible subconjunto interno entre corchetes
            int depth = 0;
            for (; i < s.size(); ++i) {
                if (s[i] == '[') ++depth;
                else if (s[i] == ']') --depth;
                else if (s[i] == '>' && depth <= 0) break;
            }
            ++i;
            return true;
        }
        return false;
    }
    XmlElement element() {
        if (s[i] != '<') fail("se esperaba un elemento");
        ++i;
        std::size_t j = i;
        while (j < s.size() && !std::isspace(static_cast<unsigned char>(s[j])) && s[j] != '>' && s[j] != '/') ++j;
        XmlElement e;
        std::string name = s.substr(i, j - i);
        const auto colon = name.find(':');
        e.tag = colon == std::string::npos ? name : name.substr(colon + 1);
        i = j;
        while (true) {
            skipSpace();
            if (i >= s.size()) fail("elemento sin cerrar");
            if (starts("/>")) {
                i += 2;
                return e;
            }
            if (s[i] == '>') {
                ++i;
                break;
            }
            std::size_t eq = s.find('=', i);
            if (eq == std::string::npos) fail("atributo");
            std::string an = s.substr(i, eq - i);
            while (!an.empty() && std::isspace(static_cast<unsigned char>(an.back()))) an.pop_back();
            i = eq + 1;
            skipSpace();
            const char q = s[i];
            if (q != '"' && q != '\'') fail("comillas");
            const std::size_t end = s.find(q, i + 1);
            if (end == std::string::npos) fail("valor sin cerrar");
            e.attrs.push_back({an, decode(s.substr(i + 1, end - i - 1))});
            i = end + 1;
        }
        // Contenido.
        bool textDone = false;
        while (i < s.size()) {
            if (starts("</")) {
                const auto gt = s.find('>', i);
                if (gt == std::string::npos) fail("cierre");
                i = gt + 1;
                return e;
            }
            if (s[i] == '<') {
                if (starts("<![CDATA[")) {
                    const auto end = s.find("]]>", i);
                    if (end == std::string::npos) fail("CDATA sin cerrar");
                    if (!textDone) e.text += s.substr(i + 9, end - i - 9);
                    i = end + 3;
                    continue;
                }
                if (skipMisc()) continue;
                e.children.push_back(element());
                textDone = true;
                continue;
            }
            const auto lt = s.find('<', i);
            if (lt == std::string::npos) fail("contenido sin cerrar");
            if (!textDone) e.text += decode(s.substr(i, lt - i));
            i = lt;
        }
        fail("elemento sin cerrar");
    }
};

// ------------------------------------------------------------------------------------------------ números

/// `float()` de Python sobre un texto (sin espacios alrededor). @throws SvgError si no es un número.
double pyFloat(const std::string& t) {
    std::size_t a = 0, b = t.size();
    while (a < b && std::isspace(static_cast<unsigned char>(t[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(t[b - 1]))) --b;
    const std::string u = t.substr(a, b - a);
    if (u.empty()) throw SvgError("could not convert string to float: ''");
    char* end = nullptr;
    const double v = std::strtod(u.c_str(), &end);
    if (end != u.c_str() + u.size()) throw SvgError("could not convert string to float: '" + u + "'");
    return v;
}

const std::string kInteger = R"([+-]?[0-9]+)";
const std::string kNumber = R"((?:[+-]?[0-9]*\.[0-9]+(?:[Ee])" + kInteger + R"()?)|(?:)" + kInteger + R"((?:[Ee])" + kInteger +
                            R"()?))";

/// `svgparselength`: el primer número del texto (las unidades no se usan). Sin texto da 0.
double svgLength(const std::optional<std::string>& s) {
    if (!s || s->empty()) return 0;
    static const std::regex re("(" + kNumber + ")(em|ex|px|in|cm|mm|pt|pc|%)?");
    std::smatch m;
    if (!std::regex_search(*s, m, re)) throw SvgError("'NoneType' object is not subscriptable");
    return pyFloat(m[1].str());
}

// ------------------------------------------------------------------------------------------------ svg.path

/// Recorte de un bytearray de svg.path: espacios, tabuladores, retornos y comas al principio.
void stripArray(std::string& a) {
    std::size_t k = 0;
    while (k < a.size() && (a[k] == ' ' || a[k] == '\t' || a[k] == '\r' || a[k] == '\n' || a[k] == ',')) ++k;
    a.erase(0, k);
}

struct InvalidPath {};

double popNumber(std::string& a) {
    static const std::regex re(R"(^[-+]?\d*\.?\d*(?:[eE][-+]?\d+)?)");
    std::smatch m;
    if (!std::regex_search(a, m, re) || m.str(0).empty()) throw InvalidPath{};
    const double v = pyFloat(m.str(0));  // "-" o "." no son números: ValueError en el original
    a.erase(0, m.length(0));
    stripArray(a);
    return v;
}

struct Token {
    char command;
    std::vector<double> nums;  // los números en orden (pares de coordenadas como dos números)
    std::vector<int> flags;    // banderas de los arcos (-1 = None)
};

std::vector<Token> tokenize(const std::string& d, std::string& error, bool& ok) {
    ok = true;
    std::vector<Token> out;
    // _commandify_path
    static const std::string commands = "MmZzLlHhVvCcSsQqTtAa";
    std::vector<std::pair<char, std::string>> cmds;
    bool have = false;
    std::string chunk;
    auto strip = [](std::string x) {
        std::size_t a = 0, b = x.size();
        while (a < b && std::isspace(static_cast<unsigned char>(x[a]))) ++a;
        while (b > a && std::isspace(static_cast<unsigned char>(x[b - 1]))) --b;
        return x.substr(a, b - a);
    };
    auto flushChunk = [&](const std::string& c) {
        const std::string x = strip(c);
        if (x.empty()) return true;
        if (!have) {
            error = "Path does not start with a command: " + d;
            return false;
        }
        cmds.back().second += x;
        return true;
    };
    for (char ch : d) {
        if (commands.find(ch) != std::string::npos) {
            if (!flushChunk(chunk)) {
                ok = false;
                return {};
            }
            chunk.clear();
            cmds.push_back({ch, std::string()});
            have = true;
        } else {
            chunk += ch;
        }
    }
    if (!flushChunk(chunk)) {
        ok = false;
        return {};
    }
    // _tokenize_path
    for (auto& [cmd, args] : cmds) {
        char command = cmd;
        if (command == 'z' || command == 'Z') {
            out.push_back({command, {}, {}});
            continue;
        }
        const char up = static_cast<char>(std::toupper(command));
        std::string seq;
        switch (up) {
            case 'M': case 'L': case 'T': seq = "c"; break;
            case 'H': case 'V': seq = "s"; break;
            case 'C': seq = "ccc"; break;
            case 'S': case 'Q': seq = "cc"; break;
            case 'A': seq = "uusffc"; break;
        }
        std::string a = args;
        bool implicit = false;
        while (!a.empty()) {
            Token t{command, {}, {}};
            for (std::size_t k = 0; k < seq.size(); ++k) {
                try {
                    const char kind = seq[k];
                    if (kind == 'c') {
                        t.nums.push_back(popNumber(a));
                        t.nums.push_back(popNumber(a));
                    } else if (kind == 's') {
                        t.nums.push_back(popNumber(a));
                    } else if (kind == 'u') {
                        const double v = popNumber(a);
                        if (v < 0) throw InvalidPath{};
                        t.nums.push_back(v);
                    } else {  // bandera: el primer carácter
                        if (a.empty()) throw InvalidPath{};
                        const char f = a[0];
                        a.erase(0, 1);
                        stripArray(a);
                        t.flags.push_back(f == '0' ? 0 : f == '1' ? 1 : -1);
                    }
                } catch (const InvalidPath&) {
                    if (k == 0 && implicit) return out;  // un carácter no válido: se toma como comentario
                    error = std::string("Invalid path element ") + command + " " + args;
                    ok = false;
                    return {};
                }
            }
            out.push_back(t);
            implicit = true;
            if (command == 'm') command = 'l';
            else if (command == 'M') command = 'L';
        }
    }
    return out;
}

void parameterizeArc(SvgSegment& s) {
    if (s.start == s.end) return;
    if (s.radius.x == 0 || s.radius.y == 0) return;
    const double cosr = std::cos(radians(s.rotation)), sinr = std::sin(radians(s.rotation));
    const double dx = (s.start.x - s.end.x) / 2, dy = (s.start.y - s.end.y) / 2;
    const double x1prim = cosr * dx + sinr * dy, x1prim_sq = x1prim * x1prim;
    const double y1prim = -sinr * dx + cosr * dy, y1prim_sq = y1prim * y1prim;
    double rx = s.radius.x, rx_sq = rx * rx, ry = s.radius.y, ry_sq = ry * ry;
    double radiusScale = (x1prim_sq / rx_sq) + (y1prim_sq / ry_sq);
    if (radiusScale > 1) {
        radiusScale = std::sqrt(radiusScale);
        rx *= radiusScale;
        ry *= radiusScale;
        rx_sq = rx * rx;
        ry_sq = ry * ry;
        s.radiusScale = radiusScale;
    } else {
        s.radiusScale = 1;
    }
    const double t1 = rx_sq * y1prim_sq, t2 = ry_sq * x1prim_sq;
    double c = std::sqrt(std::abs((rx_sq * ry_sq - t1 - t2) / (t1 + t2)));
    if (s.large == s.sweep) c = -c;
    const double cxprim = c * rx * y1prim / ry, cyprim = -c * ry * x1prim / rx;
    s.center = {(cosr * cxprim - sinr * cyprim) + ((s.start.x + s.end.x) / 2),
                (sinr * cxprim + cosr * cyprim) + ((s.start.y + s.end.y) / 2)};
    const double ux = (x1prim - cxprim) / rx, uy = (y1prim - cyprim) / ry;
    const double vx = (-x1prim - cxprim) / rx, vy = (-y1prim - cyprim) / ry;
    double n = std::sqrt(ux * ux + uy * uy);
    double theta = degrees(std::acos(ux / n));
    if (uy < 0) theta = -theta;
    s.theta = pyMod(theta, 360);
    n = std::sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
    double d = (ux * vx + uy * vy) / n;
    if (d > 1.0) d = 1.0;
    else if (d < -1.0) d = -1.0;
    double delta = degrees(std::acos(d));
    if ((ux * vy - uy * vx) < 0) delta = -delta;
    s.delta = pyMod(delta, 360);
    if (!s.sweep) s.delta -= 360;
    s.parameterized = true;
}

double absC(const Point2D& a, const Point2D& b) { return std::hypot(a.x - b.x, a.y - b.y); }

double segmentLength(const SvgSegment& c, double start, double end, const Point2D& sp, const Point2D& ep, double error,
                     int minDepth, int depth) {
    const double mid = (start + end) / 2;
    const Point2D mp = c.point(mid);
    const double length = absC(ep, sp);
    const double length2 = absC(mp, sp) + absC(ep, mp);
    if ((length2 - length > error) || (depth < minDepth)) {
        ++depth;
        return segmentLength(c, start, mid, sp, mp, error, minDepth, depth) +
               segmentLength(c, mid, end, mp, ep, error, minDepth, depth);
    }
    return length2;
}

}  // namespace

std::optional<std::string> XmlElement::get(const std::string& name) const {
    for (const auto& [k, v] : attrs)
        if (k == name) return v;
    return std::nullopt;
}

std::optional<XmlElement> parseXml(const std::string& text, std::string& error) {
    try {
        XmlReader r{text};
        while (true) {
            r.skipSpace();
            if (r.i >= text.size()) throw SvgError("documento vacío");
            if (!r.skipMisc()) break;
        }
        return r.element();
    } catch (const SvgError& e) {
        error = e.what();
        return std::nullopt;
    }
}

Point2D SvgSegment::point(double pos) const {
    switch (kind) {
        case Kind::Move: return start;
        case Kind::Line:
        case Kind::Close: return {start.x + (end.x - start.x) * pos, start.y + (end.y - start.y) * pos};
        case Kind::Cubic: {
            const double a = std::pow(1 - pos, 3), b = 3 * std::pow(1 - pos, 2) * pos, cc = 3 * (1 - pos) * std::pow(pos, 2),
                         dd = std::pow(pos, 3);
            return {a * start.x + b * c1.x + cc * c2.x + dd * end.x, a * start.y + b * c1.y + cc * c2.y + dd * end.y};
        }
        case Kind::Quadratic: {
            const double a = std::pow(1 - pos, 2), b = 2 * (1 - pos) * pos, cc = std::pow(pos, 2);
            return {a * start.x + b * c1.x + cc * end.x, a * start.y + b * c1.y + cc * end.y};
        }
        case Kind::Arc: {
            if (start == end) return start;
            if (radius.x == 0 || radius.y == 0) return {start.x + (end.x - start.x) * pos, start.y + (end.y - start.y) * pos};
            const double angle = radians(theta + (delta * pos));
            const double cosr = std::cos(radians(rotation)), sinr = std::sin(radians(rotation));
            const double rx = radius.x * radiusScale, ry = radius.y * radiusScale;
            return {cosr * std::cos(angle) * rx - sinr * std::sin(angle) * ry + center.x,
                    sinr * std::cos(angle) * rx + cosr * std::sin(angle) * ry + center.y};
        }
    }
    return start;
}

double SvgSegment::length(double error) const {
    constexpr int kMinDepth = 5;
    switch (kind) {
        case Kind::Move: return 0;
        case Kind::Line:
        case Kind::Close: return absC(end, start);
        case Kind::Cubic: return segmentLength(*this, 0, 1, point(0), point(1), error, kMinDepth, 0);
        case Kind::Quadratic: {
            const Point2D a{start.x - 2 * c1.x + end.x, start.y - 2 * c1.y + end.y};
            const Point2D b{2 * (c1.x - start.x), 2 * (c1.y - start.y)};
            const double A = 4 * (a.x * a.x + a.y * a.y), B = 4 * (a.x * b.x + a.y * b.y), C = b.x * b.x + b.y * b.y;
            const double absA = std::hypot(a.x, a.y), absB = std::hypot(b.x, b.y);
            auto fallback = [&] {
                if (absA < 1e-10) return absB;
                const double k = absB / absA;
                return k >= 2 ? absB - absA : absA * (k * k / 2 - k + 1);
            };
            if (A == 0) return fallback();  // ZeroDivisionError
            const double Sabc = 2 * std::sqrt(A + B + C), A2 = std::sqrt(A), A32 = 2 * A * A2, C2 = 2 * std::sqrt(C);
            const double BA = B / A2;
            const double arg = (2 * A2 + BA + Sabc) / (BA + C2);
            if (BA + C2 == 0 || !(arg > 0)) return fallback();  // ZeroDivisionError / ValueError del log
            return (A32 * Sabc + A2 * B * (Sabc - C2) + (4 * C * A - B * B) * std::log(arg)) / (4 * A32);
        }
        case Kind::Arc: {
            if (start == end) return 0;
            if (radius.x == 0 || radius.y == 0) return std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
            if (radius.x == radius.y) return std::abs(radius.x * radiusScale * delta * kPi / 180);
            return segmentLength(*this, 0, 1, point(0), point(1), error, kMinDepth, 0);
        }
    }
    return 0;
}

std::optional<std::vector<SvgSegment>> parseSvgPath(const std::string& d, std::string& error) {
    bool ok = true;
    const std::vector<Token> tokens = tokenize(d, error, ok);
    if (!ok) return std::nullopt;
    std::vector<SvgSegment> segs;
    std::optional<Point2D> startPos;
    char last = 0;
    Point2D cur{0, 0};
    auto P = [](const Token& t, std::size_t k) { return Point2D{t.nums[2 * k], t.nums[2 * k + 1]}; };
    for (const Token& t : tokens) {
        const bool rel = std::islower(static_cast<unsigned char>(t.command)) != 0;
        const char c = static_cast<char>(std::toupper(t.command));
        auto absP = [&](Point2D p) { return rel ? Point2D{p.x + cur.x, p.y + cur.y} : p; };
        SvgSegment s;
        s.start = cur;
        if (c == 'M') {
            cur = absP(P(t, 0));
            s.kind = SvgSegment::Kind::Move;
            s.start = s.end = cur;
            startPos = cur;
        } else if (c == 'Z') {
            if (!startPos) {
                error = "Close before Move";
                return std::nullopt;
            }
            s.kind = SvgSegment::Kind::Close;
            s.end = *startPos;
            cur = *startPos;
        } else if (c == 'L') {
            s.kind = SvgSegment::Kind::Line;
            s.end = absP(P(t, 0));
            cur = s.end;
        } else if (c == 'H') {
            s.kind = SvgSegment::Kind::Line;
            s.end = {rel ? t.nums[0] + cur.x : t.nums[0], cur.y};
            cur = s.end;
        } else if (c == 'V') {
            s.kind = SvgSegment::Kind::Line;
            s.end = {cur.x, rel ? t.nums[0] + cur.y : t.nums[0]};
            cur = s.end;
        } else if (c == 'C') {
            s.kind = SvgSegment::Kind::Cubic;
            s.c1 = absP(P(t, 0));
            s.c2 = absP(P(t, 1));
            s.end = absP(P(t, 2));
            cur = s.end;
        } else if (c == 'S') {
            s.kind = SvgSegment::Kind::Cubic;
            s.c2 = absP(P(t, 0));
            s.end = absP(P(t, 1));
            if (last == 'C' || last == 'S') s.c1 = {cur.x + cur.x - segs.back().c2.x, cur.y + cur.y - segs.back().c2.y};
            else s.c1 = cur;
            cur = s.end;
        } else if (c == 'Q') {
            s.kind = SvgSegment::Kind::Quadratic;
            s.c1 = absP(P(t, 0));
            s.end = absP(P(t, 1));
            cur = s.end;
        } else if (c == 'T') {
            s.kind = SvgSegment::Kind::Quadratic;
            s.end = absP(P(t, 0));
            if (last == 'Q' || last == 'T') s.c1 = {cur.x + cur.x - segs.back().c1.x, cur.y + cur.y - segs.back().c1.y};
            else s.c1 = cur;
            cur = s.end;
        } else if (c == 'A') {
            s.kind = SvgSegment::Kind::Arc;
            s.radius = {t.nums[0], t.nums[1]};
            s.rotation = t.nums[2];
            s.large = t.flags[0] == 1;
            s.sweep = t.flags[1] == 1;
            s.end = absP({t.nums[3], t.nums[4]});
            parameterizeArc(s);
            cur = s.end;
        }
        segs.push_back(s);
        last = c;
    }
    return segs;
}

namespace {

// ------------------------------------------------------------------------------------------------ ParseSVG

using Geos = std::optional<ShapeList>;  // None si el elemento no aporta nada

/// `Polygon(coords)` de Shapely: hacen falta al menos 3 puntos (el anillo se cierra solo).
std::optional<Shape> polygonOf(const std::vector<Point2D>& pts, const std::vector<std::vector<Point2D>>& holes = {}) {
    if (pts.size() < 3) return std::nullopt;
    for (const auto& h : holes)
        if (h.size() < 3) return std::nullopt;
    try {
        return Shape::polygon(pts, holes);
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::optional<ShapeList> path2shapely(const std::vector<SvgSegment>& path, double factor) {
    std::vector<Point2D> points;
    std::vector<std::vector<Point2D>> rings;
    bool closed = false;
    for (const SvgSegment& c : path) {
        if (c.kind == SvgSegment::Kind::Line) {
            // El inicio no se multiplica por el factor (así está en el original).
            const Point2D s = c.start;
            if (points.empty() || !(points.back() == s)) points.push_back(s);
            points.push_back({factor * c.end.x, factor * c.end.y});
            continue;
        }
        if (c.kind == SvgSegment::Kind::Arc || c.kind == SvgSegment::Kind::Cubic || c.kind == SvgSegment::Kind::Quadratic) {
            const double length = c.length(1.0 / 10.0);  // res = 1.0
            int steps = static_cast<int>(length) * 2;     // units = 'MM'
            if (steps == 0 || steps < 10) steps = 10;
            const double frac = 1.0 / steps;
            for (int i = 0; i < steps; ++i) {
                const Point2D p = c.point(i * frac);
                if (points.empty() || !(points.back() == p)) points.push_back({factor * p.x, factor * p.y});
            }
            const Point2D e = c.point(1.0);
            points.push_back({factor * e.x, factor * e.y});
            continue;
        }
        if (c.kind == SvgSegment::Kind::Move) {
            if (points.empty()) continue;
            rings.push_back(points);
            if (!closed) {
                points.clear();
            } else {
                closed = false;
                points = {{factor * c.start.x, factor * c.start.y}};
            }
            continue;
        }
        closed = false;
        if (c.kind == SvgSegment::Kind::Close) {
            if (points.empty()) continue;
            rings.push_back(points);
            points.clear();
            closed = true;
            continue;
        }
    }
    if (!points.empty()) rings.push_back(points);
    // MultiLineString(rings): cada anillo necesita al menos dos puntos.
    for (const auto& r : rings)
        if (r.size() < 2) return std::nullopt;
    ShapeList geometry;
    if (rings.empty()) return geometry;
    std::vector<std::vector<Point2D>> holes(rings.begin() + 1, rings.end());
    if (auto p = polygonOf(rings[0], holes)) {
        geometry.push_back(*p);
    } else {
        std::vector<Point2D> coords;
        for (const auto& l : rings) {
            coords.push_back(l[0]);
            coords.push_back(l[1]);
        }
        if (auto q = polygonOf(coords)) geometry.push_back(*q);
        else geometry.push_back(Shape::lineString(coords));
    }
    return geometry;
}

double attrScaled(const XmlElement& e, const char* name, double factor) {
    const double v = svgLength(e.get(name));
    return v != 0 ? v * factor : v;  // "cx * factor if cx else cx"
}

Shape cleanPolygon(const std::vector<Point2D>& pts, int resolution = 16) {
    const auto p = polygonOf(pts);
    if (!p) throw SvgError("A LinearRing must have at least 3 coordinate tuples");
    return p->buffer(0, resolution);
}

/// parse_svg_point_list, con su rareza: busca los separadores en el texto sin espacios a los costados pero corta el
/// texto original con esas posiciones.
std::vector<Point2D> pointList(const std::string& str, double factor) {
    std::size_t a = 0, b = str.size();
    while (a < b && str[a] == ' ') ++a;
    while (b > a && str[b - 1] == ' ') --b;
    const std::string stripped = str.substr(a, b - a);
    static const std::regex sep(R"((\s*,\s*)|(\s+))");
    std::vector<Point2D> pairs;
    double last = 0;
    std::size_t pos = 0;
    int i = 0;
    for (auto it = std::sregex_iterator(stripped.begin(), stripped.end(), sep); it != std::sregex_iterator(); ++it) {
        const auto mstart = static_cast<std::size_t>(it->position(0));
        const double val = pyFloat(str.substr(pos, mstart > pos ? mstart - pos : 0));
        if (i % 2 == 1) pairs.push_back({factor * last, factor * val});
        else last = val * factor;
        pos = mstart + static_cast<std::size_t>(it->length(0));
        ++i;
    }
    const double val = pyFloat(str.substr(std::min(pos, str.size())));
    if (i % 2 == 1) pairs.push_back({factor * last, factor * val});
    return pairs;
}

struct Transform {
    std::string name;
    std::vector<double> v;
};

/// parse_svg_transform. Desvío: con un texto que no entiende el original queda en un bucle infinito; acá se ignora el
/// resto.
std::vector<Transform> parseTransform(std::string tr) {
    auto strip = [](std::string& x) {
        std::size_t a = 0, b = x.size();
        while (a < b && x[a] == ' ') ++a;
        while (b > a && x[b - 1] == ' ') --b;
        x = x.substr(a, b - a);
    };
    strip(tr);
    const std::string n = "(" + kNumber + ")";
    const std::string sep = R"((?:(?:\s+)|(?:\s*,\s*)))";
    static const std::regex translate("^translate\\s*\\(\\s*" + n + "(?:" + sep + n + ")?\\s*\\)");
    static const std::regex scale("^scale\\s*\\(\\s*" + n + "(?:" + sep + n + ")?\\s*\\)");
    static const std::regex skew("^skew([XY])\\s*\\(\\s*" + n + "\\s*\\)");
    static const std::regex rotate("^rotate\\s*\\(\\s*" + n + "(?:" + sep + n + sep + n + ")?\\s*\\)");
    static const std::regex matrix("^matrix\\s*\\(\\s*" + n + sep + n + sep + n + sep + n + sep + n + sep + n + "\\s*\\)");
    std::vector<Transform> out;
    while (!tr.empty()) {
        std::smatch m;
        if (std::regex_search(tr, m, translate)) {
            out.push_back({"translate", {pyFloat(m[1]), m[2].matched ? pyFloat(m[2]) : 0.0}});
        } else if (std::regex_search(tr, m, scale)) {
            out.push_back({"scale", {pyFloat(m[1]), m[2].matched ? pyFloat(m[2]) : pyFloat(m[1])}});
        } else if (std::regex_search(tr, m, skew)) {
            const double a = pyFloat(m[2]);
            out.push_back({"skew", {m[1] == "X" ? a : 0.0, m[1] == "Y" ? a : 0.0}});
        } else if (std::regex_search(tr, m, rotate)) {
            out.push_back({"rotate", {pyFloat(m[1]), m[2].matched && !m[2].str().empty() ? pyFloat(m[2]) : 0.0,
                                      m[3].matched && !m[3].str().empty() ? pyFloat(m[3]) : 0.0}});
        } else if (std::regex_search(tr, m, matrix)) {
            Transform t{"matrix", {}};
            for (int k = 1; k <= 6; ++k) t.v.push_back(pyFloat(m[k]));
            out.push_back(t);
        } else {
            break;
        }
        tr = tr.substr(static_cast<std::size_t>(m.length(0)));
        strip(tr);
    }
    return out;
}

const XmlElement* findById(const XmlElement& node, const std::string& id) {
    for (const XmlElement& c : node.children) {
        if (c.get("id") == id) return &c;
        if (const XmlElement* f = findById(c, id)) return f;
    }
    return nullptr;
}

bool hasTspan(const XmlElement& n) {
    if (n.tag == "tspan") return true;
    for (const XmlElement& c : n.children)
        if (hasTspan(c)) return true;
    return false;
}

Geos getsvggeo(const XmlElement& node, const XmlElement& root, int res, double factor) {
    Geos geo = ShapeList{};
    const std::string& kind = node.tag;
    if (!node.children.empty()) {
        for (const XmlElement& child : node.children)
            if (Geos sub = getsvggeo(child, root, res, factor)) geo->insert(geo->end(), sub->begin(), sub->end());
    } else if (kind == "path") {
        std::string err;
        const auto p = parseSvgPath(node.get("d").value_or(""), err);
        if (!p) throw SvgError(err);
        geo = path2shapely(*p, factor);
    } else if (kind == "rect") {
        const double w = svgLength(node.get("width")), h = svgLength(node.get("height"));
        const double x = node.get("x") ? svgLength(node.get("x")) * factor : 0;
        const double y = node.get("y") ? svgLength(node.get("y")) * factor : 0;
        // Esquinas redondeadas: el original multiplica el texto de rx por el factor y falla (TypeError).
        if ((node.get("rx") && !node.get("rx")->empty()) || (node.get("ry") && !node.get("ry")->empty()))
            throw SvgError("can't multiply sequence by non-int of type 'float'");
        geo = ShapeList{cleanPolygon({{x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}, {x, y}})};
    } else if (kind == "circle") {
        const double cx = attrScaled(node, "cx", factor), cy = attrScaled(node, "cy", factor);
        const double r = attrScaled(node, "r", factor);
        geo = ShapeList{Shape::point({cx, cy}).buffer(r, res)};
    } else if (kind == "ellipse") {
        const double cx = attrScaled(node, "cx", factor), cy = attrScaled(node, "cy", factor);
        const double rx = attrScaled(node, "rx", factor), ry = attrScaled(node, "ry", factor);
        std::vector<Point2D> pts;
        for (int i = 0; i < res; ++i) {
            const double t = static_cast<double>(i) / res;
            pts.push_back({cx + rx * std::cos(2 * kPi * t), cy + ry * std::sin(2 * kPi * t)});
        }
        geo = ShapeList{cleanPolygon(pts)};
    } else if (kind == "polygon") {
        geo = ShapeList{cleanPolygon(pointList(node.get("points").value_or(""), factor), res)};
    } else if (kind == "line") {
        geo = ShapeList{Shape::lineString({{svgLength(node.get("x1")) * factor, svgLength(node.get("y1")) * factor},
                                           {svgLength(node.get("x2")) * factor, svgLength(node.get("y2")) * factor}})};
    } else if (kind == "polyline") {
        const auto pts = pointList(node.get("points").value_or(""), factor);
        if (pts.size() < 2) throw SvgError("LineStrings must have at least 2 coordinate tuples");
        geo = ShapeList{Shape::lineString(pts)};
    } else if (kind == "use") {
        auto href = node.get("href");
        if (!href) href = node.get("xlink:href");
        if (!href) throw SvgError("KeyError: href");
        std::string id = *href;
        id.erase(std::remove(id.begin(), id.end(), '#'), id.end());
        if (const XmlElement* ref = findById(root, id)) geo = getsvggeo(*ref, root, res, factor);
    } else {
        geo = std::nullopt;  // "Unknown kind"
    }
    if (geo) {
        if (const auto tr = node.get("transform")) {
            const auto list = parseTransform(*tr);
            for (auto it = list.rbegin(); it != list.rend(); ++it) {
                for (Shape& g : *geo) {
                    const auto& v = it->v;
                    if (it->name == "translate") g = g.translate(v[0], v[1]);
                    else if (it->name == "scale") g = g.scale(v[0], v[1], Point2D{0, 0});
                    else if (it->name == "rotate") g = g.rotate(v[0], Point2D{v[1], v[2]});
                    else if (it->name == "skew") g = g.skew(v[0], v[1], Point2D{0, 0});
                    // affine_transform(geo, [a, b, c, d, e, f]): Shapely lo toma como [a, b, d, e, xoff, yoff].
                    else if (it->name == "matrix") g = g.transform(AffineTransform(v[0], v[1], v[2], v[3], v[4], v[5]));
                }
            }
        }
    }
    return geo;
}

/// Lo común a las dos importaciones: leer, el factor del viewBox, las figuras y el reflejo en Y corrido el alto.
SvgImportResult readSvg(const std::string& text, int res, ShapeList& geos) {
    SvgImportResult r;
    const auto root = parseXml(text, r.error);
    if (!root) return r;
    try {
        const double h = svgLength(root->get("height"));
        double factor = 1.0;
        if (const auto vb = root->get("viewBox")) {
            std::vector<double> v;
            std::string tok;
            for (char c : *vb + " ") {
                if (std::isspace(static_cast<unsigned char>(c))) {
                    if (!tok.empty()) v.push_back(pyFloat(tok));
                    tok.clear();
                } else {
                    tok += c;
                }
            }
            if (v.size() < 3) throw SvgError("viewBox");
            factor = svgLength(root->get("width")) / v[2];
        }
        const Geos g = getsvggeo(*root, *root, res, factor);
        if (!g) throw SvgError("'NoneType' object is not iterable");
        for (const Shape& s : *g) geos.push_back(s.scale(1.0, -1.0, Point2D{0, 0}).translate(0, h));
        r.hadText = hasTspan(*root);
        r.ok = true;
    } catch (const std::exception& e) {
        r.error = e.what();
    }
    return r;
}

}  // namespace

SvgImportResult importSvgAsGeometry(const std::string& svgText, int circleSteps) {
    ShapeList geos;
    SvgImportResult r = readSvg(svgText, circleSteps, geos);
    if (!r.ok) return r;
    // Primero los polígonos; las demás figuras se unen con linemerge.
    ShapeList lines;
    for (const Shape& g : geos) {
        if (g.type() == ShapeType::Polygon) {
            r.geometry.push_back(g);
        } else if (g.type() == ShapeType::MultiPolygon) {
            r.ok = false;
            r.error = "linemerge: una figura no es una línea";
            return r;
        } else {
            lines.push_back(g);
        }
    }
    if (!lines.empty()) {
        const Shape merged = Shape::lineMerge(lines);
        if (merged.parts().empty()) {
            if (!merged.isEmpty()) r.geometry.push_back(merged);
        } else {
            for (const Shape& p : merged.parts()) r.geometry.push_back(p);
        }
    }
    return r;
}

SvgImportResult importSvgAsGerber(const std::string& svgText, int circleSteps) {
    ShapeList geos;
    SvgImportResult r = readSvg(svgText, circleSteps, geos);
    if (!r.ok) return r;
    auto fail = [&r] {
        r.ok = false;
        r.error = "'object has no attribute 'exterior'";
        return r;
    };
    if (geos.size() == 1) {
        // Una sola figura: su contorno y cada agujero como un polígono aparte.
        if (geos[0].type() != ShapeType::Polygon) return fail();
        ShapeList q{Shape::polygon(geos[0].exterior().coordinates())};
        for (const Shape& hole : geos[0].interiors()) q.push_back(Shape::polygon(hole.coordinates()).buffer(0, circleSteps));
        geos = q;
    }
    for (const Shape& g : geos)
        if (g.type() != ShapeType::Polygon) return fail();  // pol.exterior
    r.geometry = geos;
    // El Gerber empieza con solid_geometry = Polygon() (vacío), que queda al final de la lista.
    r.geometry.push_back(Shape::fromWkt("POLYGON EMPTY"));
    return r;
}

}  // namespace flatcam
