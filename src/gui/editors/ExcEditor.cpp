#include "gui/editors/ExcEditor.h"

#include <QApplication>
#include <QHeaderView>
#include <QScrollArea>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>

#include "cam/PyNumber.h"
#include "core/ArcMath.h"
#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/tools/DistanceTool.h"
#include "gui/widgets/DialogBoxRadio.h"

namespace flatcam::gui {

namespace {

constexpr int kShapesLayer = -10;
constexpr int kToolLayer = -11;
constexpr int kSelRectLayer = -12;

double dec(double v, int d = 4) { return QString::number(v, 'f', d).toDouble(); }

}  // namespace

/// La herramienta activa (`SelectEditorExc`, `DrillAdd`, `DrillArray`, `SlotAdd`, `SlotArray`, `ResizeEditorExc`,
/// `MoveEditorExc`, `CopyEditorExc`).
struct ExcEditor::Tool {
    QString name;
    bool complete{false};
    std::optional<double> dia;           ///< `selected_dia`.
    std::optional<Point2D> points;       ///< `points`: el último dato de la vista previa.
    std::optional<Point2D> origin;       ///< El centro de la grilla circular o el origen de mover.
    std::vector<double> selectedDias;    ///< `selected_dia_list` (mover, copiar).
    bool circFlag{false};                ///< `flag_for_circ_array`.
    ShapeList geometry;
};

ExcEditor::ExcEditor(App* app) : QObject(app), app_(app) {
    MainGUI* ui = app_->ui();
    ui_ = new ExcEditorUIBase();
    ui_->setupUi();
    buttons_ = {{"drill_select", ui->select_drill_btn}, {"drill_add", ui->add_drill_btn},
                {"drill_array", ui->add_drill_array_btn}, {"slot_add", ui->add_slot_btn},
                {"slot_array", ui->add_slot_array_btn},   {"drill_resize", ui->resize_drill_btn},
                {"drill_copy", ui->copy_drill_btn},       {"drill_move", ui->move_drill_btn}};
    for (auto& [name, a] : buttons_) {
        a->setCheckable(true);
        const QString n = name;
        connect(a, &QAction::triggered, this, [this, n] { if (active_) onToolSelect(n); });
    }
    auto onAct = [this](QAction* a, auto fn) { connect(a, &QAction::triggered, this, [this, fn] { if (active_) fn(); }); };
    onAct(ui->delete_drill_btn, [this] { deleteSelected(); });
    onAct(ui->exc_add_array_drill_menuitem, [this] { selectTool("drill_array"); });
    onAct(ui->exc_add_drill_menuitem, [this] { selectTool("drill_add"); });
    onAct(ui->exc_add_array_slot_menuitem, [this] { selectTool("slot_array"); });
    onAct(ui->exc_add_slot_menuitem, [this] { selectTool("slot_add"); });
    onAct(ui->exc_resize_drill_menuitem, [this] { selectTool("drill_resize"); });
    onAct(ui->exc_copy_drill_menuitem, [this] { selectTool("drill_copy"); });
    onAct(ui->exc_delete_drill_menuitem, [this] { deleteSelected(); });
    onAct(ui->exc_move_drill_menuitem, [this] { selectTool("drill_move"); });
    connect(ui_->exit_editor_button, &QPushButton::clicked, this, [this] { app_->editorToObject(); });
    connect(ui_->convert_slots_btn, &QPushButton::clicked, this, [this] { convertSlots(); });
    connect(ui_->name_entry, &QLineEdit::returnPressed, this, [this] { editedName_ = ui_->name_entry->text(); });
    connect(ui_->addtool_btn, &QPushButton::clicked, this, [this] { addTool(); });
    connect(ui_->deltool_btn, &QPushButton::clicked, this, [this] { deleteTools(); });
    connect(ui_->resize_btn, &QPushButton::clicked, this, [this] {
        // ResizeEditorExc.make
        if (!tool_ || tool_->name != "drill_resize") return;
        std::set<double> dias;
        for (double d : tableSelectedDias()) dias.insert(d);
        isModified_ = true;
        if (dias.empty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled. Nothing selected."));
        } else {
            const double newDia = ui_->resdrill_entry->value();
            const auto deleted = model_.resize(dias, newDia);
            if (!deleted) {
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("Cancelled."));
                return;
            }
            buildUi();
            replot();
            app_->ui()->inform("[success] " + tr_("Done."));
        }
        ui_->resize_frame->hide();
        selectTool("drill_select");
    });
    connect(ui_->tools_table_exc, &QTableWidget::cellPressed, this, [this](int row, int col) {
        if (col == 0) rowSelected(row);
    });
    connect(ui_->tools_table_exc, &QTableWidget::itemChanged, this, [this](QTableWidgetItem* it) {
        // on_tool_edit
        if (!active_ || it->column() != 1) return;
        bool ok = false;
        const double newDia = it->text().toDouble(&ok);
        if (!ok) return;
        const auto oldIt = tool2tooldia_.find(it->row() + 1);
        if (oldIt == tool2tooldia_.end() || oldIt->second == newDia) return;
        isModified_ = true;
        model_.changeDia(oldIt->second, newDia);
        QTimer::singleShot(0, this, [this] {
            buildUi();
            replot();
            app_->ui()->inform("[success] " + tr_("Done."));
        });
    });
    auto frames = [this](RadioSet* r, QFrame* lin, QFrame* circ) {
        connect(r, &RadioSet::activatedCustom, this, [lin, circ](const QString& v) {
            lin->setVisible(v == "linear");
            circ->setVisible(v != "linear");
        });
    };
    frames(ui_->array_type_radio, ui_->array_linear_frame, ui_->array_circular_frame);
    frames(ui_->slot_array_type_radio, ui_->slot_array_linear_frame, ui_->slot_array_circular_frame);
    connect(ui_->drill_axis_radio, &RadioSet::activatedCustom, this,
            [this](const QString& v) { ui_->linear_angle_spinner->setEnabled(v == "A"); });
    connect(ui_->slot_axis_radio, &RadioSet::activatedCustom, this,
            [this](const QString& v) { ui_->slot_angle_spinner->setEnabled(v == "A"); });
    connect(ui_->slot_array_axis_radio, &RadioSet::activatedCustom, this,
            [this](const QString& v) { ui_->slot_array_linear_angle_spinner->setEnabled(v == "A"); });
}

