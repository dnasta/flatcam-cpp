#include "gui/objects/ScriptObject.h"

#include "gui/App.h"
#include "gui/AppTextEditor.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"

namespace flatcam::gui {

TextObject::TextObject(ObjectKind kind, QString name, QString source)
    : FlatCAMObj(kind, std::move(name)), source_(std::move(source)) {}

TextObject::~TextObject() {
    // El editor vive en una pestaña (o suelto, si se la cerró): se va con el objeto y Qt quita la pestaña.
    delete editor_.data();
}

AppTextEditor* TextObject::editor() const { return editor_; }

bool TextObject::addTabIfMissing(const QString& title) {
    // El original busca la pestaña por objectName == nombre del objeto; el port, por el propio editor (así no se
    // duplica la pestaña después de renombrar el objeto).
    FCDetachableTab2* area = app_->ui()->plot_tab_area;
    if (!editor_ || area->indexOf(editor_) >= 0) return false;
    area->addTab(editor_, title);
    editor_->setObjectName(name());
    return true;
}

ScriptObject::ScriptObject(QString name, QString source) : TextObject(ObjectKind::Script, std::move(name), std::move(source)) {}

void ScriptObject::setUi(App* app) {
    app_ = app;
    auto* panel = new ScriptObjectUI();
    ui_.reset(panel);
    panel->name_entry->setText(name());  // to_form
    if (defStr("global_app_level") == "b")
        panel->level->setText(QString("<span style=\"color:green;\"><b>%1</b></span>").arg(tr_("Basic")));
    else
        panel->level->setText(QString("<span style=\"color:red;\"><b>%1</b></span>").arg(tr_("Advanced")));

    auto* ed = new AppTextEditor(app, true);
    editor_ = ed;
    ed->setModelData(app->keywords());
    panel->autocomplete_cb->setValue(defBool("script_autocompleter"));
    ed->setCompleterEnabled(defBool("script_autocompleter"));
    ed->buttonRun->show();
    const QString flt = "FlatCAM Scripts (*.FlatScript);;All Files (*.*)";
    ed->onOpen = [ed, flt] { ed->handleOpen(flt); };
    ed->onSave = [ed, flt] { ed->handleSaveGCode({}, flt); };
    // handle_run_code: ejecuta el texto del editor en la consola Tcl.
    connect(ed, &AppTextEditor::runRequested, this, [this] { app_->runScriptCode(editor_->toPlainText()); });
    ed->handleTextChanged();
    connect(panel->autocomplete_cb, &QCheckBox::checkStateChanged, this,
            [this](Qt::CheckState s) { if (editor_) editor_->setCompleterEnabled(s != Qt::Unchecked); });
    // Cargar el texto (con el cursor al final).
    ed->t_frame->hide();
    ed->loadText(sourceFile(), false, true);
    ed->t_frame->show();
    buildUi();
}

void ScriptObject::buildUi() {
    if (addTabIfMissing(tr_("Script Editor"))) app_->ui()->plot_tab_area->setCurrentWidget(editor_);
}

}  // namespace flatcam::gui
