#include "gui/tools/DrillingTool.h"

#include <QHeaderView>
#include <algorithm>
#include <cmath>
#include <set>

#include "cam/Preprocessor.h"
#include "gui/App.h"
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
    connect(tools_table, &QTableWidget::itemSelectionChanged, this, [this] { onRowSelectionChange(); });
    connect(generate_cnc_button, &QPushButton::clicked, this, [this] { generateCnc(); });
    connect(pp_excellon_name_cb, &QComboBox::currentTextChanged, this, [this] { onPpChanged(); });
    // form_to_storage: cada campo guarda su valor en las herramientas elegidas (o en todas, si es común).
    for (FCDoubleSpinner* w : {cutz_entry, maxdepth_entry, travelz_entry, feedrate_z_entry, feedrate_rapid_entry,
                               dwelltime_entry, offset_entry, drill_overlap_entry, toolchangez_entry, endz_entry}) {
        connect(w, &QDoubleSpinBox::valueChanged, this, [this, w] { formToStorage(w); });
    }
    connect(spindlespeed_entry, &QSpinBox::valueChanged, this, [this] { formToStorage(spindlespeed_entry); });
    for (FCCheckBox* w : {mpass_cb, dwell_cb, drill_slots_cb, last_drill_cb, toolchange_cb})
        connect(w, &QCheckBox::toggled, this, [this, w] { formToStorage(w); });
    for (FCEntry* w : std::initializer_list<FCEntry*>{estartz_entry, endxy_entry})
        connect(w, &QLineEdit::editingFinished, this, [this, w] { formToStorage(w); });
}

ToolInfo DrillingTool::info() { return {tr_("Drilling Tool"), tr_("Drilling Tool")}; }

DrillingTool::ToolData DrillingTool::defaultToolData() const {
    // tools_drill_* de fábrica (defaults.py), con el tipo de Python de cada valor.
    ToolData d;
    DrillToolSettings& s = d.s;
    s.cutZ = PyNumber(-1.7);
    s.multidepth = false;
    s.depthPerPass = PyNumber(0.7);
    s.travelZ = PyNumber(2);
    s.endZ = PyNumber(0.5);
    s.feedrateZ = PyNumber(300);
    s.spindleSpeed = PyNumber(0);
    s.dwell = false;
    s.dwellTime = PyNumber(1);
    s.toolchangeZ = PyNumber(15);
    s.offsetZ = PyNumber(0.0);
    s.toolchangeXY = std::array<double, 2>{0.0, 0.0};  // "0.0, 0.0"
    s.feedrateRapid = PyNumber(1500);
    s.spindleDir = "CW";
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
    for (const QString& n : preprocessorNames()) {
        pp_excellon_name_cb->addItem(n);
        pp_excellon_name_cb->setItemData(pp_excellon_name_cb->count() - 1, n, Qt::ToolTipRole);
    }
    pp_excellon_name_cb->setValue(pp.isEmpty() ? "default" : pp);
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
    if (excellon_) {
        for (const auto& [id, t] : excellon_->excellon().drillTools()) storage_[id] = defaultToolData();
    }
    exc_param_frame->setDisabled(excellon_ == nullptr);
    buildToolUi();
}

void DrillingTool::buildToolUi() {
    muted_ = true;
    tableOrder_.clear();
    const auto* tools = excellon_ ? &excellon_->excellon().drillTools() : nullptr;
    if (tools) {
        for (const auto& [id, t] : *tools) tableOrder_.push_back(id);
        const QString order = order_radio->value().toString();
        auto byDia = [tools](int a, int b) {
            return std::round(tools->at(a).tooldia * 1e4) < std::round(tools->at(b).tooldia * 1e4);
        };
        if (order == "fwd") std::stable_sort(tableOrder_.begin(), tableOrder_.end(), byDia);
        else if (order == "rev") std::stable_sort(tableOrder_.begin(), tableOrder_.end(), [&](int a, int b) { return byDia(b, a); });
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
        tools_table->setItem(row, 1, item(pyStr(t.tooldia), sel));
        tools_table->setItem(row, 2, item(QString::number(drills), Qt::ItemIsEnabled | Qt::ItemIsDragEnabled));
        tools_table->setItem(row, 3, new QTableWidgetItem(QString::number(tableOrder_[row])));
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
        else if (w == estartz_entry) s.startZ = evalNumber(estartz_entry->text());
        else if (w == endxy_entry) {
            const auto v = NumericExpression::evaluateTuple(endxy_entry->text());
            if (v && v->size() == 2) s.endXY = std::array<double, 2>{(*v)[0], (*v)[1]};
            else s.endXY.reset();
        }
    };
    const bool general = (w == toolchangez_entry || w == endz_entry || w == estartz_entry || w == endxy_entry ||
                          w == toolchange_cb);
    if (general) {
        for (auto& [id, d] : storage_) apply(d);
    } else {
        for (int id : selectedToolIds()) apply(storage_.at(id));
    }
}

FlatCAMObj* DrillingTool::generateCnc() {
    if (!excellon_) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1.").arg(tr_("Object not found")));
        return nullptr;
    }
    bool hasDrills = false;
    for (const auto& [id, t] : excellon_->excellon().drillTools()) hasDrills = hasDrills || !t.drills.empty();
    if (!hasDrills) {
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
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Preprocessor"), pp) +
                           " (todavía no está portado; están default y GRBL_laser)");
        return nullptr;
    }
    // sel_tools: las elegidas, en el orden de la tabla ("Orden de la Herram").
    DrillJobSettings js;
    js.preprocessor = pp.toStdString();
    js.order = DrillOrder::Optimized;  // excellon_optimization_type = B (OR-Tools): el optimizador propio
    js.toolchange = toolchange_cb->isChecked();
    std::vector<CncToolRow> rows;
    for (int id : tableOrder_) {
        if (std::find(sel.begin(), sel.end(), id) == sel.end()) continue;
        js.toolOrder.push_back(id);
        js.tools[id] = storage_.at(id).s;
        const ExcellonTool& t = excellon_->excellon().drillTools().at(id);
        rows.push_back({id, t.tooldia, static_cast<int>(t.drills.size()), static_cast<int>(t.slots.size()),
                        storage_.at(id).s.cutZ.value() - storage_.at(id).s.offsetZ.value()});
    }
    app_->ui()->inform(tr_("Creating a list of points to drill..."));
    auto job = std::make_unique<CncJob>(js);
    if (!job->generateFromExcellon(excellon_->excellon())) {
        app_->ui()->inform("[ERROR_NOTCL] " + QString::fromStdString(job->lastError()));
        return nullptr;
    }
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    h.type = "Geometry";  // así lo imprime la 2024.4 también para el taladrado (archivo real del usuario)
    return app_->addObject(std::make_unique<CNCJobObject>(excellon_->name() + "_cnc", std::move(job), h, std::move(rows)));
}

}  // namespace flatcam::gui
