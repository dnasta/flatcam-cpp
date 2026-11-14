#include "gui/objects/ObjectUIs.h"
#include "gui/Preferences.h"
#include <set>

#include <QHeaderView>
#include <algorithm>
#include <cmath>

#include "gui/Translation.h"
#include "gui/tools/ClearingTools.h"
#include "gui/ToolsDatabase.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "gui/tools/ToolInfo.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCCheckBox.h"

namespace flatcam::gui {

namespace {

constexpr int kDecimals = 4;  // App.decimals

QTableWidgetItem* item(const QString& text, Qt::ItemFlags flags) {
    auto* it = new QTableWidgetItem(text);
    it->setFlags(flags);
    return it;
}

QString fmt(double v) { return QString::number(v, 'f', kDecimals); }

}  // namespace

GerberObjectUI::GerberObjectUI(QWidget* parent) : GerberObjectUIBase(parent) {
    setupUi();
    connect(aperture_table_visibility_cb, &QCheckBox::toggled, this, [this](bool on) {
        // on_aperture_table_visibility_change: muestra la tabla (y la arma la primera vez).
        apertures_table->setVisible(on);
        if (on && obj_) buildAperturesTable(*obj_);
    });
}

void GerberObjectUI::build(GerberObject& obj) {
    obj_ = &obj;
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    apertures_table->setVisible(aperture_table_visibility_cb->isChecked());
    if (aperture_table_visibility_cb->isChecked()) buildAperturesTable(obj);
}

void GerberObjectUI::buildAperturesTable(GerberObject& obj) {
    // GerberObject.build_ui: aperturas ordenadas por código numérico.
    std::vector<std::pair<int, const Aperture*>> sorted;
    for (const auto& [code, ap] : obj.gerber().apertures()) sorted.push_back({std::stoi(code), &ap});
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    apertures_table->setRowCount(static_cast<int>(sorted.size()));
    const auto sel = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    int row = 0;
    for (const auto& [code, ap] : sorted) {
        QString dim;
        if (ap->type == "R" || ap->type == "O") dim = fmt(ap->width) + ", " + fmt(ap->height);
        else if (ap->type == "P") dim = fmt(ap->diam) + ", " + fmt(ap->nVertices);
        apertures_table->setItem(row, 0, item(QString::number(row + 1), sel));
        apertures_table->setItem(row, 1, item(QString::number(code), Qt::ItemIsEnabled));
        apertures_table->setItem(row, 2, item(QString::fromStdString(ap->type), Qt::ItemIsEnabled));
        apertures_table->setItem(row, 3, item(ap->size ? fmt(*ap->size) : QString(), Qt::ItemIsEnabled));
        apertures_table->setItem(row, 4, item(dim, Qt::ItemIsEnabled));
        apertures_table->setItem(row, 5, item("", Qt::ItemIsEnabled));
        auto* mark = new FCCheckBox();
        mark->setLayoutDirection(Qt::RightToLeft);
        apertures_table->setCellWidget(row, 5, mark);
        ++row;
    }
    apertures_table->selectColumn(0);
    apertures_table->resizeColumnsToContents();
    apertures_table->resizeRowsToContents();
    apertures_table->verticalHeader()->hide();
    apertures_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = apertures_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 27);
    for (int c : {1, 2, 3}) h->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::Stretch);
    h->setSectionResizeMode(5, QHeaderView::Fixed);
    h->resizeSection(5, 17);
    apertures_table->setColumnWidth(5, 17);
    apertures_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    apertures_table->setSortingEnabled(false);
    apertures_table->setMinimumHeight(apertures_table->contentHeight());
    apertures_table->setMaximumHeight(apertures_table->contentHeight());
}

ExcellonObjectUI::ExcellonObjectUI(QWidget* parent) : ExcellonObjectUIBase(parent) {
    setupUi();
    // to_form con las opciones del objeto (excellon_tooldia y excellon_slot_tooldia de las preferencias).
    tooldia_entry->setValue(defNum("excellon_tooldia"));
    slot_tooldia_entry->setValue(defNum("excellon_slot_tooldia"));
    connect(generate_milling_button, &QPushButton::clicked, this, [this] {
        if (onMill) onMill(false);
    });
    connect(generate_milling_slots_button, &QPushButton::clicked, this, [this] {
        if (onMill) onMill(true);
    });
    connect(tools_table, &QTableWidget::clicked, this, [this] { onRowSelectionChange(); });
}

