#include "parsers/Gerber.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <regex>
#include <stdexcept>

#include "core/ArcMath.h"
#include "core/GeometryError.h"
#include "core/Logger.h"

namespace flatcam {

namespace {

constexpr double kPi = 3.14159265358979323846;

// Expresiones regulares del parser original (appParsers/ParseGerber.py).
const std::regex kFmt(R"re(%?FS([LTD])?([AI])X(\d)(\d)Y\d\d\*%?$)re");
const std::regex kFmtAlt(R"re(%FS([LTD])?([AI])X(\d)(\d)Y\d\d\*MO(IN|MM)\*%$)re");
const std::regex kFmtOrcad(R"re((G\d+)*\**%FS([LTD])?([AI]).*X(\d)(\d)Y\d\d\*%$)re");
const std::regex kMode(R"re(^%?MO(IN|MM)\*%?$)re");
const std::regex kComm(R"re(^G0?4(.*)$)re");
const std::regex kAd(R"re(^%ADD(\d\d+)([a-zA-Z_$\.][a-zA-Z0-9_$\.\-]*)(?:,(.*))?\*%$)re");
const std::regex kTool(R"re(^(?:G54)?D(\d\d+)\*$)re");
const std::regex kOpcode(R"re(^D0?([123])\*$)re");
const std::regex kInterp(R"re(^(?:G0?([123]))\*)re");
const std::regex kQuad(R"re(^G7([45]).*\*$)re");
const std::regex kRegionOn(R"re(^G36\*$)re");
const std::regex kRegionOff(R"re(^G37\*$)re");
const std::regex kEof(R"re(^M02\*)re");
const std::regex kLpol(R"re(^%LP([DC])\*%$)re");
const std::regex kUnits(R"re(^G7([01])\*$)re");
const std::regex kAbsRel(R"re(^G9([01])\*$)re");
const std::regex kAm1(R"re(^%AM([^\*]+)\*([^%]+)?(%)?$)re");
const std::regex kAm2(R"re((.*)%$)re");

/// Quita los caracteres indicados de ambos extremos (`str.strip(chars)`).
std::string strip(const std::string& s, const std::string& chars) {
    const auto b = s.find_first_not_of(chars);
    if (b == std::string::npos) return "";
    const auto e = s.find_last_not_of(chars);
    return s.substr(b, e - b + 1);
}

/// Equivale a `(?=.*L([\+-]?\d+))`: el número que sigue a la última aparición válida de la letra.
std::optional<std::string> numberAfterLast(const std::string& s, char letter) {
    for (std::size_t i = s.size(); i-- > 0;) {
        if (s[i] != letter) continue;
        std::size_t p = i + 1;
        const std::size_t start = p;
        if (p < s.size() && (s[p] == '+' || s[p] == '-')) ++p;
        const std::size_t digits = p;
        while (p < s.size() && std::isdigit(static_cast<unsigned char>(s[p]))) ++p;
        if (p > digits) return s.substr(start, p - start);
    }
    return std::nullopt;
}

/// Resultado de interpretar una sentencia de movimiento lineal (`lin_re`).
struct LinMatch {
    std::optional<std::string> x, y;
    std::optional<int> d;
};

/// Equivale a `^(?:G0?(1))?(?=.*X..)?(?=.*Y..)?[XY][^DIJ]*(?:D0?([123]))?\*$`.
bool matchLin(const std::string& s, LinMatch& m) {
    if (s.size() < 2 || s.back() != '*') return false;
    std::size_t pos = 0;
    if (s[0] == 'G') {
        std::size_t p = 1;
        if (p < s.size() && s[p] == '0') ++p;
        if (p < s.size() && s[p] == '1') pos = p + 1;
    }
    if (pos >= s.size() || (s[pos] != 'X' && s[pos] != 'Y')) return false;
    const std::string body = s.substr(pos + 1, s.size() - pos - 2);
    const std::size_t dpos = body.find('D');
    const std::string before = body.substr(0, dpos);
    if (before.find_first_of("IJ") != std::string::npos) return false;
    m.d.reset();
    if (dpos != std::string::npos) {
        std::string code = body.substr(dpos + 1);
        if (!code.empty() && code[0] == '0') code.erase(0, 1);
        if (code.size() != 1 || code[0] < '1' || code[0] > '3') return false;
        m.d = code[0] - '0';
    }
    const std::string tail = s.substr(pos);
    m.x = numberAfterLast(tail, 'X');
    m.y = numberAfterLast(tail, 'Y');
    return true;
}

/// Resultado de interpretar una sentencia de arco (`circ_re`).
struct CircMatch {
    std::optional<int> mode;
    std::optional<std::string> x, y, i, j;
    std::optional<int> d;
};

/// Equivale a `^(?:G0?([23]))?...[XYIJ][^D]*(?:D0([12]))?\*$`.
bool matchCirc(const std::string& s, CircMatch& m) {
    if (s.size() < 2 || s.back() != '*') return false;
    std::size_t pos = 0;
    m.mode.reset();
    if (s[0] == 'G') {
        std::size_t p = 1;
        if (p < s.size() && s[p] == '0') ++p;
        if (p < s.size() && (s[p] == '2' || s[p] == '3')) {
            m.mode = s[p] - '0';
            pos = p + 1;
        }
    }
    if (pos >= s.size() || std::string("XYIJ").find(s[pos]) == std::string::npos) return false;
    const std::string body = s.substr(pos + 1, s.size() - pos - 2);
    const std::size_t dpos = body.find('D');
    m.d.reset();
    if (dpos != std::string::npos) {
        const std::string code = body.substr(dpos + 1);
        if (code.size() != 2 || code[0] != '0' || (code[1] != '1' && code[1] != '2')) return false;
        m.d = code[1] - '0';
    }
    const std::string tail = s.substr(pos);
    m.x = numberAfterLast(tail, 'X');
    m.y = numberAfterLast(tail, 'Y');
    m.i = numberAfterLast(tail, 'I');
    m.j = numberAfterLast(tail, 'J');
    return true;
}

Shape emptyPolygon() { return Shape::fromWkt("POLYGON EMPTY"); }

/// Descompone una figura en sus polígonos (equivale a iterar un MultiPolygon de Shapely 1.8).
ShapeList polygonParts(const Shape& s) {
    if (s.type() == ShapeType::Polygon) return {s};
    return s.parts();
}

}  // namespace

// ----------------------------------------------------------------------------- utilidades públicas

double Gerber::parseGerberNumber(const std::string& number, int intDigits, int fracDigits, char zeros) {
    auto pow10 = [](int n) {
        double r = 1.0;
        for (int i = 0; i < n; ++i) r *= 10.0;
        return r;
    };
    const auto intVal = static_cast<double>(std::stoll(number));
    if (zeros == 'T') {
        const int k = (intDigits + fracDigits) - static_cast<int>(number.size());
        const double scaled = k >= 0 ? intVal * pow10(k) : intVal * (1.0 / pow10(-k));
        return scaled * (1.0 / pow10(fracDigits));
    }
    return intVal * (1.0 / pow10(fracDigits));
}

Shape Gerber::createFlashGeometry(const Point2D& loc, const Aperture& ap, int stepsPerCircle) {
    const int quad = stepsPerCircle / 4;
    if (ap.type == "C") {
        return Shape::point(loc).buffer(ap.size.value() / 2, quad);
    }
    if (ap.type == "R") {
        return Shape::box(loc.x - ap.width / 2, loc.y - ap.height / 2, loc.x + ap.width / 2, loc.y + ap.height / 2);
    }
    if (ap.type == "O") {
        Shape c1, c2;
        if (ap.width > ap.height) {
            c1 = Shape::point({loc.x + 0.5 * (ap.width - ap.height), loc.y}).buffer(ap.height * 0.5, quad);
            c2 = Shape::point({loc.x - 0.5 * (ap.width - ap.height), loc.y}).buffer(ap.height * 0.5, quad);
        } else {
            c1 = Shape::point({loc.x, loc.y + 0.5 * (ap.height - ap.width)}).buffer(ap.width * 0.5, quad);
            c2 = Shape::point({loc.x, loc.y - 0.5 * (ap.height - ap.width)}).buffer(ap.width * 0.5, quad);
        }
        return Shape::unaryUnion({c1, c2}).convexHull();
    }
    if (ap.type == "P") {
        std::vector<Point2D> points;
        for (int i = 0; i < ap.nVertices; ++i) {
            points.push_back({loc.x + 0.5 * ap.diam * std::cos(2 * kPi * i / ap.nVertices),
                              loc.y + 0.5 * ap.diam * std::sin(2 * kPi * i / ap.nVertices)});
        }
        Shape ply = Shape::polygon(points);
        if (ap.rotation) ply = ply.rotate(*ap.rotation);
        return ply;
    }
    if (ap.type == "AM") {
        const Shape flash = ap.macro->makeGeometry(ap.modifiers);
        if (flash.isEmpty()) Logger::warning("Empty geometry for Aperture Macro: " + ap.macro->name());
        return flash.translate(loc.x, loc.y);
    }
    throw GeometryError("Unknown aperture type: " + ap.type);
}

// ------------------------------------------------------------------------------------ construcción

Gerber::Gerber(const GerberOptions& options)
    : Geometry(options.circleSteps, options.defaultUnits),
      options_(options),
      zeros_(options.defaultZeros) {
    solidGeometry().clear();
}

Gerber::~Gerber() = default;

std::optional<std::string> Gerber::apertureParse(const std::string& id, const std::string& type,
                                                 const std::optional<std::string>& params) {
    const std::string apid = std::to_string(std::stoi(id));
    std::vector<std::string> list;
    if (params) {
        std::size_t b = 0;
        for (;;) {
            const std::size_t e = params->find('X', b);
            list.push_back(params->substr(b, e == std::string::npos ? std::string::npos : e - b));
            if (e == std::string::npos) break;
            b = e + 1;
        }
    }
    auto num = [&](std::size_t i) {
        if (i >= list.size()) throw std::runtime_error("parámetro de apertura faltante");
        return std::stod(list[i]);
    };
    Aperture ap;
    if (type == "C") {
        ap.type = "C";
        ap.size = num(0);
        apertures_[apid] = ap;
        return apid;
    }
    if (type == "R" || type == "O") {
        ap.type = type;
        ap.width = num(0);
        ap.height = num(1);
        ap.size = std::sqrt(ap.width * ap.width + ap.height * ap.height);  // "Hack" del original
        apertures_[apid] = ap;
        return apid;
    }
    if (type == "P") {
        ap.type = "P";
        ap.diam = num(0);
        ap.nVertices = static_cast<int>(std::stod(list.at(1)));
        ap.size = ap.diam;  // "Hack" del original
        if (list.size() >= 3) ap.rotation = num(2);
        apertures_[apid] = ap;
        return apid;
    }
    const auto macro = macros_.find(type);
    if (macro != macros_.end()) {
        ap.type = "AM";
        ap.macro = macro->second;
        ap.modifiers = list;
        apertures_[apid] = ap;
        return apid;
    }
    Logger::warning("Aperture not implemented: " + type);
    return std::nullopt;
}

bool Gerber::parseFile(const std::string& path, bool /*follow*/) {
    std::ifstream in(path);
    if (!in) {
        lastError_ = "No se pudo abrir el archivo: " + path;
        Logger::error(lastError_);
        return false;
    }
    std::vector<std::string> statements;
    std::string line;
    while (std::getline(in, line)) {
        line = strip(line, " \r\n");
        while (!line.empty()) {
            if (line.back() == '%') {
                statements.push_back(line);
                break;
            }
            const std::size_t star = line.find('*');
            if (star != std::string::npos) {
                statements.push_back(line.substr(0, star + 1));
                line = line.substr(star + 1);
            } else {
                statements.push_back(line);
                break;
            }
        }
    }
    return parseLines(statements);
}

// ------------------------------------------------------------------------------------ análisis

bool Gerber::parseLines(const std::vector<std::string>& glines) {
    lastError_.clear();
    const int steps = options_.circleSteps;
    const int quad = steps / 4;  // segmentos por cuarto de círculo de los trazos

    // Estado del análisis (mismas variables que `parse_lines` de Python).
    std::vector<Point2D> path;
    std::optional<Shape> geoS, geoF;
    ShapeList polyBuffer, followBuffer;
    std::optional<std::string> lastPathAperture, currentAperture;
    int currentInterpolationMode = -1;  // None
    int currentOperationCode = -1;      // None
    double currentX = 0, currentY = 0;
    enum class Quadrant { None, Single, Multi } quadrantMode = Quadrant::None;
    std::optional<std::string> currentMacro;
    char currentPolarity = 'D';
    bool makingRegion = false;
    int lineNum = 0;
    std::string gline;
    Shape solid = emptyPolygon();  // acumulador `self.solid_geometry` durante el análisis

    // ---- utilidades locales
    auto keyError = [&](const std::optional<std::string>& key) -> const Aperture& {
        if (!key) throw std::runtime_error("KeyError: None");
        const auto it = apertures_.find(*key);
        if (it == apertures_.end()) throw std::runtime_error("KeyError: " + *key);
        return it->second;
    };
    auto apertureType = [&](const std::optional<std::string>& key) -> std::optional<std::string> {
        if (!key) return std::nullopt;
        const auto it = apertures_.find(*key);
        if (it == apertures_.end() || it->second.type.empty()) return std::nullopt;
        return it->second.type;
    };
    auto strokeWidth = [&](const std::optional<std::string>& key) {
        const Aperture& ap = keyError(key);
        if (!ap.size) throw std::runtime_error("KeyError: size");
        return *ap.size;
    };
    auto addPoly = [&](const Shape& s) {
        polyBuffer.push_back(options_.simplification ? s.simplify(options_.simpTolerance) : s);
    };
    auto addGeometry = [&](const std::optional<std::string>& key, const ApertureGeometry& g) {
        if (!key) throw std::runtime_error("KeyError: None");
        Aperture& ap = apertures_[*key];
        ap.hasGeometry = true;
        ap.geometry.push_back(g);
    };
    auto setSolidOrClear = [&](ApertureGeometry& d, const Shape& s) {
        if (isLpc_) d.clear = s; else d.solid = s;
    };
    auto ensureRegionAperture = [&]() {
        if (apertures_.find("0") == apertures_.end()) {
            Aperture reg;
            reg.type = "REG";
            reg.size = 0.0;
            reg.hasGeometry = true;
            apertures_["0"] = reg;
        }
    };
    auto notEmpty = [](const std::optional<Shape>& s) { return s && !s->isEmpty(); };
    auto parseNum = [&](const std::string& n) { return parseGerberNumber(n, intDigits_, fracDigits_, zeros_); };

    // Cierra el trazo en curso y lo guarda (patrón repetido en el original).
    //  @p rCheck: no hace nada con aperturas rectangulares; @p validCheck: exige figura válida.
    auto flushStroke = [&](int res, bool rCheck, bool validCheck, bool resetPath) {
        if (path.size() <= 1) return;
        if (rCheck) {
            const auto t = apertureType(lastPathAperture);
            if (t && *t == "R") return;
        }
        ApertureGeometry dict;
        const Shape gf = Shape::lineString(path);
        if (!gf.isEmpty()) {
            followBuffer.push_back(gf);
            dict.follow = gf;
        }
        const double width = strokeWidth(lastPathAperture);
        const Shape gs = Shape::lineString(path).buffer(width / 1.999, res);
        if (!gs.isEmpty() && (!validCheck || gs.isValid())) {
            addPoly(gs);
            setSolidOrClear(dict, gs);
        }
        addGeometry(lastPathAperture, dict);
        if (resetPath) path = {path.back()};
    };

    auto flashAt = [&](const Point2D& loc, const std::optional<std::string>& ap, bool bookkeepOnlyIfSolid) {
        ApertureGeometry dict;
        const Shape pt = Shape::point(loc);
        const Shape flash = createFlashGeometry(loc, keyError(ap), steps);
        dict.follow = pt;
        followBuffer.push_back(pt);
        if (!flash.isEmpty()) {
            addPoly(flash);
            setSolidOrClear(dict, flash);
            addGeometry(ap, dict);
        } else if (!bookkeepOnlyIfSolid) {
            addGeometry(ap, dict);
        }
    };

    try {
        for (const std::string& rawLine : glines) {
            ++lineNum;
            sourceFile_ += rawLine + "\n";
            gline = strip(rawLine, " \r\n");
            std::smatch m;

            if (std::regex_search(gline, m, kComm)) continue;

            // ---- polaridad de la capa
            if (std::regex_search(gline, m, kLpol)) {
                const char newPolarity = m[1].str()[0];
                isLpc_ = (newPolarity == 'C');
                if (path.size() > 1 && currentPolarity != newPolarity) {
                    const double width = strokeWidth(lastPathAperture);
                    ApertureGeometry dict;
                    const Shape gf = Shape::lineString(path);
                    if (!gf.isEmpty()) {
                        followBuffer.push_back(gf);
                        dict.follow = gf;
                    }
                    const Shape gs = Shape::lineString(path).buffer(width / 1.999, quad);
                    if (!gs.isEmpty() && gs.isValid()) {
                        addPoly(gs);
                        setSolidOrClear(dict, gs);
                    }
                    addGeometry(lastPathAperture, dict);
                    path = {path.back()};
                }
                if (!polyBuffer.empty()) {
                    const Shape u = Shape::unaryUnion(polyBuffer);
                    solid = (currentPolarity == 'D') ? solid.unite(u) : solid.difference(u);
                    polyBuffer.clear();
                }
                currentPolarity = newPolarity;
                continue;
            }

            // ---- formato de coordenadas y unidades
            if (std::regex_search(gline, m, kFmt)) {
                if (m[1].matched) zeros_ = m[1].str()[0];
                intDigits_ = std::stoi(m[3].str());
                fracDigits_ = std::stoi(m[4].str());
                continue;
            }
            if (std::regex_search(gline, m, kMode)) {
                setUnits(m[1].str() == "IN" ? Units::Inches : Units::Millimeters);
                conversionDone_ = true;
                continue;
            }
            if (std::regex_search(gline, m, kFmtAlt)) {
                if (m[1].matched) zeros_ = m[1].str()[0];
                intDigits_ = std::stoi(m[3].str());
                fracDigits_ = std::stoi(m[4].str());
                setUnits(m[5].str() == "IN" ? Units::Inches : Units::Millimeters);
                conversionDone_ = true;
                continue;
            }
            if (std::regex_search(gline, m, kFmtOrcad)) {
                if (m[1].matched) {
                    if (m[1].str() == "G74") quadrantMode = Quadrant::Single;
                    else if (m[1].str() == "G75") quadrantMode = Quadrant::Multi;
                    if (m[2].matched) zeros_ = m[2].str()[0];
                    intDigits_ = std::stoi(m[4].str());
                    fracDigits_ = std::stoi(m[5].str());
                    conversionDone_ = true;
                    continue;
                }
            }
            if (std::regex_search(gline, m, kUnits)) {
                setUnits(m[1].str() == "0" ? Units::Inches : Units::Millimeters);
                Logger::warning("Gerber obsolete units found");
                conversionDone_ = true;
                continue;
            }
            if (std::regex_search(gline, m, kAbsRel)) {
                Logger::warning("Gerber obsolete coordinates type found");
                continue;
            }

            // ---- macros de apertura
            if (!currentMacro) {
                if (std::regex_search(gline, m, kAm1)) {
                    currentMacro = m[1].str();
                    macros_[*currentMacro] = std::make_shared<ApertureMacro>(*currentMacro);
                    if (m[2].matched && m[2].length() > 0) macros_[*currentMacro]->append(m[2].str());
                    if (m[3].matched) currentMacro.reset();
                    continue;
                }
            } else {
                if (std::regex_search(gline, m, kAm2)) {
                    macros_[*currentMacro]->append(m[1].str());
                    currentMacro.reset();
                } else {
                    macros_[*currentMacro]->append(gline);
                }
                continue;
            }

            // ---- definición de apertura
            if (std::regex_search(gline, m, kAd)) {
                apertureParse(m[1].str(), m[2].str(),
                              m[3].matched ? std::optional<std::string>(m[3].str()) : std::nullopt);
                continue;
            }

            // ---- D01/D02/D03 sin coordenadas
            if (std::regex_search(gline, m, kOpcode)) {
                currentOperationCode = std::stoi(m[1].str());
                if (currentOperationCode == 3) {
                    if (!currentAperture) {
                        Logger::warning("Nothing there to flash!");
                    } else {
                        flashAt({currentX, currentY}, currentAperture, true);
                    }
                }
                continue;
            }

            // ---- selección de apertura
            if (std::regex_search(gline, m, kTool)) {
                currentAperture = m[1].str();
                Aperture& cur = const_cast<Aperture&>(keyError(currentAperture));
                if (cur.type != "AM" && cur.size && *cur.size == 0) cur.size = 1e-12;
                flushStroke(quad, /*rCheck=*/true, /*validCheck=*/false, /*resetPath=*/true);
                continue;
            }

            // ---- regiones
            if (std::regex_search(gline, m, kRegionOn)) {
                flushStroke(quad, false, false, true);
                makingRegion = true;
                continue;
            }
            if (std::regex_search(gline, m, kRegionOff)) {
                makingRegion = false;
                ensureRegionAperture();
                if (currentOperationCode == 2 && path.size() == 1) {
                    ApertureGeometry dict;
                    if (notEmpty(geoF)) {
                        followBuffer.push_back(*geoF);
                        dict.follow = *geoF;
                    }
                    if (notEmpty(geoS)) {
                        addPoly(*geoS);
                        setSolidOrClear(dict, *geoS);
                    }
                    if (notEmpty(geoS) || notEmpty(geoF)) addGeometry(std::string("0"), dict);
                    path = {{currentX, currentY}};
                }
                if (path.size() < 3) continue;
                ApertureGeometry dict;
                const Shape regionGeo = Shape::polygon(path);
                const Shape regionF = regionGeo.exterior();
                if (!regionF.isEmpty()) {
                    followBuffer.push_back(regionF);
                    dict.follow = regionF;
                }
                Shape regionS = regionGeo;
                if (!regionS.isValid()) regionS = regionS.buffer(0, steps);
                if (!regionS.isEmpty()) {
                    addPoly(regionS);
                    setSolidOrClear(dict, regionS);
                }
                if (!regionS.isEmpty() || !regionF.isEmpty()) addGeometry(std::string("0"), dict);
                path = {{currentX, currentY}};
                continue;
            }

            // ---- modo de interpolación
            if (std::regex_search(gline, m, kInterp)) {
                currentInterpolationMode = std::stoi(m[1].str());
                continue;
            }

            // ---- movimientos lineales
            LinMatch lin;
            if (matchLin(gline, lin)) {
                double linearX = currentX, linearY = currentY;
                if (lin.x) {
                    linearX = parseNum(*lin.x);
                    currentX = linearX;
                }
                if (lin.y) {
                    linearY = parseNum(*lin.y);
                    currentY = linearY;
                }
                if (lin.d) currentOperationCode = *lin.d;

                if (currentOperationCode == 1) {
                    if (path.empty()) throw std::runtime_error("list index out of range");
                    if (path.back() != Point2D{currentX, currentY}) {
                        path.push_back({currentX, currentY});
                    } else if (path.size() == 1) {
                        path = {{currentX, currentY}};
                        if (currentAperture) flashAt({currentX, currentY}, currentAperture, true);
                    }
                    if (!makingRegion) {
                        const auto t = apertureType(currentAperture);
                        if (t && *t == "R" && path.size() >= 2) {
                            const Aperture& ap = keyError(currentAperture);
                            const double minx = std::min(path[0].x, path[1].x) - ap.width / 2;
                            const double maxx = std::max(path[0].x, path[1].x) + ap.width / 2;
                            const double miny = std::min(path[0].y, path[1].y) - ap.height / 2;
                            const double maxy = std::max(path[0].y, path[1].y) + ap.height / 2;
                            ApertureGeometry dict;
                            const Shape pt = Shape::point({currentX, currentY});
                            followBuffer.push_back(pt);
                            dict.follow = pt;
                            const Shape box = Shape::box(minx, miny, maxx, maxy);
                            addPoly(box);
                            setSolidOrClear(dict, box);
                            addGeometry(currentAperture, dict);
                        }
                    }
                    lastPathAperture = currentAperture;
                    if (!lastPathAperture) {
                        ensureRegionAperture();
                        lastPathAperture = "0";
                    }
                } else if (currentOperationCode == 2) {
                    if (path.size() > 1) {
                        geoS.reset();
                        ApertureGeometry dict;
                        if (makingRegion) {
                            if (!lastPathAperture) {
                                ensureRegionAperture();
                                lastPathAperture = "0";
                            }
                            geoF = emptyPolygon();
                        } else {
                            geoF = Shape::lineString(path);
                        }
                        const auto t = apertureType(lastPathAperture);
                        if (!t || *t != "R") {
                            if (!geoF->isEmpty()) {
                                followBuffer.push_back(*geoF);
                                dict.follow = *geoF;
                            }
                        }
                        if (makingRegion) {
                            try {
                                geoS = Shape::polygon(path);
                            } catch (const GeometryError&) {
                                Logger::error("Region does not have enough points. Line number " + std::to_string(lineNum));
                            }
                        } else {
                            const double width = strokeWidth(lastPathAperture);
                            geoS = Shape::lineString(path).buffer(width / 1.999, quad);
                        }
                        if (geoS) {
                            if (!t || *t != "R") {
                                if (!geoS->isEmpty()) {
                                    addPoly(*geoS);
                                    setSolidOrClear(dict, *geoS);
                                }
                            }
                        }
                        addGeometry(lastPathAperture, dict);
                    }
                    path = {{linearX, linearY}};
                } else if (currentOperationCode == 3) {
                    if (path.size() > 1) {
                        ApertureGeometry dict;
                        const Shape gf = Shape::lineString(path);
                        const auto t = apertureType(lastPathAperture);
                        if (!gf.isEmpty()) {
                            if (!t || *t != "R") {
                                followBuffer.push_back(gf);
                                dict.follow = gf;
                            }
                        }
                        const double width = strokeWidth(lastPathAperture);
                        const Shape gs = Shape::lineString(path).buffer(width / 1.999, quad);
                        if (!gs.isEmpty()) {
                            if (!t || *t != "R") {
                                addPoly(gs);
                                setSolidOrClear(dict, gs);
                            }
                        }
                        addGeometry(lastPathAperture, dict);
                    }
                    path = {{linearX, linearY}};
                    ApertureGeometry dict;
                    const Shape pt = Shape::point({linearX, linearY});
                    followBuffer.push_back(pt);
                    dict.follow = pt;
                    const Shape flash = createFlashGeometry({linearX, linearY}, keyError(currentAperture), steps);
                    if (!flash.isEmpty()) {
                        addPoly(flash);
                        setSolidOrClear(dict, flash);
                    }
                    addGeometry(currentAperture, dict);
                }
                currentX = linearX;
                currentY = linearY;
                continue;
            }

            // ---- modo de cuadrante
            if (std::regex_search(gline, m, kQuad)) {
                quadrantMode = (m[1].str() == "4") ? Quadrant::Single : Quadrant::Multi;
                continue;
            }

            // ---- arcos
            CircMatch circ;
            if (matchCirc(gline, circ)) {
                const std::string arcdir[4] = {"", "", "cw", "ccw"};
                double circularX = circ.x ? parseNum(*circ.x) : currentX;
                double circularY = circ.y ? parseNum(*circ.y) : currentY;
                double ii = circ.i ? parseNum(*circ.i) : 0.0;
                double jj = circ.j ? parseNum(*circ.j) : 0.0;

                if (quadrantMode == Quadrant::None) {
                    Logger::error("Found arc without preceding quadrant specification G74 or G75. (" +
                                  std::to_string(lineNum) + ")");
                    continue;
                }
                if (!circ.mode && currentInterpolationMode != 2 && currentInterpolationMode != 3) {
                    Logger::error("Found arc without circular interpolation mode defined. (" + std::to_string(lineNum) + ")");
                    continue;
                }
                if (circ.mode) currentInterpolationMode = *circ.mode;
                if (circ.d) currentOperationCode = *circ.d;

                if (currentOperationCode == 2) {
                    if (path.size() > 1) {
                        ApertureGeometry dict;
                        const double width = strokeWidth(lastPathAperture);
                        const Shape gf = Shape::lineString(path);
                        if (!gf.isEmpty()) {
                            followBuffer.push_back(gf);
                            dict.follow = gf;
                        }
                        const Shape buffered = Shape::lineString(path).buffer(width / 1.999, steps);
                        if (!buffered.isEmpty()) {
                            addPoly(buffered);
                            setSolidOrClear(dict, buffered);
                        }
                        addGeometry(lastPathAperture, dict);
                    }
                    currentX = circularX;
                    currentY = circularY;
                    path = {{currentX, currentY}};
                    continue;
                }
                if (currentOperationCode == 3) continue;

                const ArcDirection dir = (currentInterpolationMode == 2) ? ArcDirection::Clockwise
                                                                         : ArcDirection::CounterClockwise;
                if (quadrantMode == Quadrant::Multi) {
                    const Point2D center{ii + currentX, jj + currentY};
                    const double radius = std::sqrt(ii * ii + jj * jj);
                    const double start = std::atan2(-jj, -ii);
                    double stop;
                    if (currentX == circularX && currentY == circularY) stop = start;
                    else stop = std::atan2(-center.y + circularY, -center.x + circularX);
                    std::vector<Point2D> thisArc = arc(center, radius, start, stop, dir, steps);
                    thisArc.back() = {circularX, circularY};
                    currentX = circularX;
                    currentY = circularY;
                    path.insert(path.end(), thisArc.begin(), thisArc.end());
                    lastPathAperture = currentAperture;
                    continue;
                }
                // Cuadrante simple (G74): se prueba cada signo posible del centro.
                const Point2D candidates[4] = {{ii + currentX, jj + currentY},
                                               {-ii + currentX, jj + currentY},
                                               {ii + currentX, -jj + currentY},
                                               {-ii + currentX, -jj + currentY}};
                bool valid = false;
                for (const Point2D& center : candidates) {
                    const double radius = std::sqrt(ii * ii + jj * jj);
                    const double radius2 = std::sqrt((center.x - circularX) * (center.x - circularX) +
                                                     (center.y - circularY) * (center.y - circularY));
                    if (radius2 < radius * 0.95 || radius2 > radius * 1.05) continue;
                    ii = center.x - currentX;
                    jj = center.y - currentY;
                    const double start = std::atan2(-jj, -ii);
                    const double stop = std::atan2(-center.y + circularY, -center.x + circularX);
                    const double angle = std::fabs(arcAngle(start, stop, dir));
                    if (angle <= (kPi + 1e-6) / 2) {
                        std::vector<Point2D> thisArc = arc(center, radius, start, stop, dir, steps);
                        thisArc.back() = {circularX, circularY};
                        currentX = circularX;
                        currentY = circularY;
                        path.insert(path.end(), thisArc.begin(), thisArc.end());
                        lastPathAperture = currentAperture;
                        valid = true;
                        break;
                    }
                }
                if (!valid) {
                    Logger::warning("Invalid arc in line " + std::to_string(lineNum) + ": " + gline);
                }
                continue;
            }

            if (std::regex_search(gline, m, kEof)) continue;

            Logger::warning("Line ignored (" + std::to_string(lineNum) + "): " + gline);
        }

        // ---- fin del archivo: se cierra el último trazo
        if (path.size() > 1) {
            const auto t = apertureType(lastPathAperture);
            if (!(t && *t == "R")) {
                ApertureGeometry dict;
                const Shape gf = Shape::lineString(path);
                if (!gf.isEmpty()) {
                    followBuffer.push_back(gf);
                    dict.follow = gf;
                }
                const double width = strokeWidth(lastPathAperture);
                const Shape gs = Shape::lineString(path).buffer(width / 1.999, quad);
                if (!gs.isEmpty()) {
                    addPoly(gs);
                    setSolidOrClear(dict, gs);
                }
                addGeometry(lastPathAperture, dict);
            }
        }

        followGeometry() = followBuffer;

        if (polyBuffer.empty() && solid.area() == 0) {
            lastError_ = "El archivo no produjo geometría";
            solidGeometry() = {solid};
            return false;
        }

        // ---- unión final y polaridad
        Shape newPoly;
        if (options_.useBufferForUnion) {
            newPoly = Shape::collection(polyBuffer);
            if (options_.buffering == "full") {
                newPoly = newPoly.buffer(0.00000001);
                newPoly = newPoly.buffer(-0.00000001);
            }
        } else {
            newPoly = Shape::unaryUnion(polyBuffer);
            newPoly = newPoly.buffer(0, quad);
        }

        if (currentPolarity == 'D') {
            ShapeList result;
            if (newPoly.isValid()) {
                solid = solid.unite(newPoly);
                result = {solid};
            } else {
                for (const Shape& p : polygonParts(newPoly)) result.push_back(p);
                for (const Shape& p : polygonParts(solid)) result.push_back(p);
            }
            if (options_.extraBuffering) {
                ShapeList candidate;
                for (const Shape& r : result) {
                    for (const Shape& p : polygonParts(r)) candidate.push_back(p.buffer(-0.0000001));
                }
                result = candidate;
            }
            solidGeometry() = result;
        } else {
            solidGeometry() = {solid.difference(newPoly)};
        }

        if (options_.cleanApertures) {
            for (auto it = apertures_.begin(); it != apertures_.end();) {
                it = it->second.hasGeometry ? std::next(it) : apertures_.erase(it);
            }
        }
        conversionDone_ = false;
    } catch (const std::exception& e) {
        lastError_ = "Gerber Parser ERROR, line " + std::to_string(lineNum) + " [" + gline + "]: " + e.what();
        Logger::error(lastError_);
        return false;
    }
    return true;
}

// ------------------------------------------------------------------------- transformaciones

template <typename Fn>
void Gerber::transformAll(Fn fn) {
    for (Shape& s : solidGeometry()) s = fn(s);
    for (Shape& s : followGeometry()) s = fn(s);
    for (auto& [id, ap] : apertures_) {
        for (ApertureGeometry& g : ap.geometry) {
            if (g.solid) g.solid = fn(*g.solid);
            if (g.follow) g.follow = fn(*g.follow);
            if (g.clear) g.clear = fn(*g.clear);
        }
    }
}

void Gerber::scale(double xfactor, double yfactor, const std::optional<Point2D>& point) {
    if (xfactor == 0 && yfactor == 0) return;
    const Point2D origin = point.value_or(Point2D{0, 0});
    transformAll([&](const Shape& s) { return s.transform(AffineTransform::scaling(xfactor, yfactor, origin)); });
    for (auto& [id, ap] : apertures_) {
        if (ap.type == "R" || ap.type == "O") {
            ap.width *= xfactor;
            ap.height *= xfactor;
        } else if (ap.type == "P") {
            ap.diam *= xfactor;
        }
        if (ap.size) ap.size = *ap.size * xfactor;
    }
}

void Gerber::offset(const Point2D& vect) {
    if (vect.x == 0 && vect.y == 0) return;
    transformAll([&](const Shape& s) { return s.translate(vect.x, vect.y); });
}

void Gerber::mirror(Axis axis, const Point2D& point) {
    transformAll([&](const Shape& s) { return s.transform(AffineTransform::mirror(axis, point)); });
}

void Gerber::rotate(double angle, const Point2D& point) {
    if (angle == 0) return;
    transformAll([&](const Shape& s) { return s.rotate(angle, point); });
}

void Gerber::skew(double angleX, double angleY, const Point2D& point) {
    if (angleX == 0 && angleY == 0) return;
    transformAll([&](const Shape& s) { return s.skew(angleX, angleY, point); });
}

void Gerber::buffer(double distance, JoinStyle join, bool factor) {
    if (distance == 0) return;
    const int steps = options_.circleSteps;

    if (!factor) {
        auto bufferGeom = [&](const Shape& s) { return s.buffer(distance, steps, join); };
        for (Shape& s : solidGeometry()) s = bufferGeom(s);
        for (auto& [id, ap] : apertures_) {
            for (ApertureGeometry& g : ap.geometry) {
                if (g.solid) g.solid = bufferGeom(*g.solid);
                if (g.clear) g.clear = bufferGeom(*g.clear);
            }
            if (ap.type == "R" || ap.type == "O") {
                ap.width += distance * 2;
                ap.height += distance * 2;
            } else if (ap.type == "P") {
                ap.diam += distance * 2;
            }
            if (ap.size) ap.size = *ap.size + distance * 2;
        }
        return;
    }

    // Con factor: se escalan las aperturas y se rehace la geometría desde las líneas centrales.
    for (auto& [id, ap] : apertures_) {
        if (ap.type == "R" || ap.type == "O") {
            ap.width *= distance;
            ap.height *= distance;
        } else if (ap.type == "P") {
            ap.diam *= distance;
        }
        if (ap.size) ap.size = *ap.size * distance;
        if (!ap.hasGeometry) continue;
        if (!ap.size) {
            Logger::error("Gerber buffer: la apertura " + id + " no tiene tamaño");
            return;
        }
        std::vector<ApertureGeometry> rebuilt;
        for (const ApertureGeometry& g : ap.geometry) {
            ApertureGeometry n;
            if (g.follow) {
                n.follow = g.follow;
                const double size = *ap.size;
                if (g.follow->type() == ShapeType::Point) {
                    if (ap.type == "C") {
                        n.solid = g.follow->buffer(size / 1.9999, steps);
                    } else if (ap.type == "R") {
                        const Point2D c = g.follow->coordinates().front();
                        n.solid = Shape::box(c.x - ap.width / 2, c.y - ap.height / 2, c.x + ap.width / 2,
                                             c.y + ap.height / 2);
                    } else {
                        Logger::warning("Gerber buffer: tipo de apertura no soportado: " + ap.type);
                    }
                } else {
                    n.solid = g.follow->buffer(size / 1.9999, steps);
                }
            }
            if (g.clear) n.clear = g.clear;
            rebuilt.push_back(n);
        }
        ap.geometry = rebuilt;
    }
    ShapeList solids;
    for (const auto& [id, ap] : apertures_) {
        for (const ApertureGeometry& g : ap.geometry) {
            if (g.solid) solids.push_back(*g.solid);
        }
    }
    Shape result = Shape::collection(solids).buffer(0.000001).buffer(-0.000001);
    solidGeometry() = {result};
}

Shape Gerber::getBoundingBox(double margin, bool rounded) const {
    ShapeList parts;
    for (const Shape& s : solidGeometry()) {
        if (!s.isEmpty()) parts.push_back(s);
    }
    if (parts.empty()) return Shape();
    const auto b = Shape::unaryUnion(parts).bounds();
    if (!b) return Shape();
    Shape bbox = Shape::box(b->xmin, b->ymin, b->xmax, b->ymax).buffer(margin);
    if (!rounded) {
        const auto bb = bbox.bounds();
        if (bb) bbox = Shape::box(bb->xmin, bb->ymin, bb->xmax, bb->ymax);
    }
    return bbox;
}

}  // namespace flatcam
