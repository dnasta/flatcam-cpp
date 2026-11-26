// Errores que encontró la auditoría adversarial de la GUI (1.0.0): objetos borrados mientras una herramienta los
// usa, herramientas borradas mientras se elige un área, el cambio de unidades con Perforación abierta y los
// "mouse bites" que colgaban el programa.
#include <gtest/gtest.h>

#include <QAbstractButton>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>

#include "core/OriginalBugs.h"
#include "gui/App.h"
#include "gui/CanvasPicker.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/ExclusionAreasManager.h"
#include "gui/editors/GCodeEditor.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/ObjectUIs.h"
#include "gui/tools/ClearingTools.h"
#include "gui/shell/FCShell.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/tools/ToolPanels.h"
#include "gui/widgets/FCLabel.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {

const QString kLuz = QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";

/// Acepta cualquier diálogo modal que aparezca (la confirmación de borrar, la de cambiar las unidades).
struct AutoAccept {
    QTimer timer;
    AutoAccept() {
        timer.setInterval(20);
        QObject::connect(&timer, &QTimer::timeout, [] {
            QWidget* w = QApplication::activeModalWidget();
            if (auto* mb = qobject_cast<QMessageBox*>(w)) {
                if (!mb->buttons().isEmpty()) mb->buttons().first()->click();
            } else if (auto* d = qobject_cast<QDialog*>(w)) {
                d->accept();
            }
        });
        timer.start();
    }
};

QString info(App& app) { return app.ui()->fcinfo->text->text(); }

void select(App& app, FlatCAMObj* o) { app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(o)); }

void deleteObject(App& app, FlatCAMObj* o) {
    select(app, o);
    ASSERT_TRUE(app.deleteSelected(true));
}

/// Un rectángulo con el mouse y el clic derecho que termina la elección.
void pickArea(App& app) {
    app.picker()->onRelease(QPointF(-200, -200), Qt::LeftButton, false);
    app.picker()->onRelease(QPointF(200, 200), Qt::LeftButton, false);
    app.picker()->onRelease(QPointF(200, 200), Qt::RightButton, false);
}

}  // namespace

TEST(GuiAudit, DrillingWithTheExcellonDeletedOrANewProject) {
    OriginalBugsScope fixed(false);
    AutoAccept accept;
    App app;
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    ASSERT_NE(exc, nullptr);
    select(app, exc);
    app.ui()->drill_btn->trigger();
    deleteObject(app, exc);
    EXPECT_EQ(app.drillingTool->generateCnc(), nullptr);  // antes: memoria liberada
    EXPECT_TRUE(info(app).contains(tr_("Object not found")));

    FlatCAMObj* again = app.openExcellon(kLuz + "PTH.drl");
    select(app, again);
    app.ui()->drill_btn->trigger();
    app.clearProject();
    EXPECT_EQ(app.drillingTool->generateCnc(), nullptr);
}

TEST(GuiAudit, ToolsThatWaitForClicksSurviveTheObjectBeingDeleted) {
    OriginalBugsScope fixed(false);
    AutoAccept accept;
    App app;
    // Código QR: generar, borrar el Gerber y hacer el clic.
    FlatCAMObj* logo = app.openByExtension(kLuz + "Logo.gbr");
    select(app, logo);
    app.ui()->qrcode_btn->trigger();
    app.qrcodeTool->text_data->setPlainText("hola");
    ASSERT_TRUE(app.qrcodeTool->execute());
    deleteObject(app, logo);
    app.qrcodeTool->onRelease(QPointF(1, 1), Qt::LeftButton);
    EXPECT_TRUE(app.collection()->objects().empty());

    // PCB a 2 caras: "Elegir agujero" y borrar el Excellon.
    FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
    select(app, exc);
    app.ui()->dblsided_btn->trigger();
    app.dblsidedTool->exc_combo->setValue(exc->name());
    app.dblsidedTool->pick_hole_button->click();
    deleteObject(app, exc);
    EXPECT_FALSE(app.dblsidedTool->pickHoleAt(QPointF(-1.61, -4.31)));

    // Fiduciales a mano: los tres clics después de borrar el Gerber.
    FlatCAMObj* cu = app.openByExtension(kLuz + "B_Cu.gbr");
    select(app, cu);
    app.ui()->fiducials_btn->trigger();
    app.fiducialsTool->mode_radio->setValue(QString("manual"));
    app.fiducialsTool->addFiducials();
    deleteObject(app, cu);
    for (int i = 0; i < 3; ++i) app.fiducialsTool->onRelease(QPointF(i * 10, i * 10), Qt::LeftButton);
    EXPECT_TRUE(app.collection()->objects().empty());
    EXPECT_FALSE(app.fiducialsTool->isPicking());
}

