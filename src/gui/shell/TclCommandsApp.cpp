// Comandos de la consola Tcl sobre la aplicación: help, version, get_sys / set_sys / list_sys / save_sys, get_names,
// set_active, clear, new, quit_flatcam, get_path / set_path, plot_all, plot_objects (tclCommands/ del original).
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <algorithm>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/shell/FCShell.h"
#include "gui/shell/TclCommand.h"
#include "gui/shell/TclCommands.h"

namespace flatcam::gui {

namespace {

/** Un valor de las opciones de la aplicación como lo devuelve tkinter a Tcl (bool 1/0, listas, None). */
QString tclValue(const JsonValue& v) {
    if (v.isNull()) return "None";
    if (v.isBool()) return v.toBool() ? "1" : "0";
    if (v.isString()) return QString::fromStdString(v.text());
    if (v.isNumber()) return QString::fromStdString(v.text());
    if (v.isArray()) {
        QStringList parts;
        for (const JsonValue& e : v.items()) {
            QString s = tclValue(e);
            if (s.isEmpty() || s.contains(' ')) s = "{" + s + "}";
            parts << s;
        }
        return parts.join(' ');
    }
    QStringList parts;  // un diccionario: clave valor ...
    for (const auto& [k, e] : v.members()) parts << QString::fromStdString(k) << tclValue(e);
    return parts.join(' ');
}

/** `str()` de una lista de textos de Python. */
QString pyListRepr(const QStringList& items) {
    QStringList r;
    for (const QString& s : items) r << pyReprString(s);
    return "[" + r.join(", ") + "]";
}

/** `bool(eval(texto))` de un "True" / "False" / número. */
bool pyTruth(const QString& text) {
    const QString t = text.trimmed();
    if (t == "True") return true;
    if (t == "False" || t == "None") return false;
    bool ok = false;
    const double d = t.toDouble(&ok);
    if (ok) return d != 0;
    throw TclError(QString("name '%1' is not defined").arg(t));
}

class Help : public TclCommand {
public:
    explicit Help(App* a) : TclCommand(a, "TclCommandHelp") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        FCShell* sh = app_->shell();
        if (args.has("name")) {
            const QString name = args.str("name");
            const auto it = sh->commands().find(name);
            if (it == sh->commands().end()) return "Unknown command: " + name;
            sh->appendOutput(it->second->decoratedHelp() + "\n\n");
            return std::nullopt;
        }
        // La lista: nombre en rojo, espacios hasta el más largo y la descripción, de a cuatro.
        int maxLen = 0;
        for (const auto& [n, c] : sh->commands()) maxLen = std::max(maxLen, static_cast<int>(n.size()));
        QStringList lines;
        int cnt = 0;
        for (const auto& [n, c] : sh->commands()) {  // el mapa ya está ordenado como sorted()
            lines << QString("> <span style=\" font-weight: bold; color: red;\" >%1</span>%2&nbsp;&nbsp;<span>%3</span>")
                         .arg(n, QString("&nbsp;").repeated(maxLen - static_cast<int>(n.size())), c->meta().description);
            if (cnt == 3) {
                cnt = 0;
                lines << " ";
            } else {
                ++cnt;
            }
        }
        QString text = QString("%1\n").arg(tr_("Available commands:")) + lines.join("<br>");
        text += QString("<br><br>%1<br>%2<br><br>").arg(tr_("Type help <command_name> for usage."),
                                                         tr_("Example: help open_gerber"));
        sh->appendRaw(text);
        return std::nullopt;
    }
};

class Version : public TclCommand {
public:
    explicit Version(App* a) : TclCommand(a, "TclCommandVersion") {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        // version_check consulta flatcam.org (con el número de serie y las estadísticas). Desvío: el port no se
        // conecta; avisa lo mismo que el original sin conexión.
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Failed checking for latest version. Could not connect."));
        return std::nullopt;
    }
};

class GetNames : public TclCommand {
public:
    explicit GetNames(App* a) : TclCommand(a, "TclCommandGetNames") {}

protected:
    std::optional<QString> execute(const TclArgs&) override { return app_->collection()->names().join('\n'); }
};

class GetSys : public TclCommand {
public:
    explicit GetSys(App* a) : TclCommand(a, "TclCommandGetSys") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        if (const JsonValue* v = appDefaults().find(name.toStdString())) return tclValue(*v);
        return QString("The keyword: %1 does not exist as a parameter").arg(name);
    }
};

