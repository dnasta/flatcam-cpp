#include "gui/shell/TclCommand.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>

namespace flatcam::gui {

namespace {

std::vector<std::pair<QString, QString>> pairs(const QJsonArray& a) {
    std::vector<std::pair<QString, QString>> out;
    for (const QJsonValue& v : a) out.push_back({v.toArray()[0].toString(), v.toArray()[1].toString()});
    return out;
}

QStringList strings(const QJsonArray& a) {
    QStringList out;
    for (const QJsonValue& v : a) out << v.toString();
    return out;
}

/** El tipo como lo escribe `%s` de Python (`<class 'float'>`). */
QString pyTypeRepr(const QString& type) { return QString("<class '%1'>").arg(type); }

}  // namespace

QString pyReprString(const QString& s) {
    const QChar q = s.contains('\'') && !s.contains('"') ? QChar('"') : QChar('\'');
    QString out = q;
    for (const QChar c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == q) out += QString("\\") + c;
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else out += c;
    }
    return out + q;
}

std::optional<double> pyFloatFromString(const QString& text, QString* error) {
    // float(): espacios alrededor, signo, dígitos con `_` entre ellos, punto, exponente; inf, infinity y nan.
    const QString t = text.trimmed();
    static const QRegularExpression num(
        R"(^[+-]?(?:\d(?:_?\d)*(?:\.(?:\d(?:_?\d)*)?)?|\.\d(?:_?\d)*)(?:[eE][+-]?\d(?:_?\d)*)?$)");
    static const QRegularExpression special("^([+-]?)(inf|infinity|nan)$", QRegularExpression::CaseInsensitiveOption);
    if (const auto m = special.match(t); m.hasMatch()) {
        if (m.captured(2).toLower() == "nan") return std::numeric_limits<double>::quiet_NaN();
        return m.captured(1) == "-" ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
    }
    if (num.match(t).hasMatch()) {
        const std::string c = QString(t).remove('_').toStdString();
        return std::strtod(c.c_str(), nullptr);  // con desborde da inf, como Python
    }
    if (error) *error = QString("could not convert string to float: %1").arg(pyReprString(text));
    return std::nullopt;
}

std::optional<long long> pyIntFromString(const QString& text, QString* error) {
    const QString t = text.trimmed();
    static const QRegularExpression num(R"(^[+-]?\d(?:_?\d)*$)");
    if (num.match(t).hasMatch()) {
        bool ok = false;
        const long long v = QString(t).remove('_').toLongLong(&ok);
        if (ok) return v;
    }
    if (error) *error = QString("invalid literal for int() with base 10: %1").arg(pyReprString(text));
    return std::nullopt;
}

bool TclArgs::hasValue(const std::string& key) const {
    const auto it = named.find(key);
    return it != named.end() && !std::holds_alternative<std::monostate>(it->second);
}

QString TclArgs::str(const std::string& key) const {
    const auto it = named.find(key);
    if (it == named.end()) return {};
    if (const auto* s = std::get_if<QString>(&it->second)) return *s;
    if (const auto* d = std::get_if<double>(&it->second)) return QString::number(*d, 'g', 17);
    if (const auto* i = std::get_if<long long>(&it->second)) return QString::number(*i);
    return "None";
}

double TclArgs::num(const std::string& key) const {
    const auto it = named.find(key);
    if (it == named.end()) return 0.0;
    if (const auto* d = std::get_if<double>(&it->second)) return *d;
    if (const auto* i = std::get_if<long long>(&it->second)) return static_cast<double>(*i);
    if (const auto* s = std::get_if<QString>(&it->second)) return s->toDouble();
    return 0.0;
}

namespace {
const QJsonObject& commandsJson() {
    static const QJsonObject o = [] {
        QFile f(":/tcl_commands.json");
        return f.open(QIODevice::ReadOnly) ? QJsonDocument::fromJson(f.readAll()).object() : QJsonObject();
    }();
    return o;
}
}  // namespace

const QStringList& TclCommandMeta::defaultsOrder() {
    static const QStringList order = strings(commandsJson()["defaults_order"].toArray());
    return order;
}

const std::vector<TclCommandMeta>& TclCommandMeta::all() {
    static const std::vector<TclCommandMeta> list = [] {
        std::vector<TclCommandMeta> out;
        for (const QJsonValue& v : commandsJson()["commands"].toArray()) {
            const QJsonObject o = v.toObject();
            TclCommandMeta m;
            m.className = o["class"].toString();
            m.signaled = o["base"].toString() == "TclCommandSignaled";
            m.registered = o["registered"].toBool();
            m.aliases = strings(o["aliases"].toArray());
            m.description = o["description"].toString();
            m.argNames = pairs(o["arg_names"].toArray());
            m.optionTypes = pairs(o["option_types"].toArray());
            m.required = strings(o["required"].toArray());
            const QJsonObject h = o["help"].toObject();
            m.helpMain = h["main"].toString();
            m.helpArgs = pairs(h["args"].toArray());
            m.examples = strings(h["examples"].toArray());
            out.push_back(std::move(m));
        }
        return out;
    }();
    return list;
}

std::optional<TclCommandMeta> TclCommandMeta::forClass(const QString& className) {
    for (const TclCommandMeta& m : all())
        if (m.className == className) return m;
    return std::nullopt;
}

