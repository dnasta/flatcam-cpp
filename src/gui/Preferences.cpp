#include "gui/Preferences.h"

#include <cctype>
#include <cmath>
#include <cstring>

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QMetaMethod>
#include <QPushButton>
#include <QSettings>
#include <QUrl>

#include "gui/MainGUI.h"
#include "gui/ProjectIO.h"
#include "gui/Translation.h"
#include "gui/tools/PropertiesTool.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCPrefWidgets.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"
#include "io/ProjectFile.h"

namespace flatcam::gui {

namespace {

// ---------------------------------------------------------------------------------------------------------------
// eval() de Python para los campos NumericalEvalEntry / NumericalEvalTupleEntry: números (enteros o reales), + - * /
// % y paréntesis, tuplas y listas, None/True/False. Conserva el tipo (int o float) para guardarlo igual que Python.
// ---------------------------------------------------------------------------------------------------------------
struct PyVal {
    enum class Kind { Num, Seq, None, Bool } kind{Kind::None};
    double v{0};
    bool isInt{false};
    bool b{false};
    std::vector<PyVal> items;
};

PyVal mk(PyVal::Kind k) {
    PyVal v;
    v.kind = k;
    return v;
}

class PyEval {
public:
    explicit PyEval(std::string s) : s_(std::move(s)) {}

    std::optional<PyVal> parse() {
        auto v = top();
        skip();
        if (!v || pos_ != s_.size()) return std::nullopt;
        return v;
    }

private:
    void skip() {
        while (pos_ < s_.size() && (s_[pos_] == ' ' || s_[pos_] == '\t')) ++pos_;
    }
    bool peek(char c) {
        skip();
        return pos_ < s_.size() && s_[pos_] == c;
    }
    bool eat(char c) {
        if (!peek(c)) return false;
        ++pos_;
        return true;
    }
    bool word(const char* w) {
        skip();
        const std::size_t n = std::strlen(w);
        if (s_.compare(pos_, n, w) != 0) return false;
        pos_ += n;
        return true;
    }
    /// expr (',' expr)* [','] -> una tupla si hay alguna coma.
    std::optional<PyVal> top(char closing = 0) {
        if (closing && peek(closing)) return mk(PyVal::Kind::Seq);  // () o []
        auto first = expr();
        if (!first) return std::nullopt;
        if (!peek(',')) return first;
        PyVal seq = mk(PyVal::Kind::Seq);
        seq.items.push_back(*first);
        while (eat(',')) {
            skip();
            if (pos_ >= s_.size() || (closing && peek(closing))) break;  // coma final
            auto x = expr();
            if (!x) return std::nullopt;
            seq.items.push_back(*x);
        }
        return seq;
    }
    static bool arith(const PyVal& a, const PyVal& b) {
        return (a.kind == PyVal::Kind::Num || a.kind == PyVal::Kind::Bool) &&
               (b.kind == PyVal::Kind::Num || b.kind == PyVal::Kind::Bool);
    }
    static PyVal num(const PyVal& a) {
        if (a.kind != PyVal::Kind::Bool) return a;
        PyVal r = mk(PyVal::Kind::Num);
        r.v = a.b ? 1 : 0;
        r.isInt = true;
        return r;
    }
    std::optional<PyVal> expr() {
        auto v = term();
        while (v) {
            char op = 0;
            if (eat('+')) op = '+';
            else if (eat('-')) op = '-';
            else break;
            auto r = term();
            if (!r) return std::nullopt;
            if (v->kind == PyVal::Kind::Seq && r->kind == PyVal::Kind::Seq && op == '+') {  // tuplas: se concatenan
                for (auto& x : r->items) v->items.push_back(x);
                continue;
            }
            if (!arith(*v, *r)) return std::nullopt;
            PyVal a = num(*v), b = num(*r);
            a.v = op == '+' ? a.v + b.v : a.v - b.v;
            a.isInt = a.isInt && b.isInt;
            v = a;
        }
        return v;
    }
    std::optional<PyVal> term() {
        auto v = factor();
        while (v) {
            char op = 0;
            if (eat('*')) op = '*';
            else if (eat('/')) op = '/';
            else if (eat('%')) op = '%';
            else break;
            auto r = factor();
            if (!r || !arith(*v, *r)) return std::nullopt;
            PyVal a = num(*v), b = num(*r);
            if (op == '*') {
                a.v *= b.v;
                a.isInt = a.isInt && b.isInt;
            } else if (op == '/') {
                if (b.v == 0) return std::nullopt;  // ZeroDivisionError
                a.v /= b.v;
                a.isInt = false;
            } else {
                if (b.v == 0) return std::nullopt;
                const double m = std::fmod(a.v, b.v);
                a.v = (m != 0 && ((m < 0) != (b.v < 0))) ? m + b.v : m;  // con el signo del divisor
                a.isInt = a.isInt && b.isInt;
            }
            v = a;
        }
        return v;
    }
    std::optional<PyVal> factor() {
        if (eat('+')) {
            auto v = factor();
            if (!v || !arith(*v, *v)) return std::nullopt;
            return num(*v);
        }
        if (eat('-')) {
            auto v = factor();
            if (!v || !arith(*v, *v)) return std::nullopt;
            PyVal a = num(*v);
            a.v = -a.v;
            return a;
        }
        if (eat('(')) {
            auto v = top(')');
            if (!v || !eat(')')) return std::nullopt;
            return v;
        }
        if (eat('[')) {
            auto v = top(']');
            if (!v || !eat(']')) return std::nullopt;
            if (v->kind != PyVal::Kind::Seq) {  // [x] es una lista de un elemento
                PyVal seq = mk(PyVal::Kind::Seq);
                seq.items.push_back(*v);
                return seq;
            }
            return v;
        }
        if (word("None")) return mk(PyVal::Kind::None);
        if (word("True") || word("False")) {
            PyVal b = mk(PyVal::Kind::Bool);
            b.b = s_[pos_ - 1] == 'e' && s_[pos_ - 2] == 'u';
            return b;
        }
        skip();
        const std::size_t start = pos_;
        bool dot = false;
        while (pos_ < s_.size() && (std::isdigit(static_cast<unsigned char>(s_[pos_])) || s_[pos_] == '.')) {
            if (s_[pos_] == '.') {
                if (dot) return std::nullopt;
                dot = true;
            }
            ++pos_;
        }
        if (pos_ == start || (pos_ - start == 1 && dot)) return std::nullopt;
        const std::string t = s_.substr(start, pos_ - start);
        if (!dot && t.size() > 1 && t[0] == '0' && t.find_first_not_of('0') != std::string::npos)
            return std::nullopt;  // 012 es un error de sintaxis en Python 3
        PyVal r = mk(PyVal::Kind::Num);
        r.v = std::stod(t);
        r.isInt = !dot;
        return r;
    }

