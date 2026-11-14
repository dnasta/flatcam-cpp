// La consola Tcl (ToolShell.FCShell y tclCommands), contra el original con el Tcl de tkinter
// (tests/tools/oracle_tcl_shell.py -> tests/data/tcl_shell/oracle.json).
#include <gtest/gtest.h>

#include <QFile>
#include <QTemporaryDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/shell/FCShell.h"
#include "gui/shell/TclCommand.h"
#include "gui/AppTextEditor.h"
#include "gui/objects/ScriptObject.h"
#include "gui/widgets/FCTextEdits.h"

/** Para que GoogleTest muestre los QString como texto. */
inline void PrintTo(const QString& s, std::ostream* os) { *os << '"' << s.toStdString() << '"'; }

using namespace flatcam;
using namespace flatcam::gui;

namespace {

QJsonObject oracle() {
    QFile f(QString(FLATCAM_TEST_DATA_DIR) + "/tcl_shell/oracle.json");
    EXPECT_TRUE(f.open(QIODevice::ReadOnly));
    return QJsonDocument::fromJson(f.readAll()).object();
}

}  // namespace

TEST(GuiTclShell, HelpOfEveryCommandLikeTheOriginal) {
    App app;
    const QJsonObject o = oracle();
    const QJsonObject help = o["help"].toObject(), desc = o["description"].toObject();
    ASSERT_EQ(static_cast<int>(app.shell()->commands().size()), help.size());
    for (auto it = help.begin(); it != help.end(); ++it) {
        SCOPED_TRACE(it.key().toStdString());
        const auto c = app.shell()->commands().find(it.key());
        ASSERT_NE(c, app.shell()->commands().end());
        EXPECT_EQ(c->second->decoratedHelp(), it.value().toString());
        EXPECT_EQ(c->second->meta().description, desc[it.key()].toString());
    }
}

TEST(GuiTclShell, SessionLikeTheOriginal) {
    App app;
    const QJsonObject o = oracle();
    FCShell* sh = app.shell();
    EXPECT_EQ(sh->browser()->toPlainText(), o["start"].toString());
    for (const QJsonValue& v : o["session"].toArray()) {
        const QJsonObject s = v.toObject();
        SCOPED_TRACE(s["cmd"].toString().toStdString());
        const QString before = sh->browser()->toPlainText();
        sh->commandLine()->setPlainText(s["cmd"].toString());
        sh->execCurrentCommand();
        const QString after = sh->browser()->toPlainText();
        const QString added = after.startsWith(before) ? after.mid(before.size()) : after;
        EXPECT_EQ(added, s["added"].toString());
        EXPECT_EQ(defStr("global_gridx"), s["gridx"].toString());
    }
    for (const QJsonValue& v : o["no_echo"].toArray()) {
        const QJsonObject s = v.toObject();
        SCOPED_TRACE(s["cmd"].toString().toStdString());
        EXPECT_EQ(sh->execCommand(s["cmd"].toString(), true), s["result"].toString());
    }
}

TEST(GuiTclShell, PythonConversionsAndArguments) {
    QString err;
    EXPECT_EQ(pyFloatFromString(" 1_000.5e1 ").value_or(0), 10005.0);
    EXPECT_FALSE(pyFloatFromString("1__0", &err));
    EXPECT_EQ(err, "could not convert string to float: '1__0'");
    EXPECT_EQ(pyFloatFromString("-Infinity").value_or(0), -std::numeric_limits<double>::infinity());
    EXPECT_EQ(pyIntFromString("+42").value_or(0), 42);
    EXPECT_FALSE(pyIntFromString("4.2", &err));
    EXPECT_EQ(err, "invalid literal for int() with base 10: '4.2'");
    std::vector<std::pair<QString, std::optional<QString>>> opts;
    const QStringList a = TclCommand::parseArguments({"x", "-a", "1", "-b", "-a", "2", "y", "-c"}, opts);
    EXPECT_EQ(a, (QStringList{"x", "y"}));
    ASSERT_EQ(opts.size(), 3u);
    EXPECT_EQ(opts[0].first, "a");
    EXPECT_EQ(opts[0].second.value_or(""), "2");  // la misma clave cambia el valor y queda en su lugar
    EXPECT_FALSE(opts[1].second.has_value());
    EXPECT_FALSE(opts[2].second.has_value());
}

TEST(GuiTclShell, RunButtonOfAScriptLikeTheOriginal) {
    App app;
    const QJsonObject rc = oracle()["run_code"].toObject();
    auto* script = static_cast<ScriptObject*>(app.onFileNewScript());
    script->editor()->plainEditor()->setPlainText(rc["script"].toString());
    app.shell()->browser()->clearLog();
    const QString before = app.shell()->browser()->toPlainText();
    script->editor()->buttonRun->click();
    EXPECT_EQ(app.shell()->browser()->toPlainText().mid(before.size()).toStdString(), rc["added"].toString().toStdString());
    EXPECT_FALSE(app.ui()->shell_dock->isHidden());
    // Archivo -> Ejecutar script: el archivo entero de una vez.
    QTemporaryDir tmp;
    QFile f(tmp.filePath("s.tcl"));
    ASSERT_TRUE(f.open(QIODevice::WriteOnly));
    f.write("set y 4\r\nexpr {$y + 1}\r\n");
    f.close();
    app.onFileRunScript(f.fileName());
    // El resultado y después el aviso de éxito.
    EXPECT_TRUE(app.shell()->browser()->toPlainText().contains("5\n[SUCCESS]"));
}

TEST(GuiTclShell, ToggleWithTheMenuAndShowOnErrors) {
    App app;
    app.ui()->show();  // toggle_shell_ui mira si la consola se ve
    EXPECT_TRUE(app.ui()->shell_dock->isHidden());
    app.ui()->menutoolshell->trigger();
    EXPECT_FALSE(app.ui()->shell_dock->isHidden());
    app.ui()->shell_btn->trigger();
    EXPECT_TRUE(app.ui()->shell_dock->isHidden());
    // Los mensajes van también a la consola; [WARNING] (no _NOTCL) la muestra.
    app.ui()->inform("[WARNING_NOTCL] aviso uno");
    EXPECT_TRUE(app.ui()->shell_dock->isHidden());
    EXPECT_TRUE(app.shell()->browser()->toPlainText().contains("[WARNING] aviso uno"));
    app.ui()->inform("[WARNING] aviso dos");
    EXPECT_FALSE(app.ui()->shell_dock->isHidden());
}
