#include "AffineTransform.h"

#include <cmath>

namespace flatcam {

namespace {
constexpr double kPi = 3.14159265358979323846;
/// Umbral por debajo del cual seno/coseno se anulan (mismo valor que usa Shapely).
constexpr double kTrigEpsilon = 2.5e-16;
}  // namespace

AffineTransform AffineTransform::translation(double dx, double dy) {
    return {1.0, 0.0, 0.0, 1.0, dx, dy};
}

AffineTransform AffineTransform::scaling(double sx, double sy, const Point2D& o) {
    return {sx, 0.0, 0.0, sy, o.x - o.x * sx, o.y - o.y * sy};
}

AffineTransform AffineTransform::rotation(double angleDegrees, const Point2D& o) {
    const double angle = angleDegrees * kPi / 180.0;
    double c = std::cos(angle);
    double s = std::sin(angle);
    if (std::fabs(c) < kTrigEpsilon) c = 0.0;
    if (std::fabs(s) < kTrigEpsilon) s = 0.0;
    return {c, -s, s, c, o.x - o.x * c + o.y * s, o.y - o.x * s - o.y * c};
}

AffineTransform AffineTransform::skew(double xAngleDegrees, double yAngleDegrees, const Point2D& o) {
    const double tx = std::tan(xAngleDegrees * kPi / 180.0);
    const double ty = std::tan(yAngleDegrees * kPi / 180.0);
    return {1.0, tx, ty, 1.0, -o.y * tx, -o.x * ty};
}

AffineTransform AffineTransform::mirror(Axis axis, const Point2D& point) {
    return axis == Axis::X ? scaling(1.0, -1.0, point) : scaling(-1.0, 1.0, point);
}

AffineTransform AffineTransform::operator*(const AffineTransform& r) const {
    return {a_ * r.a_ + b_ * r.d_,
            a_ * r.b_ + b_ * r.e_,
            d_ * r.a_ + e_ * r.d_,
            d_ * r.b_ + e_ * r.e_,
            a_ * r.xoff_ + b_ * r.yoff_ + xoff_,
            d_ * r.xoff_ + e_ * r.yoff_ + yoff_};
}

}  // namespace flatcam
