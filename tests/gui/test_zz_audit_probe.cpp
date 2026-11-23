// SONDA TEMPORAL DE AUDITORÍA: borrar.
#include <gtest/gtest.h>

#include <QApplication>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QTimer>

#include "core/OriginalBugs.h"
#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/tools/ToolPanels.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCLabel.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {
const QString kScratch =
    "/private/tmp/claude-501/-Volumes-Externo-1TB-Google-Drive-dnasta-DSD-proyectos-productos-FlatCAM-port-fuentes-"
    "flatcam-cpp/3aa537ea-8a9e-4de2-a6ee-f0827b4cc922/scratchpad/drl/";
const QString kLuz = QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
QString info(App& app) { return app.ui()->fcinfo->text->text(); }

DrillingTool* openDrill(App& app, FlatCAMObj* exc) {
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    app.ui()->drill_btn->trigger();
    return app.drillingTool;
}
}  // namespace

TEST(ZzProbe, DrillDeleteThenGenerate) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    ASSERT_NE(exc, nullptr);
    DrillingTool* t = openDrill(app, exc);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    ASSERT_TRUE(app.deleteSelected(true));
    fprintf(stderr, "deleted, combo=%s\n", t->object_combo->currentText().toStdString().c_str());
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p info=%s\n", (void*)r, info(app).toStdString().c_str());
}

TEST(ZzProbe, DrillNewProjectThenGenerate) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    DrillingTool* t = openDrill(app, exc);
    app.clearProject();
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p info=%s\n", (void*)r, info(app).toStdString().c_str());
}

TEST(ZzProbe, DrillEmptyToolSelected) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kScratch + "empty_tool.drl");
    ASSERT_NE(exc, nullptr);
    DrillingTool* t = openDrill(app, exc);
    fprintf(stderr, "rows=%d\n", t->tools_table->rowCount());
    t->tools_table->clearSelection();
    t->tools_table->selectRow(0);
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p info=%s\n", (void*)r, info(app).toStdString().c_str());
    if (r) {
        auto* c = static_cast<CNCJobObject*>(r);
        fprintf(stderr, "gcode:\n%s\n", c->job().gcode().substr(0, 800).c_str());
    }
}

TEST(ZzProbe, DrillSlotsOnly) {
    OriginalBugsScope fixed(false);
    App app;
    app.preferences()->set("global_app_level", JsonValue::string("a"));
    FlatCAMObj* exc = app.openExcellon(kScratch + "slots_only.drl");
    ASSERT_NE(exc, nullptr);
    DrillingTool* t = openDrill(app, exc);
    // la casilla global marcada pero con ninguna herramienta elegida al marcarla
    t->tools_table->clearSelection();
    t->drill_slots_cb->setChecked(true);
    t->tools_table->selectRow(0);
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p info=%s\n", (void*)r, info(app).toStdString().c_str());
}

TEST(ZzProbe, DrillSameDiaNonOne) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kScratch + "same_dia.drl");
    ASSERT_NE(exc, nullptr);
    DrillingTool* t = openDrill(app, exc);
    t->toolchange_cb->setChecked(true);
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p info=%s\n", (void*)r, info(app).toStdString().c_str());
    FlatCAMObj* r2 = t->generateCnc();
    fprintf(stderr, "result2 %p\n", (void*)r2);
}

TEST(ZzProbe, DrillMultiDepthZeroPerPass) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    DrillingTool* t = openDrill(app, exc);
    t->tools_table->selectAll();
    t->mpass_cb->setChecked(true);
    t->maxdepth_entry->setValue(0);
    t->cutz_entry->setValue(1.0);  // positivo
    QElapsedTimer e;
    e.start();
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p %lld ms info=%s\n", (void*)r, (long long)e.elapsed(), info(app).toStdString().c_str());
}

TEST(ZzProbe, DrillUnitsToggleThenGenerate) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    DrillingTool* t = openDrill(app, exc);
    QString before = t->tools_table->item(0, 1)->text();
    app.onToggleUnits(true);
    fprintf(stderr, "dia before %s after %s units=%s\n", before.toStdString().c_str(),
            t->tools_table->item(0, 1)->text().toStdString().c_str(), defStr("units").toStdString().c_str());
    FlatCAMObj* r = t->generateCnc();
    if (r) {
        auto* c = static_cast<CNCJobObject*>(r);
        fprintf(stderr, "gcode:\n%s\n", c->job().gcode().substr(0, 1500).c_str());
    }
}

TEST(ZzProbe, DrillHeaderlessTool) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kScratch + "nohdr.drl");
    ASSERT_NE(exc, nullptr);
    DrillingTool* t = openDrill(app, exc);
    FlatCAMObj* r = t->generateCnc();
    fprintf(stderr, "result %p info=%s\n", (void*)r, info(app).toStdString().c_str());
}

TEST(ZzProbe, DrillDeleteSourceThenProjectSave) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    DrillingTool* t = openDrill(app, exc);
    FlatCAMObj* r = t->generateCnc();
    ASSERT_NE(r, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    app.deleteSelected(true);
    bool ok = app.saveProject(kScratch + "p.FlatPrj");
    fprintf(stderr, "save %d\n", ok);
    App app2;
    bool ok2 = app2.openProject(kScratch + "p.FlatPrj");
    fprintf(stderr, "open %d objs=%zu\n", ok2, app2.collection()->objects().size());
    for (FlatCAMObj* o : app2.collection()->objects()) {
        if (auto* c = dynamic_cast<CNCJobObject*>(o)) {
            std::string g = c->job().gcode();
            fprintf(stderr, "gcode len %zu\n", g.size());
        }
    }
}
