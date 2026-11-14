#include "gui/editors/GrbEditor.h"

#include <QApplication>
#include <QHeaderView>
#include <QScrollArea>
#include <QSplitter>
#include <QTabWidget>

#include "cam/PyNumber.h"
#include "core/ArcMath.h"
#include "core/GeometryError.h"
#include "editors/ExcEditorModel.h"
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

constexpr int kShapesLayer = -13;
constexpr int kToolLayer = -14;
constexpr int kSelRectLayer = -15;
constexpr double kPi = 3.14159265358979323846;

}  // namespace

/// La herramienta activa del editor de Gerber.
struct GrbEditor::Tool {
    QString name;
    bool complete{false};
    bool dontExecute{false};
    std::vector<Point2D> points;
    std::vector<Point2D> tempPoints;     ///< `temp_points` de la pista y la región.
    std::optional<Point2D> interPoint;   ///< `inter_point` de la región.
    std::optional<Point2D> last;         ///< El último dato de la vista previa (`self.points` del pad).
    std::optional<Point2D> origin;       ///< Centro de la grilla circular u origen de mover.
    bool circFlag{false};
    double bufVal{0.0000001};
    int semiMode{0};                     ///< 0 c12, 1 132, 2 12c.
    bool cw{true};
    std::vector<std::string> codes;      ///< Aperturas elegidas (mover, copiar).
    std::vector<ApertureGeometry> geometry;
    std::string target;                  ///< La apertura donde va lo creado.
};

GrbEditor::GrbEditor(App* app) : QObject(app), app_(app) {
    MainGUI* ui = app_->ui();
    ui_ = new GrbEditorUIBase();
    ui_->setupUi();
    transformUi_ = new GrbTransformToolUIBase();
    transformUi_->setupUi();
    buttons_ = {{"select", ui->grb_select_btn},          {"pad", ui->grb_add_pad_btn},
                {"array", ui->add_pad_ar_btn},           {"track", ui->grb_add_track_btn},
                {"region", ui->grb_add_region_btn},      {"poligonize", ui->grb_convert_poly_btn},
                {"semidisc", ui->grb_add_semidisc_btn},  {"disc", ui->grb_add_disc_btn},
                {"buffer", ui->aperture_buffer_btn},     {"scale", ui->aperture_scale_btn},
                {"markarea", ui->aperture_markarea_btn}, {"eraser", ui->aperture_eraser_btn},
                {"copy", ui->aperture_copy_btn},         {"transform", ui->grb_transform_btn},
                {"move", ui->aperture_move_btn}};
    for (auto& [name, a] : buttons_) {
        a->setCheckable(true);
        const QString n = name;
        connect(a, &QAction::triggered, this, [this, n] { if (active_) onToolSelect(n); });
    }
    auto onAct = [this](QAction* a, auto fn) { connect(a, &QAction::triggered, this, [this, fn] { if (active_) fn(); }); };
    onAct(ui->grb_add_pad_menuitem, [this] { selectTool("pad"); });
    onAct(ui->grb_add_pad_array_menuitem, [this] { selectTool("array"); });
    onAct(ui->grb_add_track_menuitem, [this] { selectTool("track"); });
    onAct(ui->grb_add_region_menuitem, [this] { selectTool("region"); });
    onAct(ui->grb_convert_poly_menuitem, [this] { selectTool("poligonize"); });
    onAct(ui->grb_add_semidisc_menuitem, [this] { selectTool("semidisc"); });
    onAct(ui->grb_add_disc_menuitem, [this] { selectTool("disc"); });
    onAct(ui->grb_add_buffer_menuitem, [this] { selectTool("buffer"); });
    onAct(ui->grb_add_scale_menuitem, [this] { selectTool("scale"); });
    onAct(ui->grb_add_markarea_menuitem, [this] { selectTool("markarea"); });
    onAct(ui->grb_add_eraser_menuitem, [this] { selectTool("eraser"); });
    onAct(ui->grb_transform_menuitem, [this] { selectTool("transform"); });
    onAct(ui->grb_copy_menuitem, [this] { selectTool("copy"); });
    onAct(ui->grb_move_menuitem, [this] { selectTool("move"); });
    onAct(ui->grb_delete_menuitem, [this] {
        model_.deleteSelected();
        buildUi();
        replot();
    });
    connect(ui_->exit_editor_button, &QPushButton::clicked, this, [this] { app_->editorToObject(); });
    connect(ui_->addaperture_btn, &QPushButton::clicked, this, [this] { addAperture(); });
    connect(ui_->delaperture_btn, &QPushButton::clicked, this, [this] { deleteApertures(); });
    connect(ui_->name_entry, &QLineEdit::returnPressed, this, [this] { editedName_ = ui_->name_entry->text(); });
    connect(ui_->apertures_table, &QTableWidget::cellPressed, this, [this](int row, int) { rowSelected(row); });
    connect(ui_->aptype_cb, &QComboBox::currentTextChanged, this, [this](const QString& t) {
        const bool dim = t == "R" || t == "O";
        ui_->apdim_entry->setEnabled(dim);
        ui_->apsize_entry->setEnabled(!dim);
    });
    connect(ui_->buffer_button, &QPushButton::clicked, this, [this] {
        // on_buffer
        if (tableSelectedCodes().empty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("No aperture to buffer. Select at least one aperture and try again."));
            return;
        }
        try {
            model_.buffer(tableSelectedCodes(), ui_->buffer_distance_entry->value(), ui_->buffer_corner_cb->currentIndex() + 1);
        } catch (const GeometryError&) {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Failed."));
            return;
        }
        model_.selected.clear();
        replot();
        app_->ui()->inform("[success] " + tr_("Done."));
    });
    connect(ui_->scale_button, &QPushButton::clicked, this, [this] {
        if (tableSelectedCodes().empty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("No aperture to scale. Select at least one aperture and try again."));
            return;
        }
        model_.scale(tableSelectedCodes(), ui_->scale_factor_entry->value());
        model_.selected.clear();
        replot();
        app_->ui()->inform("[success] " + tr_("Done."));
    });
    connect(ui_->ma_threshold_button, &QPushButton::clicked, this, [this] {
        marked_ = model_.markArea(ui_->ma_lower_threshold_entry->value(), ui_->ma_upper_threshold_entry->value());
        model_.selected = marked_;
        replot();
        app_->ui()->inform(marked_.empty() ? "[WARNING_NOTCL] " + tr_("Nothing selected.")
                                           : QString("[success] %1 %2").arg(marked_.size()).arg(tr_("Polygons marked.")));
    });
    connect(ui_->ma_delete_button, &QPushButton::clicked, this, [this] {
        for (const auto& s : marked_) model_.remove(s);
        marked_.clear();
        buildUi();
        replot();
    });
    connect(ui_->ma_clear_button, &QPushButton::clicked, this, [this] {
        marked_.clear();
        model_.selected.clear();
        replot();
    });
    connect(transformUi_->rotate_button, &QPushButton::clicked, this, [this] { transform("rotate"); });
    connect(transformUi_->flipx_button, &QPushButton::clicked, this, [this] { transform("flipx"); });
    connect(transformUi_->flipy_button, &QPushButton::clicked, this, [this] { transform("flipy"); });
    connect(transformUi_->skewx_button, &QPushButton::clicked, this, [this] { transform("skewx"); });
    connect(transformUi_->skewy_button, &QPushButton::clicked, this, [this] { transform("skewy"); });
    connect(transformUi_->scalex_button, &QPushButton::clicked, this, [this] { transform("scalex"); });
    connect(transformUi_->scaley_button, &QPushButton::clicked, this, [this] { transform("scaley"); });
    connect(transformUi_->offx_button, &QPushButton::clicked, this, [this] { transform("offx"); });
    connect(transformUi_->offy_button, &QPushButton::clicked, this, [this] { transform("offy"); });
    connect(transformUi_->buffer_button, &QPushButton::clicked, this, [this] { transform("buffer"); });
    connect(transformUi_->buffer_factor_button, &QPushButton::clicked, this, [this] { transform("bufferfactor"); });
}

