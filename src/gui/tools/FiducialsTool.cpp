#include "gui/tools/FiducialsTool.h"

#include <QHeaderView>
#include <QTableWidgetItem>

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/Fiducials.h"

namespace flatcam::gui {

namespace {

/// str((x, y)) de Python.
QString pyTuple(const Point2D& p) {
    return QString("(%1, %2)").arg(QString::fromStdString(pyFloatRepr(p.x)), QString::fromStdString(pyFloatRepr(p.y)));
}

}  // namespace

FiducialsTool::FiducialsTool(App* app, QWidget* parent) : FiducialsToolUIBase(parent), app_(app) {
    setupUi();
    // La tabla de los puntos (FidoUI): número, nombre y un campo con las coordenadas.
    const QStringList names{tr_("Bottom Left"), tr_("Top Right"), tr_("Second Point")};
    FCEntry** entries[] = {&bottom_left_coords_entry, &top_right_coords_entry, &sec_points_coords_entry};
    for (int row = 0; row < 3; ++row) {
        auto* id = new QTableWidgetItem(QString::number(row + 1));
        id->setFlags(Qt::ItemIsEnabled);
        points_table->setItem(row, 0, id);
        auto* lbl = new QTableWidgetItem(names[row]);
        lbl->setFlags(Qt::ItemIsEnabled);
        points_table->setItem(row, 1, lbl);
        *entries[row] = new FCEntry(points_table);
        (*entries[row])->setFrame(false);
        points_table->setCellWidget(row, 2, *entries[row]);
        if (row == 2) {
            idItem3_ = id;
            secLabel_ = lbl;
        }
    }
    points_table->horizontalHeader()->setMinimumSectionSize(10);
    points_table->horizontalHeader()->setDefaultSectionSize(70);
    points_table->resizeColumnsToContents();
    points_table->resizeRowsToContents();
    points_table->horizontalHeader()->resizeSection(0, 20);

    connect(add_cfid_button, &QPushButton::clicked, this, [this] { addFiducials(); });
    connect(add_sm_opening_button, &QPushButton::clicked, this, [this] { addSoldermaskOpening(); });
    connect(fid_type_radio, &RadioSet::activatedCustom, this, &FiducialsTool::onFiducialType);
    connect(pos_radio, &RadioSet::activatedCustom, this, &FiducialsTool::onSecondPoint);
    connect(mode_radio, &RadioSet::activatedCustom, this, &FiducialsTool::onMethodChange);
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo FiducialsTool::info() { return {tr_("Fiducials Tool"), tr_("Fiducials Tool")}; }

void FiducialsTool::fillCombo(FCComboBox* combo) {
    const QString cur = combo->currentText();
    combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) combo->addItem(o->name());
    if (!cur.isEmpty() && combo->findText(cur) >= 0) combo->setValue(cur);
    else combo->setCurrentIndex(combo->count() - 1);  // is_last: el último Gerber agregado
}

void FiducialsTool::setToolUi() {
    fid_size_entry->setValue(defNum("tools_fiducials_dia"));
    margin_entry->setValue(defNum("tools_fiducials_margin"));
    mode_radio->setValue(defVariant("tools_fiducials_mode"));
    pos_radio->setValue(defVariant("tools_fiducials_second_pos"));
    fid_type_radio->setValue(defVariant("tools_fiducials_type"));
    line_thickness_entry->setValue(defNum("tools_fiducials_line_thickness"));
    // Las señales activated_custom de los RadioSet.
    onSecondPoint(pos_radio->value().toString());
    onFiducialType(fid_type_radio->value().toString());
    points_.clear();
    bottom_left_coords_entry->setValue(QString());
    top_right_coords_entry->setValue(QString());
    sec_points_coords_entry->setValue(QString());
    fillCombo(grb_object_combo);
    fillCombo(sm_object_combo);
}

void FiducialsTool::onSecondPoint(const QString& val) {
    const Qt::ItemFlags f = val == "no" ? Qt::NoItemFlags : Qt::ItemIsEnabled;
    idItem3_->setFlags(f);
    secLabel_->setFlags(f);
    sec_points_coords_entry->setDisabled(val == "no");
}

void FiducialsTool::onMethodChange(const QString& val) {
    points_.clear();
    if (val == "auto") setPicking(false);
}

void FiducialsTool::onFiducialType(const QString& val) {
    line_thickness_label->setDisabled(val != "cross");
    line_thickness_entry->setDisabled(val != "cross");
}

void FiducialsTool::addFiducials() {
    const QString mode = mode_radio->value().toString();
    const double margin = margin_entry->value();
    secPosition_ = pos_radio->value().toString();
    const QString type = fid_type_radio->value().toString();
    points_.clear();
    grb_ = app_->collection()->byName(grb_object_combo->currentText());
    if (!grb_ || grb_->kind() != ObjectKind::Gerber) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return;
    }
    if (mode == "auto") {
        const QRectF b = grb_->bounds();
        points_ = fiducialAutoPoints({b.left(), b.top(), b.right(), b.bottom()}, margin, secPosition_.toStdString(),
                                     appDecimals(), defInt("gerber_circle_steps", 64));
        // '(%.*f, %.*f)' del punto sin redondear: el mismo texto que el punto redondeado.
        const int dec = appDecimals();
        auto txt = [dec](const Point2D& p) {
            return QString("(%1, %2)").arg(QString::number(p.x, 'f', dec), QString::number(p.y, 'f', dec));
        };
        bottom_left_coords_entry->setValue(txt(points_[0]));
        top_right_coords_entry->setValue(txt(points_[1]));
        if (points_.size() > 2) sec_points_coords_entry->setValue(txt(points_[2]));
        if (!addGeo(grb_, fid_size_entry->value(), type)) {
            setPicking(false);
            return;  // el original falla con TypeError (la cruz unida en un solo polígono)
        }
        onExit();
    } else {
        app_->ui()->inform(tr_("Click to add first Fiducial. Bottom Left..."));
        bottom_left_coords_entry->setValue(QString());
        top_right_coords_entry->setValue(QString());
        sec_points_coords_entry->setValue(QString());
        setPicking(true);
    }
}

