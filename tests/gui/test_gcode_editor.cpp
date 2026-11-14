// El editor de G-code de los CNC Job y "Revisar código CNC", contra el original
// (tests/tools/oracle_gcode_editor.py -> tests/data/gcode_editor/oracle.json).
#include <gtest/gtest.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QScrollArea>
#include <QTextBlock>
#include <QToolButton>

#include "gui/App.h"
#include "gui/AppTextEditor.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/editors/GCodeEditor.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/objects/ObjectUIs.h"

/** Para que GoogleTest muestre los QString como texto. */
inline void PrintTo(const QString& s, std::ostream* os) { *os << '"' << s.toStdString() << '"'; }

using namespace flatcam;
using namespace flatcam::gui;

namespace {

const QString kDir = QString(FLATCAM_TEST_DATA_DIR);

QJsonObject oracle() {
    static const QJsonObject o = [] {
        QFile f(kDir + "/gcode_editor/oracle.json");
        EXPECT_TRUE(f.open(QIODevice::ReadOnly));
        return QJsonDocument::fromJson(f.readAll()).object();
    }();
    return o;
}

void select(App& app, FlatCAMObj* o) {
    auto* view = app.ui()->collection_view;
    view->selectionModel()->clearSelection();
    view->setCurrentIndex(app.collection()->indexOf(o));
    view->selectionModel()->select(app.collection()->indexOf(o), QItemSelectionModel::Select);
}

/** Sin la fecha de la cabecera (un trabajo creado ahora tiene otra). */
QString noDate(QString s) {
    static const QRegularExpression re(R"(\(Created on [^)]*\))");
    return s.replace(re, "(Created on)");
}

void expectTable(FCTable* t, const QJsonObject& o) {
    EXPECT_EQ(t->isHidden(), o["hidden"].toBool());
    if (o["hidden"].toBool()) return;
    const QJsonArray rows = o["rows"].toArray();
    ASSERT_EQ(t->rowCount(), rows.size());
    for (int r = 0; r < rows.size(); ++r) {
        const QJsonArray cells = rows[r].toArray();
        ASSERT_EQ(t->columnCount(), cells.size());
        for (int c = 0; c < cells.size(); ++c) {
            SCOPED_TRACE(QString("fila %1 columna %2").arg(r).arg(c).toStdString());
            const QTableWidgetItem* it = t->item(r, c);
            if (cells[c].isNull()) {
                EXPECT_EQ(it, nullptr);
            } else {
                ASSERT_NE(it, nullptr);
                EXPECT_EQ(it->text(), cells[c].toString());
            }
        }
    }
    QJsonArray hidden;
    for (int c = 0; c < t->columnCount(); ++c)
        if (t->isColumnHidden(c)) hidden.append(c);
    EXPECT_EQ(hidden, o["hiddenColumns"].toArray());
}

/** Entra al editor, prueba la tabla y las selecciones, sale guardando y revisa el código. */
void checkJob(App& app, CNCJobObject* cnc, const QJsonObject& j, bool exact) {
    select(app, cnc);
    app.objectToEditor();
    ASSERT_EQ(app.callSource(), "gcode_editor");
    GCodeEditor* ge = app.gcodeEditor;
    QPointer<AppTextEditor> tab = ge->tab();
    ASSERT_NE(tab, nullptr);
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    EXPECT_EQ(area->currentWidget(), tab);
    EXPECT_EQ(area->tabText(area->indexOf(tab)), j["tabTitle"].toString());
    EXPECT_EQ(tab->objectName(), j["tabName"].toString());
    EXPECT_EQ(tab->buttonRun->isHidden(), j["runHidden"].toBool());
    EXPECT_EQ(app.ui()->properties_scroll_area->widget(), ge->panel());
    // El texto cargado es source_file: el G-code completo.
    if (exact) {
        EXPECT_EQ(tab->toPlainText(), j["text"].toString());
        EXPECT_EQ(ge->panel()->name_entry->text(), j["nameEntry"].toString());
    } else {
        EXPECT_EQ(noDate(tab->toPlainText()), noDate(j["text"].toString()));
    }
    expectTable(ge->panel()->cnc_tools_table, j["cnc"].toObject());
    expectTable(ge->panel()->exc_cnc_tools_table, j["exc"].toObject());

    FCTable* table = cnc->isDrilling() ? ge->panel()->exc_cnc_tools_table : ge->panel()->cnc_tools_table;
    auto* ed = tab->plainEditor();
    const QString doc = ed->toPlainText();
    for (const QJsonValue& v : j["selections"].toArray()) {
        const QJsonObject s = v.toObject();
        SCOPED_TRACE(QJsonDocument(s["rows"].toArray()).toJson(QJsonDocument::Compact).toStdString());
        ed->setExtraSelections({});
        table->clearSelection();
        for (const QJsonValue& r : s["rows"].toArray())
            table->selectionModel()->select(table->model()->index(r.toInt(), 0),
                                            QItemSelectionModel::Select | QItemSelectionModel::Rows);
        ge->onRowSelectionChange();
        const QTextCursor c = ed->textCursor();
        QJsonArray extra;
        QStringList extraText, theirExtraText;
        for (const QTextEdit::ExtraSelection& e : ed->extraSelections()) {
            extra.append(QJsonArray{e.cursor.selectionStart(), e.cursor.selectionEnd()});
            extraText << noDate(e.cursor.selectedText());
        }
        const QString theirText = j["text"].toString();
        for (const QJsonValue& e : s["extra"].toArray()) {
            const int a = e.toArray()[0].toInt(), b = e.toArray()[1].toInt();
            theirExtraText << noDate(theirText.mid(a, b - a).replace('\n', QChar(0x2029)));
        }
        EXPECT_EQ(noDate(c.selectedText()), noDate(s["text"].toString()));
        EXPECT_EQ(extraText, theirExtraText);
        if (exact) {
            EXPECT_EQ(c.selectionStart(), s["start"].toInt());
            EXPECT_EQ(c.selectionEnd(), s["end"].toInt());
            EXPECT_EQ(extra, s["extra"].toArray());
        }
    }
    (void)doc;
    // Un clic en el encabezado elige todas las filas; otro, ninguna.
    table->clearSelection();
    table->selectRow(3);
    ge->onToggleAllRows();
    std::set<int> rows;
    for (const QModelIndex& i : table->selectionModel()->selectedIndexes()) rows.insert(i.row());
    EXPECT_EQ(static_cast<int>(rows.size()), j["toggle"].toArray()[0].toInt());
    ge->onToggleAllRows();
    EXPECT_EQ(static_cast<int>(table->selectionModel()->selectedIndexes().size()), j["toggle"].toArray()[1].toInt());

    // Salir guardando: lo editado queda en source_file, se cierra la pestaña y vuelven las Propiedades del objeto.
    ed->moveCursor(QTextCursor::End);
    ed->insertPlainText("M30\n");
    ASSERT_TRUE(app.editorToObject(true));
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    EXPECT_EQ(app.callSource(), "app");
    EXPECT_EQ(cnc->sourceFile().right(40), j["savedSourceTail"].toString());
    EXPECT_EQ(tab, nullptr);  // la pestaña se cerró (QPointer)
    EXPECT_EQ(ge->tab(), nullptr);
    EXPECT_EQ(app.ui()->properties_scroll_area->widget(), cnc->builtUi());

    // Revisar código CNC.
    const QJsonObject rv = j["review"].toObject();
    AppTextEditor* review = app.reviewGcode(cnc);
    EXPECT_EQ(area->tabText(area->indexOf(review)), rv["tabTitle"].toString());
    EXPECT_EQ(review->objectName(), rv["tabName"].toString());
    EXPECT_EQ(review->plainEditor()->isReadOnly(), rv["readOnly"].toBool());
    if (exact) EXPECT_EQ(review->toPlainText(), rv["text"].toString());
    const QJsonObject hidden = rv["hidden"].toObject();
    const std::map<QString, QWidget*> mine = {
        {"buttonSave", review->buttonSave},       {"buttonOpen", review->buttonOpen},
        {"buttonReplace", review->buttonReplace}, {"sel_all_cb", review->sel_all_cb},
        {"entryReplace", review->entryReplace},   {"buttonRun", review->buttonRun},
        {"buttonFind", review->buttonFind},       {"buttonPrint", review->buttonPrint},
        {"buttonPreview", review->buttonPreview}};
    for (auto it = hidden.begin(); it != hidden.end(); ++it) {
        SCOPED_TRACE(it.key().toStdString());
        EXPECT_EQ(mine.at(it.key())->isHidden(), it.value().toBool());
    }
}

}  // namespace

