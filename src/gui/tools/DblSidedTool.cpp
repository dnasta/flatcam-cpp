#include "gui/Preferences.h"
#include "gui/tools/DblSidedTool.h"

#include <QRegularExpression>
#include <cmath>
#include <limits>

#include "gui/App.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/GeometryObject.h"
#include "gui/widgets/NumericalEval.h"
#include "tools/DoubleSided.h"

namespace flatcam::gui {

namespace {

constexpr int kDecimals = 4;

/// global_point_clipboard_format: "(%.*f, %.*f)".
QString pointText(double x, double y) {
    return QString("(%1, %2)").arg(QString::number(x, 'f', kDecimals), QString::number(y, 'f', kDecimals));
}

ObjectKind kindOf(const QString& type) {
    if (type == "exc") return ObjectKind::Excellon;
    if (type == "geo") return ObjectKind::Geometry;
    return ObjectKind::Gerber;
}

}  // namespace

DblSidedTool::DblSidedTool(App* app, QWidget* parent) : DblSidedToolUIBase(parent), app_(app) {
    setupUi();
    connect(object_type_radio, &RadioSet::activatedCustom, this,
            [this](const QString& v) { fillCombo(object_combo, v); });
    connect(box_type_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { fillCombo(box_combo, v); });
    // group_toggle_fn: cada cambio de la ubicación del eje muestra los campos que corresponden.
    for (int i = 0; i < 3; ++i)
        connect(axis_location->button(i), &QRadioButton::toggled, this, [this](bool on) {
            if (on) onTogglePointbox();
        });
    connect(point_entry, &QLineEdit::textChanged, this, [this](const QString& v) { align_ref_label_val->setText(v); });
    connect(add_point_button, &QPushButton::clicked, this, [this] {
        const QPointF p = app_->lastClickPosition();
        point_entry->setText(pointText(p.x(), p.y()));
    });
    connect(add_drill_point_button, &QPushButton::clicked, this, [this] {
        const QPointF p = app_->lastClickPosition();
        drillValues_ += pointText(p.x(), p.y()) + ",";
        alignment_holes->setText(drillValues_);
    });
    connect(delete_drill_point_button, &QPushButton::clicked, this, [this] {
        drillValues_ = drillValues_.left(std::max<qsizetype>(0, drillValues_.lastIndexOf('(')));
        alignment_holes->setText(drillValues_);
    });
    connect(mirror_button, &QPushButton::clicked, this, [this] { mirrorObject(); });
    connect(create_alignment_hole_button, &QPushButton::clicked, this, [this] { createAlignmentHoles(); });
    connect(calculate_bb_button, &QPushButton::clicked, this, [this] { onBboxCoordinates(); });
    connect(xmin_btn, &QPushButton::clicked, this, [this] { setPointCoordinate(0, xmin_entry->value()); });
    connect(ymin_btn, &QPushButton::clicked, this, [this] { setPointCoordinate(1, ymin_entry->value()); });
    connect(xmax_btn, &QPushButton::clicked, this, [this] { setPointCoordinate(0, xmax_entry->value()); });
    connect(ymax_btn, &QPushButton::clicked, this, [this] { setPointCoordinate(1, ymax_entry->value()); });
    connect(center_btn, &QPushButton::clicked, this, [this] { point_entry->setText(center_entry->text()); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(pick_hole_button, &QPushButton::clicked, this, [this] {
        // on_pick_hole: el próximo clic en el canvas elige el agujero; el clic derecho cancela.
        pickObject_ = comboObject(exc_combo);
        if (!pickObject_) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Excellon object loaded ..."));
            return;
        }
        stopPicking();
        // Desactiva el ajuste a la grilla (cuesta acertar a un agujero con la grilla). El original no lo vuelve a
        // activar después (guarda grid_status_memory y no lo usa): se reproduce.
        if (app_->ui()->grid_snap_btn->isChecked()) app_->ui()->grid_snap_btn->trigger();
        app_->ui()->inform(tr_("Click on canvas within the desired Excellon drill hole") + ".");
        pickConnection_ = connect(app_->ui()->plotCanvas(), &PlotCanvas::mouseClicked, this,
                                  [this](QPointF w, Qt::MouseButton b) {
                                      if (b == Qt::LeftButton) {
                                          pickHoleAt(w);
                                      } else if (b == Qt::RightButton) {
                                          stopPicking();
                                          app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled by user request."));
                                      }
                                  });
    });
}

ToolInfo DblSidedTool::info() { return {tr_("2-Sided PCB"), tr_("2-Sided Tool")}; }

void DblSidedTool::fillCombo(FCComboBox* combo, const QString& type) {
    combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kindOf(type)) combo->addItem(o->name());
    combo->setCurrentIndex(0);
}

FlatCAMObj* DblSidedTool::comboObject(const FCComboBox* combo) const {
    return combo->currentText().isEmpty() ? nullptr : app_->collection()->byName(combo->currentText());
}

void DblSidedTool::stopPicking() {
    if (pickConnection_) disconnect(pickConnection_);
    pickConnection_ = {};
}

void DblSidedTool::setToolUi() {
    stopPicking();
    drillValues_.clear();
    point_entry->setText("");
    alignment_holes->setText("");
    // tools_2sided_* de las preferencias (de fábrica: X, punto, 3.125, X).
    mirror_axis->setValue(defVariant("tools_2sided_mirror_axis"));
    axis_location->setValue(defVariant("tools_2sided_axis_loc"));
    drill_dia->setValue(defNum("tools_2sided_drilldia"));
    align_axis_radio->setValue(defVariant("tools_2sided_allign_axis"));
    for (FCDoubleSpinner* w : {xmin_entry, ymin_entry, xmax_entry, ymax_entry}) w->setValue(0.0);
    center_entry->setText("");
    align_ref_label_val->setText(QString::number(0.0, 'f', kDecimals));
    object_type_radio->setValue("grb");
    fillCombo(object_combo, "grb");
    box_type_radio->setValue("grb");
    fillCombo(box_combo, "grb");
    fillCombo(exc_combo, "exc");
    onTogglePointbox();
}

void DblSidedTool::onTogglePointbox() {
    const QString v = axis_location->value().toString();
    const bool pt = v == "point", box = v == "box", hole = v == "hole";
    point_entry->setVisible(pt || hole);
    add_point_button->setVisible(pt);
    for (QWidget* w : std::initializer_list<QWidget*>{box_type_label, box_type_radio, box_combo}) w->setVisible(box);
    for (QWidget* w : std::initializer_list<QWidget*>{exc_hole_lbl, exc_combo, pick_hole_button}) w->setVisible(hole);
    if (pt) align_ref_label_val->setText(point_entry->text());
    else if (box) align_ref_label_val->setText("Box centroid");  // así, sin traducir, en el original
}

std::optional<QPointF> DblSidedTool::point() const {
    const auto v = point_entry->evaluated();
    if (!v || v->size() != 2) return std::nullopt;
    return QPointF((*v)[0], (*v)[1]);
}

void DblSidedTool::setPointCoordinate(int index, double value) {
    // on_xmin_clicked y compañía: reemplazan una coordenada del punto (la otra queda, o 0 si no había punto).
    axis_location->setValue("point");
    const QPointF p = point().value_or(QPointF(0.0, 0.0));
    point_entry->setText(index == 0 ? pointText(value, p.y()) : pointText(p.x(), value));
}

void DblSidedTool::onBboxCoordinates() {
    const std::vector<FlatCAMObj*> objs = app_->selectedObjects();
    if (objs.empty()) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1 %2").arg(tr_("Failed."), tr_("No object is selected.")));
        return;
    }
    double xmin = std::numeric_limits<double>::infinity(), ymin = xmin, xmax = -xmin, ymax = -xmin;
    for (FlatCAMObj* o : objs) {
        const QRectF b = o->bounds();
        xmin = std::min(xmin, b.left());
        ymin = std::min(ymin, b.top());
        xmax = std::max(xmax, b.right());
        ymax = std::max(ymax, b.bottom());
    }
    xmin_entry->setValue(xmin);
    ymin_entry->setValue(ymin);
    xmax_entry->setValue(xmax);
    ymax_entry->setValue(ymax);
    const QString c = pointText((xmax - xmin) / 2.0 + xmin, (ymax - ymin) / 2.0 + ymin);
    center_entry->setText(c);
    axis_location->setValue("point");
    point_entry->setText(c);
}

