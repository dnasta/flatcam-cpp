#include "gui/Preferences.h"
#include "gui/tools/DrillingTool.h"

#include <QHeaderView>
#include <QTimer>
#include <QRegularExpression>
#include "gui/ToolsDatabase.h"
#include <algorithm>
#include <cmath>
#include <set>

#include "cam/Preprocessor.h"
#include "gui/App.h"
#include "gui/ExclusionAreasManager.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/widgets/NumericalEval.h"

namespace flatcam::gui {

namespace {


QTableWidgetItem* item(const QString& text, Qt::ItemFlags flags) {
    auto* it = new QTableWidgetItem(text);
    it->setFlags(flags);
    return it;
}

/// `str(self.dec_format(x))`: el número redondeado a 4 decimales como lo imprime Python ("0.8", "1.0", "3.4").
QString pyStr(double v) {
    const double r = std::round(v * 1e4) / 1e4;
    QString s = QString::number(r, 'g', 15);
    if (!s.contains('.') && !s.contains('e')) s += ".0";
    return s;
}

/// Valor de un campo de fórmula como lo devuelve `eval` de Python: entero si se escribió sin punto decimal.
std::optional<PyNumber> evalNumber(const QString& text) {
    const auto v = NumericExpression::evaluate(QString(text).replace(',', '.'));
    if (!v) return std::nullopt;
    if (!text.contains('.') && !text.contains(',') && !text.contains('/') && std::floor(*v) == *v)
        return PyNumber(static_cast<int>(*v));
    return PyNumber(*v);
}

}  // namespace

DrillingTool::DrillingTool(App* app, QWidget* parent) : DrillingToolUIBase(parent), app_(app) {
    setupUi();
    connect(object_combo, &QComboBox::currentTextChanged, this, [this] { onObjectChanged(); });
    connect(order_radio, &RadioSet::activatedCustom, this, [this] { buildToolUi(); });
    // FCTable(drag_drop=True): arrastrar filas cambia el orden de las herramientas (drag_drop_sig -> rebuild_ui).
    tools_table->setDragDrop(true);
    connect(tools_table, &FCTable::dragDropped, this, [this] { rebuildUi(); });
    connect(tools_table, &QTableWidget::itemSelectionChanged, this, [this] { onRowSelectionChange(); });
    connect(generate_cnc_button, &QPushButton::clicked, this, [this] { generateCnc(); });
    connect(pp_excellon_name_cb, &QComboBox::currentTextChanged, this, [this] { onPpChanged(); });
    // form_to_storage: cada campo guarda su valor en las herramientas elegidas (o en todas, si es común).
    for (FCDoubleSpinner* w : {cutz_entry, maxdepth_entry, travelz_entry, feedrate_z_entry, feedrate_rapid_entry,
                               dwelltime_entry, offset_entry, drill_overlap_entry, toolchangez_entry, endz_entry,
                               pdepth_entry, feedrate_probe_entry}) {
        connect(w, &QDoubleSpinBox::valueChanged, this, [this, w] { formToStorage(w); });
    }
    connect(spindlespeed_entry, &QSpinBox::valueChanged, this, [this] { formToStorage(spindlespeed_entry); });
    for (FCCheckBox* w : {mpass_cb, dwell_cb, drill_slots_cb, last_drill_cb, toolchange_cb})
        connect(w, &QCheckBox::toggled, this, [this, w] { formToStorage(w); });
    for (FCEntry* w : std::initializer_list<FCEntry*>{estartz_entry, endxy_entry})
        connect(w, &QLineEdit::editingFinished, this, [this, w] { formToStorage(w); });
    // Áreas de exclusión (las de la aplicación: valen para todos los CNC Job).
    app_->excAreas()->bindPanel(exclusion_cb, exclusion_frame, area_shape_radio, strategy_radio, over_z_label,
                                over_z_entry, add_area_button, delete_area_button, delete_sel_area_button,
                                generate_cnc_button, exclusion_table,
                                [this] { return excellon_ ? excellon_->excellon().solidGeometry() : ShapeList{}; },
                                "excellon");
}

ToolInfo DrillingTool::info() { return {tr_("Drilling Tool"), tr_("Drilling Tool")}; }

DrillingTool::ToolData DrillingTool::defaultToolData() const {
    // tools_drill_* de las preferencias, con el tipo de Python de cada valor (de fábrica: -1.7, 0.7, 2, 0.5, 300...).
    ToolData d;
    DrillToolSettings& s = d.s;
    auto py = [](const char* k, PyNumber def) { return defPy(k).value_or(def); };
    s.cutZ = py("tools_drill_cutz", PyNumber(-1.7));
    s.multidepth = defBool("tools_drill_multidepth");
    s.depthPerPass = py("tools_drill_depthperpass", PyNumber(0.7));
    s.travelZ = py("tools_drill_travelz", PyNumber(2));
    s.endZ = py("tools_drill_endz", PyNumber(0.5));
    s.endXY = defPair("tools_drill_endxy");
    s.feedrateZ = py("tools_drill_feedrate_z", PyNumber(300));
    s.spindleSpeed = defPy("tools_drill_spindlespeed");
    s.dwell = defBool("tools_drill_dwell");
    s.dwellTime = py("tools_drill_dwelltime", PyNumber(1));
    s.toolchangeZ = py("tools_drill_toolchangez", PyNumber(15));
    s.offsetZ = py("tools_drill_offset", PyNumber(0.0));
    s.toolchangeXY = defPair("tools_drill_toolchangexy");  // "0.0, 0.0"
    s.startZ = defPy("tools_drill_startz");
    s.feedrateRapid = py("tools_drill_feedrate_rapid", PyNumber(1500));
    s.spindleDir = defStr("tools_drill_spindledir").toStdString();
    s.fPlunge = defBool("tools_drill_f_plunge");
    s.fRetract = defBool("tools_drill_f_retract");
    s.zPdepth = py("tools_drill_z_pdepth", PyNumber(-0.02));
    s.feedrateProbe = py("tools_drill_feedrate_probe", PyNumber(75));
    d.drillSlots = defBool("tools_drill_drill_slots");
    d.drillOverlap = defNum("tools_drill_drill_overlap");
    d.lastDrill = defBool("tools_drill_last_drill");
    return d;
}

void DrillingTool::setToolUi() {
    // Lista de objetos Excellon; si el activo es un Excellon, queda elegido.
    muted_ = true;
    const QString current = object_combo->currentText();
    object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Excellon) object_combo->addItem(o->name());
    FlatCAMObj* active = app_->activeObject();
    if (active && active->kind() == ObjectKind::Excellon) object_combo->setValue(active->name());
    else if (!current.isEmpty()) object_combo->setValue(current);
    // Postprocesadores (todos menos hpgl), con el nombre como tooltip.
    const QString pp = pp_excellon_name_cb->currentText();
    pp_excellon_name_cb->clear();
    for (const QString& n : preprocessorNames(false)) {
        pp_excellon_name_cb->addItem(n);
        pp_excellon_name_cb->setItemData(pp_excellon_name_cb->count() - 1, n, Qt::ToolTipRole);
    }
    pp_excellon_name_cb->setValue(pp.isEmpty() ? defStr("tools_drill_ppname_e") : pp);
    order_radio->setValue(defVariant("tools_drill_tool_order"));
    // Las opciones avanzadas (global_app_level): la Z de inicio y taladrar las ranuras.
    level->setText(appLevelLabel());
    const bool basic = appLevelBasic();
    estartz_label->setVisible(!basic);
    estartz_entry->setVisible(!basic);
    drill_overlap_label->hide();
    drill_overlap_entry->hide();
    last_drill_cb->hide();
    drill_slots_cb->setVisible(!basic);
    drill_slots_cb->setChecked(basic ? false : defBool("tools_drill_drill_slots"));
    muted_ = false;
    onPpChanged();
    onObjectChanged();
}

