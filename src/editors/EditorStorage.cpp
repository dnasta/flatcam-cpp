#include "editors/EditorStorage.h"

#include <algorithm>
#include <limits>

namespace flatcam {

std::vector<Point2D> EditorStorage::points(const Shape& g, std::optional<double> tolerance) {
    std::vector<Point2D> out;
    if (g.isEmpty()) return out;
    const ShapeType t = g.type();
    if (t == ShapeType::MultiPoint || t == ShapeType::MultiLineString || t == ShapeType::MultiPolygon ||
        t == ShapeType::GeometryCollection) {
        for (const Shape& p : g.parts()) {
            const auto v = points(p, tolerance);
            out.insert(out.end(), v.begin(), v.end());
        }
        return out;
    }
    if (t == ShapeType::Polygon) {
        out = points(g.exterior(), tolerance);
        for (const Shape& i : g.interiors()) {
            const auto v = points(i, tolerance);
            out.insert(out.end(), v.begin(), v.end());
        }
        return out;
    }
    return tolerance ? g.simplify(*tolerance).coordinates() : g.coordinates();
}

void EditorStorage::insert(const EditorShapePtr& s) { entries_.push_back({s, points(s->geo, tolerance)}); }

void EditorStorage::remove(const EditorShapePtr& s) {
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(), [&](const Entry& e) { return e.shape == s; }),
                   entries_.end());
}

std::vector<EditorShapePtr> EditorStorage::objects() const {
    std::vector<EditorShapePtr> out;
    out.reserve(entries_.size());
    for (const Entry& e : entries_) out.push_back(e.shape);
    return out;
}

std::size_t EditorStorage::size() const { return entries_.size(); }

std::optional<std::pair<Point2D, EditorShapePtr>> EditorStorage::nearest(const Point2D& p) const {
    double best = std::numeric_limits<double>::infinity();
    std::optional<std::pair<Point2D, EditorShapePtr>> out;
    for (const Entry& e : entries_)
        for (const Point2D& q : e.points) {
            const double d = (q.x - p.x) * (q.x - p.x) + (q.y - p.y) * (q.y - p.y);
            if (d < best) {
                best = d;
                out = std::make_pair(q, e.shape);
            }
        }
    return out;
}

}  // namespace flatcam
