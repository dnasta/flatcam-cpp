// Menús Archivo -> Nuevo proyecto, Editar y Ver, y el menú contextual del canvas (popMenu), de app_Main.py y
// appGUI/MainGUI.py: on_file_new_click, on_file_new, enable/disable_(all|other)_plots, toggle_plots, plot_all,
// clear_plots, on_workspace_toggle, on_selectall, on_move2origin, on_set_origin, on_jump_to, on_locate y
// populate_cmenu_grids.
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QSplitter>
#include <algorithm>
#include <limits>

#include <map>
#include "gui/widgets/FCPrefWidgets.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/RadioSet.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/Preferences.h"
#include "gui/App.h"
#include "gui/editors/GeoEditor.h"
#include "gui/ExclusionAreasManager.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/tools/MoveTool.h"
#include "gui/tools/PropertiesTool.h"
#include "gui/widgets/DialogBoxRadio.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/NumericalEval.h"

namespace flatcam::gui {

namespace {

constexpr int kWorkspaceLayerId = -5;

/// `str(tuple)` de Python para lo que tenga el portapapeles: los enteros sin decimales, los demás como `repr`.
QString pyTuple(const QString& text, const std::vector<double>& v) {
    static const QRegularExpression intRe(R"(^\s*[+-]?\d+\s*$)");
    QString inner = text;
    inner.remove('(').remove(')');
    const QStringList tokens = inner.split(',');
    QStringList out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i < static_cast<std::size_t>(tokens.size()) && intRe.match(tokens[static_cast<int>(i)]).hasMatch()) {
            out << QString::number(static_cast<long long>(v[i]));
        } else {
            QString r = QString::number(v[i], 'g', QLocale::FloatingPointShortest);
            if (!r.contains('.') && !r.contains('e') && !r.contains("inf") && !r.contains("nan")) r += ".0";
            out << r;
        }
    }
    return "(" + out.join(", ") + ")";
}

}  // namespace

// --------------------------------------------------------------------------------------------- Archivo

void App::onFileNewClick() {
    if (!collection_->objects().empty() && shouldWeSave_) {
        QMessageBox box(ui_.get());
        box.setText(tr_("There are files/objects opened in FlatCAM.\nCreating a New project will delete them.\n"
                        "Do you want to Save the project?"));
        box.setWindowTitle(tr_("Save changes"));
        box.setWindowIcon(QIcon(":/images/save_as.png"));
        box.setIcon(QMessageBox::Question);
        QPushButton* yes = box.addButton(tr_("Yes"), QMessageBox::YesRole);
        QPushButton* no = box.addButton(tr_("No"), QMessageBox::NoRole);
        QPushButton* cancel = box.addButton(tr_("Cancel"), QMessageBox::RejectRole);
        box.setDefaultButton(yes);
        box.exec();
        QAbstractButton* r = box.clickedButton();
        if (r == yes) onFileSaveProjectAs();  // el original guarda y no limpia el proyecto
        else if (r == cancel || r != no) return;
        else onFileNew();
    } else {
        onFileNew();
    }
    ui_->inform("[success] " + tr_("New Project created") + "...");
}

void App::onFileNew() {
    excAreas_->clearAll();
    deleteSelectionShape();
    clearProject();
    projectOptions_ = JsonValue();
    // Se cierran todas las pestañas del área de dibujo y vuelve "Área de dibujo".
    FCDetachableTab2* area = ui_->plot_tab_area;
    for (int i = area->count() - 1; i >= 0; --i) area->closeTab(i);
    area->insertTab(0, ui_->plot_tab, tr_("Plot Area"));
    area->protectTab(0);
    ui_->notebook->setCurrentWidget(ui_->project_tab);
    ui_->setUiTitle(tr_("New Project - Not saved"));
    shouldWeSave_ = false;
}

// --------------------------------------------------------------------------------------------- Ver

std::vector<FlatCAMObj*> App::nonSelectedObjects() const {
    std::vector<FlatCAMObj*> out = collection_->objects();
    for (FlatCAMObj* s : selectedObjects()) out.erase(std::remove(out.begin(), out.end(), s), out.end());
    return out;
}

void App::enablePlots(const std::vector<FlatCAMObj*>& objs) {
    for (FlatCAMObj* o : objs) {
        if (o->plotEnabled()) continue;
        o->setPlotEnabled(true);
        ui_->plotCanvas()->setLayerVisible(o->id(), true);
        if (o == activeObject()) showProperties(o);  // set_form_item("plot")
    }
}

