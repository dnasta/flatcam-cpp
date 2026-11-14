// Entrar y salir de los editores: object2editor y editor2object de app_Main.py.
#include <QMessageBox>
#include <QScrollArea>
#include <QSplitter>
#include <QTabBar>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/editors/ExcEditor.h"
#include "gui/editors/GeoEditor.h"
#include "gui/editors/GrbEditor.h"
#include "gui/editors/GCodeEditor.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

bool App::editorActive() const { return callSource_ != "app"; }

void App::objectToEditor() {
    MainGUI* ui = ui_.get();
    // Como el original, el menú Objetos queda deshabilitado aunque no se pueda editar el objeto elegido.
    ui->menuobjects->setDisabled(true);
    FlatCAMObj* obj = activeObject();
    if (!obj || obj->kind() == ObjectKind::Script || obj->kind() == ObjectKind::Document) {
        ui->inform("[WARNING_NOTCL] " + tr_("Select a Geometry, Gerber, Excellon or CNCJob Object to edit."));
        return;
    }
    if (auto* geo = dynamic_cast<GeometryObject*>(obj)) {
        // multigeo: la herramienta de la fila elegida en la tabla (la primera con ese diámetro; si no, la 1).
        std::vector<int> rows;
        QTableWidget* table = nullptr;
        if (auto* gu = dynamic_cast<GeometryObjectUI*>(obj->builtUi())) table = gu->geo_tools_table;
        if (table)
            for (const QTableWidgetItem* it : table->selectedItems())
                if (std::find(rows.begin(), rows.end(), it->row()) == rows.end()) rows.push_back(it->row());
        if (rows.size() > 1)
            ui->inform("[WARNING_NOTCL] " + tr_("Simultaneous editing of tools geometry in a MultiGeo Geometry is not "
                                                "possible.\nEdit only one geometry at a time."));
        int tool = 1;
        if (!rows.empty()) {
            const double dia = table->item(rows.front(), 1) ? table->item(rows.front(), 1)->text().toDouble() : 0.0;
            for (const auto& [id, t] : geo->tools())
                if (t.tooldia == dia) {
                    tool = id;
                    break;
                }
        } else if (geo->tools().size() == 1) {
            // Desvío: en el port todas las Geometrías son multiherramienta; con una sola herramienta se edita esa
            // (en el original una Geometría de una sola geometría se edita sin elegir nada).
            tool = geo->tools().begin()->first;
        } else {
            ui->inform("[WARNING_NOTCL] " + tr_("No Tool Selected") + ".");
            return;
        }
        geoEditor->edit(geo, tool);
        callSource_ = "geo_editor";
    } else if (auto* exc = dynamic_cast<ExcellonObject*>(obj)) {
        if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
        excEditor->edit(exc);
        callSource_ = "exc_editor";
    } else if (auto* grb = dynamic_cast<GerberObject*>(obj)) {
        if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
        grbEditor->edit(grb);
        callSource_ = "grb_editor";
    } else if (auto* cnc = dynamic_cast<CNCJobObject*>(obj)) {
        if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
        callSource_ = "gcode_editor";
        gcodeEditor->edit(cnc);
    } else {
        ui->inform("[WARNING_NOTCL] " + tr_("Select a Geometry, Gerber, Excellon or CNCJob Object to edit."));
        return;
    }
    ui->project_frame->setDisabled(true);
    QTabBar* bar = ui->notebook->tabBar();
    for (int i = 0; i < ui->notebook->count(); ++i) {
        if (ui->notebook->tabText(i) == tr_("Properties")) {
            oldTabTextColor_ = bar->tabTextColor(i);
            bar->setTabTextColor(i, QColor("red"));
            bar->setTabText(i, tr_("Editor"));
        }
        if (ui->notebook->tabText(i) == tr_("Project")) bar->setTabEnabled(i, false);
    }
    deleteSelectionShape();
    oldStateOfToolsToolbar_ = ui->toolbartools->isVisible();
    if (oldStateOfToolsToolbar_) ui->toolbartools->hide();
    ui->plot_tab_area->setTabText(0, tr_("EDITOR Area"));
    ui->plot_tab_area->protectTab(0);
    ui->inform("[WARNING_NOTCL] " + tr_("Editor is activated ..."));
    shouldWeSave_ = true;
}

