#include <QTimer>

#include "gui/Preferences.h"
#include "gui/tools/NccTool.h"

#include <QToolButton>

#include "gui/App.h"
#include "gui/CanvasPicker.h"
#include "gui/ToolsDatabase.h"
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
    // FCTable(drag_drop=True): arrastrar filas renumera las herramientas en ese orden (rebuild_ui) y, a los 20 ms,
    // vuelve a armar la tabla (build_ui).
    tools_table->setDragDrop(true);
    connect(tools_table, &FCTable::dragDropped, this, [this] {
        if (auto t = reorderToolsByTable(tools_table, 3, tools_)) tools_ = std::move(*t);
        QTimer::singleShot(20, this, [this] { rebuild(); });
    });
    connect(ncc_rest_cb, &QCheckBox::toggled, this, [this](bool on) { onRestMachiningCheck(on); });
    connect(select_combo, &QComboBox::currentIndexChanged, this, [this] { onToggleReference(); });
    connect(reference_combo_type, &QComboBox::currentIndexChanged, this, [this] { onToggleReference(); });
    connect(tools_table, &QTableWidget::itemSelectionChanged, this, [this] {
        if (!muted_) updateUi();
    });
    connect(add_newtool_button, &QPushButton::clicked, this, [this] {
        // on_tool_add: primero busca el diámetro en la base de datos de herramientas.
        // El original compara tool_target con el texto "NCC", y la base guarda números: no encuentra nunca.
        JsonValue dbTool;
        double dia = 0;
        switch (lookupToolForAdd(app_->ui(), app_->dataPath(), tools_, new_tooldia_entry->value(),
                                 [ncc = tr_("NCC")](const JsonValue& t) { return t.isString() && QString::fromStdString(t.text()) == ncc; }, dbTool, dia)) {
            case DbAddResult::Cancel: return;
            case DbAddResult::Default:
                if (addTool(new_tooldia_entry->value()))
                    app_->ui()->inform("[success] " + tr_("Default tool added to Tool Table."));
                return;
            case DbAddResult::FromDb:
                if (addDbTool(dbTool, dia, "tools_iso"))  // así en el original (copia los tools_iso*, no los tools_ncc*)
                    app_->ui()->inform("[success] " + tr_("New tool added to Tool Table from Tools Database."));
                return;
        }
    });
    // "Agregar desde la base": abre la pestaña; la herramienta que se transfiere entra a la tabla.
    connect(addtool_from_db_btn, &QPushButton::clicked, this, [this] {
        app_->openToolsDatabase([this](const JsonValue& tool) {
            const double target = tool["data"]["tool_target"].toDouble(-1);
            if (target != 0 && target != 5) {  // [General, la de esta herramienta]
                app_->closeToolsDatabase();
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("Selected tool can't be used here. Pick another."));
                return;
            }
            const bool ok = addDbTool(tool, tool["tooldia"].toDouble(), {});
            app_->closeToolsDatabase();
            if (ok) app_->ui()->inform("[success] " + tr_("Tool from DB added in Tool Table."));
        });
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
        connect(w, &QDoubleSpinBox::valueChanged, this, [this, w] { formToStorage(w); });
    connect(ncc_method_combo, &QComboBox::currentIndexChanged, this, [this] { formToStorage(ncc_method_combo); });
    for (FCCheckBox* w : {ncc_connect_cb, ncc_contour_cb, ncc_choice_offset_cb, valid_cb})
        connect(w, &QCheckBox::toggled, this, [this, w] { formToStorage(w); });
    for (RadioSet* w : {op_radio, milling_type_radio})
        connect(w, &RadioSet::activatedCustom, this, [this, w] { formToStorage(w); });
    // on_find_optimal_tooldia: el diámetro seguro pasa al campo de la herramienta nueva.
    connect(find_optimal_button, &QToolButton::clicked, this, [this] { findOptimalToolDia(); });
    // on_operation_change -> NccUI.parameters_ui: con "Aislamiento" solo se edita el tipo de fresado.
    connect(op_radio, &RadioSet::activatedCustom, this, [this](const QString& val) {
        const bool iso = val == "iso";
        milling_type_label->setEnabled(iso);
        milling_type_radio->setEnabled(iso);
        for (QWidget* w : std::initializer_list<QWidget*>{nccoverlabel, ncc_overlap_entry, methodlabel, ncc_method_combo,
                                                          nccmarginlabel, ncc_margin_entry, ncc_connect_cb,
                                                          ncc_contour_cb, ncc_choice_offset_cb, ncc_offset_spinner})
            w->setEnabled(!iso);
    });
}