void App::disablePlots(const std::vector<FlatCAMObj*>& objs) {
    for (FlatCAMObj* o : objs) {
        if (!o->plotEnabled()) continue;
        o->setPlotEnabled(false);
        ui_->plotCanvas()->setLayerVisible(o->id(), false);
        if (o == activeObject()) showProperties(o);
    }
    deleteSelectionShape();
}

void App::togglePlots() {
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) return;
    ui_->inform(tr_("Working ..."));
    for (FlatCAMObj* o : sel) {
        o->setPlotEnabled(!o->plotEnabled());
        ui_->plotCanvas()->setLayerVisible(o->id(), o->plotEnabled());
        if (o == activeObject()) showProperties(o);
    }
}

void App::plotAll() {
    ui_->inform(tr_("Redrawing all objects") + "...");
    for (FlatCAMObj* o : collection_->objects()) {
        ui_->plotCanvas()->setLayer(o->id(), o->plotData());
        ui_->plotCanvas()->setLayerVisible(o->id(), o->plotEnabled());
    }
    ui_->plotCanvas()->setLayerOrder(collection_->drawOrder());
    onZoomFit();  // object_plotted con fit_view
}

void App::clearPlots() {
    for (FlatCAMObj* o : collection_->objects()) ui_->plotCanvas()->removeLayer(o->id());
}

std::optional<std::pair<double, double>> App::pageSize(const QString& name) {
    // PlotCanvas.pagesize_dict (mm).
    static const std::map<QString, std::pair<double, double>> sizes = {
        {"A0", {841, 1189}}, {"A1", {594, 841}}, {"A2", {420, 594}}, {"A3", {297, 420}}, {"A4", {210, 297}},
        {"A5", {148, 210}}, {"A6", {105, 148}}, {"A7", {74, 105}}, {"A8", {52, 74}}, {"A9", {37, 52}},
        {"A10", {26, 37}}, {"B0", {1000, 1414}}, {"B1", {707, 1000}}, {"B2", {500, 707}}, {"B3", {353, 500}},
        {"B4", {250, 353}}, {"B5", {176, 250}}, {"B6", {125, 176}}, {"B7", {88, 125}}, {"B8", {62, 88}},
        {"B9", {44, 62}}, {"B10", {31, 44}}, {"C0", {917, 1297}}, {"C1", {648, 917}}, {"C2", {458, 648}},
        {"C3", {324, 458}}, {"C4", {229, 324}}, {"C5", {162, 229}}, {"C6", {114, 162}}, {"C7", {81, 114}},
        {"C8", {57, 81}}, {"C9", {40, 57}}, {"C10", {28, 40}}, {"LETTER", {8.5 * 25.4, 11 * 25.4}},
        {"LEGAL", {8.5 * 25.4, 14 * 25.4}}, {"ELEVENSEVENTEEN", {11 * 25.4, 17 * 25.4}},
        {"JUNIOR_LEGAL", {5 * 25.4, 8 * 25.4}}, {"HALF_LETTER", {5.5 * 25.4, 8 * 25.4}},
        {"GOV_LETTER", {8 * 25.4, 10.5 * 25.4}}, {"GOV_LEGAL", {8.5 * 25.4, 13 * 25.4}},
        {"LEDGER", {17 * 25.4, 11 * 25.4}}};
    const auto it = sizes.find(name);
    if (it == sizes.end()) return std::nullopt;
    return it->second;
}

void App::applyCursorPreferences() {
    // cursor_color_3D: global_cursor_color o, sin color propio, negro (tema claro). El tamaño y el grosor se leen de
    // las opciones en cada movimiento del mouse.
    const QColor color = cursorColorEnabled_ ? colorFromFlatCam(defStr("global_cursor_color")) : QColor("black");
    ui_->plotCanvas()->setCursorStyle(color, defInt("global_cursor_size", 20), defInt("global_cursor_width", 2),
                                      cursorBig_);
}

void App::drawWorkspace() {
    // PlotCanvas.draw_workspace(global_workspaceT): la hoja en las unidades de la aplicación, apaisada con "l".
    const QString size = defStr("global_workspaceT");
    auto dims = pageSize(size);
    if (!dims) return;  // "PlotCanvas.draw_workspace() --> ..." (solo en el registro)
    if (appUnits() != "MM") dims = std::make_pair(dims->first / 25.4, dims->second / 25.4);
    if (defStr("global_workspace_orientation") == "l") dims = std::make_pair(dims->second, dims->first);
    const auto w = float(dims->first), h = float(dims->second);
    PlotLayerData d;
    d.lineColor = QColor::fromRgbF(0.70f, 0.3f, 0.3f, 0.7f);
    d.lines = {0, 0, w, 0, w, 0, w, h, w, h, 0, h, 0, h, 0, 0};
    ui_->plotCanvas()->setLayer(kWorkspaceLayerId, std::move(d));
    ui_->wplace_label->setText(size.left(3));  // workspace_size[:3]
    ui_->wplace_label->setToolTip(size);
    ui_->wplace_label->setStyleSheet("QLabel { color: black; background-color: olivedrab; }");
}