    std::string s_;
    std::size_t pos_{0};
};

JsonValue toJson(const PyVal& v) {
    switch (v.kind) {
        case PyVal::Kind::None: return JsonValue();
        case PyVal::Kind::Bool: return JsonValue::boolean(v.b);
        case PyVal::Kind::Num:
            return v.isInt ? JsonValue::integer(static_cast<long long>(std::llround(v.v))) : JsonValue::real(v.v);
        case PyVal::Kind::Seq: {
            JsonValue a = JsonValue::array();
            for (const PyVal& x : v.items) a.push(toJson(x));
            return a;
        }
    }
    return JsonValue();
}

/// `eval(raw)`: el valor, o nulo si no se puede evaluar (el original lo registra y devuelve `None`).
JsonValue pyEval(const QString& raw) {
    const auto v = PyEval(raw.trimmed().toStdString()).parse();
    return v ? toJson(*v) : JsonValue();
}

const Preferences* gInstance = nullptr;  ///< La fuente de appDefaults().

QVariant toVariant(const JsonValue& v) {
    switch (v.type()) {
        case JsonValue::Type::Bool: return QVariant(v.toBool());
        case JsonValue::Type::Number: return v.isInteger() ? QVariant(static_cast<qlonglong>(std::llround(v.toDouble())))
                                                           : QVariant(v.toDouble());
        case JsonValue::Type::String: return QVariant(QString::fromStdString(v.text()));
        default: return QVariant();
    }
}

JsonValue fromVariant(const QVariant& v) {
    switch (v.typeId()) {
        case QMetaType::Bool: return JsonValue::boolean(v.toBool());
        case QMetaType::Int:
        case QMetaType::LongLong:
        case QMetaType::UInt:
        case QMetaType::ULongLong: return JsonValue::integer(v.toLongLong());
        case QMetaType::Double:
        case QMetaType::Float: return JsonValue::real(v.toDouble());
        case QMetaType::QString: return JsonValue::string(v.toString().toStdString());
        default: return JsonValue();
    }
}

/// `float(val)` de Python sobre un valor de las opciones.
std::optional<double> pyFloat(const JsonValue& v) {
    if (v.isNumber()) return v.toDouble();
    if (v.isBool()) return v.toBool() ? 1.0 : 0.0;
    if (v.isString()) {
        bool ok = false;
        const double d = QString::fromStdString(v.text()).trimmed().toDouble(&ok);
        if (ok) return d;
    }
    return std::nullopt;
}

/// `int(val)` de Python (trunca los reales; un texto tiene que ser un entero).
std::optional<long long> pyInt(const JsonValue& v) {
    if (v.isNumber()) return static_cast<long long>(std::trunc(v.toDouble()));
    if (v.isBool()) return v.toBool() ? 1 : 0;
    if (v.isString()) {
        bool ok = false;
        const long long i = QString::fromStdString(v.text()).trimmed().toLongLong(&ok);
        if (ok) return i;
    }
    return std::nullopt;
}

/// `bool(val)` de Python.
bool pyTruth(const JsonValue& v) {
    switch (v.type()) {
        case JsonValue::Type::Null: return false;
        case JsonValue::Type::Bool: return v.toBool();
        case JsonValue::Type::Number: return v.toDouble() != 0;
        case JsonValue::Type::String: return !v.text().empty();
        case JsonValue::Type::Array: return !v.items().empty();
        case JsonValue::Type::Object: return !v.members().empty();
    }
    return false;
}

/// `a == b` de Python entre valores de JSON (1 == 1.0 y True == 1 son verdaderos).
bool pyEqual(const JsonValue& a, const JsonValue& b) {
    const bool an = a.isNumber() || a.isBool(), bn = b.isNumber() || b.isBool();
    if (an && bn) return a.toDouble() == b.toDouble();
    if (a.type() != b.type()) return false;
    switch (a.type()) {
        case JsonValue::Type::Null: return true;
        case JsonValue::Type::String: return a.text() == b.text();
        case JsonValue::Type::Array:
            if (a.items().size() != b.items().size()) return false;
            for (std::size_t i = 0; i < a.items().size(); ++i)
                if (!pyEqual(a.items()[i], b.items()[i])) return false;
            return true;
        case JsonValue::Type::Object:
            if (a.members().size() != b.members().size()) return false;
            for (const auto& [k, v] : a.members()) {
                const JsonValue* o = b.find(k);
                if (!o || !pyEqual(v, *o)) return false;
            }
            return true;
        default: return false;
    }
}

}  // namespace

JsonValue pyEvalLiteral(const QString& text) { return pyEval(text); }

const JsonValue& appDefaults() { return gInstance ? gInstance->defaults() : Preferences::factoryDefaults(); }

double defNum(const std::string& key, double def) { return pyFloat(appDefaults()[key]).value_or(def); }

int defInt(const std::string& key, int def) {
    const auto v = pyInt(appDefaults()[key]);
    return v ? static_cast<int>(*v) : def;
}

bool defBool(const std::string& key) { return pyTruth(appDefaults()[key]); }

QString defStr(const std::string& key) { return PropertiesTool::pyStr(appDefaults()[key]); }

QVariant defVariant(const std::string& key) { return toVariant(appDefaults()[key]); }

std::optional<PyNumber> defPy(const std::string& key) {
    const JsonValue& v = appDefaults()[key];
    JsonValue n = v;
    if (v.isString()) n = pyEval(QString::fromStdString(v.text()));  // un texto con un número ("0.1")
    if (!n.isNumber()) return std::nullopt;
    if (n.isInteger()) return PyNumber(static_cast<int>(std::llround(n.toDouble())));
    return PyNumber(n.toDouble());
}

std::optional<std::array<double, 2>> defPair(const std::string& key) {
    const JsonValue& v = appDefaults()[key];
    const JsonValue e = v.isString() ? pyEval(QString::fromStdString(v.text())) : v;
    if (!e.isArray() || e.items().size() != 2 || !e.items()[0].isNumber() || !e.items()[1].isNumber()) return std::nullopt;
    return std::array<double, 2>{e.items()[0].toDouble(), e.items()[1].toDouble()};
}

std::vector<double> defDias(const std::string& key) {
    const JsonValue& v = appDefaults()[key];
    std::vector<double> out;
    if (auto d = pyFloat(v)) return {*d};  // float(...)
    if (v.isString()) {  // [float(eval(dia)) for dia in v.split(",") if dia != '']
        for (const QString& part : QString::fromStdString(v.text()).split(',')) {
            if (part.isEmpty()) continue;
            const JsonValue e = pyEval(part);
            if (!e.isNumber()) return {};
            out.push_back(e.toDouble());
        }
        return out;
    }
    for (const JsonValue& x : v.items())  // una lista (después de guardar las preferencias)
        if (x.isNumber()) out.push_back(x.toDouble());
    return out;
}

PreferencesTab::PreferencesTab(QWidget* parent) : PreferencesTabUIBase(parent) { setupUi(); }

Preferences::Preferences(MainGUI* ui, QObject* parent) : QObject(parent), ui_(ui) {
    defaults_ = factoryDefaults();
    currentDefaults_ = factoryDefaults();
    gInstance = this;
    connect(ui_->plot_tab_area, &FCDetachableTab2::tabClosed, this, [this](const QString& name, int) {
        if (name == "preferences_tab") onTabClosed();
    });
}

Preferences::~Preferences() {
    if (gInstance == this) gInstance = nullptr;
}

const char* Preferences::version() { return "2024.4"; }

const JsonValue& Preferences::factoryDefaults() {
    static const JsonValue f = [] {
        JsonValue v = ProjectIO::factoryDefaults();
        v.set("version", JsonValue::string(version()));  // FlatCAMDefaults.__init__: factory_defaults['version']
        return v;
    }();
    return f;
}

const std::map<std::string, std::pair<std::string, std::string>>& Preferences::fields() {
    static const std::map<std::string, std::pair<std::string, std::string>> f = [] {
        std::map<std::string, std::pair<std::string, std::string>> m;
        QFile file(":/preferences_fields.json");
        if (file.open(QIODevice::ReadOnly)) {
            const QJsonObject o = QJsonDocument::fromJson(file.readAll()).object();
            for (auto it = o.begin(); it != o.end(); ++it) {
                const QJsonObject e = it.value().toObject();
                m[it.key().toStdString()] = {e["member"].toString().toStdString(), e["py"].toString().toStdString()};
            }
        }
        return m;
    }();
    return f;
}

void Preferences::set(const std::string& key, JsonValue v) {
    // LoudDict.__setitem__: si la clave ya estaba con un valor igual (== de Python) no hace nada; si no, avisa
    // (on_defaults_dict_change escribe el campo).
    if (const JsonValue* old = defaults_.find(key); old && pyEqual(*old, v)) return;
    defaults_.set(key, std::move(v));
    writeFormField(key);
}

QString Preferences::settingsFile() const { return dataPath_ + "/settings.ini"; }

QString Preferences::theme() const {
    QSettings s(settingsFile(), QSettings::IniFormat);
    return s.value("theme", "white").toString();
}

void Preferences::saveFactoryDefaults(const QString& filename) {
    QFile f(filename);
    if (f.exists()) {
        JsonValue old;
        std::string err;
        if (f.open(QIODevice::ReadOnly)) {
            ProjectFile::parse(f.readAll().toStdString(), old, err);
            f.close();
        }
        if (old.isObject() && !old.members().empty()) {
            const JsonValue& ver = old["version"];
            if (PropertiesTool::pyStr(ver) == version()) return;  // str(f_defaults['version']) == str(version)
            f.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup);
            f.remove();
        }
    }
    QFile out(filename);
    if (out.exists()) {
        out.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
        out.remove();
    }
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
    out.write(QByteArray::fromStdString(ProjectFile::dump(factoryDefaults(), true)));
    out.close();
    out.setPermissions(QFile::ReadOwner | QFile::ReadGroup | QFile::ReadOther);  // de solo lectura
}