TEST(GuiGCodeEditor, ProjectJobsLikeTheOriginal) {
    App app;
    const QString dir = kDir + "/luz_de_noche/";
    ASSERT_TRUE(app.openProject(dir + "luz_de_noche_reducido.FlatPrj"));
    const QJsonObject jobs = oracle()["jobs"].toObject();
    for (const QString name : {QString("luz de noche Pascual-PTH.drl_cnc"),
                               QString("luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc")}) {
        SCOPED_TRACE(name.toStdString());
        auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName(name));
        ASSERT_NE(cnc, nullptr);
        checkJob(app, cnc, jobs[name].toObject(), true);
    }
}

TEST(GuiGCodeEditor, SessionJobsLikeTheOriginal) {
    // Los mismos trabajos como si se hubieran creado en la sesión (claves numéricas en cnc_tools y exc_cnc_tools): las
    // filas de las herramientas eligen su G-code.
    App app;
    ASSERT_TRUE(app.openProject(kDir + "/luz_de_noche/luz_de_noche_reducido.FlatPrj"));
    const QJsonObject jobs = oracle()["jobs"].toObject();
    for (const QString name : {QString("luz de noche Pascual-PTH.drl_cnc"),
                               QString("luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc")}) {
        SCOPED_TRACE(name.toStdString());
        auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName(name));
        ASSERT_NE(cnc, nullptr);
        cnc->projectData = JsonValue();
        checkJob(app, cnc, jobs[name + " (int)"].toObject(), true);
    }
}

