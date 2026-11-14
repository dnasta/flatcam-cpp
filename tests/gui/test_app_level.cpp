// El nivel de la aplicación (global_app_level: Básico o Avanzado), contra el original
// (tests/tools/oracle_app_level.py -> tests/data/app_level/oracle.json): qué se ve en cada panel, las columnas
// escondidas, los valores y el rótulo; además "Taladrar ranuras" (process_slot_as_drills) y la carga automática de la
// base de herramientas en Perforación.
#include <gtest/gtest.h>

#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QTableWidget>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/ToolsDatabase.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/DocumentObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/objects/ObjectUIs.h"
#include "gui/objects/ScriptObject.h"
#include "gui/tools/DrillingTool.h"
#include "gui/tools/IsolationTool.h"
#include "gui/widgets/RadioSet.h"

/** Para que GoogleTest muestre los QString como texto. */
inline void PrintTo(const QString& s, std::ostream* os) { *os << '"' << s.toStdString() << '"'; }

using namespace flatcam;
using namespace flatcam::gui;

namespace {

const QString kData = QString(FLATCAM_TEST_DATA_DIR);

QJsonObject oracle() {
    QFile f(kData + "/app_level/oracle.json");
    EXPECT_TRUE(f.open(QIODevice::ReadOnly));
    return QJsonDocument::fromJson(f.readAll()).object();
}

/** Compara los widgets que cambian entre los dos niveles (y el rótulo) con el original. */
void expectPanel(const std::map<std::string, QObject*>& members, const QJsonObject& mine, const QJsonObject& other) {
    for (auto it = mine.begin(); it != mine.end(); ++it) {
        const QJsonObject s = it.value().toObject();
        if (it.key() != "level" && other.value(it.key()).toObject() == s) continue;  // no depende del nivel
        SCOPED_TRACE(it.key().toStdString());
        const auto m = members.find(it.key().toStdString());
        ASSERT_NE(m, members.end());
        auto* w = qobject_cast<QWidget*>(m->second);
        ASSERT_NE(w, nullptr);
        EXPECT_EQ(w->isHidden(), s["hidden"].toBool());
        if (auto* t = qobject_cast<QTableWidget*>(w)) {
            QJsonArray cols;
            for (int c = 0; c < t->columnCount(); ++c)
                if (t->isColumnHidden(c)) cols.append(c);
            EXPECT_EQ(cols, s["hiddenCols"].toArray());
        }
        if (!s.contains("value")) continue;
        if (auto* cb = qobject_cast<QCheckBox*>(w)) EXPECT_EQ(cb->isChecked(), s["value"].toBool());
        else if (auto* co = qobject_cast<QComboBox*>(w)) EXPECT_EQ(co->currentText(), s["value"].toString());
        else if (auto* r = qobject_cast<RadioSet*>(w)) EXPECT_EQ(r->value().toString(), s["value"].toString());
        else if (auto* l = qobject_cast<QLabel*>(w)) EXPECT_EQ(l->text(), s["value"].toString());
    }
}

}  // namespace

TEST(GuiAppLevel, PanelsLikeTheOriginal) {
    const QJsonObject o = oracle();
    for (const QString level : {QString("b"), QString("a")}) {
        SCOPED_TRACE(level.toStdString());
        App app;
        app.preferences()->set("global_app_level", JsonValue::string(level.toStdString()));
        const QJsonObject mine = o[level].toObject(), other = o[level == "b" ? "a" : "b"].toObject();
        auto check = [&](const char* cls, const std::map<std::string, QObject*>& m) {
            SCOPED_TRACE(cls);
            expectPanel(m, mine[cls].toObject(), other[cls].toObject());
        };
        FlatCAMObj* grb = app.openGerber(kData + "/luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr");
        FlatCAMObj* exc = app.openExcellon(kData + "/luz_de_noche/luz de noche Pascual-PTH.drl");
        FlatCAMObj* geo = app.newGeometryObject();
        ASSERT_TRUE(grb && exc && geo);
        check("GerberObjectUI", static_cast<GerberObjectUI*>(grb->propertiesUi())->membersByName());
        check("ExcellonObjectUI", static_cast<ExcellonObjectUI*>(exc->propertiesUi())->membersByName());
        check("GeometryObjectUI", static_cast<GeometryObjectUI*>(geo->propertiesUi())->membersByName());
        auto* script = static_cast<ScriptObject*>(app.onFileNewScript());
        check("ScriptObjectUI", script->ui()->membersByName());
        auto* doc = static_cast<DocumentObject*>(app.newDocumentObject());
        check("DocumentObjectUI", static_cast<DocumentObjectUI*>(doc->propertiesUi())->membersByName());
        app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(grb));
        app.ui()->isolation_btn->trigger();
        check("IsolationToolUI", app.isolationTool->membersByName());
        app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
        app.ui()->drill_btn->trigger();
        check("DrillingToolUI", app.drillingTool->membersByName());
        ASSERT_TRUE(app.openProject(kData + "/luz_de_noche/luz_de_noche_reducido.FlatPrj"));
        auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
        ASSERT_NE(cnc, nullptr);
        app.showProperties(cnc);
        check("CNCObjectUI", static_cast<CNCObjectUI*>(cnc->builtUi())->membersByName());
    }
}