void App::deleteWorkspace() {
    ui_->plotCanvas()->removeLayer(kWorkspaceLayerId);
    ui_->wplace_label->setStyleSheet("");
}

void App::onWorkspace() {
    // on_workspace: según la casilla de la pestaña; después defaults_read_form.
    if (prefs_->tab()->general_app_set_group_workspace_cb->isChecked()) {
        drawWorkspace();
        ui_->inform(tr_("Workspace enabled."));
    } else {
        deleteWorkspace();
        ui_->inform(tr_("Workspace disabled."));
    }
    prefs_->readForm();
}

void App::onWorkspaceToggle() {
    // on_workspace_toggle: invierte la casilla sin su aviso y llama a on_workspace.
    FCCheckBox* cb = prefs_->tab()->general_app_set_group_workspace_cb;
    workspaceCbMuted_ = true;
    cb->setChecked(!cb->isChecked());
    workspaceCbMuted_ = false;
    onWorkspace();
}

void App::onWorkspaceModified() {
    // on_workspace_modified (tamaño u orientación): como el original, dibuja la hoja aunque el área esté apagada.
    deleteWorkspace();
    prefs_->readForm();
    drawWorkspace();
}

// --------------------------------------------------------------------------------------------- Editar

void App::onSelectAll() {
    deleteSelectionShape();
    for (FlatCAMObj* o : collection_->objects()) {
        setActive(o, true);
        drawSelectionShape(o);
    }
}

void App::onMove2Origin() {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    if (objs.empty()) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Failed. No object(s) selected..."));
        return;
    }
    double x = std::numeric_limits<double>::infinity(), y = x;
    for (FlatCAMObj* o : objs) {
        const QRectF b = o->bounds();
        x = std::min(x, b.left());
        y = std::min(y, b.top());
    }
    for (FlatCAMObj* o : objs)
        if (o->offset(-x, -y)) objectChanged(o);  // un CNC Job no se mueve (ver MoveTool)
    for (FlatCAMObj* o : objs) refreshSourceFile(o);
    ui_->inform("[success] " + tr_("Origin set") + "...");
}

void App::onSetOrigin() {
    ui_->inform(tr_("Click to set the origin ..."));
    if (setOriginConnection_) disconnect(setOriginConnection_);
    // on_set_zero_click: el primer clic izquierdo (ajustado a la grilla); los demás botones no hacen nada.
    setOriginConnection_ = connect(ui_->plotCanvas(), &PlotCanvas::mouseClicked, this, [this](QPointF w, Qt::MouseButton b) {
        if (b != Qt::LeftButton) return;
        disconnect(setOriginConnection_);
        setOriginConnection_ = {};
        setOriginAt(w);
    });
}

void App::setOriginAt(const QPointF& p) {
    for (FlatCAMObj* o : collection_->objects())
        if (o->offset(-p.x(), -p.y())) objectChanged(o);
    for (FlatCAMObj* o : collection_->objects()) refreshSourceFile(o);
    ui_->inform("[success] " + tr_("Origin set") + "...");
    plotAll();  // origin_replot: dibuja todo y encuadra
}

void App::onJumpTo() {
    // El texto inicial: lo que tenga el portapapeles, si es una tupla.
    QString initial;
    const QString clip = QApplication::clipboard()->text();
    if (clip.contains(',')) {
        if (const auto v = NumericExpression::evaluateTuple(clip)) initial = pyTuple(clip, *v);
    }
    DialogBoxRadio dia(tr_("Jump to ..."), tr_("Enter the coordinates in format X,Y:"), QIcon(":/images/jump_to16.png"),
                       initial, jumpRef_, ui_.get());
    if (dia.exec() != QDialog::Accepted) return;
    const QString text = dia.lineEdit->text();
    const auto v = text.contains(',') ? NumericExpression::evaluateTuple(text) : std::nullopt;
    if (!v || v->size() != 2) {
        if (!v) ui_->inform(tr_("Wrong coordinates. Enter coordinates in format: X,Y"));
        return;
    }
    QPointF location((*v)[0], (*v)[1]);
    const QString ref = dia.ref_radio->value().toString();
    if (ref == "rel") location += mouse_;
    jumpRef_ = ref;
    jumpTo(location, true);
}