ToolInfo NccTool::info() { return {tr_("Non-Copper Clearing"), tr_("NCC Tool")}; }

void NccTool::onRestMachiningCheck(bool state) {
    // NccUI.on_rest_machining_check
    if (state) ncc_order_radio->setValue("rev");
    ncc_order_label->setDisabled(state);
    ncc_order_radio->setDisabled(state);
    for (QWidget* w : std::initializer_list<QWidget*>{nccmarginlabel, ncc_margin_entry, ncc_connect_cb, ncc_contour_cb,
                                                      ncc_choice_offset_cb, ncc_offset_spinner})
        w->setVisible(!state);
    for (QWidget* w : std::initializer_list<QWidget*>{rest_nccmarginlabel, rest_ncc_margin_entry, rest_ncc_connect_cb,
                                                      rest_ncc_contour_cb, rest_ncc_choice_offset_cb,
                                                      rest_ncc_offset_spinner})
        w->setVisible(state);
}

void NccTool::onToggleReference() {
    // on_toggle_reference: Sí mismo, Selección de área u Objeto de referencia.
    const int sel = select_combo->indexValue();
    for (QWidget* w : std::initializer_list<QWidget*>{reference_combo, reference_combo_type, reference_combo_type_label})
        w->setVisible(sel == 2);
    area_shape_label->setVisible(sel == 1);
    area_shape_radio->setVisible(sel == 1);
    if (sel != 1) ncc_rest_cb->setDisabled(false);
    const int t = reference_combo_type->indexValue();
    const ObjectKind kind = t == 1 ? ObjectKind::Excellon : t == 2 ? ObjectKind::Geometry : ObjectKind::Gerber;
    reference_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kind) reference_combo->addItem(o->name());
}

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
    // tools_ncc_* de las preferencias (de fábrica: limpiar, 40 %, margen 1, Semilla, "Sí mismo", 1.0 y 0.5).
    op_radio->setValue(defVariant("tools_ncc_operation"));
    ncc_order_radio->setValue(defVariant("tools_ncc_order"));
    ncc_overlap_entry->setValue(defNum("tools_ncc_overlap"));
    ncc_margin_entry->setValue(defNum("tools_ncc_margin"));
    ncc_method_combo->setIndexValue(defInt("tools_ncc_method"));
    ncc_connect_cb->setChecked(defBool("tools_ncc_connect"));
    ncc_contour_cb->setChecked(defBool("tools_ncc_contour"));
    ncc_choice_offset_cb->setChecked(defBool("tools_ncc_offset_choice"));
    ncc_offset_spinner->setValue(defNum("tools_ncc_offset_value"));
    ncc_rest_cb->setChecked(defBool("tools_ncc_rest"));
    rest_ncc_margin_entry->setValue(defNum("tools_ncc_margin"));
    rest_ncc_connect_cb->setChecked(defBool("tools_ncc_connect"));
    rest_ncc_contour_cb->setChecked(defBool("tools_ncc_contour"));
    rest_ncc_choice_offset_cb->setChecked(defBool("tools_ncc_offset_choice"));
    rest_ncc_offset_spinner->setValue(defNum("tools_ncc_offset_value"));
    select_combo->setIndexValue(defInt("tools_ncc_ref"));
    area_shape_radio->setValue(defVariant("tools_ncc_area_shape"));
    valid_cb->setChecked(defBool("tools_ncc_check_valid"));
    milling_type_radio->setValue(defVariant("tools_ncc_milling_type"));
    new_tooldia_entry->setValue(defNum("tools_ncc_newdia"));
    tools_.clear();
    muted_ = false;
    for (double dia : defDias("tools_ncc_tools")) addTool(dia);
    onRestMachiningCheck(defBool("tools_ncc_rest"));
    onToggleReference();
}