TEST(GuiAppLevel, SlotsAsDrillsLikeTheOriginal) {
    for (const QJsonValue& v : oracle()["slots"].toArray()) {
        const QJsonObject s = v.toObject();
        const QJsonArray sl = s["slot"].toArray();
        SCOPED_TRACE(QJsonDocument(sl).toJson(QJsonDocument::Compact).toStdString());
        const auto pts = DrillingTool::slotAsDrills({sl[0].toDouble(), sl[1].toDouble()},
                                                    {sl[2].toDouble(), sl[3].toDouble()}, s["overlap"].toDouble(),
                                                    s["last"].toBool());
        const QJsonArray theirs = s["drills"].toArray();
        ASSERT_EQ(static_cast<int>(pts.size()), theirs.size());
        for (int i = 0; i < theirs.size(); ++i) {
            EXPECT_NEAR(pts[static_cast<std::size_t>(i)].x, theirs[i].toArray()[0].toDouble(), 1e-12);
            EXPECT_NEAR(pts[static_cast<std::size_t>(i)].y, theirs[i].toArray()[1].toDouble(), 1e-12);
        }
    }
}

TEST(GuiAppLevel, AdvancedDrillingUsesSlotsAndTheToolsDatabase) {
    App app;
    app.preferences()->set("global_app_level", JsonValue::string("a"));
    // Una base con una herramienta de 0,9 que acepta de 0,95 a 1,05 (la de 1,0 del Excellon) y Z de corte -3.
    JsonValue db;
    QString err;
    const QString file = ToolsDatabase::fileName(app.dataPath());
    ToolsDatabase::ensureFile(app.dataPath());
    JsonValue tool = ToolsDatabase::newTool(JsonValue::object(), appDefaults());
    tool.set("tooldia", JsonValue::real(0.9));
    JsonValue& data = const_cast<JsonValue&>(tool["data"]);
    data.set("tol_min", JsonValue::real(0.95));
    data.set("tol_max", JsonValue::real(1.05));
    data.set("tools_drill_cutz", JsonValue::real(-3.0));
    db = JsonValue::object();
    db.set("1", tool);
    ASSERT_TRUE(ToolsDatabase::save(file, db));
    FlatCAMObj* exc = app.openExcellon(kData + "/excellon_samples/slots_g85.drl");
    ASSERT_NE(exc, nullptr);
    // Sin la casilla: sin cambios. Con "Cargar automáticamente de la base" del Excellon, Perforación toma la base.
    auto* eui = static_cast<ExcellonObjectUI*>(exc->propertiesUi());
    EXPECT_FALSE(eui->autoload_db_cb->isHidden());
    eui->autoload_db_cb->setChecked(true);
    EXPECT_TRUE(defBool("excellon_autoload_db"));
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    app.ui()->drill_btn->trigger();
    DrillingTool* t = app.drillingTool;
    EXPECT_FALSE(t->drill_slots_cb->isHidden());
    EXPECT_FALSE(t->estartz_entry->isHidden());
    // Taladrar ranuras: el CNC Job cuenta los agujeros de las ranuras y no las ranuras.
    t->tools_table->selectAll();
    t->drill_slots_cb->setChecked(true);
    auto* job = dynamic_cast<CNCJobObject*>(t->generateCnc());
    ASSERT_NE(job, nullptr);
    ASSERT_EQ(job->tools().size(), 1u);
    EXPECT_EQ(job->tools()[0].slots, 0);
    EXPECT_GT(job->tools()[0].drills, 0);
    // La base cambió el diámetro (por la tolerancia) y la Z de corte.
    EXPECT_DOUBLE_EQ(job->tools()[0].dia, 0.9);
    EXPECT_DOUBLE_EQ(job->tools()[0].cutZ, -3.0);
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "0.9");
    app.preferences()->set("excellon_autoload_db", JsonValue::boolean(false));
    ASSERT_TRUE(ToolsDatabase::save(file, JsonValue::object()));  // la base de las pruebas queda vacía
}
