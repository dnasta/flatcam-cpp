// Menú contextual de la pestaña Proyecto y sus acciones (port de `MainGUI.menuproject`, `ObjectCollection.on_menu_request`
// y de `on_enable_sel_plots`, `on_disable_sel_plots`, `on_set_color_action_triggered`, `generate_cnc_job`,
// `on_view_source`, `on_copy_command`, `on_delete` y `on_project_context_save` de `app_Main.py`).
#include <QApplication>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <cmath>

#include "gui/Preferences.h"
#include "gui/App.h"
#include "gui/tools/PropertiesTool.h"
#include "gui/ProjectIO.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"

namespace flatcam::gui {

namespace {

/// color_variant de appCommon/Common.py: cada canal multiplicado por el factor (redondeo de Python, al par).
QColor colorVariant(const QColor& c, double factor) {
    factor = std::clamp(factor, 0.0, 1.0);
    auto ch = [factor](int v) { return static_cast<int>(std::nearbyint(v * factor)); };
    return QColor(ch(c.red()), ch(c.green()), ch(c.blue()));
}

}  // namespace

void App::setupProjectMenu() {
    // MainGUI.menuproject, con los mismos textos e íconos.
    menuProject_ = new QMenu(ui_.get());
    QMenu* m = menuProject_;
    menuProjectEnable = m->addAction(QIcon(":/images/replot32.png"), tr_("Enable Plot"));
    menuProjectDisable = m->addAction(QIcon(":/images/clear_plot32.png"), tr_("Disable Plot"));
    m->addSeparator();
    menuProjectColor = m->addMenu(QIcon(":/images/set_color32.png"), tr_("Set Color"));
    const std::pair<const char*, const char*> colors[] = {
        {":/images/red32.png", "Red"},       {":/images/blue32.png", "Blue"},     {":/images/yellow32.png", "Yellow"},
        {":/images/green32.png", "Green"},   {":/images/violet32.png", "Purple"}, {":/images/brown32.png", "Brown"},
        {":/images/white32.png", "White"},   {":/images/black32.png", "Black"}};
    for (const auto& [icon, name] : colors) {
        QAction* a = menuProjectColor->addAction(QIcon(icon), tr_(name));
        connect(a, &QAction::triggered, this, [this, n = QString(name)] { setSelectedColor(n); });
    }
    menuProjectColor->addSeparator();
    for (const char* name : {"Custom"}) {
        QAction* a = menuProjectColor->addAction(QIcon(":/images/set_color32.png"), tr_(name));
        connect(a, &QAction::triggered, this, [this, n = QString(name)] { setSelectedColor(n); });
    }
    menuProjectColor->addSeparator();
    for (const char* name : {"Opacity", "Default"}) {
        QAction* a = menuProjectColor->addAction(QIcon(":/images/set_color32.png"), tr_(name));
        connect(a, &QAction::triggered, this, [this, n = QString(name)] { setSelectedColor(n); });
    }
    m->addSeparator();
    menuProjectGenerateCnc = m->addAction(QIcon(":/images/cnc32.png"), tr_("Create CNCJob"));
    menuProjectViewSource = m->addAction(QIcon(":/images/source32.png"), tr_("View Source"));
    menuProjectEdit = m->addAction(QIcon(":/images/edit_ok32.png"), tr_("Edit"));
    menuProjectCopy = m->addAction(QIcon(":/images/copy32.png"), tr_("Copy"));
    menuProjectDelete = m->addAction(QIcon(":/images/delete32.png"), tr_("Delete"));
    menuProjectSave = m->addAction(QIcon(":/images/save_as.png"), tr_("Save"));
    m->addSeparator();
    menuProjectProperties = m->addAction(QIcon(":/images/properties32.png"), tr_("Properties"));

    connect(menuProjectEnable, &QAction::triggered, this, [this] { setSelectedPlots(true); });
    connect(menuProjectDisable, &QAction::triggered, this, [this] { setSelectedPlots(false); });
    connect(menuProjectGenerateCnc, &QAction::triggered, this, [this] {
        // generate_cnc_job: un CNC Job por Geometría elegida, con todas sus herramientas.
        for (FlatCAMObj* o : selectedObjects()) {
            if (auto* g = dynamic_cast<GeometryObject*>(o)) {
                std::vector<int> ids;
                for (const auto& [uid, t] : g->tools()) ids.push_back(uid);
                generateCncJob(g, ids);
            }
        }
    });
    connect(menuProjectViewSource, &QAction::triggered, this, [this] { viewSource(activeObject()); });
    connect(menuProjectEdit, &QAction::triggered, this, [this] {
        // Los editores de Gerber, Excellon y Geometría quedan fuera del alcance del port.
        ui_->inform("[WARNING_NOTCL] Los editores no forman parte del port.");
    });
    connect(menuProjectCopy, &QAction::triggered, this, [this] { copySelected(); });
    connect(menuProjectDelete, &QAction::triggered, this, [this] { deleteSelected(); });
    connect(menuProjectSave, &QAction::triggered, this, [this] { saveSelected(); });
    // obj_properties: la herramienta Propiedades, run(toggle=False).
    connect(menuProjectProperties, &QAction::triggered, this, [this] { propertiesTool->run(false); });
    connect(ui_->collection_view, &QWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        updateProjectMenu();
        menuProject_->popup(ui_->collection_view->mapToGlobal(pos));
    });
}