std::vector<int> ExcellonObjectUI::selectedTools() const {
    // rows = set(...): las filas en orden creciente; las de los totales no se pueden elegir.
    std::set<int> rows;
    for (QTableWidgetItem* it : tools_table->selectedItems()) rows.insert(it->row());
    std::vector<int> ids;
    for (int r : rows) ids.push_back(tools_table->item(r, 0)->text().toInt());
    return ids;
}

void ExcellonObjectUI::onRowSelectionChange() {
    // on_row_selection_change: los botones de fresado según las herramientas elegidas tengan agujeros o ranuras.
    if (!obj_) return;
    std::set<int> rows;
    for (const QModelIndex& i : tools_table->selectionModel()->selectedIndexes()) rows.insert(i.row());
    const bool any = !rows.empty();
    bool drills = any, slots = any;
    for (int r : rows) {
        const QTableWidgetItem* it = tools_table->item(r, 1);
        if (!it) continue;
        const double rowDia = std::round(it->text().toDouble() * 1e4) / 1e4;  // dec_format
        for (const auto& [id, t] : obj_->excellon().drillTools()) {
            if (std::round(t.tooldia * 1e4) / 1e4 != rowDia) continue;
            if (t.drills.empty()) drills = false;
            if (t.slots.empty()) slots = false;
        }
    }
    tooldia_entry->setDisabled(!drills);
    generate_milling_button->setDisabled(!drills);
    slot_tooldia_entry->setDisabled(!slots);
    generate_milling_slots_button->setDisabled(!slots);
}

void ExcellonObjectUI::build(ExcellonObject& obj) {
    obj_ = &obj;
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    const auto& tools = obj.excellon().drillTools();
    std::vector<std::pair<int, double>> sorted;
    for (const auto& [id, t] : tools) sorted.push_back({id, t.tooldia});
    std::stable_sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    tools_table->setRowCount(static_cast<int>(sorted.size()) + 2);
    const auto sel = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    int row = 0, totDrills = 0, totSlots = 0;
    bool hasDrills = false, hasSlots = false;
    const QColor fill = colorFromFlatCam("#C40000BF");  // excellon_plot_fill
    for (const auto& [id, dia] : sorted) {
        const ExcellonTool& t = tools.at(id);
        const int drills = static_cast<int>(t.drills.size()), slots = static_cast<int>(t.slots.size());
        totDrills += drills;
        totSlots += slots;
        hasDrills = hasDrills || drills > 0;
        hasSlots = hasSlots || slots > 0;
        tools_table->setItem(row, 0, item(QString::number(id), sel));
        tools_table->setItem(row, 1, item(fmt(dia), sel));
        tools_table->setItem(row, 2, item(QString::number(drills), Qt::ItemIsEnabled));
        tools_table->setItem(row, 3, item(slots > 0 ? QString::number(slots) : QString(), Qt::ItemIsEnabled));
        auto* color = item("", Qt::NoItemFlags);
        color->setBackground(fill);
        tools_table->setItem(row, 4, color);
        auto* plot = new FCCheckBox();
        plot->setLayoutDirection(Qt::RightToLeft);
        plot->setChecked(plot_cb->isChecked());
        tools_table->setCellWidget(row, 5, plot);
        ++row;
    }
    // Filas de totales, en negrita y de color.
    QFont bold;
    bold.setBold(true);
    auto totals = [&](int r, const QString& label, int col, int count, const QColor& color) {
        for (int c = 0; c < 6; ++c) tools_table->setItem(r, c, item("", Qt::NoItemFlags));
        tools_table->setItem(r, 1, item(label, Qt::ItemIsEnabled));
        tools_table->setItem(r, col, item(QString::number(count), Qt::ItemIsEnabled));
        for (int c : {1, col}) {
            tools_table->item(r, c)->setForeground(color);
            tools_table->item(r, c)->setFont(bold);
        }
    };
    totals(row, tr_("Total Drills"), 2, totDrills, QColor(127, 0, 255));
    totals(row + 1, tr_("Total Slots"), 3, totSlots, QColor(0, 70, 255));
    tools_table->item(row + 1, 2)->setFont(bold);

    tools_table->selectColumn(0);
    tools_table->resizeColumnsToContents();
    tools_table->resizeRowsToContents();
    tools_table->verticalHeader()->hide();
    tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = tools_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::Fixed);
    h->resizeSection(4, 17);
    h->setSectionResizeMode(5, QHeaderView::Fixed);
    h->resizeSection(5, 17);
    tools_table->setColumnWidth(5, 17);
    tools_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tools_table->setSortingEnabled(false);
    tools_table->setMinimumHeight(tools_table->contentHeight());
    tools_table->setMaximumHeight(tools_table->contentHeight());
    tooldia_entry->setDisabled(!hasDrills);
    generate_milling_button->setDisabled(!hasDrills);
    slot_tooldia_entry->setDisabled(!hasSlots);
    generate_milling_slots_button->setDisabled(!hasSlots);
    onRowSelectionChange();  // "update the milling section"
}

