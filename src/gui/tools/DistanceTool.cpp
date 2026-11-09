#include "gui/Preferences.h"
#include "gui/tools/DistanceTool.h"

#include <QSplitter>
#include <cmath>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

namespace {

constexpr int kDecimals = 4;  // app.decimals

/** @brief `'%.*f' % (decimals, v)`. */
QString fmt(double v) { return QString::asprintf("%.*f", kDecimals, v); }

/** @brief `"(%.*f, %.*f)"`. */
QString pointText(double x, double y) { return "(" + fmt(x) + ", " + fmt(y) + ")"; }

/** @brief El mensaje del resultado; el original usa `'%*f'` (ancho 4, no decimales: quedan los 6 de `%f`). */
QString resultMessage(double dx, double dy, double d) {
    return QString("%1: %2 D(x) = %3 | D(y) = %4 | %5 = %6")
        .arg(tr_("MEASURING"), tr_("Result"), QString::asprintf("%*f", kDecimals, std::abs(dx)),
             QString::asprintf("%*f", kDecimals, std::abs(dy)), tr_("Distance"),
             QString::asprintf("%*f", kDecimals, std::abs(d)));
}

}  // namespace

// ------------------------------------------------------------------------------------------------ Distancia

DistanceTool::DistanceTool(App* app, QWidget* parent) : DistanceToolUIBase(parent), app_(app) {
    setupUi();
    connect(measure_btn, &QPushButton::clicked, this, [this] { activate(); });
}

ToolInfo DistanceTool::info() { return {tr_("Distance Tool"), tr_("Distance Tool")}; }

void DistanceTool::run() {
    points_.clear();
    relPoint1_.reset();
    toolDone_ = false;
    MainGUI* ui = app_->ui();
    ui->notebook->setTabText(2, tr_("Distance Tool"));
    if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
    if (!active_) activate();
    else deactivate();
}

void DistanceTool::setToolUi() {
    app_->runTool(this, info(), false);
    start_entry->setValue(QString("(0, 0)"));
    stop_entry->setValue(QString("(0, 0)"));
    distance_x_entry->setValue(QString("0.0"));
    distance_y_entry->setValue(QString("0.0"));
    angle_entry->setValue(QString("0.0"));
    total_distance_entry->setValue(QString("0.0"));
    snap_center_cb->setChecked(defBool("tools_dist_snap_center"));
    snap_center_cb->hide();             // solo en los editores de Gerber y Excellon
    // El original dispara dos veces la grilla (sin efecto neto) y recuerda si estaba activa; nunca lo vuelve a False.
    if (app_->ui()->grid_snap_btn->isChecked()) gridMemory_ = true;
}

void DistanceTool::activate() {
    active_ = true;
    measure_btn->setDisabled(true);
    measure_btn->setText(tr_("Working") + "...");
    MainGUI* ui = app_->ui();
    ui->inform(tr_("MEASURING: Click on the Start point ..."));
    PlotCanvas* canvas = ui->plotCanvas();
    for (auto& c : connections_) disconnect(c);
    connections_.clear();
    connections_.push_back(connect(canvas, &PlotCanvas::mouseMoved, this, [this](QPointF w) { onMove(w); }));
    connections_.push_back(connect(canvas, &PlotCanvas::mouseReleased, this,
                                   [this](QPointF w, Qt::MouseButton b, bool dragged) { onRelease(w, b, dragged); }));
    // Se desconectan los manejadores del mouse de la aplicación; el HUD muestra Dx = Dy = 0.
    ui->setCanvasHandlersEnabled(false);
    canvas->setHudShowsDelta(false);
    setToolUi();
}

