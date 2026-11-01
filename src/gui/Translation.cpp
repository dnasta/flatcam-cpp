#include "gui/Translation.h"

#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>

namespace flatcam::gui {

namespace {

const QHash<QString, QString>& table() {
    static const QHash<QString, QString> t = [] {
        QHash<QString, QString> h;
        QFile f(":/es.json");
        if (f.open(QIODevice::ReadOnly)) {
            const QJsonObject o = QJsonDocument::fromJson(f.readAll()).object();
            for (auto it = o.begin(); it != o.end(); ++it) h.insert(it.key(), it.value().toString());
        }
        return h;
    }();
    return t;
}

}  // namespace

QString tr_(const QString& english) { return table().value(english, english); }

QString tr_(const char* english) { return tr_(QString::fromUtf8(english)); }

}  // namespace flatcam::gui
