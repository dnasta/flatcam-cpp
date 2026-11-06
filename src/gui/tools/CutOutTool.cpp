#include "gui/tools/CutOutTool.h"

#include "gui/App.h"
#include "gui/Translation.h"
#include "gui/tools/ClearingTools.h"
#include "gui/ProjectIO.h"
#include "gui/ToolsDatabase.h"
#include "gui/objects/GeometryObject.h"
#include "tools/CutOut.h"

namespace flatcam::gui {

CutOutTool::CutOutTool(App* app, QWidget* parent) : CutOutToolUIBase(parent), app_(app) {
    setupUi();
    connect(ff_cutout_object_btn, &QPushButton::clicked, this, [this] { freeformCutout(); });
    connect(rect_cutout_object_btn, &QPushButton::clicked, this, [this] { rectangularCutout(); });
    // "Buscar y agregar" (on_tool_add): busca en la base comparando tool_target con el texto "Cutout" (la base guarda
    // números: no encuentra nunca) y deja la herramienta de fábrica (on_tool_default_add: los parámetros del corte de
    // fábrica; el diámetro queda el escrito).
    connect(add_newtool_button, &QPushButton::clicked, this, [this] {
        JsonValue dbTool;
        double dia = 0;
        const QString cut = tr_("Cutout");
        const auto r = lookupToolForAdd(app_->ui(), app_->dataPath(), {}, dia_value(),
                                        [cut](const JsonValue& t) { return t.isString() && QString::fromStdString(t.text()) == cut; },
                                        dbTool, dia);
        if (r == DbAddResult::Cancel) return;
        if (r == DbAddResult::FromDb) {
            applyCutoutData(dbTool["data"]);
            this->dia->setValue(dia);
            app_->ui()->inform("[success] " + tr_("New tool added to Tool Table from Tools Database."));
            return;
        }
        applyCutoutData(ProjectIO::factoryDefaults());
        app_->ui()->inform("[success] " + tr_("Default tool added."));
    });
    // "Agregar desde la base" (on_cutout_tool_add_from_db_executed).
    connect(addtool_from_db_btn, &QPushButton::clicked, this, [this] {
        app_->openToolsDatabase([this](const JsonValue& tool) {
            const double target = tool["data"]["tool_target"].toDouble(-1);
            if (target != 0 && target != 6) {  // [General, Corte]
                app_->closeToolsDatabase();
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("Selected tool can't be used here. Pick another."));
                return;
            }
            JsonValue data = tool["data"];
            data.set("tools_cutout_tooldia", tool["tooldia"]);
            data.set("tools_cutout_z", data["cutz"]);
            data.set("tools_cutout_mdepth", data["multidepth"]);
            data.set("tools_cutout_depthperpass", data["depthperpass"]);
            applyCutoutData(data);
            this->dia->setValue(tool["tooldia"].toDouble());
            app_->closeToolsDatabase();
            app_->ui()->inform("[success] " + tr_("Tool updated from Tools Database."));
        });
    });
    connect(type_obj_radio, &RadioSet::activatedCustom, this, [this] { onTypeObjChanged(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

double CutOutTool::dia_value() const { return dia->value(); }

void CutOutTool::applyCutoutData(const JsonValue& d) {
    // update_ui(tool_dict): los parámetros del corte.
    auto num = [&](const char* k, double def) {
        const JsonValue& v = d[k];
        return v.isNumber() || v.isBool() ? v.toDouble(def) : v.isString() ? QString::fromStdString(v.text()).toDouble() : def;
    };
    margin->setValue(num("tools_cutout_margin", margin->value()));
    gapsize->setValue(num("tools_cutout_gapsize", gapsize->value()));
    if (d["tools_cutout_gap_type"].isString()) gaptype_radio->setValue(QString::fromStdString(d["tools_cutout_gap_type"].text()));
    thin_depth_entry->setValue(num("tools_cutout_gap_depth", thin_depth_entry->value()));
    mb_dia_entry->setValue(num("tools_cutout_mb_dia", mb_dia_entry->value()));
    mb_spacing_entry->setValue(num("tools_cutout_mb_spacing", mb_spacing_entry->value()));
    if (!d["tools_cutout_convexshape"].isNull()) convex_box_cb->setChecked(d["tools_cutout_convexshape"].toBool());
    if (!d["tools_cutout_gaps_ff"].isNull()) gaps->setValue(QString::fromStdString(d["tools_cutout_gaps_ff"].text()));
    cutz_entry->setValue(num("tools_cutout_z", cutz_entry->value()));
    if (!d["tools_cutout_mdepth"].isNull()) mpass_cb->setChecked(d["tools_cutout_mdepth"].toBool());
    maxdepth_entry->setValue(num("tools_cutout_depthperpass", maxdepth_entry->value()));
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
    // excellon_init (puentes con "mouse bites"): un Excellon `<objeto>_mouse_bites` con los agujeros, antes que la
    // Geometría, como el original.
    if (o.mouseBites) {
        if (r.mouseBiteHoles.empty()) {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Mouse bites failed."));
        } else {
            auto exc = std::make_unique<Excellon>();
            exc->addDrillTool(1, o.mbDia, r.mouseBiteHoles);
            exc->createGeometry();
            app_->addObject(std::make_unique<ExcellonObject>(src->name() + "_mouse_bites", std::move(exc)));
        }
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
    FlatCAMObj* added = app_->addObject(std::move(obj));
    app_->ui()->inform("[success] " + (rectangular ? tr_("Rectangular CutOut operation finished.")
                                                   : tr_("Any-form Cutout operation finished.")));
    return added;
}

}  // namespace flatcam::gui
