// Scripts, documentos y el editor de código: on_filenewscript, on_fileopenscript, open_script, on_filerunscript,
// new_document_object, save_source_file, on_file_savescript / on_file_savedocument, on_toggle_code_editor y
// goto_text_line del original (app_Main.py, appObjects/AppObject.py).
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QTextBlock>
#include <QTextStream>

#include "gui/App.h"
#include "gui/AppTextEditor.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/DocumentObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ScriptObject.h"

namespace flatcam::gui {

namespace {

const char* kScriptFilter =
    "TCL script .FlatScript (*.FlatScript);;TCL script .tcl (*.TCL);;TCL script .txt (*.TXT);;All Files (*.*)";

QStringList jsonList(const QJsonObject& o, const char* key) {
    QStringList out;
    for (const QJsonValue& v : o[key].toArray()) out << v.toString();
    return out;
}

/** `open(filename, "r").readlines()` unidas: el texto con los fines de línea de Python (universal newlines). */
std::optional<QString> readText(const QString& filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) return std::nullopt;
    QString s = QString::fromUtf8(f.readAll());
    s.replace("\r\n", "\n");
    s.replace('\r', '\n');
    return s;
}

}  // namespace

QStringList App::keywords() const {
    // myKeywords = tcl_commands_list + autocomplete_kw_list + tcl_keywords, más el nombre de cada objeto creado.
    static const QJsonObject base = [] {
        QFile f(":/tcl_keywords.json");
        if (!f.open(QIODevice::ReadOnly)) return QJsonObject();
        return QJsonDocument::fromJson(f.readAll()).object();
    }();
    QStringList out = jsonList(base, "tcl_commands_list");
    out << defStr("util_autocomplete_keywords").remove(' ').split(',');
    out << jsonList(base, "tcl_keywords");
    for (FlatCAMObj* o : collection_->objects()) out << o->name();
    return out;
}

FlatCAMObj* App::onFileNewScript(bool silent) {
    if (!silent) ui_->inform("[success] " + tr_("New TCL script file created in Code Editor."));
    ui_->coords_toolbar->hide();
    ui_->delta_coords_toolbar->hide();
    // new_script_object
    QString text = QString("# %1\n").arg(tr_("CREATE A NEW FLATCAM TCL SCRIPT")) +
                   QString("# %1:\n").arg(tr_("TCL Tutorial is here")) +
                   "# https://www.tcl.tk/man/tcl8.5/tutorial/tcltutorial.html\n" + "\n\n" +
                   QString("# %1:\n").arg(tr_("FlatCAM commands list"));
    text += QString("# %1\n\n").arg(tr_("Type >help< followed by Run Code for a list of FlatCAM Tcl Commands "
                                         "(displayed in Tcl Shell)."));
    return addObject(std::make_unique<ScriptObject>("new_script", text));
}

void App::onFileOpenScript(const QString& name) {
    QStringList files;
    if (!name.isEmpty()) files << name;
    else files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Open TCL script"), lastFolder(), kScriptFilter);
    if (files.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    for (const QString& f : files)
        if (!f.isEmpty()) openScript(f);
}

void App::onFileOpenScriptExample() {
    // La carpeta de ejemplos del original lleva '\\' también fuera de Windows (y no viene con la 2024.4).
    const QString examples = QCoreApplication::applicationDirPath() + "\\assets\\examples";
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Open TCL script"), examples, kScriptFilter);
    if (files.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    for (const QString& f : files)
        if (!f.isEmpty()) openScript(f);
}

FlatCAMObj* App::openScript(const QString& filename, const QString& outname) {
    const QString scriptName = outname.isEmpty() ? filename.section('/', -1).section('\\', -1) : outname;
    QString file = filename;
    // obj_init: el aviso va antes de leer el archivo.
    auto init = [this](const QString& f) -> std::optional<QString> {
        ui_->inform("[success] " + tr_("TCL script file opened in Code Editor."));
        auto text = readText(f);
        if (!text) ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open file"), f));
        return text;
    };
    auto text = init(file);
    if (!text) {
        // Si no está, se lo busca en la carpeta de scripts de las preferencias.
        file = defStr("global_tcl_path") + "/" + scriptName;
        text = init(file);
        if (!text) {
            ui_->inform("[ERROR_NOTCL]" + tr_("Failed to open TCL Script."));
            return nullptr;
        }
    }
    FlatCAMObj* obj = addObject(std::make_unique<ScriptObject>(scriptName, *text));
    registerRecent("script", file);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), file));
    return obj;
}