void DrillingTool::onPpChanged() {
    const QString pp = pp_excellon_name_cb->currentText().toLower();
    const bool probe = pp.contains("toolchange_probe");
    for (QWidget* w : std::initializer_list<QWidget*>{pdepth_entry, pdepth_label, feedrate_probe_entry, feedrate_probe_label})
        w->setVisible(probe);
    const bool marlin = pp.contains("marlin");
    feedrate_rapid_label->setVisible(marlin);
    feedrate_rapid_entry->setVisible(marlin);
}

void DrillingTool::onObjectChanged() {
    if (muted_) return;
    FlatCAMObj* o = app_->collection()->byName(object_combo->currentText());
    excellon_ = (o && o->kind() == ObjectKind::Excellon) ? static_cast<ExcellonObject*>(o) : nullptr;
    storage_.clear();
    baseOrder_.clear();
    if (excellon_) {
        for (const auto& [id, t] : excellon_->excellon().drillTools()) storage_[id] = defaultToolData();
    }
    diaOverride_.clear();
    // excellon_autoload_db ("Cargar automáticamente de la base", nivel Avanzado): on_tool_db_load + replace_tools.
    if (excellon_ && defBool("excellon_autoload_db")) loadToolsFromDb();
    exc_param_frame->setDisabled(excellon_ == nullptr);
    buildToolUi();
}

