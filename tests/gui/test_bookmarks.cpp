// Los marcadores de Ayuda (Bookmark.py, install_bookmarks), contra el original
// (tests/tools/oracle_bookmarks.py -> tests/data/bookmarks/oracle.json).
#include <gtest/gtest.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QTemporaryDir>
#include <QTextBrowser>

#include "gui/App.h"
#include "gui/BookmarkManager.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCTable.h"

/** Para que GoogleTest muestre los QString como texto. */
inline void PrintTo(const QString& s, std::ostream* os) { *os << '"' << s.toStdString() << '"'; }

using namespace flatcam;
using namespace flatcam::gui;

namespace {

const QString kDir = QString(FLATCAM_TEST_DATA_DIR) + "/bookmarks";

QJsonObject oracle() {
    QFile f(kDir + "/oracle.json");
    EXPECT_TRUE(f.open(QIODevice::ReadOnly));
    return QJsonDocument::fromJson(f.readAll()).object();
}

QJsonArray dictJson(const BookmarkDict& d) {
    QJsonArray a;
    for (const auto& [k, b] : d) a.append(QJsonArray{k, QJsonArray{b.first, b.second}});
    return a;
}

/** Compara el estado con el paso del original. */
void expectStep(App& app, BookmarkManager* mgr, const QJsonObject& s, QStringList& msgs, const QString& exportDir) {
    SCOPED_TRACE(s["step"].toString().toStdString());
    QJsonArray menu;
    for (QAction* a : app.ui()->menuhelp_bookmarks->actions())
        menu.append(QJsonObject{{"text", a->text()}, {"separator", a->isSeparator()},
                                {"manager", a == app.ui()->menuhelp_bookmarks_manager}});
    EXPECT_EQ(menu, s["menu"].toArray());
    EXPECT_EQ(dictJson(app.bookmarks()), s["dict"].toArray());
    QStringList theirs;
    for (const QJsonValue& m : s["msgs"].toArray()) theirs << m.toString();
    // El oráculo guarda las rutas como <DATA> (tests/data/bookmarks).
    QStringList mine = msgs;
    if (!exportDir.isEmpty()) mine.replaceInStrings(exportDir, "<DATA>");
    mine.replaceInStrings(kDir, "<DATA>");
    EXPECT_EQ(mine, theirs);
    msgs.clear();
    if (!mgr || !s.contains("table")) return;
    EXPECT_EQ(dictJson(mgr->bookmarks()), s["bm"].toArray());
    QJsonArray rows;
    FCTable* t = mgr->table_widget;
    for (int r = 0; r < t->rowCount(); ++r) {
        auto* w = qobject_cast<QTextBrowser*>(t->cellWidget(r, 2));
        rows.append(QJsonArray{t->item(r, 0)->text(), t->item(r, 1)->text(),
                               w ? QJsonValue(w->toPlainText()) : QJsonValue(), t->item(r, 0)->background().color().name()});
    }
    EXPECT_EQ(rows, s["table"].toArray());
}

}  // namespace