GrbEditor::~GrbEditor() {
    delete ui_;
    delete transformUi_;
}

QString GrbEditor::activeToolName() const { return tool_ ? tool_->name : QString(); }

std::vector<std::string> GrbEditor::tableSelectedCodes() const {
    std::vector<std::string> out;
    for (QTableWidgetItem* it : ui_->apertures_table->selectedItems())
        if (QTableWidgetItem* c = ui_->apertures_table->item(it->row(), 1)) {
            const std::string code = c->text().toStdString();
            if (std::find(out.begin(), out.end(), code) == out.end()) out.push_back(code);
        }
    return out;
}

void GrbEditor::edit(GerberObject* obj) {
    MainGUI* ui = app_->ui();
    deactivate();
    active_ = true;
    obj_ = obj;
    toolbarOldState_ = ui->grb_edit_toolbar->isVisible();
    ui->menueditedit->setDisabled(true);
    ui->menueditok->setDisabled(false);
    ui->grb_editor_menu->setDisabled(false);
    ui->grb_editor_menu->menuAction()->setVisible(true);
    ui->update_obj_btn->setEnabled(true);
    ui->grb_edit_toolbar->setDisabled(false);
    ui->grb_edit_toolbar->setVisible(true);
    ui->setCanvasHandlersEnabled(false);
    ui->plotCanvas()->setLayerVisible(obj->id(), false);
    model_.load(obj->gerber());
    editedName_ = obj->name();
    lastAperture_.reset();
    for (QFrame* f : {ui_->array_frame, ui_->buffer_tool_frame, ui_->scale_tool_frame, ui_->ma_tool_frame}) f->hide();
    selectTool("select");
    buildUi();
    replot();
}

void GrbEditor::deactivate() {
    MainGUI* ui = app_->ui();
    if (active_) ui->setCanvasHandlersEnabled(true);
    ui->menueditedit->setDisabled(false);
    ui->menueditok->setDisabled(true);
    ui->grb_edit_toolbar->setDisabled(true);
    ui->grb_edit_toolbar->setVisible(toolbarOldState_);
    ui->grb_editor_menu->setDisabled(true);
    ui->grb_editor_menu->menuAction()->setVisible(false);
    ui->update_obj_btn->setEnabled(false);
    for (int id : {kShapesLayer, kToolLayer, kSelRectLayer}) ui->plotCanvas()->removeLayer(id);
    for (auto& [n, a] : buttons_) a->setChecked(false);
    tool_.reset();
    model_.selected.clear();
    active_ = false;
}

