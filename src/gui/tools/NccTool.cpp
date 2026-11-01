#include "gui/tools/NccTool.h"

#include "gui/App.h"
#include "gui/Translation.h"
#include "gui/tools/ClearingTools.h"
#include "tools/Ncc.h"

namespace flatcam::gui {

NccTool::NccTool(App* app, QWidget* parent) : NccToolUIBase(parent), app_(app) {
    setupUi();
    connect(generate_ncc_button, &QPushButton::clicked, this, [this] { clearCopper(); });
    connect(type_obj_radio, &RadioSet::activatedCustom, this, [this] { onTypeObjChanged(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(ncc_order_radio, &RadioSet::activatedCustom, this, [this] { rebuild(); });
    connect(tools_table, &QTableWidget::itemSelectionChanged, this, [this] {
        if (!muted_) updateUi();
    });
    connect(add_newtool_button, &QPushButton::clicked, this, [this] {
        if (addTool(new_tooldia_entry->value())) app_->ui()->inform("[success] " + tr_("Default tool added to Tool Table."));
    });
    connect(deltool_btn, &QPushButton::clicked, this, [this] {
        const std::vector<int> ids = selectedClearingTools(tools_table);
        if (ids.empty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Delete failed. Select a tool to delete."));
            return;
        }
        for (int id : ids) tools_.erase(id);
        rebuild();
        app_->ui()->inform("[success] " + tr_("Tool(s) deleted from Tool Table."));
    });
    // form_to_storage: lo que se cambia va a las herramientas elegidas.
    for (FCDoubleSpinner* w : {ncc_overlap_entry, ncc_margin_entry, ncc_offset_spinner})
        connect(w, &QDoubleSpinBox::valueChanged, this, [this] { formToStorage(); });
    connect(ncc_method_combo, &QComboBox::currentIndexChanged, this, [this] { formToStorage(); });
    for (FCCheckBox* w : {ncc_connect_cb, ncc_contour_cb, ncc_choice_offset_cb, valid_cb})
        connect(w, &QCheckBox::toggled, this, [this] { formToStorage(); });
    for (RadioSet* w : {op_radio, milling_type_radio})
        connect(w, &RadioSet::activatedCustom, this, [this] { formToStorage(); });
}

ToolInfo NccTool::info() { return {tr_("Non-Copper Clearing"), tr_("NCC Tool")}; }

void NccTool::onTypeObjChanged() {
    const bool gerber = type_obj_radio->value().toString() == "gerber";
    object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == (gerber ? ObjectKind::Gerber : ObjectKind::Geometry)) object_combo->addItem(o->name());
    object_combo->setCurrentIndex(0);
}

void NccTool::setToolUi() {
    muted_ = true;
    FlatCAMObj* active = app_->activeObject();
    const bool geometry = active && active->kind() != ObjectKind::Gerber;
    type_obj_radio->setValue(geometry ? "geometry" : "gerber");
    onTypeObjChanged();
    if (active && (active->kind() == ObjectKind::Gerber || active->kind() == ObjectKind::Geometry))
        object_combo->setValue(active->name());
    // tools_ncc_* de fábrica.
    op_radio->setValue("clear");
    ncc_order_radio->setValue("rev");
    ncc_overlap_entry->setValue(40.0);
    ncc_margin_entry->setValue(1.0);
    ncc_method_combo->setIndexValue(1);  // Semilla
    ncc_connect_cb->setChecked(true);
    ncc_contour_cb->setChecked(true);
    ncc_choice_offset_cb->setChecked(false);
    ncc_offset_spinner->setValue(0.0);
    ncc_rest_cb->setChecked(false);
    rest_ncc_margin_entry->setValue(1.0);
    rest_ncc_connect_cb->setChecked(true);
    rest_ncc_contour_cb->setChecked(true);
    rest_ncc_choice_offset_cb->setChecked(false);
    rest_ncc_offset_spinner->setValue(0.0);
    select_combo->setIndexValue(0);  // Sí mismo
    area_shape_radio->setValue("square");
    valid_cb->setChecked(true);
    milling_type_radio->setValue("cl");
    new_tooldia_entry->setValue(0.1);
    tools_.clear();
    muted_ = false;
    for (double dia : {1.0, 0.5}) addTool(dia);  // tools_ncc_tools = "1.0, 0.5"
}

bool NccTool::addTool(double dia) {
    // on_tool_add → sin base de datos de herramientas → on_tool_default_add.
    const double d = QString::number(dia, 'f', 4).toDouble();
    if (d == 0.0) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Please enter a tool diameter with non-zero value, in Float format."));
        return false;
    }
    for (const auto& [id, t] : tools_) {
        if (QString::number(t.geo.tooldia, 'f', 4).toDouble() == d) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled. Tool already in Tool Table."));
            return false;
        }
    }
    const int uid = tools_.empty() ? 1 : tools_.rbegin()->first + 1;
    ClearingTool t;  // data = default_data: geometry_* y tools_ncc_* de fábrica
    t.geo = defaultGeometryTool();
    t.geo.tooldia = d;
    t.geo.offset = "Path";
    t.geo.type = "Iso";
    t.geo.toolType = "C1";  // tools_ncc_tool_type
    t.overlap = 40.0;
    t.margin = 1.0;
    t.method = 1;
    t.connect = true;
    t.contour = true;
    t.offsetChoice = false;
    t.offset = 0.0;
    t.operation = "clear";
    t.millingType = "cl";
    t.checkValid = true;
    tools_[uid] = t;
    rebuild();
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (tools_table->item(r, 3)->text().toInt() == uid) tools_table->selectRow(r);
    updateUi();
    return true;
}

void NccTool::rebuild() {
    muted_ = true;
    buildClearingTable(tools_table, tools_, ncc_order_radio->value().toString());
    muted_ = false;
}

void NccTool::updateUi() {
    const std::vector<int> ids = selectedClearingTools(tools_table);
    // Color de get_fg_blue(): '#7777FF', porque en el original is_theme_black se usa sin llamarla.
    auto label = [this](const QString& what) {
        tool_data_label->setText(QString("<b>%1: <font color=\"#7777FF\">%2</font></b>").arg(tr_("Parameters for"), what));
    };
    if (ids.empty()) {
        generate_ncc_button->setDisabled(true);
        label(tr_("No Tool Selected"));
        return;
    }
    generate_ncc_button->setDisabled(false);
    if (ids.size() > 1) {
        label(tr_("Multiple Tools"));
        return;
    }
    int row = 0;
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (tools_table->item(r, 3)->text().toInt() == ids.front()) row = r;
    label(QString("%1 %2").arg(tr_("Tool"), tools_table->item(row, 0)->text()));
    const ClearingTool& t = tools_.at(ids.front());
    muted_ = true;
    op_radio->setValue(t.operation);
    ncc_overlap_entry->setValue(t.overlap);
    ncc_margin_entry->setValue(t.margin);
    ncc_method_combo->setIndexValue(t.method);
    ncc_connect_cb->setChecked(t.connect);
    ncc_contour_cb->setChecked(t.contour);
    ncc_choice_offset_cb->setChecked(t.offsetChoice);
    ncc_offset_spinner->setValue(t.offset);
    milling_type_radio->setValue(t.millingType);
    valid_cb->setChecked(t.checkValid);
    muted_ = false;
}

void NccTool::formToStorage() {
    if (muted_) return;
    for (int id : selectedClearingTools(tools_table)) {
        ClearingTool& t = tools_.at(id);
        t.operation = op_radio->value().toString();
        t.overlap = ncc_overlap_entry->value();
        t.margin = ncc_margin_entry->value();
        t.method = ncc_method_combo->indexValue();
        t.connect = ncc_connect_cb->isChecked();
        t.contour = ncc_contour_cb->isChecked();
        t.offsetChoice = ncc_choice_offset_cb->isChecked();
        t.offset = ncc_offset_spinner->value();
        t.millingType = milling_type_radio->value().toString();
        t.checkValid = valid_cb->isChecked();
    }
}

FlatCAMObj* NccTool::clearCopper() {
    const QString name = object_combo->currentText();
    FlatCAMObj* src = app_->collection()->byName(name);
    if (!src) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), name));
        return nullptr;
    }
    const std::vector<int> ids = selectedClearingTools(tools_table);
    if (ids.empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("There are no tools selected in the Tool Table."));
        return nullptr;
    }
    if (select_combo->indexValue() != 0) {
        app_->ui()->inform("[WARNING_NOTCL] Todavía solo está la selección \"Sí mismo\".");
        return nullptr;
    }
    if (ncc_rest_cb->isChecked()) {
        app_->ui()->inform("[WARNING_NOTCL] Todavía no está el mecanizado de resto.");
        return nullptr;
    }
    ShapeList geometry;
    if (src->kind() == ObjectKind::Gerber) {
        geometry = static_cast<GerberObject*>(src)->gerber().solidGeometry();
    } else {
        auto* g = static_cast<GeometryObject*>(src);
        geometry = g->tools().empty() ? g->solidGeometry() : g->tools().begin()->second.solidGeometry;
    }
    // El original calcula en un hilo de trabajo; acá se calcula en el de la interfaz con el indicador de actividad.
    BusyScope busy(app_->ui());
    // gen_clear_area: el margen es el del formulario; el resto, de cada herramienta.
    const double margin = ncc_margin_entry->value();
    auto obj = std::make_unique<GeometryObject>(src->name() + "_ncc");
    for (int id : orderedClearingTools(tools_, ids, ncc_order_radio->value().toString())) {
        const ClearingTool& t = tools_.at(id);
        if (t.operation != "clear" || t.method != 0) {
            app_->ui()->inform("[WARNING_NOTCL] Todavía solo están la operación \"Limpiar\" y el método estándar.");
            return nullptr;
        }
        app_->ui()->inform(QString("[success] %1 = %2%3 %4")
                               .arg(tr_("NCC Tool clearing with tool diameter"), QString::number(t.geo.tooldia), "mm",
                                    tr_("started.")));
        NccOptions o;
        o.toolDia = t.geo.tooldia;
        o.overlap = t.overlap / 100.0;
        o.connect = t.connect;
        o.margin = margin;
        o.hasOffset = t.offsetChoice;
        o.offset = t.offset;
        ShapeList paths;
        std::string error;
        if (!Ncc::clearCopper(geometry, o, paths, error) || paths.empty()) {
            app_->ui()->inform(QString("[WARNING] %1 %2: %3 %4: %5")
                                   .arg(tr_("Could not use the tool for copper clear."), tr_("Tool"))
                                   .arg(id)
                                   .arg(tr_("with diameter"))
                                   .arg(t.geo.tooldia));
            continue;
        }
        GeometryTool gt = t.geo;
        gt.solidGeometry = paths;
        obj->tools()[id] = gt;
        obj->solidGeometry().insert(obj->solidGeometry().end(), paths.begin(), paths.end());
    }
    if (obj->tools().empty()) {
        app_->ui()->inform("[ERROR] " + tr_("There is no NCC Geometry in the file.\n"
                                            "Usually it means that the tool diameter is too big for the painted geometry.\n"
                                            "Change the painting parameters and try again."));
        return nullptr;
    }
    FlatCAMObj* added = app_->addObject(std::move(obj));
    app_->ui()->inform("[success] " + tr_("NCC Tool clear all done."));
    return added;
}

}  // namespace flatcam::gui
