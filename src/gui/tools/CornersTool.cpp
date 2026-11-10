#include "gui/tools/CornersTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/CornerMarkers.h"

namespace flatcam::gui {

CornersTool::CornersTool(App* app, QWidget* parent) : CornersToolUIBase(parent), app_(app) {
    setupUi();
    connect(add_marker_button, &QPushButton::clicked, this, [this] { addMarkers(); });
    connect(toggle_all_cb, &QCheckBox::toggled, this, [this](bool v) {
        for (FCCheckBox* c : {bl_cb, br_cb, tl_cb, tr_cb}) c->setChecked(v);
    });
    connect(drill_button, &QPushButton::clicked, this, [this] { createDrills(); });
}

ToolInfo CornersTool::info() { return {tr_("Corner Markers Tool"), tr_("Corners Tool")}; }

void CornersTool::setToolUi() {
    thick_entry->setValue(defNum("tools_corners_thickness"));
    l_entry->setValue(defNum("tools_corners_length"));
    margin_entry->setValue(defNum("tools_corners_margin"));
    toggle_all_cb->setChecked(false);
    type_radio->setValue(defVariant("tools_corners_type"));
    drill_dia_entry->setValue(defNum("tools_corners_drill_dia"));
    const QString cur = object_combo->currentText();
    object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) object_combo->addItem(o->name());
    if (!cur.isEmpty()) object_combo->setValue(cur);
}

FlatCAMObj* CornersTool::gerber() const {
    FlatCAMObj* o = app_->collection()->byName(object_combo->currentText());
    return o && o->kind() == ObjectKind::Gerber ? o : nullptr;
}

FlatCAMObj* CornersTool::addMarkers() {
    FlatCAMObj* src = gerber();
    if (!src) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    const QRectF b = src->bounds();
    const CornerSelection sel{tl_cb->isChecked(), tr_cb->isChecked(), bl_cb->isChecked(), br_cb->isChecked()};
    const ShapeList lines = cornerLines({b.left(), b.top(), b.right(), b.bottom()}, sel,
                                        type_radio->value().toString() == "c", thick_entry->value(), l_entry->value(),
                                        margin_entry->value());
    if (lines.empty()) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1.").arg(tr_("Please select at least a location")));
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed."));
        return nullptr;
    }
    const Gerber& g = static_cast<GerberObject*>(src)->gerber();
    CornersGerber r = addCornerMarkers(g, lines, thick_entry->value(), defInt("gerber_circle_steps", 64));
    auto out = std::make_unique<Gerber>();
    out->restore(std::move(r.solid), std::move(r.follow), std::move(r.apertures), g.intDigits(), g.fracDigits(),
                 std::string());
    FlatCAMObj* obj = app_->addObject(std::make_unique<GerberObject>(src->name() + "_corners", std::move(out)));
    app_->ui()->inform("[success] " + tr_("A Gerber object with corner markers was created."));
    return obj;
}

FlatCAMObj* CornersTool::createDrills() {
    const double dia = drill_dia_entry->value();
    if (dia == 0) {
        app_->ui()->inform(QString("[WARNING_NOTCL] %1 %2").arg(tr_("Cancelled."), tr_("The tool diameter is zero.")));
        return nullptr;
    }
    FlatCAMObj* src = gerber();
    if (!src) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    const CornerSelection sel{tl_cb->isChecked(), tr_cb->isChecked(), bl_cb->isChecked(), br_cb->isChecked()};
    if (!sel.any()) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1.").arg(tr_("Please select at least a location")));
        return nullptr;
    }
    const QRectF b = src->bounds();
    auto exc = std::make_unique<Excellon>();
    exc->addDrillTool(1, dia, cornerDrills({b.left(), b.top(), b.right(), b.bottom()}, sel, thick_entry->value(),
                                           margin_entry->value()));
    exc->createGeometry();
    FlatCAMObj* obj = app_->addObject(std::make_unique<ExcellonObject>(src->name() + "_corner_drills", std::move(exc)));
    app_->ui()->inform("[success] " + tr_("Excellon object with corner drills created."));
    return obj;
}

}  // namespace flatcam::gui
