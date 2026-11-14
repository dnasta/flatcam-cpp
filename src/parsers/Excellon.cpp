#include "parsers/Excellon.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <regex>
#include <set>
#include <stdexcept>

#include "core/GeometryError.h"
#include "core/Logger.h"

namespace flatcam {

namespace {

// Expresiones regulares del parser original (appParsers/ParseExcellon.py) que no usan lookahead.
const std::regex kHBegin(R"re(^M48$)re");
const std::regex kAllegroHBegin(R"re(;\s*(HEADER))re");
const std::regex kHEnd(R"re(^(?:M95|%)$)re");
const std::regex kUnits(R"re(^(INCH|METRIC)(?:,([TL])Z)?,?(\d*\.\d+)?.*$)re");
const std::regex kDetectGcode(R"re(^G2([01])$)re");
const std::regex kToolSel(R"re(^T(\d+))re");
const std::regex kToolSetHl(R"re(^T(\d+)(?:.?C(\d+\.?\d*))?)re");
const std::regex kComm(R"re(^;(.*)$)re");
const std::regex kAbsInc(R"re(^G9([01])$)re");
const std::regex kMeas(R"re(^M7([12])$)re");
const std::regex kSlots(R"re(^([^G]+)G85(.*)$)re");
const std::regex kStop(R"re(^((G04)|(M09)|(M06)|(M00)|(M30)))re");
const std::regex kToolUnits(R"re((;\s*Holesize \d+.\s*=\s*(\d+.\d+).*(MILS|MM)))re");
const std::regex kAltium(R"re(^;\s*(?:FILE_FORMAT)?(?:Format)?[=|:]\s*(\d+)[:|.](\d+).*$)re");
const std::regex kLeadingZeros(R"re(^[-\+]?(0*)(\d*))re");
const std::regex kRepeat(R"re(R(\d+))re");
const std::regex kLzTz(R"re([LT]Z)re");

std::string strip(const std::string& s, const std::string& chars) {
    const auto b = s.find_first_not_of(chars);
    if (b == std::string::npos) return "";
    const auto e = s.find_last_not_of(chars);
    return s.substr(b, e - b + 1);
}

/// Texto que sigue a la última aparición de @p letter con la forma `[-+]?\d*\.\d*` (con punto decimal).
std::optional<std::string> periodNumberAfterLast(const std::string& s, char letter) {
    for (std::size_t i = s.size(); i-- > 0;) {
        if (s[i] != letter) continue;
        std::size_t p = i + 1;
        const std::size_t start = p;
        if (p < s.size() && (s[p] == '+' || s[p] == '-')) ++p;
        while (p < s.size() && std::isdigit(static_cast<unsigned char>(s[p]))) ++p;
        if (p < s.size() && s[p] == '.') {
            ++p;
            while (p < s.size() && std::isdigit(static_cast<unsigned char>(s[p]))) ++p;
            return s.substr(start, p - start);
        }
    }
    return std::nullopt;
}

/// Texto que sigue a la última aparición de @p letter con la forma `[-+]?\d*` (sin punto decimal; puede ser vacío).
std::optional<std::string> plainNumberAfterLast(const std::string& s, char letter) {
    const std::size_t i = s.rfind(letter);
    if (i == std::string::npos) return std::nullopt;
    std::size_t p = i + 1;
    const std::size_t start = p;
    if (p < s.size() && (s[p] == '+' || s[p] == '-')) ++p;
    while (p < s.size() && std::isdigit(static_cast<unsigned char>(s[p]))) ++p;
    return s.substr(start, p - start);
}

struct CoordMatch {
    std::optional<std::string> x, y;
};

/// Equivale a `coordsperiod_re.search` (con punto decimal) o `coordsnoperiod_re.search` (sin él).
std::optional<CoordMatch> matchCoords(const std::string& s, bool period) {
    for (std::size_t p = 0; p < s.size(); ++p) {
        if (s[p] != 'X' && s[p] != 'Y') continue;
        const std::string tail = s.substr(p);
        if (!period && tail.find('.') != std::string::npos) continue;  // (?!.*\.)
        CoordMatch m;
        if (period) {
            m.x = periodNumberAfterLast(tail, 'X');
            m.y = periodNumberAfterLast(tail, 'Y');
        } else {
            m.x = plainNumberAfterLast(tail, 'X');
            m.y = plainNumberAfterLast(tail, 'Y');
        }
        return m;
        // La primera posición con X o Y que cumple la condición es la coincidencia, como en `re.search`.
    }
    return std::nullopt;
}

/// Equivale a `toolset_re`: `T<n>` seguido de una letra de parámetro y el diámetro tras la última `C`.
struct ToolSetMatch {
    int tool{0};
    std::string diameter;
    bool hasDiameter{false};
};

std::optional<ToolSetMatch> matchToolSet(const std::string& s) {
    if (s.empty() || s[0] != 'T') return std::nullopt;
    std::size_t p = 1;
    while (p < s.size() && std::isdigit(static_cast<unsigned char>(s[p]))) ++p;
    if (p == 1 || p >= s.size() || std::string("CFSBHT").find(s[p]) == std::string::npos) return std::nullopt;
    ToolSetMatch m;
    m.tool = std::stoi(s.substr(1, p - 1));
    const std::size_t c = s.rfind('C');
    if (c != std::string::npos && c >= p) {
        std::size_t q = c + 1;
        if (q < s.size() && s[q] == ',') ++q;
        const std::size_t start = q;
        while (q < s.size() && std::isdigit(static_cast<unsigned char>(s[q]))) ++q;
        if (q < s.size() && s[q] == '.') ++q;
        while (q < s.size() && std::isdigit(static_cast<unsigned char>(s[q]))) ++q;
        m.diameter = s.substr(start, q - start);
        m.hasDiameter = true;
    }
    return m;
}

}  // namespace

Excellon::Excellon(const ExcellonOptions& options)
    : Geometry(options.circleSteps, options.units),
      options_(options),
      zeros_(options.zeros),
      excellonUnits_(options.excellonUnits),
      formatUpperIn_(options.formatUpperIn),
      formatLowerIn_(options.formatLowerIn),
      formatUpperMm_(options.formatUpperMm),
      formatLowerMm_(options.formatLowerMm) {}

Excellon::~Excellon() = default;

std::optional<double> Excellon::parseNumber(const std::string& text) const {
    std::smatch m;
    if (!std::regex_search(text, m, kLeadingZeros)) return std::nullopt;
    const double nrLength = static_cast<double>(m[1].length() + m[2].length());
    try {
        const double value = std::stod(text);
        const bool inches = units() == Units::Inches;
        if (zeros_ == "L" || zeros_ == "LZ") {
            const double upper = inches ? formatUpperIn_ : formatUpperMm_;
            return value / std::pow(10.0, nrLength - upper);
        }
        const double lower = inches ? formatLowerIn_ : formatLowerMm_;
        return value / std::pow(10.0, lower);
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool Excellon::parseFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        lastError_ = "No se pudo abrir el archivo: " + path;
        Logger::error(lastError_);
        return false;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) lines.push_back(line);
    if (!parseLines(lines)) return false;
    return createGeometry();
}

bool Excellon::parseLines(const std::vector<std::string>& elines) {
    lastError_.clear();

    std::optional<int> currentTool;
    bool inHeader = false, headerless = false;
    std::optional<double> currentX, currentY;
    std::optional<double> slotStartX, slotStartY, slotStopX, slotStopY;
    int nameTool = 0;
    bool allegroWarning = false, lineUnitsFound = false;
    double repeatingX = 0, repeatingY = 0;
    int repeat = 0;
    std::string lineUnits;
    int lineNum = 0;
    std::string eline;

    auto toolKey = [&]() { return currentTool.value_or(0); };
    auto addDrill = [&](double x, double y) { toolRef(toolKey()).drills.push_back({x, y}); };
    auto addSlot = [&](double x0, double y0, double x1, double y1) {
        toolRef(toolKey()).slots.push_back({{x0, y0}, {x1, y1}});
    };
    auto contains = [](const std::string& s, const char* what) { return s.find(what) != std::string::npos; };

    try {
        for (const std::string& raw : elines) {
            ++lineNum;
            sourceFile_ += raw;
            eline = strip(raw, " \r\n");
            std::smatch m;

            if (std::regex_search(eline, m, kDetectGcode)) {
                Logger::warning("This is GCODE mark: " + eline);
                return false;
            }
            if (std::regex_search(eline, m, kHBegin)) {
                inHeader = true;
                headerless = false;
                continue;
            }
            if (std::regex_search(eline, m, kAllegroHBegin)) {
                inHeader = true;
                allegroWarning = true;
                continue;
            }

            if (std::regex_search(eline, m, kComm)) {
                std::smatch tu;
                if (std::regex_search(eline, tu, kToolUnits)) {
                    if (!lineUnitsFound) {
                        lineUnitsFound = true;
                        lineUnits = tu[3].str();
                        convertUnits(lineUnits == "MILS" ? Units::Inches : Units::Millimeters);
                    }
                    if (tu[2].matched && tu[2].length() > 0) {
                        ++nameTool;
                        const double dia = std::stod(tu[2].str());
                        toolRef(nameTool).tooldia = (lineUnits == "MILS") ? dia / 1000 : dia;
                        continue;
                    }
                }
                std::smatch alt;
                if (std::regex_search(eline, alt, kAltium)) {
                    formatUpperMm_ = formatUpperIn_ = std::stoi(alt[1].str());
                    formatLowerMm_ = formatLowerIn_ = std::stoi(alt[2].str());
                    continue;
                }
                Logger::debug("Line ignored, it's a comment: " + eline);
            } else if (std::regex_search(eline, m, kHEnd)) {
                if (!inHeader || drillTools_.empty()) {
                    headerless = true;
                    convertUnits(excellonUnits_ == "INCH" ? Units::Inches : Units::Millimeters);
                }
                inHeader = false;
                if (allegroWarning) nameTool = 0;
                // Si no se declararon unidades, se deducen por el tamaño de las herramientas.
                if (!unitsFound_ && !drillTools_.empty()) {
                    int lower = 0, greater = 0;
                    std::set<double> seenLower, seenGreater;
                    for (const auto& [id, tool] : drillTools_) {
                        if (tool.tooldia <= 0.1) seenLower.insert(tool.tooldia);
                        else seenGreater.insert(tool.tooldia);
                    }
                    lower = static_cast<int>(seenLower.size());
                    greater = static_cast<int>(seenGreater.size());
                    setUnits(lower > greater ? Units::Inches : Units::Millimeters);
                }
                continue;
            }

            if (std::regex_search(eline, m, kMeas)) {
                const Units u = (m[1].str() == "1") ? Units::Millimeters : Units::Inches;
                setUnits(u);
                convertUnits(u);
                continue;
            }

            if (!inHeader) {
                if (std::regex_search(eline, m, kToolSel)) {
                    currentTool = std::stoi(m[1].str());
                    if (headerless) {
                        std::smatch hl;
                        if (std::regex_search(eline, hl, kToolSetHl)) {
                            const int name = std::stoi(hl[1].str());
                            double diam;
                            if (hl[2].matched && hl[2].length() > 0) {
                                diam = std::stod(hl[2].str());
                            } else {
                                diameterless_ = true;
                                Logger::warning("No tool diameter info for T" + std::to_string(*currentTool) +
                                                ": using fake diameters.");
                                // Igual que el original: compara con "MM", pero `excellon_units` vale "METRIC" o
                                // "INCH", así que en la práctica siempre se usa la rama de pulgadas.
                                if (excellonUnits_ == "MM") {
                                    diam = options_.toollessDiameter + (*currentTool - 1) / 100.0;
                                } else {
                                    diam = (options_.toollessDiameter + (*currentTool - 1) / 100.0) / 25.4;
                                }
                            }
                            toolRef(name).tooldia = diam;
                            toolRef(name).solidGeometry.clear();
                        }
                    }
                    continue;
                }
                if (allegroWarning) {
                    if (std::regex_search(eline, m, kAbsInc) || std::regex_search(eline, m, kStop)) {
                        ++nameTool;
                        currentTool = nameTool;
                        continue;
                    }
                }

                // ---- ranuras con G85
                if (std::regex_search(eline, m, kSlots)) {
                    const std::string startPart = m[1].str(), stopPart = m[2].str();
                    auto parsePair = [&](bool period, const std::string& part, std::optional<double>& x,
                                         std::optional<double>& y, std::optional<double>& curX,
                                         std::optional<double>& curY) -> bool {
                        const auto mc = matchCoords(part, period);
                        if (!mc) return false;
                        auto conv = [&](const std::optional<std::string>& g) -> std::optional<double> {
                            if (period) return std::stod(*g);
                            const auto v = parseNumber(*g);
                            return v;
                        };
                        if (mc->x) { x = conv(mc->x); curX = x; } else { x = curX; }
                        if (mc->y) { y = conv(mc->y); curY = y; } else { y = curY; }
                        return true;
                    };
                    std::optional<double> slotCurX, slotCurY;
                    for (const bool period : {false, true}) {
                        std::optional<double> sx, sy, ex, ey;
                        if (!parsePair(period, startPart, sx, sy, slotCurX, slotCurY)) continue;
                        if (!parsePair(period, stopPart, ex, ey, slotCurX, slotCurY)) continue;
                        if (!sx || !sy || !ex || !ey) break;
                        addSlot(*sx, *sy, *ex, *ey);
                        break;
                    }
                    continue;
                }

                // ---- coordenadas sin punto decimal
                if (const auto nc = matchCoords(eline, false)) {
                    std::smatch rep;
                    if (std::regex_search(eline, rep, kRepeat)) {
                        repeat = std::stoi(rep[1].str());
                        repeatingX = nc->x && !nc->x->empty() ? parseNumber(*nc->x).value_or(0.0) : 0.0;
                        repeatingY = nc->y && !nc->y->empty() ? parseNumber(*nc->y).value_or(0.0) : 0.0;
                        double coordX = currentX.value_or(0.0), coordY = currentY.value_or(0.0);
                        while (repeat > 0) {
                            if (repeatingX != 0) coordX += repeatingX;
                            if (repeatingY != 0) coordY += repeatingY;
                            addDrill(coordX, coordY);
                            --repeat;
                        }
                        currentX = coordX;
                        currentY = coordY;
                        continue;
                    }
                    std::optional<double> x = currentX, y = currentY;
                    if (nc->x) { x = parseNumber(*nc->x); currentX = x; }
                    if (nc->y) { y = parseNumber(*nc->y); currentY = y; }
                    if (!x || !y) continue;
                    if (contains(eline, "G00")) {
                        matchRoutingStart_ = true;
                        routingFlag_ = false;
                        slotStartX = x;
                        slotStartY = y;
                        continue;
                    }
                    if (!routingFlag_ && contains(eline, "G01")) {
                        matchRoutingStop_ = true;
                        routingFlag_ = true;
                        slotStopX = x;
                        slotStopY = y;
                        if (slotStartX && slotStartY) addSlot(*slotStartX, *slotStartY, *x, *y);
                        continue;
                    }
                    if (!matchRoutingStart_ && !matchRoutingStop_) {
                        addDrill(*x, *y);
                        continue;
                    }
                }

                // ---- coordenadas con punto decimal
                if (const auto pc = matchCoords(eline, true)) {
                    std::smatch rep;
                    if (std::regex_search(eline, rep, kRepeat)) repeat = std::stoi(rep[1].str());
                    std::optional<double> x, y;
                    if (pc->x) {
                        x = std::stod(*pc->x);
                        repeatingX = currentX.value_or(0.0);
                        currentX = x;
                    } else {
                        x = currentX;
                        repeatingX = 0;
                    }
                    if (pc->y) {
                        y = std::stod(*pc->y);
                        repeatingY = currentY.value_or(0.0);
                        currentY = y;
                    } else {
                        y = currentY;
                        repeatingY = 0;
                    }
                    if (!x || !y) continue;
                    if (contains(eline, "G00")) {
                        matchRoutingStart_ = true;
                        routingFlag_ = false;
                        slotStartX = x;
                        slotStartY = y;
                        continue;
                    }
                    if (!routingFlag_ && contains(eline, "G01")) {
                        matchRoutingStop_ = true;
                        routingFlag_ = true;
                        slotStopX = x;
                        slotStopY = y;
                        if (slotStartX && slotStartY) addSlot(*slotStartX, *slotStartY, *x, *y);
                        continue;
                    }
                    if (!matchRoutingStart_ && !matchRoutingStop_) {
                        if (repeat == 0) {
                            addDrill(*x, *y);
                        } else {
                            double coordX = *x, coordY = *y;
                            while (repeat > 0) {
                                if (repeatingX != 0) coordX = repeat * *x + repeatingX;
                                if (repeatingY != 0) coordY = repeat * *y + repeatingY;
                                addDrill(coordX, coordY);
                                --repeat;
                            }
                        }
                        repeatingX = repeatingY = 0;
                        continue;
                    }
                }
            }

            if (inHeader) {
                if (const auto ts = matchToolSet(eline)) {
                    if (!ts->hasDiameter) throw std::runtime_error("float() argument must be a string or a number, not 'NoneType'");
                    toolRef(ts->tool).tooldia = std::stod(ts->diameter);
                    toolRef(ts->tool).solidGeometry.clear();
                    continue;
                }
                std::smatch um;
                if (std::regex_search(eline, um, kUnits)) {
                    const Units u = (um[1].str() == "METRIC") ? Units::Millimeters : Units::Inches;
                    setUnits(u);
                    unitsFound_ = u;
                    zeros_ = um[2].matched ? um[2].str() : "";
                    if (um[3].matched && um[3].length() > 0) {
                        const std::string fmt = um[3].str();
                        const int upper = static_cast<int>(fmt.find('.'));
                        const int lower = static_cast<int>(fmt.size()) - upper - 1;
                        if (u == Units::Millimeters) { formatUpperMm_ = upper; formatLowerMm_ = lower; }
                        else { formatUpperIn_ = upper; formatLowerIn_ = lower; }
                    }
                    convertUnits(u);
                    continue;
                }
                if (contains(eline, "INCH")) {
                    convertUnits(Units::Inches);
                    unitsFound_ = Units::Inches;
                    continue;
                } else if (contains(eline, "METRIC")) {
                    convertUnits(Units::Millimeters);
                    unitsFound_ = Units::Millimeters;
                    continue;
                }
                std::smatch lz;
                if (std::regex_search(eline, lz, kLzTz)) {
                    zeros_ = lz[0].str();
                    continue;
                }
            }

            std::smatch um;
            if (std::regex_search(eline, um, kUnits)) {
                const Units u = (um[1].str() == "METRIC") ? Units::Millimeters : Units::Inches;
                setUnits(u);
                unitsFound_ = u;
                zeros_ = um[2].matched ? um[2].str() : "";
                if (um[3].matched && um[3].length() > 0) {
                    const std::string fmt = um[3].str();
                    const int upper = static_cast<int>(fmt.find('.'));
                    const int lower = static_cast<int>(fmt.size()) - upper - 1;
                    if (u == Units::Millimeters) { formatUpperMm_ = upper; formatLowerMm_ = lower; }
                    else { formatUpperIn_ = upper; formatLowerIn_ = lower; }
                }
                convertUnits(u);
                continue;
            }
        }
    } catch (const std::exception& e) {
        lastError_ = "Excellon Parser error. Parsing Failed. Line " + std::to_string(lineNum) + " [" + eline + "]: " + e.what();
        Logger::error(lastError_);
        return false;
    }
    return true;
}

void Excellon::addDrillTool(int id, double diameter, const std::vector<Point2D>& drills,
                            const std::vector<std::pair<Point2D, Point2D>>& slots) {
    ExcellonTool& t = toolRef(id);
    t.tooldia = diameter;
    t.drills = drills;
    t.slots = slots;
}

bool Excellon::createGeometry() {
    solidGeometry().clear();
    tools().clear();
    try {
        const int quad = options_.circleSteps / 4;
        for (auto& [id, tool] : drillTools_) {
            tool.solidGeometry.clear();
            for (const Point2D& d : tool.drills) {
                const Shape poly = Shape::point(d).buffer(tool.tooldia / 2.0, quad);
                tool.solidGeometry.push_back(poly);
                solidGeometry().push_back(poly);
            }
            for (const auto& [a, b] : tool.slots) {
                const Shape poly = Shape::lineString({a, b}).buffer(tool.tooldia / 2.0, quad);
                tool.solidGeometry.push_back(poly);
                solidGeometry().push_back(poly);
            }
            tools()[id].solidGeometry = tool.solidGeometry;
        }
    } catch (const std::exception& e) {
        lastError_ = std::string("Excellon geometry creation failed: ") + e.what();
        Logger::error(lastError_);
        return false;
    }
    return true;
}

double Excellon::convertUnits(Units objUnits) {
    double factor = 1.0;
    if (objUnits == units()) factor = 1.0;
    else if (objUnits == Units::Millimeters) factor = 25.4;
    else factor = 1 / 25.4;
    setUnits(objUnits);
    scale(factor, factor);
    for (auto& [id, tool] : drillTools_) tool.tooldia *= factor;
    // `scale()` ya recreó la geometría con los diámetros anteriores; se recrea con los nuevos.
    createGeometry();
    return factor;
}

void Excellon::transformAll(const AffineTransform& t) {
    for (auto& [id, tool] : drillTools_) {
        for (Point2D& d : tool.drills) d = t.apply(d);
        for (auto& slot : tool.slots) {
            slot.first = t.apply(slot.first);
            slot.second = t.apply(slot.second);
        }
    }
    createGeometry();
}

void Excellon::scale(double xfactor, double yfactor, const std::optional<Point2D>& point) {
    if (xfactor == 0 && yfactor == 0) return;
    transformAll(AffineTransform::scaling(xfactor, yfactor, point.value_or(Point2D{0, 0})));
}

void Excellon::offset(const Point2D& vect) {
    if (vect.x == 0 && vect.y == 0) return;
    transformAll(AffineTransform::translation(vect.x, vect.y));
}

void Excellon::mirror(Axis axis, const Point2D& point) { transformAll(AffineTransform::mirror(axis, point)); }

void Excellon::rotate(double angle, const Point2D& point) { transformAll(AffineTransform::rotation(angle, point)); }

void Excellon::skew(double angleX, double angleY, const Point2D& point) {
    if (angleX == 0 && angleY == 0) return;
    transformAll(AffineTransform::skew(angleX, angleY, point));
}

void Excellon::buffer(double distance, JoinStyle, bool factor) {
    if (distance == 0.0) return;
    for (auto& [id, t] : drillTools_) {
        if (factor) t.tooldia *= distance;
        else t.tooldia += distance;
    }
    createGeometry();
}

ExcellonTool& Excellon::toolRef(int id) {
    // Como el diccionario de Python: se recuerda el orden en que aparece cada herramienta.
    if (drillTools_.find(id) == drillTools_.end()) definitionOrder_.push_back(id);
    return drillTools_[id];
}

std::vector<int> Excellon::toolIdsInOrder() const {
    std::vector<int> out;
    for (int id : definitionOrder_)
        if (drillTools_.count(id) && std::find(out.begin(), out.end(), id) == out.end()) out.push_back(id);
    for (const auto& [id, t] : drillTools_)
        if (std::find(out.begin(), out.end(), id) == out.end()) out.push_back(id);
    return out;
}

Excellon Excellon::renumbered(const std::vector<int>& order) const {
    Excellon out(*this);
    out.drillTools_.clear();
    int id = 0;
    for (int old : order) {
        const auto it = drillTools_.find(old);
        if (it != drillTools_.end()) out.drillTools_[++id] = it->second;
    }
    out.definitionOrder_.clear();
    for (const auto& [k, t] : out.drillTools_) out.definitionOrder_.push_back(k);
    return out;
}

}  // namespace flatcam
