// Objetos Script y Documento y el editor de texto (AppTextEditor), contra el original
// (tests/tools/oracle_script_document.py -> tests/data/scripts/oracle.json).
#include <gtest/gtest.h>

#include <QApplication>
#include <QCheckBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QTemporaryDir>
#include <QToolButton>

#include "gui/App.h"
#include "gui/AppTextEditor.h"
#include "gui/ProjectIO.h"
#include "gui/objects/DocumentObject.h"
#include "gui/objects/ScriptObject.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {

const QString kDir = QString(FLATCAM_TEST_DATA_DIR) + "/scripts";

QJsonObject oracle() {
    static const QJsonObject o = [] {
        QFile f(kDir + "/oracle.json");
        EXPECT_TRUE(f.open(QIODevice::ReadOnly));
        return QJsonDocument::fromJson(f.readAll()).object();
    }();
    return o;
}

/** Las opciones del to_dict del port, comparadas con las del original (el archivo las guarda ordenadas). */
void expectOptions(const JsonValue& mine, const QJsonArray& theirs) {
    ASSERT_TRUE(mine.isObject());
    EXPECT_EQ(mine.members().size(), static_cast<std::size_t>(theirs.size()));
    for (const QJsonValue& kv : theirs) {
        const std::string key = kv.toArray()[0].toString().toStdString();
        const QJsonValue v = kv.toArray()[1];
        SCOPED_TRACE(key);
        const JsonValue* m = mine.find(key);
        ASSERT_NE(m, nullptr);
        if (v.isBool()) EXPECT_EQ(m->toBool(), v.toBool());
        else if (v.isDouble()) EXPECT_EQ(m->toDouble(), v.toDouble());
        else EXPECT_EQ(QString::fromStdString(m->text()), v.toString());
    }
}

void expectEditorWidgets(AppTextEditor* ed, const QJsonArray& widgets) {
    const std::map<QString, QWidget*> mine = {
        {"buttonFind", ed->buttonFind},       {"entryFind", ed->entryFind},     {"buttonReplace", ed->buttonReplace},
        {"entryReplace", ed->entryReplace},   {"sel_all_cb", ed->sel_all_cb},   {"button_update_code", ed->button_update_code},
        {"buttonPreview", ed->buttonPreview}, {"buttonPrint", ed->buttonPrint}, {"buttonOpen", ed->buttonOpen},
        {"buttonSave", ed->buttonSave},       {"buttonRun", ed->buttonRun}};
    for (const QJsonValue& v : widgets) {
        const QJsonObject w = v.toObject();
        SCOPED_TRACE(w["name"].toString().toStdString());
        QWidget* x = mine.at(w["name"].toString());
        EXPECT_EQ(x->toolTip(), w["toolTip"].toString());
        EXPECT_EQ(x->isHidden(), w["hidden"].toBool());
        if (auto* b = qobject_cast<QAbstractButton*>(x)) EXPECT_EQ(b->text(), w["text"].toString());
    }
}

/** Elige un objeto en el árbol del Proyecto (como un clic). */
void select(App& app, FlatCAMObj* o) {
    auto* view = app.ui()->collection_view;
    view->selectionModel()->clearSelection();
    view->setCurrentIndex(app.collection()->indexOf(o));
    view->selectionModel()->select(app.collection()->indexOf(o), QItemSelectionModel::Select);
}

void press(QWidget* w, int key, Qt::KeyboardModifiers mod, const QString& text) {
    QKeyEvent ev(QEvent::KeyPress, key, mod, text);
    QApplication::sendEvent(w, &ev);
}

}  // namespace

TEST(GuiScript, NewScriptLikeTheOriginal) {
    App app;
    const QJsonObject o = oracle();
    auto* s = static_cast<ScriptObject*>(app.onFileNewScript());
    ASSERT_NE(s, nullptr);
    const QJsonObject ns = o["new_script"].toObject();
    EXPECT_EQ(s->name(), "new_script");
    EXPECT_EQ(s->sourceFile(), ns["source_file"].toString());
    EXPECT_EQ(s->editor()->toPlainText(), o["new_script_editor_text"].toString());
    const JsonValue d = ProjectIO::toDict(*s, app.appOptions());
    EXPECT_EQ(d["kind"].text(), "script");
    expectOptions(d["options"], ns["options"].toArray());
    // La pestaña "Editor de guiones" con el nombre del objeto, a la vista.
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    const int idx = area->indexOf(s->editor());
    ASSERT_GE(idx, 0);
    EXPECT_EQ(area->tabText(idx), o["new_script_tab"].toArray()[0].toString());
    EXPECT_EQ(s->editor()->objectName(), o["new_script_tab"].toArray()[1].toString());
    EXPECT_EQ(area->currentWidget(), s->editor());
    expectEditorWidgets(s->editor(), o["script_editor"].toArray());
    EXPECT_EQ(s->ui()->autocomplete_cb->isChecked(), o["script_ui"].toObject()["autocomplete"].toBool());
    // Cerrar la pestaña y volver a elegir el objeto la vuelve a poner (build_ui).
    area->closeTab(idx);
    EXPECT_LT(area->indexOf(s->editor()), 0);
    app.showProperties(s);
    EXPECT_GE(area->indexOf(s->editor()), 0);
    // Borrar el objeto cierra su pestaña.
    const int before = area->count();
    select(app, s);
    ASSERT_TRUE(app.deleteSelected(true));
    EXPECT_EQ(area->count(), before - 1);
}

