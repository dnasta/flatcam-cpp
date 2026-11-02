#include "cam/ExclusionAreas.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace flatcam {

namespace {

double dist(const Point2D& a, const Point2D& b) { return std::hypot(a.x - b.x, a.y - b.y); }

/// `nearest_point`: el más cercano; con empates, el último (`dist <= old_dist`).
std::optional<Point2D> nearestPoint(const Point2D& origin, const std::vector<Point2D>& pts) {
    std::optional<Point2D> best;
    double old = std::numeric_limits<double>::infinity();
    for (const Point2D& p : pts) {
        const double d = dist(origin, p);
        if (d <= old) {
            best = p;
            old = d;
        }
    }
    return best;
}

/// `farthest_point`: el más lejano; con empates, el último (`dist >= old_dist`).
std::optional<Point2D> farthestPoint(const Point2D& origin, const std::vector<Point2D>& pts) {
    std::optional<Point2D> best;
    double old = 0;
    for (const Point2D& p : pts) {
        const double d = dist(origin, p);
        if (d >= old) {
            best = p;
            old = d;
        }
    }
    return best;
}

double pathLength(const std::vector<Point2D>& path) {
    double l = 0;
    for (std::size_t i = 0; i + 1 < path.size(); ++i) l += dist(path[i], path[i + 1]);
    return l;
}

}  // namespace

std::vector<ExclusionTravel> ExclusionAreas::travelCoordinates(const std::vector<ExclusionArea>& areas,
                                                               const Point2D& start, const Point2D& end,
                                                               double tooldia, bool inches) {
    std::vector<ExclusionTravel> ret;
    Shape travelLine = Shape::lineString({start, end});
    const double buffered = tooldia / 2.0 + (inches ? 0.00393701 : 0.1);
    // Las áreas agrandadas, de la más cercana a la más lejana al punto de partida (orden estable).
    std::vector<std::pair<double, ExclusionArea>> tmp;
    for (const ExclusionArea& a : areas) {
        ExclusionArea b = a;
        b.shape = a.shape.buffer(buffered, 16, JoinStyle::Mitre);
        tmp.push_back({Shape::point(start).distance(b.shape), b});
    }
    std::stable_sort(tmp.begin(), tmp.end(), [](const auto& x, const auto& y) { return x.first < y.first; });

    for (const auto& [d, area] : tmp) {
        (void)d;
        const Shape outline = area.shape.exterior();
        if (!travelLine.intersects(outline)) continue;
        const Shape inter = travelLine.intersection(outline);
        if (inter.type() == ShapeType::Point) continue;  // solo toca el contorno
        if (inter.type() != ShapeType::MultiPoint) continue;  // tramo sobre el contorno: el original falla al iterarlo
        std::vector<Point2D> pts;
        for (const Shape& p : inter.parts()) pts.push_back(p.coordinates().front());
        const Point2D entry = *nearestPoint(start, pts);
        const Point2D exitPt = *farthestPoint(start, pts);
        std::vector<ExclusionTravel> pathCoords;
        if (area.strategy == "around") {
            std::vector<Point2D> vertex = outline.coordinates();
            vertex.pop_back();  // sin el punto repetido del cierre
            // Los vértices que se alcanzan en línea recta desde el inicio y desde el final sin cruzar el área.
            std::vector<Point2D> closeStart, closeEnd;
            for (const Point2D& v : vertex) {
                if (!Shape::lineString({start, v}).crosses(area.shape)) closeStart.push_back(v);
                if (!Shape::lineString({end, v}).crosses(area.shape)) closeEnd.push_back(v);
            }
            const auto entryV = nearestPoint(entry, closeStart);
            const auto exitV = nearestPoint(exitPt, closeEnd);
            if (!entryV || !exitV) continue;  // el original falla (`index(None)`)
            auto indexOf = [&](const Point2D& p) {
                return static_cast<std::size_t>(std::find(vertex.begin(), vertex.end(), p) - vertex.begin());
            };
            const std::size_t startIdx = indexOf(*entryV), endIdx = indexOf(*exitV);
            const std::size_t n = vertex.size();
            std::vector<Point2D> path1, path2;
            auto walkBack = [&](std::size_t from, std::size_t to) {
                std::vector<Point2D> p{vertex[from]};
                std::size_t idx = from;
                for (std::size_t k = 0; k < n; ++k) {
                    idx = idx > 0 ? idx - 1 : n - 1;
                    p.push_back(vertex[idx]);
                    if (idx == to) break;
                }
                return p;
            };
            if (endIdx > startIdx) {
                path1.assign(vertex.begin() + static_cast<long>(startIdx), vertex.begin() + static_cast<long>(endIdx) + 1);
                path2 = walkBack(startIdx, endIdx);
            } else {
                path1.assign(vertex.begin() + static_cast<long>(endIdx), vertex.begin() + static_cast<long>(startIdx) + 1);
                path2 = walkBack(endIdx, startIdx);
                std::reverse(path1.begin(), path1.end());
                std::reverse(path2.begin(), path2.end());
            }
            const std::vector<Point2D>& path = pathLength(path1) < pathLength(path2) ? path1 : path2;
            for (const Point2D& p : path) pathCoords.push_back({std::nullopt, p});
        } else {
            pathCoords.push_back({area.overZ, entry});
            pathCoords.push_back({std::nullopt, exitPt});
        }
        ret.insert(ret.end(), pathCoords.begin(), pathCoords.end());
        travelLine = Shape::lineString({pathCoords.back().point, end});
    }
    ret.push_back({std::nullopt, end});
    return ret;
}

}  // namespace flatcam