TEST(GuiGCodeEditor, CancelAndShortcuts) {
    App app;
    ASSERT_TRUE(app.openProject(kDir + "/luz_de_noche/luz_de_noche_reducido.FlatPrj"));
    auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
    ASSERT_NE(cnc, nullptr);
    const QString before = cnc->sourceFile();
    // El botón "Editor de código G" del panel entra al editor.
    select(app, cnc);
    auto* panel = qobject_cast<CNCObjectUI*>(cnc->builtUi());
    ASSERT_NE(panel, nullptr);
    panel->editor_button->click();
    ASSERT_EQ(app.callSource(), "gcode_editor");
    QPointer<AppTextEditor> tab = app.gcodeEditor->tab();
    tab->plainEditor()->insertPlainText("X");
    // Salir sin guardar: source_file queda igual y se cierra la pestaña.
    ASSERT_TRUE(app.editorToObject(false));
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    EXPECT_EQ(cnc->sourceFile(), before);
    EXPECT_EQ(tab, nullptr);
    // Ctrl+S dentro del editor guarda y sale.
    app.objectToEditor();
    ASSERT_EQ(app.callSource(), "gcode_editor");
    tab = app.gcodeEditor->tab();
    tab->plainEditor()->moveCursor(QTextCursor::End);
    tab->plainEditor()->insertPlainText("M30\n");
    // Como en el original, pregunta si se guarda: "Sí".
    QTimer::singleShot(0, [] {
        auto* box = qobject_cast<QMessageBox*>(QApplication::activeModalWidget());
        ASSERT_NE(box, nullptr);
        for (QAbstractButton* b : box->buttons())
            if (box->buttonRole(b) == QMessageBox::YesRole) b->click();
    });
    QKeyEvent ctrlS(QEvent::KeyPress, Qt::Key_S, Qt::ControlModifier, "s");
    QApplication::sendEvent(app.ui(), &ctrlS);
    EXPECT_EQ(app.callSource(), "app");
    EXPECT_TRUE(cnc->sourceFile().endsWith("M30\n"));
    // "Insertar código" no hace nada (insert_gcode es pass).
    app.objectToEditor();
    tab = app.gcodeEditor->tab();
    const QString text = tab->toPlainText();
    app.gcodeEditor->panel()->prepend_text->setPlainText("G04 P1");
    app.gcodeEditor->panel()->update_gcode_button->click();
    app.gcodeEditor->panel()->update_gcode_sec_button->click();
    EXPECT_EQ(tab->toPlainText(), text);
    ASSERT_TRUE(app.editorToObject(false));
}

TEST(GuiCncExport, SnippetsFooterAndLineEndings) {
    // export_gcode: los fragmentos de las Preferencias, M02 al final con cncjob_footer y \r\n con cncjob_line_ending.
    App app;
    ASSERT_TRUE(app.openProject(kDir + "/luz_de_noche/luz_de_noche_reducido.FlatPrj"));
    auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
    ASSERT_NE(cnc, nullptr);
    const std::string plain = cnc->exportText();
    const std::string header = cnc->headerText(), start = cnc->job().startGcode(), body = cnc->job().gcode();
    EXPECT_EQ(plain, header + start + "\n" + body + "\n");
    app.preferences()->set("cncjob_prepend", JsonValue::string("G04 P1"));
    EXPECT_EQ(cnc->exportText(), header + start + "\nG04 P1\n" + body + "\n");
    app.preferences()->set("cncjob_append", JsonValue::string("M05"));
    EXPECT_EQ(cnc->exportText(), header + start + "\nG04 P1\n" + body + "\nM05\n");
    app.preferences()->set("cncjob_prepend", JsonValue::string(""));
    EXPECT_EQ(cnc->exportText(), header + start + "\n" + body + "\nM05\n");
    app.preferences()->set("cncjob_append", JsonValue::string(""));
    app.preferences()->set("cncjob_footer", JsonValue::boolean(true));
    EXPECT_EQ(cnc->exportText(), header + start + "\n" + body + "\nM02");
    // Los fragmentos pasados ganan a los de las Preferencias.
    EXPECT_EQ(cnc->exportText("A", "B"), header + start + "\nA\n" + body + "\nB\nM02");
    app.preferences()->set("cncjob_footer", JsonValue::boolean(false));
    app.preferences()->set("cncjob_line_ending", JsonValue::boolean(true));
    const QString file = QDir::tempPath() + "/flatcam_crlf_test.nc";
    ASSERT_TRUE(cnc->saveTo(file));
    QFile f(file);
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    const QByteArray bytes = f.readAll();
    EXPECT_TRUE(bytes.contains("\r\n"));
    EXPECT_EQ(QString::fromUtf8(bytes).replace("\r\n", "\n").toStdString(), plain);
    f.close();
    QFile::remove(file);
    app.preferences()->set("cncjob_line_ending", JsonValue::boolean(false));
}
