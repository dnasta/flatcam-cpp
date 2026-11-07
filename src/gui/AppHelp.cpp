// Menú Ayuda: on_about (AboutDialog), on_howto (HowtoDialog), los enlaces y on_shortcut_list (ShortcutsTab).
#include <QDesktopServices>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>
#include <geos_c.h>
#include <lzma.h>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/generated/AboutDialogUIBase.h"
#include "gui/generated/HowtoDialogUIBase.h"

namespace flatcam::gui {

namespace {

// Enlaces de App (app_Main.py).
const char* kManualUrl = "http://flatcam.org/manual/index.html";
const char* kVideoUrl = "https://www.youtube.com/playlist?list=PLVvP2SYRpx-AQgNlfoxw93tXUXon7G94_";
const char* kGerberSpecUrl =
    "https://www.ucamco.com/files/downloads/file/81/The_Gerber_File_Format_specification.pdf?7ac957791daba2cdf4c2c913f67a43da";
const char* kExcellonSpecUrl = "https://www.ucamco.com/files/downloads/file/305/the_xnc_file_format_specification.pdf";
const char* kBugReportUrl = "https://bitbucket.org/jpcgt/flatcam/issues?status=new&status=open";

/// El botón "Cerrar" de un diálogo generado (en el original es una variable local, no un atributo).
void connectClose(QDialog* d) {
    const auto buttons = d->findChildren<QPushButton*>();
    if (!buttons.isEmpty()) QObject::connect(buttons.back(), &QPushButton::clicked, d, &QDialog::accept);
}

}  // namespace

std::unique_ptr<QDialog> App::aboutDialog() {
    auto d = std::make_unique<AboutDialogUIBase>(ui_.get());
    d->setupUi();
    connectClose(d.get());
    // Desvío: "Componentes" lista las bibliotecas del port (C++), no las de Python.
    for (QLabel* l : d->components_tab->findChildren<QLabel*>()) {
        if (!l->text().startsWith("<table>")) continue;
        l->setText(QString("<table><tr><td>Qt</td><td>%1</td></tr><tr><td>GEOS</td><td>%2</td></tr>"
                           "<tr><td>liblzma</td><td>%3</td></tr></table>")
                       .arg(qVersion(), GEOSversion(), lzma_version_string()));
    }
    return d;
}

std::unique_ptr<QDialog> App::howtoDialog() {
    auto d = std::make_unique<HowtoDialogUIBase>(ui_.get());
    d->setupUi();
    connectClose(d.get());
    return d;
}

QWidget* App::shortcutsTab() {
    if (shortcutsTab_) return shortcutsTab_;
    // ShortcutsTab (MainGUI.py): el título y las dos columnas, con los textos de resources/shortcuts.json
    // (tools/gui_dump/extract_shortcuts.py, sacados del original ya traducidos).
    QFile f(":/shortcuts.json");
    QJsonObject texts;
    if (f.open(QIODevice::ReadOnly)) texts = QJsonDocument::fromJson(f.readAll()).object();
    auto* tab = new QWidget(ui_.get());
    auto* lay = new QVBoxLayout();
    lay->setContentsMargins(2, 2, 2, 2);
    tab->setLayout(lay);
    auto* hlay = new QHBoxLayout();
    auto* title = new QTextEdit(texts.value("title").toString());
    title->setTextInteractionFlags(Qt::NoTextInteraction);
    title->setFrameStyle(QFrame::NoFrame);
    title->setMaximumHeight(30);
    QFont font = title->font();
    font.setPointSize(12);
    title->setFont(font);
    lay->addWidget(title);
    lay->addLayout(hlay);
    for (const char* key : {"app", "editor"}) {
        auto* t = new QTextEdit();
        t->setTextInteractionFlags(Qt::NoTextInteraction);
        t->setText(texts.value(key).toString());
        t->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        hlay->addWidget(t);
    }
    shortcutsTab_ = tab;
    return tab;
}

void App::onShortcutList() {
    QWidget* tab = shortcutsTab();
    ui_->plot_tab_area->addTab(tab, tr_("Key Shortcut List"));
    ui_->coords_toolbar->hide();
    ui_->delta_coords_toolbar->hide();
    ui_->plot_tab_area->setCurrentWidget(tab);
}

void App::setupHelpMenu() {
    connect(ui_->menuhelp_about, &QAction::triggered, this, [this] { aboutDialog()->exec(); });
    connect(ui_->menuhelp_readme, &QAction::triggered, this, [this] { howtoDialog()->exec(); });
    auto link = [this](QAction* a, const char* url) {
        connect(a, &QAction::triggered, this, [url] { QDesktopServices::openUrl(QUrl(url)); });
    };
    link(ui_->menuhelp_manual, kManualUrl);
    link(ui_->menuhelp_report_bug, kBugReportUrl);
    link(ui_->menuhelp_exc_spec, kExcellonSpecUrl);
    link(ui_->menuhelp_gerber_spec, kGerberSpecUrl);
    link(ui_->menuhelp_videohelp, kVideoUrl);
    connect(ui_->menuhelp_shortcut_list, &QAction::triggered, this, [this] { onShortcutList(); });
}

void App::openHelpLink(int which) {
    const char* urls[] = {kManualUrl, kVideoUrl};
    QDesktopServices::openUrl(QUrl(urls[which]));
}

}  // namespace flatcam::gui
