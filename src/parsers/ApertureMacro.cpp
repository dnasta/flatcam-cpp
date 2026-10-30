#include "parsers/ApertureMacro.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>

#include "core/GeometryError.h"
#include "core/Logger.h"

namespace flatcam {

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr int kMacroResolution = 16;  ///< Segmentos por cuarto de círculo (valor por defecto de Shapely).

/// Evaluador recursivo descendente de expresiones de macros.
class ExprParser {
public:
    ExprParser(const std::string& text, const std::map<std::string, double>& vars)
        : s_(text), vars_(vars) {}

    double parse() {
        const double v = expr();
        skipSpaces();
        if (pos_ != s_.size()) fail();
        return v;
    }

private:
    [[noreturn]] void fail() const {
        throw GeometryError("Expresión inválida en la macro de apertura: '" + s_ + "'");
    }
    void skipSpaces() {
        while (pos_ < s_.size() && std::isspace(static_cast<unsigned char>(s_[pos_]))) ++pos_;
    }
    bool peekIs(char c) {
        skipSpaces();
        return pos_ < s_.size() && s_[pos_] == c;
    }

    // expr := term (('+' | '-') term)*
    double expr() {
        double v = term();
        for (;;) {
            if (peekIs('+')) { ++pos_; v += term(); }
            else if (peekIs('-')) { ++pos_; v -= term(); }
            else return v;
        }
    }
    // term := unary (('*' | 'x' | 'X' | '/') unary)*
    double term() {
        double v = unary();
        for (;;) {
            skipSpaces();
            if (pos_ >= s_.size()) return v;
            const char c = s_[pos_];
            if (c == '*' || c == 'x' || c == 'X') { ++pos_; v *= unary(); }
            else if (c == '/') { ++pos_; v /= unary(); }
            else return v;
        }
    }
    double unary() {
        skipSpaces();
        if (peekIs('+')) { ++pos_; return unary(); }
        if (peekIs('-')) { ++pos_; return -unary(); }
        return primary();
    }
    double primary() {
        skipSpaces();
        if (pos_ >= s_.size()) fail();
        if (s_[pos_] == '(') {
            ++pos_;
            const double v = expr();
            if (!peekIs(')')) fail();
            ++pos_;
            return v;
        }
        if (s_[pos_] == '$') {
            ++pos_;
            const std::size_t start = pos_;
            // Según la especificación, los nombres de variable son `$` seguido de dígitos (`$1`, `$12`).
            while (pos_ < s_.size() && std::isdigit(static_cast<unsigned char>(s_[pos_]))) ++pos_;
            if (start == pos_) fail();
            const auto it = vars_.find(s_.substr(start, pos_ - start));
            return it == vars_.end() ? 0.0 : it->second;
        }
        // Número: dígitos, punto decimal y exponente opcional.
        const std::size_t start = pos_;
        while (pos_ < s_.size() && (std::isdigit(static_cast<unsigned char>(s_[pos_])) || s_[pos_] == '.')) ++pos_;
        if (pos_ == start) fail();
        if (pos_ < s_.size() && (s_[pos_] == 'e' || s_[pos_] == 'E')) {
            std::size_t p = pos_ + 1;
            if (p < s_.size() && (s_[p] == '+' || s_[p] == '-')) ++p;
            if (p < s_.size() && std::isdigit(static_cast<unsigned char>(s_[p]))) {
                while (p < s_.size() && std::isdigit(static_cast<unsigned char>(s_[p]))) ++p;
                pos_ = p;
            }
        }
        return std::strtod(s_.substr(start, pos_ - start).c_str(), nullptr);
    }

    const std::string& s_;
    const std::map<std::string, double>& vars_;
    std::size_t pos_{0};
};

std::string stripChars(const std::string& s, const std::string& chars) {
    const auto b = s.find_first_not_of(chars);
    if (b == std::string::npos) return "";
    const auto e = s.find_last_not_of(chars);
    return s.substr(b, e - b + 1);
}

/// Completa con ceros hasta @p n elementos (`default2zero`).
std::vector<double> padZero(std::size_t n, const std::vector<double>& mods) {
    std::vector<double> x(std::max(n, mods.size()), 0.0);
    std::copy(mods.begin(), mods.end(), x.begin());
    return x;
}

Shape emptyPolygon() { return Shape::fromWkt("POLYGON EMPTY"); }

}  // namespace

