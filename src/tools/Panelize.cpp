#include "tools/Panelize.h"

namespace flatcam {

PanelLayout panelLayout(const BoundingBox& b, const PanelizeSettings& s) {
    PanelLayout l;
    l.columns = s.columns;
    l.rows = s.rows;
    l.stepX = b.xmax - b.xmin + s.spacingColumns;
    l.stepY = b.ymax - b.ymin + s.spacingRows;
    if (s.constrain) {
        auto lenX = [&] { return (b.xmax - b.xmin) * l.columns + s.spacingColumns * (l.columns - 1); };
        auto lenY = [&] { return (b.ymax - b.ymin) * l.rows + s.spacingRows * (l.rows - 1); };
        if (lenX() > s.constrainX || lenY() > s.constrainY) {
            l.constrained = true;
            while (lenX() > s.constrainX) --l.columns;
            while (lenY() > s.constrainY) --l.rows;
        }
    }
    return l;
}

namespace {

/// Recorre las copias como el original: fila por fila (Y), y en cada fila columna por columna (X).
template <typename F>
void forEachCopy(const PanelLayout& l, F f) {
    double y = 0.0;
    for (int r = 0; r < l.rows; ++r) {
        double x = 0.0;
        for (int c = 0; c < l.columns; ++c) {
            f(x, y);
            x += l.stepX;
        }
        y += l.stepY;
    }
}

}  // namespace

std::unique_ptr<Excellon> panelizeExcellon(const Excellon& src, const PanelLayout& l) {
    std::map<int, std::vector<Point2D>> drills;
    std::map<int, std::vector<std::pair<Point2D, Point2D>>> slots;
    forEachCopy(l, [&](double x, double y) {
        for (const auto& [id, t] : src.drillTools()) {
            for (const Point2D& d : t.drills) drills[id].push_back({d.x + x, d.y + y});
            for (const auto& [a, b] : t.slots) slots[id].push_back({{a.x + x, a.y + y}, {b.x + x, b.y + y}});
        }
    });
    auto out = std::make_unique<Excellon>();
    for (const auto& [id, t] : src.drillTools()) out->addDrillTool(id, t.tooldia, drills[id], slots[id]);
    out->createGeometry();
    out->setZeros(src.zeros());
    out->setUnits(src.units());
    return out;
}

void panelizeGerber(const Gerber& src, const PanelLayout& l, ShapeList& solid, std::map<std::string, Aperture>& apertures) {
    solid.clear();
    apertures = src.apertures();
    for (auto& [id, ap] : apertures) ap.geometry.clear();
    forEachCopy(l, [&](double x, double y) {
        for (const Shape& g : src.solidGeometry()) solid.push_back(g.translate(x, y));
        for (const auto& [id, ap] : src.apertures()) {
            if (!ap.hasGeometry) continue;
            for (const ApertureGeometry& el : ap.geometry) {
                ApertureGeometry n;
                if (el.solid) n.solid = el.solid->translate(x, y);
                if (el.clear) n.clear = el.clear->translate(x, y);
                if (el.follow) n.follow = el.follow->translate(x, y);
                apertures[id].geometry.push_back(n);
            }
        }
    });
}

ShapeList panelizeShapes(const ShapeList& shapes, const PanelLayout& l) {
    ShapeList out;
    forEachCopy(l, [&](double x, double y) {
        for (const Shape& g : shapes) out.push_back(g.translate(x, y));
    });
    return out;
}

std::map<int, ShapeList> panelizeGeometryTools(const std::map<int, ShapeList>& tools, const PanelLayout& l,
                                               bool optimize) {
    std::map<int, ShapeList> out;
    for (const auto& [id, g] : tools) out[id];
    forEachCopy(l, [&](double x, double y) {
        for (const auto& [id, g] : tools)
            for (const Shape& s : g) out[id].push_back(s.translate(x, y));
    });
    // "Optimizar": para los cortes, las líneas se acercan entre sí y se unen.
    for (auto& [id, geo] : out) {
        ShapeList lines, other;
        for (const Shape& g : geo) {
            if (g.type() == ShapeType::LineString) lines.push_back(g);
            else if (g.type() == ShapeType::MultiLineString)
                for (const Shape& p : g.parts()) lines.push_back(p);
            else other.push_back(g);
        }
        if (optimize) {
            for (std::size_t i = 0; i < lines.size(); ++i) {
                const Shape line = lines[i];
                for (std::size_t j = i + 1; j < lines.size(); ++j) {
                    const Shape res = lines[j].snap(line, 1e-7);
                    if (!res.isEmpty()) lines[j] = res;
                }
            }
        }
        ShapeList result;
        result.push_back(Shape::unaryUnion({Shape::lineMerge(lines)}));  // [unary_union(linemerge(lines))]
        result.insert(result.end(), other.begin(), other.end());
        geo = result;
    }
    return out;
}

}  // namespace flatcam