TEST(GuiBookmarks, LikeTheOriginal) {
    App app;
    QStringList msgs;
    QObject::connect(app.ui(), &MainGUI::informed, [&msgs](const QString& m) { msgs << m; });
    QStringList opened;
    app.urlOpener = [&opened](const QString& u) { opened << u; };
    const QJsonObject o = oracle();
    const QJsonArray steps = o["steps"].toArray();
    QTemporaryDir tmp;
    int i = 0;
    // Al arrancar: FlatCAM y el sitio de respaldo (sin el separador).
    expectStep(app, nullptr, steps[i++].toObject(), msgs, {});
    app.ui()->menuhelp_bookmarks_manager->trigger();
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    auto* mgr = qobject_cast<BookmarkManager*>(area->currentWidget());
    ASSERT_NE(mgr, nullptr);
    const QJsonObject tab = o["tab"].toObject();
    EXPECT_EQ(area->tabText(area->indexOf(mgr)), tab["title"].toString());
    EXPECT_EQ(mgr->objectName(), tab["name"].toString());
    EXPECT_EQ(mgr->link_entry->text(), tab["link"].toString());
    for (int c = 0; c < 3; ++c) {
        EXPECT_EQ(mgr->table_widget->horizontalHeaderItem(c)->text(), tab["headers"].toArray()[c].toString());
        EXPECT_EQ(mgr->table_widget->horizontalHeaderItem(c)->toolTip(), tab["tips"].toArray()[c].toString());
    }
    QJsonArray buttons, labels;
    for (QPushButton* b : mgr->findChildren<QPushButton*>()) buttons.append(b->text());
    for (QLabel* l : mgr->findChildren<QLabel*>()) labels.append(l->text());
    EXPECT_EQ(buttons, tab["buttons"].toArray());
    EXPECT_EQ(labels, tab["labels"].toArray());
    // Una sola pestaña a la vez.
    EXPECT_EQ(app.onBookmarksManager(), nullptr);
    expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    const std::vector<std::pair<QString, QString>> adds = {{"Foro", "https://foro.example/"}, {"", "https://x.example"},
                                                           {"Otro", "http://"},           {"Foro", "https://y.example"},
                                                           {"Z", "http://flatcam.org"},   {"Docs", "https://docs.example"}};
    for (const auto& [title, link] : adds) {
        mgr->title_entry->setText(title);
        mgr->link_entry->setText(link);
        mgr->add_entry_btn->click();
        expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    }
    for (const QString title : {QString("FlatCAM"), QString("Foro")}) {
        mgr->table_widget->clearSelection();
        for (int r = 0; r < mgr->table_widget->rowCount(); ++r)
            if (mgr->table_widget->item(r, 1)->text() == title) mgr->table_widget->selectRow(r);
        mgr->remove_entry_btn->click();
        expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    }
    mgr->table_widget->clearSelection();
    mgr->removeEntry();
    expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    const QString exp = tmp.path() + "/exported.txt";
    mgr->exportBookmarks(exp);
    QFile ef(exp);
    ASSERT_TRUE(ef.open(QIODevice::ReadOnly));
    EXPECT_EQ(QString::fromUtf8(ef.readAll()), o["exported"].toString());
    expectStep(app, mgr, steps[i++].toObject(), msgs, tmp.path());
    mgr->importBookmarks(kDir + "/import.txt");
    expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    mgr->importBookmarks(kDir + "/import2.txt");
    expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    ASSERT_TRUE(mgr->rebuildActions());
    expectStep(app, mgr, steps[i++].toObject(), msgs, {});
    // Con el límite en 3: los tres primeros del diccionario (las claves como texto), ordenados por número.
    app.preferences()->set("global_bookmarks_limit", JsonValue::integer(3));
    BookmarkDict many;
    for (int k : {1, 10, 11, 12, 2, 3, 4, 5, 6, 7, 8, 9})
        many.push_back({QString::number(k), {QString("M%1").arg(k), QString("http://m%1.example").arg(k)}});
    app.installBookmarks(many);
    expectStep(app, nullptr, steps[i++].toObject(), msgs, {});
    app.preferences()->set("global_bookmarks_limit", JsonValue::integer(10));
    app.installBookmarks();
    expectStep(app, nullptr, steps[i++].toObject(), msgs, {});
    EXPECT_EQ(i, steps.size());
    // Las acciones abren su enlace.
    for (QAction* a : app.ui()->menuhelp_bookmarks->actions())
        if (a->text() == "M3") a->trigger();
    EXPECT_EQ(opened, QStringList{"http://m3.example"});
}

TEST(GuiBookmarks, ClosingTheManagerRebuildsTheMenuInTableOrder) {
    App app;
    app.urlOpener = [](const QString&) {};
    BookmarkManager* mgr = app.onBookmarksManager();
    ASSERT_NE(mgr, nullptr);
    mgr->addEntry("A", "http://a.example", true);
    mgr->addEntry("B", "http://b.example", true);
    // Arrastrar la fila de A (2) al final: como el original, el enlace (un widget de celda) no se copia y, al cerrar,
    // rebuild_actions falla.
    mgr->table_widget->clearSelection();
    mgr->table_widget->selectRow(2);
    mgr->table_widget->moveSelectedRows(mgr->table_widget->rowCount());
    EXPECT_EQ(mgr->table_widget->item(3, 1)->text(), "A");
    EXPECT_EQ(mgr->table_widget->cellWidget(3, 2), nullptr);
    EXPECT_FALSE(mgr->rebuildActions());
    // Sin arrastrar: al cerrar la pestaña el menú queda con el orden de la tabla.
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    area->closeTab(area->indexOf(mgr));
    BookmarkManager* again = app.onBookmarksManager();
    ASSERT_NE(again, nullptr);
    area->closeTab(area->indexOf(again));
    QStringList texts;
    for (QAction* a : app.ui()->menuhelp_bookmarks->actions()) texts << a->text();
    EXPECT_EQ(texts.mid(0, 4), (QStringList{"FlatCAM", tr_("Backup Site"), "A", "B"}));
}