bool DblSidedTool::mirrorObject() {
    FlatCAMObj* obj = comboObject(object_combo);
    if (!obj) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return false;
    }
    QPointF p;
    if (axis_location->value().toString() == "box") {
        FlatCAMObj* box = comboObject(box_combo);
        if (!box) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Box object loaded ..."));
            return false;
        }
        p = box->bounds().center();
    } else {
        const auto pt = point();
        if (!pt) {
            app_->ui()->inform("[WARNING_NOTCL] " +
                               tr_("There are no Point coordinates in the Point field. Add coords and try again ..."));
            return false;
        }
        p = *pt;
    }
    // mirror("X") invierte las Y y mirror("Y") las X: lo mismo que Axis::X y Axis::Y del núcleo.
    const Axis axis = mirror_axis->value().toString() == "X" ? Axis::X : Axis::Y;
    if (!obj->mirror(axis, {p.x(), p.y()})) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Only Gerber, Excellon and Geometry objects can be mirrored."));
        return false;
    }
    app_->objectChanged(obj);
    app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Object was mirrored"), obj->name()));
    return true;
}

FlatCAMObj* DblSidedTool::createAlignmentHoles() {
    QPointF p;
    if (axis_location->value().toString() == "point") {
        const auto pt = point();
        if (!pt) {
            app_->ui()->inform("[WARNING_NOTCL] " +
                               tr_("'Point' reference is selected and 'Point' coordinates are missing. Add them and retry."));
            return nullptr;
        }
        p = *pt;
    } else {
        // El original busca el objeto de la caja con el índice de box_combo pero en el grupo de object_combo; acá se usa
        // el objeto de la caja (con la misma configuración de grupos, es el mismo).
        FlatCAMObj* box = comboObject(box_combo);
        if (!box) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Box reference object loaded. Load one and retry."));
            return nullptr;
        }
        p = box->bounds().center();
    }
    // eval('[{}]'.format(text)): una lista de pares "(x, y)".
    std::vector<Point2D> holes;
    static const QRegularExpression re(R"(\(\s*([^(),]+?)\s*,\s*([^(),]+?)\s*\))");
    for (auto it = re.globalMatch(alignment_holes->text()); it.hasNext();) {
        const auto m = it.next();
        const auto x = NumericExpression::evaluate(m.captured(1)), y = NumericExpression::evaluate(m.captured(2));
        if (x && y) holes.push_back({*x, *y});
    }
    if (holes.empty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There are no Alignment Drill Coordinates to use. Add them and retry."));
        return nullptr;
    }
    const Axis axis = align_axis_radio->value().toString() == "X" ? Axis::X : Axis::Y;
    auto exc = std::make_unique<Excellon>(DoubleSided::alignmentExcellon(holes, axis, {p.x(), p.y()}, drill_dia->value()));
    FlatCAMObj* obj = app_->addObject(std::make_unique<ExcellonObject>(tr_("Alignment Drills"), std::move(exc)));
    drillValues_.clear();
    app_->ui()->inform("[success] " + tr_("Excellon object with alignment drills created..."));
    return obj;
}

bool DblSidedTool::pickHoleAt(const QPointF& world) {
    auto* exc = dynamic_cast<ExcellonObject*>(pickObject_);
    if (!exc) return false;
    const Shape click = Shape::point({world.x(), world.y()});
    for (const auto& [id, t] : exc->excellon().drillTools()) {
        for (const Shape& g : t.solidGeometry) {
            if (!click.within(g)) continue;
            const Point2D c = g.centroid();
            axis_location->setValue("point");
            point_entry->setText(pointText(QString::number(c.x, 'f', kDecimals).toDouble(),
                                           QString::number(c.y, 'f', kDecimals).toDouble()));
            app_->ui()->inform("[success] " + tr_("Mirror reference point set."));
            return true;
        }
    }
    return false;
}

}  // namespace flatcam::gui