TEST(GuiScript, OpensAndSavesScriptsLikeTheOriginal) {
    App app;
    const QJsonObject o = oracle();
    ScriptObject* last = nullptr;
    for (const QJsonValue& v : o["opened"].toArray()) {
        const QJsonObject op = v.toObject();
        SCOPED_TRACE(op["file"].toString().toStdString());
        auto* s = static_cast<ScriptObject*>(app.openScript(kDir + "/" + op["file"].toString()));
        ASSERT_NE(s, nullptr);
        const QJsonObject d = op["dict"].toObject();
        EXPECT_EQ(s->name(), op["file"].toString());
        EXPECT_EQ(s->sourceFile(), d["source_file"].toString());  // CRLF -> LF, como open() de Python
        EXPECT_EQ(s->editor()->toPlainText(), op["editor_text"].toString());
        expectOptions(ProjectIO::toDict(*s, app.appOptions())["options"], d["options"].toArray());
        if (!last) last = s;
    }
    // Un archivo que no existe no crea nada.
    EXPECT_EQ(app.openScript(kDir + "/no_existe.tcl"), nullptr);
    // save_source_file: la cabecera G04 (de un Gerber) y el texto; la hora es la de ahora.
    QTemporaryDir tmp;
    const QString f = tmp.filePath("guardado.FlatScript");
    ASSERT_TRUE(app.saveSourceFile(last, f));
    QFile in(f);
    ASSERT_TRUE(in.open(QIODevice::ReadOnly));
    QStringList mine = QString::fromUtf8(in.readAll()).split('\n');
    QStringList theirs = o["saved_source"].toString().split('\n');
    ASSERT_EQ(mine.size(), theirs.size());
    EXPECT_TRUE(mine[3].startsWith("G04 Created on : ")) << mine[3].toStdString();
    mine.removeAt(3);
    theirs.removeAt(3);
    EXPECT_EQ(mine, theirs);
    // Con el texto vacío no se guarda.
    auto* empty = static_cast<DocumentObject*>(app.newDocumentObject());
    EXPECT_FALSE(app.saveSourceFile(empty, tmp.filePath("vacio.txt")));
    EXPECT_FALSE(QFile::exists(tmp.filePath("vacio.txt")));
}

TEST(GuiScript, ProjectRoundTrip) {
    App app;
    auto* s = static_cast<ScriptObject*>(app.openScript(kDir + "/ejemplo.FlatScript"));
    auto* d = static_cast<DocumentObject*>(app.newDocumentObject());
    ASSERT_TRUE(s && d);
    d->setSourceFile("<b>hola</b>");
    QTemporaryDir tmp;
    const QString f = tmp.filePath("p.FlatPrj");
    ASSERT_TRUE(app.saveProject(f));
    // Para comprobar que lo abre el original: tests/tools/check_project_in_original.py <archivo>.
    if (const QByteArray copy = qgetenv("FLATCAM_SAVE_COPY"); !copy.isEmpty()) {
        QFile::remove(QString::fromLocal8Bit(copy));
        QFile::copy(f, QString::fromLocal8Bit(copy));
    }
    App other;
    other.openProject(f);
    auto* s2 = dynamic_cast<ScriptObject*>(other.collection()->byName("ejemplo.FlatScript"));
    auto* d2 = dynamic_cast<DocumentObject*>(other.collection()->byName("new_document"));
    ASSERT_TRUE(s2 && d2);
    EXPECT_EQ(s2->sourceFile(), s->sourceFile());
    EXPECT_EQ(d2->sourceFile(), "<b>hola</b>");
    // Al abrirlo, el Documento con HTML se carga como texto con formato.
    EXPECT_EQ(d2->editor()->toPlainText(), "hola");
    // Copiar: "<nombre>_copy" con el mismo texto.
    select(other, s2);
    const auto copies = other.copySelected();
    ASSERT_EQ(copies.size(), 1u);
    EXPECT_EQ(copies[0]->name(), "ejemplo.FlatScript_copy");
    EXPECT_EQ(static_cast<ScriptObject*>(copies[0])->sourceFile(), s->sourceFile());
}