void DrillingTool::buildToolUi() {
    muted_ = true;
    tableOrder_.clear();
    const auto* tools = excellon_ ? &excellon_->excellon().drillTools() : nullptr;
    if (tools) {
        // build_tool_ui ordena y renumera excellon_tools: el orden que dejó la vez anterior es la base de esta.
        if (baseOrder_.size() == tools->size()) tableOrder_ = baseOrder_;
        else tableOrder_ = excellon_->excellon().toolIdsInOrder();  // el orden del diccionario del original
        const QString order = order_radio->value().toString();
        auto byDia = [tools](int a, int b) {
            return std::round(tools->at(a).tooldia * 1e4) < std::round(tools->at(b).tooldia * 1e4);
        };
        if (order == "fwd") std::stable_sort(tableOrder_.begin(), tableOrder_.end(), byDia);
        else if (order == "rev") std::stable_sort(tableOrder_.begin(), tableOrder_.end(), [&](int a, int b) { return byDia(b, a); });
        baseOrder_ = tableOrder_;
    }
    const int n = static_cast<int>(tableOrder_.size());
    tools_table->setRowCount(n + 2);
    const auto sel = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
    int totDrills = 0, totSlots = 0;
    for (int row = 0; row < n; ++row) {
        const ExcellonTool& t = tools->at(tableOrder_[row]);
        const int drills = static_cast<int>(t.drills.size()), slots = static_cast<int>(t.slots.size());
        totDrills += drills;
        totSlots += slots;
        tools_table->setItem(row, 0, item(QString::number(row + 1), sel));  // se renumeran según el orden
        tools_table->setItem(row, 1, item(pyStr(toolDia(tableOrder_[row])), sel));
        tools_table->setItem(row, 2, item(QString::number(drills), Qt::ItemIsEnabled | Qt::ItemIsDragEnabled));
        // La columna oculta lleva el número renumerado (la fila + 1), como excellon_tools del original.
        tools_table->setItem(row, 3, new QTableWidgetItem(QString::number(row + 1)));
        tools_table->setItem(row, 4, item(slots > 0 ? QString::number(slots) : QString(),
                                          Qt::ItemIsEnabled | Qt::ItemIsDragEnabled));
    }
    QFont bold;
    bold.setBold(true);
    tools_table->setItem(n, 0, item("", Qt::ItemIsEnabled));
    tools_table->setItem(n, 1, item(tr_("Total Drills"), Qt::ItemIsEnabled));
    tools_table->setItem(n, 2, item(QString::number(totDrills), Qt::ItemIsEnabled));
    tools_table->setItem(n, 4, item("", Qt::ItemIsEnabled));
    for (int c : {1, 2}) {
        tools_table->item(n, c)->setForeground(QColor(127, 0, 255));
        tools_table->item(n, c)->setFont(bold);
    }
    tools_table->setItem(n + 1, 0, item("", Qt::ItemIsEnabled));
    tools_table->setItem(n + 1, 1, item(tr_("Total Slots"), Qt::ItemIsEnabled));
    tools_table->setItem(n + 1, 2, item("", Qt::ItemIsEnabled));
    tools_table->setItem(n + 1, 4, item(QString::number(totSlots), Qt::ItemIsEnabled));
    for (int c : {1, 2, 4}) {
        tools_table->item(n + 1, c)->setFont(bold);
        tools_table->item(n + 1, c)->setForeground(QColor(0, 70, 255));
    }
    tools_table->resizeColumnsToContents();
    tools_table->resizeRowsToContents();
    tools_table->verticalHeader()->hide();
    tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tools_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = tools_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tools_table->setSortingEnabled(false);
    tools_table->setMinimumHeight(tools_table->contentHeight());
    tools_table->setMaximumHeight(tools_table->contentHeight());
    muted_ = false;
    // build_tool_ui selecciona todas las filas de herramientas.
    tools_table->clearSelection();
    tools_table->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int row = 0; row < n; ++row) tools_table->selectRow(row);
    tools_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    onRowSelectionChange();
}