ExcEditor::~ExcEditor() { delete ui_; }

QString ExcEditor::activeToolName() const { return tool_ ? tool_->name : QString(); }

std::optional<double> ExcEditor::lastToolDia() const {
    if (!lastToolSelected_) return std::nullopt;
    const auto it = tool2tooldia_.find(*lastToolSelected_);
    if (it == tool2tooldia_.end()) return std::nullopt;
    return it->second;
}

std::vector<double> ExcEditor::tableSelectedDias() const {
    std::vector<double> out;
    for (const QModelIndex& i : ui_->tools_table_exc->selectionModel()->selectedIndexes())
        if (const QTableWidgetItem* it = ui_->tools_table_exc->item(i.row(), 1)) {
            bool ok = false;
            const double d = it->text().toDouble(&ok);
            if (ok) out.push_back(d);
        }
    return out;
}

void ExcEditor::edit(ExcellonObject* obj) {
    MainGUI* ui = app_->ui();
    deactivate();
    active_ = true;
    obj_ = obj;
    isModified_ = false;
    toolbarOldState_ = ui->exc_edit_toolbar->isVisible();
    ui->menueditedit->setDisabled(true);
    ui->menueditok->setDisabled(false);
    ui->exc_editor_menu->setDisabled(false);
    ui->exc_editor_menu->menuAction()->setVisible(true);
    ui->update_obj_btn->setEnabled(true);
    ui->exc_edit_toolbar->setDisabled(false);
    ui->exc_edit_toolbar->setVisible(true);
    ui->setCanvasHandlersEnabled(false);
    ui->plotCanvas()->setLayerVisible(obj->id(), false);
    model_.load(obj->excellon(), 4);
    // set_ui
    ui_->addtool_entry->setValue(defNum("excellon_editor_newdia"));
    ui_->drill_array_size_entry->setValue(defInt("excellon_editor_array_size", 5));
    ui_->drill_axis_radio->setValue(defVariant("excellon_editor_lin_dir"));
    ui_->drill_pitch_entry->setValue(defNum("excellon_editor_lin_pitch"));
    ui_->linear_angle_spinner->setValue(defNum("excellon_editor_lin_angle"));
    ui_->drill_array_dir_radio->setValue(defVariant("excellon_editor_circ_dir"));
    ui_->drill_angle_entry->setValue(defNum("excellon_editor_circ_angle"));
    ui_->slot_length_entry->setValue(defNum("excellon_editor_slot_length"));
    ui_->slot_axis_radio->setValue(defVariant("excellon_editor_slot_direction"));
    ui_->slot_angle_spinner->setValue(defNum("excellon_editor_slot_angle"));
    ui_->slot_array_size_entry->setValue(defInt("excellon_editor_slot_array_size", 5));
    ui_->slot_array_axis_radio->setValue(defVariant("excellon_editor_slot_lin_dir"));
    ui_->slot_array_pitch_entry->setValue(defNum("excellon_editor_slot_lin_pitch"));
    ui_->slot_array_linear_angle_spinner->setValue(defNum("excellon_editor_slot_lin_angle"));
    ui_->slot_array_direction_radio->setValue(defVariant("excellon_editor_slot_circ_dir"));
    ui_->slot_array_angle_entry->setValue(defNum("excellon_editor_slot_circ_angle"));
    ui_->array_type_radio->setValue(QString("linear"));
    ui_->slot_array_type_radio->setValue(QString("linear"));
    selectTool("drill_select");
    lastToolSelected_.reset();
    editedName_ = obj->name();
    buildUi();
    // build_ui(first_run=True) deja elegida la última herramienta.
    if (!tool2tooldia_.empty()) lastToolSelected_ = static_cast<int>(tool2tooldia_.size());
    replot();
    if (tool2tooldia_.empty()) addTool(dec(defNum("excellon_editor_newdia")));
}