TEST(GuiAudit, ClearingToolsDeletedWhilePickingTheArea) {
    // Antes: std::out_of_range desde un slot de Qt (el programa terminaba).
    OriginalBugsScope fixed(false);
    AutoAccept accept;
    App app;
    FlatCAMObj* logo = app.openByExtension(kLuz + "Logo.gbr");
    const std::size_t objects = app.collection()->objects().size();

    select(app, logo);
    app.ui()->ncc_btn->trigger();
    app.nccTool->select_combo->setCurrentIndex(1);
    app.nccTool->tools_table->selectAll();
    app.nccTool->generate_ncc_button->click();
    ASSERT_TRUE(app.picker()->active());
    app.nccTool->tools_table->selectAll();
    app.nccTool->deltool_btn->click();
    pickArea(app);
    EXPECT_EQ(app.collection()->objects().size(), objects);
    EXPECT_TRUE(info(app).contains(tr_("No selected tools in Tool Table."))) << info(app).toStdString();

    select(app, logo);
    app.ui()->paint_btn->trigger();
    app.paintTool->selectmethod_combo->setCurrentIndex(2);
    app.paintTool->tools_table->selectAll();
    app.paintTool->generate_paint_button->click();
    ASSERT_TRUE(app.picker()->active());
    app.paintTool->tools_table->selectAll();
    app.paintTool->deltool_btn->click();
    pickArea(app);
    EXPECT_EQ(app.collection()->objects().size(), objects);

    select(app, logo);
    app.ui()->isolation_btn->trigger();
    app.isolationTool->select_combo->setCurrentIndex(1);
    app.isolationTool->tools_table->selectAll();
    app.isolationTool->generate_iso_button->click();
    ASSERT_TRUE(app.picker()->active());
    app.isolationTool->tools_table->selectAll();
    app.isolationTool->deltool_btn->click();
    pickArea(app);
    EXPECT_EQ(app.collection()->objects().size(), objects);
}

TEST(GuiAudit, DrillingParametersFollowTheUnits) {
    // Con la tecla Q el G-code en pulgadas salía con las Z en mm (-1,7 pulgadas = 43 mm de bajada).
    auto cutZ = [](bool original) {
        OriginalBugsScope scope(original);
        App app;
        FlatCAMObj* exc = app.openExcellon(kLuz + "PTH.drl");
        select(app, exc);
        app.ui()->drill_btn->trigger();
        const double before = app.drillingTool->cutz_entry->value();
        app.setUnitsAutoAnswer(true);
        app.onToggleUnitsClick();
        app.drillingTool->tools_table->selectAll();
        auto* cnc = dynamic_cast<CNCJobObject*>(app.drillingTool->generateCnc());
        EXPECT_NE(cnc, nullptr);
        const std::string g = cnc ? cnc->job().gcode() : std::string();
        return std::make_tuple(before, app.drillingTool->cutz_entry->value(), g);
    };
    {
        const auto [before, after, g] = cutZ(true);
        EXPECT_DOUBLE_EQ(after, before);  // el original deja los mm
    }
    const auto [before, after, g] = cutZ(false);
    EXPECT_NEAR(after, before / 25.4, 1e-3);
    EXPECT_EQ(g.find("Z-1.7000"), std::string::npos) << g.substr(0, 800);
    EXPECT_NE(g.find("Z-0.0669"), std::string::npos) << g.substr(0, 800);
}

TEST(GuiAudit, MouseBitesWithZeroDiameterAndSpacingDoNotHang) {
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* edge = app.openByExtension(kLuz + "Edge_Cuts.gbr");
    select(app, edge);
    app.ui()->cutout_btn->trigger();
    CutOutTool* c = app.cutoutTool;
    c->gaptype_radio->setValue(QString("mb"));
    c->mb_dia_entry->setValue(0);
    c->mb_spacing_entry->setValue(0);
    EXPECT_EQ(c->freeformCutout(), nullptr);
    EXPECT_TRUE(info(app).contains(tr_("Mouse bites diameter and spacing can't both be zero.")));
    EXPECT_EQ(c->rectangularCutout(), nullptr);
}

TEST(GuiAudit, DeletingTheEditedObjectFromTclDoesNotClaimItWasSaved) {
    AutoAccept accept;
    App app;
    FlatCAMObj* logo = app.openByExtension(kLuz + "Logo.gbr");
    select(app, logo);
    app.objectToEditor();
    bool ok = false;
    app.shell()->eval("delete {" + logo->name() + "}", &ok);
    app.editorToObject(true);
    EXPECT_TRUE(info(app).contains(tr_("Editor exited. Editor content was not saved."))) << info(app).toStdString();
    EXPECT_TRUE(app.collection()->objects().empty());
}