TclCommand::TclCommand(App* app, const QString& className)
    : app_(app), meta_(TclCommandMeta::forClass(className).value_or(TclCommandMeta{})) {}

QString TclCommand::decoratedHelp() const {
    auto typeOf = [this](const QString& key, QString& symbol) -> std::optional<QString> {
        symbol.clear();
        for (const auto& [n, t] : meta_.argNames)
            if (n == key) return t;
        for (const auto& [n, t] : meta_.optionTypes)
            if (n == key) {
                symbol = "-";
                return t;
            }
        return std::nullopt;
    };
    auto argument = [&](const QString& key, const QString& text, bool inCommand) {
        QString symbol;
        const auto t = typeOf(key, symbol);
        const QString type = t.value_or("?");
        QString name;
        bool isArg = false;
        for (const auto& [n, ty] : meta_.argNames) isArg = isArg || n == key;
        if (isArg) name = key;
        else name = symbol + key + " <" + type + ">";
        const bool required = meta_.required.contains(key);
        if (inCommand) return required ? name : "[" + name + "]";
        if (required) return "\t" + symbol + key + " <" + type + ">: " + text;
        return "\t[" + symbol + key + " <" + type + ">: " + text + "]";
    };
    QStringList out{meta_.helpMain};
    for (const QString& alias : meta_.aliases) {
        QStringList parts;
        for (const auto& [k, t] : meta_.helpArgs) parts << argument(k, t, true);
        out << "> " + alias + " " + parts.join(' ');
    }
    for (const auto& [k, t] : meta_.helpArgs) out << argument(k, t, false);
    if (meta_.signaled) out << "\t[-timeout <int>: Max wait for job timeout before error.]";
    for (const QString& e : meta_.examples) out << "> " + e;
    return out.join('\n');
}

QStringList TclCommand::parseArguments(const QStringList& args,
                                       std::vector<std::pair<QString, std::optional<QString>>>& options) {
    // Un diccionario de Python: la misma clave otra vez cambia el valor sin moverla de lugar.
    auto set = [&options](const QString& key, std::optional<QString> value) {
        for (auto& [k, v] : options)
            if (k == key) {
                v = std::move(value);
                return;
            }
        options.push_back({key, std::move(value)});
    };
    static const QRegularExpression re("^-([a-zA-Z].*)$", QRegularExpression::DotMatchesEverythingOption);
    QStringList arguments;
    std::optional<QString> optionName;
    for (const QString& a : args) {
        const auto m = re.match(a);
        if (m.hasMatch()) {
            if (optionName) set(*optionName, std::nullopt);
            optionName = m.captured(1);
            continue;
        }
        if (!optionName) arguments << a;
        else {
            set(*optionName, a);
            optionName.reset();
        }
    }
    if (optionName) set(*optionName, std::nullopt);
    return arguments;
}

TclArgs TclCommand::checkArgs(const QStringList& args) const {
    std::vector<std::pair<QString, std::optional<QString>>> options;
    const QStringList arguments = parseArguments(args, options);
    TclArgs out;
    auto convert = [](const QString& type, const QString& text, QString& error) -> std::optional<TclValue> {
        if (type == "float") {
            if (auto v = pyFloatFromString(text, &error)) return TclValue(*v);
            return std::nullopt;
        }
        if (type == "int") {
            if (auto v = pyIntFromString(text, &error)) return TclValue(*v);
            return std::nullopt;
        }
        return TclValue(text);
    };
    for (int i = 0; i < arguments.size(); ++i) {
        if (i < static_cast<int>(meta_.argNames.size())) {
            const auto& [key, type] = meta_.argNames[static_cast<std::size_t>(i)];
            QString error;
            const auto v = convert(type, arguments[i], error);
            if (!v)
                raiseTclError(QString("Cannot cast named argument '%1' to type %2  with exception '%3'.")
                                  .arg(key, pyTypeRepr(type), error));
            out.named[key.toStdString()] = *v;
        } else {
            out.unnamed << arguments[i];
        }
    }
    for (const auto& [key, value] : options) {
        const auto it = std::find_if(meta_.optionTypes.begin(), meta_.optionTypes.end(),
                                     [&key = key](const auto& p) { return p.first == key; });
        if (it == meta_.optionTypes.end() && key != "timeout") raiseTclError("Unknown parameter: " + key);
        if (key == "timeout") {
            QString error;
            const auto v = value ? pyIntFromString(*value, &error) : std::nullopt;
            if (!v) raiseTclError(value ? error : "int() argument must be a string, a bytes-like object or a real number, "
                                                  "not 'NoneType'");
            out.named["timeout"] = *v;
            continue;
        }
        if (!value) {
            out.named[key.toStdString()] = std::monostate{};  // las opciones sin valor quedan en None
            continue;
        }
        QString error;
        const auto v = convert(it->second, *value, error);
        if (!v)
            raiseTclError(QString("Cannot cast argument '-%1' to type '%2' with exception '%3'.")
                              .arg(key, pyTypeRepr(it->second), error));
        out.named[key.toStdString()] = *v;
    }
    for (const QString& key : meta_.required)
        if (!out.has(key.toStdString())) raiseTclError(QString("Missing required argument '%1'.").arg(key));
    return out;
}

std::optional<QString> TclCommand::run(const QStringList& args) {
    originalArgs_ = args;
    return execute(checkArgs(args));
}

}  // namespace flatcam::gui