void ExcEditor::deactivate() {
    MainGUI* ui = app_->ui();
    if (active_) ui->setCanvasHandlersEnabled(true);
    ui->menueditedit->setDisabled(false);
    ui->menueditok->setDisabled(true);
    ui->exc_edit_toolbar->setDisabled(true);
    ui->exc_edit_toolbar->setVisible(toolbarOldState_);
    ui->exc_editor_menu->setDisabled(true);
    ui->exc_editor_menu->menuAction()->setVisible(false);
    ui->update_obj_btn->setEnabled(false);
    for (int id : {kShapesLayer, kToolLayer, kSelRectLayer}) ui->plotCanvas()->removeLayer(id);
    for (auto& [n, a] : buttons_) a->setChecked(false);
    tool_.reset();
    model_.selected.clear();
    active_ = false;
}

void ExcEditor::save(ExcellonObject* obj) {
    // update_fcexcellon: un Excellon nuevo con lo editado; "_edit" (o el número siguiente) si se cambió algo.
    QString name = editedName_.isEmpty() ? obj->name() : editedName_;
    if (isModified_) {
        if (name.contains("_edit")) {
            bool ok = false;
            const int idd = name.right(1).toInt(&ok);
            name = ok ? name.left(name.size() - 1) + QString::number(idd + 1) : name + "_1";
        } else {
            name += "_edit";
        }
    }
    auto e = std::make_unique<Excellon>();
    int id = 0;
    for (const auto& t : model_.result()) e->addDrillTool(++id, t.dia, t.drills, t.slots);
    e->createGeometry();
    auto o = std::make_unique<ExcellonObject>(name, std::move(e));
    lastCreated_ = app_->addObject(std::move(o));
    app_->refreshSourceFile(lastCreated_);
    deactivate();
    app_->ui()->plotCanvas()->setLayerVisible(obj->id(), obj->plotEnabled());
    app_->ui()->inform("[success] " + tr_("Excellon editing finished."));
}

void ExcEditor::buildUi() {
    FCTable* t = ui_->tools_table_exc;
    QSignalBlocker block(t);
    ui_->name_entry->setValue(editedName_);
    std::vector<double> dias;
    for (const auto& [d, nd] : model_.olddiaNewdia) dias.push_back(d);
    std::sort(dias.begin(), dias.end());
    t->clear();
    t->setColumnCount(4);
    t->setHorizontalHeaderLabels({"#", tr_("Diameter"), "D", "S"});
    t->setRowCount(static_cast<int>(dias.size()) + 2);
    int totD = 0, totS = 0, row = 0;
    for (double d : dias) {
        EditorStorage* st = model_.storage(d);
        int dc = 0, sc = 0;
        if (model_.pointsEdit.count(d)) dc = model_.pointsEdit.at(d);
        if (model_.slotPointsEdit.count(d)) sc = model_.slotPointsEdit.at(d);
        (void)st;
        totD += dc;
        totS += sc;
        t->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        auto* dia = new QTableWidgetItem(QString::number(model_.olddiaNewdia[d], 'f', 4));
        dia->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        t->setItem(row, 1, dia);
        t->setItem(row, 2, new QTableWidgetItem(QString::number(dc)));
        t->setItem(row, 3, new QTableWidgetItem(sc > 0 ? QString::number(sc) : QString()));
        for (int c : {0, 2, 3}) t->item(row, c)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ++row;
    }
    auto* e1 = new QTableWidgetItem("9998");
    e1->setForeground(QColor(255, 255, 255));
    t->setItem(row, 0, e1);
    t->setItem(row, 1, new QTableWidgetItem(tr_("Total Drills")));
    t->setItem(row, 2, new QTableWidgetItem(QString::number(totD)));
    t->setItem(row, 3, new QTableWidgetItem(""));
    for (int k : {1, 2}) t->item(row, k)->setForeground(QColor(127, 0, 255));
    ++row;
    auto* e2 = new QTableWidgetItem("9999");
    e2->setForeground(QColor(255, 255, 255));
    t->setItem(row, 0, e2);
    t->setItem(row, 1, new QTableWidgetItem(tr_("Total Slots")));
    t->setItem(row, 2, new QTableWidgetItem(""));
    t->setItem(row, 3, new QTableWidgetItem(QString::number(totS)));
    for (int k : {1, 2, 3}) t->item(row, k)->setForeground(QColor(0, 70, 255));
    t->resizeColumnsToContents();
    t->resizeRowsToContents();
    t->verticalHeader()->hide();
    // sortItems(1): ordena por el texto del diámetro (como texto, "10.0000" antes que "2.0000").
    t->sortItems(1);
    tool2tooldia_.clear();
    for (int r = 0; r < t->rowCount() - 2; ++r)
        if (t->item(r, 0) && t->item(r, 1)) tool2tooldia_[t->item(r, 0)->text().toInt()] = t->item(r, 1)->text().toDouble();
    t->setMaximumHeight(t->contentHeight());
    t->clearSelection();
    MainGUI* ui = app_->ui();
    ui->properties_scroll_area->takeWidget();
    ui->properties_scroll_area->setWidget(ui_);
    ui->notebook->setCurrentWidget(ui->properties_tab);
}