void Preferences::setup(const QString& dataPath) {
    dataPath_ = dataPath;
    QDir().mkpath(dataPath);
    const QString current = dataPath + "/current_defaults.FlatConfig";
    if (!QFile::exists(current)) {
        QFile f(current);
        if (f.open(QIODevice::WriteOnly)) f.write("{}");
    }
    saveFactoryDefaults(dataPath + "/factory_defaults.FlatConfig");
    defaults_ = factoryDefaults();
    currentDefaults_ = factoryDefaults();
    load(current);
    set("global_theme", JsonValue::string(theme().toStdString()));  // app_Main: defaults['global_theme'] = theme()
    if (tab_) {
        resetAlphaControls();
        initColorPickers();
        writeForm();
    }
}

bool Preferences::load(const QString& filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        ui_->inform("[ERROR] " + tr_("Could not load the file."));
        defaults_.set("global_toolbar_view", JsonValue::integer(511));
        return false;
    }
    JsonValue loaded;
    std::string err;
    if (!ProjectFile::parse(f.readAll().toStdString(), loaded, err)) {
        defaults_.set("global_toolbar_view", JsonValue::integer(511));
        ui_->inform("[ERROR] " + tr_("Failed to parse defaults file."));
        return false;
    }
    if (loaded.isNull()) return true;
    const JsonValue* ver = loaded.find("version");
    const bool old = !ver || !(ver->isString() && ver->text() == version());
    const bool nonEmpty = loaded.isObject() ? !loaded.members().empty() : pyTruth(loaded);
    if (old && nonEmpty) {
        // Mientras la aplicación es beta (la 2024.4 lo es) las preferencias de otra versión se descartan.
        resetToFactoryDefaults();
        oldDefaultsFound_ = true;
    } else {
        oldDefaultsFound_ = false;
        for (const auto& [k, v] : loaded.members()) set(k, v);
        for (const auto& [k, v] : defaults_.members()) currentDefaults_.set(k, v);
    }
    return true;
}

