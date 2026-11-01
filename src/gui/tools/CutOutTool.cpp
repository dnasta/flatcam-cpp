#include "gui/tools/CutOutTool.h"

#include "gui/App.h"
#include "gui/Translation.h"
#include "gui/objects/GeometryObject.h"
#include "tools/CutOut.h"

namespace flatcam::gui {

CutOutTool::CutOutTool(App* app, QWidget* parent) : CutOutToolUIBase(parent), app_(app) {
    setupUi();
    connect(ff_cutout_object_btn, &QPushButton::clicked, this, [this] { freeformCutout(); });
    connect(rect_cutout_object_btn, &QPushButton::clicked, this, [this] { rectangularCutout(); });
    connect(type_obj_radio, &RadioSet::activatedCustom, this, [this] { onTypeObjChanged(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo CutOutTool::info() { return {tr_("Cutout PCB"), tr_("Cutout Tool")}; }

void CutOutTool::onTypeObjChanged() {
    // on_type_obj_changed: el combo muestra los objetos del grupo elegido, desde el primero.
    const bool gerber = type_obj_radio->value().toString() == "grb";
    obj_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == (gerber ? ObjectKind::Gerber : ObjectKind::Geometry)) obj_combo->addItem(o->name());
    obj_combo->setCurrentIndex(0);
    convex_box_label->setDisabled(!gerber);
    convex_box_cb->setDisabled(!gerber);
}

void CutOutTool::setToolUi() {
    // set_tool_ui: el objeto activo (Gerber o, si no, Geometría) queda elegido.
    FlatCAMObj* active = app_->activeObject();
    const bool gerber = !active || active->kind() == ObjectKind::Gerber;
    type_obj_radio->setValue(gerber ? "grb" : "geo");
    onTypeObjChanged();
    if (active && (active->kind() == ObjectKind::Gerber || active->kind() == ObjectKind::Geometry))
        obj_combo->setValue(active->name());
    // tools_cutout_* de fábrica (update_ui con default_data).
    dia->setValue(2.4);
    obj_kind_combo->setValue("single");
    big_cursor_cb->setChecked(true);
    margin->setValue(0.1);
    gapsize->setValue(4.0);
    gaptype_radio->setValue("b");
    thin_depth_entry->setValue(-1.0);
    mb_dia_entry->setValue(0.6);
    mb_spacing_entry->setValue(0.3);
    convex_box_cb->setChecked(false);
    gaps->setValue("4");
    cutz_entry->setValue(-1.8);
    mpass_cb->setChecked(true);
    maxdepth_entry->setValue(0.6);
}

FlatCAMObj* CutOutTool::freeformCutout() { return cutout(false); }
FlatCAMObj* CutOutTool::rectangularCutout() { return cutout(true); }

FlatCAMObj* CutOutTool::cutout(bool rectangular) {
    const QString name = obj_combo->currentText();
    FlatCAMObj* src = app_->collection()->byName(name);
    if (!src) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("There is no object selected for Cutout.\nSelect one and try again."));
        return nullptr;
    }
    const double d = dia->value();
    if (d == 0.0) {
        app_->ui()->inform("[WARNING_NOTCL] " +
                           tr_("Tool Diameter is zero value. Change it to a positive real number."));
        return nullptr;
    }
    static const std::map<QString, CutoutGaps> kGaps = {
        {"None", CutoutGaps::None}, {"LR", CutoutGaps::LR},       {"TB", CutoutGaps::TB}, {"2LR", CutoutGaps::TwoLR},
        {"2TB", CutoutGaps::TwoTB}, {"4", CutoutGaps::Four},      {"8", CutoutGaps::Eight}};
    const auto g = kGaps.find(gaps->currentText());
    if (g == kGaps.end()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Gaps value can be only one of: 'None', 'lr', 'tb', '2lr', '2tb', 4 or 8.\n"
                                                    "Fill in a correct value and retry."));
        return nullptr;
    }
    CutoutOptions o;
    o.toolDia = d;
    o.margin = margin->value();
    o.gaps = g->second;
    o.gapSize = gapsize->value();
    o.convexBox = convex_box_cb->isChecked();
    o.kind = obj_kind_combo->value().toString() == "panel" ? CutoutKind::Multi : CutoutKind::Single;
    const QString gapType = gaptype_radio->value().toString();
    o.thinBridges = gapType == "bt";
    o.thinDepth = thin_depth_entry->value();
    o.mouseBites = gapType == "mb";
    o.mbDia = mb_dia_entry->value();
    o.mbSpacing = mb_spacing_entry->value();

    ShapeList geometry;
    const bool isGerber = src->kind() == ObjectKind::Gerber;
    if (isGerber) {
        geometry = static_cast<GerberObject*>(src)->gerber().solidGeometry();
    } else {
        auto* geo = static_cast<GeometryObject*>(src);
        if (!geo->tools().empty()) geometry = geo->tools().begin()->second.solidGeometry;  // la primera herramienta
    }
    CutoutResult r;
    std::string error;
    const bool ok = rectangular ? CutOut::rectangular(geometry, isGerber, o, r, error)
                                : CutOut::freeform(geometry, isGerber, o, r, error);
    if (!ok || r.solidGeometry.isEmpty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed.") + (error.empty() ? QString() : " " + QString::fromStdString(error)));
        return nullptr;
    }
    // geo_init: una herramienta con cut_tool_dict (geometry_* de fábrica) y la Z del panel.
    auto obj = std::make_unique<GeometryObject>(src->name() + "_cutout");
    obj->solidGeometry() = {r.solidGeometry};
    GeometryTool t = defaultGeometryTool();
    t.tooldia = d;
    t.data.cutZ = cutz_entry->value();
    t.data.multidepth = mpass_cb->isChecked();
    t.data.depthPerPass = maxdepth_entry->value();
    t.solidGeometry = {r.solidGeometry};
    obj->tools()[1] = t;
    if (!r.gapsSolidGeometry.empty()) {
        GeometryTool thin = t;
        thin.data.cutZ = thin_depth_entry->value();
        thin.overrideColor = colorFromFlatCam("#29a3a3fa");
        thin.solidGeometry = r.gapsSolidGeometry;
        obj->tools()[9999] = thin;
    }
    if (o.mouseBites) {
        app_->ui()->inform("[WARNING_NOTCL] Los agujeros de los \"mouse bites\" todavía no se generan.");
    }
    FlatCAMObj* added = app_->addObject(std::move(obj));
    app_->ui()->inform("[success] " + (rectangular ? tr_("Rectangular CutOut operation finished.")
                                                   : tr_("Any-form Cutout operation finished.")));
    return added;
}

}  // namespace flatcam::gui