void ExcEditor::addTool(std::optional<double> diaIn) {
    isModified_ = true;
    const double d = diaIn ? *diaIn : ui_->addtool_entry->value();
    if (model_.olddiaNewdia.count(d)) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Tool already in the original or actual tool list.\nSave and "
                                                    "reedit Excellon if you need to add this tool. "));
        return;
    }
    model_.ensure(d);
    model_.olddiaNewdia[d] = d;
    app_->ui()->inform(QString("[success] %1: %2 %3").arg(tr_("Added new tool with dia"),
                                                          QString::fromStdString(pyFloatRepr(d)), defStr("units")));
    buildUi();
    for (const auto& [k, v] : tool2tooldia_)
        if (v == d) {
            lastToolSelected_ = k;
            ui_->tools_table_exc->selectRow(k - 1);
            break;
        }
}

void ExcEditor::deleteTools(const std::vector<double>& diasIn) {
    isModified_ = true;
    std::vector<double> dias;
    if (diasIn.empty()) {
        for (const QModelIndex& i : ui_->tools_table_exc->selectionModel()->selectedRows())
            if (auto* it = ui_->tools_table_exc->item(i.row(), 1)) dias.push_back(it->text().toDouble());
    } else {
        for (double d : diasIn) dias.push_back(dec(d));
    }
    for (double d : dias) {
        model_.drop(d);
        bool flag = false;
        for (auto it = tool2tooldia_.begin(); it != tool2tooldia_.end();) {
            if (it->second == d) {
                it = tool2tooldia_.erase(it);
                flag = true;
            } else {
                ++it;
            }
        }
        if (flag) model_.pointsEdit[d] = 0;
        model_.olddiaNewdia.erase(d);
        app_->ui()->inform(QString("[success] %1: %2 %3").arg(tr_("Deleted tool with diameter"),
                                                              QString::fromStdString(pyFloatRepr(d)), defStr("units")));
    }
    replot();
    buildUi();
}

void ExcEditor::rowSelected(int row) {
    const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
    if (QApplication::keyboardModifiers() != m) model_.selected.clear();
    const auto it = tool2tooldia_.find(row + 1);
    if (it != tool2tooldia_.end()) {
        lastToolSelected_ = row + 1;
        if (EditorStorage* st = model_.storage(it->second))
            for (const auto& o : st->objects()) model_.selected.push_back(o);
    }
    replot();
}

void ExcEditor::convertSlots() {
    std::set<int> rows;
    for (QTableWidgetItem* it : ui_->tools_table_exc->selectedItems()) rows.insert(it->row());
    for (int r : rows) {
        auto* it = ui_->tools_table_exc->item(r, 1);
        if (!it) continue;
        const double td = dec(it->text().toDouble());
        for (auto& t : model_.storages) {
            if (dec(t.dia) != td) continue;
            EditorStorage st;
            for (const auto& s : t.storage.objects()) {
                if (s->geo.type() == ShapeType::MultiLineString) st.insert(s);
                if (s->geo.type() == ShapeType::Polygon) st.insert(makeEditorShape(excdraw::drill(s->geo.centroid(), td)));
            }
            // storage_dict.update(new_storage_dict): la clave es el diámetro de la tabla.
            if (t.dia == td) t.storage = st;
            else model_.storages.push_back({td, st});
            break;
        }
    }
    replot();
}

void ExcEditor::selectTool(const QString& name) {
    auto it = buttons_.find(name);
    if (it == buttons_.end()) return;
    it->second->setChecked(true);
    onToolSelect(name);
}

void ExcEditor::onToolSelect(const QString& name) {
    if (!buttons_.count(name)) return;
    if (buttons_[name]->isChecked()) {
        for (auto& [n, a] : buttons_)
            if (n != name) a->setChecked(false);
        newTool(name);
    } else {
        for (auto& [n, a] : buttons_) a->setChecked(false);
        selectTool("drill_select");
    }
}