void App::updateProjectMenu() {
    // ObjectCollection.on_menu_request
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    const bool any = !sel.empty();
    for (QAction* a : {menuProjectEnable, menuProjectDisable, menuProjectViewSource, menuProjectCopy, menuProjectEdit,
                       menuProjectDelete, menuProjectSave, menuProjectProperties})
        a->setEnabled(any);
    menuProjectColor->setEnabled(any);
    menuProjectGenerateCnc->setVisible(any);
    if (!any) return;
    for (QAction* a : {menuProjectEdit, menuProjectSave, menuProjectViewSource}) a->setVisible(true);
    menuProjectColor->setEnabled(false);
    for (FlatCAMObj* o : sel) {
        const ObjectKind k = o->kind();
        if (k == ObjectKind::Gerber || k == ObjectKind::Excellon) menuProjectColor->setEnabled(true);
        if (k != ObjectKind::Geometry) menuProjectGenerateCnc->setVisible(false);
        if (k != ObjectKind::Gerber && k != ObjectKind::Excellon && k != ObjectKind::CNCJob)
            menuProjectViewSource->setVisible(false);
    }
}

void App::setSelectedPlots(bool on) {
    for (FlatCAMObj* o : selectedObjects()) {
        o->setPlotEnabled(on);
        ui_->plotCanvas()->setLayerVisible(o->id(), on);
        if (o == activeObject()) showProperties(o);
    }
    ui_->inform("[success] " + tr_(on ? "Selected plots enabled..." : "Selected plots disabled..."));
}

void App::setSelectedColor(const QString& action) {
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) return;
    // on_set_color_action_triggered: transparencia BF (gerber_alpha / excellon_alpha de fábrica, 191) o FF en Geometría.
    auto alphaOf = [](const FlatCAMObj* o) { return o->kind() == ObjectKind::Geometry ? 255 : 191; };
    auto apply = [this](FlatCAMObj* o, const QColor& fill, const QColor& line) {
        o->fillColor = fill;
        o->outlineColor = line;
        ui_->plotCanvas()->setLayer(o->id(), o->plotData());
    };
    if (action == "Default") {
        for (FlatCAMObj* o : sel) {
            if (o->kind() == ObjectKind::Excellon) apply(o, colorFromFlatCam("#C40000BF"), colorFromFlatCam("#750000BF"));
            else if (o->kind() == ObjectKind::Gerber) apply(o, colorFromFlatCam("#BBF268BF"), colorFromFlatCam("#006E20BF"));
            else if (o->kind() == ObjectKind::Geometry) apply(o, colorFromFlatCam("#FFAAAA"), colorFromFlatCam("#FFAAAA"));
        }
        return;
    }
    if (action == "Opacity") {
        bool ok = false;
        const int alpha = QInputDialog::getInt(ui_.get(), tr_("Set alpha level ..."), tr_("Value") + ":", 191, 0, 255, 1, &ok);
        if (!ok) return;
        for (FlatCAMObj* o : sel) {
            QColor c = o->fillColor;
            c.setAlpha(alpha);
            apply(o, c, o->outlineColor);
        }
        return;
    }
    QColor base;
    if (action == "Custom") {
        base = QColorDialog::getColor(QColor("#BBF268"), ui_.get());
        if (!base.isValid()) return;
    } else {
        static const std::map<QString, QString> named = {{"Red", "#FF0000"},    {"Blue", "#0000FF"},  {"Yellow", "#FFDF00"},
                                                         {"Green", "#00FF00"},  {"Purple", "#FF00FF"}, {"Brown", "#A52A2A"},
                                                         {"White", "#FFFFFF"},  {"Black", "#000000"}};
        base = QColor(named.at(action));
    }
    // El contorno es el mismo color al 70 % (en blanco, el de #dedede).
    const QColor line = colorVariant(action == "White" ? QColor("#dedede") : base, 0.7);
    for (FlatCAMObj* o : sel) {
        QColor fill = base;
        fill.setAlpha(alphaOf(o));
        apply(o, fill, line);
    }
}