bool Preferences::write(const QString& filename) const {
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    f.write(QByteArray::fromStdString(ProjectFile::dump(defaults_, true)));
    return true;
}

void Preferences::resetToFactoryDefaults() {
    for (const auto& [k, v] : factoryDefaults().members()) set(k, v);
    for (const auto& [k, v] : factoryDefaults().members()) currentDefaults_.set(k, v);
    oldDefaultsFound_ = false;
}

PreferencesTab* Preferences::tab() {
    if (!tab_) {
        tab_ = new PreferencesTab();
        tab_->setObjectName("preferences_tab");
        members_ = tab_->membersByName();
        // Como el arranque del original: las señales de los grupos, show_preferences_gui (__init_color_pickers) y
        // defaults_write_form().
        resetAlphaControls();
        connectGroups();
        initColorPickers();
        writeForm();
    }
    return tab_;
}

void Preferences::writeForm(const JsonValue* source) {
    const JsonValue& src = source ? *source : defaults_;
    for (const auto& [k, v] : src.members()) writeFormField(k, source);
}

void Preferences::writeFormField(const std::string& key, const JsonValue* source) {
    if (!tab_) return;
    const auto it = fields().find(key);
    if (it == fields().end()) return;
    const JsonValue* pv = (source ? *source : defaults_).find(key);
    if (!pv) return;
    const JsonValue& v = *pv;
    const auto m = members_.find(it->second.first);
    if (m == members_.end()) return;
    QObject* w = m->second;
    const std::string& py = it->second.second;
    if (py == "FCDoubleSpinner") {
        if (auto d = pyFloat(v)) static_cast<FCDoubleSpinner*>(w)->setValue(*d);
    } else if (py == "FCSpinner") {
        if (auto i = pyInt(v)) static_cast<FCSpinner*>(w)->setValue(static_cast<int>(*i));
    } else if (py == "FCSliderWithSpinner") {
        if (auto i = pyInt(v)) static_cast<FCSliderWithSpinner*>(w)->setValue(static_cast<int>(*i));
    } else if (py == "FCCheckBox") {
        if (auto i = pyInt(v)) static_cast<FCCheckBox*>(w)->setValue(*i != 0);  // setChecked(int(val))
    } else if (py == "RadioSet") {
        static_cast<RadioSet*>(w)->setValue(toVariant(v));
    } else if (py == "FCColorEntry") {
        static_cast<FCColorEntry*>(w)->setValue(PropertiesTool::pyStr(v));
    } else if (py == "FCTextArea") {
        static_cast<FCTextArea*>(w)->setValue(PropertiesTool::pyStr(v));
    } else if (py == "FCComboBox") {
        static_cast<FCComboBox*>(w)->setValue(PropertiesTool::pyStr(v));
    } else if (py == "FCComboBox2") {
        if (auto i = pyInt(v)) static_cast<FCComboBox2*>(w)->setIndexValue(static_cast<int>(*i));
    } else if (py == "NumericalEvalTupleEntry") {
        static_cast<FCEntry*>(w)->setText(PropertiesTool::pyStr(v));  // EvalEntry.set_value: str(val)
    } else if (py == "NumericalEvalEntry" || py == "FCEntry") {
        // FCEntry.set_value: un float con los decimales del campo, None vacío, lo demás con str().
        auto* e = static_cast<FCEntry*>(w);
        if (v.isNumber() && !v.isInteger()) e->setValue(v.toDouble());
        else if (v.isNull()) e->setText("");
        else e->setText(PropertiesTool::pyStr(v));
    }
}

