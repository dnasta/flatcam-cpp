#include "gui/objects/FlatCAMObj.h"

#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

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
    if (s.isEmpty()) return;
    const ShapeType t = s.type();
    if (t == ShapeType::MultiPolygon || t == ShapeType::MultiLineString || t == ShapeType::GeometryCollection) {
        for (const Shape& p : s.parts()) addShape(p, out, fill);
        return;
    }
    auto addLine = [&out](const std::vector<Point2D>& c) {
        for (std::size_t i = 0; i + 1 < c.size(); ++i) {
            out.lines.insert(out.lines.end(), {float(c[i].x), float(c[i].y), float(c[i + 1].x), float(c[i + 1].y)});
        }
    };
    if (t == ShapeType::Polygon) {
        if (fill) {
            for (const Point2D& p : s.triangulate()) out.fill.insert(out.fill.end(), {float(p.x), float(p.y)});
        }
        addLine(s.exterior().coordinates());
        for (const Shape& h : s.interiors()) addLine(h.coordinates());
    } else if (t == ShapeType::LineString || t == ShapeType::LinearRing) {
        addLine(s.coordinates());
    }
    if (const auto b = s.bounds()) {
        const QRectF r(b->xmin, b->ymin, b->xmax - b->xmin, b->ymax - b->ymin);
        out.bounds = out.bounds.isNull() ? r : out.bounds.united(r);
    }
}

GerberObject::GerberObject(QString name, std::unique_ptr<Gerber> gerber)
    : FlatCAMObj(ObjectKind::Gerber, std::move(name)), gerber_(std::move(gerber)) {
    outlineColor = colorFromFlatCam("#006E20BF");  // gerber_plot_line
    fillColor = colorFromFlatCam("#BBF268BF");     // gerber_plot_fill
}

PlotLayerData GerberObject::plotData() const {
    PlotLayerData d;
    d.lineColor = outlineColor;
    d.fillColor = fillColor;
    // GerberObject.plot con options["solid"] verdadero: polígonos y líneas; los puntos no se dibujan.
    for (const Shape& g : gerber_->solidGeometry()) addShape(g, d);
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
    outlineColor = colorFromFlatCam("#750000BF");  // excellon_plot_line
    fillColor = colorFromFlatCam("#C40000BF");     // excellon_plot_fill
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
