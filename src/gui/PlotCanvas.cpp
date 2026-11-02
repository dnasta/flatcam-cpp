#include "gui/PlotCanvas.h"

#include <algorithm>

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QVector4D>
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

void PlotCanvas::initializeGL() {
    initializeOpenGLFunctions();
    // GLSL 1.20: funciona con el contexto de compatibilidad por defecto en macOS, Windows y Linux.
    program_ = std::make_unique<QOpenGLShaderProgram>();
    program_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                      "#version 120\n"
                                      "attribute vec2 pos;\n"
                                      "uniform vec4 view;  // centro x, centro y, escala x, escala y\n"
                                      "void main() { gl_Position = vec4((pos - view.xy) * view.zw, 0.0, 1.0); }\n");
    program_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                      "#version 120\n"
                                      "uniform vec4 color;\n"
                                      "void main() { gl_FragColor = color; }\n");
    program_->bindAttributeLocation("pos", 0);
    program_->link();
    for (auto& [id, l] : layers_) l.dirty = true;  // el contexto es nuevo: hay que volver a cargar todo
}

void PlotCanvas::setLayer(int id, PlotLayerData data) {
    Layer& l = layers_[id];
    l.data = std::move(data);
    l.dirty = true;
    update();
}

void PlotCanvas::setLayerVisible(int id, bool visible) {
    auto it = layers_.find(id);
    if (it == layers_.end()) return;
    it->second.visible = visible;
    update();
}

void PlotCanvas::setLayerOrder(std::vector<int> idsBottomFirst) {
    order_ = std::move(idsBottomFirst);
    update();
}

void PlotCanvas::removeLayer(int id) {
    auto it = layers_.find(id);
    if (it == layers_.end()) return;
    makeCurrent();
    layers_.erase(it);  // los buffers se liberan con el contexto activo
    doneCurrent();
    update();
}

std::optional<QRectF> PlotCanvas::layersBounds() const {
    std::optional<QRectF> b;
    for (const auto& [id, l] : layers_) {
        if (id < 0 || !l.visible || l.data.bounds.isNull()) continue;
        b = b ? b->united(l.data.bounds) : l.data.bounds;
    }
    return b;
}

void PlotCanvas::fitView() {
    // Sin objetos: Rect(-1, -1, 20, 20) de vispy (izquierda, abajo, ancho, alto).
    const QRectF r = layersBounds().value_or(QRectF(-1, -1, 20, 20));
    const double dx = r.width() * 0.02, dy = r.height() * 0.02;
    fitTo(r.adjusted(-dx, -dy, dx, dy));
}

