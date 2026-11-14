#include "gui/BookmarkManager.h"

#include <QDateTime>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QTextBrowser>
#include <QTextStream>
#include <QUrl>
#include <QVBoxLayout>
#include <algorithm>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/widgets/FCButton.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCTable.h"

namespace flatcam::gui {

namespace {

/** `repr()` de un texto de Python (comillas simples salvo que tenga una simple y ninguna doble). */
QString pyRepr(const QString& s) {
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

int toInt(const QString& key) { return key.trimmed().toInt(); }

}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// App: install_bookmarks, on_bookmarks_manager, on_backup_site
// ---------------------------------------------------------------------------------------------------------------

BookmarkDict App::bookmarks() const {
    BookmarkDict out;
    const JsonValue& v = appDefaults()["global_bookmarks"];
    if (!v.isObject()) return out;
    for (const auto& [k, b] : v.members()) {
        const auto& it = b.items();
        out.push_back({QString::fromStdString(k),
                       {it.size() > 0 ? QString::fromStdString(it[0].text()) : QString(),
                        it.size() > 1 ? QString::fromStdString(it[1].text()) : QString()}});
    }
    return out;
}

void App::setBookmarks(const BookmarkDict& dict) {
    JsonValue obj = JsonValue::object();
    for (const auto& [k, b] : dict) {
        JsonValue pair = JsonValue::array();
        pair.push(JsonValue::string(b.first.toStdString()));
        pair.push(JsonValue::string(b.second.toStdString()));
        obj.set(k.toStdString(), pair);
    }
    prefs_->set("global_bookmarks", obj);
}

void App::openUrl(const QString& url) {
    if (urlOpener) urlOpener(url);
    else QDesktopServices::openUrl(QUrl(url));
}

void App::installBookmarks(std::optional<BookmarkDict> dict) {
    BookmarkDict bm = bookmarks();
    if (!dict) {
        // dict.update: las claves que ya están conservan su lugar (y se pisan), las nuevas van al final.
        for (const auto& [k, b] : BookmarkDict{{"1", {"FlatCAM", "http://flatcam.org"}}, {"2", {tr_("Backup Site"), ""}}}) {
            auto it = std::find_if(bm.begin(), bm.end(), [&k = k](const auto& e) { return e.first == k; });
            if (it != bm.end()) it->second = b;
            else bm.push_back({k, b});
        }
    } else {
        bm = *dict;
    }
    setBookmarks(bm);
    QMenu* menu = ui_->menuhelp_bookmarks;
    // Se quitan todas las acciones salvo la última (el administrador): también el separador.
    const QList<QAction*> acts = menu->actions();
    for (QAction* a : acts) {
        if (a == acts.back()) continue;
        menu->removeAction(a);
        if (a->parent() == menu) a->deleteLater();
    }
    const int limit = static_cast<int>(defNum("global_bookmarks_limit", 10));
    // Los primeros `limit` del diccionario, ordenados por el número de la clave.
    BookmarkDict sorted(bm.begin(), bm.begin() + std::min<std::size_t>(bm.size(), std::max(0, limit)));
    std::stable_sort(sorted.begin(), sorted.end(),
                     [](const auto& a, const auto& b) { return toInt(a.first) < toInt(b.first); });
    for (const auto& [k, b] : sorted) {
        auto* act = new QAction(menu);
        act->setText(b.first);
        act->setIcon(QIcon(":/images/link16.png"));
        const QString link = b.second;
        if (b.first == tr_("Backup Site") && link.isEmpty())
            connect(act, &QAction::triggered, this, [this] { onBackupSite(); });
        else
            connect(act, &QAction::triggered, this, [this, link] { openUrl(link); });
        menu->insertAction(ui_->menuhelp_bookmarks_manager, act);
    }
}

BookmarkManager* App::onBookmarksManager() {
    FCDetachableTab2* area = ui_->plot_tab_area;
    // Una sola a la vez.
    for (int i = 0; i < area->count(); ++i)
        if (area->tabText(i) == tr_("Bookmarks Manager")) return nullptr;
    auto* tab = new BookmarkManager(this, bookmarks());
    tab->setObjectName("bookmarks_tab");
    bookTab_ = tab;
    area->addTab(tab, tr_("Bookmarks Manager"));
    ui_->coords_toolbar->hide();
    ui_->delta_coords_toolbar->hide();
    area->setCurrentWidget(tab);
    return tab;
}

void App::onBackupSite() {
    QMessageBox box(ui_.get());
    box.setText(tr_("This entry will resolve to another website if:\n\n1. FlatCAM.org website is down\n2. Someone "
                    "forked FlatCAM project and wants to point\nto his own website\n\nIf you can't get any "
                    "informations about the application\nuse the YouTube channel link from the Help menu."));
    box.setWindowTitle(tr_("Alternative website"));
    box.setWindowIcon(QIcon(":/images/globe16.png"));
    box.setIcon(QMessageBox::Question);
    QPushButton* close = box.addButton(tr_("Close"), QMessageBox::YesRole);
    box.setDefaultButton(close);
    box.exec();
}

void App::setupBookmarks() {
    installBookmarks();
    connect(ui_->menuhelp_bookmarks_manager, &QAction::triggered, this, [this] { onBookmarksManager(); });
    // on_plot_area_tab_closed: al cerrar el administrador se arma el menú con el orden de la tabla.
    connect(ui_->plot_tab_area, &FCDetachableTab2::tabClosed, this, [this](const QString& name, int) {
        if (name != "bookmarks_tab" || !bookTab_) return;
        // Si falla (una fila arrastrada perdió su enlace) el original no sigue: la pestaña queda sin liberar y las
        // barras de coordenadas, ocultas.
        if (!bookTab_->rebuildActions()) return;
        bookTab_->deleteLater();
        ui_->coords_toolbar->setVisible(defBool("global_coordsbar_show"));
        ui_->delta_coords_toolbar->setVisible(defBool("global_delta_coordsbar_show"));
    });
}

// ---------------------------------------------------------------------------------------------------------------
// BookmarkManager
// ---------------------------------------------------------------------------------------------------------------

BookmarkManager::BookmarkManager(App* app, BookmarkDict storage) : app_(app), bm_(std::move(storage)) {
    auto* layout = new QVBoxLayout(this);
    auto* tableLay = new QHBoxLayout();
    layout->addLayout(tableLay);
    table_widget = new FCTable();
    table_widget->setDragDrop(true);  // protected_rows=[0, 1] no se usa en el original
    table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLay->addWidget(table_widget);
    table_widget->setColumnCount(3);
    table_widget->setColumnWidth(0, 20);
    table_widget->setHorizontalHeaderLabels({"#", tr_("Title"), tr_("Web Link")});
    table_widget->horizontalHeaderItem(0)->setToolTip(
        tr_("Index.\nThe rows in gray color will populate the Bookmarks menu.\n"
            "The number of gray colored rows is set in Preferences."));
    table_widget->horizontalHeaderItem(1)->setToolTip(
        tr_("Description of the link that is set as an menu action.\n"
            "Try to keep it short because it is installed as a menu item."));
    table_widget->horizontalHeaderItem(2)->setToolTip(tr_("Web Link. E.g: https://your_website.org "));

    auto* newLay = new QVBoxLayout();
    layout->addLayout(newLay);
    newLay->addWidget(new QLabel(QString("<b>%1</b>").arg(tr_("New Bookmark"))));
    auto* form = new QFormLayout();
    newLay->addLayout(form);
    title_entry = new FCEntry();
    form->addRow(new QLabel(QString("%1:").arg(tr_("Title"))), title_entry);
    link_entry = new FCEntry();
    link_entry->setText("http://");
    form->addRow(new QLabel(QString("%1:").arg(tr_("Web Link"))), link_entry);

    auto* buttons = new QHBoxLayout();
    layout->addLayout(buttons);
    add_entry_btn = new FCButton();
    add_entry_btn->setText(tr_("Add Entry"));
    remove_entry_btn = new FCButton();
    remove_entry_btn->setText(tr_("Remove Entry"));
    export_list_btn = new FCButton();
    export_list_btn->setText(tr_("Export List"));
    import_list_btn = new FCButton();
    import_list_btn->setText(tr_("Import List"));
    for (FCButton* b : {add_entry_btn, remove_entry_btn, export_list_btn, import_list_btn}) buttons->addWidget(b);

    connect(add_entry_btn, &QPushButton::clicked, this, [this] { addEntry(); });
    connect(remove_entry_btn, &QPushButton::clicked, this, [this] { removeEntry(); });
    connect(export_list_btn, &QPushButton::clicked, this, [this] { exportBookmarks(); });
    connect(import_list_btn, &QPushButton::clicked, this, [this] { importBookmarks(); });
    connect(title_entry, &QLineEdit::returnPressed, this, [this] { addEntry(); });
    connect(link_entry, &QLineEdit::returnPressed, this, [this] { addEntry(); });
    connect(table_widget, &FCTable::dragDropped, this, [this] { markRows(); });
    buildUi();
}

void BookmarkManager::buildUi() {
    // build_bm_ui
    table_widget->setRowCount(static_cast<int>(bm_.size()));
    BookmarkDict sorted = bm_;
    std::stable_sort(sorted.begin(), sorted.end(),
                     [](const auto& a, const auto& b) { return toInt(a.first) < toInt(b.first); });
    int row = 0;
    for (const auto& [k, b] : sorted) {
        table_widget->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        table_widget->setItem(row, 1, new QTableWidgetItem(b.first));
        auto* link = new QTextBrowser();
        link->setOpenExternalLinks(true);
        link->setFrameStyle(QFrame::NoFrame);
        link->document()->setDefaultStyleSheet("a{ text-decoration: none; }");
        link->setHtml(QString("<a href=%1>%1</a>").arg(b.second));
        table_widget->setCellWidget(row, 2, link);
        ++row;
    }
    table_widget->verticalHeader()->hide();
    QHeaderView* h = table_widget->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(2, QHeaderView::Stretch);
    markRows();
    app_->setBookmarks(bm_);
}

void BookmarkManager::markRows() {
    // mark_table_rows_for_actions: las filas que van al menú, en gris.
    const int limit = static_cast<int>(defNum("global_bookmarks_limit", 10));
    for (int r = 0; r < table_widget->rowCount(); ++r)
        if (QTableWidgetItem* it = table_widget->item(r, 0)) it->setBackground(QColor(r < limit ? "gray" : "white"));
}

bool BookmarkManager::addEntry(const QString& titleArg, const QString& linkArg, bool fromArgs, bool* indexError) {
    MainGUI* ui = app_->ui();
    const QString title = fromArgs ? titleArg : title_entry->text();
    if (title.isEmpty()) {
        ui->inform("[ERROR_NOTCL] " + tr_("Title entry is empty."));
        return false;
    }
    QString link = fromArgs ? linkArg : link_entry->text();
    if (link == "http://") {
        ui->inform("[ERROR_NOTCL] " + tr_("Web link entry is empty."));
        return false;
    }
    for (const auto& [k, b] : bm_) {
        if (title == b.first || link == b.second) {
            ui->inform("[ERROR_NOTCL] " + tr_("Either the Title or the Weblink already in the table."));
            return false;
        }
    }
    // link[-1] con un enlace vacío: IndexError (en la GUI no pasa nada más; corta la importación).
    if (link.isEmpty()) {
        if (indexError) *indexError = true;
        return false;
    }
    if (link.endsWith('/')) link.chop(1);
    bm_.push_back({QString::number(bm_.size() + 1), {title, link}});
    // En el menú solo si entra en el límite.
    if (static_cast<int>(bm_.size()) < static_cast<int>(defNum("global_bookmarks_limit", 10))) {
        auto* act = new QAction(ui->menuhelp_bookmarks);
        act->setText(title);
        act->setIcon(QIcon(":/images/link16.png"));
        connect(act, &QAction::triggered, app_, [app = app_, link] { app->openUrl(link); });
        ui->menuhelp_bookmarks->insertAction(ui->menuhelp_bookmarks_manager, act);
    }
    ui->inform("[success] " + tr_("Bookmark added."));
    buildUi();
    return true;
}

void BookmarkManager::removeEntry() {
    MainGUI* ui = app_->ui();
    for (const QModelIndex& idx : table_widget->selectionModel()->selectedRows()) {
        const QTableWidgetItem* it = table_widget->item(idx.row(), 1);
        const QString title = it ? it->text() : QString();
        if (title == "FlatCAM" || title == tr_("Backup Site")) {
            ui->inform("[WARNING_NOTCL] " + tr_("This bookmark can not be removed") + ".");
            buildUi();
            return;
        }
        for (auto e = bm_.begin(); e != bm_.end();) {
            if (e->second.first != title) {
                ++e;
                continue;
            }
            e = bm_.erase(e);
            for (QAction* a : ui->menuhelp_bookmarks->actions())
                if (a->text() == title) ui->menuhelp_bookmarks->removeAction(a);
        }
    }
    // Las claves se renumeran desde 1, en el orden del diccionario.
    int n = 0;
    for (auto& e : bm_) e.first = QString::number(++n);
    ui->inform("[success] " + tr_("Bookmark removed."));
    buildUi();
}

void BookmarkManager::exportBookmarks(const QString& filename) {
    MainGUI* ui = app_->ui();
    QString file = filename;
    if (file.isEmpty()) {
        // str(datetime.today()).rpartition('.')[0] sin ':' ni '-' y con '_' (vacío si no hay microsegundos).
        const QDateTime now = QDateTime::currentDateTime();
        const QString date = now.time().msec() == 0 ? QString() : now.toString("yyyyMMdd_HHmmss");
        file = QFileDialog::getSaveFileName(ui, tr_("Export Bookmarks"),
                                            QString("%1/%2_%3").arg(app_->lastFolder(), tr_("Bookmarks"), date),
                                            "Text File (*.TXT);;All Files (*.*)");
    }
    if (file.isEmpty()) {
        ui->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    QFile f(file);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        ui->inform("[WARNING] " + tr_("Permission denied, saving not possible.\n"
                                      "Most likely another app is holding the file open and not accessible."));
        return;
    }
    // Cada línea es la clave y la lista [título, enlace] escrita por str() de Python.
    QTextStream out(&f);
    for (const auto& [k, b] : bm_) out << k << ":[" << pyRepr(b.first) << ", " << pyRepr(b.second) << "]\n";
    ui->inform(QString("[success] %1: %2").arg(tr_("Exported bookmarks to"), file));
}

void BookmarkManager::importBookmarks(const QString& filename) {
    MainGUI* ui = app_->ui();
    const QString file = filename.isEmpty()
                             ? QFileDialog::getOpenFileName(ui, tr_("Import Bookmarks"), QString(),
                                                            "Text File (*.txt);;All Files (*.*)")
                             : filename;
    if (file.isEmpty()) {
        ui->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->inform("[ERROR_NOTCL] " + tr_("Could not load the file."));
        return;
    }
    // readlines: cada línea con su '\n'.
    const QString text = QString::fromUtf8(f.readAll());
    QStringList lines;
    int start = 0;
    for (int i = 0; i < text.size(); ++i)
        if (text[i] == '\n') {
            lines << text.mid(start, i - start + 1);
            start = i + 1;
        }
    if (start < text.size()) lines << text.mid(start);
    for (QString line : lines) {
        line.remove(' ');
        const int colon = line.indexOf(':');
        const QString title = colon < 0 ? line : line.left(colon);
        const QString link = colon < 0 ? QString() : line.mid(colon + 1);
        bool error = false;
        addEntry(title, link, true, &error);
        if (error) return;  // la excepción corta la importación sin aviso
    }
    ui->inform(QString("[success] %1: %2").arg(tr_("Imported Bookmarks from"), file));
}

bool BookmarkManager::rebuildActions() {
    // El orden de la tabla (con lo arrastrado) pasa a ser el de los marcadores.
    bm_.clear();
    for (int r = 0; r < table_widget->rowCount(); ++r) {
        const QTableWidgetItem* t = table_widget->item(r, 1);
        auto* w = qobject_cast<QTextBrowser*>(table_widget->cellWidget(r, 2));
        // Una fila arrastrada pierde su enlace (el widget no se copia) y el original falla con AttributeError.
        if (!w) return false;
        bm_.push_back({QString::number(r + 1), {t ? t->text() : QString(), w->toPlainText()}});
    }
    app_->installBookmarks(bm_);
    return true;
}

}  // namespace flatcam::gui