void App::onFileRunScript(const QString& name) {
    QString filename = name;
    if (filename.isEmpty())
        filename = QFileDialog::getOpenFileName(ui_.get(), tr_("Run TCL script"), lastFolder(), kScriptFilter);
    if (filename.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    const auto text = readText(filename);
    if (!text) return;
    runScriptCode(*text);
}

void App::runScriptCode(const QString& code) {
    // handle_run_code / exec_command: muestran la consola Tcl y le pasan el texto.
    (void)code;
    if (ui_->shell_dock->isHidden()) ui_->shell_dock->show();
    ui_->inform("[WARNING_NOTCL] La consola Tcl todavía no está en el port: el script no se ejecutó.");
}

FlatCAMObj* App::newDocumentObject() { return addObject(std::make_unique<DocumentObject>("new_document", QString())); }

FlatCAMObj* App::newGeometryObject() {
    // new_geometry_object: multiherramienta, con la herramienta 1 de geometry_cnctooldia (sin geometría).
    auto geo = std::make_unique<GeometryObject>("new_geo");
    GeometryTool t = defaultGeometryTool();
    t.offset = "Path";
    t.offsetValue = 0.0;
    t.type = "Rough";
    t.toolType = "C1";
    geo->tools()[1] = t;
    return addObject(std::move(geo), false);
}

FlatCAMObj* App::newGerberObject() {
    // new_gerber_object: sin aperturas ni geometría.
    return addObject(std::make_unique<GerberObject>("new_grb", std::make_unique<Gerber>()), false);
}

FlatCAMObj* App::newExcellonObject() {
    return addObject(std::make_unique<ExcellonObject>("new_exc", std::make_unique<Excellon>()), false);
}

bool App::saveSourceFile(FlatCAMObj* obj, const QString& filename) {
    auto* t = dynamic_cast<TextObject*>(obj);
    if (!t) return false;
    if (t->sourceFile().isEmpty()) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Save cancelled because source file is empty. Try to export the file."));
        return false;
    }
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui_->inform("[WARNING] " + tr_("Permission denied, saving not possible.\n"
                                       "Most likely another app is holding the file open and not accessible."));
        return false;
    }
    // La cabecera es la de un Gerber (G04), también en los scripts.
    const QString when = QLocale::c().toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm");
    const QString kind = obj->kind() == ObjectKind::Script ? "SCRIPT" : "DOCUMENT";
    QString out = "G04*\n";
    out += QString("G04 %1 (RE)GENERATED BY FLATCAM v%2 - www.flatcam.org - Version Date: %3*\n")
               .arg(kind, Preferences::version(), "2024/04/15");
    out += QString("G04 Filename: %1*\n").arg(obj->name());
    out += QString("G04 Created on : %1*\n").arg(when);
    out += t->sourceFile();
    f.write(out.toUtf8());
    return true;
}

void App::onFileSaveScript(const QString& filename) {
    FlatCAMObj* obj = activeObject();
    if (!obj) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    if (obj->kind() != ObjectKind::Script) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Failed. Only Script objects can be saved as TCL Script files..."));
        return;
    }
    QString f = filename;
    if (f.isEmpty())
        f = QFileDialog::getSaveFileName(ui_.get(), "Save Script source file", lastFolder() + "/" + obj->name(),
                                         "FlatCAM Scripts (*.FlatScript);;All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    saveSourceFile(obj, f);
    // "Script" con mayúscula no tiene ícono en los recientes: no aparece (KeyError en el original).
    if (!defBool("global_open_style")) registerRecent("Script", f);
}

void App::onFileSaveDocument(const QString& filename) {
    (void)filename;
    FlatCAMObj* obj = activeObject();
    if (!obj) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    // El original comprueba que sea un ScriptObject: con un Documento siempre falla.
    if (obj->kind() != ObjectKind::Script) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Failed. Only Document objects can be saved as Document files..."));
        return;
    }
}

AppTextEditor* App::codeEditorTab() const { return codeEditor_; }

void App::onToggleCodeEditor() {
    FCDetachableTab2* area = ui_->plot_tab_area;
    if (!codeEditor_ || area->indexOf(codeEditor_) < 0) {
        // init_code_editor: un editor de texto plano nuevo, sin "Ejecutar" ni autocompletado.
        delete codeEditor_.data();
        auto* ed = new AppTextEditor(this, true);
        codeEditor_ = ed;
        ed->setModelData(keywords());
        area->addTab(ed, tr_("Code Editor"));
        ed->setObjectName("text_editor_tab");
        ui_->coords_toolbar->hide();
        ui_->delta_coords_toolbar->hide();
        ed->setCompleterEnabled(false);
        ed->buttonRun->hide();
        area->setCurrentWidget(ed);
    } else {
        area->closeTab(area->indexOf(codeEditor_));
    }
}

void App::openConfigFile(const QString& filename) {
    // Si el editor de código está abierto se cierra, y se abre uno nuevo con el texto del archivo.
    if (codeEditor_ && ui_->plot_tab_area->indexOf(codeEditor_) >= 0) onToggleCodeEditor();
    onToggleCodeEditor();
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open config file"), filename));
        return;
    }
    QTextStream stream(&f);
    codeEditor_->loadText(stream.readAll(), true);
}

void App::gotoTextLine(std::optional<int> line) {
    auto* ed = qobject_cast<AppTextEditor*>(ui_->plot_tab_area->currentWidget());
    if (!ed) return;
    int n = 0;
    bool ok = true;
    if (line) {
        n = *line - 1;
    } else {
        // Dialog_box "Ir a la línea ...": un texto; si no es un número, la línea 1.
        QInputDialog dlg(ui_.get());
        dlg.setWindowTitle(tr_("Go to Line ..."));
        dlg.setWindowIcon(QIcon(":/images/jump_to16.png"));
        dlg.setLabelText(QString("%1:").arg(tr_("Line")));
        ok = dlg.exec() == QDialog::Accepted;
        bool isNum = false;
        n = dlg.textValue().trimmed().toInt(&isNum) - 1;
        if (!isNum) n = 0;
    }
    if (!ok) return;
    // Primero al final, así la línea buscada queda arriba de la ventana.
    ed->moveCursor(QTextCursor::End);
    ed->setTextCursor(QTextCursor(ed->document()->findBlockByLineNumber(n)));
}

}  // namespace flatcam::gui