TEST(GuiAudit, GCodeEditorAfterItsCncJobIsDeleted) {
    // Antes: memoria liberada (el editor guardaba un puntero crudo al CNC Job).
    AutoAccept accept;
    App app;
    FlatCAMObj* cnc = app.openGcode(kLuz + "PTH.drl_cnc.nc");
    ASSERT_NE(cnc, nullptr);
    select(app, cnc);
    app.objectToEditor();
    bool ok = false;
    app.shell()->eval("delete {" + cnc->name() + "}", &ok);
    app.gcodeEditor->onToggleAllRows();
    app.gcodeEditor->onRowSelectionChange();
    EXPECT_TRUE(app.collection()->objects().empty());

    // Nuevo proyecto con el editor abierto: sale del editor.
    FlatCAMObj* again = app.openGcode(kLuz + "PTH.drl_cnc.nc");
    select(app, again);
    app.objectToEditor();
    ASSERT_TRUE(app.editorActive());
    app.clearProject();
    EXPECT_FALSE(app.editorActive());
    app.gcodeEditor->onToggleAllRows();
}

TEST(GuiAudit, ExclusionAreaWhileItsGeometryIsDeleted) {
    // Antes: el área terminada leía la estrategia y la Z del panel de la Geometría borrada.
    AutoAccept accept;
    App app;
    for (int round = 0; round < 2; ++round) {
        FlatCAMObj* geo = app.newGeometryObject();
        select(app, geo);
        app.showProperties(geo);
        auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
        ASSERT_NE(ui, nullptr);
        ui->exclusion_cb->setChecked(true);
        ui->area_shape_radio->setValue("square");
        ui->add_area_button->click();
        ASSERT_TRUE(app.picker()->active());
        if (round == 0) {
            bool ok = false;
            app.shell()->eval("delete {" + geo->name() + "}", &ok);
            for (const QPointF& p : {QPointF(-200, -200), QPointF(200, 200)})
                app.picker()->onRelease(p, Qt::LeftButton, false);
            app.picker()->onRelease(QPointF(200, 200), Qt::RightButton, false);
            EXPECT_TRUE(app.excAreas()->areas().empty());
        } else {
            app.clearProject();  // Nuevo proyecto: la elección se cancela
            EXPECT_FALSE(app.picker()->active());
        }
    }
}

TEST(GuiAudit, OpenToolsFollowTheUnits) {
    // Con la tecla Q, NCC, Paint, Isolation y Corte se quedaban con los números en mm leídos como pulgadas.
    OriginalBugsScope fixed(false);
    App app;
    FlatCAMObj* logo = app.openByExtension(kLuz + "Logo.gbr");
    FlatCAMObj* edge = app.openByExtension(kLuz + "Edge_Cuts.gbr");
    select(app, logo);
    app.ui()->ncc_btn->trigger();
    app.ui()->paint_btn->trigger();
    app.ui()->isolation_btn->trigger();
    select(app, edge);
    app.ui()->cutout_btn->trigger();
    auto firstDia = [](const std::map<int, ClearingTool>& tools) { return tools.begin()->second.geo.tooldia; };
    ASSERT_FALSE(app.nccTool->tools().empty());
    const double ncc = firstDia(app.nccTool->tools()), paint = firstDia(app.paintTool->tools()),
                 iso = firstDia(app.isolationTool->tools()), cut = app.cutoutTool->dia->value(),
                 margin = app.nccTool->ncc_margin_entry->value();
    app.setUnitsAutoAnswer(true);
    app.onToggleUnitsClick();
    EXPECT_NEAR(firstDia(app.nccTool->tools()), ncc / 25.4, 1e-4);
    EXPECT_NEAR(firstDia(app.paintTool->tools()), paint / 25.4, 1e-4);
    EXPECT_NEAR(firstDia(app.isolationTool->tools()), iso / 25.4, 1e-4);
    EXPECT_NEAR(app.cutoutTool->dia->value(), cut / 25.4, 1e-4);
    EXPECT_NEAR(app.nccTool->ncc_margin_entry->value(), margin / 25.4, 1e-4);
    EXPECT_NE(app.cutoutTool->rectangularCutout(), nullptr) << info(app).toStdString();
    // Al volver a abrir una herramienta, sus valores de las preferencias también están en pulgadas.
    select(app, edge);
    app.ui()->cutout_btn->trigger();
    EXPECT_NEAR(app.cutoutTool->dia->value(), cut / 25.4, 1e-3);
    select(app, logo);
    app.ui()->ncc_btn->trigger();
    EXPECT_NEAR(firstDia(app.nccTool->tools()), ncc / 25.4, 1e-3);
}