void App::onLocate() {
    FlatCAMObj* obj = activeObject();
    if (!obj) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    DialogBoxChoice dia(tr_("Locate ..."), QIcon(":/images/locate16.png"), locatePt_, ui_.get());
    if (dia.exec() != QDialog::Accepted) return;
    const QString pt = dia.ref_radio->value().toString();
    locatePt_ = pt;
    const QRectF b = obj->bounds();
    QPointF location;
    if (pt == "bl") location = {b.left(), b.top()};
    else if (pt == "tl") location = {b.left(), b.bottom()};
    else if (pt == "br") location = {b.right(), b.top()};
    else if (pt == "tr") location = {b.right(), b.bottom()};
    else location = {b.left() + b.width() / 2, b.top() + b.height() / 2};
    jumpTo(location, true);
}

// --------------------------------------------------------------------------------------------- Opciones

void App::onRotateSelection(std::optional<double> preset) {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    if (objs.empty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    double num = preset.value_or(0);
    if (!preset) {
        FCInputDoubleSpinner box(ui_.get(), tr_("Transform"), tr_("Enter the Angle value:"), -360, 360, 1, 4,
                                 defNum("tools_transform_rotate"));
        box.setWindowIcon(QIcon(":/images/rotate.png"));
        if (box.exec() != QDialog::Accepted) return;
        num = box.wdg->value();
    }
    // Alrededor del centro de la caja de todos los elegidos.
    QRectF all;
    for (FlatCAMObj* o : objs) all = all.isNull() ? o->bounds() : all.united(o->bounds());
    const Point2D c{0.5 * (all.left() + all.right()), 0.5 * (all.top() + all.bottom())};
    for (FlatCAMObj* o : objs)
        if (o->rotate(-num, c)) objectChanged(o);
    ui_->inform("[success] " + tr_("Rotation done."));
}

void App::onSkewSelection(char axis, std::optional<double> preset) {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    if (objs.empty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    double num = preset.value_or(0);
    if (!preset) {
        FCInputDoubleSpinner box(ui_.get(), tr_("Transform"), tr_("Enter the Angle value:"), -360, 360, 1, 4,
                                 defNum(axis == 'X' ? "tools_transform_skew_x" : "tools_transform_skew_y"));
        box.setWindowIcon(QIcon(axis == 'X' ? ":/images/skewX.png" : ":/images/skewY.png"));
        if (box.exec() != QDialog::Accepted) return;
        num = box.wdg->value();
    }
    // Desde la esquina inferior izquierda de la caja de todos.
    double x = std::numeric_limits<double>::infinity(), y = x;
    for (FlatCAMObj* o : objs) {
        x = std::min(x, o->bounds().left());
        y = std::min(y, o->bounds().top());
    }
    for (FlatCAMObj* o : objs)
        if (axis == 'X' ? o->skew(num, 0, {x, y}) : o->skew(0, num, {x, y})) objectChanged(o);
    ui_->inform("[success] " + (axis == 'X' ? tr_("Skew on X axis done.") : tr_("Skew on Y axis done.")));
}

// --------------------------------------------------------------------------------------------- menú del canvas

void App::setupCanvasMenu() {
    // Archivo, Editar y Ver de la ventana principal.
    connect(ui_->menufilenewproject, &QAction::triggered, this, [this] { onFileNewClick(); });
    for (QAction* a : {ui_->menueditdelete, ui_->delete_btn}) connect(a, &QAction::triggered, this, [this] { deleteSelected(); });
    for (QAction* a : {ui_->menueditcopyobject, ui_->copy_btn}) connect(a, &QAction::triggered, this, [this] { copySelected(); });
    connect(ui_->menueditselectall, &QAction::triggered, this, [this] { onSelectAll(); });
    connect(ui_->menuoptions_transform_rotate, &QAction::triggered, this, [this] { onRotateSelection(); });
    connect(ui_->menuoptions_transform_skewx, &QAction::triggered, this, [this] { onSkewSelection('X'); });
    connect(ui_->menuoptions_transform_skewy, &QAction::triggered, this, [this] { onSkewSelection('Y'); });
    connect(ui_->menuoptions_view_source, &QAction::triggered, this, [this] { viewSource(activeObject()); });
    connect(ui_->menuedit_join2geo, &QAction::triggered, this, [this] { joinGeometry(); });
    connect(ui_->menuedit_join_exc2exc, &QAction::triggered, this, [this] { joinExcellon(); });
    connect(ui_->menuedit_join_grb2grb, &QAction::triggered, this, [this] { joinGerber(); });
    connect(ui_->menueditconvert_any2geo, &QAction::triggered, this, [this] { convertToGeometry(); });
    connect(ui_->menueditconvert_any2gerber, &QAction::triggered, this, [this] { convertToGerber(); });
    connect(ui_->menueditconvert_any2excellon, &QAction::triggered, this, [this] { convertToExcellon(); });
    // Las Geometrías del port son siempre multiherramienta ("MultiGeo"): convertir entre los dos tipos no aplica.
    for (QAction* a : {ui_->menuedit_convert_sg2mg, ui_->menuedit_convert_mg2sg})
        connect(a, &QAction::triggered, this, [this] {
            ui_->inform("[WARNING_NOTCL] Las Geometrías del port son siempre de varias herramientas (MultiGeo).");
        });
    for (QAction* a : {ui_->menuedit_move2origin, ui_->move2origin_btn})
        connect(a, &QAction::triggered, this, [this] { onMove2Origin(); });
    for (QAction* a : {ui_->menueditorigin, ui_->origin_btn}) connect(a, &QAction::triggered, this, [this] { onSetOrigin(); });
    for (QAction* a : {ui_->menueditjump, ui_->jmp_btn}) connect(a, &QAction::triggered, this, [this] { onJumpTo(); });
    for (QAction* a : {ui_->menueditlocate, ui_->locate_btn}) connect(a, &QAction::triggered, this, [this] { onLocate(); });
    connect(ui_->menuviewenable, &QAction::triggered, this, [this] {
        enablePlots(collection_->objects());
        ui_->inform("[success] " + tr_("All plots enabled."));
    });
    connect(ui_->menuviewdisableall, &QAction::triggered, this, [this] {
        disablePlots(collection_->objects());
        ui_->inform("[success] " + tr_("All plots disabled."));
    });
    connect(ui_->menuviewenableother, &QAction::triggered, this, [this] {
        enablePlots(nonSelectedObjects());
        ui_->inform("[success] " + tr_("All non selected plots enabled."));
    });
    connect(ui_->menuviewdisableother, &QAction::triggered, this, [this] {
        disablePlots(nonSelectedObjects());
        ui_->inform("[success] " + tr_("All non selected plots disabled."));
    });
    for (QAction* a : {ui_->menuview_replot, ui_->replot_btn}) connect(a, &QAction::triggered, this, [this] { plotAll(); });
    connect(ui_->clear_plot_btn, &QAction::triggered, this, [this] { clearPlots(); });
    connect(ui_->menuview_toggle_workspace, &QAction::triggered, this, [this] { onWorkspaceToggle(); });
    connect(ui_->wplace_label, &FCLabel::clicked, this, [this] { onWorkspaceToggle(); });
    PreferencesTab* pt = prefs_->tab();
    connect(pt->general_app_set_group_wk_cb, &QComboBox::currentIndexChanged, this, [this] { onWorkspaceModified(); });
    connect(pt->general_app_set_group_wk_orientation_radio, &RadioSet::activatedCustom, this,
            [this] { onWorkspaceModified(); });
    connect(pt->general_app_set_group_workspace_cb, &QCheckBox::checkStateChanged, this, [this] {
        if (!workspaceCbMuted_) onWorkspace();
    });
    if (defBool("global_workspace")) drawWorkspace();  // PlotCanvas.__init__
    // El cursor: el tipo (on_cursor_type), el color (on_mouse_cursor_color_enable / on_mouse_cursor_entry) y lo que se
    // guarda en las preferencias.
    connect(pt->general_app_set_group_cursor_radio, &RadioSet::activatedCustom, this, [this, pt](const QString& v) {
        pt->general_app_set_group_cursor_size_entry->setDisabled(v != "small");
        pt->general_app_set_group_cursor_size_lbl->setDisabled(v != "small");
        cursorBig_ = v != "small";  // new_cursor(big=...)
        applyCursorPreferences();
    });
    connect(pt->general_app_set_group_mouse_cursor_color_cb, &QCheckBox::checkStateChanged, this, [this, pt] {
        cursorColorEnabled_ = pt->general_app_set_group_mouse_cursor_color_cb->isChecked();
        applyCursorPreferences();
    });
    connect(pt->general_app_set_group_mouse_cursor_entry, &FCColorEntry::editingFinished, this, [this] {
        cursorColorEnabled_ = true;  // cursor_color_3D = global_cursor_color
        applyCursorPreferences();
    });
    connect(prefs_, &Preferences::saved, this, [this] { applyCursorPreferences(); });
    cursorColorEnabled_ = defBool("global_cursor_color_enabled");
    cursorBig_ = defStr("global_cursor_type") == "big";
    applyCursorPreferences();

    // popMenu (MainGUI.py): transcripto, porque es un QMenu suelto que el volcador no recorre.
    popMenu_ = new QMenu(ui_.get());
    QAction* disable = popMenu_->addAction(QIcon(":/images/disable32.png"), tr_("Toggle Visibility"));
    QAction* panel = popMenu_->addAction(QIcon(":/images/notebook16.png"), tr_("Toggle Panel"));
    popMenu_->addSeparator();
    QMenu* newMenu = popMenu_->addMenu(QIcon(":/images/file32.png"), tr_("New"));
    QAction* newGeo = newMenu->addAction(QIcon(":/images/new_file_geo16.png"), tr_("Geometry"));
    QAction* newGrb = newMenu->addAction(QIcon(":/images/new_file_grb16.png"), "Gerber");
    QAction* newExc = newMenu->addAction(QIcon(":/images/new_file_exc16.png"), tr_("Excellon"));
    newMenu->addSeparator();
    QAction* newPrj = newMenu->addAction(QIcon(":/images/file16.png"), tr_("Project"));
    popMenu_->addSeparator();
    gridMenu_ = popMenu_->addMenu(QIcon(":/images/grid32_menu.png"), tr_("Grids"));
    QMenu* viewMenu = popMenu_->addMenu(QIcon(":/images/view64.png"), tr_("View"));
    QAction* zoomfit = viewMenu->addAction(QIcon(":/images/zoom_fit32.png"), tr_("Zoom Fit"));
    QAction* clearplot = viewMenu->addAction(QIcon(":/images/clear_plot32.png"), tr_("Clear Plot"));
    QAction* replot = viewMenu->addAction(QIcon(":/images/replot32.png"), tr_("Replot"));
    popMenu_->addSeparator();
    // Los submenús de los editores (g_editor_cmenu, grb_editor_cmenu, e_editor_cmenu): solo se ven dentro del editor.
    auto act = [](QMenu* m, const char* icon, const QString& text) { return m->addAction(QIcon(icon), text); };
    QMenu* gm = popMenu_->addMenu(QIcon(":/images/draw32.png"), tr_("Geo Editor"));
    canvasMenu_.geo = gm;
    const std::vector<std::pair<QAction*, QString>> geoActs = {
        {act(gm, ":/images/path32.png", tr_("Path")), "path"},
        {act(gm, ":/images/rectangle32.png", tr_("Rectangle")), "rectangle"},
        {gm->addSeparator(), ""},
        {act(gm, ":/images/circle32.png", tr_("Circle")), "circle"},
        {act(gm, ":/images/polygon32.png", tr_("Polygon")), "polygon"},
        {act(gm, ":/images/arc32.png", tr_("Arc")), "arc"},
        {gm->addSeparator(), ""},
        {act(gm, ":/images/text32.png", tr_("Text")), "text"},
        {act(gm, ":/images/buffer16-2.png", tr_("Buffer")), "buffer"},
        {act(gm, ":/images/paint20_1.png", tr_("Paint")), "paint"},
        {act(gm, ":/images/eraser26.png", tr_("Eraser")), "eraser"},
        {gm->addSeparator(), ""},
        {act(gm, ":/images/union32.png", tr_("Union")), "#union"},
        {act(gm, ":/images/intersection32.png", tr_("Intersection")), "#intersection"},
        {act(gm, ":/images/subtract32.png", tr_("Subtraction")), "#subtract"},
        {act(gm, ":/images/cutpath32.png", tr_("Cut")), "#cut"},
        {act(gm, ":/images/transform.png", tr_("Transformations")), "transform"},
        {gm->addSeparator(), ""},
        {act(gm, ":/images/move32.png", tr_("Move")), "move"}};
    for (const auto& [a, name] : geoActs) {
        if (name.isEmpty()) continue;
        const QString n = name;
        connect(a, &QAction::triggered, this, [this, n] {
            if (n == "#union") geoEditor->unite();
            else if (n == "#intersection") geoEditor->intersection();
            else if (n == "#subtract") geoEditor->subtract();
            else if (n == "#cut") geoEditor->cutpath();
            else if (n == "transform") ui_->geo_transform_menuitem->trigger();  // draw_transform: transform_tool.run
            else geoEditor->selectTool(n);
        });
    }
    QMenu* grm = popMenu_->addMenu(QIcon(":/images/draw32.png"), tr_("Gerber Editor"));
    canvasMenu_.grb = grm;
    for (const auto& [icon, text, name] : std::vector<std::tuple<const char*, QString, QString>>{
             {":/images/aperture32.png", tr_("Pad"), "pad"},
             {":/images/padarray32.png", tr_("Pad Array"), "array"},
             {nullptr, "", ""},
             {":/images/track32.png", tr_("Track"), "track"},
             {":/images/polygon32.png", tr_("Region"), "region"},
             {":/images/poligonize32.png", tr_("Poligonize"), "poligonize"},
             {":/images/semidisc32.png", tr_("SemiDisc"), "semidisc"},
             {":/images/disc32.png", tr_("Disc"), "disc"},
             {nullptr, "", ""},
             {":/images/buffer16-2.png", tr_("Buffer"), "buffer"},
             {":/images/scale32.png", tr_("Scale"), "scale"},
             {":/images/markarea32.png", tr_("Mark Area"), "markarea"},
             {":/images/eraser26.png", tr_("Eraser"), "eraser"},
             {nullptr, "", ""},
             {":/images/transform.png", tr_("Transformations"), "transform"}}) {
        if (!icon) {
            grm->addSeparator();
            continue;
        }
        QAction* a = act(grm, icon, text);
        canvasMenu_.grbActions[name] = a;
    }
    QMenu* em = popMenu_->addMenu(QIcon(":/images/drill32.png"), tr_("Exc Editor"));
    canvasMenu_.exc = em;
    for (const auto& [icon, text, name] : std::vector<std::tuple<const char*, QString, QString>>{
             {":/images/drill32.png", tr_("Add Drill"), "drill_add"},
             {":/images/addarray32.png", tr_("Add Drill Array"), "drill_array"},
             {nullptr, "", ""},
             {":/images/slot26.png", tr_("Add Slot"), "slot_add"},
             {":/images/slot_array26.png", tr_("Add Slot Array"), "slot_array"},
             {nullptr, "", ""},
             {":/images/resize16.png", tr_("Resize Drill"), "drill_resize"}}) {
        if (!icon) {
            em->addSeparator();
            continue;
        }
        canvasMenu_.excActions[name] = act(em, icon, text);
    }
    for (QMenu* m : {gm, grm, em}) m->menuAction()->setVisible(false);
    popMenu_->addSeparator();
    QAction* copy = popMenu_->addAction(QIcon(":/images/copy32.png"), tr_("Copy"));
    QAction* del = popMenu_->addAction(QIcon(":/images/delete32.png"), tr_("Delete"));
    QAction* edit = popMenu_->addAction(QIcon(":/images/edit32.png"), tr_("Edit"));
    QAction* save = popMenu_->addAction(QIcon(":/images/power16.png"), tr_("Exit Editor"));
    save->setVisible(false);
    popMenu_->addSeparator();
    QAction* move = popMenu_->addAction(QIcon(":/images/move32.png"), tr_("Move"));
    QAction* props = popMenu_->addAction(QIcon(":/images/properties32.png"), tr_("Properties"));
    canvasMenu_.disable = disable;
    canvasMenu_.newMenu = newMenu->menuAction();
    canvasMenu_.edit = edit;
    canvasMenu_.save = save;
    canvasMenu_.properties = props;
    connect(save, &QAction::triggered, this, [this] { editorToObject(); });
    connect(disable, &QAction::triggered, this, [this] { togglePlots(); });
    connect(panel, &QAction::triggered, ui_.get(), &MainGUI::onToggleNotebook);
    connect(newGeo, &QAction::triggered, this, [this] { newGeometryObject(); });
    connect(newGrb, &QAction::triggered, this, [this] { newGerberObject(); });
    connect(newExc, &QAction::triggered, this, [this] { newExcellonObject(); });
    connect(newPrj, &QAction::triggered, this, [this] { onFileNew(); });
    connect(zoomfit, &QAction::triggered, this, [this] { onZoomFit(); });
    connect(clearplot, &QAction::triggered, this, [this] { clearPlots(); });
    connect(replot, &QAction::triggered, this, [this] { plotAll(); });
    // Dentro del editor de Geometría, Copiar, Borrar y Mover son las del editor (connect_canvas_event_handlers).
    connect(copy, &QAction::triggered, this, [this] {
        if (callSource_ == "geo_editor") geoEditor->selectTool("copy");
        else if (callSource_ == "app") copySelected();
    });
    connect(del, &QAction::triggered, this, [this] {
        if (callSource_ == "geo_editor") geoEditor->deleteSelected();
        else if (callSource_ == "app") deleteSelected();
    });
    connect(edit, &QAction::triggered, this, [this] { objectToEditor(); });
    connect(move, &QAction::triggered, this, [this] {
        if (callSource_ == "geo_editor") geoEditor->selectTool("move");
        else if (callSource_ == "app") moveTool->toggle();  // obj_move
    });
    connect(props, &QAction::triggered, this, [this] { propertiesTool->run(false); });
}

void App::populateGridMenu() {
    // populate_cmenu_grids: activar o no la grilla, los valores de la lista (ordenados), Agregar y Borrar.
    gridMenu_->clear();
    QAction* toggle = gridMenu_->addAction(QIcon(":/images/grid32_menu.png"), tr_("Grid On/Off"));
    toggle->setCheckable(true);
    toggle->setChecked(ui_->grid_snap_btn->isChecked());
    gridMenu_->addSeparator();
    QStringList sorted = gridContextMenu_;
    std::sort(sorted.begin(), sorted.end(), [](const QString& a, const QString& b) { return a.toDouble() < b.toDouble(); });
    for (const QString& g : sorted) {
        QAction* a = gridMenu_->addAction(QIcon(":/images/grid32_menu.png"), g);
        connect(a, &QAction::triggered, this, [this, g] {  // set_grid
            ui_->grid_gap_x_entry->setText(g);
            ui_->grid_gap_y_entry->setText(g);
        });
    }
    gridMenu_->addSeparator();
    QAction* add = gridMenu_->addAction(QIcon(":/images/plus32.png"), tr_("Add"));
    QAction* del = gridMenu_->addAction(QIcon(":/images/delete32.png"), tr_("Delete"));
    connect(toggle, &QAction::triggered, this, [this] { ui_->grid_snap_btn->trigger(); });
    connect(add, &QAction::triggered, this, [this] {  // on_grid_add
        FCInputDoubleSpinner box(ui_.get(), tr_("New Grid ..."), tr_("Enter a Grid Value:"), 0.0, 99.9999, 1, 4, 0.0);
        box.setWindowIcon(QIcon(":/images/plus32.png"));
        const bool ok = box.exec() == QDialog::Accepted;
        const double val = box.wdg->value();
        if (!ok) {
            ui_->inform("[WARNING_NOTCL] " + tr_("Adding New Grid cancelled") + "...");
        } else if (val == 0) {
            ui_->inform("[WARNING_NOTCL] " + tr_("Please enter a grid value with non-zero value, in Float format."));
        } else {
            const QString text = QString::number(val, 'g', QLocale::FloatingPointShortest);
            const bool exists = std::any_of(gridContextMenu_.begin(), gridContextMenu_.end(),
                                            [val](const QString& g) { return g.toDouble() == val; });
            if (!exists) {
                gridContextMenu_ << (text.contains('.') ? text : text + ".0");
                ui_->inform("[success] " + tr_("New Grid added") + "...");
            } else {
                ui_->inform("[WARNING_NOTCL] " + tr_("Grid already exists") + "...");
            }
        }
    });
    connect(del, &QAction::triggered, this, [this] {  // on_grid_delete
        FCInputDoubleSpinner box(ui_.get(), "Delete Grid ...", "Enter a Grid Value:", 0.0, 99.9999, 1, 4, 0.0);
        box.setWindowIcon(QIcon(":/images/delete32.png"));
        const bool ok = box.exec() == QDialog::Accepted;
        const double val = box.wdg->value();
        if (!ok) {
            ui_->inform("[WARNING_NOTCL] " + tr_("Delete Grid value cancelled") + "...");
            return;
        }
        if (val == 0) {
            ui_->inform("[WARNING_NOTCL] " + tr_("Please enter a grid value with non-zero value, in Float format."));
            return;
        }
        const auto it = std::find_if(gridContextMenu_.begin(), gridContextMenu_.end(),
                                     [val](const QString& g) { return g.toDouble() == val; });
        if (it == gridContextMenu_.end()) {
            ui_->inform("[ERROR_NOTCL]" + tr_("Grid Value does not exist") + "...");
        } else {
            gridContextMenu_.erase(it);
            ui_->inform("[success] " + tr_("Grid Value deleted") + "...");
        }
    });
}

void App::popupCanvasMenu() {
    // activate / deactivate de cada editor: qué se ve del menú.
    const bool inEditor = callSource_ != "app";
    canvasMenu_.edit->setVisible(!inEditor);
    canvasMenu_.save->setVisible(inEditor);
    canvasMenu_.disable->setVisible(!inEditor);
    canvasMenu_.newMenu->setVisible(!inEditor);
    canvasMenu_.properties->setVisible(!inEditor);
    canvasMenu_.geo->menuAction()->setVisible(callSource_ == "geo_editor");
    canvasMenu_.grb->menuAction()->setVisible(callSource_ == "grb_editor");
    canvasMenu_.exc->menuAction()->setVisible(callSource_ == "exc_editor");
    populateGridMenu();
    popMenu_->popup(QCursor::pos());
}

}  // namespace flatcam::gui
