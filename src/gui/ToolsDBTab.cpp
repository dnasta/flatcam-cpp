#include "gui/ToolsDBTab.h"

#include <QFileDialog>
#include <QMenu>
#include <QTabBar>
#include <QTreeWidgetItem>
#include <cmath>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/ToolsDatabase.h"
#include "gui/Translation.h"
#include "gui/widgets/FCButton.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTree.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

namespace {

/// Claves que van en la herramienta y no en su `data` (`update_storage`).
bool isToolKey(const std::string& key) {
    return key == "name" || key == "tooldia" || key == "offset" || key == "offset_value" || key == "type" ||
           key == "tool_type";
}

/// `get_value()` del widget, con el tipo de Python (los FCDoubleSpinner dan float, los FCSpinner int, FCComboBox2 el
/// índice, FCComboBox y FCEntry el texto, RadioSet el valor elegido, FCCheckBox un booleano).
JsonValue widgetValue(QWidget* w) {
    if (auto* d = qobject_cast<FCDoubleSpinner*>(w)) return JsonValue::real(d->value());
    if (auto* s = qobject_cast<FCSpinner*>(w)) return JsonValue::integer(s->value());
    if (auto* c = qobject_cast<FCCheckBox*>(w)) return JsonValue::boolean(c->isChecked());
    if (auto* r = qobject_cast<RadioSet*>(w)) return JsonValue::string(r->value().toString().toStdString());
    if (auto* c2 = qobject_cast<FCComboBox2*>(w)) return JsonValue::integer(c2->indexValue());
    if (auto* c = qobject_cast<FCComboBox*>(w)) return JsonValue::string(c->value().toStdString());
    if (auto* e = qobject_cast<FCEntry*>(w)) return JsonValue::string(e->text().toStdString());
    return JsonValue();
}

/// `set_value()` del widget.
void setWidgetValue(QWidget* w, const JsonValue& v) {
    if (v.isNull()) return;
    const QString text = QString::fromStdString(v.text());
    if (auto* d = qobject_cast<FCDoubleSpinner*>(w)) d->setValue(v.toDouble());
    else if (auto* s = qobject_cast<FCSpinner*>(w)) s->setValue(static_cast<int>(v.toDouble()));
    else if (auto* c = qobject_cast<FCCheckBox*>(w)) c->setChecked(v.toBool());
    else if (auto* r = qobject_cast<RadioSet*>(w)) r->setValue(text);
    else if (auto* c2 = qobject_cast<FCComboBox2*>(w)) c2->setIndexValue(static_cast<int>(v.toDouble()));
    else if (auto* cb = qobject_cast<FCComboBox*>(w)) cb->setValue(v.isNumber() ? text : text);
    else if (auto* e = qobject_cast<FCEntry*>(w)) e->setText(text);
}

}  // namespace