void DrillingTool::rebuildUi() {
    // rebuild_ui: el orden de la tabla (las filas de los totales no tienen número) y, después, build_tool_ui.
    std::vector<int> order;
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (const QTableWidgetItem* it = tools_table->item(r, 3)) {
            const int pos = it->text().toInt() - 1;
            if (pos >= 0 && pos < static_cast<int>(tableOrder_.size())) order.push_back(tableOrder_[pos]);
        }
    baseOrder_ = order;
    QTimer::singleShot(20, this, [this] { buildToolUi(); });
}

void DrillingTool::loadToolsFromDb() {
    JsonValue db;
    QString error;
    if (!ToolsDatabase::load(ToolsDatabase::fileName(app_->dataPath()), db, error)) {
        app_->ui()->inform("[ERROR] " + tr_(error.toStdString().c_str()));
        return;
    }
    if (!db.isObject() || db.members().empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Tools DB empty."));
        return;
    }
    // Un número de la base con el tipo de Python (int o float); los textos se evalúan como en el formulario.
    auto num = [](const JsonValue& v) -> std::optional<PyNumber> {
        JsonValue n = v.isString() ? pyEvalLiteral(QString::fromStdString(v.text())) : v;
        if (!n.isNumber()) return std::nullopt;
        return n.isInteger() ? PyNumber(static_cast<int>(n.toDouble())) : PyNumber(n.toDouble());
    };
    auto pair = [](const JsonValue& v) -> std::optional<std::array<double, 2>> {
        if (v.isArray() && v.items().size() == 2) return std::array<double, 2>{v.items()[0].toDouble(), v.items()[1].toDouble()};
        if (v.isString()) {
            const QStringList p = QString::fromStdString(v.text()).remove(QRegularExpression("[()\\[\\]]")).split(',');
            if (p.size() == 2) return std::array<double, 2>{p[0].trimmed().toDouble(), p[1].trimmed().toDouble()};
        }
        return std::nullopt;
    };
    auto apply = [&](ToolData& d, const JsonValue& data) {
        DrillToolSettings& s = d.s;
        for (const auto& [k, v] : data.members()) {
            if (k == "tools_drill_cutz") { if (auto n = num(v)) s.cutZ = *n; }
            else if (k == "tools_drill_multidepth") s.multidepth = v.toBool();
            else if (k == "tools_drill_depthperpass") { if (auto n = num(v)) s.depthPerPass = *n; }
            else if (k == "tools_drill_travelz") { if (auto n = num(v)) s.travelZ = *n; }
            else if (k == "tools_drill_feedrate_z") { if (auto n = num(v)) s.feedrateZ = *n; }
            else if (k == "tools_drill_feedrate_rapid") { if (auto n = num(v)) s.feedrateRapid = *n; }
            else if (k == "tools_drill_spindlespeed") s.spindleSpeed = num(v);
            else if (k == "tools_drill_spindledir") s.spindleDir = v.text();
            else if (k == "tools_drill_dwell") s.dwell = v.toBool();
            else if (k == "tools_drill_dwelltime") { if (auto n = num(v)) s.dwellTime = *n; }
            else if (k == "tools_drill_offset") { if (auto n = num(v)) s.offsetZ = *n; }
            else if (k == "tools_drill_toolchangez") { if (auto n = num(v)) s.toolchangeZ = *n; }
            else if (k == "tools_drill_toolchangexy") s.toolchangeXY = pair(v);
            else if (k == "tools_drill_startz") s.startZ = num(v);
            else if (k == "tools_drill_endz") { if (auto n = num(v)) s.endZ = *n; }
            else if (k == "tools_drill_endxy") s.endXY = pair(v);
            else if (k == "tools_drill_z_pdepth") { if (auto n = num(v)) s.zPdepth = *n; }
            else if (k == "tools_drill_feedrate_probe") { if (auto n = num(v)) s.feedrateProbe = *n; }
            else if (k == "tools_drill_f_plunge") s.fPlunge = v.toBool();
            else if (k == "tools_drill_f_retract") s.fRetract = v.toBool();
            else if (k == "tools_drill_drill_slots") d.drillSlots = v.toBool();
            else if (k == "tools_drill_drill_overlap") d.drillOverlap = v.toDouble();
            else if (k == "tools_drill_last_drill") d.lastDrill = v.toBool();
        }
    };
    // replace_tools: la herramienta de la base con el mismo diámetro, o la que lo tiene dentro de su tolerancia (y
    // entonces el diámetro pasa a ser el de la base). Si a una le corresponden varias, no cambia nada.
    std::map<int, ToolData> next = storage_;
    std::map<int, double> dias;
    for (const auto& [id, t] : excellon_->excellon().drillTools()) {
        int found = 0;
        for (const auto& [key, dbTool] : db.members()) {
            const double dbDia = dbTool["tooldia"].toDouble();
            const JsonValue& data = dbTool["data"];
            const double lo = data["tol_min"].isString() ? QString::fromStdString(data["tol_min"].text()).toDouble()
                                                         : data["tol_min"].toDouble();
            const double hi = data["tol_max"].isString() ? QString::fromStdString(data["tol_max"].text()).toDouble()
                                                         : data["tol_max"].toDouble();
            if (t.tooldia == dbDia) {
                ++found;
                apply(next.at(id), data);
            } else if (hi >= t.tooldia && t.tooldia >= lo) {
                ++found;
                dias[id] = dbDia;
                apply(next.at(id), data);
            }
        }
        if (found > 1) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled.\nMultiple tools for one tool diameter found in Tools "
                                                       "Database."));
            return;
        }
    }
    storage_ = std::move(next);
    diaOverride_ = std::move(dias);
}