void App::viewSource(FlatCAMObj* obj) {
    std::string text;
    if (auto* g = dynamic_cast<GerberObject*>(obj)) text = g->gerber().sourceFile();
    else if (auto* e = dynamic_cast<ExcellonObject*>(obj)) text = e->excellon().sourceFile();
    else if (auto* c = dynamic_cast<CNCJobObject*>(obj)) text = c->exportText();
    else {
        ui_->inform("[WARNING_NOTCL] " + tr_("Select an Gerber or Excellon file to view it's source file."));
        return;
    }
    ui_->inform(tr_("Viewing the source code of the selected object."));
    // on_view_source abre un editor de texto (AppTextEditor) en una pestaña del área de dibujo; acá, solo lectura.
    auto* editor = new QPlainTextEdit(QString::fromStdString(text));
    editor->setReadOnly(true);
    editor->setObjectName("source_editor_tab");
    editor->setLineWrapMode(QPlainTextEdit::NoWrap);
    const int idx = ui_->plot_tab_area->addTab(editor, tr_("Source Editor"));
    ui_->plot_tab_area->setCurrentIndex(idx);
}

std::vector<FlatCAMObj*> App::copySelected() {
    // on_copy_command: una copia `<nombre>_copy` de cada Gerber, Excellon o Geometría (los CNC Job no se copian).
    std::vector<FlatCAMObj*> out;
    const JsonValue& options = appOptions();
    for (FlatCAMObj* o : selectedObjects()) {
        if (o->kind() == ObjectKind::CNCJob) continue;
        JsonValue d = ProjectIO::toDict(*o, options);
        d.find("options")->set("name", JsonValue::string((o->name() + "_copy").toStdString()));
        QString warning;
        if (auto copy = ProjectIO::objectFrom(d, warning)) out.push_back(addObject(std::move(copy)));
    }
    return out;
}

bool App::deleteSelected(bool force) {
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) {
        ui_->inform(QString("[ERROR_NOTCL] %1 %2").arg(tr_("Failed."), tr_("No object is selected.")));
        return false;
    }
    // global_delete_confirmation (verdadero de fábrica): se pregunta antes de borrar.
    if (!force && defBool("global_delete_confirmation")) {
        QMessageBox box(ui_.get());
        box.setWindowTitle(tr_("Delete objects"));
        box.setWindowIcon(QIcon(":/images/deleteshape32.png"));
        box.setIcon(QMessageBox::Question);
        box.setText(tr_("Are you sure you want to permanently delete\nthe selected objects?"));
        QPushButton* ok = box.addButton(tr_("Ok"), QMessageBox::AcceptRole);
        box.addButton(tr_("Cancel"), QMessageBox::RejectRole);
        box.setDefaultButton(ok);
        box.exec();
        if (box.clickedButton() != ok) return false;
    }
    ui_->properties_scroll_area->takeWidget();
    for (FlatCAMObj* o : sel) {
        const QString name = o->name();
        ui_->plotCanvas()->removeLayer(o->id());
        collection_->remove(o);
        ui_->inform(QString("%1: %2").arg(tr_("Object deleted"), name));
    }
    showProperties(nullptr);  // setup_default_properties_tab
    ui_->plotCanvas()->setLayerOrder(collection_->drawOrder());
    ui_->inform(tr_("Object(s) deleted") + "...");
    shouldWeSave_ = true;
    return true;
}

void App::saveSelected() {
    // on_project_context_save: según el tipo del objeto activo.
    FlatCAMObj* o = activeObject();
    if (auto* c = dynamic_cast<CNCJobObject*>(o)) {
        exportGcode(c);
        return;
    }
    std::string source;
    QString filter;
    if (auto* g = dynamic_cast<GerberObject*>(o)) {
        source = g->gerber().sourceFile();
        filter = "Gerber File .gbr (*.GBR);;All Files (*.*)";  // on_file_savegerber
    } else if (auto* e = dynamic_cast<ExcellonObject*>(o)) {
        source = e->excellon().sourceFile();
        filter = "Excellon File .drl (*.DRL);;All Files (*.*)";  // on_file_saveexcellon
    } else {
        onFileExportDxf();  // on_project_context_save: una Geometría se exporta a DXF
        return;
    }
    const QString f = QFileDialog::getSaveFileName(ui_.get(), tr_("Save"), lastFolder() + "/" + o->name(), filter);
    if (f.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1").arg(tr_("Cancelled.")));
        return;
    }
    QFile out(f);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
    out.write(QByteArray::fromStdString(source));
    ui_->inform(QString("[success] %1: %2").arg(tr_("File saved to"), f));
}

}  // namespace flatcam::gui