GeometryObjectUI::GeometryObjectUI(QWidget* parent) : GeometryObjectUIBase(parent) {
    setupUi();
    // set_ui: los postprocesadores, con el nombre como tooltip; "default" es geometry_ppname_g.
    for (const QString& n : preprocessorNames()) {
        pp_geometry_name_cb->addItem(n);
        pp_geometry_name_cb->setItemData(pp_geometry_name_cb->count() - 1, n, Qt::ToolTipRole);
    }
    pp_geometry_name_cb->setValue("default");
    connect(search_and_add_btn, &QPushButton::clicked, this, [this] { addTool(addtool_entry->value()); });
    connect(deltool_btn, &QPushButton::clicked, this, [this] { deleteTools(); });
    connect(addtool_from_db_btn, &QPushButton::clicked, this, [this] {
        if (onAddFromDb) onAddFromDb();
    });
    connect(generate_cnc_button, &QPushButton::clicked, this, [this] {
        if (onGenerateCnc) onGenerateCnc();
    });
    // ois_polish: "Agregar acabado" muestra sus parámetros.
    const std::vector<QWidget*> polish = {polish_dia_lbl,      polish_dia_entry,     polish_pressure_lbl,
                                                    polish_pressure_entry, polish_travelz_lbl, polish_travelz_entry,
                                                    polish_margin_lbl,   polish_margin_entry,  polish_over_lbl,
                                                    polish_over_entry,   polish_method_lbl,    polish_method_combo};
    for (QWidget* w : polish) w->setVisible(polish_cb->isChecked());
    connect(polish_cb, &QCheckBox::toggled, this, [polish](bool on) {
        for (QWidget* w : polish) w->setVisible(on);
    });
    connect(geo_tools_table, &QTableWidget::itemSelectionChanged, this, [this] {
        if (!muted_) updateUi();
    });
    // ui_connect: cada campo del formulario guarda en la herramienta de la fila actual.
    for (FCDoubleSpinner* w : {cutz_entry, maxdepth_entry, travelz_entry, cncfeedrate_entry, feedrate_z_entry,
                               feedrate_rapid_entry, e_cut_entry, dwelltime_entry})
        connect(w, &QDoubleSpinBox::valueChanged, this, [this] { formToStorage(); });
    connect(cncspindlespeed_entry, &QSpinBox::valueChanged, this, [this] { formToStorage(); });
    for (FCCheckBox* w : {mpass_cb, dwell_cb, extracut_cb})
        connect(w, &QCheckBox::toggled, this, [this] { formToStorage(); });
    connect(extracut_cb, &QCheckBox::toggled, this, [this](bool on) { e_cut_entry->setDisabled(!on); });
    // update_common_param_in_storage
    connect(toolchangeg_cb, &QCheckBox::toggled, this, [this] { commonToStorage(); });
    for (FCDoubleSpinner* w : {toolchangez_entry, endz_entry})
        connect(w, &QDoubleSpinBox::valueChanged, this, [this] { commonToStorage(); });
    connect(endxy_entry, &QLineEdit::editingFinished, this, [this] { commonToStorage(); });
    connect(pp_geometry_name_cb, &QComboBox::currentTextChanged, this, [this] {
        commonToStorage();
        onPpChanged(muted_);
    });
}

int GeometryObjectUI::toolAt(int row) const {
    const QTableWidgetItem* it = geo_tools_table->item(row, 5);
    return it ? it->text().toInt() : -1;
}