bool NccTool::addDbTool(const JsonValue& tool, double dia, const std::string& prefix) {
    // La de fábrica con los datos de la base encima (on_tool_add / on_*_tool_from_db_inserted).
    if (!addTool(dia)) return false;
    const int uid = tools_.rbegin()->first;
    ToolsDatabase::applyTo(tool, tools_.at(uid), prefix);
    tools_.at(uid).geo.tooldia = QString::number(dia, 'f', 4).toDouble();
    rebuild();
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (tools_table->item(r, 3)->text().toInt() == uid) tools_table->selectRow(r);
    updateUi();
    return true;
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
    ClearingTool t;  // data = default_data: geometry_* y tools_ncc_* de las preferencias
    t.geo = defaultGeometryTool();
    t.geo.tooldia = d;
    t.geo.offset = "Path";
    t.geo.type = "Iso";
    t.geo.toolType = defStr("tools_ncc_tool_type");
    t.overlap = defNum("tools_ncc_overlap");
    t.margin = defNum("tools_ncc_margin");
    t.method = defInt("tools_ncc_method");
    t.connect = defBool("tools_ncc_connect");
    t.contour = defBool("tools_ncc_contour");
    t.offsetChoice = defBool("tools_ncc_offset_choice");
    t.offset = defNum("tools_ncc_offset_value");
    t.operation = defStr("tools_ncc_operation");
    t.millingType = defStr("tools_ncc_milling_type");
    t.checkValid = defBool("tools_ncc_check_valid");
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

void NccTool::formToStorage(QObject* w) {
    if (muted_) return;
    for (int id : selectedClearingTools(tools_table)) {
        ClearingTool& t = tools_.at(id);
        if (w == op_radio) t.operation = op_radio->value().toString();
        else if (w == ncc_overlap_entry) t.overlap = ncc_overlap_entry->value();
        else if (w == ncc_margin_entry) t.margin = ncc_margin_entry->value();
        else if (w == ncc_method_combo) t.method = ncc_method_combo->indexValue();
        else if (w == ncc_connect_cb) t.connect = ncc_connect_cb->isChecked();
        else if (w == ncc_contour_cb) t.contour = ncc_contour_cb->isChecked();
        else if (w == ncc_choice_offset_cb) t.offsetChoice = ncc_choice_offset_cb->isChecked();
        else if (w == ncc_offset_spinner) t.offset = ncc_offset_spinner->value();
        else if (w == milling_type_radio) t.millingType = milling_type_radio->value().toString();
        else if (w == valid_cb) t.checkValid = valid_cb->isChecked();
    }
}

void NccTool::findOptimalToolDia() {
    const QString name = object_combo->currentText();
    if (const auto d = findSafeToolDia(app_->ui(), app_->collection()->byName(name), name)) {
        new_tooldia_entry->setValue(*d);
        app_->ui()->inform(QString("[success] %1: %2 %3").arg(tr_("Optimal tool diameter found")).arg(*d).arg("mm"));
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
    if (valid_cb->isChecked()) {
        // find_safe_tooldia_multiprocessing: solo informa.
        if (const auto safe = findSafeToolDia(app_->ui(), src, name)) {
            if (ids.empty()) {
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("There are no tools selected in the Tool Table."));
            } else {
                bool suitable = false;
                for (int id : ids) suitable = suitable || tools_.at(id).geo.tooldia <= *safe;
                if (suitable)
                    app_->ui()->inform("[success] " + tr_("At least one of the selected tools can do a complete isolation."));
                else
                    app_->ui()->inform("[WARNING] " + tr_("Incomplete isolation. None of the selected tools could do a "
                                                          "complete isolation."));
            }
        }
    }
    if (ids.empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("There are no tools selected in the Tool Table."));
        return nullptr;
    }
    ShapeList geometry;
    if (src->kind() == ObjectKind::Gerber) {
        geometry = static_cast<GerberObject*>(src)->gerber().solidGeometry();
    } else {
        auto* g = static_cast<GeometryObject*>(src);
        geometry = g->tools().empty() ? g->solidGeometry() : g->tools().begin()->second.solidGeometry;
    }
    const int sel = select_combo->indexValue();
    const double margin = ncc_margin_entry->value();
    const QString srcName = src->name();
    if (sel == 1) {  // Selección de área
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Click the start point of the area."));
        const auto mode = area_shape_radio->value().toString() == "square" ? CanvasPicker::Mode::Rectangles
                                                                             : CanvasPicker::Mode::Polygons;
        app_->picker()->start(mode, {}, [this, srcName, geometry, ids, margin](const ShapeList& zones) {
            if (FlatCAMObj* s = app_->collection()->byName(srcName))
                clearRegion(s, geometry, ids, Ncc::areaRegion(zones, margin));
        });
        return nullptr;
    }
    if (sel == 2) {  // Objeto de referencia
        FlatCAMObj* ref = app_->collection()->byName(reference_combo->currentText());
        if (!ref) {
            app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Could not retrieve object"),
                                                                   reference_combo->currentText()));
            return nullptr;
        }
        if (auto* g = dynamic_cast<GerberObject*>(ref))
            return clearRegion(src, geometry, ids, Ncc::referenceRegion(geometry, g->gerber().solidGeometry(), true, margin));
        if (auto* gg = dynamic_cast<GeometryObject*>(ref))
            return clearRegion(src, geometry, ids, Ncc::referenceRegion(geometry, gg->solidGeometry(), false, margin));
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("The reference object type is not supported."));
        return nullptr;
    }
    return clearRegion(src, geometry, ids, std::nullopt);
}

