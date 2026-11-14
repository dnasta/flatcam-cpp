// Objetos vacíos de Archivo -> Nuevo (new_geometry_object, new_gerber_object, new_excellon_object de AppObject.py):
// se crean sin dibujar, se editan con su editor y se guardan.
#include <gtest/gtest.h>

#include <QApplication>
#include <QKeyEvent>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/editors/ExcEditor.h"
#include "gui/editors/GeoEditor.h"
#include "gui/editors/GrbEditor.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/objects/ScriptObject.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {

void key(App& app, int k) {
    QKeyEvent e(QEvent::KeyPress, k, Qt::NoModifier);
    QApplication::sendEvent(app.ui(), &e);
}

void selectOnly(App& app, FlatCAMObj* o) {
    auto* sm = app.ui()->collection_view->selectionModel();
    sm->clearSelection();
    sm->select(app.collection()->indexOf(o), QItemSelectionModel::Select);
}

}  // namespace

TEST(GuiNewObjects, MenuAndKeysCreateEmptyObjects) {
    App app;
    app.ui()->menufilenewgeo->trigger();
    app.ui()->menufilenewgrb->trigger();
    app.ui()->menufilenewexc->trigger();
    EXPECT_EQ(app.collection()->names(), (QStringList{"new_grb", "new_exc", "new_geo"}));
    // N, B, L y D sin modificador crean otros (con el nombre repetido numerado).
    key(app, Qt::Key_N);
    key(app, Qt::Key_B);
    key(app, Qt::Key_L);
    key(app, Qt::Key_D);
    EXPECT_EQ(app.collection()->objects().size(), 7u);
    EXPECT_NE(app.collection()->byName("new_document"), nullptr);
    // La Geometría: multiherramienta, una herramienta de geometry_cnctooldia sin geometría.
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("new_geo"));
    ASSERT_NE(geo, nullptr);
    ASSERT_EQ(geo->tools().size(), 1u);
    const GeometryTool& t = geo->tools().at(1);
    EXPECT_DOUBLE_EQ(t.tooldia, defNum("geometry_cnctooldia"));
    EXPECT_EQ(t.offset, "Path");
    EXPECT_EQ(t.type, "Rough");
    EXPECT_EQ(t.toolType, "C1");
    EXPECT_TRUE(t.solidGeometry.empty());
    auto* grb = dynamic_cast<GerberObject*>(app.collection()->byName("new_grb"));
    ASSERT_NE(grb, nullptr);
    EXPECT_TRUE(grb->gerber().apertures().empty());
    EXPECT_TRUE(grb->gerber().solidGeometry().empty());
    auto* exc = dynamic_cast<ExcellonObject*>(app.collection()->byName("new_exc"));
    ASSERT_NE(exc, nullptr);
    EXPECT_TRUE(exc->excellon().drillTools().empty());
}

TEST(GuiNewObjects, EmptyGeometryIsEditedAndSaved) {
    App app;
    FlatCAMObj* obj = app.newGeometryObject();
    selectOnly(app, obj);
    key(app, Qt::Key_E);
    GeoEditor* ed = app.geoEditor;
    ASSERT_TRUE(ed->active());
    EXPECT_EQ(ed->model().storage.size(), 0u);
    ed->selectTool("rectangle");
    ed->onPress({0, 0}, Qt::LeftButton);
    ed->onMove({4, 3}, false);
    ed->onPress({4, 3}, Qt::LeftButton);
    ASSERT_EQ(ed->model().storage.size(), 1u);
    ASSERT_TRUE(app.editorToObject(true));
    auto* geo = static_cast<GeometryObject*>(obj);
    ASSERT_EQ(geo->tools().at(1).solidGeometry.size(), 1u);
    EXPECT_NEAR(geo->tools().at(1).solidGeometry.front().area(), 12.0, 1e-9);
}

TEST(GuiNewObjects, EmptyExcellonIsEditedAndSavedAsANewObject) {
    App app;
    FlatCAMObj* obj = app.newExcellonObject();
    selectOnly(app, obj);
    app.objectToEditor();
    ExcEditor* ed = app.excEditor;
    ASSERT_TRUE(ed->active());
    ed->addTool(1.0);
    ed->rowSelected(0);
    ed->selectTool("drill_add");
    ed->onMove({5, 5}, false);
    ed->onPress({5, 5}, Qt::LeftButton);
    ASSERT_NE(ed->model().storage(1.0), nullptr);
    EXPECT_EQ(ed->model().storage(1.0)->size(), 1u);
    ASSERT_TRUE(app.editorToObject(true));
    FlatCAMObj* out = ed->lastCreated();
    ASSERT_NE(out, nullptr);
    const auto& tools = static_cast<ExcellonObject*>(out)->excellon().drillTools();
    ASSERT_EQ(tools.size(), 1u);
    EXPECT_EQ(tools.begin()->second.drills.size(), 1u);
    // Como el original, el Excellon vacío de origen se borra.
    EXPECT_EQ(app.collection()->names(), QStringList{out->name()});
}

TEST(GuiNewObjects, EmptyGerberIsEditedAndSavedAsANewObject) {
    App app;
    FlatCAMObj* obj = app.newGerberObject();
    selectOnly(app, obj);
    app.objectToEditor();
    GrbEditor* ed = app.grbEditor;
    ASSERT_TRUE(ed->active());
    ed->addAperture("10");
    ASSERT_GT(ed->panel()->apertures_table->rowCount(), 0);
    ed->rowSelected(0);
    ed->selectTool("pad");
    ed->onMove({2, 2}, false);
    ed->onPress({2, 2}, Qt::LeftButton);
    ASSERT_TRUE(app.editorToObject(true));
    FlatCAMObj* out = ed->lastCreated();
    ASSERT_NE(out, nullptr);
    EXPECT_EQ(out->name(), "new_grb_edit");
    EXPECT_FALSE(static_cast<GerberObject*>(out)->gerber().solidGeometry().empty());
    // Como el original, el Gerber vacío de origen se borra.
    EXPECT_EQ(app.collection()->names(), QStringList{"new_grb_edit"});
}
