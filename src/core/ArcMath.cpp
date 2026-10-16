#include "ArcMath.h"

#include <algorithm>
#include <cmath>

namespace flatcam {

namespace {
constexpr double kTwoPi = 6.283185307179586476925286766559;
}

std::vector<Point2D> arc(const Point2D& center, double radius, double start, double stop,
                         ArcDirection direction, int stepsPerCircle) {
    const bool ccw = direction == ArcDirection::CounterClockwise;
    const double sign = ccw ? 1.0 : -1.0;

    if (ccw && stop <= start) stop += kTwoPi;
    if (!ccw && stop >= start) stop -= kTwoPi;

    const double angle = std::fabs(stop - start);
    const int steps = std::max(static_cast<int>(std::ceil(angle / kTwoPi * stepsPerCircle)), 2);
    const double deltaAngle = sign * angle * 1.0 / steps;

    std::vector<Point2D> points;
    points.reserve(static_cast<std::size_t>(steps) + 1);
    for (int i = 0; i <= steps; ++i) {
        const double theta = start + deltaAngle * i;
        points.push_back({center.x + radius * std::cos(theta), center.y + radius * std::sin(theta)});
    }
    return points;
}

std::vector<Point2D> arc2(const Point2D& p1, const Point2D& p2, const Point2D& center,
                          ArcDirection direction, int stepsPerCircle) {
    const double r = std::sqrt((center.x - p1.x) * (center.x - p1.x) + (center.y - p1.y) * (center.y - p1.y));
    const double start = std::atan2(p1.y - center.y, p1.x - center.x);
    const double stop = std::atan2(p2.y - center.y, p2.x - center.x);
    return arc(center, r, start, stop, direction, stepsPerCircle);
}

double arcAngle(double start, double stop, ArcDirection direction) {
    if (direction == ArcDirection::CounterClockwise && stop <= start) stop += kTwoPi;
    if (direction == ArcDirection::Clockwise && stop >= start) stop -= kTwoPi;
    return std::fabs(stop - start);
}

std::optional<Circle3P> threePointCircle(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    // Puntos medios.
    const Point2D a1 = (p1 + p2) / 2.0;
    const Point2D a2 = (p2 + p3) / 2.0;

    // Normales (mismas matrices de giro que usa camlib).
    const Point2D v = p2 - p1;
    const Point2D w = p3 - p2;
    const Point2D b1{v.y, -v.x};
    const Point2D b2{-w.y, w.x};

    // Resuelve [-b1 | b2] * T = a1 - a2 por la regla de Cramer.
    const double m00 = -b1.x, m01 = b2.x;
    const double m10 = -b1.y, m11 = b2.y;
    const double det = m00 * m11 - m01 * m10;
    if (det == 0.0) return std::nullopt;

    const Point2D rhs = a1 - a2;
    const double t0 = (rhs.x * m11 - m01 * rhs.y) / det;

    Circle3P c;
    c.center = a1 + b1 * t0;
    c.radius = (c.center - p1).norm();
    c.t = t0;
    return c;
}

double distance(const Point2D& a, const Point2D& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double distanceEuclidian(double x1, double y1, double x2, double y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

}  // namespace flatcam