void DistanceTool::deactivate() {
    active_ = false;
    points_.clear();
    measure_btn->setDisabled(false);
    measure_btn->setText(tr_("Measure"));
    MainGUI* ui = app_->ui();
    for (auto& c : connections_) disconnect(c);
    connections_.clear();
    ui->setCanvasHandlersEnabled(true);
    ui->plotCanvas()->setHudShowsDelta(true);
    ui->plotCanvas()->removeLayer(kLayerId);
    // Restaura la grilla como estaba al empezar (según grid_status_memory).
    const bool on = ui->grid_snap_btn->isChecked();
    if ((on && !gridMemory_) || (!on && gridMemory_)) ui->grid_snap_btn->trigger();
    if (!toolDone_) ui->inform(tr_("Distance Tool finished."));
}

void DistanceTool::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!active_) return;
    if (button == Qt::LeftButton) {
        const QPointF pos = world;  // sin "Ajustar al centro": la posición ya viene ajustada a la grilla
        points_.push_back(pos);
        // Se reinicia la referencia relativa en el punto del clic.
        if (!relPoint1_) {
            app_->ui()->rel_position_label->setText(
                QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;").arg(fmt(0.0), fmt(0.0)));
        }
        relPoint1_ = pos;
        calculateDistance(pos);
    } else if (button == Qt::RightButton && !dragged) {
        deactivate();
        app_->ui()->inform(tr_("Distance Tool cancelled."));
    }
}