std::optional<JsonValue> Preferences::formValue(const std::string& key) {
    tab();
    const auto it = fields().find(key);
    if (it == fields().end()) return std::nullopt;
    const auto m = members_.find(it->second.first);
    if (m == members_.end()) return std::nullopt;
    QObject* w = m->second;
    const std::string& py = it->second.second;
    if (py == "FCDoubleSpinner") return JsonValue::real(static_cast<FCDoubleSpinner*>(w)->value());
    if (py == "FCSpinner") return JsonValue::integer(static_cast<FCSpinner*>(w)->value());
    if (py == "FCSliderWithSpinner") return JsonValue::integer(static_cast<FCSliderWithSpinner*>(w)->value());
    if (py == "FCCheckBox") return JsonValue::boolean(static_cast<FCCheckBox*>(w)->isChecked());
    if (py == "RadioSet") return fromVariant(static_cast<RadioSet*>(w)->value());  // None si no hay ninguna
    if (py == "FCColorEntry") return JsonValue::string(static_cast<FCColorEntry*>(w)->value().toStdString());
    if (py == "FCTextArea") return JsonValue::string(static_cast<FCTextArea*>(w)->value().toStdString());
    if (py == "FCComboBox") return JsonValue::string(static_cast<FCComboBox*>(w)->currentText().toStdString());
    if (py == "FCComboBox2") return JsonValue::integer(static_cast<FCComboBox2*>(w)->currentIndex());
    if (py == "NumericalEvalTupleEntry") return pyEval(static_cast<FCEntry*>(w)->text());
    if (py == "NumericalEvalEntry") return pyEval(static_cast<FCEntry*>(w)->text().replace(',', '.'));
    if (py == "FCEntry") return JsonValue::string(static_cast<FCEntry*>(w)->text().toStdString());
    return std::nullopt;
}