std::vector<int> GeometryObjectUI::selectedRows() const {
    std::vector<int> rows;
    for (const QTableWidgetItem* it : geo_tools_table->selectedItems())
        if (std::find(rows.begin(), rows.end(), it->row()) == rows.end()) rows.push_back(it->row());
    std::sort(rows.begin(), rows.end());
    return rows;
}

std::vector<int> GeometryObjectUI::toolsForCnc() const {
    std::vector<int> ids;
    for (int r : selectedRows()) ids.push_back(toolAt(r));
    if (ids.empty() && geo_tools_table->rowCount() == 1) ids.push_back(toolAt(0));
    return ids;
}

void GeometryObjectUI::build(GeometryObject& obj) {
    obj_ = &obj;
    muted_ = true;
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    auto& tools = obj.tools();
    geo_tools_table->setRowCount(static_cast<int>(tools.size()));
    int row = 0;
    auto combo = [](const QStringList& options, const QString& value) {
        auto* c = new FCComboBox();
        c->addItems(options);
        const int idx = c->findText(value);
        c->setCurrentIndex(idx < 0 ? 0 : idx);
        return c;
    };
    for (const auto& [uid, t] : tools) {
        geo_tools_table->setItem(row, 0, item(QString::number(row + 1), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        // La columna del diámetro se puede editar (build_ui la habilita al final).
        geo_tools_table->setItem(row, 1, item(fmt(t.tooldia), Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled));
        geo_tools_table->setCellWidget(row, 2, combo({"Path", "In", "Out", "Custom"}, t.offset));
        geo_tools_table->setCellWidget(row, 3, combo({"Iso", "Rough", "Finish"}, t.type));
        geo_tools_table->setCellWidget(row, 4, combo({"C1", "C2", "C3", "C4", "B", "V"}, t.toolType));
        geo_tools_table->setItem(row, 5, new QTableWidgetItem(QString::number(uid)));
        auto* plot = new FCCheckBox();
        plot->setLayoutDirection(Qt::RightToLeft);
        plot->setChecked(plot_cb->isChecked());
        geo_tools_table->setCellWidget(row, 6, plot);
        ++row;
    }
    geo_tools_table->resizeColumnsToContents();
    geo_tools_table->resizeRowsToContents();
    geo_tools_table->verticalHeader()->hide();
    geo_tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = geo_tools_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::Fixed);
    h->setSectionResizeMode(6, QHeaderView::Fixed);
    h->resizeSection(4, 17);  // así en el original (pisa el 40 de la línea anterior)
    geo_tools_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    geo_tools_table->setColumnWidth(0, 20);
    geo_tools_table->setColumnWidth(4, 40);
    geo_tools_table->setColumnWidth(6, 17);
    geo_tools_table->setMinimumHeight(geo_tools_table->contentHeight());
    geo_tools_table->setMaximumHeight(geo_tools_table->contentHeight());
    geo_tools_table->setColumnHidden(6, false);  // multigeo
    // select_tools_table_row(0, clearsel=True) + update_ui()
    geo_tools_table->clearSelection();
    if (row > 0) geo_tools_table->selectRow(0);
    muted_ = false;
    updateUi();
    e_cut_entry->setDisabled(!extracut_cb->isChecked());
}

void GeometryObjectUI::updateUi() {
    if (!obj_) return;
    const std::vector<int> rows = selectedRows();
    // Color de get_fg_blue(): '#7777FF', porque en el original is_theme_black se usa sin llamarla.
    auto label = [this](const QString& what) {
        tool_data_label->setText(QString("<b>%1: <font color=\"#7777FF\">%2</font></b>").arg(tr_("Parameters for"), what));
    };
    if (rows.empty()) {
        generate_cnc_button->setDisabled(true);
        label(tr_("No Tool Selected"));
        return;
    }
    generate_cnc_button->setDisabled(false);
    if (rows.size() == 1) label(QString("%1 %2").arg(tr_("Tool")).arg(toolAt(rows.front())));
    else label(tr_("Multiple Tools"));
    muted_ = true;
    for (int r : rows) {
        auto it = obj_->tools().find(toolAt(r));
        if (it == obj_->tools().end()) continue;
        // ui_update_v_shape: con herramienta en V se muestran la punta y el ángulo y la Z de corte se calcula
        // (update_cutz, con los valores que tenga el formulario en ese momento, como el original).
        const QComboBox* tt = qobject_cast<QComboBox*>(geo_tools_table->cellWidget(r, 4));
        const bool vShape = tt && tt->currentText() == "V";
        for (QWidget* w : std::initializer_list<QWidget*>{tipdialabel, tipdia_entry, tipanglelabel, tipangle_entry})
            w->setVisible(vShape);
        cutz_entry->setDisabled(vShape);
        if (vShape) {
            const double halfAngle = tipangle_entry->value() / 2.0 * M_PI / 180.0;
            const double cut = (it->second.tooldia - tipdia_entry->value()) / (2.0 * std::tan(halfAngle));
            it->second.data.cutZ = QString::number(cut, 'f', 4).toDouble() * -1.0;
        }
        const GeometryTool& t = it->second;
        const GeometryToolSettings& d = t.data;
        cutz_entry->setValue(d.cutZ);
        tipdia_entry->setValue(t.vtipDia);
        tipangle_entry->setValue(t.vtipAngle);
        mpass_cb->setChecked(d.multidepth);
        maxdepth_entry->setValue(d.depthPerPass);
        travelz_entry->setValue(d.travelZ);
        cncfeedrate_entry->setValue(d.feedrate);
        feedrate_z_entry->setValue(d.feedrateZ);
        feedrate_rapid_entry->setValue(d.feedrateRapid);
        cncspindlespeed_entry->setValue(static_cast<int>(d.spindleSpeed));
        dwell_cb->setChecked(d.dwell);
        dwelltime_entry->setValue(d.dwellTime);
        extracut_cb->setChecked(d.extracut);
        e_cut_entry->setValue(d.extracutLength);
        toolchangeg_cb->setChecked(t.toolchange);
        toolchangez_entry->setValue(d.toolchangeZ);
        endz_entry->setValue(d.endZ);
        endxy_entry->setText(d.endXY ? QString("%1, %2").arg((*d.endXY)[0]).arg((*d.endXY)[1]) : QString("None"));
        pp_geometry_name_cb->setValue(t.ppname);
        tool_offset_entry->setValue(t.offsetValue);
    }
    muted_ = false;
    onPpChanged(true);
}

void GeometryObjectUI::addTool(double diaIn) {
    if (!obj_) return;
    auto say = [this](const QString& m) {
        if (inform) inform(m);
    };
    if (diaIn == 0) {
        build(*obj_);
        say("[WARNING_NOTCL] " + tr_("Please enter a tool diameter with non-zero value, in Float format."));
        return;
    }
    const double dia = std::round(diaIn * 1e4) / 1e4;
    auto& tools = obj_->tools();
    const int uid = tools.empty() ? 1 : tools.rbegin()->first + 1;
    // on_tool_default_add: copia de la última herramienta (o la de fábrica), con el diámetro nuevo.
    auto defaultAdd = [&] {
        GeometryTool t;
        if (!tools.empty()) {
            t = tools.rbegin()->second;
            if (t.solidGeometry.empty()) t.solidGeometry = obj_->solidGeometry();
        } else {
            t = defaultGeometryTool();
            t.solidGeometry = obj_->solidGeometry();
        }
        t.tooldia = dia;
        tools[uid] = t;
        say("[success] " + tr_("Tool added in Tool Table."));
        build(*obj_);
        if (onToolsChanged) onToolsChanged();
    };
    JsonValue db;
    QString error;
    if (!ToolsDatabase::load(ToolsDatabase::fileName(dataPath), db, error)) {
        say("[ERROR] " + tr_(error.startsWith("Could") ? "Could not load Tools DB file." : "Failed to parse Tools DB file."));
        defaultAdd();
        return;
    }
    // Como el original: offset, tipo y forma quedan los de la ÚLTIMA herramienta recorrida de la base; con el mismo
    // diámetro se copian los datos tools_mill_* y generales, y por la tolerancia los tools_iso* y generales.
    int found = 0;
    std::optional<double> updated;
    JsonValue match, last;
    std::string prefix;
    for (const auto& [id, t] : db.members()) {
        last = t;
        const JsonValue& d = t["data"];
        if (!(d["tool_target"].isNumber() && d["tool_target"].toDouble() == 1)) continue;  // Fresado
        const double dbDia = t["tooldia"].toDouble();
        if (dia == dbDia) {
            ++found;
            match = t;
            prefix = "tools_mill_";
        } else if (d["tol_max"].toDouble() >= dia && dia >= d["tol_min"].toDouble()) {
            ++found;
            match = t;
            updated = dbDia;
            prefix = "tools_iso";
        }
    }
    if (found == 0) {
        say("[WARNING_NOTCL] " + tr_("Tool not in Tools Database. Adding a default tool."));
        defaultAdd();
        return;
    }
    if (found > 1) {
        say("[WARNING_NOTCL] " + tr_("Cancelled.\nMultiple tools for one tool diameter found in Tools Database."));
        return;
    }
    ClearingTool ct;
    ct.geo = defaultGeometryTool();
    ToolsDatabase::applyTo(match, ct, prefix);
    ct.geo.tooldia = updated ? *updated : dia;
    ct.geo.offset = QString::fromStdString(last["offset"].text());
    ct.geo.offsetValue = last["offset_value"].toDouble();
    ct.geo.type = QString::fromStdString(last["type"].text());
    ct.geo.toolType = QString::fromStdString(last["tool_type"].text());
    ct.geo.solidGeometry = obj_->solidGeometry();
    tools[uid] = ct.geo;
    build(*obj_);
    for (int r = 0; r < geo_tools_table->rowCount(); ++r)
        if (toolAt(r) == uid) geo_tools_table->selectRow(r);
    say("[success] " + tr_("New tool added to Tool Table from Tools Database."));
    if (onToolsChanged) onToolsChanged();
}

void GeometryObjectUI::deleteTools() {
    if (!obj_) return;
    const std::vector<int> rows = selectedRows();
    if (rows.empty()) {
        if (inform) inform("[WARNING_NOTCL] " + tr_("Failed. Select a tool to delete."));
        return;
    }
    std::vector<int> ids;
    for (int r : rows) ids.push_back(toolAt(r));
    for (int id : ids) {
        if (obj_->tools().size() == 1) obj_->solidGeometry() = obj_->tools().begin()->second.solidGeometry;
        obj_->tools().erase(id);
    }
    build(*obj_);
    if (inform) inform("[success] " + tr_("Tool was deleted in Tool Table."));
    if (onToolsChanged) onToolsChanged();
}

void GeometryObjectUI::formToStorage() {
    if (muted_ || !obj_ || geo_tools_table->rowCount() == 0) return;
    // gui_form_to_storage: solo la herramienta de la fila actual (o la primera).
    int row = geo_tools_table->currentRow();
    if (row < 0) row = 0;
    auto it = obj_->tools().find(toolAt(row));
    if (it == obj_->tools().end()) return;
    GeometryTool& t = it->second;
    GeometryToolSettings& d = t.data;
    d.cutZ = cutz_entry->value();
    t.vtipDia = tipdia_entry->value();
    t.vtipAngle = tipangle_entry->value();
    d.multidepth = mpass_cb->isChecked();
    d.depthPerPass = maxdepth_entry->value();
    d.travelZ = travelz_entry->value();
    d.feedrate = cncfeedrate_entry->value();
    d.feedrateZ = feedrate_z_entry->value();
    d.feedrateRapid = feedrate_rapid_entry->value();
    d.spindleSpeed = cncspindlespeed_entry->value();
    d.dwell = dwell_cb->isChecked();
    d.dwellTime = dwelltime_entry->value();
    d.extracut = extracut_cb->isChecked();
    d.extracutLength = e_cut_entry->value();
    if (const auto* c = qobject_cast<QComboBox*>(geo_tools_table->cellWidget(row, 2))) t.offset = c->currentText();
    if (const auto* c = qobject_cast<QComboBox*>(geo_tools_table->cellWidget(row, 3))) t.type = c->currentText();
    if (const auto* c = qobject_cast<QComboBox*>(geo_tools_table->cellWidget(row, 4))) t.toolType = c->currentText();
    t.offsetValue = tool_offset_entry->value();
}

void GeometryObjectUI::commonToStorage() {
    if (muted_ || !obj_) return;
    const auto xy = endxy_entry->evaluated();
    for (auto& [uid, t] : obj_->tools()) {
        t.toolchange = toolchangeg_cb->isChecked();
        t.data.toolchangeZ = toolchangez_entry->value();
        t.data.endZ = endz_entry->value();
        if (xy && xy->size() == 2) t.data.endXY = std::array<double, 2>{(*xy)[0], (*xy)[1]};
        else t.data.endXY.reset();
        t.ppname = pp_geometry_name_cb->currentText();
    }
}

void GeometryObjectUI::onPpChanged(bool fromBuild) {
    // on_pp_changed.
    const QString pp = pp_geometry_name_cb->currentText().toLower();
    if (pp == "hpgl") {
        // El plotter no tiene profundidad: sin pasadas múltiples y con cambio de herramienta (SPn).
        oldPpState_ = mpass_cb->isChecked();
        oldToolchangeState_ = toolchangeg_cb->isChecked();
        mpass_cb->setChecked(false);
        mpass_cb->setDisabled(true);
        toolchangeg_cb->setChecked(true);
        toolchangeg_cb->setDisabled(true);
    } else {
        // Al abrir el panel (fromBuild) el original ya restauró estos valores antes de cargar los de la herramienta.
        if (!fromBuild) {
            mpass_cb->setChecked(oldPpState_);
            toolchangeg_cb->setChecked(oldToolchangeState_);
        }
        mpass_cb->setDisabled(false);
        toolchangeg_cb->setDisabled(false);
    }
    const bool probe = pp.contains("toolchange_probe");
    for (QWidget* w : std::initializer_list<QWidget*>{pdepth_entry, pdepth_label, feedrate_probe_entry, feedrate_probe_label})
        w->setVisible(probe);
    const bool rapid = pp.contains("marlin") || pp.contains("custom");
    fr_rapidlabel->setVisible(rapid);
    feedrate_rapid_entry->setVisible(rapid);
    const bool laser = pp.contains("laser");
    cutzlabel->setVisible(!laser);
    cutz_entry->setVisible(!laser);
    mpass_cb->setVisible(!laser);
    maxdepth_entry->setVisible(!laser);
    frzlabel->setVisible(!laser);
    feedrate_z_entry->setVisible(!laser);
    dwell_cb->setVisible(!laser);
    dwelltime_entry->setVisible(!laser);
    if (laser && pp.contains("marlin")) {
        travelzlabel->setText(tr_("Focus Z") + ":");
        endz_label->show();
        endz_entry->show();
    } else if (laser) {
        travelzlabel->hide();
        travelz_entry->hide();
        endz_label->hide();
        endz_entry->hide();
    } else {
        travelzlabel->setText(tr_("Travel Z") + ":");
        travelzlabel->show();
        travelz_entry->show();
        endz_label->show();
        endz_entry->show();
        // El original muestra el rótulo del offset (y falla al mostrar `offset_entry`, que no existe).
        tool_offset_lbl->show();
    }
    spindle_label->setText((laser ? tr_("Laser Power") : tr_("Spindle speed")) + ":");
}

CNCObjectUI::CNCObjectUI(QWidget* parent) : CNCObjectUIBase(parent) {
    setupUi();
    connect(export_gcode_button, &QPushButton::clicked, this, [this] {
        if (onExport) onExport();
    });
    connect(review_gcode_button, &QToolButton::clicked, this, [this] {
        if (onReview) onReview();
    });
    connect(editor_button, &QPushButton::clicked, this, [this] {
        if (onEdit) onEdit();
    });
}

void CNCObjectUI::build(CNCJobObject& obj) {
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    // set_ui: distancia recorrida y duración estimada, si el trabajo se generó en esta sesión (travel_distance no se
    // guarda en los proyectos).
    if (const double travel = obj.job().travelDistance(); travel != 0) {
        t_distance_label->show();
        t_distance_entry->setVisible(true);
        t_distance_entry->setDisabled(true);
        t_distance_entry->setText(QString::asprintf("%.4f", travel));
        units_label->setText(QString::fromStdString(obj.header().units).toLower());
        units_label->setDisabled(true);
        t_time_label->show();
        t_time_entry->setVisible(true);
        t_time_entry->setDisabled(true);
        // Más de 1: minutos; si no, segundos.
        const double r = obj.job().routingTime();
        if (r > 1) {
            t_time_entry->setText(QString::asprintf("%.4f", std::ceil(r)));
            units_time_label->setText("min");
        } else {
            t_time_entry->setText(QString::asprintf("%.4f", std::ceil(r * 60)));
            units_time_label->setText("sec");
        }
        units_time_label->setDisabled(true);
    }
    // CNCJobObject.build_ui: los trabajos de taladrado muestran la tabla de herramientas de Excellon.
    const auto& tools = obj.tools();
    const bool excellon = !tools.empty() && tools.front().drills > 0;
    cnc_tools_table->setVisible(!excellon);
    exc_cnc_tools_table->setVisible(excellon);
    if (!excellon) {
        // build_cnc_tools_table: herramientas de un trabajo de geometría.
        cnc_tools_table->setRowCount(static_cast<int>(tools.size()));
        int r = 0;
        for (const CncToolRow& t : tools) {
            QString off = t.offset;
            if (!off.isEmpty()) off[0] = off[0].toUpper();
            cnc_tools_table->setItem(r, 0, item(QString::number(r + 1), Qt::ItemIsEnabled));
            cnc_tools_table->setItem(r, 1, item(fmt(t.dia), Qt::ItemIsEnabled));
            cnc_tools_table->setItem(r, 2, item(off, Qt::ItemIsEnabled));
            cnc_tools_table->setItem(r, 3, item(t.type, Qt::ItemIsEnabled));
            cnc_tools_table->setItem(r, 4, item(t.toolType, Qt::ItemIsEnabled));
            cnc_tools_table->setItem(r, 5, new QTableWidgetItem(QString::number(t.tool)));
            auto* plot = new FCCheckBox();
            plot->setLayoutDirection(Qt::RightToLeft);
            plot->setChecked(plot_cb->isChecked());
            cnc_tools_table->setCellWidget(r, 6, plot);
            ++r;
        }
        cnc_tools_table->resizeColumnsToContents();
        cnc_tools_table->resizeRowsToContents();
        cnc_tools_table->verticalHeader()->hide();
        cnc_tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QHeaderView* h = cnc_tools_table->horizontalHeader();
        h->setMinimumSectionSize(10);
        h->setDefaultSectionSize(70);
        h->setSectionResizeMode(0, QHeaderView::Fixed);
        h->resizeSection(0, 20);
        h->setSectionResizeMode(1, QHeaderView::Stretch);
        h->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        h->setSectionResizeMode(4, QHeaderView::Fixed);
        h->resizeSection(4, 40);
        h->setSectionResizeMode(6, QHeaderView::Fixed);
        h->resizeSection(4, 17);  // así en el original
        cnc_tools_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        cnc_tools_table->setColumnWidth(0, 20);
        cnc_tools_table->setColumnWidth(4, 40);
        cnc_tools_table->setColumnWidth(6, 17);
        cnc_tools_table->setMinimumHeight(cnc_tools_table->contentHeight());
        cnc_tools_table->setMaximumHeight(cnc_tools_table->contentHeight());
        return;
    }
    exc_cnc_tools_table->setRowCount(static_cast<int>(tools.size()));
    int row = 0;
    for (const CncToolRow& t : tools) {
        exc_cnc_tools_table->setItem(row, 0, item(QString::number(row + 1), Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 1, item(fmt(t.dia), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 2, item(QString::number(t.drills), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 3, item(QString::number(t.slots), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 4, item(QString::number(t.tool), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 5, item(QString::asprintf("%f", t.cutZ), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        auto* plot = new FCCheckBox();
        plot->setLayoutDirection(Qt::RightToLeft);
        plot->setChecked(true);
        exc_cnc_tools_table->setCellWidget(row, 6, plot);
        ++row;
    }
    exc_cnc_tools_table->resizeColumnsToContents();
    exc_cnc_tools_table->resizeRowsToContents();
    exc_cnc_tools_table->verticalHeader()->hide();
    exc_cnc_tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    exc_cnc_tools_table->setColumnHidden(4, true);  // identificador interno de la herramienta
    exc_cnc_tools_table->setMinimumHeight(exc_cnc_tools_table->contentHeight());
    exc_cnc_tools_table->setMaximumHeight(exc_cnc_tools_table->contentHeight());
}

}  // namespace flatcam::gui
