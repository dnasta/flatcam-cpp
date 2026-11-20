#include "gui/Translation.h"

#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QRegularExpression>
#include <QLocale>
#include <QSettings>

#include <algorithm>
#include <map>
#include <vector>

namespace flatcam::gui {

namespace {

struct Table {
    QString code;
    QHash<QString, QString> msg, ui;
    // Mensajes propios del port con %1, %2... adentro (los arma el núcleo con un número o un nombre).
    std::vector<std::pair<QRegularExpression, QString>> patterns;
};

// Código -> nombre, de los archivos de :/i18n/ (leídos una vez).
const std::map<QString, QString>& names() {
    static const std::map<QString, QString> n = [] {
        std::map<QString, QString> m;
        QFile file(":/i18n/languages.json");  // {código: nombre}, de build_translations.py
        if (file.open(QIODevice::ReadOnly)) {
            const QJsonObject o = QJsonDocument::fromJson(file.readAll()).object();
            for (auto it = o.begin(); it != o.end(); ++it) m[it.key()] = it.value().toString();
        }
        return m;
    }();
    return n;
}

Table& current() {
    static Table t;
    return t;
}

bool load(const QString& code) {
    QFile f(":/i18n/" + code + ".json");
    if (!f.open(QIODevice::ReadOnly)) return false;
    const QJsonObject o = QJsonDocument::fromJson(f.readAll()).object();
    Table t;
    t.code = code;
    const QJsonObject msg = o.value("msg").toObject(), ui = o.value("ui").toObject();
    t.msg.reserve(msg.size());
    for (auto it = msg.begin(); it != msg.end(); ++it) t.msg.insert(it.key(), it.value().toString());
    t.ui.reserve(ui.size());
    for (auto it = ui.begin(); it != ui.end(); ++it) t.ui.insert(it.key(), it.value().toString());
    for (const QJsonValue& k : o.value("patterns").toArray()) {
        const QString key = k.toString();
        QStringList parts;
        for (const QString& part : key.split(QRegularExpression(R"(%\d)"))) parts << QRegularExpression::escape(part);
        const QString re = parts.join("(.*)");
        t.patterns.push_back({QRegularExpression("^" + re + "$", QRegularExpression::DotMatchesEverythingOption),
                              t.msg.value(key, key)});
    }
    current() = std::move(t);
    return true;
}

const Table& table() {
    if (current().code.isEmpty()) load(systemLanguage());
    return current();
}

}  // namespace

QString tr_(const QString& english) {
    const Table& t = table();
    const auto it = t.msg.constFind(english);
    if (it != t.msg.constEnd()) return *it;
    for (const auto& [re, tr] : t.patterns) {
        const QRegularExpressionMatch m = re.match(english);
        if (!m.hasMatch()) continue;
        QString r = tr;
        for (int i = 1; i <= m.lastCapturedIndex(); ++i) r = r.arg(m.captured(i));
        return r;
    }
    return english;
}

QString tr_(const char* english) { return tr_(QString::fromUtf8(english)); }

QString trUi_(const QString& english) { return table().ui.value(english, english); }

QStringList availableLanguages() {
    std::vector<std::pair<QString, QString>> v;
    for (const auto& [code, name] : names()) v.push_back({name, code});
    std::sort(v.begin(), v.end());  // sorted(get_languages_names()): por el nombre
    QStringList r;
    for (const auto& [name, code] : v) r << code;
    return r;
}

QString languageName(const QString& code) {
    const auto it = names().find(code);
    return it == names().end() ? QString() : it->second;
}

QString languageCode(const QString& name) {
    for (const auto& [code, n] : names())
        if (n.compare(name, Qt::CaseInsensitive) == 0) return code;
    return {};
}

QString systemLanguage() {
    const QString loc = QLocale::system().name();  // es_AR, pt_BR, en_US...
    if (names().count(loc)) return loc;
    const QString lang = loc.section('_', 0, 0);
    if (names().count(lang)) return lang;
    for (const auto& [code, n] : names())  // pt_PT -> pt_BR
        if (code.section('_', 0, 0) == lang) return code;
    return "en";
}

QString language() { return table().code; }

bool setLanguage(const QString& code) {
    if (current().code == code) return true;
    return load(code);
}

QString startupLanguage(const QString& settingsFile) {
    const QString env = qEnvironmentVariable("FLATCAM_LANGUAGE");
    if (!env.isEmpty() && names().count(env)) return env;
    const QString saved = QSettings(settingsFile, QSettings::IniFormat).value("language").toString();
    if (names().count(saved)) return saved;
    return systemLanguage();
}

}  // namespace flatcam::gui