void GrbEditor::save(GerberObject* obj) {
    // update_fcgerber: siempre un Gerber nuevo, "<nombre>_edit" (o el número siguiente).
    QString name = editedName_.isEmpty() ? obj->name() : editedName_;
    if (name.contains("_edit")) {
        bool ok = false;
        const int idd = name.right(1).toInt(&ok);
        name = ok ? name.left(name.size() - 1) + QString::number(idd + 1) : name + "_1";
    } else {
        name += "_edit";
    }
    auto out = model_.result();
    auto g = std::make_unique<Gerber>(obj->gerber());
    std::map<std::string, Aperture> aps = out.apertures;
    g->restore(std::move(out.solid), std::move(out.follow), {}, obj->gerber().intDigits(), obj->gerber().fracDigits(),
               std::string());
    g->replaceGeometry(g->solidGeometry(), std::move(aps), out.order);
    auto o = std::make_unique<GerberObject>(name, std::move(g));
    lastCreated_ = app_->addObject(std::move(o));
    app_->refreshSourceFile(lastCreated_);
    deactivate();
    app_->ui()->plotCanvas()->setLayerVisible(obj->id(), obj->plotEnabled());
    // editor2object: el Gerber de origen sin geometría (uno vacío de Archivo -> Nuevo) se borra al salir.
    if (obj->gerber().solidGeometry().empty()) app_->requestDeleteEditedObject();
    app_->ui()->inform("[success] " + tr_("Done."));
}

void GrbEditor::buildUi() {
    FCTable* t = ui_->apertures_table;
    QSignalBlocker block(t);
    ui_->name_entry->setValue(editedName_);
    std::vector<int> codes;
    for (const auto& a : model_.apertures) codes.push_back(std::stoi(a.code));
    std::sort(codes.begin(), codes.end());
    t->clear();
    t->setColumnCount(5);
    t->setHorizontalHeaderLabels({"#", tr_("Code"), tr_("Type"), tr_("Size"), tr_("Dim")});
    t->setRowCount(static_cast<int>(codes.size()));
    int row = 0;
    for (int c : codes) {
        const auto* a = model_.aperture(std::to_string(c));
        t->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        t->setItem(row, 1, new QTableWidgetItem(QString::number(c)));
        t->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(a->ap.type)));
        QString dim;
        if (a->ap.type == "R" || a->ap.type == "O")
            dim = QString("%1, %2").arg(a->ap.width, 0, 'f', 4).arg(a->ap.height, 0, 'f', 4);
        else if (a->ap.type == "P")
            dim = QString("%1, %2").arg(a->ap.diam, 0, 'f', 4).arg(a->ap.nVertices);
        t->setItem(row, 3, new QTableWidgetItem(a->ap.size ? QString::number(*a->ap.size, 'f', 4) : QString()));
        t->setItem(row, 4, new QTableWidgetItem(dim));
        for (int k = 0; k < 5; ++k) t->item(row, k)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ++row;
    }
    t->resizeColumnsToContents();
    t->resizeRowsToContents();
    t->verticalHeader()->hide();
    t->clearSelection();
    MainGUI* ui = app_->ui();
    ui->properties_scroll_area->takeWidget();
    ui->properties_scroll_area->setWidget(ui_);
    ui->notebook->setCurrentWidget(ui->properties_tab);
}

void GrbEditor::addAperture(const QString& codeIn) {
    MainGUI* ui = app_->ui();
    const std::string code = codeIn.isEmpty() ? QString::number(ui_->apcode_entry->value()).toStdString() : codeIn.toStdString();
    if (code.empty()) return;
    if (code == "0") {
        model_.ensureZero();
    } else if (model_.aperture(code)) {
        ui->inform("[WARNING_NOTCL] " + tr_("Aperture already in the aperture table."));
        return;
    } else {
        GrbEditorModel::Ap a;
        a.code = code;
        a.ap.type = ui_->aptype_cb->currentText().toStdString();
        if (a.ap.type == "R" || a.ap.type == "O") {
            const QStringList d = ui_->apdim_entry->text().split(',');
            if (d.size() != 2) {
                ui->inform("[WARNING_NOTCL] " + tr_("Aperture dimensions value is missing or wrong format. Add it in "
                                                    "format (width, height) and retry."));
                return;
            }
            a.ap.width = d[0].trimmed().toDouble();
            a.ap.height = d[1].trimmed().toDouble();
            a.ap.size = std::sqrt(a.ap.width * a.ap.width + a.ap.height * a.ap.height);
        } else {
            a.ap.size = ui_->apsize_entry->value();
        }
        a.ap.hasGeometry = true;
        model_.apertures.push_back(a);
        ui->inform("[success] " + tr_("Added new aperture with code") + ": " + QString::fromStdString(code));
    }
    lastAperture_ = code;
    buildUi();
    for (int r = 0; r < ui_->apertures_table->rowCount(); ++r)
        if (ui_->apertures_table->item(r, 1)->text().toStdString() == code) ui_->apertures_table->selectRow(r);
}

void GrbEditor::deleteApertures() {
    std::vector<std::string> codes = tableSelectedCodes();
    if (codes.empty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Select an aperture in Aperture Table"));
        return;
    }
    for (const std::string& c : codes) {
        model_.apertures.erase(std::remove_if(model_.apertures.begin(), model_.apertures.end(),
                                              [&](const GrbEditorModel::Ap& a) { return a.code == c; }),
                               model_.apertures.end());
        app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Deleted aperture with code"), QString::fromStdString(c)));
    }
    model_.selected.clear();
    buildUi();
    replot();
}