void Preferences::readForm() {
    if (!tab_) return;  // sin la pestaña armada, los campos son las opciones mismas
    for (const auto& [k, f] : fields())
        if (auto v = formValue(k)) defaults_.set(k, *v);
}

// ---------------------------------------------------------------------------------------------------------------
// La pestaña
// ---------------------------------------------------------------------------------------------------------------

int Preferences::tabIndex() const {
    FCDetachableTab2* area = ui_->plot_tab_area;
    for (int i = 0; i < area->count(); ++i)
        if (area->tabText(i) == tr_("Preferences")) return i;
    return -1;
}

bool Preferences::isOpen() const { return tabIndex() >= 0; }

void Preferences::open() {
    FCDetachableTab2* area = ui_->plot_tab_area;
    PreferencesTab* t = tab();
    if (area->indexOf(t) < 0) area->addTab(t, tr_("Preferences"));
    ui_->coords_toolbar->hide();
    ui_->delta_coords_toolbar->hide();
    area->setCurrentWidget(t);
    ui_->pref_status_label->setStyleSheet("\n\t\t\t\t\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t\t\t\t\t{\n"
                                          "\t\t\t\t\t\t\t\t\t\t\t\t\tcolor: black;\n\t\t\t\t\t\t\t\t\t\t\t\t\t"
                                          "background-color: lightseagreen;\n\t\t\t\t\t\t\t\t\t\t\t\t}\n"
                                          "\t\t\t\t\t\t\t\t\t\t\t\t");
    connectEdited(true);
}

void Preferences::toggle() {
    const int idx = tabIndex();
    if (idx >= 0) {
        ui_->plot_tab_area->removeTab(idx);
        ui_->pref_status_label->setStyleSheet("");
    } else {
        open();
    }
}