ToolsDBTab::ToolsDBTab(App* app, std::function<void(const JsonValue&)> onToolRequest, QWidget* parent)
    : ToolsDB2UIBase(parent), app_(app), onToolRequest_(std::move(onToolRequest)) {
    setupUi();
    tree_widget->setColumnCount(2);
    tree_widget->setHeaderLabels({tr_("ID"), tr_("Tool Name")});
    fields_ = {
        {"tool_target", tool_op_combo}, {"tol_min", tol_min_entry}, {"tol_max", tol_max_entry}, {"name", name_entry},
        {"tooldia", dia_entry}, {"tool_type", mill_shape_combo}, {"cutz", mill_cutz_entry},
        {"multidepth", mill_multidepth_cb}, {"depthperpass", mill_multidepth_entry}, {"travelz", mill_travelz_entry},
        {"feedrate", mill_frxy_entry}, {"feedrate_z", mill_frz_entry}, {"spindlespeed", mill_spindle_entry},
        {"dwell", mill_dwell_cb}, {"dwelltime", mill_dwelltime_entry}, {"type", mill_type_combo},
        {"offset", mill_tooloffset_combo}, {"offset_value", mill_custom_offset_entry}, {"vtipdia", mill_vdia_entry},
        {"vtipangle", mill_vangle_entry}, {"feedrate_rapid", mill_frapids_entry}, {"extracut", mill_ecut_cb},
        {"extracut_length", mill_ecut_length_entry}, {"tools_ncc_operation", ncc_op_radio},
        {"tools_ncc_milling_type", ncc_milling_type_radio}, {"tools_ncc_overlap", ncc_overlap_entry},
        {"tools_ncc_margin", ncc_margin_entry}, {"tools_ncc_method", ncc_method_combo},
        {"tools_ncc_connect", ncc_connect_cb}, {"tools_ncc_contour", ncc_contour_cb},
        {"tools_ncc_offset_choice", ncc_choice_offset_cb}, {"tools_ncc_offset_value", ncc_offset_spinner},
        {"tools_paint_overlap", paint_overlap_entry}, {"tools_paint_offset", paint_offset_entry},
        {"tools_paint_method", paint_method_combo}, {"tools_paint_connect", path_connect_cb},
        {"tools_paint_contour", paint_contour_cb}, {"tools_iso_passes", iso_passes_entry},
        {"tools_iso_overlap", iso_overlap_entry}, {"tools_iso_milling_type", iso_milling_type_radio},
        {"tools_iso_follow", iso_follow_cb}, {"tools_iso_isotype", iso_type_radio},
        {"tools_drill_cutz", drill_cutz_entry}, {"tools_drill_multidepth", drill_mpass_cb},
        {"tools_drill_depthperpass", drill_maxdepth_entry}, {"tools_drill_travelz", drill_travelz_entry},
        {"tools_drill_feedrate_z", drill_feedrate_z_entry}, {"tools_drill_feedrate_rapid", drill_feedrate_rapid_entry},
        {"tools_drill_spindlespeed", drill_spindlespeed_entry}, {"tools_drill_dwell", drill_dwell_cb},
        {"tools_drill_dwelltime", drill_dwelltime_entry}, {"tools_drill_offset", drill_offset_entry},
        {"tools_drill_drill_slots", drill_slots_cb}, {"tools_drill_drill_overlap", drill_overlap_entry},
        {"tools_drill_last_drill", drill_last_drill_cb}, {"tools_cutout_margin", cutout_margin_entry},
        {"tools_cutout_gapsize", cutout_gapsize}, {"tools_cutout_gaps_ff", cutout_gaps},
        {"tools_cutout_convexshape", cutout_convex_box}, {"tools_cutout_gap_type", cutout_gaptype_radio},
        {"tools_cutout_gap_depth", cutout_thin_depth_entry}, {"tools_cutout_mb_dia", cutout_mb_dia_entry},
        {"tools_cutout_mb_spacing", cutout_mb_spacing_entry},
    };
    // ui_connect: cada campo actualiza la herramienta actual (update_storage).
    for (auto& [key, w] : fields_) {
        QWidget* wd = w;
        if (auto* d = qobject_cast<FCDoubleSpinner*>(wd))
            connect(d, &QDoubleSpinBox::valueChanged, this, [this, wd] { onWidgetChanged(wd); });
        else if (auto* s = qobject_cast<FCSpinner*>(wd))
            connect(s, &QSpinBox::valueChanged, this, [this, wd] { onWidgetChanged(wd); });
        else if (auto* c = qobject_cast<FCCheckBox*>(wd))
            connect(c, &QCheckBox::toggled, this, [this, wd] { onWidgetChanged(wd); });
        else if (auto* r = qobject_cast<RadioSet*>(wd))
            connect(r, &RadioSet::activatedCustom, this, [this, wd] { onWidgetChanged(wd); });
        else if (auto* cb = qobject_cast<QComboBox*>(wd))
            connect(cb, &QComboBox::currentIndexChanged, this, [this, wd] { onWidgetChanged(wd); });
        else if (auto* e = qobject_cast<QLineEdit*>(wd))
            connect(e, &QLineEdit::textChanged, this, [this, wd] { onWidgetChanged(wd); });
    }
    connect(name_entry, &QLineEdit::editingFinished, this, [this] {
        // update_tree_name
        if (QTreeWidgetItem* it = tree_widget->currentItem()) it->setText(1, name_entry->text());
    });
    for (QWidget* w : std::initializer_list<QWidget*>{mill_shape_combo, mill_cutz_entry, mill_vdia_entry,
                                                      mill_vangle_entry}) {
        if (auto* cb = qobject_cast<QComboBox*>(w))
            connect(cb, &QComboBox::currentIndexChanged, this, [this] { calculateToolDia(); });
        else if (auto* d = qobject_cast<QDoubleSpinBox*>(w))
            connect(d, &QDoubleSpinBox::valueChanged, this, [this] { calculateToolDia(); });
    }
    connect(tool_op_combo, &QComboBox::currentIndexChanged, this, [this](int i) { onTargetChanged(i); });
    connect(add_entry_btn, &QPushButton::clicked, this, [this] { addTool(); });
    connect(remove_entry_btn, &QPushButton::clicked, this, [this] { deleteTools(); });
    connect(save_db_btn, &QPushButton::clicked, this, [this] {
        changed_ = false;
        save();
    });
    connect(add_tool_from_db, &QPushButton::clicked, this, [this] { requestTool(); });
    connect(cancel_tool_from_db, &QPushButton::clicked, this, [this] {
        // on_cancel_tool: cierra la pestaña.
        FCDetachableTab2* area = app_->ui()->plot_tab_area;
        const int idx = area->indexOf(this);
        if (idx >= 0) area->removeTab(idx);
        deleteLater();
        app_->ui()->inform(tr_("Cancelled adding tool from DB."));
    });
    connect(export_db_btn, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getSaveFileName(this, tr_("Export Tools Database"), QString(),
                                                       "Text File (*.TXT);;All Files (*.*)");
        if (f.isEmpty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
            return;
        }
        if (!ToolsDatabase::save(f, db_)) {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed to write Tools DB to file."));
            return;
        }
        app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Exported Tools DB to"), f));
    });
    connect(import_db_btn, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getOpenFileName(this, tr_("Import FlatCAM Tools DB"), QString(),
                                                       "Text File (*.TXT);;All Files (*.*)");
        if (f.isEmpty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
            return;
        }
        JsonValue db;
        QString error;
        if (!ToolsDatabase::load(f, db, error)) {
            app_->ui()->inform((error.startsWith("Could") ? "[ERROR_NOTCL] " : "[ERROR] ") + tr_(error.toUtf8().constData()));
            return;
        }
        db_ = db;
        app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Loaded Tools DB from"), f));
        buildDbUi();
        markEdited();
    });
    connect(tree_widget, &QTreeWidget::currentItemChanged, this, [this](QTreeWidgetItem* cur, QTreeWidgetItem*) {
        if (!cur) return;
        currentId_ = cur->text(0).toInt();
        if (JsonValue* t = currentTool()) storageToForm(*t);
    });
    connect(tree_widget, &QTreeWidget::itemChanged, this, [this](QTreeWidgetItem* item, int column) {
        if (column == 1) name_entry->setText(item->text(1));  // on_list_item_edited
    });
    connect(tree_widget, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem*, int column) {
        if (column == 0 && onToolRequest_) requestTool();  // ok_to_add: abierta desde una herramienta
    });
    tree_widget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree_widget, &QWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        QMenu menu;
        connect(menu.addAction(QIcon(":/images/plus16.png"), tr_("Add to DB")), &QAction::triggered, this,
                [this] { addTool(); });
        connect(menu.addAction(QIcon(":/images/copy16.png"), tr_("Copy from DB")), &QAction::triggered, this,
                [this] { copyTools(); });
        connect(menu.addAction(QIcon(":/images/delete32.png"), tr_("Delete from DB")), &QAction::triggered, this,
                [this] { deleteTools(); });
        connect(menu.addAction(QIcon(":/images/save_as.png"), tr_("Save changes")), &QAction::triggered, this,
                [this] {
                    changed_ = false;
                    save();
                });
        menu.exec(tree_widget->viewport()->mapToGlobal(pos));
    });

    // setup_db_ui: lee la base.
    const QString path = ToolsDatabase::fileName(app_->dataPath());
    QString error;
    if (!ToolsDatabase::load(path, db_, error)) {
        app_->ui()->inform((error.startsWith("Could") ? "[ERROR] " : "[ERROR] ") + tr_(error.toUtf8().constData()));
        db_ = JsonValue::object();
        return;
    }
    app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Loaded Tools DB from"), path));
    buildDbUi();
}