bool App::editorToObject(std::optional<bool> save, bool cleanup) {
    MainGUI* ui = ui_.get();
    ui->menuobjects->setDisabled(false);
    if (callSource_ == "app") return false;
    FlatCAMObj* obj = activeObject();
    auto restoreTools = [this, ui] {
        if (oldStateOfToolsToolbar_) ui->toolbartools->show();
        ui->tool_scroll_area->takeWidget();
        ui->tool_scroll_area->setWidget(new QWidget());
        ui->notebook->setTabText(2, tr_("Tool"));
    };
    if (cleanup) {
        if (oldStateOfToolsToolbar_) ui->toolbartools->show();
        if (callSource_ == "geo_editor") geoEditor->deactivate();
        else if (callSource_ == "grb_editor") grbEditor->deactivate();
        else if (callSource_ == "exc_editor") excEditor->deactivate();
    } else {
        if (!save) {
            QMessageBox box(ui);
            box.setText(tr_("Do you want to save the edited object?"));
            box.setWindowTitle(tr_("Exit Editor"));
            box.setWindowIcon(QIcon(":/images/save_as.png"));
            box.setIcon(QMessageBox::Question);
            QPushButton* yes = box.addButton(tr_("Yes"), QMessageBox::YesRole);
            QPushButton* no = box.addButton(tr_("No"), QMessageBox::NoRole);
            box.addButton(tr_("Cancel"), QMessageBox::RejectRole);
            box.setDefaultButton(yes);
            box.exec();
            if (box.clickedButton() == yes) save = true;
            else if (box.clickedButton() == no) save = false;
            else return false;
        }
        if (*save) {
            restoreTools();
            QString type;
            if (auto* geo = dynamic_cast<GeometryObject*>(obj)) {
                type = "Geometry";
                geoEditor->save(geo);
                ui->tool_scroll_area->takeWidget();
                if (geo->solidGeometry().empty()) ui->inform("[WARNING] " + tr_("Object empty after edit."));
                objectChanged(geo);
            } else if (auto* grb = dynamic_cast<GerberObject*>(obj)) {
                type = "Gerber";
                grbEditor->save(grb);
                ui->properties_scroll_area->takeWidget();
            } else if (auto* exc = dynamic_cast<ExcellonObject*>(obj)) {
                type = "Excellon";
                excEditor->save(exc);
                ui->tool_scroll_area->takeWidget();
            } else if (auto* cnc = dynamic_cast<CNCJobObject*>(obj)) {
                type = "CNCJob";
                gcodeEditor->save(cnc);
                ui->tool_scroll_area->takeWidget();
                showProperties(cnc);  // build_ui
                gcodeEditor->closeTab();
            }
            ui->inform("[success] " + tr_("Editor exited. Editor content saved."));
            ui->inform(QString("[selected] %1 %2").arg(type, tr_("is updated, returning to App...")));
        } else {
            restoreTools();
            ui->inform("[WARNING_NOTCL] " + tr_("Editor exited. Editor content was not saved."));
            if (callSource_ == "geo_editor") geoEditor->deactivate();
            else if (callSource_ == "grb_editor") grbEditor->deactivate();
            else if (callSource_ == "exc_editor") excEditor->deactivate();
            else if (callSource_ == "gcode_editor") {
                gcodeEditor->deactivate();
                gcodeEditor->closeTab();
            }
            if (obj) {
                ui->plotCanvas()->setLayerVisible(obj->id(), obj->plotEnabled());
                showProperties(obj);
            }
        }
    }
    if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
    QTabBar* bar = ui->notebook->tabBar();
    for (int i = 0; i < ui->notebook->count(); ++i) {
        if (ui->notebook->tabText(i) == tr_("Editor")) {
            bar->setTabTextColor(i, oldTabTextColor_);
            bar->setTabText(i, tr_("Properties"));
        }
        if (ui->notebook->tabText(i) == tr_("Project")) bar->setTabEnabled(i, true);
    }
    callSource_ = "app";
    ui->plot_tab_area->setTabText(0, tr_("Plot Area"));
    ui->plot_tab_area->protectTab(0);
    ui->project_frame->setDisabled(false);
    // Un Gerber o un Excellon que queda vacío se borra (update_fcgerber / update_fcexcellon).
    if (obj && pendingDelete_) {
        pendingDelete_ = false;
        deleteByName(obj->name());
    }
    return true;
}

}  // namespace flatcam::gui