void Preferences::connectEdited(bool on) {
    // Como el original: textEdited, modificationChanged, toggled, valueChanged y currentIndexChanged de todos los
    // objetos de las pestañas internas marcan las preferencias como editadas.
    if (!tab_ || on == edConnected_) return;
    edConnected_ = on;
    static const char* const kSignals[] = {"textEdited(QString)", "modificationChanged(bool)", "toggled(bool)",
                                           "valueChanged(int)", "valueChanged(double)", "currentIndexChanged(int)"};
    const QMetaMethod slot = metaObject()->method(metaObject()->indexOfSlot("onPreferencesEdited()"));
    for (int i = 0; i < tab_->pref_tab_area->count(); ++i) {
        for (QObject* o : tab_->pref_tab_area->widget(i)->findChildren<QObject*>()) {
            for (const char* sig : kSignals) {
                const int idx = o->metaObject()->indexOfSignal(sig);
                if (idx < 0) continue;
                const QMetaMethod m = o->metaObject()->method(idx);
                if (on) QObject::connect(o, m, this, slot, Qt::UniqueConnection);
                else QObject::disconnect(o, m, this, slot);
            }
        }
    }
}

void Preferences::onPreferencesEdited() {
    if (changedFlag_) return;
    ui_->inform("[WARNING_NOTCL] " + tr_("Preferences edited but not saved."));
    const int idx = tabIndex();
    if (idx >= 0) {
        oldColor_ = ui_->plot_tab_area->tabBar()->tabTextColor(idx);
        ui_->plot_tab_area->tabBar()->setTabTextColor(idx, QColor("red"));
    }
    if (tab_) {
        tab_->pref_apply_button->setStyleSheet("QPushButton {color: red;}");
        tab_->pref_apply_button->setIcon(QIcon(":/images/apply_red32.png"));
    }
    changedFlag_ = true;
}

bool Preferences::ask(const QString& title, const QString& text, const QString& icon, const QString& yes,
                      const QString& no, bool defaultYes) {
    if (autoAnswer_) return *autoAnswer_;
    QMessageBox box;
    box.setText(text);
    box.setWindowTitle(title);
    box.setWindowIcon(QIcon(icon));
    box.setIcon(QMessageBox::Question);
    QPushButton* bYes = box.addButton(yes, QMessageBox::YesRole);
    QPushButton* bNo = box.addButton(no, QMessageBox::NoRole);
    box.setDefaultButton(defaultYes ? bYes : bNo);
    box.exec();
    return box.clickedButton() == bYes;
}

void Preferences::onSave(bool saveToFile) {
    changedFlag_ = false;
    if (const int idx = tabIndex(); idx >= 0 && oldColor_.isValid())
        ui_->plot_tab_area->tabBar()->setTabTextColor(idx, oldColor_);
    PreferencesTab* t = tab();
    t->pref_apply_button->setStyleSheet("");
    t->pref_apply_button->setIcon(QIcon(":/images/apply32.png"));
    ui_->inform(tr_("Preferences applied."));
    // Como el original, "Aplicar" no lee el formulario: current_defaults.update(defaults).
    for (const auto& [k, v] : defaults_.members()) currentDefaults_.set(k, v);

    bool shouldRestart = false;
    const QString themeNew = PropertiesTool::pyStr(fromVariant(t->general_gui_group_theme_radio->value()));
    const JsonValue ge = defaults_["global_graphic_engine"];
    const JsonValue geNew = fromVariant(t->general_app_group_ge_radio->value());
    if (themeNew != theme() || PropertiesTool::pyStr(ge) != PropertiesTool::pyStr(geNew)) {
        const bool yes = ask(tr_("Application will restart"), tr_("Are you sure you want to continue?"),
                             ":/images/warning.png", tr_("Yes"), tr_("Cancel"), true);
        if (themeNew != theme()) {
            if (yes) {
                QSettings s(settingsFile(), QSettings::IniFormat);
                s.setValue("theme", themeNew);
                shouldRestart = true;
            } else {
                t->general_gui_group_theme_radio->setValue(theme());
            }
        } else {
            if (yes) {
                defaults_.set("global_graphic_engine", geNew);
                shouldRestart = true;
            } else {
                t->general_app_group_ge_radio->setValue(toVariant(ge));
            }
        }
    }
    if (saveToFile || shouldRestart) {
        Q_EMIT optionsToProject();  // on_options_app2project: defaults_read_form() y options.update(defaults)
        saveDefaults(false);
        load(dataPath_ + "/current_defaults.FlatConfig");
    }
    {
        QSettings s(settingsFile(), QSettings::IniFormat);
        s.setValue("notebook_font_size", t->general_app_set_group_notebook_font_size_spinner->value());
        s.setValue("axis_font_size", t->general_app_set_group_axis_font_size_spinner->value());
        s.setValue("textbox_font_size", t->general_app_set_group_textbox_font_size_spinner->value());
        s.setValue("hud_font_size", t->general_app_set_group_hud_font_size_spinner->value());
        s.setValue("machinist", t->general_app_set_group_machinist_cb->isChecked() ? 1 : 0);
    }
    if (saveToFile && !closingTab_) {  // si se está cerrando la pestaña, ya la cierra quien llamó
        const int idx = tabIndex();
        if (idx >= 0) {
            if (oldColor_.isValid()) ui_->plot_tab_area->tabBar()->setTabTextColor(idx, oldColor_);
            ui_->plot_tab_area->closeTab(idx);
        }
    }
    Q_EMIT saved();
    if (shouldRestart && onRestart) onRestart();
}