JsonValue* ToolsDBTab::currentTool() {
    JsonValue* t = db_.find(std::to_string(currentId_));
    return t && t->isObject() ? t : nullptr;
}

void ToolsDBTab::buildDbUi() {
    muted_ = true;
    tree_widget->blockSignals(true);
    tree_widget->clear();
    int row = 0;
    for (const auto& [id, t] : db_.members()) {
        auto* it = new QTreeWidgetItem(tree_widget);
        it->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
        it->setFlags(it->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable);
        it->setText(0, QString::number(++row));
        it->setText(1, QString::fromStdString(t["name"].text()));
    }
    tree_widget->blockSignals(false);
    QWidget* boxes[] = {tool_description_box, milling_box, ncc_box, paint_box, iso_box, drill_box, cutout_box};
    if (currentId_ < 1) {
        if (const JsonValue* first = db_.find("1")) {
            currentId_ = 1;
            storageToForm(*first);
            const JsonValue& target = (*first)["data"]["tool_target"];
            onTargetChanged(target.isNumber() ? static_cast<int>(target.toDouble()) : 0);
            tree_widget->setCurrentItem(tree_widget->topLevelItem(0));
        } else {
            // Base vacía: todas las secciones a la vista y deshabilitadas.
            for (QWidget* b : boxes) {
                b->show();
                b->setEnabled(false);
            }
        }
    } else if (JsonValue* t = currentTool()) {
        storageToForm(*t);
    }
    muted_ = false;
}