double DrillingTool::toolDia(int id) const {
    const auto it = diaOverride_.find(id);
    return it != diaOverride_.end() ? it->second : excellon_->excellon().drillTools().at(id).tooldia;
}

std::vector<int> DrillingTool::selectedToolIds() const {
    std::set<int> rows;
    for (const QModelIndex& i : tools_table->selectionModel()->selectedIndexes()) rows.insert(i.row());
    std::vector<int> ids;
    for (int r : rows)
        if (r < static_cast<int>(tableOrder_.size())) ids.push_back(tableOrder_[static_cast<std::size_t>(r)]);
    return ids;
}

void DrillingTool::onRowSelectionChange() {
    if (muted_) return;
    const auto ids = selectedToolIds();
    const QString pre = "<b>" + tr_("Parameters for") + ": <font color='#0000FF'>";
    if (ids.empty()) {
        tool_data_label->setText(pre + tr_("No Tool Selected") + "</font></b>");
        generate_cnc_button->setDisabled(true);
        return;
    }
    generate_cnc_button->setDisabled(false);
    if (ids.size() > 1) tool_data_label->setText(pre + tr_("Multiple Tools") + "</font></b>");
    else {
        const auto it = std::find(tableOrder_.begin(), tableOrder_.end(), ids.front());
        tool_data_label->setText(pre + tr_("Tool") + QString(" %1</font></b>").arg(it - tableOrder_.begin() + 1));
    }
    storageToForm(storage_.at(ids.front()));
}