double ApertureMacro::evaluate(const std::string& text, const std::map<std::string, double>& variables) {
    return ExprParser(text, variables).parse();
}

std::vector<std::vector<double>> ApertureMacro::parseContent(std::map<std::string, double>& locvars) const {
    std::string raw;
    for (char c : raw_) {
        if (c != '\n' && c != '\r') raw += c;
    }
    raw = stripChars(raw, " *");

    std::vector<std::vector<double>> primitives;
    std::size_t begin = 0;
    while (begin <= raw.size()) {
        std::size_t end = raw.find('*', begin);
        if (end == std::string::npos) end = raw.size();
        const std::string part = raw.substr(begin, end - begin);
        begin = end + 1;

        if (!part.empty() && part[0] == '0') continue;  // comentario

        if (!part.empty() && part[0] == '$') {  // definición de variable: $K=expresión
            std::size_t p = 1;
            while (p < part.size() && (std::isalnum(static_cast<unsigned char>(part[p])) ||
                                       std::string("[]\\^_`").find(part[p]) != std::string::npos)) ++p;
            if (p > 1 && p < part.size() && part[p] == '=') {
                locvars[part.substr(1, p - 1)] = evaluate(part.substr(p + 1), locvars);
                continue;
            }
        }

        if (!part.empty() && part[0] >= '1' && part[0] <= '9') {  // primitiva
            std::vector<double> elements;
            std::size_t b = 0;
            while (b <= part.size()) {
                std::size_t e = part.find(',', b);
                if (e == std::string::npos) e = part.size();
                elements.push_back(evaluate(part.substr(b, e - b), locvars));
                b = e + 1;
            }
            primitives.push_back(std::move(elements));
            continue;
        }
        if (!part.empty()) Logger::warning("Unknown syntax of aperture macro part: " + part);
    }
    return primitives;
}

Shape ApertureMacro::makeGeometry(const std::vector<std::string>& modifiers) const {
    std::map<std::string, double> locvars;
    for (std::size_t i = 0; i < modifiers.size(); ++i) {
        locvars[std::to_string(i + 1)] = std::strtod(modifiers[i].c_str(), nullptr);
    }
    const auto primitives = parseContent(locvars);

    Shape geometry = emptyPolygon();
    for (const auto& prim : primitives) {
        const int code = static_cast<int>(prim[0]);
        const std::vector<double> mods(prim.begin() + 1, prim.end());
        Primitive made;
        switch (code) {
            case 1:  made = makeCircle(mods); break;
            case 2:
            case 20: made = makeVectorLine(mods); break;
            case 21: made = makeCenterLine(mods); break;
            case 22: made = makeLowerLeftLine(mods); break;
            case 4:  made = makeOutline(mods); break;
            case 5:  made = makePolygon(mods); break;
            case 6:  made = makeMoire(mods); break;
            case 7:  made = makeThermal(mods); break;
            default:
                throw GeometryError("Primitiva de macro no soportada: " + std::to_string(code));
        }
        if (made.exposure == 1) geometry = geometry.unite(made.geometry);
        else if (made.exposure == 0) geometry = geometry.difference(made.geometry);
    }
    return geometry;
}

ApertureMacro::Primitive ApertureMacro::makeCircle(const std::vector<double>& mods) {
    const auto v = padZero(4, mods);
    return {static_cast<int>(v[0]), Shape::point({v[2], v[3]}).buffer(v[1] / 2, kMacroResolution)};
}

ApertureMacro::Primitive ApertureMacro::makeVectorLine(const std::vector<double>& mods) {
    const auto v = padZero(7, mods);
    const Shape line = Shape::lineString({{v[2], v[3]}, {v[4], v[5]}});
    const Shape box = line.buffer(v[1] / 2, kMacroResolution, JoinStyle::Round, CapStyle::Flat);
    return {static_cast<int>(v[0]), box.rotate(v[6], Point2D{0, 0})};
}

