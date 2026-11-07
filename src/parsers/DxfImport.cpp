#include "parsers/DxfImport.h"

#include <cmath>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace flatcam {

namespace {

constexpr double kPi = 3.14159265358979323846;

struct DxfError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Vec3 {
    double x{0}, y{0}, z{0};
};

/// Una entidad: su tipo y sus pares (código, valor) en orden; las POLYLINE llevan sus VERTEX.
struct Entity {
    std::string type;
    std::vector<std::pair<int, std::string>> tags;
    std::vector<Entity> vertices;

    const std::string* first(int code) const {
        for (const auto& [c, v] : tags)
            if (c == code) return &v;
        return nullptr;
    }
    double num(int code, double def) const {
        const std::string* v = first(code);
        return v ? std::stod(*v) : def;
    }
    int integer(int code, int def) const {
        const std::string* v = first(code);
        return v ? std::stoi(*v) : def;
    }
    Vec3 point(int code, Vec3 def = {}) const {
        // x en el código, y en código + 10, z en código + 20 (los primeros que aparecen).
        Vec3 p = def;
        bool any = false;
        if (const auto* v = first(code)) {
            p.x = std::stod(*v);
            any = true;
        }
        if (const auto* v = first(code + 10)) p.y = std::stod(*v);
        if (const auto* v = first(code + 20)) p.z = std::stod(*v);
        if (!any && def.x == 0 && def.y == 0 && def.z == 0 && code != 210) throw DxfError("falta el punto " + std::to_string(code));
        return p;
    }
    Vec3 extrusion() const { return point(210, Vec3{0, 0, 1}); }
};

struct Block {
    Vec3 base;
    std::vector<Entity> entities;
};

struct Document {
    std::vector<Entity> modelspace;
    std::map<std::string, Block> blocks;
};

std::string trim(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

Document readDxf(const std::string& text) {
    std::vector<std::pair<int, std::string>> tags;
    std::istringstream in(text);
    std::string code, value;
    while (std::getline(in, code)) {
        if (!std::getline(in, value)) break;
        const std::string c = trim(code);
        if (c.empty()) continue;
        try {
            tags.push_back({std::stoi(c), trim(value)});
        } catch (const std::exception&) {
            throw DxfError("no es un DXF ASCII");
        }
    }
    Document doc;
    std::string section;
    std::size_t i = 0;
    auto readEntity = [&](std::size_t& k) {
        Entity e;
        e.type = tags[k].second;
        ++k;
        while (k < tags.size() && tags[k].first != 0) e.tags.push_back(tags[k++]);
        return e;
    };
    while (i < tags.size()) {
        const auto& [c, v] = tags[i];
        if (c != 0) {
            ++i;
            continue;
        }
        if (v == "SECTION") {
            section = i + 1 < tags.size() ? tags[i + 1].second : "";
            i += 2;
            continue;
        }
        if (v == "ENDSEC") {
            section.clear();
            ++i;
            continue;
        }
        if (v == "EOF") break;
        if (section == "ENTITIES" || section == "BLOCKS") {
            if (section == "BLOCKS" && v == "BLOCK") {
                Entity b = readEntity(i);
                Block blk;
                blk.base = b.point(10, Vec3{0, 0, 0});
                const std::string name = b.first(2) ? *b.first(2) : "";
                // Las entidades hasta ENDBLK.
                while (i < tags.size() && !(tags[i].first == 0 && tags[i].second == "ENDBLK")) {
                    Entity e = readEntity(i);
                    if (e.type == "POLYLINE") {
                        while (i < tags.size() && tags[i].second == "VERTEX") e.vertices.push_back(readEntity(i));
                        if (i < tags.size() && tags[i].second == "SEQEND") readEntity(i);
                    }
                    blk.entities.push_back(e);
                }
                if (i < tags.size()) readEntity(i);  // ENDBLK
                doc.blocks[name] = blk;
                continue;
            }
            Entity e = readEntity(i);
            if (e.type == "POLYLINE") {
                while (i < tags.size() && tags[i].second == "VERTEX") e.vertices.push_back(readEntity(i));
                if (i < tags.size() && tags[i].second == "SEQEND") readEntity(i);
            }
            // Espacio modelo: lo que no está marcado como espacio papel (67 = 1).
            if (section == "ENTITIES" && e.integer(67, 0) != 1) doc.modelspace.push_back(e);
            continue;
        }
        ++i;
    }
    return doc;
}

// ------------------------------------------------------------------------------------------------ OCS

Vec3 cross(const Vec3& a, const Vec3& b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
Vec3 normalized(const Vec3& v) {
    const double m = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return {v.x / m, v.y / m, v.z / m};
}

/// `ocs.to_wcs`: el algoritmo del eje arbitrario de AutoCAD.
Vec3 toWcs(const Vec3& extrusion, const Vec3& p) {
    const Vec3 az = normalized(extrusion);
    const Vec3 ax = normalized(std::abs(az.x) < 1.0 / 64 && std::abs(az.y) < 1.0 / 64 ? cross({0, 1, 0}, az)
                                                                                     : cross({0, 0, 1}, az));
    const Vec3 ay = normalized(cross(az, ax));
    return {ax.x * p.x + ay.x * p.y + az.x * p.z, ax.y * p.x + ay.y * p.y + az.y * p.z,
            ax.z * p.x + ay.z * p.y + az.z * p.z};
}

bool defaultExtrusion(const Vec3& e) { return e.x == 0 && e.y == 0 && e.z == 1; }

// ------------------------------------------------------------------------------------------------ ParseDXF_Spline

void rbasis(int c, double t, int npts, const std::vector<double>& x, const std::vector<double>& h, std::vector<double>& r) {
    const int nplusc = npts + c;
    std::vector<double> temp(static_cast<std::size_t>(nplusc + 1), 0.0);
    for (int i = 1; i < nplusc; ++i) temp[i] = (x[i] <= t && t < x[i + 1]) ? 1.0 : 0.0;
    for (int k = 2; k <= c; ++k) {
        for (int i = 1; i <= nplusc - k; ++i) {
            const double d = temp[i] != 0.0 ? ((t - x[i]) * temp[i]) / (x[i + k - 1] - x[i]) : 0.0;
            const double e = temp[i + 1] != 0.0 ? ((x[i + k] - t) * temp[i + 1]) / (x[i + k] - x[i + 1]) : 0.0;
            temp[i] = d + e;
        }
    }
    if (t >= x[nplusc]) temp[npts] = 1.0;
    double s = 0.0;
    for (int i = 1; i <= npts; ++i) s += temp[i] * h[i];
    for (int i = 1; i <= npts; ++i) r[i] = s != 0.0 ? (temp[i] * h[i]) / s : 0;
}

/// `_rbspline` / `_rbsplinu` con los nudos uniformes (`_knot` / `_knotu`: los de la DXF nunca tienen el largo que piden).
void rbspline(bool closed, int npts, int k, int p1, const std::vector<double>& b, std::vector<double>& p) {
    const int nplusc = npts + k;
    std::vector<double> x(static_cast<std::size_t>(nplusc + 1), 0.0);
    for (int i = 2; i <= nplusc; ++i) {
        if (closed) x[i] = static_cast<double>(i - 1);
        else x[i] = (k < i && i < npts + 2) ? x[i - 1] + 1.0 : x[i - 1];
    }
    const std::vector<double> h(static_cast<std::size_t>(npts + 1), 1.0);
    int icount = 0;
    double t = closed ? k - 1 : 0;
    const double step = closed ? (static_cast<double>(npts) - (k - 1)) / static_cast<double>(p1 - 1)
                               : x[nplusc] / static_cast<double>(p1 - 1);
    for (int i1 = 1; i1 <= p1; ++i1) {
        if (x[nplusc] - t < 5e-6) t = x[nplusc];
        std::vector<double> nbasis(static_cast<std::size_t>(npts + 1), 0.0);
        rbasis(k, t, npts, x, h, nbasis);
        for (int j = 1; j <= 3; ++j) {
            int jcount = j;
            p[icount + j] = 0.0;
            for (int i = 1; i <= npts; ++i) {
                p[icount + j] += nbasis[i] * b[jcount];
                jcount += 3;
            }
        }
        icount += 3;
        t += step;
    }
}

std::vector<Point2D> spline2Polyline(std::vector<Vec3> xyz, int degree, bool closed, int segments) {
    const Vec3 d{xyz.front().x - xyz.back().x, xyz.front().y - xyz.back().y, xyz.front().z - xyz.back().z};
    if (d.x * d.x + d.y * d.y + d.z * d.z < 1e-10) closed = false;
    if (closed) {
        const std::vector<Vec3> head(xyz.begin(), xyz.begin() + std::min<std::size_t>(degree, xyz.size()));
        xyz.insert(xyz.end(), head.begin(), head.end());
    }
    const int npts = static_cast<int>(xyz.size());
    const int k = degree + 1;
    if (degree < 1 || degree > 3 || npts < k) throw DxfError("zip argument #1 must support iteration");
    const int nseg = segments * npts;
    std::vector<double> b(static_cast<std::size_t>(npts * 3 + 1), 0.0), p(static_cast<std::size_t>(nseg * 3 + 1), 0.0);
    int i = 1;
    for (const Vec3& pt : xyz) {
        b[i] = pt.x;
        b[i + 1] = pt.y;
        b[i + 2] = pt.z;
        i += 3;
    }
    rbspline(closed, npts, k, nseg, b, p);
    std::vector<Point2D> out;
    for (int j = 1; j < 3 * nseg + 1; j += 3) out.push_back({p[j], p[j + 1]});
    return out;
}

// ------------------------------------------------------------------------------------------------ ParseDXF

double radians(double d) { return d * (kPi / 180.0); }

std::optional<Shape> entityGeo(const Document& doc, const Entity& e, ShapeList& many);

ShapeList getGeo(const Document& doc, const std::vector<Entity>& container) {
    ShapeList geo;
    for (const Entity& e : container) {
        ShapeList many;
        if (const auto g = entityGeo(doc, e, many)) geo.push_back(*g);
        geo.insert(geo.end(), many.begin(), many.end());
    }
    return geo;
}

ShapeList geoFromInsert(const Document& doc, const Entity& ins) {
    ShapeList out;
    const double phi = ins.num(50, 0);
    const Vec3 tr = ins.point(10, Vec3{0, 0, 0});
    const double sx = ins.num(41, 1), sy = ins.num(42, 1);
    const int rCount = ins.integer(71, 1), cCount = ins.integer(70, 1);
    const double rSpacing = ins.num(45, 0), cSpacing = ins.num(44, 0);
    const std::string name = ins.first(2) ? *ins.first(2) : "";
    const auto it = doc.blocks.find(name);
    if (it == doc.blocks.end()) throw DxfError("KeyError: bloque " + name);
    const Vec3 bc = it->second.base;
    for (Shape geo : getGeo(doc, it->second.entities)) {
        if (tr.x != 0 || tr.y != 0) geo = geo.translate(tr.x - bc.x, tr.y - bc.y);
        if (rCount > 1)
            for (int r = 0; r < rCount; ++r) out.push_back(geo.translate(tr.x + (r * rSpacing) - bc.x, 0));
        if (cCount > 1)
            for (int c = 0; c < cCount; ++c) out.push_back(geo.translate(0, tr.y + (c * cSpacing) - bc.y));
        if (sx != 1 || sy != 1) geo = geo.scale(sx, sy);  // alrededor del centro de la figura
        if (phi != 0) geo = geo.rotate(phi, Point2D{tr.x, tr.y});
        out.push_back(geo);
    }
    return out;
}

std::optional<Shape> entityGeo(const Document& doc, const Entity& e, ShapeList& many) {
    const std::string& t = e.type;
    if (t == "POINT") {
        const Vec3 p = e.point(10);
        return Shape::point({p.x, p.y}).buffer(0.01);
    }
    if (t == "LINE") {
        if (!e.first(10) || !e.first(11)) return std::nullopt;  // el original devuelve None si falla
        const Vec3 a = e.point(10), b = e.point(11);
        return Shape::lineString({{a.x, a.y}, {b.x, b.y}});
    }
    if (t == "CIRCLE") {
        const Vec3 ext = e.extrusion();
        Vec3 c = e.point(10, Vec3{0, 0, 0});
        if (!defaultExtrusion(ext)) c = toWcs(ext, c);
        return Shape::point({c.x, c.y}).buffer(e.num(40, 1), 25);  // int(100 / 4)
    }
    if (t == "ARC") {
        const Vec3 ext = e.extrusion();
        Vec3 c = e.point(10, Vec3{0, 0, 0});
        double start = e.num(50, 0), end = e.num(51, 360);
        bool ccw = true;
        if (!defaultExtrusion(ext)) {
            c = toWcs(ext, c);
            start += 180;
            end += 180;
            ccw = false;
        }
        const double r = e.num(40, 1);
        if (start > end) start -= 360;
        double angle = start;
        const double step = std::abs(end - start) / 100;
        std::vector<Point2D> pts;
        auto at = [&](double a) {
            const double aa = ccw ? a : -a;
            return Point2D{c.x + r * std::cos(radians(aa)), c.y + r * std::sin(radians(aa))};
        };
        while (angle <= end) {
            pts.push_back(at(angle));
            angle += std::abs(step);
        }
        if (angle != end) pts.push_back(at(end));
        if (pts.size() < 2) throw DxfError("LineStrings must have at least 2 coordinate tuples");
        return Shape::lineString(pts);
    }
    if (t == "ELLIPSE") {
        if (!defaultExtrusion(e.extrusion())) throw DxfError("ocs.to_wcs de un número");  // el original falla
        const Vec3 c = e.point(10, Vec3{0, 0, 0});
        double start = e.num(41, 0), end = e.num(42, 2 * kPi);
        const Vec3 major = e.point(11, Vec3{1, 0, 0});
        const double ratio = e.num(40, 1);
        if (start >= end) end += 2.0 * kPi;
        const int lineSeg = static_cast<int>((100 * (end - start)) / kPi);
        const double step = std::abs(end - start) / static_cast<double>(lineSeg);
        double angle = start;
        const double m = std::sqrt(major.x * major.x + major.y * major.y + major.z * major.z);
        const Vec3 majorDim{major.x / m, major.y / m, major.z / m};
        const Vec3 minorDim{ratio * majorDim.x, ratio * majorDim.y, ratio * majorDim.z};
        std::vector<Point2D> pts;
        for (int s = 0; s <= lineSeg; ++s) {
            const double vx = (majorDim.x + majorDim.y) * std::cos(angle);
            const double vy = (minorDim.x + minorDim.y) * std::sin(angle);
            pts.push_back({c.x + major.x * vx + major.y * vy, c.y + major.y * vx + major.x * vy});
            angle += step;
        }
        return Shape::lineString(pts);
    }
    if (t == "LWPOLYLINE") {
        std::vector<Point2D> pts;
        // Los vértices: cada código 10 con su 20 (el ancho y la curva se ignoran).
        for (std::size_t k = 0; k < e.tags.size(); ++k)
            if (e.tags[k].first == 10) {
                Point2D p{std::stod(e.tags[k].second), 0};
                if (k + 1 < e.tags.size() && e.tags[k + 1].first == 20) p.y = std::stod(e.tags[k + 1].second);
                pts.push_back(p);
            }
        if (e.integer(70, 0) & 1) pts.push_back(pts.front());
        if (pts.size() < 2) throw DxfError("LineStrings must have at least 2 coordinate tuples");
        return Shape::lineString(pts);
    }
    if (t == "POLYLINE") {
        std::vector<Point2D> pts;
        for (const Entity& v : e.vertices) {
            const Vec3 p = v.point(10, Vec3{0, 0, 0});
            pts.push_back({p.x, p.y});
        }
        if (e.integer(70, 0) & 1) pts.push_back(pts.front());
        if (pts.size() < 2) throw DxfError("LineStrings must have at least 2 coordinate tuples");
        return Shape::lineString(pts);
    }
    if (t == "SOLID" || t == "TRACE") return std::nullopt;  // dxfsolid2shapely devuelve None
    if (t == "SPLINE") {
        std::vector<Vec3> ctrl;
        for (std::size_t k = 0; k < e.tags.size(); ++k)
            if (e.tags[k].first == 10) {
                Vec3 p{std::stod(e.tags[k].second), 0, 0};
                if (k + 1 < e.tags.size() && e.tags[k + 1].first == 20) p.y = std::stod(e.tags[k + 1].second);
                if (k + 2 < e.tags.size() && e.tags[k + 2].first == 30) p.z = std::stod(e.tags[k + 2].second);
                ctrl.push_back(p);
            }
        if (ctrl.empty()) throw DxfError("spline sin puntos de control");
        return Shape::lineString(spline2Polyline(ctrl, e.integer(71, 3), (e.integer(70, 0) & 1) != 0, 20));
    }
    if (t == "INSERT") {
        many = geoFromInsert(doc, e);
        return std::nullopt;
    }
    return std::nullopt;  // "is not supported yet"
}

}  // namespace

DxfImportResult importDxfAsGeometry(const std::string& dxfText) {
    DxfImportResult r;
    try {
        const Document doc = readDxf(dxfText);
        const ShapeList geos = getGeo(doc, doc.modelspace);
        ShapeList lines;
        for (const Shape& g : geos) {
            if (g.type() == ShapeType::Polygon) r.geometry.push_back(g);
            else lines.push_back(g);
        }
        const Shape merged = Shape::lineMerge(lines);
        // `for l in merged_lines`: una sola LineString no es iterable en Shapely 1.8 y el original falla.
        if (merged.type() == ShapeType::LineString) throw DxfError("'LineString' object is not iterable");
        for (const Shape& p : merged.parts()) r.geometry.push_back(p);
        r.ok = true;
    } catch (const std::exception& e) {
        r.error = e.what();
    }
    return r;
}

DxfImportResult importDxfAsGerber(const std::string& dxfText) {
    DxfImportResult r;
    try {
        const Document doc = readDxf(dxfText);
        const ShapeList geos = getGeo(doc, doc.modelspace);
        for (const Shape& g : geos)
            if (g.type() == ShapeType::Polygon || g.type() == ShapeType::MultiPolygon)
                throw DxfError("linemerge de un polígono");  // linemerge(geos) del original
        const Shape merged = Shape::lineMerge(geos);
        r.geometry.push_back(Shape::fromWkt("POLYGON EMPTY"));  // el solid_geometry inicial del Gerber
        if (merged.parts().empty()) {
            if (!merged.isEmpty()) r.geometry.push_back(merged);
        } else {
            for (const Shape& p : merged.parts()) r.geometry.push_back(p);
        }
        r.ok = true;
    } catch (const std::exception& e) {
        r.error = e.what();
    }
    return r;
}

}  // namespace flatcam