void DistanceTool::calculateDistance(const QPointF& pos) {
    MainGUI* ui = app_->ui();
    if (points_.size() == 1) {
        start_entry->setValue(pointText(pos.x(), pos.y()));
        ui->inform(tr_("Click on the DESTINATION point ..."));
    } else if (points_.size() == 2) {
        const double dx = points_[1].x() - points_[0].x();
        const double dy = points_[1].y() - points_[0].y();
        const double d = std::sqrt(dx * dx + dy * dy);
        stop_entry->setValue(pointText(pos.x(), pos.y()));
        ui->inform(resultMessage(dx, dy, d));
        distance_x_entry->setValue(fmt(std::abs(dx)));
        distance_y_entry->setValue(fmt(std::abs(dy)));
        double angle = std::atan2(dy, dx) * 180.0 / M_PI;
        if (angle < 0) angle += 360;
        angle_entry->setValue(fmt(angle));
        total_distance_entry->setValue(fmt(std::abs(d)));
        ui->rel_position_label->setText(
            QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;").arg(fmt(pos.x()), fmt(pos.y())));
        toolDone_ = true;
        deactivate();
    }
}

void DistanceTool::onMove(const QPointF& pos) {
    if (!active_) return;
    MainGUI* ui = app_->ui();
    ui->position_label->setText(
        QString("&nbsp;&nbsp;&nbsp;&nbsp;<b>X</b>: %1&nbsp;&nbsp;   <b>Y</b>: %2").arg(fmt(pos.x()), fmt(pos.y())));
    const double dx = relPoint1_ ? pos.x() - relPoint1_->x() : pos.x();
    const double dy = relPoint1_ ? pos.y() - relPoint1_->y() : pos.y();
    ui->rel_position_label->setText(
        QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;").arg(fmt(dx), fmt(dy)));
    if (points_.size() == 1) {
        drawLine(pos);
        double angle = std::atan2(dy, dx) * 180.0 / M_PI;  // el ángulo provisorio
        if (angle < 0) angle += 360;
        angle_entry->setValue(fmt(angle));
    }
}

void DistanceTool::drawLine(const QPointF& pos) {
    // utility_geometry: la línea del inicio al cursor, negra (tema claro).
    PlotLayerData d;
    d.lineColor = colorFromFlatCam("#000000FF");
    PlotGroup g;
    QRectF bounds;
    FlatCAMObj::addShapeTo(Shape::lineString({{pos.x(), pos.y()}, {points_[0].x(), points_[0].y()}}), g, bounds, false);
    d.lines = std::move(g.lines);
    app_->ui()->plotCanvas()->setLayer(kLayerId, std::move(d));
}

// ------------------------------------------------------------------------------------------------ Distancia mínima

DistanceMinTool::DistanceMinTool(App* app, QWidget* parent) : DistanceMinToolUIBase(parent), app_(app) {
    setupUi();
    connect(measure_btn, &QPushButton::clicked, this, [this] { measure(); });
    connect(jump_hp_btn, &QPushButton::clicked, this, [this] { jumpToHalfPoint(); });
}

ToolInfo DistanceMinTool::info() { return {tr_("Minimum Distance Tool"), tr_("Minimum Distance Tool")}; }

void DistanceMinTool::run() {
    setToolUi();
    app_->ui()->inform("MEASURING: " + tr_("Select two objects and no more, to measure the distance between them ..."));
}

void DistanceMinTool::setToolUi() {
    app_->runTool(this, info(), false);
    start_entry->setValue(QString("(0, 0)"));
    stop_entry->setValue(QString("(0, 0)"));
    distance_x_entry->setValue(QString("0.0"));
    distance_y_entry->setValue(QString("0.0"));
    angle_entry->setValue(QString("0.0"));
    total_distance_entry->setValue(QString("0.0"));
    half_point_entry->setValue(QString("(0, 0)"));
    jump_hp_btn->setDisabled(true);
}

void DistanceMinTool::measure() {
    jump_hp_btn->setDisabled(false);
    MainGUI* ui = app_->ui();
    const std::vector<FlatCAMObj*> sel = app_->selectedObjects();
    if (sel.size() != 2) {
        ui->inform(QString("[WARNING_NOTCL] %1 %2")
                       .arg(tr_("Select two objects and no more. Currently the selection has objects: "))
                       .arg(sel.size()));
        return;
    }
    // La lista de figuras pasa a MultiPolygon y, si no son todos polígonos, a su unión (unary_union). Desvío: el
    // original solo convierte si la del primer objeto es una lista, y si no lo es y la del segundo sí, falla.
    auto asShape = [](const ShapeList& list) {
        bool polygons = true;
        for (const Shape& s : list) polygons = polygons && s.type() == ShapeType::Polygon;
        return polygons ? Shape::collection(list) : Shape::unaryUnion(list);
    };
    const auto pts = asShape(sel[0]->solidShapes()).nearestPoints(asShape(sel[1]->solidShapes()));
    if (!pts) return;  // nearest_points con una geometría vacía: el original falla y no muestra nada
    const Point2D first = pts->first, last = pts->second;
    start_entry->setValue(pointText(first.x, first.y));
    stop_entry->setValue(pointText(last.x, last.y));
    const double dx = first.x - last.x, dy = first.y - last.y;
    distance_x_entry->setValue(fmt(std::abs(dx)));
    distance_y_entry->setValue(fmt(std::abs(dy)));
    if (dx != 0) angle_entry->setValue(fmt(std::atan(dy / dx) * 180.0 / M_PI));  // dy / 0 da ZeroDivisionError
    const double d = std::sqrt(dx * dx + dy * dy);
    total_distance_entry->setValue(fmt(std::abs(d)));
    hPoint_ = QPointF(std::min(first.x, last.x) + std::abs(dx) / 2, std::min(first.y, last.y) + std::abs(dy) / 2);
    half_point_entry->setValue(d != 0 ? pointText(hPoint_.x(), hPoint_.y()) : pointText(0.0, 0.0));
    if (d != 0)
        ui->inform(resultMessage(dx, dy, d));
    else
        ui->inform(QString("[WARNING_NOTCL] %1: %2")
                       .arg(tr_("Objects intersects or touch at"), pointText(hPoint_.x(), hPoint_.y())));
}

void DistanceMinTool::jumpToHalfPoint() {
    app_->jumpTo(hPoint_);
    app_->ui()->inform(QString("[success] %1: %2")
                           .arg(tr_("Jumped to the half point between the two selected objects"),
                                pointText(hPoint_.x(), hPoint_.y())));
}

}  // namespace flatcam::gui