void DrillingTool::storageToForm(const ToolData& d) {
    // storage_to_form: los parámetros de la herramienta (los comunes no se tocan).
    muted_ = true;
    const DrillToolSettings& s = d.s;
    cutz_entry->setValue(s.cutZ.value());
    mpass_cb->setChecked(s.multidepth);
    maxdepth_entry->setValue(s.depthPerPass.value());
    travelz_entry->setValue(s.travelZ.value());
    feedrate_z_entry->setValue(s.feedrateZ.value());
    feedrate_rapid_entry->setValue(s.feedrateRapid.value());
    spindlespeed_entry->setValue(s.spindleSpeed ? static_cast<int>(s.spindleSpeed->value()) : 0);
    dwell_cb->setChecked(s.dwell);
    dwelltime_entry->setValue(s.dwellTime.value());
    offset_entry->setValue(s.offsetZ.value());
    drill_slots_cb->setChecked(d.drillSlots);
    drill_overlap_entry->setValue(d.drillOverlap);
    last_drill_cb->setChecked(d.lastDrill);
    muted_ = false;
}

void DrillingTool::formToStorage(QObject* w) {
    if (muted_ || storage_.empty()) return;
    // Lo que se edita en el formulario pasa a ser un número decimal (FCDoubleSpinner) o entero (FCSpinner), como en Python.
    auto apply = [&](ToolData& d) {
        DrillToolSettings& s = d.s;
        if (w == cutz_entry) s.cutZ = PyNumber(cutz_entry->value());
        else if (w == mpass_cb) s.multidepth = mpass_cb->isChecked();
        else if (w == maxdepth_entry) s.depthPerPass = PyNumber(maxdepth_entry->value());
        else if (w == travelz_entry) s.travelZ = PyNumber(travelz_entry->value());
        else if (w == feedrate_z_entry) s.feedrateZ = PyNumber(feedrate_z_entry->value());
        else if (w == feedrate_rapid_entry) s.feedrateRapid = PyNumber(feedrate_rapid_entry->value());
        else if (w == spindlespeed_entry) s.spindleSpeed = PyNumber(spindlespeed_entry->value());
        else if (w == dwell_cb) s.dwell = dwell_cb->isChecked();
        else if (w == dwelltime_entry) s.dwellTime = PyNumber(dwelltime_entry->value());
        else if (w == offset_entry) s.offsetZ = PyNumber(offset_entry->value());
        else if (w == drill_slots_cb) d.drillSlots = drill_slots_cb->isChecked();
        else if (w == drill_overlap_entry) d.drillOverlap = drill_overlap_entry->value();
        else if (w == last_drill_cb) d.lastDrill = last_drill_cb->isChecked();
        // Comunes: van a todas las herramientas.
        else if (w == toolchangez_entry) s.toolchangeZ = PyNumber(toolchangez_entry->value());
        else if (w == endz_entry) s.endZ = PyNumber(endz_entry->value());
        else if (w == pdepth_entry) s.zPdepth = PyNumber(pdepth_entry->value());
        else if (w == feedrate_probe_entry) s.feedrateProbe = PyNumber(feedrate_probe_entry->value());
        else if (w == estartz_entry) s.startZ = evalNumber(estartz_entry->text());
        else if (w == endxy_entry) {
            const auto v = NumericExpression::evaluateTuple(endxy_entry->text());
            if (v && v->size() == 2) s.endXY = std::array<double, 2>{(*v)[0], (*v)[1]};
            else s.endXY.reset();
        }
    };
    const bool general = (w == toolchangez_entry || w == endz_entry || w == estartz_entry || w == endxy_entry ||
                          w == toolchange_cb || w == pdepth_entry || w == feedrate_probe_entry);
    if (general) {
        for (auto& [id, d] : storage_) apply(d);
    } else {
        for (int id : selectedToolIds()) apply(storage_.at(id));
    }
}

