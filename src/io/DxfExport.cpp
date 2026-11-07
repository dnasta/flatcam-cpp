#include "io/DxfExport.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <functional>
#include <random>
#include <vector>

#include "cam/PyNumber.h"
#include "io/generated/DxfR2010Template.h"

namespace flatcam {

namespace {

/// Un GUID con el formato de `uuid1()` en mayúsculas, como el que pone ezdxf.
std::string newGuid() {
    static std::mt19937_64 rng{std::random_device{}()};
    const unsigned long long a = rng(), b = rng();
    char buf[40];
    std::snprintf(buf, sizeof buf, "%08llX-%04llX-%04llX-%04llX-%012llX", a >> 32, (a >> 16) & 0xFFFF,
                  0x1000 | (a & 0x0FFF), 0x8000 | ((b >> 48) & 0x3FFF), b & 0xFFFFFFFFFFFFULL);
    return buf;
}

/// Un código de grupo de DXF alineado a la derecha en tres columnas, como lo escribe ezdxf.
std::string code(int c) {
    char buf[8];
    std::snprintf(buf, sizeof buf, "%3d", c);
    return buf;
}

std::string hex(unsigned long long v) {
    char buf[20];
    std::snprintf(buf, sizeof buf, "%llX", v);
    return buf;
}

void lwpolyline(std::string& out, unsigned long long handle, const std::vector<Point2D>& pts) {
    out += code(0) + "\nLWPOLYLINE\n" + code(5) + "\n" + hex(handle) + "\n" + code(330) + "\n17\n" + code(100) +
           "\nAcDbEntity\n" + code(8) + "\n0\n" + code(100) + "\nAcDbPolyline\n" + code(90) + "\n" +
           std::to_string(pts.size()) + "\n" + code(70) + "\n0\n";
    for (const Point2D& p : pts) out += code(10) + "\n" + pyFloatRepr(p.x) + "\n" + code(20) + "\n" + pyFloatRepr(p.y) + "\n";
}

/// `ezdxf.tools.juliandate(datetime.now())`, con la hora local a segundos.
double julianNow() {
    const std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    const double y = (tm.tm_year + 1900) + (static_cast<double>(tm.tm_mon + 1) - 2.85) / 12.;
    const double A = std::floor(367. * y) - 1.75 * std::floor(y) + tm.tm_mday;
    const double B = std::floor(A) - 0.75 * std::floor(y / 100.);
    const double jd = std::floor(B) + 1721115.;
    const double seconds = tm.tm_hour * 3600. + tm.tm_min * 60. + tm.tm_sec;
    return jd + seconds / 86400.;
}

}  // namespace

std::string exportDxf(const ShapeList& shapes) {
    // g2dxf: contorno y agujeros de cada polígono; cada línea o anillo.
    std::vector<std::vector<Point2D>> polylines;
    std::function<void(const Shape&)> add = [&](const Shape& g) {
        switch (g.type()) {
            case ShapeType::Polygon:
                polylines.push_back(g.exterior().coordinates());
                for (const Shape& r : g.interiors()) polylines.push_back(r.coordinates());
                break;
            case ShapeType::MultiPolygon:
            case ShapeType::MultiLineString:
                for (const Shape& p : g.parts()) add(p);
                break;
            case ShapeType::LineString:
            case ShapeType::LinearRing: polylines.push_back(g.coordinates()); break;
            default: break;
        }
    };
    for (const Shape& s : shapes) add(s);
    const unsigned long long first = std::stoull(dxf_template::kFirstHandle, nullptr, 16);
    std::string entities;
    for (std::size_t i = 0; i < polylines.size(); ++i) lwpolyline(entities, first + i, polylines[i]);
    const unsigned long long n = polylines.size();
    std::string out;
    bool skipNewline = false;
    for (std::size_t i = 0; i < dxf_template::kParts.size(); ++i) {
        std::string part = dxf_template::kParts[i];
        if (skipNewline && !part.empty() && part[0] == '\n') part.erase(0, 1);
        skipNewline = false;
        out += part;
        if (i >= dxf_template::kMarks.size()) break;
        const std::string& m = dxf_template::kMarks[i];
        if (m == "HANDSEED") out += hex(first + n + 1);
        else if (m == "DATE0" || m == "DATE1") out += pyFloatRepr(julianNow());  // $TDCREATE y $TDUPDATE
        else if (m == "APPID") out += hex(first + n);  // el registro APPID que ezdxf crea al guardar
        else if (m == "GUID0" || m == "GUID1") out += newGuid();
        else if (m == "ENTITIES") {
            out += entities;
            skipNewline = true;
        }
    }
    return out;
}

}  // namespace flatcam