FlatCAMObj* FiducialsTool::addGeo(FlatCAMObj* obj, double size, const QString& type) {
    const FiducialType t = type == "circular" ? FiducialType::Circular
                           : type == "cross"  ? FiducialType::Cross
                                              : FiducialType::Chess;
    const Gerber& g = static_cast<GerberObject*>(obj)->gerber();
    auto r = ::flatcam::addFiducials(g, points_, size, t, line_thickness_entry->value(), defInt("gerber_circle_steps", 64),
                          appDecimals());
    if (!r) return nullptr;
    auto out = std::make_unique<Gerber>();
    out->restore(std::move(r->solid), std::move(r->follow), std::move(r->apertures), g.intDigits(), g.fracDigits(),
                 std::string());
    auto o = std::make_unique<GerberObject>(obj->name() + "_fid", std::move(out));
    lastCreated_ = app_->addObject(std::move(o));
    return lastCreated_;
}

FlatCAMObj* FiducialsTool::addSoldermaskOpening() {
    FlatCAMObj* sm = app_->collection()->byName(sm_object_combo->currentText());
    if (!sm || sm->kind() != ObjectKind::Gerber) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    // Con los puntos del último "Agregar fiducial" (ninguno si no se agregó).
    FlatCAMObj* out = addGeo(sm, fid_size_entry->value() * 2.0, "circular");
    onExit();
    return out;
}

void FiducialsTool::onRelease(const QPointF& world, Qt::MouseButton button) {
    if (!picking_ || button != Qt::LeftButton) return;
    const int dec = appDecimals();
    points_.push_back({QString::number(world.x(), 'f', dec).toDouble(), QString::number(world.y(), 'f', dec).toDouble()});
    checkPoints();
}

void FiducialsTool::checkPoints() {
    MainGUI* ui = app_->ui();
    const QString type = fid_type_radio->value().toString();
    const std::size_t n = points_.size();
    if (n == 1) {
        bottom_left_coords_entry->setValue(pyTuple(points_[0]));
        ui->inform(tr_("Click to add the last fiducial. Top Right..."));
    }
    const std::size_t last = secPosition_ != "no" ? 3 : 2;
    if (secPosition_ != "no" && n == 2) {
        top_right_coords_entry->setValue(pyTuple(points_[1]));
        ui->inform(tr_("Click to add the second fiducial. Top Left or Bottom Right..."));
    }
    if (n == last) {
        (last == 3 ? sec_points_coords_entry : top_right_coords_entry)->setValue(pyTuple(points_[n - 1]));
        ui->inform("[success] " + tr_("Done."));
        // Si el original falla (TypeError) sigue esperando clics, sin crear nada.
        if (!addGeo(grb_, fid_size_entry->value(), type)) return;
        onExit();
    }
}

void FiducialsTool::onExit() {
    setPicking(false);
    app_->ui()->inform("[success] " + tr_("Fiducials Tool exit."));
}

void FiducialsTool::setPicking(bool on) {
    MainGUI* ui = app_->ui();
    if (on == picking_) return;
    picking_ = on;
    disconnect(conn_);
    if (on)
        conn_ = connect(ui->plotCanvas(), &PlotCanvas::mouseReleased, this,
                        [this](QPointF w, Qt::MouseButton b, bool) { onRelease(w, b); });
    ui->setCanvasHandlersEnabled(!on);
}

}  // namespace flatcam::gui