JsonValue DrillingTool::drillDataJson(const ToolData& d, const std::string& pp) {
    // excellon_tools[tool]['data'] con las claves tools_drill_* (con el tipo de Python de cada número).
    auto num = [](const PyNumber& n) { return n.isInt() ? JsonValue::number(n.str()) : JsonValue::real(n.value()); };
    auto pair = [](const std::optional<std::array<double, 2>>& p) {
        if (!p) return JsonValue();
        JsonValue a = JsonValue::array();
        a.push(JsonValue::real((*p)[0]));
        a.push(JsonValue::real((*p)[1]));
        return a;
    };
    const DrillToolSettings& s = d.s;
    JsonValue j = JsonValue::object();
    j.set("tools_drill_cutz", num(s.cutZ));
    j.set("tools_drill_multidepth", JsonValue::boolean(s.multidepth));
    j.set("tools_drill_depthperpass", num(s.depthPerPass));
    j.set("tools_drill_travelz", num(s.travelZ));
    j.set("tools_drill_feedrate_z", num(s.feedrateZ));
    j.set("tools_drill_feedrate_rapid", num(s.feedrateRapid));
    j.set("tools_drill_spindlespeed", s.spindleSpeed ? num(*s.spindleSpeed) : JsonValue());
    j.set("tools_drill_spindledir", JsonValue::string(s.spindleDir));
    j.set("tools_drill_dwell", JsonValue::boolean(s.dwell));
    j.set("tools_drill_dwelltime", num(s.dwellTime));
    j.set("tools_drill_offset", num(s.offsetZ));
    j.set("tools_drill_toolchangez", num(s.toolchangeZ));
    j.set("tools_drill_toolchangexy", pair(s.toolchangeXY));
    j.set("tools_drill_startz", s.startZ ? num(*s.startZ) : JsonValue());
    j.set("tools_drill_endz", num(s.endZ));
    j.set("tools_drill_endxy", pair(s.endXY));
    j.set("tools_drill_z_pdepth", num(s.zPdepth));
    j.set("tools_drill_feedrate_probe", num(s.feedrateProbe));
    j.set("tools_drill_f_plunge", JsonValue::boolean(s.fPlunge));
    j.set("tools_drill_f_retract", JsonValue::boolean(s.fRetract));
    j.set("tools_drill_drill_slots", JsonValue::boolean(d.drillSlots));
    j.set("tools_drill_drill_overlap", JsonValue::real(d.drillOverlap));
    j.set("tools_drill_last_drill", JsonValue::boolean(d.lastDrill));
    j.set("tools_drill_ppname_e", JsonValue::string(pp));
    return j;
}

std::vector<Point2D> DrillingTool::slotAsDrills(const Point2D& start, const Point2D& stop, double overlap,
                                               bool addLastPoint) {
    // process_slot_as_drills: el inicio y cada `overlap` sobre la ranura mientras el punto quede dentro de la línea
    // (within con GEOS: un punto interpolado que no cae exacto sobre una ranura diagonal corta la serie, como en el
    // original); con "Último taladro", también el final si está a overlap / 10 o más del anterior.
    std::vector<Point2D> out{start};
    const Shape line = Shape::lineString({start, stop});
    for (int i = 1; overlap > 0; ++i) {
        const Shape p = line.interpolate(overlap * i);
        if (!p.within(line)) break;
        const auto b = p.bounds();
        out.push_back({b->xmin, b->ymin});
    }
    if (addLastPoint && std::hypot(stop.x - out.back().x, stop.y - out.back().y) >= overlap / 10) out.push_back(stop);
    return out;
}