void PlotCanvas::drawLayers(const QRectF& vr) {
    if (!program_ || layers_.empty()) return;
    const double dpr = devicePixelRatioF();
    const int vx = static_cast<int>(vr.left() * dpr);
    const int vy = static_cast<int>((height() - vr.bottom()) * dpr);  // OpenGL cuenta desde abajo
    const int vw = static_cast<int>(vr.width() * dpr), vh = static_cast<int>(vr.height() * dpr);
    glViewport(vx, vy, vw, vh);
    glEnable(GL_SCISSOR_TEST);
    glScissor(vx, vy, vw, vh);
    glEnable(GL_BLEND);
    // El alfa del destino tiene que quedar en 1: Qt compone el framebuffer del widget como color premultiplicado, y un
    // alfa menor deja los colores corridos (se veían magenta y verde azulado).
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    program_->bind();
    // Centro del mundo en el medio de la vista; escala en unidades normalizadas por mm.
    const QPointF c = toWorld(vr.center());
    program_->setUniformValue("view", QVector4D(float(c.x()), float(c.y()), float(2.0 * scale_ / vr.width()),
                                                float(2.0 * scale_ / vr.height())));
    auto upload = [](std::unique_ptr<QOpenGLBuffer>& vbo, const std::vector<float>& v) {
        if (!vbo) {
            vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
            vbo->create();
        }
        vbo->bind();
        vbo->allocate(v.data(), static_cast<int>(v.size() * sizeof(float)));
        vbo->release();
    };
    auto draw = [this](QOpenGLBuffer* vbo, std::size_t floats, GLenum mode, const QColor& color) {
        if (!vbo || floats == 0) return;
        program_->setUniformValue("color", color);
        vbo->bind();
        program_->enableAttributeArray(0);
        program_->setAttributeBuffer(0, GL_FLOAT, 0, 2);
        glDrawArrays(mode, 0, static_cast<GLsizei>(floats / 2));
        program_->disableAttributeArray(0);
        vbo->release();
    };
    std::vector<Layer*> seq;
    for (auto& [id, l] : layers_)
        if (id >= 0 && std::find(order_.begin(), order_.end(), id) == order_.end()) seq.push_back(&l);
    for (int id : order_)
        if (auto it = layers_.find(id); it != layers_.end()) seq.push_back(&it->second);
    for (auto& [id, l] : layers_)
        if (id < 0) seq.push_back(&l);  // capas auxiliares, encima de todo
    for (Layer* lp : seq) {
        Layer& l = *lp;
        if (l.dirty) {
            upload(l.fillVbo, l.data.fill);
            upload(l.lineVbo, l.data.lines);
            upload(l.travelVbo, l.data.travel);
            l.groupVbos.resize(l.data.groups.size() * 2);
            for (std::size_t g = 0; g < l.data.groups.size(); ++g) {
                upload(l.groupVbos[2 * g], l.data.groups[g].fill);
                upload(l.groupVbos[2 * g + 1], l.data.groups[g].lines);
            }
            l.dirty = false;
        }
        if (!l.visible) continue;
        draw(l.fillVbo.get(), l.data.fill.size(), GL_TRIANGLES, l.data.fillColor);
        draw(l.lineVbo.get(), l.data.lines.size(), GL_LINES, l.data.lineColor);
        for (std::size_t g = 0; g < l.data.groups.size(); ++g) {
            const PlotGroup& pg = l.data.groups[g];
            draw(l.groupVbos[2 * g].get(), pg.fill.size(), GL_TRIANGLES, pg.fillColor);
            draw(l.groupVbos[2 * g + 1].get(), pg.lines.size(), GL_LINES, pg.lineColor);
        }
        draw(l.travelVbo.get(), l.data.travel.size(), GL_LINES, l.data.travelColor);
    }
    program_->release();
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, static_cast<int>(width() * dpr), static_cast<int>(height() * dpr));
}

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
    p.beginNativePainting();
    drawLayers(vr);
    p.endNativePainting();
    if (axisEnabled_) {
        const QPointF o = toPixel({0, 0});
        p.setPen(QPen(kAxisLineColor, 2));
        p.drawLine(QPointF(o.x(), vr.top()), QPointF(o.x(), vr.bottom()));
        p.drawLine(QPointF(vr.left(), o.y()), QPointF(vr.right(), o.y()));
    }
    if (cursorInside_ && cursorEnabled_) {
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
    pressPos_ = event->position();
    if (event->button() == Qt::RightButton) {  // global_pan_button = 2 (botón derecho en vispy)
        panning_ = true;
        lastPan_ = event->position();
    } else if (event->button() == Qt::LeftButton) {
        lastClick_ = eventWorld(event->position());
        update();
    }
    Q_EMIT mouseClicked(eventWorld(event->position()), event->button());
}

void PlotCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (panning_) {
        const QPointF delta = event->position() - lastPan_;
        lastPan_ = event->position();
        center_ -= QPointF(delta.x() / scale_, -delta.y() / scale_);
    }
    cursorInside_ = viewRect().contains(event->position());
    cursorWorld_ = eventWorld(event->position());
    Q_EMIT mouseMoved(cursorWorld_);
    update();
}

void PlotCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) panning_ = false;
    const QPointF d = event->position() - pressPos_;
    Q_EMIT mouseReleased(eventWorld(event->position()), event->button(), std::hypot(d.x(), d.y()) > 3.0);
}

QPointF PlotCanvas::eventWorld(const QPointF& pixel) const {
    const QPointF w = toWorld(pixel);
    return snapper_ ? snapper_(w) : w;
}

void PlotCanvas::leaveEvent(QEvent*) {
    cursorInside_ = false;
    update();
}

}  // namespace flatcam::gui