TEST(GuiDocument, NewDocumentLikeTheOriginal) {
    App app;
    const QJsonObject o = oracle();
    auto* d = static_cast<DocumentObject*>(app.newDocumentObject());
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(d->name(), "new_document");
    expectOptions(ProjectIO::toDict(*d, app.appOptions())["options"], o["new_document"].toObject()["options"].toArray());
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    ASSERT_GE(area->indexOf(d->editor()), 0);
    EXPECT_EQ(area->tabText(area->indexOf(d->editor())), o["new_document_tab"].toArray()[0].toString());
    expectEditorWidgets(d->editor(), o["document_editor"].toArray());
    const QJsonObject ui = o["document_ui"].toObject();
    EXPECT_EQ(d->ui()->font_size_cb->currentText(), ui["font_size"].toString());
    EXPECT_EQ(d->ui()->tab_size_spinner->value(), ui["tab_size"].toInt());
    EXPECT_EQ(d->ui()->font_color_entry->text(), ui["font_color"].toString());
    EXPECT_EQ(d->ui()->sel_color_entry->text(), ui["sel_color"].toString());
    EXPECT_EQ(d->ui()->font_color_button->styleSheet(), ui["font_color_style"].toString());
    EXPECT_EQ(d->ui()->autocomplete_cb->isChecked(), ui["autocomplete"].toBool());
    EXPECT_EQ(d->editor()->richEditor()->styleSheet(), ui["editor_style"].toString());
}

TEST(GuiDocument, EditsNeverReachTheSourceFile) {
    // El load_text de set_ui desconecta on_text_changed: lo que se escribe no llega a source_file.
    App app;
    const QJsonObject o = oracle()["documents"].toObject();
    const QString html = "<html><body><p><b>Informe</b></p><p>linea 2</p></body></html>";
    auto* dh = static_cast<DocumentObject*>(app.addObject(std::make_unique<DocumentObject>("doc_html", html)));
    auto* dt = static_cast<DocumentObject*>(
        app.addObject(std::make_unique<DocumentObject>("doc_texto", QString("linea 1\nlinea 2"))));
    dh->editor()->richEditor()->insertPlainText("agregado ");
    EXPECT_EQ(dh->sourceFile(), o["html"].toObject()["source_after_edit"].toString());
    EXPECT_EQ(dh->editor()->toPlainText(), o["html"].toObject()["plain"].toString());
    EXPECT_EQ(dt->sourceFile(), o["text"].toObject()["source_after_edit"].toString());
    EXPECT_EQ(dt->editor()->toPlainText(), o["text"].toObject()["plain"].toString());
    const QJsonArray rich = o["text"].toObject()["might_be_rich"].toArray();
    int i = 0;
    for (const QString t : {html, QString("linea 1\nlinea 2"), QString("a < b"), QString("<b>x</b>")})
        EXPECT_EQ(Qt::mightBeRichText(t), rich[i++].toBool()) << t.toStdString();
    // "Guardar" del menú del Proyecto: el original comprueba que sea un Script y siempre falla.
    select(app, dh);
    QTemporaryDir tmp;
    app.onFileSaveDocument(tmp.filePath("doc.FlatDoc"));
    EXPECT_FALSE(QFile::exists(tmp.filePath("doc.FlatDoc")));
}

