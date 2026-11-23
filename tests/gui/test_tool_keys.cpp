// Teclas T y Supr sin modificador (on_tool_add_keypress y on_delete_keypress de app_Main.py, y las ramas de la base de
// herramientas de MainGUI.keyPressEvent): agregan o borran herramientas en la tabla que está a la vista; Supr borra
// los objetos elegidos solo fuera de las pestañas Propiedades y Herramienta.
#include <gtest/gtest.h>

#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>
#include <QTreeWidget>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/ToolsDBTab.h"
#include "gui/Translation.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/objects/ObjectUIs.h"
#include "gui/widgets/DialogBoxRadio.h"
#include "gui/widgets/FCLabel.h"  // fcinfo->text
#include "gui/widgets/FCSpinners.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {

void key(App& app, int k) {
    QKeyEvent e(QEvent::KeyPress, k, Qt::NoModifier);
    QApplication::sendEvent(app.ui(), &e);
}

/** Responde el próximo diálogo del diámetro (de los dos tipos): con un valor, o lo cancela si es negativo. */
void answerDia(double dia) {
    QTimer::singleShot(0, [dia] {
        QWidget* w = QApplication::activeModalWidget();
        auto* d = qobject_cast<QDialog*>(w);
        ASSERT_NE(d, nullptr);
        EXPECT_EQ(d->windowTitle(), tr_("New Tool") + "...");
        FCDoubleSpinner* s = nullptr;
        if (auto* a = qobject_cast<FCInputDoubleSpinner*>(w)) s = a->wdg;
        if (auto* b = qobject_cast<FCInputDialogSpinnerButton*>(w)) s = b->spinner;
        ASSERT_NE(s, nullptr);
        if (dia < 0) {
            d->reject();
            return;
        }
        s->setValue(dia);
        d->accept();
    });
}

QString info(App& app) { return app.ui()->fcinfo->text->text(); }

GerberObject* openLogo(App& app) {
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* g = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(g));
    return g;
}

void selectLastRow(QTableWidget* t) {
    t->clearSelection();
    t->selectRow(t->rowCount() - 1);
}

}  // namespace

TEST(GuiToolKeys, NccPaintIsolationAndSolderPasteAddAndDeleteWithTAndDelete) {
    App app;
    ASSERT_NE(openLogo(app), nullptr);
    const std::size_t objects = app.collection()->objects().size();

    // NCC: el diálogo con el botón del diámetro óptimo.
    app.ui()->ncc_btn->trigger();
    NccTool* ncc = app.nccTool;
    const std::size_t n0 = ncc->tools().size();
    answerDia(0.33);
    key(app, Qt::Key_T);
    ASSERT_EQ(ncc->tools().size(), n0 + 1);
    EXPECT_NEAR(ncc->tools().rbegin()->second.geo.tooldia, 0.33, 1e-9);
    // Cancelar no agrega nada. (El diálogo de NCC empieza en 0,0001, como el original: no se le puede dar un 0.)
    answerDia(-1);
    key(app, Qt::Key_T);
    EXPECT_EQ(info(app), tr_("Adding Tool cancelled") + "...");
    EXPECT_EQ(ncc->tools().size(), n0 + 1);
    // Supr borra la herramienta elegida, no el objeto.
    selectLastRow(ncc->tools_table);
    key(app, Qt::Key_Delete);
    EXPECT_EQ(ncc->tools().size(), n0);
    EXPECT_EQ(app.collection()->objects().size(), objects);

    // El botón del diálogo calcula el diámetro seguro del Gerber y lo pone en el campo.
    QTimer::singleShot(0, [] {
        auto* d = qobject_cast<FCInputDialogSpinnerButton*>(QApplication::activeModalWidget());
        ASSERT_NE(d, nullptr);
        d->button->click();
        EXPECT_NEAR(d->spinner->value(), 0.0285, 1e-4);  // el del Logo (oracle_safe_tooldia.py)
        d->accept();
    });
    key(app, Qt::Key_T);
    EXPECT_NEAR(ncc->tools().rbegin()->second.geo.tooldia, 0.0285, 1e-4);

    // Isolation.
    app.ui()->isolation_btn->trigger();
    const std::size_t i0 = app.isolationTool->tools().size();
    answerDia(0.41);
    key(app, Qt::Key_T);
    ASSERT_EQ(app.isolationTool->tools().size(), i0 + 1);
    selectLastRow(app.isolationTool->tools_table);
    key(app, Qt::Key_Delete);
    EXPECT_EQ(app.isolationTool->tools().size(), i0);

    // Paint.
    app.ui()->paint_btn->trigger();
    const std::size_t p0 = app.paintTool->tools().size();
    answerDia(0.27);
    key(app, Qt::Key_T);
    ASSERT_EQ(app.paintTool->tools().size(), p0 + 1);
    // El de Paint empieza en 0: un diámetro 0 avisa y no agrega nada.
    answerDia(0);
    key(app, Qt::Key_T);
    EXPECT_EQ(info(app), tr_("Please enter a tool diameter with non-zero value, in Float format."));
    EXPECT_EQ(app.paintTool->tools().size(), p0 + 1);
    selectLastRow(app.paintTool->tools_table);
    key(app, Qt::Key_Delete);
    EXPECT_EQ(app.paintTool->tools().size(), p0);

    // Pasta de soldar.
    app.ui()->solder_btn->trigger();
    const int s0 = app.solderPasteTool->tools_table->rowCount();
    answerDia(0.77);
    key(app, Qt::Key_T);
    ASSERT_EQ(app.solderPasteTool->tools_table->rowCount(), s0 + 1);
    EXPECT_EQ(info(app), tr_("New Nozzle tool added to Tool Table."));
    // Las boquillas van ordenadas por diámetro: se elige una celda visible de la fila de 0,77.
    QTableWidget* st = app.solderPasteTool->tools_table;
    st->clearSelection();
    for (int r = 0; r < st->rowCount(); ++r)
        if (st->item(r, 1)->text().toDouble() == 0.77) st->item(r, 1)->setSelected(true);
    ASSERT_EQ(st->selectedItems().size(), 1);
    key(app, Qt::Key_Delete);
    EXPECT_EQ(info(app), tr_("Tools deleted from Tool Table."));
    for (int r = 0; r < st->rowCount(); ++r) EXPECT_NE(st->item(r, 1)->text().toDouble(), 0.77);
    EXPECT_EQ(app.solderPasteTool->tools_table->rowCount(), s0);
    EXPECT_EQ(app.collection()->objects().size(), objects);

    // En el panel de otra herramienta, T no pide nada y Supr no borra el objeto.
    app.ui()->cutout_btn->trigger();
    QWidget* modal = nullptr;
    QTimer::singleShot(0, [&modal] {
        modal = QApplication::activeModalWidget();
        if (auto* d = qobject_cast<QDialog*>(modal)) d->reject();
    });
    key(app, Qt::Key_T);
    QApplication::processEvents();
    EXPECT_EQ(modal, nullptr);
    key(app, Qt::Key_Delete);
    EXPECT_EQ(app.collection()->objects().size(), objects);
}

