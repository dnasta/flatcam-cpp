#include "gui/tools/AlignTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/AlignObjects.h"

namespace flatcam::gui {

AlignTool::AlignTool(App* app, QWidget* parent) : AlignToolUIBase(parent), app_(app) {
    setupUi();
    connect(align_object_button, &QPushButton::clicked, this, [this] { align(); });
    connect(type_obj_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { fillCombo(object_combo, v); });
    connect(type_aligner_obj_radio, &RadioSet::activatedCustom, this,
            [this](const QString& v) { fillCombo(aligner_object_combo, v); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo AlignTool::info() { return {tr_("Align Objects"), tr_("Align Tool")}; }

void AlignTool::setToolUi() {
    points_.clear();
    target_ = aligned_ = aligner_ = nullptr;
    a_type_radio->setValue(defVariant("tools_align_objects_align_type"));
    type_obj_radio->setValue(QString("grb"));
    type_aligner_obj_radio->setValue(QString("grb"));
    // reset_fields y on_type_*_changed: los dos combos muestran los Gerber, con el primero elegido.
    fillCombo(object_combo, "grb");
    fillCombo(aligner_object_combo, "grb");
    if (picking_) disconnectEvents();
}

void AlignTool::fillCombo(FCComboBox* combo, const QString& type) {
    const ObjectKind kind = type == "exc" ? ObjectKind::Excellon : ObjectKind::Gerber;
    combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kind) combo->addItem(o->name());
    combo->setCurrentIndex(0);
}

bool AlignTool::align() {
    app_->deleteSelectionShape();
    MainGUI* ui = app_->ui();
    aligned_ = app_->collection()->byName(object_combo->currentText());
    if (object_combo->currentText().isEmpty() || !aligned_) {
        ui->inform("[WARNING_NOTCL] " + tr_("There is no aligned FlatCAM object selected..."));
        return false;
    }
    aligner_ = app_->collection()->byName(aligner_object_combo->currentText());
    if (aligner_object_combo->currentText().isEmpty() || !aligner_) {
        ui->inform("[WARNING_NOTCL] " + tr_("There is no aligner FlatCAM object selected..."));
        return false;
    }
    alignType_ = a_type_radio->value().toString();
    // Sin la grilla: sería difícil caer en los agujeros o los pads.
    gridMemory_ = ui->grid_snap_btn->isChecked();
    if (gridMemory_) ui->grid_snap_btn->trigger();
    picking_ = true;
    target_ = aligned_;
    setColor();
    ui->inform(QString("%1: %2").arg(tr_("First Point"), tr_("Click on the START point.")));
    disconnect(conn_);
    conn_ = connect(ui->plotCanvas(), &PlotCanvas::mouseReleased, this,
                    [this](QPointF w, Qt::MouseButton b, bool dragged) { onRelease(w, b, dragged); });
    ui->setCanvasHandlersEnabled(false);
    return true;
}

void AlignTool::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!picking_) return;
    if (button == Qt::LeftButton) {
        // Con arrastre el original borra el rectángulo de selección y no elige nada.
        if (dragged) return;
        const Point2D click{world.x(), world.y()};
        std::vector<Point2D> picked;
        if (target_->kind() == ObjectKind::Excellon)
            picked = alignPickExcellon(static_cast<ExcellonObject*>(target_)->excellon(), click, appDecimals());
        else if (target_->kind() == ObjectKind::Gerber)
            picked = alignPickGerber(static_cast<GerberObject*>(target_)->gerber(), click, appDecimals());
        // Cada punto agregado avanza un paso (con dos figuras bajo el clic, dos pasos), como el original.
        for (const Point2D& p : picked) {
            points_.push_back(p);
            checkPoints();
            if (!picking_) break;
        }
    } else if (button == Qt::RightButton && !dragged) {
        resetColor();
        points_.clear();
        disconnectEvents();
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled by user request."));
    }
}

void AlignTool::checkPoints() {
    MainGUI* ui = app_->ui();
    const std::size_t n = points_.size();
    if (n == 1) {
        ui->inform(QString("%1: %2. %3").arg(tr_("First Point"), tr_("Click on the DESTINATION point ..."),
                                            tr_("Or right click to cancel.")));
        target_ = aligner_;
        resetColor();
        setColor();
    }
    if (n == 2) {
        if (alignType_ == "sp") {
            applyMove(false);
            ui->inform("[success] " + tr_("Done."));
            app_->plotAll();
            disconnectEvents();
            return;
        }
        ui->inform(QString("%1: %2. %3").arg(tr_("Second Point"), tr_("Click on the START point."),
                                            tr_("Or right click to cancel.")));
        target_ = aligned_;
        resetColor();
        setColor();
    }
    if (n == 3) {
        ui->inform(QString("%1: %2. %3").arg(tr_("Second Point"), tr_("Click on the DESTINATION point ..."),
                                            tr_("Or right click to cancel.")));
        target_ = aligner_;
        resetColor();
        setColor();
    }
    if (n == 4) {
        applyMove(true);
        if (picking_ && alignMove(points_, appDecimals()).fails) return;  // el original se corta acá
        ui->inform("[success] " + tr_("Done."));
        disconnectEvents();
        app_->plotAll();
    }
}

void AlignTool::applyMove(bool rotate) {
    const AlignMove m = alignMove(points_, appDecimals());
    aligned_->offset(m.offset.x, m.offset.y);
    if (rotate && m.angle) aligned_->rotate(*m.angle, m.origin);
    // Con la división por cero el original ya desplazó el objeto, no lo gira y sigue esperando clics.
    app_->objectChanged(aligned_);
}

void AlignTool::setColor() {
    const QColor c = colorFromFlatCam("#15678abf");
    const QColor fill = target_->fillColor, line = target_->outlineColor;
    target_->fillColor = target_->outlineColor = c;
    app_->ui()->plotCanvas()->setLayer(target_->id(), target_->plotData());
    target_->fillColor = fill;
    target_->outlineColor = line;
}

void AlignTool::resetColor() {
    for (FlatCAMObj* o : {aligned_, aligner_})
        if (o) app_->ui()->plotCanvas()->setLayer(o->id(), o->plotData());
}

void AlignTool::disconnectEvents() {
    MainGUI* ui = app_->ui();
    if (gridMemory_) ui->grid_snap_btn->trigger();
    disconnect(conn_);
    ui->setCanvasHandlersEnabled(true);
    picking_ = false;
}

}  // namespace flatcam::gui
