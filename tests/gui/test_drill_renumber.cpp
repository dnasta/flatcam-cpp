// Perforación: la tabla ordena y renumera las herramientas y el G-code usa esos números (build_tool_ui y
// on_cnc_button_click de ToolDrilling), contra el original (tests/tools/oracle_drill_renumber.py ->
// tests/data/drill_renumber/oracle.json); también arrastrar filas (drag_drop_sig -> rebuild_ui).
#include <gtest/gtest.h>

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/tools/DrillingTool.h"
#include "gui/tools/IsolationTool.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectUIs.h"
#include "gui/widgets/RadioSet.h"

/** Para que GoogleTest muestre los QString como texto. */
inline void PrintTo(const QString& s, std::ostream* os) { *os << '"' << s.toStdString() << '"'; }

using namespace flatcam;
using namespace flatcam::gui;

namespace {

QJsonObject oracle() {
    QFile f(QString(FLATCAM_TEST_DATA_DIR) + "/drill_renumber/oracle.json");
    EXPECT_TRUE(f.open(QIODevice::ReadOnly));
    return QJsonDocument::fromJson(f.readAll()).object();
}

QJsonArray tableRows(FCTable* t) {
    QJsonArray rows;
    for (int r = 0; r < t->rowCount(); ++r) {
        QJsonArray row;
        for (int c = 0; c < 5; ++c) row.append(t->item(r, c) ? QJsonValue(t->item(r, c)->text()) : QJsonValue());
        rows.append(row);
    }
    return rows;
}

void setOrder(DrillingTool* t, const QString& order) {
    t->order_radio->setValue(order);
    Q_EMIT t->order_radio->activatedCustom(order);
}

QJsonArray toolLines(const CNCJobObject* job) {
    QJsonArray out;
    for (const QString& l : QString::fromStdString(job->job().gcode()).split('\n'))
        if (l.startsWith("T") || l.startsWith("M6")) out.append(l);
    return out;
}

}  // namespace