ApertureMacro::Primitive ApertureMacro::makeCenterLine(const std::vector<double>& mods) {
    const auto v = padZero(6, mods);
    const Shape box = Shape::box(v[3] - v[1] / 2, v[4] - v[2] / 2, v[3] + v[1] / 2, v[4] + v[2] / 2);
    return {static_cast<int>(v[0]), box.rotate(v[5], Point2D{0, 0})};
}

ApertureMacro::Primitive ApertureMacro::makeLowerLeftLine(const std::vector<double>& mods) {
    const auto v = padZero(6, mods);
    const Shape box = Shape::box(v[3], v[4], v[3] + v[1], v[4] + v[2]);
    return {static_cast<int>(v[0]), box.rotate(v[5], Point2D{0, 0})};
}

ApertureMacro::Primitive ApertureMacro::makeOutline(const std::vector<double>& mods) {
    const int n = static_cast<int>(mods.at(1));
    std::vector<Point2D> points;
    for (int i = 0; i <= n; ++i) points.push_back({mods.at(2 * i + 2), mods.at(2 * i + 3)});
    const double angle = mods.at(2 * n + 4);
    return {static_cast<int>(mods[0]), Shape::polygon(points).rotate(angle, Point2D{0, 0})};
}

ApertureMacro::Primitive ApertureMacro::makePolygon(const std::vector<double>& mods) {
    const auto v = padZero(6, mods);
    const int nverts = static_cast<int>(v[1]);
    std::vector<Point2D> points;
    for (int i = 0; i < nverts; ++i) {
        points.push_back({v[2] + 0.5 * v[4] * std::cos(2 * kPi * i / nverts),
                          v[3] + 0.5 * v[4] * std::sin(2 * kPi * i / nverts)});
    }
    return {static_cast<int>(v[0]), Shape::polygon(points).rotate(v[5], Point2D{0, 0})};
}

ApertureMacro::Primitive ApertureMacro::makeMoire(const std::vector<double>& mods) {
    const auto v = padZero(9, mods);
    const double x = v[0], y = v[1], dia = v[2], thickness = v[3], gap = v[4];
    const double nrings = v[5], crossTh = v[6], crossLen = v[7];
    double r = dia / 2 - thickness / 2;
    auto ringAt = [&](double radius) {
        return Shape::point({x, y}).buffer(radius, kMacroResolution).exterior().buffer(thickness / 2.0, kMacroResolution);
    };
    Shape result = ringAt(r);
    Shape ring = ringAt(r);
    int i = 1;
    while (!ring.interiors().empty() && i < nrings) {
        r -= thickness + gap;
        if (r <= 0) break;
        ring = ringAt(r);
        result = Shape::unaryUnion({result, ring});
        ++i;
    }
    const Shape hor = Shape::lineString({{x - crossLen, y}, {x + crossLen, y}})
                          .buffer(crossTh / 2.0, kMacroResolution, JoinStyle::Round, CapStyle::Flat);
    const Shape ver = Shape::lineString({{x, y - crossLen}, {x, y + crossLen}})
                          .buffer(crossTh / 2.0, kMacroResolution, JoinStyle::Round, CapStyle::Flat);
    return {1, Shape::unaryUnion({result, hor, ver})};
}

ApertureMacro::Primitive ApertureMacro::makeThermal(const std::vector<double>& mods) {
    const auto v = padZero(6, mods);
    const double x = v[0], y = v[1], dout = v[2], din = v[3], t = v[4];
    const Shape ring = Shape::point({x, y}).buffer(dout / 2.0, kMacroResolution)
                           .difference(Shape::point({x, y}).buffer(din / 2.0, kMacroResolution));
    const Shape hline = Shape::lineString({{x - dout / 2.0, y}, {x + dout / 2.0, y}})
                            .buffer(t / 2.0, kMacroResolution, JoinStyle::Round, CapStyle::Square);
    const Shape vline = Shape::lineString({{x, y - dout / 2.0}, {x, y + dout / 2.0}})
                            .buffer(t / 2.0, kMacroResolution, JoinStyle::Round, CapStyle::Square);
    return {1, ring.difference(hline.unite(vline))};
}

}  // namespace flatcam
