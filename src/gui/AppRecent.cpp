// Archivos y proyectos recientes (port de `register_recent` y `setup_recent_items` de `app_Main.py`).
#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QStandardPaths>

#include "gui/App.h"
#include "gui/Translation.h"
#include "io/ProjectFile.h"

namespace flatcam::gui {

namespace {

constexpr std::size_t kRecentLimit = 10;  // global_recent_limit

std::vector<std::pair<QString, QString>> readList(const QString& path) {
    std::vector<std::pair<QString, QString>> out;
    JsonValue v;
    std::string err;
    if (!QFileInfo::exists(path) || !ProjectFile::read(path.toStdString(), v, err)) return out;
    for (const JsonValue& r : v.items())
        out.emplace_back(QString::fromStdString(r["kind"].text()), QString::fromStdString(r["filename"].text()));
    return out;
}

void writeList(const QString& path, const std::vector<std::pair<QString, QString>>& list) {
    // json.dump(self.recent, f, indent=2, sort_keys=True): [{"filename": ..., "kind": ...}, ...]
    JsonValue a = JsonValue::array();
    for (const auto& [kind, file] : list) {
        JsonValue r = JsonValue::object();
        r.set("filename", JsonValue::string(file.toStdString()));
        r.set("kind", JsonValue::string(kind.toStdString()));
        a.push(r);
    }
    std::string err;
    ProjectFile::write(path.toStdString(), a, false, 0, err);
}

QString iconOf(const QString& kind) {
    if (kind == "gerber") return ":/images/flatcam_icon16.png";
    if (kind == "excellon") return ":/images/drill16.png";
    if (kind == "geometry") return ":/images/geometry16.png";
    if (kind == "cncjob") return ":/images/cnc16.png";
    if (kind == "project") return ":/images/project16.png";
    return ":/images/notes16_1.png";
}

}  // namespace

QString App::dataPath() const {
    if (!dataPath_.isEmpty()) return dataPath_;
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

void App::setDataPath(const QString& path) {
    dataPath_ = path;
    setupRecentItems();
}

void App::registerRecent(const QString& kind, const QString& filename) {
    const QString dir = dataPath();
    QDir().mkpath(dir);
    auto recent = readList(dir + "/recent.json");
    auto projects = readList(dir + "/recent_projects.json");
    const std::pair<QString, QString> record{kind, filename};
    if (std::find(recent.begin(), recent.end(), record) != recent.end()) return;
    if (std::find(projects.begin(), projects.end(), record) != projects.end()) return;
    auto& list = kind == "project" ? projects : recent;
    list.insert(list.begin(), record);
    if (list.size() > kRecentLimit) list.pop_back();
    writeList(dir + "/recent.json", recent);
    writeList(dir + "/recent_projects.json", projects);
    setupRecentItems();
}

void App::setupRecentItems() {
    const QString dir = dataPath();
    ui_->recent->clear();
    ui_->recent_projects->clear();
    for (const auto& [kind, file] : readList(dir + "/recent_projects.json")) {
        if (kind != "project") continue;
        QAction* a = ui_->recent_projects->addAction(QIcon(iconOf(kind)), QFileInfo(file).fileName());
        connect(a, &QAction::triggered, this, [this, f = file] { openProject(f); });
    }
    ui_->recent_projects->addSeparator();
    QAction* clearProjects = ui_->recent_projects->addAction(QIcon(":/images/trash32.png"), tr_("Clear Recent projects"));
    connect(clearProjects, &QAction::triggered, this, [this, dir] {
        writeList(dir + "/recent_projects.json", {});
        setupRecentItems();
    });
    for (const auto& [kind, file] : readList(dir + "/recent.json")) {
        if (kind == "project") continue;
        QAction* a = ui_->recent->addAction(QIcon(iconOf(kind)), QFileInfo(file).fileName());
        connect(a, &QAction::triggered, this, [this, k = kind, f = file] {
            if (k == "gerber") openGerber(f);
            else if (k == "excellon") openExcellon(f);
            else ui_->inform("[WARNING_NOTCL] " + tr_("Unsupported file type") + ": " + k);
        });
    }
    ui_->recent->addSeparator();
    QAction* clearFiles = ui_->recent->addAction(QIcon(":/images/trash32.png"), tr_("Clear Recent files"));
    connect(clearFiles, &QAction::triggered, this, [this, dir] {
        writeList(dir + "/recent.json", {});
        setupRecentItems();
    });
}

}  // namespace flatcam::gui