TEST(GuiDrillRenumber, TableAndToolNumbersLikeTheOriginal) {
    App app;
    FlatCAMObj* exc = app.openExcellon(QString(FLATCAM_TEST_DATA_DIR) + "/excellon_samples/renumber.drl");
    ASSERT_NE(exc, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    app.ui()->drill_btn->trigger();
    DrillingTool* t = app.drillingTool;
    const QJsonObject o = oracle();
    const QJsonObject tables = o["tables"].toObject();
    for (const auto& [name, order] : std::vector<std::pair<QString, QString>>{
             {"no", "no"}, {"fwd", "fwd"}, {"rev", "rev"}, {"no after rev", "no"}}) {
        SCOPED_TRACE(name.toStdString());
        setOrder(t, order);
        EXPECT_EQ(tableRows(t->tools_table), tables[name].toArray());
    }
    // El G-code con cambio de herramienta: T1, T2, T3 en el orden de la tabla.
    t->toolchange_cb->setChecked(true);
    for (const QString name : {QString("fwd"), QString("no")}) {
        SCOPED_TRACE(name.toStdString());
        // Como en el oráculo: se vuelve a abrir el objeto (el orden del archivo) y se elige el orden.
        app.ui()->drill_btn->trigger();
        app.ui()->drill_btn->trigger();
        setOrder(t, name);
        t->tools_table->selectAll();
        auto* job = dynamic_cast<CNCJobObject*>(t->generateCnc());
        ASSERT_NE(job, nullptr);
        const QJsonObject g = o["gcode"].toObject()[name].toObject();
        // El original, con "no" (la segunda vez), suma el G-code de la generación anterior (ver la sección 14).
        QJsonArray theirs = g["lines"].toArray();
        QJsonArray mine = toolLines(job);
        while (theirs.size() > mine.size()) theirs.removeAt(0);
        EXPECT_EQ(QJsonDocument(mine).toJson(QJsonDocument::Compact).toStdString(),
                  QJsonDocument(theirs).toJson(QJsonDocument::Compact).toStdString());
        QJsonArray rows;
        for (const CncToolRow& r : job->tools())
            rows.append(QJsonArray{QString::number(r.dia, 'f', 1), r.tool, r.drills});
        EXPECT_EQ(rows, g["exc_cnc_tools"].toArray());
    }
}

TEST(GuiDrillRenumber, DraggingRowsReordersTheTools) {
    App app;
    FlatCAMObj* exc = app.openExcellon(QString(FLATCAM_TEST_DATA_DIR) + "/excellon_samples/renumber.drl");
    ASSERT_NE(exc, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    app.ui()->drill_btn->trigger();
    DrillingTool* t = app.drillingTool;
    setOrder(t, "no");
    // Filas: 1,0 / 0,5 / 0,8. La de 0,8 va arriba de todo.
    t->tools_table->clearSelection();
    t->tools_table->selectRow(2);
    t->tools_table->moveSelectedRows(0);
    // rebuild_ui reconstruye la tabla a los 20 ms.
    QEventLoop loop;
    QTimer::singleShot(60, &loop, &QEventLoop::quit);
    loop.exec();
    QStringList dias;
    for (int r = 0; r < 3; ++r) dias << t->tools_table->item(r, 1)->text();
    EXPECT_EQ(dias, (QStringList{"0.8", "1.0", "0.5"}));
    EXPECT_EQ(t->tools_table->item(0, 3)->text(), "1");
    t->toolchange_cb->setChecked(true);
    t->tools_table->selectAll();
    auto* job = dynamic_cast<CNCJobObject*>(t->generateCnc());
    ASSERT_NE(job, nullptr);
    ASSERT_EQ(job->tools().size(), 3u);
    EXPECT_DOUBLE_EQ(job->tools()[0].dia, 0.8);
    EXPECT_EQ(job->tools()[0].tool, 1);
    // Con "fwd" vuelve a ordenar por diámetro.
    setOrder(t, "fwd");
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "0.5");
}

TEST(GuiToolTables, DraggingRowsRenumbersIsolationAndGeometryTools) {
    App app;
    app.ui()->isolation_btn->trigger();
    IsolationTool* iso = app.isolationTool;
    iso->order_radio->setValue(QString("no"));
    Q_EMIT iso->order_radio->activatedCustom("no");
    ASSERT_TRUE(iso->addTool(0.3));
    ASSERT_TRUE(iso->addTool(0.5));
    const int n = iso->tools_table->rowCount();
    ASSERT_GE(n, 3);
    const double last = iso->tools().rbegin()->second.geo.tooldia;
    // La última fila arriba: pasa a ser la herramienta 1.
    iso->tools_table->clearSelection();
    iso->tools_table->selectRow(n - 1);
    iso->tools_table->moveSelectedRows(0);
    EXPECT_DOUBLE_EQ(iso->tools().at(1).geo.tooldia, last);
    QEventLoop loop;
    QTimer::singleShot(60, &loop, &QEventLoop::quit);
    loop.exec();
    EXPECT_EQ(iso->tools_table->item(0, 1)->text(), QString::number(last, 'f', 4));
    EXPECT_EQ(iso->tools_table->item(0, 3)->text(), "1");

    // Geometría: las herramientas se renumeran pero la tabla no se vuelve a armar.
    auto g = std::make_unique<GeometryObject>("geo");
    for (int i = 1; i <= 2; ++i) {
        GeometryTool t = defaultGeometryTool();
        t.tooldia = i;
        g->tools()[i] = t;
    }
    FlatCAMObj* obj = app.addObject(std::move(g));
    auto* ui = qobject_cast<GeometryObjectUI*>(obj->propertiesUi());
    ASSERT_NE(ui, nullptr);
    ui->geo_tools_table->clearSelection();
    ui->geo_tools_table->selectRow(1);
    ui->geo_tools_table->moveSelectedRows(0);
    auto* geo = static_cast<GeometryObject*>(obj);
    EXPECT_DOUBLE_EQ(geo->tools().at(1).tooldia, 2.0);
    EXPECT_DOUBLE_EQ(geo->tools().at(2).tooldia, 1.0);
    EXPECT_EQ(ui->geo_tools_table->item(0, 5)->text(), "2");  // el número viejo, como el original
}