TEST(GuiToolKeys, GeometryToolsOnlyInAdvancedAndDeleteKeepsObjectsOutsideTheProjectTab) {
    App app;
    app.preferences()->set("global_delete_confirmation", JsonValue::boolean(false));
    auto* geo = static_cast<GeometryObject*>(app.newGeometryObject());
    ASSERT_NE(geo, nullptr);
    app.setActive(geo, true);
    app.showProperties(geo);
    app.ui()->notebook->setCurrentWidget(app.ui()->properties_tab);
    const std::size_t t0 = geo->tools().size();

    // Nivel Básico: el aviso de que hace falta el Avanzado, sin pedir el diámetro.
    app.preferences()->set("global_app_level", JsonValue::string("b"));
    QString text;
    QTimer::singleShot(0, [&text] {
        auto* box = qobject_cast<QMessageBox*>(QApplication::activeModalWidget());
        ASSERT_NE(box, nullptr);
        text = box->text();
        box->accept();
    });
    key(app, Qt::Key_T);
    EXPECT_EQ(text, tr_("Adding Tool works only when Advanced is checked.\n"
                        "Go to Preferences -> General - Show Advanced Options."));
    EXPECT_EQ(geo->tools().size(), t0);

    // Nivel Avanzado: agrega la herramienta.
    app.preferences()->set("global_app_level", JsonValue::string("a"));
    app.showProperties(geo);
    answerDia(0.6);
    key(app, Qt::Key_T);
    ASSERT_EQ(geo->tools().size(), t0 + 1);
    EXPECT_NEAR(geo->tools().rbegin()->second.tooldia, 0.6, 1e-9);

    // Supr en Propiedades borra la herramienta elegida y deja el objeto.
    auto* p = qobject_cast<GeometryObjectUI*>(geo->builtUi());
    ASSERT_NE(p, nullptr);
    selectLastRow(p->geo_tools_table);
    key(app, Qt::Key_Delete);
    EXPECT_EQ(geo->tools().size(), t0);
    ASSERT_EQ(app.collection()->objects().size(), 1u);

    // En la pestaña Proyecto, Supr borra los objetos elegidos.
    app.ui()->notebook->setCurrentWidget(app.ui()->project_tab);
    auto* sm = app.ui()->collection_view->selectionModel();
    sm->clearSelection();
    sm->select(app.collection()->indexOf(geo), QItemSelectionModel::Select);
    key(app, Qt::Key_Delete);
    EXPECT_TRUE(app.collection()->objects().empty());
}

TEST(GuiToolKeys, ToolsDatabaseTabAddsAndDeletesWithTAndDelete) {
    App app;
    ToolsDBTab* db = app.openToolsDatabase();
    ASSERT_NE(db, nullptr);
    const std::size_t n0 = db->tools().members().size();
    key(app, Qt::Key_T);
    ASSERT_EQ(db->tools().members().size(), n0 + 1);
    // addTool deja elegida la nueva: Supr la borra.
    key(app, Qt::Key_Delete);
    EXPECT_EQ(db->tools().members().size(), n0);
    EXPECT_EQ(info(app), tr_("Tool removed from Tools DB."));
}