void GrbEditor::rowSelected(int row) {
    const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
    if (QApplication::keyboardModifiers() != m) model_.selected.clear();
    QTableWidgetItem* it = ui_->apertures_table->item(row, 1);
    if (!it) return;
    lastAperture_ = it->text().toStdString();
    if (auto* a = model_.aperture(*lastAperture_))
        for (const auto& s : a->geometry) model_.selected.push_back(s);
    replot();
}

void GrbEditor::selectTool(const QString& name) {
    auto it = buttons_.find(name);
    if (it == buttons_.end()) return;
    it->second->setChecked(true);
    onToolSelect(name);
}

void GrbEditor::onToolSelect(const QString& name) {
    if (!buttons_.count(name)) return;
    if (buttons_[name]->isChecked()) {
        for (auto& [n, a] : buttons_)
            if (n != name) a->setChecked(false);
        newTool(name);
    } else {
        for (auto& [n, a] : buttons_) a->setChecked(false);
        selectTool("select");
    }
}

void GrbEditor::newTool(const QString& name) {
    MainGUI* ui = app_->ui();
    auto t = std::make_unique<Tool>();
    t->name = name;
    auto apSize = [this]() -> std::optional<double> {
        if (!lastAperture_) return std::nullopt;
        const auto* a = model_.aperture(*lastAperture_);
        return a && a->ap.size ? a->ap.size : std::nullopt;
    };
    for (QFrame* f : {ui_->array_frame, ui_->buffer_tool_frame, ui_->scale_tool_frame, ui_->ma_tool_frame}) f->hide();
    if (name == "pad" || name == "array") {
        if (!lastAperture_) {
            ui->inform("[WARNING_NOTCL] " + tr_("You need to preselect a aperture in the Aperture Table that has a size."));
            tool_ = std::move(t);
            selectTool("select");
            return;
        }
        const auto* a = model_.aperture(*lastAperture_);
        if (a && a->ap.type != "C" && a->ap.type != "R" && a->ap.type != "O") {
            ui->inform("[WARNING_NOTCL] " + tr_("Incompatible aperture type. Select an aperture with type 'C', 'R' or 'O'."));
            tool_ = std::move(t);
            selectTool("select");
            return;
        }
        t->target = *lastAperture_;
        if (name == "array") ui_->array_frame->show();
        ui->inform(tr_("Click to place ..."));
    } else if (name == "track" || name == "region" || name == "disc" || name == "semidisc") {
        const auto sz = apSize();
        if (!sz) {
            ui->inform("[WARNING_NOTCL] " + tr_("You need to preselect a aperture in the Aperture Table that has a size."));
            t->dontExecute = true;
            t->complete = true;
            tool_ = std::move(t);
            selectTool("select");
            return;
        }
        t->bufVal = *sz > 0 ? *sz / 2 : 0.0000001;
        t->target = (name == "track") ? *lastAperture_ : "0";
        if (name != "track") model_.ensureZero();
        ui->inform(name == "track" || name == "region" ? tr_("Click on 1st point ...") : tr_("Click on Center point ..."));
    } else if (name == "buffer") {
        ui_->buffer_tool_frame->show();
    } else if (name == "scale") {
        ui_->scale_tool_frame->show();
    } else if (name == "markarea") {
        ui_->ma_tool_frame->show();
    } else if (name == "transform") {
        app_->runTool(transformUi_, {tr_("Transform Tool"), tr_("Transform Tool")});
    } else if (name == "poligonize") {
        tool_ = std::move(t);
        const std::string last = lastAperture_.value_or("0");
        if (!model_.poligonize(last)) {
            ui->inform("[WARNING_NOTCL] " + (model_.selected.empty()
                                                 ? tr_("Failed. Nothing selected.")
                                                 : tr_("Failed. Poligonize works only on geometries belonging to the same aperture.")));
        } else {
            buildUi();
            replot();
            ui->inform("[success] " + tr_("Done."));
        }
        selectTool("select");
        return;
    } else if (name == "move" || name == "copy" || name == "eraser") {
        t->codes = tableSelectedCodes();
        if (t->codes.empty())
            for (const auto& s : model_.selected)
                if (const auto c = model_.codeOf(s))
                    if (std::find(t->codes.begin(), t->codes.end(), *c) == t->codes.end()) t->codes.push_back(*c);
        ui->inform(model_.selected.empty() ? "[WARNING_NOTCL] " + tr_("Cancelled. No shape selected.")
                                           : tr_("Click on reference location ..."));
    }
    tool_ = std::move(t);
}

void GrbEditor::finishTool() {
    if (!tool_) return;
    for (const ApertureGeometry& e : tool_->geometry) model_.add(tool_->target, e);
    tool_->geometry.clear();
    deleteUtility();
    buildUi();
    replot();
}

