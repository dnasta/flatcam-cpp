#include "tools/Fiducials.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace flatcam {

namespace {

/// float('%.*f' % (n, v)); también vale para round(v, n) de Python (los dos redondean el valor binario exacto).
double roundPy(double v, int decimals) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return std::strtod(buf, nullptr);
}

/// str(int(max(ap_keys)) + 1), o "10" sin aperturas.
std::string nextApertureId(const std::map<std::string, Aperture>& aps) {
    if (aps.empty()) return "10";
    return std::to_string(std::stoi(aps.rbegin()->first) + 1);
}

Shape square(double xc, double yc, double side) {
    const double h = side / 2;
    return Shape::polygon({{xc - h, yc - h}, {xc + h, yc - h}, {xc + h, yc + h}, {xc - h, yc + h}, {xc - h, yc - h}});
}

}  // namespace

std::vector<Point2D> fiducialAutoPoints(const BoundingBox& b, double margin, const std::string& second, int decimals,
                                        int steps) {
    const BoundingBox r = Shape::box(b.xmin, b.ymin, b.xmax, b.ymax).buffer(margin, steps, JoinStyle::Mitre).bounds().value_or(b);
    std::vector<Point2D> pts{{roundPy(r.xmin, decimals), roundPy(r.ymin, decimals)},
                             {roundPy(r.xmax, decimals), roundPy(r.ymax, decimals)}};
    if (second == "up") pts.push_back({roundPy(r.xmin, decimals), roundPy(r.ymax, decimals)});
    else if (second == "down") pts.push_back({roundPy(r.xmax, decimals), roundPy(r.ymin, decimals)});
    return pts;
}

std::optional<FiducialsGerber> addFiducials(const Gerber& g, const std::vector<Point2D>& points, double size,
                                            FiducialType type, double lineThickness, int steps, int decimals) {
    FiducialsGerber r;
    r.apertures = g.apertures();
    const double radius = size / 2.0;
    // La apertura que ya tiene el tamaño (la primera, en el orden del archivo).
    auto findAperture = [&](auto pred) -> std::optional<std::string> {
        for (const std::string& id : g.apertureOrder()) {
            const auto it = g.apertures().find(id);
            if (it != g.apertures().end() && pred(it->second)) return id;
        }
        return std::nullopt;
    };
    auto target = [&](std::optional<std::string> found, const char* t, double apSize, double w, double h) {
        if (found) return *found;
        const std::string id = nextApertureId(r.apertures);
        Aperture ap;
        ap.type = t;
        ap.size = apSize;
        ap.width = w;
        ap.height = h;
        r.apertures[id] = ap;
        return id;
    };
    ShapeList added;  // lo que va a la geometría sólida
    ShapeList follow;  // geo_list
    std::vector<ApertureGeometry> elements;
    if (type == FiducialType::Circular) {
        for (const Point2D& p : points) {
            const Shape c = Shape::point(p).buffer(radius, steps);
            added.push_back(c);
            follow.push_back(c);
            elements.push_back({c, Shape::point(c.centroid()), std::nullopt});
        }
        const auto found = findAperture([&](const Aperture& a) { return a.type == "C" && a.size && *a.size == size; });
        Aperture& ap = r.apertures[target(found, "C", size, 0.0, 0.0)];
        ap.hasGeometry = true;
        ap.geometry.insert(ap.geometry.end(), elements.begin(), elements.end());
    } else if (type == FiducialType::Cross) {
        const double t2 = lineThickness / 2.0;
        ShapeList buffs;
        for (const Point2D& p : points) {
            const Shape hor = Shape::lineString({{p.x - radius + t2, p.y}, {p.x + radius - t2, p.y}});
            const Shape ver = Shape::lineString({{p.x, p.y - radius + t2}, {p.x, p.y + radius - t2}});
            follow.push_back(hor);
            follow.push_back(ver);
            for (const Shape& l : {hor, ver}) {
                const Shape b = l.buffer(t2, steps);
                buffs.push_back(b);
                elements.push_back({b, Shape::point(b.centroid()), std::nullopt});
            }
        }
        const auto found =
            findAperture([&](const Aperture& a) { return a.type == "C" && a.size && *a.size == lineThickness; });
        Aperture& ap = r.apertures[target(found, "C", lineThickness, 0.0, 0.0)];
        ap.hasGeometry = true;
        ap.geometry.insert(ap.geometry.end(), elements.begin(), elements.end());
        // MultiPolygon(geo_buff_list).buffer(0) y se recorre: con un solo polígono, TypeError.
        const Shape u = Shape::unaryUnion(buffs);
        if (u.type() == ShapeType::Polygon) return std::nullopt;
        for (const Shape& p : u.parts()) added.push_back(p);
    } else {
        for (const Point2D& p : points) {
            for (const Shape& s : {square(p.x - size / 4, p.y + size / 4, size / 2),
                                   square(p.x + size / 4, p.y - size / 4, size / 2)}) {
                added.push_back(s);
                follow.push_back(s);
                elements.push_back({s, Shape::point(s.centroid()), std::nullopt});
            }
        }
        const double diag = std::sqrt(size * size + size * size);
        const auto found = findAperture([&](const Aperture& a) {
            return a.type == "R" && a.size && roundPy(*a.size, decimals) == roundPy(diag, decimals);
        });
        Aperture& ap = r.apertures[target(found, "R", diag, size, size)];
        ap.hasGeometry = true;
        ap.geometry.insert(ap.geometry.end(), elements.begin(), elements.end());
    }
    ShapeList all = g.solidGeometry();
    all.insert(all.end(), added.begin(), added.end());
    const Shape u = Shape::unaryUnion(all);
    r.solid = u.parts().empty() ? ShapeList{u} : u.parts();
    r.follow = g.followGeometry();
    r.follow.insert(r.follow.end(), follow.begin(), follow.end());
    return r;
}

}  // namespace flatcam