class SetSys : public TclCommand {
public:
    explicit SetSys(App* a) : TclCommand(a, "TclCommandSetSys") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString param = args.str("name");
        const QString value = args.str("value");
        if (!appDefaults().find(param.toStdString())) raiseTclError(QString("No such system parameter \"%1\".").arg(param));
        // tcl2py: algunas palabras pasan a None, True, False o a mayúsculas; el resto queda como texto.
        static const std::map<QString, JsonValue> tcl2py = {
            {"None", JsonValue()}, {"none", JsonValue()},
            {"false", JsonValue::boolean(false)}, {"False", JsonValue::boolean(false)},
            {"true", JsonValue::boolean(true)}, {"True", JsonValue::boolean(true)},
            {"l", JsonValue::string("L")}, {"L", JsonValue::string("L")}, {"T", JsonValue::string("T")},
            {"t", JsonValue::string("T")}, {"in", JsonValue::string("IN")}, {"IN", JsonValue::string("IN")},
            {"mm", JsonValue::string("MM")}, {"MM", JsonValue::string("MM")}};
        const auto it = tcl2py.find(value);
        app_->preferences()->set(param.toStdString(), it != tcl2py.end() ? it->second : JsonValue::string(value.toStdString()));
        return std::nullopt;
    }
};

class ListSys : public TclCommand {
public:
    explicit ListSys(App* a) : TclCommand(a, "TclCommandListSys") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        QStringList keys;
        const QString prefix = args.has("selection") ? args.str("selection") : QString();
        // En el orden de app.defaults (el de defaults.py; lo que no está ahí, al final).
        QStringList order;
        for (const QString& k : TclCommandMeta::defaultsOrder())
            if (appDefaults().find(k.toStdString())) order << k;
        for (const auto& [k, v] : appDefaults().members())
            if (!order.contains(QString::fromStdString(k))) order << QString::fromStdString(k);
        for (const QString& key : order)
            if (key.startsWith(prefix)) keys << key;
        return pyListRepr(keys);
    }
};

class SaveSys : public TclCommand {
public:
    explicit SaveSys(App* a) : TclCommand(a, "TclCommandSaveSys") {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        // save_defaults(args): el diccionario de argumentos (vacío) va como `silent`, que es falso.
        app_->preferences()->saveDefaults(false);
        return std::nullopt;
    }
};

class SetActive : public TclCommand {
public:
    explicit SetActive(App* a) : TclCommand(a, "TclCommandSetActive") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        FlatCAMObj* obj = app_->collection()->byName(args.str("name"));
        if (!obj) return QString("Command failed: 'NoneType' object has no attribute 'item'");
        app_->setActive(obj, true);
        return std::nullopt;
    }
};

class ClearShell : public TclCommand {
public:
    explicit ClearShell(App* a) : TclCommand(a, "TclCommandClearShell") {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        app_->ui()->inform("Tcl Shell Editor cleared ...");
        app_->shell()->browser()->clearLog();
        return std::nullopt;
    }
};

class New : public TclCommand {
public:
    explicit New(App* a) : TclCommand(a, "TclCommandNew") {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        app_->onFileNew();
        return std::nullopt;
    }
};

class Quit : public TclCommand {
public:
    explicit Quit(App* a) : TclCommand(a, "TclCommandQuit") {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        // quit_application: cierra los editores, guarda las preferencias y sale (sin preguntar por el proyecto).
        if (app_->editorActive()) app_->editorToObject(false, true);
        app_->preferences()->saveDefaults(true);
        QApplication::quit();
        return std::nullopt;
    }
};