void GrbEditor::onPress(const QPointF& world, Qt::MouseButton button) {
    if (!active_ || button != Qt::LeftButton || !tool_) return;
    MainGUI* ui = app_->ui();
    pos_ = {world.x(), world.y()};
    Tool& t = *tool_;
    const QString n = t.name;
    const int steps = defInt("geometry_circle_steps", 64);
    const int gsteps = defInt("gerber_circle_steps", 64);
    if (n == "select") {
        const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
        if (QApplication::keyboardModifiers() != m) model_.selected.clear();
        return;
    }
    if (n == "pad" && t.last) {
        const auto* a = model_.aperture(t.target);
        if (const auto e = a ? grbdraw::pad(a->ap, *t.last, steps) : std::nullopt) t.geometry = {*e};
        t.complete = true;
    } else if (n == "array") {
        const auto* a = model_.aperture(t.target);
        if (!a) return;
        const int size = ui_->pad_array_size_entry->value();
        if (ui_->array_type_radio->value().toInt() == 0) {
            if (!t.last) return;
            const char axis = ui_->pad_axis_radio->value().toString().at(0).toLatin1();
            for (const Point2D& p : excdraw::linearArray(*t.last, size, axis, ui_->pad_pitch_entry->value(),
                                                         ui_->linear_angle_spinner->value()))
                if (const auto e = grbdraw::pad(a->ap, p, steps)) t.geometry.push_back(*e);
        } else {
            if (!t.circFlag) {
                inAction_ = true;
                t.circFlag = true;
                t.origin = pos_;
                return;
            }
            t.circFlag = false;
            const double step = ui_->pad_angle_entry->value();
            if (step * size > 360) {
                ui->inform("[WARNING_NOTCL] " + tr_("Too many items for the selected spacing angle."));
                return;
            }
            const auto items = excdraw::circularArray(*t.origin, pos_, size, step,
                                                      ui_->pad_direction_radio->value().toString() == "CW");
            if (!items) {
                ui->inform("[ERROR_NOTCL] " + tr_("Failed."));
                selectTool("select");
                return;
            }
            for (const auto& it : *items)
                if (auto e = grbdraw::pad(a->ap, it.p, steps)) {
                    e->solid = e->solid->rotate(it.rotation * 180.0 / kPi);
                    t.geometry.push_back(*e);
                }
        }
        t.complete = true;
        inAction_ = false;
        ui_->array_frame->hide();
    } else if (n == "track") {
        inAction_ = true;
        if (!t.points.empty() && t.points.back() == pos_) return;
        t.points.push_back(pos_);
        // El tramo de la vista previa va enseguida a la apertura.
        ApertureGeometry e;
        if (t.tempPoints.size() == 1 || t.tempPoints.empty()) {
            const Shape pt = Shape::point(t.tempPoints.empty() ? pos_ : t.tempPoints.front());
            e.solid = pt.buffer(t.bufVal, gsteps);
            e.follow = pt;
        } else {
            const Shape l = Shape::lineString(t.tempPoints);
            e.solid = l.buffer(t.bufVal, gsteps);
            e.follow = l;
        }
        model_.add(t.target, e);
        replot();
        ui->inform(tr_("Click on next Point or click right mouse button to complete ..."));
        return;
    } else if (n == "region") {
        inAction_ = true;
        if (t.interPoint) t.points.push_back(*t.interPoint);
        t.points.push_back(pos_);
        ui->inform(tr_("Click on next Point or click right mouse button to complete ..."));
        return;
    } else if (n == "disc") {
        t.points.push_back(pos_);
        if (t.points.size() < 2) {
            ui->inform(tr_("Click on Perimeter point to complete ..."));
            return;
        }
        const double r = distance(t.points[0], t.points[1]) + t.bufVal / 2;
        const Shape d = Shape::point(t.points[0]).buffer(r, steps / 4);
        ApertureGeometry e;
        e.solid = d;
        e.follow = d.exterior();
        t.geometry = {e};
        t.complete = true;
    } else if (n == "semidisc") {
        t.points.push_back(pos_);
        if (t.points.size() < 3) return;
        if (const auto e = grbdraw::semiDisc(t.semiMode, t.points[0], t.points[1], t.points[2], t.cw, t.bufVal, steps))
            t.geometry = {*e};
        t.complete = true;
    } else if (n == "move" || n == "copy" || n == "eraser") {
        if (model_.selected.empty()) {
            model_.clickSelect(pos_);
            replot();
            return;
        }
        if (!t.origin) {
            t.origin = pos_;
            ui->inform(tr_("Click on destination point ..."));
            return;
        }
        const double dx = pos_.x - t.origin->x, dy = pos_.y - t.origin->y;
        try {
            if (n == "eraser") model_.erase(dx, dy);
            else model_.move(t.codes, dx, dy, n == "copy");
        } catch (const GeometryError&) {
            return;
        }
        deleteUtility();
        buildUi();
        replot();
        ui->inform("[success] " + tr_("Done."));
        if (n != "eraser") selectTool("select");
        return;
    }
    if (t.complete) {
        finishTool();
        ui->inform("[success] " + tr_("Done."));
        const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
        if (QApplication::keyboardModifiers() == m || n == "pad") selectTool(n);
        else selectTool("select");
    }
}

