#include "gui/PlotCanvas.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QWheelEvent>
#include <cmath>

namespace flatcam::gui {

namespace {

// VisPyCanvas: margen de la grilla 10, eje Y de 55 px de ancho y eje X de 30 px de alto.
constexpr double kMargin = 10.0;
constexpr double kYAxisWidth = 55.0;
constexpr double kXAxisHeight = 30.0;
// Camera: zoom_factor de vispy (0.007) elevado a 30 por cada paso de la rueda; límites del tamaño de la escena.
constexpr double kZoomPerStep = 1.2271;  // 1.007^30
constexpr double kMinSceneSize = 0.01;
constexpr double kMaxSceneSize = 10000.0;
// PlotCanvas (tema claro): línea de ejes, color del HUD y tamaño de letra.
const QColor kAxisLineColor = QColor::fromRgbF(0.70f, 0.3f, 0.3f, 0.8f);
const QColor kCursorColor = QColor("#FF0000");  // global_cursor_color
const QColor kHudRectColor = QColor(0x00, 0x00, 0xFF, 0x10);
constexpr int kHudFontSize = 8;     // hud_font_size
constexpr int kAxisFontSize = 8;    // axis_font_size

/// Paso "redondo" (1, 2 o 5 por una potencia de 10) más cercano por arriba a `raw`.
double niceStep(double raw) {
    const double p = std::pow(10.0, std::floor(std::log10(raw)));
    const double m = raw / p;
    if (m <= 1) return p;
    if (m <= 2) return 2 * p;
    if (m <= 5) return 5 * p;
    return 10 * p;
}

QString tickLabel(double v, double step) {
    if (std::fabs(v) < step * 1e-6) v = 0;
    const int decimals = step >= 1 ? 0 : static_cast<int>(std::ceil(-std::log10(step)));
    return QString::number(v, 'f', decimals);
}

}  // namespace

PlotCanvas::PlotCanvas(QWidget* parent) : QOpenGLWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

QRectF PlotCanvas::viewRect() const {
    return QRectF(kMargin + kYAxisWidth, kMargin, width() - 2 * kMargin - kYAxisWidth,
                  height() - 2 * kMargin - kXAxisHeight);
}

QPointF PlotCanvas::toWorld(const QPointF& px) const {
    const QRectF vr = viewRect();
    return {center_.x() + (px.x() - vr.center().x()) / scale_, center_.y() - (px.y() - vr.center().y()) / scale_};
}

QPointF PlotCanvas::toPixel(const QPointF& w) const {
    const QRectF vr = viewRect();
    return {vr.center().x() + (w.x() - center_.x()) * scale_, vr.center().y() - (w.y() - center_.y()) * scale_};
}

void PlotCanvas::fitTo(const QRectF& rect) {
    const QRectF vr = viewRect();
    if (vr.width() <= 0 || vr.height() <= 0 || rect.width() <= 0 || rect.height() <= 0) return;
    center_ = rect.center();
    scale_ = std::min(vr.width() / rect.width(), vr.height() / rect.height());
    update();
}

void PlotCanvas::zoom(double factor, const QPointF& c) {
    const double sceneWidth = viewRect().width() / scale_;
    const bool zoomIn = factor > 1;
    // limited_zoom: no achicar por debajo de 0,01 mm ni agrandar por encima de 10.000 mm.
    if ((!zoomIn && sceneWidth >= kMaxSceneSize) || (zoomIn && sceneWidth <= kMinSceneSize)) return;
    center_ = c + (center_ - c) / factor;
    scale_ *= factor;
    update();
}

void PlotCanvas::setHudEnabled(bool on) {
    hudEnabled_ = on;
    update();
}

void PlotCanvas::setAxisEnabled(bool on) {
    axisEnabled_ = on;
    update();
}

void PlotCanvas::setGridEnabled(bool on) {
    gridEnabled_ = on;
    update();
}

void PlotCanvas::initializeGL() {}

void PlotCanvas::resizeGL(int, int) {
    if (firstResize_ && viewRect().width() > 0) {
        firstResize_ = false;
        fitTo(QRectF(-25, -25, 150, 150));  // Camera(aspect=1, rect=(-25, -25, 150, 150))
    }
}

double PlotCanvas::tickStep() const { return niceStep(80.0 / scale_); }

void PlotCanvas::paintGL() {
    QPainter p(this);
    const QRectF vr = viewRect();
    // Fondo: color de la ventana alrededor y blanco dentro de la vista, con borde negro.
    p.fillRect(rect(), QApplication::palette().color(QPalette::Window));
    p.fillRect(vr, Qt::white);
    p.save();
    p.setClipRect(vr);
    p.setRenderHint(QPainter::Antialiasing, true);
    if (gridEnabled_) drawGrid(p, vr);
    if (axisEnabled_) {
        const QPointF o = toPixel({0, 0});
        p.setPen(QPen(kAxisLineColor, 2));
        p.drawLine(QPointF(o.x(), vr.top()), QPointF(o.x(), vr.bottom()));
        p.drawLine(QPointF(vr.left(), o.y()), QPointF(vr.right(), o.y()));
    }
    if (cursorInside_) {
        // Cursor de FlatCAM: una cruz chica en el punto, del color global_cursor_color.
        const QPointF c = toPixel(cursorWorld_);
        p.setPen(QPen(kCursorColor, 1.5));
        p.drawLine(c + QPointF(-10, 0), c + QPointF(10, 0));
        p.drawLine(c + QPointF(0, -10), c + QPointF(0, 10));
    }
    if (hudEnabled_) drawHud(p, vr);
    p.restore();
    p.setPen(QPen(Qt::black, 1));
    p.drawRect(vr);
    drawAxesLabels(p, vr);
}

void PlotCanvas::drawGrid(QPainter& p, const QRectF& vr) const {
    // GridLines de vispy: líneas finas en varias escalas; las más chicas se desvanecen al alejarse.
    const double major = tickStep();
    const double minor = major / 10.0;
    const QPointF tl = toWorld(vr.topLeft()), br = toWorld(vr.bottomRight());
    auto lines = [&](double step, const QColor& color) {
        if (step * scale_ < 6) return;
        p.setPen(QPen(color, 1));
        for (double x = std::ceil(tl.x() / step) * step; x <= br.x(); x += step) {
            const double px = toPixel({x, 0}).x();
            p.drawLine(QPointF(px, vr.top()), QPointF(px, vr.bottom()));
        }
        for (double y = std::ceil(br.y() / step) * step; y <= tl.y(); y += step) {
            const double py = toPixel({0, y}).y();
            p.drawLine(QPointF(vr.left(), py), QPointF(vr.right(), py));
        }
    };
    lines(minor, QColor(105, 105, 105, 22));  // dimgray tenue
    lines(major, QColor(105, 105, 105, 60));
}

void PlotCanvas::drawAxesLabels(QPainter& p, const QRectF& vr) const {
    QFont f = font();
    f.setPointSize(kAxisFontSize);
    p.setFont(f);
    p.setPen(QPen(Qt::black, 1));
    const double step = tickStep();
    const QPointF tl = toWorld(vr.topLeft()), br = toWorld(vr.bottomRight());
    const QFontMetricsF fm(f);
    for (double x = std::ceil(tl.x() / step) * step; x <= br.x(); x += step) {
        const double px = toPixel({x, 0}).x();
        p.drawLine(QPointF(px, vr.bottom()), QPointF(px, vr.bottom() + 5));
        const QString s = tickLabel(x, step);
        p.drawText(QPointF(px - fm.horizontalAdvance(s) / 2, vr.bottom() + 6 + fm.ascent()), s);
    }
    for (double y = std::ceil(br.y() / step) * step; y <= tl.y(); y += step) {
        const double py = toPixel({0, y}).y();
        p.drawLine(QPointF(vr.left() - 5, py), QPointF(vr.left(), py));
        const QString s = tickLabel(y, step);
        p.drawText(QPointF(vr.left() - 8 - fm.horizontalAdvance(s), py + fm.ascent() / 2 - 1), s);
    }
}

void PlotCanvas::drawHud(QPainter& p, const QRectF& vr) const {
    // PlotCanvas: alto = tamaño de letra * 11, ancho = el doble, a 5 px de la esquina de la vista.
    const double h = kHudFontSize * 11, w = h * 2;
    const QRectF r(vr.left() + 5, vr.top() + 5, w, h);
    QPainterPath path;
    path.addRoundedRect(r, 5, 5);
    p.fillPath(path, kHudRectColor);
    QFont f = font();
    f.setPointSize(kHudFontSize);
    p.setFont(f);
    p.setPen(Qt::black);
    const QPointF d = cursorWorld_ - lastClick_;
    const QStringList rows = {"Dx:", "Dy:", "", "X:", "Y:"};
    const double vals[] = {d.x(), d.y(), 0, cursorWorld_.x(), cursorWorld_.y()};
    const QFontMetricsF fm(f);
    double y = r.top() + 10 + fm.ascent();
    for (int i = 0; i < 5; ++i) {
        if (!rows[i].isEmpty()) {
            p.drawText(QPointF(r.left() + 5, y), rows[i]);
            p.drawText(QPointF(r.left() + 5 + fm.horizontalAdvance("Dx:") + 12, y),
                       QString::number(vals[i], 'f', 4) + " [mm]");
        }
        y += fm.height();
    }
}

void PlotCanvas::wheelEvent(QWheelEvent* event) {
    const double steps = event->angleDelta().y() / 120.0;
    if (steps == 0 || event->modifiers() != Qt::NoModifier) return;
    zoom(std::pow(kZoomPerStep, steps), toWorld(event->position()));
    event->accept();
}

void PlotCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {  // global_pan_button = 2 (botón derecho en vispy)
        panning_ = true;
        lastPan_ = event->position();
    } else if (event->button() == Qt::LeftButton) {
        lastClick_ = toWorld(event->position());
        update();
    }
    emit mouseClicked(toWorld(event->position()), event->button());
}

void PlotCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (panning_) {
        const QPointF delta = event->position() - lastPan_;
        lastPan_ = event->position();
        center_ -= QPointF(delta.x() / scale_, -delta.y() / scale_);
    }
    cursorInside_ = viewRect().contains(event->position());
    cursorWorld_ = toWorld(event->position());
    emit mouseMoved(cursorWorld_);
    update();
}

void PlotCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) panning_ = false;
}

void PlotCanvas::leaveEvent(QEvent*) {
    cursorInside_ = false;
    update();
}

}  // namespace flatcam::gui
