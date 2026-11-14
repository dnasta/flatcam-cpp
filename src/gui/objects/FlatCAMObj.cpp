#include "gui/Preferences.h"
#include "gui/objects/FlatCAMObj.h"

#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

ShapeList FlatCAMObj::polygonsOf(const Shape& s) {
    if (s.type() == ShapeType::Polygon) return {s};
    ShapeList out;
    for (const Shape& p : s.parts())
        if (p.type() == ShapeType::Polygon) out.push_back(p);
    return out;
}

QColor colorFromFlatCam(const QString& fc) {
    // FlatCAM guarda los colores como #RRGGBBAA; QColor entiende #AARRGGBB.
    if (fc.size() == 9 && fc.startsWith('#')) return QColor("#" + fc.mid(7, 2) + fc.mid(1, 6));
    return QColor(fc);
}

FlatCAMObj::FlatCAMObj(ObjectKind kind, QString name) : kind_(kind), name_(std::move(name)) {
    static int next = 1;
    id_ = next++;
}

FlatCAMObj::~FlatCAMObj() = default;

void FlatCAMObj::addShape(const Shape& s, PlotLayerData& out, bool fill) {
    PlotGroup g;
    g.fill = std::move(out.fill);
    g.lines = std::move(out.lines);
    addShape(s, g, out.bounds, fill);
    out.fill = std::move(g.fill);
    out.lines = std::move(g.lines);
}

void FlatCAMObj::addShape(const Shape& s, PlotGroup& out, QRectF& bounds, bool fill) {
    if (s.isEmpty()) return;
    const ShapeType t = s.type();
    if (t == ShapeType::MultiPolygon || t == ShapeType::MultiLineString || t == ShapeType::GeometryCollection) {
        for (const Shape& p : s.parts()) addShape(p, out, bounds, fill);
        return;
    }
    auto addLine = [&out](const std::vector<Point2D>& c) {
        for (std::size_t i = 0; i + 1 < c.size(); ++i) {
            out.lines.insert(out.lines.end(), {float(c[i].x), float(c[i].y), float(c[i + 1].x), float(c[i + 1].y)});
        }
    };
    if (t == ShapeType::Polygon) {
        if (fill) {
            // Un polígono inválido (por ejemplo un trayecto SVG abierto que el original cierra) no se puede
            // triangular: se rellena su versión corregida (buffer(0)) o, si tampoco, solo se dibuja el contorno.
            std::vector<Point2D> tri;
            try {
                tri = s.triangulate();
            } catch (const std::exception&) {
                try {
                    for (const Shape& part : FlatCAMObj::polygonsOf(s.buffer(0))) {
                        const auto t2 = part.triangulate();
                        tri.insert(tri.end(), t2.begin(), t2.end());
                    }
                } catch (const std::exception&) {
                    tri.clear();
                }
            }
            for (const Point2D& p : tri) out.fill.insert(out.fill.end(), {float(p.x), float(p.y)});
        }
        addLine(s.exterior().coordinates());
        for (const Shape& h : s.interiors()) addLine(h.coordinates());
    } else if (t == ShapeType::LineString || t == ShapeType::LinearRing) {
        addLine(s.coordinates());
    }
    if (const auto b = s.bounds()) {
        const QRectF r(b->xmin, b->ymin, b->xmax - b->xmin, b->ymax - b->ymin);
        bounds = bounds.isNull() ? r : bounds.united(r);
    }
}

bool FlatCAMObj::mirror(Axis, const Point2D&) { return false; }
bool FlatCAMObj::rotate(double, const Point2D&) { return false; }
bool FlatCAMObj::skew(double, double, const Point2D&) { return false; }
bool FlatCAMObj::scale(double, double, const Point2D&) { return false; }
bool FlatCAMObj::offset(double, double) { return false; }
bool FlatCAMObj::buffer(double, JoinStyle, bool) { return false; }

bool GerberObject::rotate(double angle, const Point2D& p) {
    gerber_->rotate(angle, p);
    return true;
}
bool GerberObject::skew(double ax, double ay, const Point2D& p) {
    gerber_->skew(ax, ay, p);
    return true;
}
bool GerberObject::scale(double fx, double fy, const Point2D& p) {
    gerber_->scale(fx, fy, p);
    return true;
}
bool GerberObject::offset(double dx, double dy) {
    gerber_->offset({dx, dy});
    return true;
}
bool GerberObject::buffer(double distance, JoinStyle join, bool factor) {
    gerber_->buffer(distance, join, factor);
    return true;
}