void ToolsDBTab::storageToForm(const JsonValue& tool) {
    muted_ = true;
    for (auto& [key, w] : fields_) {
        if (const JsonValue* v = tool.find(key)) setWidgetValue(w, *v);
        if (const JsonValue* v = tool["data"].isObject() ? const_cast<JsonValue&>(tool["data"]).find(key) : nullptr)
            setWidgetValue(w, *v);
    }
    muted_ = false;
}

void ToolsDBTab::onTargetChanged(int target) {
    // on_tool_target_changed: 0 General, 1 Fresado, 2 Perforación, 3 Aislamiento, 4 Pintura, 5 NCC, 6 Corte.
    tool_description_box->setEnabled(true);
    if (db_.members().empty()) return;
    QWidget* all[] = {milling_box, ncc_box, paint_box, iso_box, drill_box, cutout_box};
    if (target == 0) {
        for (QWidget* b : all) {
            b->setEnabled(true);
            b->show();
        }
        return;
    }
    for (QWidget* b : all) b->hide();
    auto showBox = [](QWidget* b) {
        b->setEnabled(true);
        b->show();
    };
    if (target == 1) showBox(milling_box);
    if (target == 2) showBox(drill_box);
    if (target == 3) showBox(iso_box), showBox(milling_box);
    if (target == 4) showBox(paint_box), showBox(milling_box);
    if (target == 5) showBox(ncc_box), showBox(milling_box);
    if (target == 6) showBox(cutout_box), showBox(milling_box);
}

void ToolsDBTab::onWidgetChanged(QWidget* w) {
    if (muted_) return;
    JsonValue* tool = currentTool();
    if (!tool) return;
    const char* key = nullptr;
    for (auto& [k, wd] : fields_)
        if (wd == w) key = k;
    if (!key) return;
    // update_storage: solo con una herramienta elegida.
    const int sel = static_cast<int>(tree_widget->selectedItems().size());
    if (sel > 1) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2")
                               .arg(tr_("To change tool properties select only one tool. Tools currently selected"))
                               .arg(sel));
        muted_ = true;
        setWidgetValue(w, (*tool)["data"][key]);
        muted_ = false;
        return;
    }
    if (isToolKey(key)) tool->set(key, widgetValue(w));
    else {
        if (!(*tool)["data"].isObject()) tool->set("data", JsonValue::object());
        tool->find("data")->set(key, widgetValue(w));
    }
    markEdited();
}

