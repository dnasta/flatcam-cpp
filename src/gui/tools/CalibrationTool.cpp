#include "gui/tools/CalibrationTool.h"

#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Preferences.h"
#include "gui/ProjectIO.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/widgets/NumericalEval.h"
#include "tools/AlignObjects.h"

namespace flatcam::gui {

namespace {

/// get_value de EvalEntry: el número que da el texto (vacío o no numérico: None).
std::optional<double> evalEntry(const FCEntry* e) { return NumericExpression::evaluate(e->text().trimmed()); }

ObjectKind kindOf(int typeIndex) {
    return typeIndex == 0 ? ObjectKind::Gerber : typeIndex == 1 ? ObjectKind::Excellon : ObjectKind::Geometry;
}

}  // namespace

CalibrationTool::CalibrationTool(App* app, QWidget* parent) : CalibrationToolUIBase(parent), app_(app) {
    setupUi();
    buildPointsTable();
    connect(cal_source_radio, &RadioSet::activatedCustom, this, [this](const QString& v) {
        const bool dis = v != "object";
        obj_type_label->setDisabled(dis);
        obj_type_combo->setDisabled(dis);
        object_label->setDisabled(dis);
        object_combo->setDisabled(dis);
    });
    connect(obj_type_combo, &QComboBox::currentIndexChanged, this,
            [this] { fillCombo(object_combo, obj_type_combo->currentIndex()); });
    connect(adj_object_type_combo, &QComboBox::currentIndexChanged, this,
            [this] { fillCombo(adj_object_combo, adj_object_type_combo->currentIndex()); });
    connect(start_button, &QPushButton::clicked, this, [this] { startCollectPoints(); });
    connect(gcode_button, &QPushButton::clicked, this, [this] { generateVerificationGcode(); });
    connect(adj_gcode_button, &QPushButton::clicked, this, [this] { generateVerificationGcode(); });
    connect(generate_factors_button, &QPushButton::clicked, this, [this] { calculateFactors(); });
    connect(scale_button, &QPushButton::clicked, this, [this] { scalePoints(); });
    connect(skew_button, &QPushButton::clicked, this, [this] { skewPoints(); });
    connect(cal_button, &QPushButton::clicked, this, [this] { calibrate(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo CalibrationTool::info() { return {tr_("Calibration Tool"), tr_("Calibration Tool")}; }

void CalibrationTool::buildPointsTable() {
    // CalibUI: 8 filas; el número del punto en las filas pares, el rótulo y dos EvalEntry en cada una.
    static const char* labels[8] = {"Bot Left X",  "Bot Left Y",  "Bot Right X", "Bot Right Y",
                                    "Top Left X",  "Top Left Y",  "Top Right X", "Top Right Y"};
    for (int row = 0; row < 8; ++row) {
        if (row % 2 == 0) {
            auto* id = new QTableWidgetItem(QString::number(row / 2 + 1));
            id->setFlags(Qt::ItemIsEnabled);
            points_table->setItem(row, 0, id);
        }
        points_table->setCellWidget(row, 1, new QLabel(tr_(labels[row])));
        tgt[row] = new FCEntry();
        tgt[row]->setReadOnly(true);
        points_table->setCellWidget(row, 2, tgt[row]);
        found[row] = new FCEntry();
        points_table->setCellWidget(row, 3, found[row]);
    }
    for (int i : {0, 1, 6, 7}) found[i]->setDisabled(true);
    points_table->verticalHeader()->hide();
    points_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    points_table->resizeColumnsToContents();
    points_table->resizeRowsToContents();
    points_table->setMinimumHeight(points_table->contentHeight() + 2);
    points_table->setMaximumHeight(points_table->contentHeight() + 3);
}

void CalibrationTool::fillCombo(FCComboBox* combo, int typeIndex) {
    // setRootModelIndex(collection.index(obj_type, 0)): los objetos de ese grupo.
    const QString cur = combo->currentText();
    combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kindOf(typeIndex)) combo->addItem(o->name());
    if (!cur.isEmpty() && combo->findText(cur) >= 0) combo->setValue(cur);
}

void CalibrationTool::setToolUi() {
    if (picking_) disconnectEvents();
    found[0]->setText(tr_("Origin"));
    found[1]->setText(tr_("Origin"));
    resetPoints();
    cal_source_radio->setValue(defVariant("tools_cal_calsource"));
    Q_EMIT cal_source_radio->activatedCustom(cal_source_radio->value().toString());
    travelz_entry->setValue(defNum("tools_cal_travelz"));
    verz_entry->setValue(defNum("tools_cal_verz"));
    zeroz_cb->setChecked(defBool("tools_cal_zeroz"));
    toolchangez_entry->setValue(defNum("tools_cal_toolchangez"));
    toolchange_xy_entry->setText(defStr("tools_cal_toolchange_xy"));
    second_point_radio->setValue(defVariant("tools_cal_sec_point"));
    scalex_entry->setValue(1.0);
    scaley_entry->setValue(1.0);
    skewx_entry->setValue(0.0);
    skewy_entry->setValue(0.0);
    obj_type_combo->setCurrentIndex(1);
    fillCombo(object_combo, 1);
    adj_object_type_combo->setCurrentIndex(0);
    fillCombo(adj_object_combo, 0);
    app_->ui()->inform(QString("%1...").arg(tr_("Tool initialized")));
}

void CalibrationTool::resetPoints() {
    points_.clear();
    for (FCEntry* e : tgt) e->setText("");
    for (int i = 2; i < 6; ++i) found[i]->setText("");
}

bool CalibrationTool::startCollectPoints() {
    MainGUI* ui = app_->ui();
    if (cal_source_radio->value().toString() == "object") {
        target_ = app_->collection()->byName(object_combo->currentText());
        if (!target_) {
            ui->inform("[WARNING_NOTCL] " + tr_("There is no source FlatCAM object selected..."));
            return false;
        }
    }
    // Sin la grilla: sería difícil caer en los agujeros.
    gridMemory_ = ui->grid_snap_btn->isChecked();
    if (gridMemory_) ui->grid_snap_btn->trigger();
    disconnect(conn_);
    conn_ = connect(ui->plotCanvas(), &PlotCanvas::mouseReleased, this,
                    [this](QPointF w, Qt::MouseButton b, bool dragged) { onRelease(w, b, dragged); });
    ui->setCanvasHandlersEnabled(false);
    picking_ = true;
    resetPoints();
    ui->inform(tr_("Get First calibration point. Bottom Left..."));
    return true;
}

void CalibrationTool::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!picking_) return;
    const int decimals = appDecimals();
    if (button == Qt::LeftButton) {
        if (dragged) return;  // el original borra el rectángulo de selección y no elige nada
        const Point2D click{world.x(), world.y()};
        if (cal_source_radio->value().toString() == "object") {
            std::vector<Point2D> picked;
            if (target_->kind() == ObjectKind::Excellon)
                picked = alignPickExcellon(static_cast<ExcellonObject*>(target_)->excellon(), click, decimals);
            else if (target_->kind() == ObjectKind::Gerber)
                picked = alignPickGerber(static_cast<GerberObject*>(target_)->gerber(), click, decimals);
            // Cada figura bajo el clic agrega un punto (también después del cuarto), como el original.
            for (const Point2D& p : picked) {
                points_.push_back(p);
                checkPoints();
            }
        } else {
            points_.push_back({calibrationRound(click.x, decimals), calibrationRound(click.y, decimals)});
            checkPoints();
        }
    } else if (button == Qt::RightButton && !dragged) {
        if (points_.size() != 4) {
            resetPoints();
            disconnectEvents();
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled by user request."));
        }
    }
}

void CalibrationTool::checkPoints() {
    MainGUI* ui = app_->ui();
    const std::size_t n = points_.size();
    if (n >= 1 && n <= 4) {
        tgt[2 * (n - 1)]->setText(QString::fromStdString(pyFloatRepr(points_[n - 1].x)));
        tgt[2 * (n - 1) + 1]->setText(QString::fromStdString(pyFloatRepr(points_[n - 1].y)));
    }
    if (n == 1) ui->inform(tr_("Get Second calibration point. Bottom Right (Top Left)..."));
    else if (n == 2) ui->inform(tr_("Get Third calibration point. Top Left (Bottom Right)..."));
    else if (n == 3) ui->inform(tr_("Get Forth calibration point. Top Right..."));
    else if (n == 4) {
        ui->inform("[success] " + tr_("Done."));
        disconnectEvents();
    }
}

void CalibrationTool::disconnectEvents() {
    MainGUI* ui = app_->ui();
    if (gridMemory_) ui->grid_snap_btn->trigger();
    disconnect(conn_);
    ui->setCanvasHandlersEnabled(true);
    picking_ = false;
}

QString CalibrationTool::generateVerificationGcode() {
    MainGUI* ui = app_->ui();
    CalibrationGcodeParams p;
    p.secondPoint = second_point_radio->value().toString().toStdString();
    p.travelZ = travelz_entry->value();
    p.toolchangeZ = toolchangez_entry->value();
    p.verificationZ = verz_entry->value();
    for (const QString& a : toolchange_xy_entry->text().split(',')) {
        if (a.isEmpty()) continue;
        if (auto v = NumericExpression::evaluate(a)) p.toolchangeXy.push_back(*v);
    }
    if (points_.size() != 4) {
        ui->inform("[WARNING_NOTCL] " + tr_("Cancelled. Four points are needed for GCode generation."));
        return QString();
    }
    p.units = appUnits().toUpper().toStdString();
    p.zeroZ = zeroz_cb->isChecked();
    p.decimals = appDecimals();
    p.version = Preferences::version();
    p.versionDate = "2024/04/15";
    p.timeStr = (!timeOverride_.isEmpty()
                     ? timeOverride_
                     : QLocale::c().toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm"))
                    .toStdString();
    p.name = tr_("Verification GCode for FlatCAM Calibration Tool").toStdString();
    const QString gcode = QString::fromStdString(
        calibrationGcode({points_[0], points_[1], points_[2], points_[3]}, p));

    // AppTextEditor en una pestaña "Gcode Viewer" del área de dibujo, con "Guardar" (fc_ver_gcode).
    auto* tab = new QWidget();
    tab->setObjectName("gcode_viewer_tab");
    auto* lay = new QVBoxLayout(tab);
    auto* editor = new QPlainTextEdit(gcode);
    editor->setLineWrapMode(QPlainTextEdit::NoWrap);
    lay->addWidget(editor);
    auto* buttons = new QHBoxLayout();
    auto* save = new QPushButton(tr_("Save Code"));
    buttons->addStretch();
    buttons->addWidget(save);
    lay->addLayout(buttons);
    connect(save, &QPushButton::clicked, tab, [this, editor] {
        const QString f = QFileDialog::getSaveFileName(this, tr_("Save Code"), app_->lastFolder() + "/fc_ver_gcode",
                                                       "G-Code Files (*.nc);;All Files (*.*)");
        if (f.isEmpty()) return;
        QFile out(f);
        if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) return;
        out.write(editor->toPlainText().toUtf8());
        out.close();
        // close_tab: cierra las pestañas "Gcode Viewer".
        QTabWidget* area = app_->ui()->plot_tab_area;
        for (int i = area->count() - 1; i >= 0; --i)
            if (area->tabText(i) == tr_("Gcode Viewer")) {
                area->widget(i)->deleteLater();
                area->removeTab(i);
            }
    });
    const int idx = ui->plot_tab_area->addTab(tab, tr_("Gcode Viewer"));
    ui->position_label->setText("");
    ui->rel_position_label->setText("");
    ui->plot_tab_area->setCurrentIndex(idx);
    ui->inform(QString("[success] %1...").arg(tr_("Loaded Machine Code into Code Editor")));
    return gcode;
}

void CalibrationTool::calculateFactors() {
    // Sin los cuatro puntos el original falla (IndexError) y no hace nada.
    if (points_.size() < 3) return;
    std::vector<Point2D> pts = points_;
    pts.resize(4);
    const CalibrationFactors f = calibrationFactors({pts[0], pts[1], pts[2], pts[3]}, evalEntry(found[4]),
                                                    evalEntry(found[5]), evalEntry(found[2]), evalEntry(found[3]),
                                                    appDecimals());
    if (f.scaleX) scalex_entry->setValue(*f.scaleX);
    if (f.scaleY) scaley_entry->setValue(*f.scaleY);
    if (f.skewX) skewx_entry->setValue(*f.skewX);
    if (f.skewY) skewy_entry->setValue(*f.skewY);
}

CalibrationPoints CalibrationTool::readTargets() {
    // target_values_in_table (getter): los puntos pasan a ser los que muestran los campos.
    CalibrationPoints p{};
    for (std::size_t i = 0; i < 4; ++i) {
        p[i] = {evalEntry(tgt[2 * i]).value_or(0.0), evalEntry(tgt[2 * i + 1]).value_or(0.0)};
        points_[i] = p[i];
    }
    return p;
}

void CalibrationTool::setTargets(const CalibrationPoints& pts) {
    const int d = appDecimals();
    for (std::size_t i = 0; i < 4; ++i) {
        points_[i] = pts[i];
        tgt[2 * i]->setText(QString::fromStdString(pyFloatRepr(calibrationRound(pts[i].x, d))));
        tgt[2 * i + 1]->setText(QString::fromStdString(pyFloatRepr(calibrationRound(pts[i].y, d))));
    }
}

void CalibrationTool::scalePoints() {
    if (points_.size() < 4) return;  // IndexError en el original
    setTargets(calibrationScale(readTargets(), scalex_entry->value(), scaley_entry->value()));
}

void CalibrationTool::skewPoints() {
    if (points_.size() < 4) return;
    setTargets(calibrationSkew(readTargets(), skewx_entry->value(), skewy_entry->value()));
}

FlatCAMObj* CalibrationTool::calibrate() {
    MainGUI* ui = app_->ui();
    FlatCAMObj* obj = app_->collection()->byName(adj_object_combo->currentText());
    if (!obj) {
        ui->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return nullptr;
    }
    if (points_.empty()) return nullptr;  // IndexError: 'fail' sin mensaje
    const Point2D origin = points_[0];
    const QString name = obj->name() + "_calibrated";
    if (obj->kind() == ObjectKind::Excellon) {
        // initialize_excellon copia obj.drills, que la 2024.4 ya no tiene: new_object falla siempre.
        ui->inform(QString("[ERROR_NOTCL] %1%2").arg(
            tr_("An internal error has occurred. See shell.\n"),
            tr_("Object ({kind}) failed because: {error} \n\n")
                .replace("{kind}", "excellon")
                .replace("{error}", "'ExcellonObject' object has no attribute 'drills'")));
        return nullptr;
    }
    // Una copia del objeto (deepcopy de su geometría, aperturas y herramientas), escalada e inclinada desde el origen.
    JsonValue d = ProjectIO::toDict(*obj, app_->appOptions());
    d.find("options")->set("name", JsonValue::string(name.toStdString()));
    QString warning;
    auto copy = ProjectIO::objectFrom(d, warning);
    if (!copy) return nullptr;
    copy->scale(scalex_entry->value(), scaley_entry->value(), origin);
    copy->skew(skewx_entry->value(), skewy_entry->value(), origin);
    FlatCAMObj* out = app_->addObject(std::move(copy));
    if (out->kind() == ObjectKind::Gerber) app_->refreshSourceFile(out);  // export_gerber(local_use=obj_init)
    return out;
}

}  // namespace flatcam::gui