void Preferences::onCloseButton() {
    changedFlag_ = false;
    ignoreTabCloseEvent_ = true;
    ui_->pref_status_label->setStyleSheet("");
    writeForm(&currentDefaults_);
    for (const auto& [k, v] : currentDefaults_.members()) defaults_.set(k, v);
    const int idx = tabIndex();
    if (idx >= 0) {
        if (oldColor_.isValid()) ui_->plot_tab_area->tabBar()->setTabTextColor(idx, oldColor_);
        ui_->plot_tab_area->closeTab(idx);
    }
    ui_->inform(tr_("Preferences closed without saving."));
    ignoreTabCloseEvent_ = false;
}

void Preferences::onTabClosed() {
    if (ignoreTabCloseEvent_) return;
    ui_->pref_status_label->setStyleSheet("");
    connectEdited(false);
    if (changedFlag_) {
        closingTab_ = true;
        if (ask(tr_("Save Preferences"), tr_("One or more values are changed.\nDo you want to save the Preferences?"),
                ":/images/save_as.png", tr_("Yes"), tr_("No"), true)) {
            onSave(true);
            ui_->inform("[success] " + tr_("Preferences saved."));
        } else {
            changedFlag_ = false;
            ui_->inform("");
        }
        closingTab_ = false;
    }
}

void Preferences::onRestoreDefaults() {
    resetToFactoryDefaults();
    onPreferencesEdited();
    ui_->inform("[success] " + tr_("Preferences default values are restored."));
}

void Preferences::openFolder() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(dataPath_));
    ui_->inform("[success] " + tr_("FlatCAM Preferences Folder opened."));
}

void Preferences::clearGuiSettings(bool forced) {
    {
        QSettings s(settingsFile(), QSettings::IniFormat);
        s.setValue("theme", "white");
    }
    if (forced || ask(tr_("Clear GUI Settings"), tr_("Are you sure you want to delete the GUI Settings? \n"),
                      ":/images/trash32.png", tr_("Yes"), tr_("No"), false)) {
        QSettings s(settingsFile(), QSettings::IniFormat);
        s.clear();
    }
}

void Preferences::saveToolbarView() {
    int st = 0;
    if (ui_->toolbarfile->isVisible()) st += 1;
    if (ui_->toolbaredit->isVisible()) st += 2;
    if (ui_->toolbarview->isVisible()) st += 4;
    if (ui_->toolbartools->isVisible()) st += 8;
    if (ui_->exc_edit_toolbar->isVisible()) st += 16;
    if (ui_->geo_edit_toolbar->isVisible()) st += 32;
    if (ui_->grb_edit_toolbar->isVisible()) st += 64;
    if (ui_->status_toolbar->isVisible()) st += 128;
    if (ui_->toolbarshell->isVisible()) st += 256;
    defaults_.set("global_toolbar_view", JsonValue::integer(st));
}

bool Preferences::saveDefaults(bool silent) {
    saveToolbarView();
    const QString filename = dataPath_ + "/current_defaults.FlatConfig";
    if (!write(filename)) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Failed to write defaults to file.") + " " + filename);
        return false;
    }
    if (!silent) ui_->inform("[success] " + tr_("Preferences saved."));
    return true;
}

}  // namespace flatcam::gui