bool ExcellonObject::rotate(double angle, const Point2D& p) {
    excellon_->rotate(angle, p);
    return true;
}
bool ExcellonObject::skew(double ax, double ay, const Point2D& p) {
    excellon_->skew(ax, ay, p);
    return true;
}
bool ExcellonObject::scale(double fx, double fy, const Point2D& p) {
    excellon_->scale(fx, fy, p);
    return true;
}
bool ExcellonObject::offset(double dx, double dy) {
    excellon_->offset({dx, dy});
    return true;
}
bool ExcellonObject::buffer(double distance, JoinStyle join, bool factor) {
    excellon_->buffer(distance, join, factor);
    return true;
}


ShapeList GerberObject::solidShapes() const { return gerber_->solidGeometry(); }

bool GerberObject::mirror(Axis axis, const Point2D& point) {
    gerber_->mirror(axis, point);
    return true;
}

ShapeList ExcellonObject::solidShapes() const { return excellon_->solidGeometry(); }

bool ExcellonObject::mirror(Axis axis, const Point2D& point) {
    excellon_->mirror(axis, point);
    return true;
}

GerberObject::GerberObject(QString name, std::unique_ptr<Gerber> gerber)
    : FlatCAMObj(ObjectKind::Gerber, std::move(name)), gerber_(std::move(gerber)) {
    outlineColor = colorFromFlatCam(defStr("gerber_plot_line"));
    fillColor = colorFromFlatCam(defStr("gerber_plot_fill"));
}

PlotLayerData GerberObject::plotData() const {
    PlotLayerData d;
    d.lineColor = outlineColor;
    d.fillColor = fillColor;
    // GerberObject.plot con options["solid"] verdadero: polígonos y líneas; los puntos no se dibujan.
    // Con "Seguir" (follow_cb), la geometría de seguimiento.
    for (const Shape& g : plotFollow_ ? gerber_->followGeometry() : gerber_->solidGeometry()) addShape(g, d);
    return d;
}

QRectF GerberObject::bounds() const {
    const BoundingBox b = gerber_->bounds();
    return QRectF(b.xmin, b.ymin, b.xmax - b.xmin, b.ymax - b.ymin);
}

QWidget* GerberObject::propertiesUi() {
    if (!ui_) ui_ = std::make_unique<GerberObjectUI>();
    static_cast<GerberObjectUI*>(ui_.get())->build(*this);
    return ui_.get();
}

ExcellonObject::ExcellonObject(QString name, std::unique_ptr<Excellon> excellon)
    : FlatCAMObj(ObjectKind::Excellon, std::move(name)), excellon_(std::move(excellon)) {
    outlineColor = colorFromFlatCam(defStr("excellon_plot_line"));
    fillColor = colorFromFlatCam(defStr("excellon_plot_fill"));
}

PlotLayerData ExcellonObject::plotData() const {
    PlotLayerData d;
    d.lineColor = outlineColor;
    d.fillColor = fillColor;
    // ExcellonObject.plot: la geometría sólida de cada herramienta (círculos de los agujeros y ranuras).
    for (const auto& [id, tool] : excellon_->drillTools()) {
        for (const Shape& g : tool.solidGeometry) addShape(g, d);
    }
    return d;
}

QRectF ExcellonObject::bounds() const {
    const BoundingBox b = excellon_->bounds();
    return QRectF(b.xmin, b.ymin, b.xmax - b.xmin, b.ymax - b.ymin);
}

}  // namespace flatcam::gui

namespace flatcam::gui {

QWidget* ExcellonObject::propertiesUi() {
    if (!ui_) ui_ = std::make_unique<ExcellonObjectUI>();
    static_cast<ExcellonObjectUI*>(ui_.get())->build(*this);
    return ui_.get();
}

}  // namespace flatcam::gui

namespace flatcam::gui {

double GerberObject::convertUnits(Units units) { return gerber_->convertUnits(units); }

double ExcellonObject::convertUnits(Units units) { return excellon_->convertUnits(units); }

}  // namespace flatcam::gui