void ExcEditor::newTool(const QString& name) {
    MainGUI* ui = app_->ui();
    auto t = std::make_unique<Tool>();
    t->name = name;
    if (name == "drill_select") {
        for (QFrame* f : {ui_->resize_frame, ui_->array_frame, ui_->slot_frame, ui_->slot_array_frame}) f->hide();
    } else if (name == "drill_add" || name == "drill_array" || name == "slot_add" || name == "slot_array") {
        if (name == "drill_array") ui_->array_frame->show();
        if (name == "slot_add") ui_->slot_frame->show();
        if (name == "slot_array") {
            ui_->slot_frame->show();
            ui_->slot_array_frame->show();
        }
        t->dia = lastToolDia();
        if (!t->dia) {
            ui->inform("[WARNING_NOTCL] " + (name == "drill_add" ? tr_("To add a drill first select a tool")
                                                                 : tr_("To add an Drill Array first select a tool in Tool Table")));
            tool_ = std::move(t);
            if (name == "drill_add") selectTool("drill_select");
            return;
        }
        ui->inform(name == "drill_array" ? tr_("Click on target location ...") : tr_("Click to place ..."));
        ui->notebook->setCurrentWidget(ui->properties_tab);
    } else if (name == "drill_resize") {
        ui->inform(tr_("Click on the Drill(s) to resize ..."));
        ui_->resize_frame->show();
    } else if (name == "drill_move" || name == "drill_copy") {
        t->selectedDias = tableSelectedDias();
        ui->notebook->setCurrentWidget(ui->properties_tab);
        if (model_.selected.empty()) {
            ui->inform("[WARNING_NOTCL] " + tr_("Cancelled. Nothing selected."));
            tool_ = std::move(t);
            buttons_["drill_select"]->setChecked(true);
            onToolSelect("drill_select");
            return;
        }
        ui->inform(tr_("Click on reference location ..."));
    }
    tool_ = std::move(t);
}

void ExcEditor::complete() {
    if (!tool_ || !tool_->dia) return;
    EditorStorage& st = model_.ensure(*tool_->dia);
    for (const Shape& g : tool_->geometry) st.insert(makeEditorShape(g));
    deleteUtility();
    replot();
}

void ExcEditor::onPress(const QPointF& world, Qt::MouseButton button) {
    if (!active_ || button != Qt::LeftButton || !tool_) return;
    MainGUI* ui = app_->ui();
    pos_ = {world.x(), world.y()};
    ui->rel_position_label->setText("<b>Dx</b>: 0.0000&nbsp;&nbsp;  <b>Dy</b>: 0.0000&nbsp;&nbsp;&nbsp;&nbsp;");
    Tool& t = *tool_;
    const QString n = t.name;
    const int steps = defInt("geometry_circle_steps", 64);
    if ((n == "drill_add" || n == "slot_add") && t.dia && t.points) {
        const Point2D p = *t.points;
        if (n == "drill_add") {
            t.geometry = {excdraw::drill(p, *t.dia)};
            ++model_.pointsEdit[*t.dia];
        } else {
            t.geometry = {excdraw::slot(p, *t.dia, ui_->slot_length_entry->value(),
                                        ui_->slot_axis_radio->value().toString().at(0).toLatin1(),
                                        ui_->slot_angle_spinner->value(), steps, false)};
            ++model_.slotPointsEdit[*t.dia];
            ui_->slot_frame->hide();
        }
        t.complete = true;
        ui->inform("[success] " + tr_("Done."));
    } else if ((n == "drill_array" || n == "slot_array") && t.dia) {
        const bool slots = n == "slot_array";
        const QString type = (slots ? ui_->slot_array_type_radio : ui_->array_type_radio)->value().toString();
        const int size = slots ? ui_->slot_array_size_entry->value() : ui_->drill_array_size_entry->value();
        auto shapeAt = [&](const Point2D& p) {
            return slots ? excdraw::slot(p, *t.dia, ui_->slot_length_entry->value(),
                                         ui_->slot_axis_radio->value().toString().at(0).toLatin1(),
                                         ui_->slot_angle_spinner->value(), steps, true)
                         : excdraw::drill(p, *t.dia);
        };
        if (type == "linear") {
            if (!t.points) return;
            const char axis = (slots ? ui_->slot_array_axis_radio : ui_->drill_axis_radio)->value().toString().at(0).toLatin1();
            const double pitch = slots ? ui_->slot_array_pitch_entry->value() : ui_->drill_pitch_entry->value();
            const double ang = slots ? ui_->slot_array_linear_angle_spinner->value() : ui_->linear_angle_spinner->value();
            for (const Point2D& p : excdraw::linearArray(*t.points, size, axis, pitch, ang)) t.geometry.push_back(shapeAt(p));
        } else {
            const Point2D p{pos_.x, pos_.y};
            if (!t.circFlag) {
                inAction_ = true;
                t.circFlag = true;
                t.origin = p;
                ui->inform(tr_("Click on the Drill Circular Array Start position"));
                return;
            }
            t.circFlag = false;
            const double step = slots ? ui_->slot_array_angle_entry->value() : ui_->drill_angle_entry->value();
            if (step * size > 360) {
                ui->inform("[WARNING_NOTCL] " + tr_("Too many items for the selected spacing angle."));
                return;
            }
            const bool cw = (slots ? ui_->slot_array_direction_radio : ui_->drill_array_dir_radio)->value().toString() == "CW";
            const auto items = excdraw::circularArray(*t.origin, p, size, step, cw);
            if (!items) {
                ui->inform("[ERROR_NOTCL] " + tr_("Failed."));
                deleteUtility();
                selectTool("drill_select");
                return;
            }
            for (const auto& it : *items) {
                Shape g = shapeAt(it.p);
                if (slots) g = g.rotate(it.rotation * 180.0 / 3.14159265358979323846);
                t.geometry.push_back(g);
            }
        }
        if (slots) model_.slotPointsEdit[*t.dia] += size;
        else model_.pointsEdit[*t.dia] += size;
        t.complete = true;
        inAction_ = false;
        ui->inform("[success] " + tr_("Done."));
        (slots ? ui_->slot_array_frame : ui_->array_frame)->hide();
    } else if (n == "drill_move" || n == "drill_copy") {
        if (model_.selected.empty()) {
            ui->inform("[WARNING_NOTCL] " + tr_("Cancelled. Nothing selected."));
            return;
        }
        const Point2D p = pos_;
        if (!t.origin) {
            t.origin = p;
            ui->inform(tr_("Click on target location ..."));
            return;
        }
        const double dx = p.x - t.origin->x, dy = p.y - t.origin->y;
        for (double d : t.selectedDias) {
            EditorStorage* st = model_.storage(d);
            if (!st) continue;
            const auto objs = st->objects();
            std::vector<EditorShapePtr> done;
            for (const auto& s : model_.selected) {
                if (std::find(objs.begin(), objs.end(), s) == objs.end()) continue;
                st->insert(makeEditorShape(s->geo.translate(dx, dy)));
                if (n == "drill_move") {
                    st->remove(s);
                } else {
                    if (model_.pointsEdit.count(d)) ++model_.pointsEdit[d];
                    if (model_.slotPointsEdit.count(d)) ++model_.slotPointsEdit[d];
                }
                done.push_back(s);
            }
            for (const auto& s : done) model_.selected.erase(std::remove(model_.selected.begin(), model_.selected.end(), s), model_.selected.end());
        }
        buildUi();
        deleteUtility();
        replot();
        ui->inform("[success] " + tr_("Done."));
        selectTool("drill_select");
        return;
    }
    if (t.name != "drill_select" && t.complete) {
        complete();
        buildUi();
        const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
        // Con la tecla de elección múltiple se sigue con la misma herramienta; agregar agujero o ranura sigue siempre.
        if (QApplication::keyboardModifiers() == m || n == "drill_add" || n == "slot_add") selectTool(n);
        else selectTool("drill_select");
        return;
    }
    if (t.name == "drill_select") replot();
}

