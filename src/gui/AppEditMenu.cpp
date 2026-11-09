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

#include "gui/Preferences.h"
#include "gui/App.h"
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

void App::onWorkspaceToggle() {
    workspace_ = !workspace_;
    if (workspace_) {
        // draw_workspace(global_workspaceT = "A4"), vertical (global_workspace_orientation = "p"): 210 x 297 mm.
        const double w = 210, h = 297;
        PlotLayerData d;
        d.lineColor = QColor::fromRgbF(0.70f, 0.3f, 0.3f, 0.7f);
        d.lines = {0, 0, float(w), 0, float(w), 0, float(w), float(h), float(w), float(h), 0, float(h), 0, float(h), 0, 0};
        ui_->plotCanvas()->setLayer(kWorkspaceLayerId, std::move(d));
        ui_->wplace_label->setText("A4");
        ui_->wplace_label->setToolTip("A4");
        ui_->wplace_label->setStyleSheet("QLabel { color: black; background-color: olivedrab; }");
        ui_->inform(tr_("Workspace enabled."));
    } else {
        ui_->plotCanvas()->removeLayer(kWorkspaceLayerId);
        ui_->wplace_label->setStyleSheet("");
        ui_->inform(tr_("Workspace disabled."));
    }
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
    // Los submenús de los editores (Geo Editor, Gerber Editor, Exc Editor) solo se ven dentro de un editor, que el port
    // no tiene: no se agregan.
    popMenu_->addSeparator();
    QAction* copy = popMenu_->addAction(QIcon(":/images/copy32.png"), tr_("Copy"));
    QAction* del = popMenu_->addAction(QIcon(":/images/delete32.png"), tr_("Delete"));
    QAction* edit = popMenu_->addAction(QIcon(":/images/edit32.png"), tr_("Edit"));
    QAction* save = popMenu_->addAction(QIcon(":/images/power16.png"), tr_("Exit Editor"));
    save->setVisible(false);
    popMenu_->addSeparator();
    QAction* move = popMenu_->addAction(QIcon(":/images/move32.png"), tr_("Move"));
    QAction* props = popMenu_->addAction(QIcon(":/images/properties32.png"), tr_("Properties"));
    connect(disable, &QAction::triggered, this, [this] { togglePlots(); });
    connect(panel, &QAction::triggered, ui_.get(), &MainGUI::onToggleNotebook);
    auto notPorted = [this] { ui_->inform("[WARNING_NOTCL] Los objetos vacíos para los editores no forman parte del port."); };
    for (QAction* a : {newGeo, newGrb, newExc}) connect(a, &QAction::triggered, this, notPorted);
    connect(newPrj, &QAction::triggered, this, [this] { onFileNew(); });
    connect(zoomfit, &QAction::triggered, this, [this] { onZoomFit(); });
    connect(clearplot, &QAction::triggered, this, [this] { clearPlots(); });
    connect(replot, &QAction::triggered, this, [this] { plotAll(); });
    connect(copy, &QAction::triggered, this, [this] { copySelected(); });
    connect(del, &QAction::triggered, this, [this] { deleteSelected(); });
    connect(edit, &QAction::triggered, this,
            [this] { ui_->inform("[WARNING_NOTCL] Los editores no forman parte del port."); });
    connect(move, &QAction::triggered, this, [this] { moveTool->toggle(); });  // obj_move
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
    populateGridMenu();
    popMenu_->popup(QCursor::pos());
}

}  // namespace flatcam::gui