void ToolsDBTab::markEdited(bool silent) {
    // on_tools_db_edited: la pestaña y el botón Guardar en rojo.
    FCDetachableTab2* area = app_->ui()->plot_tab_area;
    const int idx = area->indexOf(this);
    if (idx >= 0) area->tabBar()->setTabTextColor(idx, QColor("red"));
    save_db_btn->setStyleSheet("QPushButton {color: red;}");
    changed_ = true;
    if (!silent) app_->ui()->inform("[WARNING_NOTCL] " + tr_("Tools in Tools Database edited but not saved."));
}

void ToolsDBTab::calculateToolDia() {
    // on_calculate_tooldia: con punta en V, el diámetro a la Z de corte.
    if (muted_ || mill_shape_combo->value() != "V") return;
    const double tip = mill_vdia_entry->value();
    const double half = mill_vangle_entry->value() / 2.0;
    const double cut = std::fabs(mill_cutz_entry->value());
    dia_entry->setValue(tip + 2 * cut * std::tan(half * M_PI / 180.0));
}

void ToolsDBTab::addTool() {
    JsonValue tool = ToolsDatabase::newTool(db_);
    const int newId = static_cast<int>(db_.members().size()) + 1;
    db_.set(std::to_string(newId), tool);
    markEdited(true);
    currentId_ = newId;
    buildDbUi();
    const int n = tree_widget->topLevelItemCount();
    if (n) {
        tree_widget->setCurrentItem(tree_widget->topLevelItem(n - 1));
        tree_widget->topLevelItem(n - 1)->setSelected(true);
    }
    onTargetChanged(0);
    app_->ui()->inform("[success] " + tr_("Tool added to DB."));
}

void ToolsDBTab::copyTools() {
    int newId = static_cast<int>(db_.members().size());
    for (QTreeWidgetItem* it : tree_widget->selectedItems()) {
        const JsonValue* src = db_.find(it->text(0).toStdString());
        if (!src) continue;
        const JsonValue copy = *src;
        db_.set(std::to_string(++newId), copy);
    }
    currentId_ = newId;
    buildDbUi();
    const int n = tree_widget->topLevelItemCount();
    if (n) tree_widget->setCurrentItem(tree_widget->topLevelItem(n - 1));
    markEdited();
    app_->ui()->inform("[success] " + tr_("Tool copied from Tools DB."));
}

void ToolsDBTab::deleteTools() {
    std::vector<std::string> ids;
    for (QTreeWidgetItem* it : tree_widget->selectedItems()) ids.push_back(it->text(0).toStdString());
    auto& m = db_.members();
    for (const std::string& id : ids)
        m.erase(std::remove_if(m.begin(), m.end(), [&](const auto& p) { return p.first == id; }), m.end());
    --currentId_;
    markEdited(true);
    buildDbUi();
    if (tree_widget->topLevelItemCount()) tree_widget->setCurrentItem(tree_widget->topLevelItem(0));
    app_->ui()->inform("[success] " + tr_("Tool removed from Tools DB."));
}

void ToolsDBTab::save(bool silent) {
    FCDetachableTab2* area = app_->ui()->plot_tab_area;
    const int idx = area->indexOf(this);
    if (idx >= 0) area->tabBar()->setTabTextColor(idx, palette().color(QPalette::WindowText));
    save_db_btn->setStyleSheet("");
    if (!ToolsDatabase::save(ToolsDatabase::fileName(app_->dataPath()), db_)) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed to write Tools DB to file."));
        return;
    }
    changed_ = false;
    if (!silent) app_->ui()->inform("[success] " + tr_("Saved Tools DB."));
}

void ToolsDBTab::requestTool() {
    if (tree_widget->selectedItems().isEmpty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("No Tool/row selected in the Tools Database table") + "...");
        return;
    }
    if (db_.members().empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Tools DB empty."));
        return;
    }
    const auto items = tree_widget->selectedItems();
    for (QTreeWidgetItem* it : items) {
        if (const JsonValue* t = db_.find(it->text(0).toStdString())) {
            const JsonValue copy = *t;
            if (onToolRequest_) onToolRequest_(copy);
        }
    }
}

}  // namespace flatcam::gui