TEST(GuiTextEditor, ButtonsFindReplaceCommentLikeTheOriginal) {
    App app;
    const QJsonObject o = oracle();
    AppTextEditor plain(&app, true), rich(&app, false);
    expectEditorWidgets(&plain, o["editor_plain"].toArray());
    expectEditorWidgets(&rich, o["editor_rich"].toArray());
    EXPECT_EQ(plain.plainEditor()->styleSheet(), o["editor_style"].toArray()[0].toString());
    EXPECT_EQ(rich.richEditor()->styleSheet(), o["editor_style"].toArray()[1].toString());

    const QString text = "G01 X1\nG00 Z5\nG01 X2\ng01 X3\n";
    plain.loadText(text, true);
    plain.entryFind->setText("G01");
    for (const QJsonValue& v : o["find"].toArray()) {
        plain.handleFindGCode();
        const QTextCursor c = plain.textCursor();
        EXPECT_EQ(c.selectionStart(), v.toArray()[0].toInt());
        EXPECT_EQ(c.selectionEnd(), v.toArray()[1].toInt());
    }
    plain.entryReplace->setText("G1");
    plain.handleReplaceGCode();
    EXPECT_EQ(plain.toPlainText(), o["replace_one"].toString());
    // "Todos" reemplaza desde el cursor hasta el final: con el cursor al final, nada.
    plain.sel_all_cb->setChecked(true);
    plain.loadText(text, true);
    plain.moveCursor(QTextCursor::End);
    plain.handleReplaceGCode();
    EXPECT_EQ(plain.toPlainText(), o["replace_all"].toString());
    EXPECT_EQ(plain.textCursor().position(), o["replace_all_cursor"].toInt());
    // Y desde el principio, todos.
    plain.loadText(text, true);
    plain.handleReplaceGCode();
    EXPECT_EQ(plain.toPlainText(), "G1 X1\nG00 Z5\nG1 X2\ng01 X3\n");

    for (const QJsonValue& v : o["comment"].toArray()) {
        const QJsonObject c = v.toObject();
        AppTextEditor& ed = c["editor"].toString() == "plain" ? plain : rich;
        SCOPED_TRACE((c["editor"].toString() + ": " + c["src"].toString()).toStdString());
        QWidget* w = ed.codeEditor();
        ed.loadText(c["src"].toString());
        QTextCursor cur = ed.textCursor();
        cur.setPosition(c["pos"].toInt());
        ed.setTextCursor(cur);
        press(w, Qt::Key_Slash, Qt::ControlModifier, "/");
        EXPECT_EQ(ed.toPlainText(), c["result"].toString());
        EXPECT_EQ(ed.textCursor().position(), c["cursor"].toInt());
    }
    for (const QJsonValue& v : o["brackets"].toArray()) {
        const QJsonObject c = v.toObject();
        AppTextEditor& ed = c["editor"].toString() == "plain" ? plain : rich;
        SCOPED_TRACE((c["editor"].toString() + ": " + c["keys"].toString()).toStdString());
        ed.loadText("");
        for (const QChar ch : c["keys"].toString()) {
            const int key = ch == '{' ? Qt::Key_BraceLeft : ch == '}' ? Qt::Key_BraceRight : ch == '(' ? Qt::Key_ParenLeft
                          : ch == ')' ? Qt::Key_ParenRight : ch == '[' ? Qt::Key_BracketLeft
                          : ch == ']' ? Qt::Key_BracketRight : Qt::Key_A + (ch.unicode() - 'a');
            press(ed.codeEditor(), key, Qt::NoModifier, QString(ch));
        }
        EXPECT_EQ(ed.toPlainText(), c["result"].toString());
        EXPECT_EQ(ed.textCursor().position(), c["cursor"].toInt());
    }

    // Guardar en un .txt: el texto tal cual y el botón vuelve a su color; editar lo pone en rojo.
    QTemporaryDir tmp;
    plain.loadText("linea 1\nlinea 2\n", true);
    ASSERT_TRUE(plain.handleSaveGCode({}, {}, tmp.filePath("editor.txt")));
    QFile f(tmp.filePath("editor.txt"));
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    EXPECT_EQ(QString::fromUtf8(f.readAll()), o["editor_saved"].toString());
    EXPECT_EQ(plain.buttonSave->styleSheet(), o["save_button_after_save"].toArray()[0].toString());
    plain.plainEditor()->insertPlainText("x");
    EXPECT_EQ(plain.buttonSave->styleSheet(), o["save_button_after_edit"].toArray()[0].toString());
    // Y en PDF.
    plain.loadText("G01 X1\nG00 Z5\n", true);
    ASSERT_TRUE(plain.handleSaveGCode({}, {}, tmp.filePath("editor.pdf")));
    QFile pdf(tmp.filePath("editor.pdf"));
    ASSERT_TRUE(pdf.open(QIODevice::ReadOnly));
    EXPECT_TRUE(pdf.readAll().startsWith("%PDF"));
}

TEST(GuiTextEditor, CodeEditorAndGotoLine) {
    App app;
    app.onToggleCodeEditor();
    AppTextEditor* ed = app.codeEditorTab();
    ASSERT_NE(ed, nullptr);
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    EXPECT_EQ(area->currentWidget(), ed);
    EXPECT_EQ(ed->objectName(), "text_editor_tab");
    EXPECT_EQ(area->tabText(area->indexOf(ed)), QString::fromUtf8("Editor de código"));
    ed->loadText("a\nb\nc\nd\n", true);
    app.gotoTextLine(3);
    EXPECT_EQ(ed->textCursor().blockNumber(), 2);
    app.onToggleCodeEditor();  // cierra
    EXPECT_LT(area->indexOf(ed), 0);
    // Un .FlatConfig se abre en el editor de código.
    app.openConfigFile(QString(FLATCAM_TEST_DATA_DIR) + "/scripts/bucle.tcl");
    ASSERT_NE(app.codeEditorTab(), nullptr);
    EXPECT_TRUE(app.codeEditorTab()->toPlainText().startsWith("version\n"));
}