class GetPath : public TclCommand {
public:
    explicit GetPath(App* a) : TclCommand(a, "TclCommandGetPath") {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        app_->shell()->appendOutput("Current default Tcl Shell path is: ");
        return defStr("global_tcl_path");
    }
};

class SetPath : public TclCommand {
public:
    explicit SetPath(App* a) : TclCommand(a, "TclCommandSetPath") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        if (!args.has("path")) return QString("Failed. The Tcl command set_path was used but no path argument was provided.");
        const QString path = args.str("path").replace('\\', '/');
        const QFileInfo fi(path);
        if (!fi.isDir()) {
            if (fi.isFile()) {
                app_->shellMessage(QString("[ERROR] The provided path: %1, is a path to file and not a directory as "
                                           "expected.").arg(path));
                return QString("Failed. The Tcl command set_path was used but it was not a directory.");
            }
            app_->shellMessage(QString("[ERROR] The provided path: %1, do not exist. Check for typos.").arg(path));
            return QString("Failed. The Tcl command set_path was used but it does not exist.");
        }
        app_->shell()->execCommand("cd " + path, false);
        app_->preferences()->set("global_tcl_path", JsonValue::string(path.toStdString()));
        app_->shellMessage("[success] Relative path set to: " + path);
        return std::nullopt;
    }
};

class PlotAll : public TclCommand {
public:
    explicit PlotAll(App* a) : TclCommand(a, "TclCommandPlotAll") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        // use_thread no cambia nada en el port; plot_status: bool(eval(...)) (sin valor, True).
        if (args.hasValue("use_thread")) {
            QString p = args.str("use_thread");
            if (!p.isEmpty()) p[0] = p[0].toUpper();
            pyTruth(p);
        }
        bool status = true;
        if (args.hasValue("plot_status")) {
            QString p = args.str("plot_status");
            if (!p.isEmpty()) p[0] = p[0].toUpper();
            status = pyTruth(p);
        }
        for (FlatCAMObj* o : app_->collection()->objects()) o->setPlotEnabled(status);
        app_->plotAll();
        return std::nullopt;
    }
};

class PlotObjects : public TclCommand {
public:
    explicit PlotObjects(App* a) : TclCommand(a, "TclCommandPlotObjects") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const bool status = args.hasValue("plot_status") ? pyTruth(args.str("plot_status")) : true;
        std::vector<FlatCAMObj*> objs;
        for (const QString& n : args.str("names").split(',')) {
            if (n.isEmpty()) continue;
            FlatCAMObj* o = app_->collection()->byName(n.trimmed());
            if (!o) raiseTclError("'NoneType' object has no attribute 'options'");
            o->setPlotEnabled(status);
            objs.push_back(o);
        }
        for (FlatCAMObj* o : objs) {
            app_->ui()->plotCanvas()->setLayer(o->id(), o->plotData());
            app_->ui()->plotCanvas()->setLayerVisible(o->id(), o->plotEnabled());
        }
        return std::nullopt;
    }
};

}  // namespace

void addAppTclCommands(App* app, std::vector<std::unique_ptr<TclCommand>>& out) {
    out.push_back(std::make_unique<Help>(app));
    out.push_back(std::make_unique<Version>(app));
    out.push_back(std::make_unique<GetNames>(app));
    out.push_back(std::make_unique<GetSys>(app));
    out.push_back(std::make_unique<SetSys>(app));
    out.push_back(std::make_unique<ListSys>(app));
    out.push_back(std::make_unique<SaveSys>(app));
    out.push_back(std::make_unique<SetActive>(app));
    out.push_back(std::make_unique<ClearShell>(app));
    out.push_back(std::make_unique<New>(app));
    out.push_back(std::make_unique<Quit>(app));
    out.push_back(std::make_unique<GetPath>(app));
    out.push_back(std::make_unique<SetPath>(app));
    out.push_back(std::make_unique<PlotAll>(app));
    out.push_back(std::make_unique<PlotObjects>(app));
}

}  // namespace flatcam::gui