void ExcEditor::onMove(const QPointF& world, bool leftDragging) {
    if (!active_) return;
    MainGUI* ui = app_->ui();
    const Point2D p{world.x(), world.y()};
    mouse_ = p;
    const double dx = p.x - pos_.x, dy = p.y - pos_.y;
    ui->position_label->setText(QString("&nbsp;<b>X</b>: %1&nbsp;&nbsp;   <b>Y</b>: %2&nbsp;").arg(p.x, 0, 'f', 4).arg(p.y, 0, 'f', 4));
    ui->rel_position_label->setText(QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;").arg(dx, 0, 'f', 4).arg(dy, 0, 'f', 4));
    updateUtility(p);
    if (leftDragging) {
        PlotLayerData d;
        PlotGroup g;
        QRectF ignored;
        FlatCAMObj::addShapeTo(Shape::polygon({pos_, {p.x, pos_.y}, p, {pos_.x, p.y}}), g, ignored, true);
        const bool left = dx < 0;
        g.lineColor = colorFromFlatCam(defStr(left ? "global_alt_sel_line" : "global_sel_line"));
        g.fillColor = colorFromFlatCam(defStr(left ? "global_alt_sel_fill" : "global_sel_fill").chopped(2) + "4c");
        d.groups.push_back(g);
        ui->plotCanvas()->setLayer(kSelRectLayer, std::move(d));
        selectionType_ = !left;
    } else {
        selectionType_.reset();
    }
}

void ExcEditor::updateUtility(const Point2D& data) {
    if (!tool_) return;
    Tool& t = *tool_;
    const int steps = defInt("geometry_circle_steps", 64);
    ShapeList geo;
    if (t.name == "drill_add" && t.dia) {
        t.points = data;
        geo = {excdraw::drill(data, *t.dia)};
    } else if (t.name == "slot_add" && t.dia) {
        t.points = data;
        geo = {excdraw::slot(data, *t.dia, ui_->slot_length_entry->value(),
                             ui_->slot_axis_radio->value().toString().at(0).toLatin1(), ui_->slot_angle_spinner->value(),
                             steps, false)};
    } else if ((t.name == "drill_array" || t.name == "slot_array") && t.dia) {
        const bool slots = t.name == "slot_array";
        const QString type = (slots ? ui_->slot_array_type_radio : ui_->array_type_radio)->value().toString();
        const int size = slots ? ui_->slot_array_size_entry->value() : ui_->drill_array_size_entry->value();
        auto shapeAt = [&](const Point2D& p) {
            return slots ? excdraw::slot(p, *t.dia, ui_->slot_length_entry->value(),
                                         ui_->slot_axis_radio->value().toString().at(0).toLatin1(),
                                         ui_->slot_angle_spinner->value(), steps, true)
                         : excdraw::drill(p, *t.dia);
        };
        if (type == "linear") {
            t.points = data;
            const char axis = (slots ? ui_->slot_array_axis_radio : ui_->drill_axis_radio)->value().toString().at(0).toLatin1();
            const double pitch = slots ? ui_->slot_array_pitch_entry->value() : ui_->drill_pitch_entry->value();
            const double ang = slots ? ui_->slot_array_linear_angle_spinner->value() : ui_->linear_angle_spinner->value();
            for (const Point2D& p : excdraw::linearArray(data, size, axis, pitch, ang)) geo.push_back(shapeAt(p));
        } else if (t.origin) {
            const double step = slots ? ui_->slot_array_angle_entry->value() : ui_->drill_angle_entry->value();
            const bool cw = (slots ? ui_->slot_array_direction_radio : ui_->drill_array_dir_radio)->value().toString() == "CW";
            if (const auto items = excdraw::circularArray(*t.origin, data, size, step, cw)) {
                for (const auto& it : *items) {
                    Shape g = shapeAt(it.p);
                    if (slots) g = g.rotate(it.rotation * 180.0 / 3.14159265358979323846);
                    geo.push_back(g);
                }
                geo.push_back(Shape::lineString({*t.origin, data}));
            } else {
                deleteUtility();
            }
        }
    } else if ((t.name == "drill_move" || t.name == "drill_copy") && t.origin && !model_.selected.empty()) {
        const double dx = data.x - t.origin->x, dy = data.y - t.origin->y;
        for (const auto& s : model_.selected) geo.push_back(s->geo.translate(dx, dy));
    }
    if (geo.empty()) return;
    PlotLayerData d;
    PlotGroup g;
    QRectF ignored;
    for (const Shape& s : geo) FlatCAMObj::addShapeTo(s, g, ignored, false);
    g.lineColor = colorFromFlatCam(defStr("global_draw_color") + "80");
    d.groups.push_back(g);
    app_->ui()->plotCanvas()->setLayer(kToolLayer, std::move(d));
}

void ExcEditor::deleteUtility() { app_->ui()->plotCanvas()->removeLayer(kToolLayer); }

void ExcEditor::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!active_ || !tool_) return;
    MainGUI* ui = app_->ui();
    const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
    if (button == Qt::RightButton && !dragged) {
        if (!tool_->complete && tool_->name != "drill_select") {
            tool_->complete = true;
            inAction_ = false;
            deleteUtility();
            ui->inform("[success] " + tr_("Done."));
            selectTool("drill_select");
        } else {
            app_->popupCanvasMenu();
        }
        return;
    }
    if (button != Qt::LeftButton) return;
    if (selectionType_) {
        ui->plotCanvas()->removeLayer(kSelRectLayer);
        model_.areaSelect(pos_, {world.x(), world.y()}, *selectionType_, QApplication::keyboardModifiers() == m);
        selectionType_.reset();
        selectRowsOfSelection();
        replot();
    } else if (tool_->name == "drill_select") {
        ui_->tools_table_exc->clearSelection();
        model_.clickSelect(pos_, QApplication::keyboardModifiers() == m);
        selectRowsOfSelection();
        if (!model_.selected.empty()) replot();
        else replot();
    }
}