FlatCAMObj* DrillingTool::generateCnc() {
    if (!excellon_) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1.").arg(tr_("Object not found")));
        return nullptr;
    }
    // is_valid_excellon: sin agujeros vale si hay ranuras y "Taladrar ranuras" está marcado.
    bool hasDrills = false, hasSlots = false;
    for (const auto& [id, t] : excellon_->excellon().drillTools()) {
        hasDrills = hasDrills || !t.drills.empty();
        hasSlots = hasSlots || !t.slots.empty();
    }
    if (!hasDrills && !(drill_slots_cb->isChecked() && hasSlots)) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1...").arg(tr_("The loaded Excellon file has no drills")));
        return nullptr;
    }
    std::vector<int> sel = selectedToolIds();
    if (sel.empty()) {
        if (tools_table->rowCount() >= 3) sel.push_back(tableOrder_.front());
        else {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Please select one or more tools from the list and try again."));
            return nullptr;
        }
    }
    const QString pp = pp_excellon_name_cb->currentText();
    if (!makePreprocessor(pp.toStdString())) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Preprocessor"), pp));
        return nullptr;
    }
    // sel_tools: las elegidas, en el orden de la tabla ("Orden de la Herram").
    DrillJobSettings js;
    js.preprocessor = pp.toStdString();
    js.order = DrillOrder::Optimized;  // excellon_optimization_type = B (OR-Tools): el optimizador propio
    js.toolchange = toolchange_cb->isChecked();
    js.units = appUnits().toStdString();
    js.coordsDecimals = defInt("cncjob_coords_decimals", 4);
    js.frDecimals = defInt("cncjob_fr_decimals", 2);
    js.decimals = appDecimals();
    std::vector<CncToolRow> rows;
    // excellon_tools está renumerado en el orden de la tabla: el número T de cada herramienta es su fila + 1.
    Excellon renumbered = excellon_->excellon().renumbered(tableOrder_);
    for (std::size_t pos = 0; pos < tableOrder_.size(); ++pos) {
        const int id = tableOrder_[pos];
        if (std::find(sel.begin(), sel.end(), id) == sel.end()) continue;
        const int newId = static_cast<int>(pos) + 1;
        js.toolOrder.push_back(newId);
        js.tools[newId] = storage_.at(id).s;
        const ExcellonTool& t = excellon_->excellon().drillTools().at(id);
        CncToolRow row;
        row.tool = newId;
        row.dia = toolDia(id);
        if (diaOverride_.count(id)) renumbered.setToolDiameter(newId, row.dia);
        // create_drill_points: con "Taladrar ranuras" (de la herramienta) cada ranura pasa a ser una fila de agujeros.
        const ToolData& d = storage_.at(id);
        std::vector<Point2D> converted;
        if (d.drillSlots) {
            const double step = (1 - drill_overlap_entry->value() / 100.0) * toolDia(id);
            for (const auto& slot : t.slots) {
                const auto pts = slotAsDrills(slot.first, slot.second, step, last_drill_cb->isChecked());
                converted.insert(converted.end(), pts.begin(), pts.end());
            }
            renumbered.appendDrills(newId, converted);
        }
        row.drills = static_cast<int>(t.drills.size() + converted.size());
        row.slots = d.drillSlots ? 0 : static_cast<int>(t.slots.size());
        row.cutZ = storage_.at(id).s.cutZ.value() - storage_.at(id).s.offsetZ.value();
        row.data = drillDataJson(storage_.at(id), pp_excellon_name_cb->currentText().toStdString());
        rows.push_back(std::move(row));
    }
    app_->ui()->inform(tr_("Creating a list of points to drill..."));
    // check_intersection: ningún agujero (con su radio) puede tocar un área de exclusión.
    js.exclusionAreas = app_->excAreas()->areas();
    for (int id : js.toolOrder) {
        const ExcellonTool& t = renumbered.drillTools().at(id);
        for (const Point2D& pt : t.drills) {
            const Shape buf = Shape::point(pt).buffer(t.tooldia / 2.0);
            for (const ExclusionArea& a : js.exclusionAreas) {
                if (buf.within(a.shape) || buf.intersects(a.shape)) {
                    app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed. Drill points inside the exclusion zones."));
                    return nullptr;
                }
            }
        }
    }
    auto job = std::make_unique<CncJob>(js);
    if (!job->generateFromExcellon(renumbered)) {
        app_->ui()->inform("[ERROR_NOTCL] " + QString::fromStdString(job->lastError()));
        return nullptr;
    }
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    h.units = appUnits().toStdString();
    h.type = "Geometry";  // así lo imprime la 2024.4 también para el taladrado (archivo real del usuario)
    return app_->addObject(std::make_unique<CNCJobObject>(excellon_->name() + "_cnc", std::move(job), h, std::move(rows)));
}

}  // namespace flatcam::gui
