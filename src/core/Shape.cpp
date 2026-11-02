#include "Shape.h"

#include <geos_c.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "GeometryError.h"
#include "internal/GeosContext.h"

namespace flatcam {

namespace detail {

/// Acceso privilegiado a los detalles internos de Shape (solo para este archivo).
struct ShapeAccess {
    /// Toma posesión de una geometría GEOS y la envuelve en una Shape.
    static Shape make(GEOSGeometry* g);
    /// Geometría GEOS subyacente.
    static const GEOSGeometry* get(const Shape& s) { return s.geom_.get(); }
};

}  // namespace detail

namespace {

using detail::GeosContext;
using detail::ShapeAccess;

GEOSContextHandle_t H() { return GeosContext::handle(); }

/// Lanza GeometryError con el contexto y el último mensaje de GEOS.
[[noreturn]] void fail(const std::string& what) {
    std::string msg = what;
    const std::string& last = GeosContext::lastError();
    if (!last.empty()) msg += ": " + last;
    throw GeometryError(msg);
}

/// Libera una geometría GEOS.
struct GeomDeleter {
    void operator()(GEOSGeometry* g) const {
        if (g) GEOSGeom_destroy_r(H(), g);
    }
};

const GEOSGeometry* raw(const Shape& s) { return ShapeAccess::get(s); }

/// Envuelve el resultado de una función GEOS, lanzando si es nulo.
Shape wrap(GEOSGeometry* g, const char* op) {
    if (!g) fail(op);
    return ShapeAccess::make(g);
}

/// Resultado `char` de un predicado GEOS (2 = excepción).
bool predicate(char r, const char* op) {
    if (r == 2) fail(op);
    return r == 1;
}

/// Crea una secuencia de coordenadas; si @p closeRing la cierra repitiendo el primer vértice.
GEOSCoordSequence* makeSeq(const std::vector<Point2D>& pts, bool closeRing) {
    const bool needClose = closeRing && !pts.empty() && pts.front() != pts.back();
    const std::size_t n = pts.size() + (needClose ? 1 : 0);
    GEOSCoordSequence* seq = GEOSCoordSeq_create_r(H(), static_cast<unsigned int>(n), 2);
    if (!seq) fail("No se pudo crear la secuencia de coordenadas");
    for (std::size_t i = 0; i < pts.size(); ++i) {
        GEOSCoordSeq_setXY_r(H(), seq, static_cast<unsigned int>(i), pts[i].x, pts[i].y);
    }
    if (needClose) {
        GEOSCoordSeq_setXY_r(H(), seq, static_cast<unsigned int>(pts.size()), pts.front().x,
                             pts.front().y);
    }
    return seq;
}

GEOSGeometry* makeRing(const std::vector<Point2D>& pts) {
    // Igual que Shapely: un anillo cerrado necesita al menos 4 coordenadas (o ninguna, si es vacío).
    const bool needClose = !pts.empty() && pts.front() != pts.back();
    const std::size_t total = pts.size() + (needClose ? 1 : 0);
    if (total != 0 && total < 4) throw GeometryError("Un anillo requiere al menos 4 coordenadas");
    GEOSGeometry* ring = GEOSGeom_createLinearRing_r(H(), makeSeq(pts, true));
    if (!ring) fail("Anillo inválido");
    return ring;
}

std::vector<Point2D> readSeq(const GEOSCoordSequence* seq) {
    unsigned int n = 0;
    GEOSCoordSeq_getSize_r(H(), seq, &n);
    std::vector<Point2D> out;
    out.reserve(n);
    for (unsigned int i = 0; i < n; ++i) {
        double x = 0, y = 0;
        GEOSCoordSeq_getXY_r(H(), seq, i, &x, &y);
        out.push_back({x, y});
    }
    return out;
}

/// Datos que se pasan al callback de transformación de vértices.
struct TransformData {
    AffineTransform t;
};

int transformCallback(double* x, double* y, void* userdata) {
    const auto* d = static_cast<const TransformData*>(userdata);
    const Point2D p = d->t.apply({*x, *y});
    *x = p.x;
    *y = p.y;
    return 1;
}

}  // namespace

namespace detail {
Shape ShapeAccess::make(GEOSGeometry* g) {
    return Shape(std::shared_ptr<GEOSGeom_t>(g, GeomDeleter{}));
}
}  // namespace detail

// ------------------------------------------------------------------- fábricas

Shape::Shape() {
    *this = wrap(GEOSGeom_createEmptyCollection_r(H(), GEOS_GEOMETRYCOLLECTION),
                 "No se pudo crear la figura vacía");
}

Shape Shape::point(const Point2D& p) {
    return wrap(GEOSGeom_createPointFromXY_r(H(), p.x, p.y), "No se pudo crear el punto");
}

Shape Shape::lineString(const std::vector<Point2D>& points) {
    return wrap(GEOSGeom_createLineString_r(H(), makeSeq(points, false)),
                "Polilínea inválida");
}

Shape Shape::linearRing(const std::vector<Point2D>& points) {
    return wrap(makeRing(points), "Anillo inválido");
}

Shape Shape::polygon(const std::vector<Point2D>& exterior,
                     const std::vector<std::vector<Point2D>>& holes) {
    GEOSGeometry* shell = makeRing(exterior);
    std::vector<GEOSGeometry*> rings;
    rings.reserve(holes.size());
    for (const auto& h : holes) rings.push_back(makeRing(h));
    GEOSGeometry* poly = GEOSGeom_createPolygon_r(H(), shell, rings.empty() ? nullptr : rings.data(),
                                                  static_cast<unsigned int>(rings.size()));
    return wrap(poly, "Polígono inválido");
}

Shape Shape::box(double xmin, double ymin, double xmax, double ymax) {
    return polygon({{xmax, ymin}, {xmax, ymax}, {xmin, ymax}, {xmin, ymin}});
}

Shape Shape::circle(const Point2D& center, double radius, int resolution) {
    return point(center).buffer(radius, resolution);
}

Shape Shape::fromWkt(const std::string& wkt) {
    return wrap(GEOSGeomFromWKT_r(H(), wkt.c_str()), "WKT inválido");
}

Shape Shape::collection(const std::vector<Shape>& shapes) {
    int kind = GEOS_GEOMETRYCOLLECTION;
    if (!shapes.empty()) {
        const ShapeType first = shapes.front().type();
        bool same = true;
        for (const auto& s : shapes) same = same && (s.type() == first);
        if (same && first == ShapeType::Polygon) kind = GEOS_MULTIPOLYGON;
        else if (same && first == ShapeType::LineString) kind = GEOS_MULTILINESTRING;
        else if (same && first == ShapeType::Point) kind = GEOS_MULTIPOINT;
    }
    std::vector<GEOSGeometry*> kids;
    kids.reserve(shapes.size());
    for (const auto& s : shapes) kids.push_back(GEOSGeom_clone_r(H(), raw(s)));
    return wrap(GEOSGeom_createCollection_r(H(), kind, kids.empty() ? nullptr : kids.data(),
                                            static_cast<unsigned int>(kids.size())),
                "No se pudo crear la colección");
}

Shape Shape::unaryUnion(const std::vector<Shape>& shapes) {
    if (shapes.empty()) return Shape();
    std::vector<GEOSGeometry*> kids;
    kids.reserve(shapes.size());
    for (const auto& s : shapes) kids.push_back(GEOSGeom_clone_r(H(), raw(s)));
    GEOSGeometry* coll = GEOSGeom_createCollection_r(H(), GEOS_GEOMETRYCOLLECTION, kids.data(),
                                                     static_cast<unsigned int>(kids.size()));
    if (!coll) fail("No se pudo agrupar para la unión");
    GEOSGeometry* result = GEOSUnaryUnion_r(H(), coll);
    GEOSGeom_destroy_r(H(), coll);
    return wrap(result, "Falló la unión");
}

Shape Shape::lineMerge(const std::vector<Shape>& shapes) {
    if (shapes.empty()) return Shape();
    std::vector<GEOSGeometry*> kids;
    for (const auto& s : shapes) kids.push_back(GEOSGeom_clone_r(H(), raw(s)));
    GEOSGeometry* coll = GEOSGeom_createCollection_r(H(), GEOS_GEOMETRYCOLLECTION, kids.data(),
                                                     static_cast<unsigned int>(kids.size()));
    if (!coll) fail("No se pudo agrupar para unir líneas");
    GEOSGeometry* result = GEOSLineMerge_r(H(), coll);
    GEOSGeom_destroy_r(H(), coll);
    return wrap(result, "Falló la unión de líneas");
}

// ---------------------------------------------------------------- propiedades

ShapeType Shape::type() const {
    return static_cast<ShapeType>(GEOSGeomTypeId_r(H(), raw(*this)));
}

bool Shape::isEmpty() const { return predicate(GEOSisEmpty_r(H(), raw(*this)), "isEmpty"); }

bool Shape::isValid() const { return predicate(GEOSisValid_r(H(), raw(*this)), "isValid"); }

bool Shape::isRing() const { return predicate(GEOSisRing_r(H(), raw(*this)), "isRing"); }

namespace {

/// Punto a una distancia dada desde el inicio de una polilínea (se acota a [0, largo]).
Point2D interpolateAlong(const std::vector<Point2D>& pts, double distance) {
    if (distance <= 0) return pts.front();
    double acc = 0;
    for (std::size_t i = 0; i + 1 < pts.size(); ++i) {
        const double seg = std::hypot(pts[i + 1].x - pts[i].x, pts[i + 1].y - pts[i].y);
        if (acc + seg >= distance) {
            const double f = seg > 0 ? (distance - acc) / seg : 0.0;
            return {pts[i].x + (pts[i + 1].x - pts[i].x) * f, pts[i].y + (pts[i + 1].y - pts[i].y) * f};
        }
        acc += seg;
    }
    return pts.back();
}

}  // namespace

Shape Shape::interpolate(double distance) const { return point(interpolateAlong(coordinates(), distance)); }

Shape Shape::substring(double startDistance, double endDistance) const {
    const std::vector<Point2D> coords = coordinates();
    double length = 0;
    for (std::size_t i = 0; i + 1 < coords.size(); ++i) {
        length += std::hypot(coords[i + 1].x - coords[i].x, coords[i + 1].y - coords[i].y);
    }
    startDistance = std::min(std::max(startDistance, 0.0), length);
    endDistance = std::min(std::max(endDistance, 0.0), length);
    if (startDistance == endDistance) return point(interpolateAlong(coords, startDistance));

    const bool reversed = startDistance > endDistance;
    if (reversed) std::swap(startDistance, endDistance);
    std::vector<Point2D> vertices{interpolateAlong(coords, startDistance)};
    double current = 0;
    for (std::size_t i = 0; i + 1 < coords.size(); ++i) {
        if (startDistance < current && current < endDistance) vertices.push_back(coords[i]);
        else if (current >= endDistance) break;
        current += std::hypot(coords[i + 1].x - coords[i].x, coords[i + 1].y - coords[i].y);
    }
    vertices.push_back(interpolateAlong(coords, endDistance));
    if (reversed) std::reverse(vertices.begin(), vertices.end());
    return lineString(vertices);
}

double Shape::area() const {
    double a = 0;
    if (!GEOSArea_r(H(), raw(*this), &a)) fail("area");
    return a;
}

double Shape::length() const {
    double l = 0;
    if (!GEOSLength_r(H(), raw(*this), &l)) fail("length");
    return l;
}

std::optional<BoundingBox> Shape::bounds() const {
    if (isEmpty()) return std::nullopt;
    BoundingBox b;
    if (!GEOSGeom_getXMin_r(H(), raw(*this), &b.xmin) || !GEOSGeom_getYMin_r(H(), raw(*this), &b.ymin) ||
        !GEOSGeom_getXMax_r(H(), raw(*this), &b.xmax) || !GEOSGeom_getYMax_r(H(), raw(*this), &b.ymax)) {
        fail("bounds");
    }
    return b;
}

Point2D Shape::centroid() const {
    Shape c = wrap(GEOSGetCentroid_r(H(), raw(*this)), "centroid");
    return c.coordinates().front();
}

Point2D Shape::representativePoint() const {
    Shape c = wrap(GEOSPointOnSurface_r(H(), raw(*this)), "representativePoint");
    return c.coordinates().front();
}

std::size_t Shape::numParts() const {
    if (static_cast<int>(type()) < static_cast<int>(ShapeType::MultiPoint)) return 1;
    const int n = GEOSGetNumGeometries_r(H(), raw(*this));
    return n < 0 ? 0 : static_cast<std::size_t>(n);
}

std::vector<Shape> Shape::parts() const {
    if (static_cast<int>(type()) < static_cast<int>(ShapeType::MultiPoint)) return {*this};
    std::vector<Shape> out;
    const int n = GEOSGetNumGeometries_r(H(), raw(*this));
    for (int i = 0; i < n; ++i) {
        const GEOSGeometry* child = GEOSGetGeometryN_r(H(), raw(*this), i);
        out.push_back(wrap(GEOSGeom_clone_r(H(), child), "parts"));
    }
    return out;
}

Shape Shape::exterior() const {
    if (type() != ShapeType::Polygon) throw GeometryError("exterior(): la figura no es un polígono");
    const GEOSGeometry* ring = GEOSGetExteriorRing_r(H(), raw(*this));
    return wrap(GEOSGeom_clone_r(H(), ring), "exterior");
}

std::vector<Shape> Shape::interiors() const {
    std::vector<Shape> out;
    if (type() != ShapeType::Polygon) return out;
    const int n = GEOSGetNumInteriorRings_r(H(), raw(*this));
    for (int i = 0; i < n; ++i) {
        const GEOSGeometry* ring = GEOSGetInteriorRingN_r(H(), raw(*this), i);
        out.push_back(wrap(GEOSGeom_clone_r(H(), ring), "interiors"));
    }
    return out;
}

std::vector<Point2D> Shape::coordinates() const {
    const GEOSGeometry* g = raw(*this);
    if (type() == ShapeType::Polygon) g = GEOSGetExteriorRing_r(H(), g);
    const GEOSCoordSequence* seq = GEOSGeom_getCoordSeq_r(H(), g);
    if (!seq) throw GeometryError("coordinates(): la figura no tiene secuencia de coordenadas");
    return readSeq(seq);
}

// ------------------------------------------------------- operaciones booleanas

Shape Shape::unite(const Shape& o) const {
    // Atajo con figuras vacías: evita que algunas versiones de GEOS devuelvan miembros vacíos en el resultado.
    if (isEmpty()) return o;
    if (o.isEmpty()) return *this;
    return wrap(GEOSUnion_r(H(), raw(*this), raw(o)), "unite");
}
Shape Shape::difference(const Shape& o) const {
    if (isEmpty() || o.isEmpty()) return *this;
    return wrap(GEOSDifference_r(H(), raw(*this), raw(o)), "difference");
}
Shape Shape::intersection(const Shape& o) const {
    return wrap(GEOSIntersection_r(H(), raw(*this), raw(o)), "intersection");
}
Shape Shape::symmetricDifference(const Shape& o) const {
    return wrap(GEOSSymDifference_r(H(), raw(*this), raw(o)), "symmetricDifference");
}

// ------------------------------------------------------------------ predicados

bool Shape::contains(const Shape& o) const {
    return predicate(GEOSContains_r(H(), raw(*this), raw(o)), "contains");
}
bool Shape::within(const Shape& o) const {
    return predicate(GEOSWithin_r(H(), raw(*this), raw(o)), "within");
}
bool Shape::crosses(const Shape& o) const { return predicate(GEOSCrosses_r(H(), raw(*this), raw(o)), "crosses"); }

bool Shape::intersects(const Shape& o) const {
    return predicate(GEOSIntersects_r(H(), raw(*this), raw(o)), "intersects");
}
bool Shape::disjoint(const Shape& o) const {
    return predicate(GEOSDisjoint_r(H(), raw(*this), raw(o)), "disjoint");
}
double Shape::distance(const Shape& o) const {
    double d = 0;
    if (!GEOSDistance_r(H(), raw(*this), raw(o), &d)) fail("distance");
    return d;
}
bool Shape::equals(const Shape& o) const {
    return predicate(GEOSEquals_r(H(), raw(*this), raw(o)), "equals");
}
bool Shape::equalsExact(const Shape& o, double tolerance) const {
    return predicate(GEOSEqualsExact_r(H(), raw(*this), raw(o), tolerance), "equalsExact");
}

// --------------------------------------------------------------- construcciones

Shape Shape::buffer(double distance, int resolution, JoinStyle join, CapStyle cap,
                    double mitreLimit) const {
    GEOSBufferParams* params = GEOSBufferParams_create_r(H());
    if (!params) fail("buffer: no se pudieron crear los parámetros");
    GEOSBufferParams_setEndCapStyle_r(H(), params, static_cast<int>(cap));
    GEOSBufferParams_setJoinStyle_r(H(), params, static_cast<int>(join));
    GEOSBufferParams_setQuadrantSegments_r(H(), params, resolution);
    GEOSBufferParams_setMitreLimit_r(H(), params, mitreLimit);
    GEOSGeometry* result = GEOSBufferWithParams_r(H(), raw(*this), params, distance);
    GEOSBufferParams_destroy_r(H(), params);
    return wrap(result, "buffer");
}

Shape Shape::simplify(double tolerance, bool preserveTopology) const {
    GEOSGeometry* g = preserveTopology ? GEOSTopologyPreserveSimplify_r(H(), raw(*this), tolerance)
                                       : GEOSSimplify_r(H(), raw(*this), tolerance);
    return wrap(g, "simplify");
}

Shape Shape::parallelOffset(double distance, bool rightSide, int resolution, JoinStyle join, double mitreLimit) const {
    if (rightSide) distance = -distance;
    return wrap(GEOSOffsetCurve_r(H(), raw(*this), distance, resolution, static_cast<int>(join), mitreLimit),
                "parallelOffset");
}

Shape Shape::convexHull() const { return wrap(GEOSConvexHull_r(H(), raw(*this)), "convexHull"); }

std::vector<Point2D> Shape::triangulate() const {
    std::vector<Point2D> out;
    if (isEmpty()) return out;
    const ShapeType t = type();
    if (t == ShapeType::MultiPolygon || t == ShapeType::GeometryCollection) {
        for (const Shape& p : parts()) {
            const auto tri = p.triangulate();
            out.insert(out.end(), tri.begin(), tri.end());
        }
        return out;
    }
    if (t != ShapeType::Polygon) return out;
    const Shape tris = wrap(GEOSConstrainedDelaunayTriangulation_r(H(), raw(*this)), "triangulate");
    for (const Shape& tri : tris.parts()) {
        const auto c = tri.exterior().coordinates();
        if (c.size() >= 4) out.insert(out.end(), c.begin(), c.begin() + 3);
    }
    return out;
}

Shape Shape::makeValid() const { return wrap(GEOSMakeValid_r(H(), raw(*this)), "makeValid"); }

Shape Shape::asLineString() const {
    if (type() != ShapeType::LinearRing) return *this;
    const GEOSCoordSequence* seq = GEOSGeom_getCoordSeq_r(H(), raw(*this));
    GEOSCoordSequence* copy = GEOSCoordSeq_clone_r(H(), seq);
    return wrap(GEOSGeom_createLineString_r(H(), copy), "asLineString");
}

// --------------------------------------------------------------- transformaciones

Shape Shape::transform(const AffineTransform& t) const {
    TransformData data{t};
    return wrap(GEOSGeom_transformXY_r(H(), raw(*this), transformCallback, &data), "transform");
}

namespace {
/// Origen de una transformación: el indicado o el centro de la caja delimitadora.
Point2D resolveOrigin(const Shape& s, const std::optional<Point2D>& origin) {
    if (origin) return *origin;
    const auto b = s.bounds();
    return b ? b->center() : Point2D{};
}
}  // namespace

Shape Shape::scale(double sx, double sy, const std::optional<Point2D>& origin) const {
    return transform(AffineTransform::scaling(sx, sy, resolveOrigin(*this, origin)));
}

Shape Shape::translate(double dx, double dy) const {
    return transform(AffineTransform::translation(dx, dy));
}

Shape Shape::rotate(double angleDegrees, const std::optional<Point2D>& origin) const {
    return transform(AffineTransform::rotation(angleDegrees, resolveOrigin(*this, origin)));
}

Shape Shape::skew(double xAngleDegrees, double yAngleDegrees,
                  const std::optional<Point2D>& origin) const {
    return transform(AffineTransform::skew(xAngleDegrees, yAngleDegrees, resolveOrigin(*this, origin)));
}

// ------------------------------------------------------------------------ salida

std::string Shape::toWkt(int precision) const {
    GEOSWKTWriter* writer = GEOSWKTWriter_create_r(H());
    GEOSWKTWriter_setTrim_r(H(), writer, 1);
    if (precision >= 0) GEOSWKTWriter_setRoundingPrecision_r(H(), writer, precision);
    char* text = GEOSWKTWriter_write_r(H(), writer, raw(*this));
    GEOSWKTWriter_destroy_r(H(), writer);
    if (!text) fail("toWkt");
    std::string out(text);
    GEOSFree_r(H(), text);
    return out;
}

struct PreparedShape::Impl {
    Shape shape;  // mantiene viva la figura original, que GEOS referencia
    const GEOSPreparedGeometry* prepared{nullptr};
};

PreparedShape::PreparedShape(const Shape& shape) : impl_(std::make_unique<Impl>()) {
    impl_->shape = shape;
    impl_->prepared = GEOSPrepare_r(H(), raw(impl_->shape));
    if (!impl_->prepared) throw GeometryError("No se pudo preparar la figura");
}

PreparedShape::~PreparedShape() {
    if (impl_ && impl_->prepared) GEOSPreparedGeom_destroy_r(H(), impl_->prepared);
}

bool PreparedShape::contains(const Shape& other) const {
    return predicate(GEOSPreparedContains_r(H(), impl_->prepared, raw(other)), "contains (preparado)");
}

bool PreparedShape::intersects(const Shape& other) const {
    return predicate(GEOSPreparedIntersects_r(H(), impl_->prepared, raw(other)), "intersects (preparado)");
}

}  // namespace flatcam