void ExcEditor::selectRowsOfSelection() {
    // Las filas de los diámetros de lo elegido.
    QSignalBlocker block(ui_->tools_table_exc);
    ui_->tools_table_exc->clearSelection();
    ui_->tools_table_exc->setSelectionMode(QAbstractItemView::MultiSelection);
    for (const auto& s : model_.selected)
        if (const auto d = model_.diaOf(s))
            for (const auto& [k, v] : tool2tooldia_)
                if (v == *d) {
                    ui_->tools_table_exc->selectRow(k - 1);
                    lastToolSelected_ = k;
                    break;
                }
    ui_->tools_table_exc->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void ExcEditor::deleteSelected() {
    isModified_ = true;
    const auto sel = model_.selected;
    for (const auto& s : sel) {
        if (const auto d = model_.diaOf(s)) {
            if (s->geo.type() == ShapeType::MultiLineString && model_.pointsEdit[*d] > 0) --model_.pointsEdit[*d];
            if (s->geo.type() == ShapeType::Polygon && model_.slotPointsEdit[*d] > 0) --model_.slotPointsEdit[*d];
        }
        model_.remove(s);
    }
    model_.selected.clear();
    buildUi();
    replot();
}

void ExcEditor::replot() {
    PlotLayerData d;
    PlotGroup normal, sel;
    QRectF ignored;
    for (const auto& t : model_.storages)
        for (const auto& s : t.storage.objects()) {
            if (s->geo.isEmpty()) continue;
            const bool isSel = std::find(model_.selected.begin(), model_.selected.end(), s) != model_.selected.end();
            FlatCAMObj::addShapeTo(s->geo, isSel ? sel : normal, ignored, false);
        }
    normal.lineColor = colorFromFlatCam(defStr("global_draw_color") + "FF");
    sel.lineColor = colorFromFlatCam(defStr("global_sel_draw_color") + "FF");
    d.groups = {normal, sel};
    app_->ui()->plotCanvas()->setLayer(kShapesLayer, std::move(d));
}

bool ExcEditor::handleKey(int key, Qt::KeyboardModifiers mods) {
    MainGUI* ui = app_->ui();
    if (mods == Qt::ControlModifier) {
        if (key == Qt::Key_S) app_->editorToObject();
        else if (key == Qt::Key_M) app_->distanceTool->run();
        else return false;
        return true;
    }
    if (mods == Qt::ShiftModifier) return false;
    if (mods != Qt::NoModifier && mods != Qt::KeypadModifier) return false;
    switch (key) {
        case Qt::Key_Escape:
            deleteUtility();
            model_.selected.clear();
            ui_->tools_table_exc->clearSelection();
            replot();
            selectTool("drill_select");
            return true;
        case Qt::Key_Delete: deleteTools(); return true;  // on_tool_delete (las filas elegidas)
        case Qt::Key_Minus: ui->plotCanvas()->zoom(1.0 / defNum("global_zoom_ratio", 1.5), {mouse_.x, mouse_.y}); return true;
        case Qt::Key_Equal: ui->plotCanvas()->zoom(defNum("global_zoom_ratio", 1.5), {mouse_.x, mouse_.y}); return true;
        case Qt::Key_QuoteLeft: ui->onToggleNotebook(); return true;
        case Qt::Key_1: ui->notebook->setCurrentWidget(ui->project_tab); return true;
        case Qt::Key_2: ui->notebook->setCurrentWidget(ui->properties_tab); return true;
        case Qt::Key_3: ui->notebook->setCurrentWidget(ui->tool_tab); return true;
        case Qt::Key_G: ui->grid_snap_btn->trigger(); return true;
        case Qt::Key_K: ui->corner_snap_btn->trigger(); return true;
        case Qt::Key_V: app_->onZoomFit(); return true;
        case Qt::Key_W: selectTool("slot_add"); return true;
        case Qt::Key_F3: app_->onShortcutList(); return true;
        default: break;
    }
    if (tool_ && !ui->select_drill_btn->isChecked()) {
        // on_key de la herramienta: Espacio cambia el eje de la grilla o de la ranura.
        if (key == Qt::Key_Space && tool_->name == "drill_array") {
            RadioSet* r = ui_->drill_axis_radio;
            const QString v = r->value().toString();
            r->setValue(v == "X" ? QString("Y") : v == "Y" ? QString("A") : QString("X"));
            updateUtility(mouse_);
        } else if (key == Qt::Key_Space && (tool_->name == "slot_add" || tool_->name == "slot_array")) {
            RadioSet* r = ui_->slot_axis_radio;
            const QString v = r->value().toString();
            r->setValue(v == "X" ? QString("Y") : v == "Y" ? QString("A") : QString("X"));
            updateUtility(mouse_);
        } else if (key == Qt::Key_J) {
            app_->onJumpTo();
        }
        return true;
    }
    switch (key) {
        case Qt::Key_A:
            ui->inform("Click on target point.");
            selectTool("drill_array");
            return true;
        case Qt::Key_C:
            if (!model_.selected.empty()) {
                ui->inform(tr_("Click on target point."));
                ui->copy_drill_btn->setChecked(true);
                onToolSelect("drill_copy");
                if (tool_) tool_->origin = mouse_;
            } else {
                ui->inform("[WARNING_NOTCL] " + tr_("Cancelled. Nothing selected."));
            }
            return true;
        case Qt::Key_D:
            ui->inform(tr_("Click on target point."));
            selectTool("drill_add");
            return true;
        case Qt::Key_J: app_->onJumpTo(); return true;
        case Qt::Key_M:
            if (!model_.selected.empty()) {
                ui->inform(tr_("Click on target location ..."));
                ui->move_drill_btn->setChecked(true);
                onToolSelect("drill_move");
                if (tool_) tool_->origin = mouse_;
            } else {
                ui->inform("[WARNING_NOTCL] " + tr_("Cancelled. Nothing selected."));
            }
            return true;
        case Qt::Key_Q:
            ui->inform("Click on target point.");
            selectTool("slot_array");
            return true;
        case Qt::Key_R: selectTool("drill_resize"); return true;
        case Qt::Key_T: {
            FCInputDoubleSpinner box(ui, tr_("New Tool ..."), tr_("Enter a Tool Diameter") + ":", 0.0, 100.0, 1, 4,
                                     defNum("excellon_editor_newdia"));
            box.setWindowIcon(QIcon(":/images/letter_t_32.png"));
            if (box.exec() == QDialog::Accepted) {
                addTool(box.wdg->value());
                ui->inform("[success] " + tr_("Added new tool with dia") + ": " + QString::number(box.wdg->value()));
            } else {
                ui->inform("[WARNING_NOTCL] " + tr_("Adding Tool cancelled") + "...");
            }
            return true;
        }
        default: return true;
    }
}

}  // namespace flatcam::gui
