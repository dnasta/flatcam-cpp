#include "gui/tools/PanelizeTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/Panelize.h"

namespace flatcam::gui {

namespace {

ObjectKind kindOfIndex(int i) {
    return i == 1 ? ObjectKind::Excellon : i == 2 ? ObjectKind::Geometry : ObjectKind::Gerber;
}

}  // namespace

PanelizeTool::PanelizeTool(App* app, QWidget* parent) : PanelizeToolUIBase(parent), app_(app) {
    setupUi();
    connect(reference_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { onReferenceChanged(v); });
    connect(panelize_object_button, &QPushButton::clicked, this, [this] { panelize(); });
    connect(type_obj_combo, &QComboBox::currentIndexChanged, this, [this] { onTypeObjChanged(); });
    connect(type_box_combo, &QComboBox::currentIndexChanged, this, [this] { onTypeBoxChanged(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    // PanelizeUI.on_panel_type: "Optimizar" solo para un panel de tipo Geometría.
    connect(panel_type_radio, &RadioSet::activatedCustom, this,
            [this](const QString& v) { optimization_cb->setDisabled(v != "geometry"); });
}

ToolInfo PanelizeTool::info() { return {tr_("Panelize PCB"), tr_("Panel. Tool")}; }

void PanelizeTool::setToolUi() {
    reference_radio->setValue(QString("bbox"));
    spacing_columns->setValue(defNum("tools_panelize_spacing_columns"));
    spacing_rows->setValue(defNum("tools_panelize_spacing_rows"));
    rows->setValue(defInt("tools_panelize_rows"));
    columns->setValue(defInt("tools_panelize_columns"));
    // `x if x else True`: aunque las preferencias digan False queda en True (error del original que se reproduce).
    optimization_cb->setChecked(true);
    constrain_cb->setChecked(defBool("tools_panelize_constrain"));
    x_width_entry->setValue(defNum("tools_panelize_constrainx"));
    y_height_entry->setValue(defNum("tools_panelize_constrainy"));
    const QString pt = defStr("tools_panelize_panel_type").isEmpty() ? "gerber" : defStr("tools_panelize_panel_type");
    panel_type_radio->setValue(pt);
    optimization_cb->setDisabled(pt != "geometry");
    onTypeObjChanged();
    onTypeBoxChanged();
}

void PanelizeTool::onTypeObjChanged() {
    const ObjectKind k = kindOfIndex(type_obj_combo->currentIndex());
    object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == k) object_combo->addItem(o->name());
    object_combo->setCurrentIndex(0);
    // Un Excellon solo puede dar un panel Excellon: el tipo de panel se deshabilita (y queda en Geometría).
    const bool exc = k == ObjectKind::Excellon;
    panel_type_label->setDisabled(exc);
    panel_type_radio->setDisabled(exc);
    if (exc) {
        panel_type_radio->setValue(QString("geometry"));
        optimization_cb->setDisabled(true);
    } else {
        optimization_cb->setDisabled(panel_type_radio->value().toString() != "geometry");
    }
}

void PanelizeTool::onTypeBoxChanged() {
    const ObjectKind k = type_box_combo->currentIndex() == 1 ? ObjectKind::Geometry : ObjectKind::Gerber;
    box_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == k) box_combo->addItem(o->name());
    box_combo->setCurrentIndex(0);
}

void PanelizeTool::onReferenceChanged(const QString& v) {
    const bool obj = v == "object";
    for (QWidget* w : std::initializer_list<QWidget*>{type_box_combo, type_box_combo_label, box_combo}) w->setDisabled(!obj);
}

FlatCAMObj* PanelizeTool::panelize() {
    const QString name = object_combo->currentText();
    app_->deleteSelectionShape();
    FlatCAMObj* src = app_->collection()->byName(name);
    if (!src) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), "None"));
        return nullptr;
    }
    FlatCAMObj* box = app_->collection()->byName(box_combo->currentText());
    if (!box) {
        app_->ui()->inform(QString("[WARNING_NOTCL] %1: %2").arg(tr_("No object Box. Using instead"), src->name()));
        reference_radio->setValue(QString("bbox"));
    }
    if (reference_radio->value().toString() == "bbox") box = src;
    const QString outname = name + "_panelized";
    PanelizeSettings s;
    s.spacingColumns = spacing_columns->value();
    s.spacingRows = spacing_rows->value();
    s.rows = rows->value();
    s.columns = columns->value();
    s.constrainX = x_width_entry->value();
    s.constrainY = y_height_entry->value();
    s.constrain = constrain_cb->isChecked();
    s.optimize = optimization_cb->isChecked();
    const QString panelType = panel_type_radio->value().toString();
    if (s.columns == 0 || s.rows == 0) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Columns or Rows are zero value. Change them to a positive integer."));
        return nullptr;
    }
    const QRectF b = box->bounds();
    const PanelLayout l = panelLayout({b.left(), b.top(), b.right(), b.bottom()}, s);
    app_->ui()->inform(tr_("Generating panel ... "));
    app_->ui()->inform(QString("%1: %2").arg(tr_("Generating panel... Spawning copies")).arg(l.rows * l.columns));
    FlatCAMObj* out = nullptr;
    if (src->kind() == ObjectKind::Excellon) {
        auto exc = panelizeExcellon(static_cast<ExcellonObject*>(src)->excellon(), l);
        out = app_->addObject(std::make_unique<ExcellonObject>(outname, std::move(exc)));
    } else if (panelType == "gerber") {
        ShapeList solid;
        std::map<std::string, Aperture> aps;
        if (src->kind() == ObjectKind::Gerber) {
            const Gerber& g = static_cast<GerberObject*>(src)->gerber();
            panelizeGerber(g, l, solid, aps);
        }
        // Una Geometría como Gerber: el original solo copia las herramientas (que un Gerber no tiene) y el Gerber
        // queda vacío; se reproduce.
        auto g = std::make_unique<Gerber>();
        const Gerber* sg = src->kind() == ObjectKind::Gerber ? &static_cast<GerberObject*>(src)->gerber() : nullptr;
        g->restore(std::move(solid), {}, std::move(aps), sg ? sg->intDigits() : 3, sg ? sg->fracDigits() : 4,
                   std::string());
        out = app_->addObject(std::make_unique<GerberObject>(outname, std::move(g)));
    } else {
        auto geo = std::make_unique<GeometryObject>(outname);
        if (src->kind() == ObjectKind::Geometry) {
            const auto* sgeo = static_cast<GeometryObject*>(src);
            std::map<int, ShapeList> tools;
            for (const auto& [id, t] : sgeo->tools()) tools[id] = t.solidGeometry;
            const auto panel = panelizeGeometryTools(tools, l, s.optimize);
            for (const auto& [id, t] : sgeo->tools()) {
                GeometryTool nt = t;
                nt.solidGeometry = panel.at(id);
                geo->tools()[id] = nt;
                geo->solidGeometry().insert(geo->solidGeometry().end(), nt.solidGeometry.begin(), nt.solidGeometry.end());
            }
        } else {
            // Un Gerber como Geometría: la geometría sólida copiada, en una herramienta como cualquier Geometría nueva.
            const ShapeList solid = panelizeShapes(static_cast<GerberObject*>(src)->gerber().solidGeometry(), l);
            geo->solidGeometry() = solid;
            GeometryTool t = defaultGeometryTool();
            t.solidGeometry = solid;
            geo->tools()[1] = t;
        }
        out = app_->addObject(std::move(geo));
    }
    if (!l.constrained) {
        app_->ui()->inform("[success] " + tr_("Done."));
    } else {
        app_->ui()->inform(tr_("{text} Too big for the constrain area. Final panel has {col} columns and {row} rows")
                               .replace("{text}", "[WARNING] ")
                               .replace("{col}", QString::number(l.columns))
                               .replace("{row}", QString::number(l.rows)));
    }
    app_->ui()->inform("[success] " + tr_("Panel created successfully."));
    return out;
}

}  // namespace flatcam::gui