void GrbEditor::onMove(const QPointF& world, bool leftDragging) {
    if (!active_) return;
    MainGUI* ui = app_->ui();
    const Point2D p{world.x(), world.y()};
    mouse_ = p;
    const double dx = p.x - pos_.x, dy = p.y - pos_.y;
    ui->position_label->setText(QString("&nbsp;<b>X</b>: %1&nbsp;&nbsp;   <b>Y</b>: %2&nbsp;").arg(p.x, 0, 'f', 4).arg(p.y, 0, 'f', 4));
    ui->rel_position_label->setText(QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;").arg(dx, 0, 'f', 4).arg(dy, 0, 'f', 4));
    updateUtility(p);
    if (leftDragging && tool_ && tool_->name == "select") {
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

void GrbEditor::updateUtility(const Point2D& data) {
    if (!tool_ || tool_->dontExecute) return;
    Tool& t = *tool_;
    const int steps = defInt("geometry_circle_steps", 64);
    const int gsteps = defInt("gerber_circle_steps", 64);
    MainGUI* ui = app_->ui();
    auto grid = [](QLineEdit* e) {
        bool ok = false;
        const double v = e->text().replace(',', '.').toDouble(&ok);
        return ok && v != 0 ? v : 1.0;
    };
    const double gx = grid(ui->grid_gap_x_entry), gy = grid(ui->grid_gap_y_entry);
    const bool gridOn = ui->grid_snap_btn->isChecked();
    ShapeList geo;
    const QString n = t.name;
    if (n == "pad" || (n == "array" && ui_->array_type_radio->value().toInt() == 0)) {
        const auto* a = model_.aperture(t.target);
        if (!a) return;
        t.last = data;
        if (n == "pad") {
            if (const auto e = grbdraw::pad(a->ap, data, steps)) geo = {*e->solid};
        } else {
            const char axis = ui_->pad_axis_radio->value().toString().at(0).toLatin1();
            for (const Point2D& p : excdraw::linearArray(data, ui_->pad_array_size_entry->value(), axis,
                                                         ui_->pad_pitch_entry->value(), ui_->linear_angle_spinner->value()))
                if (const auto e = grbdraw::pad(a->ap, p, steps)) geo.push_back(*e->solid);
        }
    } else if (n == "track") {
        if (t.points.empty()) {
            geo = {Shape::point(data).buffer(t.bufVal, gsteps)};
        } else {
            const Point2D old = t.points.back();
            t.tempPoints = {old};
            if (gridOn && bendMode_ != 5)
                if (const auto b = grbdraw::bend(bendMode_, old, data, gx, gy, false, std::nullopt)) t.tempPoints.push_back(*b);
            t.tempPoints.push_back(data);
            geo = {t.tempPoints.size() == 1 ? Shape::point(t.tempPoints[0]).buffer(t.bufVal, gsteps)
                                            : Shape::lineString(t.tempPoints).buffer(t.bufVal, gsteps)};
        }
    } else if (n == "region") {
        if (t.points.empty()) {
            geo = {Shape::point(data).buffer(t.bufVal, gsteps / 4)};
        } else {
            const bool first = t.points.size() == 1;
            t.tempPoints = t.points;
            const Point2D old = t.points.back();
            const double mx = std::abs(std::nearbyint((data.x - old.x) / gx));
            const double my = std::abs(std::nearbyint((data.y - old.y) / gy));
            if (gridOn && mx != 0 && my != 0) {
                if (bendMode_ != 5) {
                    t.interPoint = grbdraw::bend(bendMode_, old, data, gx, gy, !first, t.interPoint);
                    if (t.interPoint) t.tempPoints.push_back(*t.interPoint);
                    else if (first) t.interPoint = data;
                } else {
                    t.interPoint.reset();
                }
            } else {
                t.interPoint = data;
            }
            t.tempPoints.push_back(data);
            try {
                if (first) geo = {Shape::lineString(t.tempPoints).buffer(t.bufVal, gsteps / 4)};
                else {
                    auto ring = t.tempPoints;
                    ring.push_back(ring.front());
                    geo = {Shape::linearRing(ring).buffer(t.bufVal, gsteps / 4)};
                }
            } catch (const GeometryError&) {
            }
        }
    } else if (n == "disc" && t.points.size() == 1) {
        geo = {Shape::point(t.points[0]).buffer(distance(t.points[0], data) + t.bufVal / 2, steps / 4)};
    } else if (n == "semidisc" && t.points.size() == 2) {
        if (const auto e = grbdraw::semiDisc(t.semiMode, t.points[0], t.points[1], data, t.cw, t.bufVal, steps)) geo = {*e->solid};
    } else if ((n == "move" || n == "copy" || n == "eraser") && t.origin) {
        for (const auto& s : model_.selected)
            if (s->solid) geo.push_back(s->solid->translate(data.x - t.origin->x, data.y - t.origin->y));
    }
    if (geo.empty()) return;
    PlotLayerData d;
    PlotGroup g;
    QRectF ignored;
    for (const Shape& s : geo) FlatCAMObj::addShapeTo(s, g, ignored, false);
    g.lineColor = colorFromFlatCam(defStr("global_draw_color") + "80");
    d.groups.push_back(g);
    ui->plotCanvas()->setLayer(kToolLayer, std::move(d));
}

void GrbEditor::deleteUtility() { app_->ui()->plotCanvas()->removeLayer(kToolLayer); }

void GrbEditor::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!active_ || !tool_) return;
    MainGUI* ui = app_->ui();
    if (button == Qt::RightButton && !dragged) {
        Tool& t = *tool_;
        if (inAction_ && (t.name == "track" || t.name == "region")) {
            // make(): la pista agrega el último tramo; la región, el polígono de sus puntos.
            ApertureGeometry e;
            if (t.name == "track") {
                if (t.tempPoints.size() == 1) {
                    const Shape pt = Shape::point(t.tempPoints[0]);
                    e.follow = pt;
                    e.solid = pt.buffer(t.bufVal, defInt("gerber_circle_steps", 64));
                } else if (t.tempPoints.size() > 1) {
                    const Shape l = Shape::lineString(t.tempPoints);
                    e.follow = l;
                    e.solid = l.buffer(t.bufVal, defInt("gerber_circle_steps", 64)).buffer(0);
                }
                if (e.solid) t.geometry = {e};
            } else if (t.points.size() > 2) {
                model_.ensureZero();
                lastAperture_ = "0";
                const Shape poly = Shape::polygon(t.points);
                e.solid = poly.buffer(t.bufVal, defInt("gerber_circle_steps", 64) / 4, JoinStyle::Mitre);
                e.follow = poly.exterior();
                t.geometry = {e};
            }
            inAction_ = false;
            t.complete = true;
            finishTool();
            ui->inform("[success] " + tr_("Done."));
            selectTool(t.name);
            return;
        }
        if (!t.complete && t.name != "select") {
            t.complete = true;
            inAction_ = false;
            deleteUtility();
            ui->inform("[success] " + tr_("Done."));
            selectTool("select");
        } else {
            app_->popupCanvasMenu();
        }
        return;
    }
    if (button != Qt::LeftButton) return;
    const Qt::KeyboardModifier m = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
    if (selectionType_) {
        ui->plotCanvas()->removeLayer(kSelRectLayer);
        model_.areaSelect(pos_, {world.x(), world.y()}, *selectionType_, QApplication::keyboardModifiers() == m);
        selectionType_.reset();
        selectRowsOfSelection();
        replot();
    } else if (tool_->name == "select") {
        model_.clickSelect(pos_);
        selectRowsOfSelection();
        replot();
    }
}

void GrbEditor::selectRowsOfSelection() {
    // after_selection: solo la apertura del primer elegido.
    QSignalBlocker block(ui_->apertures_table);
    ui_->apertures_table->clearSelection();
    if (model_.selected.empty()) return;
    if (const auto c = model_.codeOf(model_.selected.front()))
        for (int r = 0; r < ui_->apertures_table->rowCount(); ++r)
            if (ui_->apertures_table->item(r, 1)->text().toStdString() == *c) {
                ui_->apertures_table->selectRow(r);
                lastAperture_ = *c;
            }
}

void GrbEditor::transform(const QString& op) {
    // TransformEditorTool (Gerber): sobre el sólido, el seguimiento y el borrado de los elegidos, alrededor del centro
    // de su caja (o del origen, según la referencia).
    if (model_.selected.empty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
        return;
    }
    Point2D c{0, 0};
    if (transformUi_->ref_combo->currentIndex() != 0) {
        BoundingBox b{1e300, 1e300, -1e300, -1e300};
        for (const auto& s : model_.selected)
            if (s->solid)
                if (const auto sb = s->solid->bounds()) {
                    b.xmin = std::min(b.xmin, sb->xmin);
                    b.ymin = std::min(b.ymin, sb->ymin);
                    b.xmax = std::max(b.xmax, sb->xmax);
                    b.ymax = std::max(b.ymax, sb->ymax);
                }
        c = {(b.xmin + b.xmax) / 2, (b.ymin + b.ymax) / 2};
    }
    auto apply = [&](auto fn) {
        for (const auto& s : model_.selected) {
            if (s->solid) s->solid = fn(*s->solid);
            if (s->follow) s->follow = fn(*s->follow);
            if (s->clear) s->clear = fn(*s->clear);
        }
    };
    GrbTransformToolUIBase* u = transformUi_;
    if (op == "rotate") apply([&](const Shape& g) { return g.rotate(-u->rotate_entry->value(), c); });
    else if (op == "flipx") apply([&](const Shape& g) { return g.scale(-1, 1, c); });
    else if (op == "flipy") apply([&](const Shape& g) { return g.scale(1, -1, c); });
    else if (op == "skewx") apply([&](const Shape& g) { return g.skew(u->skewx_entry->value(), u->skew_link_cb->isChecked() ? u->skewx_entry->value() : 0, c); });
    else if (op == "skewy") apply([&](const Shape& g) { return g.skew(0, u->skewy_entry->value(), c); });
    else if (op == "scalex") apply([&](const Shape& g) { return g.scale(u->scalex_entry->value(), u->scale_link_cb->isChecked() ? u->scalex_entry->value() : 1, c); });
    else if (op == "scaley") apply([&](const Shape& g) { return g.scale(1, u->scaley_entry->value(), c); });
    else if (op == "offx") apply([&](const Shape& g) { return g.translate(u->offx_entry->value(), 0); });
    else if (op == "offy") apply([&](const Shape& g) { return g.translate(0, u->offy_entry->value()); });
    else if (op == "buffer") apply([&](const Shape& g) { return g.buffer(u->buffer_entry->value(), 16, u->buffer_rounded_cb->isChecked() ? JoinStyle::Round : JoinStyle::Mitre); });
    else if (op == "bufferfactor") {
        const double f = 1 + u->buffer_factor_entry->value() / 100.0;
        apply([&](const Shape& g) { return g.scale(f, f); });
    }
    replot();
    app_->ui()->inform("[success] " + tr_("Done."));
}

void GrbEditor::replot() {
    PlotLayerData d;
    PlotGroup normal, sel;
    QRectF ignored;
    for (const auto& a : model_.apertures)
        for (const auto& s : a.geometry) {
            if (!s->solid || s->solid->isEmpty()) continue;
            const bool isSel = std::find(model_.selected.begin(), model_.selected.end(), s) != model_.selected.end();
            FlatCAMObj::addShapeTo(*s->solid, isSel ? sel : normal, ignored, true);
        }
    normal.lineColor = colorFromFlatCam(defStr("global_draw_color") + "FF");
    normal.fillColor = colorFromFlatCam(defStr("global_draw_color") + "40");
    sel.lineColor = colorFromFlatCam(defStr("global_sel_draw_color") + "FF");
    sel.fillColor = colorFromFlatCam(defStr("global_sel_draw_color") + "40");
    d.groups = {normal, sel};
    app_->ui()->plotCanvas()->setLayer(kShapesLayer, std::move(d));
}

bool GrbEditor::handleKey(int key, Qt::KeyboardModifiers mods) {
    MainGUI* ui = app_->ui();
    if (mods == Qt::ControlModifier) {
        if (key == Qt::Key_S) app_->editorToObject();
        else if (key == Qt::Key_M) app_->distanceTool->run();
        else return false;
        return true;
    }
    if (mods == Qt::ShiftModifier) {
        if (key == Qt::Key_E) selectTool("eraser");
        else if (key == Qt::Key_A) selectTool("markarea");
        else if (key == Qt::Key_N) selectTool("poligonize");
        else return false;
        return true;
    }
    if (mods == Qt::AltModifier) {
        if (key == Qt::Key_R) selectTool("transform");
        else return false;
        return true;
    }
    if (mods != Qt::NoModifier && mods != Qt::KeypadModifier) return false;
    if (tool_ && (tool_->name == "track" || tool_->name == "region")) {
        if (key == Qt::Key_T || key == Qt::Key_R) {
            static const int next[] = {0, 2, 3, 4, 5, 1}, prev[] = {0, 5, 1, 2, 3, 4};
            bendMode_ = key == Qt::Key_T ? next[bendMode_] : prev[bendMode_];
            static const char* msgs[] = {"", "Track Mode 1: 45 degrees ...", "Track Mode 2: Reverse 45 degrees ...",
                                         "Track Mode 3: 90 degrees ...", "Track Mode 4: Reverse 90 degrees ...",
                                         "Track Mode 5: Free angle ..."};
            ui->inform(tr_(msgs[bendMode_]));
            updateUtility(mouse_);
            return true;
        }
        if (key == Qt::Key_Backspace && !tool_->points.empty()) {
            tool_->points.pop_back();
            updateUtility(mouse_);
            ui->inform(tr_("Backtracked one point ..."));
            return true;
        }
        if (key == Qt::Key_Enter || key == Qt::Key_Return) {
            onRelease({mouse_.x, mouse_.y}, Qt::RightButton, false);
            return true;
        }
    }
    if (tool_ && tool_->name == "semidisc") {
        if (key == Qt::Key_D) {
            tool_->cw = !tool_->cw;
            ui->inform(QString("%1: %2").arg(tr_("Direction"), tool_->cw ? "CW" : "CCW"));
            return true;
        }
        if (key == Qt::Key_M) {
            tool_->points.clear();
            tool_->semiMode = tool_->semiMode == 0 ? 2 : tool_->semiMode == 2 ? 1 : 0;
            return true;
        }
    }
    switch (key) {
        case Qt::Key_Escape:
            deleteUtility();
            model_.selected.clear();
            inAction_ = false;
            replot();
            selectTool("select");
            return true;
        case Qt::Key_Delete: deleteApertures(); return true;
        case Qt::Key_QuoteLeft: ui->onToggleNotebook(); return true;
        case Qt::Key_1: ui->notebook->setCurrentWidget(ui->project_tab); return true;
        case Qt::Key_2: ui->notebook->setCurrentWidget(ui->properties_tab); return true;
        case Qt::Key_3: ui->notebook->setCurrentWidget(ui->tool_tab); return true;
        case Qt::Key_Minus: ui->plotCanvas()->zoom(1.0 / defNum("global_zoom_ratio", 1.5), {mouse_.x, mouse_.y}); return true;
        case Qt::Key_Equal: ui->plotCanvas()->zoom(defNum("global_zoom_ratio", 1.5), {mouse_.x, mouse_.y}); return true;
        case Qt::Key_G: ui->grid_snap_btn->trigger(); return true;
        case Qt::Key_V: app_->onZoomFit(); return true;
        case Qt::Key_F3: app_->onShortcutList(); return true;
        default: break;
    }
    if (tool_ && !ui->grb_select_btn->isChecked()) {
        if (key == Qt::Key_J) app_->onJumpTo();
        return true;
    }
    switch (key) {
        case Qt::Key_A: selectTool("array"); return true;
        case Qt::Key_B: selectTool("buffer"); return true;
        case Qt::Key_C:
            selectTool("copy");
            if (tool_) tool_->origin = mouse_;
            return true;
        case Qt::Key_D: selectTool("disc"); return true;
        case Qt::Key_E: selectTool("semidisc"); return true;
        case Qt::Key_J: app_->onJumpTo(); return true;
        case Qt::Key_M:
            selectTool("move");
            if (tool_) tool_->origin = mouse_;
            return true;
        case Qt::Key_N: selectTool("region"); return true;
        case Qt::Key_P: selectTool("pad"); return true;
        case Qt::Key_S: selectTool("scale"); return true;
        case Qt::Key_T: selectTool("track"); return true;
        case Qt::Key_Space: transform("rotate"); return true;
        default: return true;
    }
}

}  // namespace flatcam::gui