FlatCAMObj* NccTool::clearRegion(FlatCAMObj* src, const ShapeList& geometry, const std::vector<int>& ids,
                                 const std::optional<Shape>& region) {
    // El original calcula en un hilo de trabajo; acá se calcula en el de la interfaz con el indicador de actividad.
    BusyScope busy(app_->ui());
    // gen_clear_area: el margen es el del formulario; el resto, de cada herramienta.
    const double margin = ncc_margin_entry->value();
    auto obj = std::make_unique<GeometryObject>(src->name() + "_ncc");
    auto methodOf = [](int m) {
        return m == 1 ? PolygonClearing::Method::Seed
             : m == 2 ? PolygonClearing::Method::Lines
             : m == 3 ? PolygonClearing::Method::Combo
                      : PolygonClearing::Method::Standard;
    };
    // on_ncc_click: las herramientas de "Aislamiento" (iso_dia_list) y las de "Limpiar" (ncc_dia_list), en el orden de
    // la tabla. Hace falta al menos una de limpieza.
    std::vector<int> isoIds, clearIds;
    std::vector<double> isoDias;
    for (int id : ids) {
        if (tools_.at(id).operation == "iso") {
            isoIds.push_back(id);
            isoDias.push_back(tools_.at(id).geo.tooldia);
        } else {
            clearIds.push_back(id);
        }
    }
    if (clearIds.empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("There is no copper clearing tool in the selection "
                                                  "and at least one is needed."));
        return nullptr;
    }
    const bool climb = milling_type_radio->value().toString() == "cl";
    // get_tool_empty_area: el recorrido de cada herramienta de aislamiento (solo con un Gerber).
    int brokenTools = 0;
    auto addIsolation = [&](GeometryObject& o, const NccOptions& opt) {
        if (isoIds.empty() || src->kind() != ObjectKind::Gerber) return;
        const Shape box = Ncc::clearingBox(geometry, opt);
        for (int id : isoIds) {
            bool broken = false;
            ShapeList paths = Ncc::isolationPaths(geometry, tools_.at(id).geo.tooldia, climb, box, 64, broken);
            if (paths.empty()) {
                app_->ui()->inform(QString("[ERROR_NOTCL] %1 %2")
                                       .arg(tr_("Isolation geometry could not be generated."))
                                       .arg(tools_.at(id).geo.tooldia));
                continue;
            }
            if (opt.margin < tools_.at(id).geo.tooldia)
                app_->ui()->inform("[WARNING_NOTCL] " + tr_("Isolation geometry is broken. Margin is less "
                                                            "than isolation tool diameter."));
            if (broken) ++brokenTools;
            GeometryTool gt = tools_.at(id).geo;
            gt.solidGeometry = std::move(paths);
            o.tools()[id] = gt;
        }
    };
    // geo_obj.solid_geometry: la de todas las herramientas, en el orden de sus identificadores.
    auto rebuildSolid = [](GeometryObject& o) {
        o.solidGeometry().clear();
        for (const auto& [id, t] : o.tools())
            o.solidGeometry().insert(o.solidGeometry().end(), t.solidGeometry.begin(), t.solidGeometry.end());
    };
    if (ncc_rest_cb->isChecked()) {
        // gen_clear_area_rest: margen de ncc_margin_entry (así en el original, aunque esté oculto); unir, contorno y
        // agrandado de los campos rest_*; solape y método de cada herramienta.
        NccOptions common;
        common.margin = margin;
        common.region = region;
        common.connect = rest_ncc_connect_cb->isChecked();
        common.contour = rest_ncc_contour_cb->isChecked();
        common.hasOffset = rest_ncc_choice_offset_cb->isChecked();
        common.offset = rest_ncc_offset_spinner->value();
        if (src->kind() == ObjectKind::Gerber) common.isoToolDias = isoDias;
        common.climb = climb;
        std::vector<NccRestTool> rt;
        for (int id : clearIds) {
            const ClearingTool& t = tools_.at(id);
            rt.push_back({t.geo.tooldia, t.overlap / 100.0, methodOf(t.method)});
        }
        std::vector<std::pair<std::size_t, ShapeList>> perTool;
        ShapeList left;
        std::string error;
        if (!Ncc::clearCopperRest(geometry, rt, common, perTool, left, error)) {
            app_->ui()->inform("[ERROR] " + tr_("There is no NCC Geometry in the file.\n"
                                                "Usually it means that the tool diameter is too big for the painted geometry.\n"
                                                "Change the painting parameters and try again."));
            return nullptr;
        }
        for (const auto& [idx, paths] : perTool) {
            GeometryTool gt = tools_.at(clearIds[idx]).geo;
            gt.solidGeometry = paths;
            obj->tools()[clearIds[idx]] = gt;
        }
        addIsolation(*obj, common);
        rebuildSolid(*obj);
        FlatCAMObj* added = app_->addObject(std::move(obj));
        if (brokenTools == 0)
            app_->ui()->inform("[success] " + tr_("NCC Tool Rest Machining clear all done."));
        else
            app_->ui()->inform(QString("[WARNING] %1: %2 %3.")
                                   .arg(tr_("NCC Tool Rest Machining clear all done but the copper features "
                                            "isolation is broken for"))
                                   .arg(brokenTools)
                                   .arg(tr_("tools")));
        return added;
    }
    bool isolationDone = false;
    for (int id : orderedClearingTools(tools_, clearIds, ncc_order_radio->value().toString())) {
        const ClearingTool& t = tools_.at(id);
        app_->ui()->inform(QString("[success] %1 = %2%3 %4")
                               .arg(tr_("NCC Tool clearing with tool diameter"), QString::number(t.geo.tooldia), "mm",
                                    tr_("started.")));
        NccOptions o;
        o.toolDia = t.geo.tooldia;
        o.overlap = t.overlap / 100.0;
        o.connect = t.connect;
        o.margin = margin;
        o.region = region;
        o.hasOffset = t.offsetChoice;
        o.offset = t.offset;
        o.contour = t.contour;
        // ncc_method_combo: Estándar, Semilla, Líneas, Combo.
        o.method = methodOf(t.method);
        if (src->kind() == ObjectKind::Gerber) o.isoToolDias = isoDias;
        o.climb = climb;
        // El original recalcula el aislamiento con cada herramienta de limpieza (da lo mismo): se hace una vez.
        if (!isolationDone) {
            addIsolation(*obj, o);
            isolationDone = true;
        }
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
    }
    rebuildSolid(*obj);
    if (obj->tools().empty()) {
        app_->ui()->inform("[ERROR] " + tr_("There is no NCC Geometry in the file.\n"
                                            "Usually it means that the tool diameter is too big for the painted geometry.\n"
                                            "Change the painting parameters and try again."));
        return nullptr;
    }
    FlatCAMObj* added = app_->addObject(std::move(obj));
    if (brokenTools == 0)
        app_->ui()->inform("[success] " + tr_("NCC Tool clear all done."));
    else
        app_->ui()->inform(QString("[WARNING] %1: %2 %3.")
                               .arg(tr_("NCC Tool clear all done but the copper features isolation is broken for"))
                               .arg(brokenTools)
                               .arg(tr_("tools")));
    return added;
}

}  // namespace flatcam::gui
