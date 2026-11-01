// Tests de la interfaz gráfica (Qt6). Corren sin pantalla (plataforma "offscreen").
#include <gtest/gtest.h>

#include <QApplication>
#include <QMenuBar>
#include <QSplitter>
#include <QToolBar>
#include <cmath>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FlatCAMInfoBar.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"
#include "gui/objects/ObjectUIs.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "tools/CutOut.h"
#include <QFile>
#include <QMimeData>
#include <QDropEvent>
#include "gui/widgets/KeySensitiveListView.h"
#include <QTemporaryDir>
#include <algorithm>

using namespace flatcam::gui;

namespace {

QStringList menuTitles(QMenuBar* bar) {
    QStringList out;
    for (QAction* a : bar->actions()) out << a->text();
    return out;
}

}  // namespace

TEST(GuiTranslation, UsesFlatCamSpanishTable) {
    EXPECT_EQ(tr_("Idle."), QString::fromUtf8("Ocioso."));
    EXPECT_EQ(tr_("File"), QString::fromUtf8("Archivo"));
    EXPECT_EQ(tr_("una frase que no está en la tabla"), QString::fromUtf8("una frase que no está en la tabla"));
    // Correcciones del usuario sobre la traducción de FlatCAM: "Open" es una acción.
    EXPECT_EQ(tr_("Open"), QString::fromUtf8("Abrir"));
    EXPECT_EQ(tr_("Open Project"), QString::fromUtf8("Abrir proyecto"));
}

TEST(GuiMainWindow, OpenMenuUsesTheVerb) {
    MainGUI w;
    EXPECT_EQ(w.menufile_open->title(), QString::fromUtf8("Abrir"));
    EXPECT_EQ(w.menufileopenproject->text(), QString::fromUtf8("Abrir proyecto...\tCtrl+O"));
    EXPECT_EQ(w.menufileopenexcellon->text(), QString::fromUtf8("Abrir Excellon...\tCtrl+E"));
    EXPECT_EQ(w.menufileopengerber->text(), QString::fromUtf8("Abrir Gerber...\tCtrl+G"));
}

TEST(GuiMainWindow, MenusToolbarsAndTabsLikeTheOriginal) {
    MainGUI w;
    // Menú principal: el de FlatCAM 2024.4 en español (con los de los editores, que el original oculta al no editar).
    const QStringList menus = menuTitles(w.menuBar());
    ASSERT_GE(menus.size(), 8);
    EXPECT_EQ(menus.mid(0, 8), (QStringList{"Panel de palanca", "Archivo", "Editar", "Opciones", "Ver", "Objetos",
                                            "Herramienta", "Ayuda"}));
    EXPECT_EQ(w.windowTitle(), QString::fromUtf8("FlatCAM 2024.4 BETA - 64bit - [3D]    Proyecto nuevo: no guardado"));
    // Diseño "minimal" de la primera ejecución: archivo, edición y herramientas visibles; vista, consola y editores no.
    EXPECT_FALSE(w.toolbarfile->isHidden());
    EXPECT_FALSE(w.toolbaredit->isHidden());
    EXPECT_FALSE(w.toolbartools->isHidden());
    EXPECT_TRUE(w.toolbarview->isHidden());
    EXPECT_TRUE(w.toolbarshell->isHidden());
    EXPECT_TRUE(w.geo_edit_toolbar->isHidden());
    EXPECT_EQ(w.toolbartools->actions().size(), 26);
    // Íconos: los de FlatCAM, desde los recursos.
    EXPECT_FALSE(w.menufilenewproject->icon().isNull());
    EXPECT_EQ(w.menufilenewproject->text(), QString::fromUtf8("Nuevo Proyecto...\tCtrl+N"));
    // Panel izquierdo: Proyecto, Propiedades y Herramienta; el área de dibujo protegida contra el cierre.
    ASSERT_EQ(w.notebook->count(), 3);
    EXPECT_EQ(w.notebook->tabText(0), "Proyecto");
    EXPECT_EQ(w.notebook->tabText(1), "Propiedades");
    EXPECT_EQ(w.notebook->tabText(2), "Herramienta");
    EXPECT_EQ(w.plot_tab_area->tabText(0), QString::fromUtf8("Área de la parcela"));
    EXPECT_FALSE(w.plot_tab_area->isClosable(0));
    // La consola Tcl empieza oculta y el panel izquierdo cerrado (global_shell_at_startup y global_project_at_startup).
    EXPECT_TRUE(w.shell_dock->isHidden());
    EXPECT_EQ(w.splitter->sizes().value(0), 0);
    w.onToggleNotebook();
    EXPECT_TRUE(w.menu_toggle_nb->isChecked());
}

TEST(GuiMainWindow, InformSetsTheStatusLevel) {
    MainGUI w;
    w.inform("[success] Hecho.");
    EXPECT_EQ(w.fcinfo->text->text(), QString::fromUtf8("Hecho."));
    w.inform("Sin nivel");
    EXPECT_EQ(w.fcinfo->text->text(), QString::fromUtf8("Sin nivel"));
}

TEST(GuiWidgets, EntryFormatsNumbersLikeFlatCam) {
    FCEntry e;
    e.setValue(1.5);
    EXPECT_EQ(e.text(), "1.5000");
    e.setDecimals(2);
    e.setValue(1.5);
    EXPECT_EQ(e.text(), "1.50");
    FCEntry2 e2;
    e2.setValue(0.1);
    EXPECT_EQ(e2.text(), "0.1000");
    EXPECT_EQ(e2.sizeHint().width(), FCEntry::kEditSizeHint);
}

TEST(GuiWidgets, DetachableTabProtectsAndCloses) {
    FCDetachableTab2 tabs;
    tabs.addTab(new QWidget, "a");
    tabs.addTab(new QWidget, "b");
    tabs.protectTab(0);
    EXPECT_FALSE(tabs.isClosable(0));
    EXPECT_TRUE(tabs.isClosable(1));
    int closed = -1;
    QObject::connect(&tabs, &FCDetachableTab2::tabClosed, [&](const QString&, int i) { closed = i; });
    tabs.closeTab(1);
    EXPECT_EQ(closed, 1);
    EXPECT_EQ(tabs.count(), 1);
}

TEST(GuiCanvas, TransformsAndZoomKeepTheCenterPoint) {
    PlotCanvas c;
    c.resize(800, 600);
    c.fitTo(QRectF(-25, -25, 150, 150));
    // El rectángulo inicial de FlatCAM entra entero y centrado.
    const QPointF center = c.toWorld(c.viewRect().center());
    EXPECT_NEAR(center.x(), 50, 1e-9);
    EXPECT_NEAR(center.y(), 50, 1e-9);
    // Ida y vuelta entre píxeles y milímetros; el Y crece hacia arriba.
    const QPointF p(123.5, 77.25);
    const QPointF back = c.toWorld(c.toPixel(p));
    EXPECT_NEAR(back.x(), p.x(), 1e-9);
    EXPECT_NEAR(back.y(), p.y(), 1e-9);
    EXPECT_LT(c.toPixel({0, 10}).y(), c.toPixel({0, 0}).y());
    // El zoom deja quieto el punto alrededor del cual se hace.
    const QPointF anchor(10, 20);
    const QPointF before = c.toPixel(anchor);
    c.zoom(1.5, anchor);
    const QPointF after = c.toPixel(anchor);
    EXPECT_NEAR(before.x(), after.x(), 1e-9);
    EXPECT_NEAR(before.y(), after.y(), 1e-9);
}

TEST(GuiObjects, FlatCamColorsAreRgbaHex) {
    const QColor c = colorFromFlatCam("#BBF268BF");  // gerber_plot_fill
    EXPECT_EQ(c.red(), 0xBB);
    EXPECT_EQ(c.green(), 0xF2);
    EXPECT_EQ(c.blue(), 0x68);
    EXPECT_EQ(c.alpha(), 0xBF);
    EXPECT_EQ(colorFromFlatCam("#FF0000").alpha(), 255);
}

TEST(GuiObjects, CollectionGroupsAndRenamesDuplicates) {
    ObjectCollection c;
    ASSERT_EQ(c.rowCount(), 6);
    EXPECT_EQ(c.data(c.index(2, 0)).toString(), QString::fromUtf8("Geometría"));
    EXPECT_EQ(c.data(c.index(4, 0)).toString(), QString::fromUtf8("Secuencia de Comandos"));
    EXPECT_FALSE(c.flags(c.index(0, 0)).testFlag(Qt::ItemIsSelectable));  // los grupos no se seleccionan
    auto make = [](const QString& n) {
        auto g = std::make_unique<flatcam::Gerber>();
        g->parseFile(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Edge_Cuts.gm1");
        return std::make_unique<GerberObject>(n, std::move(g));
    };
    // Mismo nombre: "x" -> "x_1"; termina en número: se incrementa (ObjectCollection.append).
    EXPECT_EQ(c.append(make("borde"))->name(), "borde");
    EXPECT_EQ(c.append(make("borde"))->name(), "borde_1");
    EXPECT_EQ(c.append(make("borde"))->name(), "borde_2");
    EXPECT_EQ(c.append(make("capa9"))->name(), "capa9");
    EXPECT_EQ(c.append(make("capa9"))->name(), "capa10");
    EXPECT_EQ(c.rowCount(c.groupIndex(ObjectKind::Gerber)), 5);
    FlatCAMObj* o = c.byName("borde_1");
    ASSERT_NE(o, nullptr);
    EXPECT_EQ(o->kind(), ObjectKind::Gerber);
    c.remove(o);
    EXPECT_EQ(c.byName("borde_1"), nullptr);
    EXPECT_EQ(c.rowCount(c.groupIndex(ObjectKind::Gerber)), 4);
}

TEST(GuiApp, OpensUserFilesPlotsAndFits) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    FlatCAMObj* g = app.openByExtension(QString::fromStdString(dir + "B_Cu.gbr"));
    FlatCAMObj* e = app.openByExtension(QString::fromStdString(dir + "PTH.drl"));
    ASSERT_NE(g, nullptr);
    ASSERT_NE(e, nullptr);
    EXPECT_EQ(g->kind(), ObjectKind::Gerber);
    EXPECT_EQ(e->kind(), ObjectKind::Excellon);
    EXPECT_EQ(app.openByExtension("algo.xyz"), nullptr);
    // Lo dibujado tiene relleno y contorno, con los colores de FlatCAM.
    const PlotLayerData d = g->plotData();
    EXPECT_GT(d.fill.size(), 1000u);
    EXPECT_EQ(d.fill.size() % 6, 0u);  // triángulos de 3 vértices x/y
    EXPECT_GT(d.lines.size(), 1000u);
    EXPECT_EQ(d.fillColor, colorFromFlatCam("#BBF268BF"));
    EXPECT_EQ(e->plotData().fillColor, colorFromFlatCam("#C40000BF"));
    // Los límites de lo dibujado son los del Gerber; el encuadre muestra toda la placa.
    const QRectF b = g->bounds();
    EXPECT_NEAR(d.bounds.left(), b.left(), 1e-6);
    EXPECT_NEAR(d.bounds.right(), b.right(), 1e-6);
    PlotCanvas* c = app.ui()->plotCanvas();
    c->resize(800, 600);
    app.onZoomFit();
    const QRectF all = c->layersBounds().value();
    EXPECT_TRUE(c->viewRect().contains(c->toPixel(all.topLeft())));
    EXPECT_TRUE(c->viewRect().contains(c->toPixel(all.bottomRight())));
    // El panel izquierdo se abre al agregar un objeto (global_project_autohide).
    EXPECT_GT(app.ui()->splitter->sizes().value(0), 0);
    EXPECT_EQ(app.collection()->names(), (QStringList{"luz de noche Pascual-B_Cu.gbr", "luz de noche Pascual-PTH.drl"}));
}

TEST(GuiWidgets, NumericExpressionsLikePythonEval) {
    EXPECT_DOUBLE_EQ(*NumericExpression::evaluate("1.5"), 1.5);
    EXPECT_DOUBLE_EQ(*NumericExpression::evaluate("2*3+1"), 7.0);
    EXPECT_DOUBLE_EQ(*NumericExpression::evaluate("-(1+2)/4"), -0.75);
    EXPECT_DOUBLE_EQ(*NumericExpression::evaluate("7 % 3"), 1.0);
    EXPECT_DOUBLE_EQ(*NumericExpression::evaluate("-7 % 3"), 2.0);  // como Python
    EXPECT_FALSE(NumericExpression::evaluate("1/0"));
    EXPECT_FALSE(NumericExpression::evaluate("2+"));
    EXPECT_FALSE(NumericExpression::evaluate(""));
    const auto t = NumericExpression::evaluateTuple("(0.0, 2*1.5)");
    ASSERT_TRUE(t);
    EXPECT_EQ(*t, (std::vector<double>{0.0, 3.0}));
    EXPECT_FALSE(NumericExpression::evaluateTuple("(1, x)"));
}

TEST(GuiWidgets, RadioSetValues) {
    RadioSet r({{"No", QVariant("no")}, {"Adelante", QVariant("fwd")}, {"Atras", QVariant("rev")}});
    EXPECT_FALSE(r.value().isValid());
    QString last;
    QObject::connect(&r, &RadioSet::activatedCustom, [&](const QString& v) { last = v; });
    r.setValue(QVariant("rev"));
    EXPECT_EQ(r.value().toString(), "rev");
    EXPECT_EQ(last, "rev");
    EXPECT_TRUE(r.button(2)->isChecked());
    r.setOptionsDisabled({"No"}, true);
    EXPECT_FALSE(r.button(0)->isEnabled());
}

TEST(GuiApp, PropertiesAndToolPanelsLikeTheOriginal) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    FlatCAMObj* e = app.openByExtension(QString::fromStdString(dir + "PTH.drl"));
    ASSERT_NE(e, nullptr);
    // Al elegir el objeto, Propiedades muestra su panel con la tabla de herramientas y los totales.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(e));
    auto* ui = qobject_cast<ExcellonObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    EXPECT_EQ(ui->name_entry->text(), "luz de noche Pascual-PTH.drl");
    ASSERT_EQ(ui->tools_table->rowCount(), 6);
    EXPECT_EQ(ui->tools_table->item(0, 1)->text(), "0.8000");
    EXPECT_EQ(ui->tools_table->item(0, 2)->text(), "84");
    EXPECT_EQ(ui->tools_table->item(3, 1)->text(), "3.4000");
    EXPECT_EQ(ui->tools_table->item(4, 1)->text(), QString::fromUtf8("Taladros totales"));
    EXPECT_EQ(ui->tools_table->item(4, 2)->text(), "120");
    // Sin selección vuelve el panel vacío.
    app.showProperties(nullptr);
    EXPECT_EQ(qobject_cast<ExcellonObjectUI*>(app.ui()->properties_scroll_area->widget()), nullptr);
    // Las herramientas se abren en la pestaña Herramienta con su título; elegir la misma otra vez cierra el panel.
    app.runTool(app.nccTool, NccTool::info());
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), app.nccTool);
    EXPECT_EQ(app.ui()->notebook->tabText(2), QString::fromUtf8("Herramienta NCC"));
    EXPECT_EQ(app.ui()->notebook->currentWidget(), app.ui()->tool_tab);
    app.runTool(app.drillingTool, DrillingTool::info());
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), app.drillingTool);
    EXPECT_EQ(app.ui()->notebook->tabText(2), QString::fromUtf8("Herramienta de Perforación"));
    app.runTool(app.drillingTool, DrillingTool::info());
    EXPECT_EQ(app.ui()->splitter->sizes().value(0), 0);
}

TEST(GuiDrilling, UserWorkflowReproducesTheRealFile) {
    // Como en las capturas del usuario: PTH.drl reflejado, Perforación con todas las herramientas, Corte Z -2,4 (la de
    // 3,4 mm: -3,4), avance Z 10, cambio de herramienta en Z 19, Z final 19, X,Y final 0,0, velocidad 10000.
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(exc, nullptr);
    exc->excellon().mirror(flatcam::Axis::Y, {0, 0});
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    DrillingTool* t = app.drillingTool;
    app.runTool(t, DrillingTool::info());
    t->setToolUi();
    EXPECT_EQ(t->object_combo->currentText(), exc->name());
    ASSERT_EQ(t->tools_table->rowCount(), 6);
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "0.8");
    EXPECT_EQ(t->tools_table->item(4, 2)->text(), "120");
    EXPECT_TRUE(t->tool_data_label->text().contains(QString::fromUtf8("Herramientas múltiples")));
    // Todas elegidas (como deja build_tool_ui): parámetros para todas.
    t->cutz_entry->setValue(-2.4);
    t->feedrate_z_entry->setValue(10.0);
    t->spindlespeed_entry->setValue(10000);
    t->toolchange_cb->setChecked(true);
    t->toolchangez_entry->setValue(19.0);
    t->endz_entry->setValue(19.0);
    t->endxy_entry->setText("0,0");
    Q_EMIT t->endxy_entry->editingFinished();
    // Solo la de 3,4 mm: -3,4.
    t->tools_table->clearSelection();
    t->tools_table->selectRow(3);
    EXPECT_TRUE(t->tool_data_label->text().contains("4"));
    t->cutz_entry->setValue(-3.4);
    t->tools_table->clearSelection();
    t->tools_table->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int r = 0; r < 4; ++r) t->tools_table->selectRow(r);
    auto* cnc = static_cast<CNCJobObject*>(t->generateCnc());
    ASSERT_NE(cnc, nullptr);
    EXPECT_EQ(cnc->name(), "luz de noche Pascual-PTH.drl_cnc");
    EXPECT_EQ(cnc->kind(), ObjectKind::CNCJob);

    // Exportar con el nombre del archivo real: mismas líneas salvo la fecha y el orden de los agujeros (OR-Tools).
    QTemporaryDir tmp;
    const QString out = tmp.filePath("luz de noche Pascual-PTH.drl_cnc.nc");
    ASSERT_TRUE(app.exportGcode(cnc, out));
    auto readLines = [](const QString& path) {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return QStringList();
        QStringList l = QString::fromUtf8(f.readAll()).split('\n');
        return l;
    };
    QStringList mine = readLines(out), real = readLines(QString::fromStdString(dir + "PTH.drl_cnc.nc"));
    // En el proyecto del usuario la Z de corte quedó como resultado de una cuenta en coma flotante (-1,7 - 0,7); desde
    // el campo (4 decimales) se obtiene -2,4 exacto. Es el mismo número a menos de 1e-15.
    real.replaceInStrings("-2.4000000000000004", "-2.4");
    real.replaceInStrings("-3.400000000000001", "-3.4");
    ASSERT_EQ(mine.size(), real.size());
    QStringList fixedMine, fixedReal, holesMine, holesReal;
    auto split = [](const QStringList& L, QStringList& fixed, QStringList& holes) {
        for (int i = 0; i < L.size(); ++i) {
            if (L[i].startsWith("(Created on")) continue;
            if (L[i].startsWith("G00 X") && i + 1 < L.size() && L[i + 1].startsWith("G01 Z-")) {
                holes << L[i] + "|" + L[i + 1];
                ++i;
            } else {
                fixed << L[i];
            }
        }
        holes.sort();
    };
    split(mine, fixedMine, holesMine);
    split(real, fixedReal, holesReal);
    EXPECT_EQ(holesMine.size(), 120);
    EXPECT_EQ(holesMine, holesReal);
    EXPECT_EQ(fixedMine, fixedReal);
}

int main(int argc, char** argv) {
    // Sin pantalla salvo que se pida otra plataforma (QT_QPA_PLATFORM=cocoa para mirar el canvas OpenGL).
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(flatcam);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// Agregado pedido por el usuario: los objetos de un grupo se reordenan arrastrándolos y el orden del árbol es el de dibujo.
TEST(GuiApp, ReorderingTheProjectTreeChangesTheDrawOrder) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    FlatCAMObj* cu = app.openByExtension(QString::fromStdString(dir + "B_Cu.gbr"));
    FlatCAMObj* edge = app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr"));
    FlatCAMObj* logo = app.openByExtension(QString::fromStdString(dir + "Logo.gbr"));
    FlatCAMObj* drl = app.openByExtension(QString::fromStdString(dir + "PTH.drl"));
    ASSERT_TRUE(cu && edge && logo && drl);
    ObjectCollection* col = app.collection();
    PlotCanvas* c = app.ui()->plotCanvas();
    // El primero del grupo queda arriba (se dibuja último dentro del grupo); el Excellon, sobre los Gerber.
    EXPECT_EQ(c->layerOrder(), (std::vector<int>{logo->id(), edge->id(), cu->id(), drl->id()}));

    // Arrastrar el Logo arriba de todo (soltarlo sobre B_Cu lo pone en su lugar).
    const QModelIndexList drag{col->indexOf(logo)};
    std::unique_ptr<QMimeData> m(col->mimeData(drag));
    ASSERT_TRUE(m);
    EXPECT_TRUE(col->canDropMimeData(m.get(), Qt::MoveAction, -1, -1, col->indexOf(cu)));
    col->dropMimeData(m.get(), Qt::MoveAction, -1, -1, col->indexOf(cu));
    EXPECT_EQ(col->names().mid(0, 3), (QStringList{"luz de noche Pascual-Logo.gbr", "luz de noche Pascual-B_Cu.gbr",
                                                   "luz de noche Pascual-Edge_Cuts.gbr"}));
    EXPECT_EQ(c->layerOrder(), (std::vector<int>{edge->id(), cu->id(), logo->id(), drl->id()}));

    // Entre filas: B_Cu al final del grupo (fila 3).
    m.reset(col->mimeData({col->indexOf(cu)}));
    col->dropMimeData(m.get(), Qt::MoveAction, 3, 0, col->groupIndex(ObjectKind::Gerber));
    EXPECT_EQ(col->indexOf(cu).row(), 2);
    EXPECT_EQ(c->layerOrder(), (std::vector<int>{cu->id(), edge->id(), logo->id(), drl->id()}));

    // No se puede pasar un objeto a otro grupo.
    m.reset(col->mimeData({col->indexOf(drl)}));
    EXPECT_FALSE(col->canDropMimeData(m.get(), Qt::MoveAction, -1, -1, col->indexOf(cu)));
    EXPECT_FALSE(col->canDropMimeData(m.get(), Qt::MoveAction, 0, 0, col->groupIndex(ObjectKind::Gerber)));
    EXPECT_FALSE(col->moveObject(cu, 2));  // ya está ahí
    EXPECT_EQ(col->indexOf(drl).parent(), col->groupIndex(ObjectKind::Excellon));
    // Con el mouse en la vista (Logo, Edge_Cuts, B_Cu): soltar Edge_Cuts en el borde de arriba de la fila de Logo lo
    // pone primero.
    KeySensitiveListView* view = app.ui()->collection_view;
    EXPECT_TRUE(view->dragEnabled());
    app.ui()->show();
    view->expandAll();
    QApplication::processEvents();
    const QRect r = view->visualRect(col->indexOf(logo));
    ASSERT_FALSE(r.isEmpty());
    m.reset(col->mimeData({col->indexOf(edge)}));
    const QPointF pos(r.center().x(), r.top() + 1);
    QDragEnterEvent enter(pos, Qt::MoveAction, m.get(), Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &enter);
    QDragMoveEvent mv(pos, Qt::MoveAction, m.get(), Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &mv);
    EXPECT_TRUE(mv.isAccepted());
    QDropEvent drop(pos, Qt::MoveAction, m.get(), Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(view->viewport(), &drop);
    EXPECT_EQ(col->indexOf(edge).row(), 0);
    EXPECT_EQ(c->layerOrder(), (std::vector<int>{cu->id(), logo->id(), edge->id(), drl->id()}));
}

TEST(GuiCutout, UserWorkflowReproducesTheRealFile) {
    // Como lo hizo el usuario con su 2024.4: Edge_Cuts reflejado -> Corte libre (fresa 1,2, margen 0, 4 puentes de 0,6,
    // Z -1,8 de una pasada: su archivo no tiene profundidad múltiple) -> Geometría (avance 20, husillo 10000, Z de cambio y final 19, X,Y final 0,0) -> CNC Job.
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    edge->gerber().mirror(flatcam::Axis::Y, {0, 0});
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    CutOutTool* t = app.cutoutTool;
    app.ui()->cutout_btn->trigger();
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), t);
    EXPECT_EQ(t->obj_combo->currentText(), edge->name());
    // Valores de fábrica del panel (tools_cutout_*).
    EXPECT_DOUBLE_EQ(t->dia->value(), 2.4);
    EXPECT_EQ(t->gaps->currentText(), "4");
    EXPECT_DOUBLE_EQ(t->cutz_entry->value(), -1.8);
    EXPECT_TRUE(t->mpass_cb->isChecked());
    t->dia->setValue(1.2);
    t->margin->setValue(0.0);
    t->gapsize->setValue(0.6);
    t->mpass_cb->setChecked(false);
    t->ff_cutout_object_btn->click();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_cutout"));
    ASSERT_NE(geo, nullptr);
    ASSERT_EQ(geo->tools().size(), 1u);
    EXPECT_DOUBLE_EQ(geo->tools().at(1).data.cutZ, -1.8);
    EXPECT_EQ(geo->plotData().lineColor, colorFromFlatCam("#FFAAAA"));

    // Propiedades de la Geometría: tabla con la herramienta y sus parámetros en el formulario.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(geo));
    auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    ASSERT_EQ(ui->geo_tools_table->rowCount(), 1);
    EXPECT_EQ(ui->geo_tools_table->item(0, 1)->text(), "1.2000");
    EXPECT_TRUE(ui->geo_tools_table->isColumnHidden(2));  // nivel Básico
    EXPECT_TRUE(ui->tool_data_label->text().contains(QString::fromUtf8("Herramienta 1")));
    EXPECT_DOUBLE_EQ(ui->cutz_entry->value(), -1.8);
    EXPECT_DOUBLE_EQ(ui->cncfeedrate_entry->value(), 120.0);  // geometry_feedrate de fábrica
    ui->cncfeedrate_entry->setValue(20.0);
    ui->cncspindlespeed_entry->setValue(10000);
    ui->toolchangez_entry->setValue(19.0);
    ui->endz_entry->setValue(19.0);
    ui->endxy_entry->setText("0,0");
    Q_EMIT ui->endxy_entry->editingFinished();
    EXPECT_DOUBLE_EQ(geo->tools().at(1).data.feedrate, 20.0);
    ui->generate_cnc_button->click();
    auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc"));
    ASSERT_NE(cnc, nullptr);
    EXPECT_GT(cnc->plotData().fill.size(), 100u);  // la trayectoria se dibuja engrosada con el radio de la fresa

    QTemporaryDir tmp;
    const QString out = tmp.filePath("luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc.nc");
    ASSERT_TRUE(app.exportGcode(cnc, out));
    auto readLines = [](const QString& path) {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return QStringList();
        return QString::fromUtf8(f.readAll()).split('\n');
    };
    QStringList mine = readLines(out), real = readLines(QString::fromStdString(dir + "Edge_Cuts.gbr_cutout_cnc.nc"));
    ASSERT_GT(real.size(), 100);
    // Todo igual salvo la fecha.
    auto noDate = [](QStringList l) {
        for (QString& s : l)
            if (s.startsWith("(Created on ")) s = "(Created on)";
        return l;
    };
    const QStringList a = noDate(mine), b = noDate(real);
    EXPECT_EQ(a.size(), b.size());
    for (int i = 0; i < std::min(a.size(), b.size()); ++i) {
        if (a[i] != b[i]) {
            ADD_FAILURE() << "línea " << i + 1 << ": '" << a[i].toStdString() << "' != '" << b[i].toStdString() << "'";
            break;
        }
    }
}


TEST(GuiCutout, ThinGapsAndMouseBites) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    app.ui()->cutout_btn->trigger();
    CutOutTool* t = app.cutoutTool;
    t->dia->setValue(1.2);
    t->margin->setValue(0.0);
    t->gapsize->setValue(0.6);

    // Puentes delgados: herramienta 9999 con la Z de los puentes y su color propio.
    t->gaptype_radio->setValue("bt");
    t->thin_depth_entry->setValue(-1.0);
    auto* geo = dynamic_cast<GeometryObject*>(t->freeformCutout());
    ASSERT_NE(geo, nullptr);
    ASSERT_EQ(geo->tools().size(), 2u);
    EXPECT_DOUBLE_EQ(geo->tools().at(9999).data.cutZ, -1.0);
    const PlotLayerData pd = geo->plotData();
    ASSERT_EQ(pd.groups.size(), 1u);
    EXPECT_EQ(pd.groups[0].lineColor, colorFromFlatCam("#29a3a3fa"));
    EXPECT_FALSE(pd.groups[0].lines.empty());
    // Las dos herramientas en un CNC Job: cada una con su cambio de herramienta (T1 y T9999) y un solo cierre.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(geo));
    auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    ASSERT_EQ(ui->geo_tools_table->rowCount(), 2);
    ui->geo_tools_table->selectAll();
    EXPECT_EQ(ui->toolsForCnc(), (std::vector<int>{1, 9999}));
    EXPECT_TRUE(ui->tool_data_label->text().contains(QString::fromUtf8("Herramientas múltiples")));
    ui->generate_cnc_button->click();
    auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc"));
    ASSERT_NE(cnc, nullptr);
    EXPECT_EQ(cnc->tools().size(), 2u);
    const QString text = QString::fromStdString(cnc->exportText());
    EXPECT_EQ(text.count("\nT1\n"), 1);
    EXPECT_EQ(text.count("\nT9999\n"), 1);
    // `default` apaga el husillo en el cambio de cada herramienta ("M5", 2 veces) y una sola vez al final ("M05").
    EXPECT_EQ(text.count("\nM5\n"), 2);
    EXPECT_EQ(text.count("\nM05\n"), 1);
    EXPECT_EQ(cnc->job().toolGcodes().size(), 2u);
    // Los viajes van en un grupo amarillo translúcido, engrosados.
    const PlotLayerData cd = cnc->plotData();
    ASSERT_FALSE(cd.groups.empty());
    EXPECT_EQ(cd.groups.back().fillColor, colorFromFlatCam("#F0E24D4C"));
    EXPECT_GT(cd.groups.back().fill.size(), 100u);

    // Mouse bites: además de la Geometría, un Excellon con los agujeros de los puentes.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    t->setToolUi();
    t->dia->setValue(1.2);
    t->margin->setValue(0.0);
    t->gapsize->setValue(0.6);
    t->gaptype_radio->setValue("mb");
    ASSERT_NE(t->freeformCutout(), nullptr);
    auto* bites = dynamic_cast<ExcellonObject*>(app.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_mouse_bites"));
    ASSERT_NE(bites, nullptr);
    ASSERT_EQ(bites->excellon().drillTools().size(), 1u);
    const flatcam::ExcellonTool& holes = bites->excellon().drillTools().at(1);
    EXPECT_DOUBLE_EQ(holes.tooldia, 0.6);  // tools_cutout_mb_dia
    // Los mismos agujeros que calcula el núcleo con las mismas opciones.
    flatcam::CutoutOptions o;
    o.toolDia = 1.2;
    o.margin = 0.0;
    o.gaps = flatcam::CutoutGaps::Four;
    o.gapSize = 0.6;
    o.mouseBites = true;
    o.mbDia = 0.6;
    o.mbSpacing = 0.3;
    flatcam::CutoutResult r;
    std::string err;
    ASSERT_TRUE(flatcam::CutOut::freeform(edge->gerber().solidGeometry(), true, o, r, err)) << err;
    ASSERT_FALSE(r.mouseBiteHoles.empty());
    EXPECT_EQ(holes.drills.size(), r.mouseBiteHoles.size());
}

namespace {

QStringList readTextLines(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return {};
    return QString::fromUtf8(f.readAll()).split('\n');
}

/// Líneas de G-code sin la fecha, sin líneas vacías y sin lo que cambia entre la variante 8.994 (la del port) y el
/// ejecutable 2024.4 en el láser: `G00 Z0` al principio y `G00 Z..` al final.
QStringList comparableLaser(QStringList l) {
    QStringList out;
    for (QString s : l) {
        if (s.startsWith("(Created on ")) continue;
        if (s.trimmed().isEmpty() || s == "G00 Z0" || s.startsWith("G00 Z15") || s.startsWith("G00 Z19")) continue;
        if (s == "G21f") s = "G21";  // errata que el usuario escribió a mano en su archivo del NCC
        out << s;
    }
    return out;
}

/// Genera el CNC Job de láser de una Geometría desde su panel, como el usuario: GRBL_laser, avance 500, potencia 300.
CNCJobObject* laserJob(App& app, GeometryObject* geo, bool endAtOrigin) {
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(geo));
    auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
    if (!ui) return nullptr;
    ui->pp_geometry_name_cb->setValue("GRBL_laser");
    ui->cncfeedrate_entry->setValue(500.0);
    ui->cncspindlespeed_entry->setValue(300);
    if (endAtOrigin) {
        ui->endxy_entry->setText("0,0");
        Q_EMIT ui->endxy_entry->editingFinished();
    }
    ui->generate_cnc_button->click();
    return dynamic_cast<CNCJobObject*>(app.collection()->byName(geo->name() + "_cnc"));
}

}  // namespace

TEST(GuiPaint, UserWorkflowMatchesTheRealFile) {
    // Logo reflejado -> Paint (Gerber, herramienta 0,1, solape 80 %, estándar, "Todos") -> Geometría -> láser.
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* logo = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    ASSERT_NE(logo, nullptr);
    logo->gerber().mirror(flatcam::Axis::Y, {0, 0});
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    PaintTool* t = app.paintTool;
    app.ui()->paint_btn->trigger();
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), t);
    EXPECT_EQ(t->type_obj_radio->value().toString(), "gerber");
    EXPECT_EQ(t->obj_combo->currentText(), logo->name());
    // De fábrica: una herramienta de 0,3 con solape 20 %.
    ASSERT_EQ(t->tools_table->rowCount(), 1);
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "0.3000");
    EXPECT_DOUBLE_EQ(t->paintoverlap_entry->value(), 20.0);
    EXPECT_TRUE(t->tool_data_label->text().contains("Herramienta 1"));
    // Como el usuario: quitar la de 0,3, agregar la de 0,1 y ponerle 80 %.
    t->deltool_btn->click();
    EXPECT_EQ(t->tools_table->rowCount(), 0);
    t->new_tooldia_entry->setValue(0.1);
    t->add_newtool_button->click();
    ASSERT_EQ(t->tools_table->rowCount(), 1);
    t->paintoverlap_entry->setValue(80.0);
    EXPECT_DOUBLE_EQ(t->tools().begin()->second.overlap, 80.0);
    t->generate_paint_button->click();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_mt_paint"));
    ASSERT_NE(geo, nullptr);
    CNCJobObject* cnc = laserJob(app, geo, false);
    ASSERT_NE(cnc, nullptr);

    QTemporaryDir tmp;
    const QString out = tmp.filePath("luz de noche Pascual-Logo.gbr_mt_paint_cnc.nc");
    ASSERT_TRUE(app.exportGcode(cnc, out));
    const QStringList mine = comparableLaser(readTextLines(out));
    const QStringList real = comparableLaser(readTextLines(QString::fromStdString(dir + "Logo.gbr_mt_paint_cnc.nc")));
    ASSERT_EQ(mine.size(), real.size());
    // Con GEOS 3.11 es idéntico; con 3.12 y 3.15 cambian 17 líneas en las últimas cifras (test del núcleo).
    int differing = 0;
    for (int i = 0; i < real.size(); ++i) differing += mine[i] != real[i];
    EXPECT_LE(differing, real.size() / 200);
    EXPECT_EQ(mine.mid(0, 30), real.mid(0, 30));  // cabecera y comienzo
}

TEST(GuiNcc, UserWorkflowMatchesTheRealFile) {
    // B_Cu reflejado -> NCC (Gerber, herramienta 0,1, solape 80 %, margen 0, estándar, "Sí mismo") -> láser con X,Y
    // final 0,0. El archivo real está editado a mano por el usuario (variante 8.994, la del port).
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* cu = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "B_Cu.gbr")));
    ASSERT_NE(cu, nullptr);
    cu->gerber().mirror(flatcam::Axis::Y, {0, 0});
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(cu));
    NccTool* t = app.nccTool;
    app.ui()->ncc_btn->trigger();
    EXPECT_EQ(t->object_combo->currentText(), cu->name());
    // De fábrica: 1,0 y 0,5 (orden "Atrás": la más grande primero), 40 %, margen 1, método "Semilla".
    ASSERT_EQ(t->tools_table->rowCount(), 2);
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "1.0000");
    EXPECT_EQ(t->tools_table->item(1, 1)->text(), "0.5000");
    EXPECT_EQ(t->ncc_method_combo->indexValue(), 1);
    EXPECT_TRUE(t->tool_data_label->text().contains("Herramienta 2"));  // la última agregada queda elegida
    // Como el usuario: solo la de 0,1 con 80 %, margen 0 y método estándar.
    t->tools_table->selectAll();
    t->deltool_btn->click();
    t->new_tooldia_entry->setValue(0.1);
    t->add_newtool_button->click();
    ASSERT_EQ(t->tools_table->rowCount(), 1);
    t->ncc_overlap_entry->setValue(80.0);
    t->ncc_margin_entry->setValue(0.0);
    t->ncc_method_combo->setIndexValue(0);
    t->generate_ncc_button->click();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-B_Cu.gbr_ncc"));
    ASSERT_NE(geo, nullptr);
    CNCJobObject* cnc = laserJob(app, geo, true);
    ASSERT_NE(cnc, nullptr);

    QTemporaryDir tmp;
    const QString out = tmp.filePath("luz de noche Pascual-B_Cu.gbr_ncc_cnc.nc");
    ASSERT_TRUE(app.exportGcode(cnc, out));
    const QStringList mineAll = readTextLines(out);
    const QStringList realAll = readTextLines(QString::fromStdString(dir + "B_Cu.gbr_ncc_cnc.nc"));
    const QStringList mine = comparableLaser(mineAll), real = comparableLaser(realAll);
    EXPECT_EQ(mine.mid(0, 25), real.mid(0, 25));  // cabecera y comienzo
    // Cantidad de trazados: 272 en el real; empates de rtree y GEOS cambian unos pocos (test del núcleo).
    EXPECT_NEAR(static_cast<double>(mine.count("M03 S300.0")), static_cast<double>(real.count("M03 S300.0")), 272 * 0.02);
    EXPECT_NEAR(static_cast<double>(mine.size()), static_cast<double>(real.size()), real.size() * 0.003);
}

TEST(GuiApp, FlipSelectedObjectsAroundTheirCommonCenter) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    QStringList names;
    for (const char* f : {"Edge_Cuts.gbr", "B_Cu.gbr", "Logo.gbr", "PTH.drl"})
        ASSERT_NE(app.openByExtension(QString::fromStdString(dir + f)), nullptr);
    auto* cu = static_cast<GerberObject*>(app.collection()->byName("luz de noche Pascual-B_Cu.gbr"));
    auto* drl = static_cast<ExcellonObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl"));
    // Lo esperado: el reflejo en x = 0 que dan los .nc del usuario.
    flatcam::Gerber refCu;
    ASSERT_TRUE(refCu.parseFile(dir + "B_Cu.gbr"));
    refCu.mirror(flatcam::Axis::Y, {0, 0});
    flatcam::Excellon refDrl;
    ASSERT_TRUE(refDrl.parseFile(dir + "PTH.drl"));
    refDrl.mirror(flatcam::Axis::Y, {0, 0});

    // Sin nada elegido: aviso.
    app.ui()->collection_view->clearSelection();
    EXPECT_FALSE(app.flip(flatcam::Axis::Y));
    EXPECT_EQ(app.ui()->fcinfo->text->text(), QString::fromUtf8("No se selecciona ningún objeto."));

    // Las cuatro elegidas y la tecla X en el árbol del Proyecto (on_flipx): el contorno es simétrico, así que el
    // centro común es x = 0.
    app.ui()->collection_view->expandAll();
    app.ui()->collection_view->selectAll();
    EXPECT_EQ(app.selectedObjects().size(), 4u);
    Q_EMIT app.ui()->collection_view->keyPressed(Qt::Key_X);
    const flatcam::BoundingBox a = cu->gerber().bounds(), b = refCu.bounds();
    EXPECT_NEAR(a.xmin, b.xmin, 1e-9);
    EXPECT_NEAR(a.xmax, b.xmax, 1e-9);
    EXPECT_NEAR(a.ymin, b.ymin, 1e-9);
    EXPECT_NEAR(a.ymax, b.ymax, 1e-9);
    const auto& mine = drl->excellon().drillTools();
    const auto& ref = refDrl.drillTools();
    ASSERT_EQ(mine.size(), ref.size());
    for (const auto& [id, t] : ref) {
        ASSERT_EQ(mine.at(id).drills.size(), t.drills.size());
        for (std::size_t i = 0; i < t.drills.size(); ++i) {
            EXPECT_NEAR(mine.at(id).drills[i].x, t.drills[i].x, 1e-9);
            EXPECT_NEAR(mine.at(id).drills[i].y, t.drills[i].y, 1e-9);
        }
    }
    // Lo dibujado también se reflejó.
    EXPECT_NEAR(drl->plotData().bounds.left(), drl->bounds().left(), 1e-6);

    // Un solo objeto se refleja alrededor de su propio centro: con Y (on_flipy) se invierten las Y y la caja no se mueve.
    app.ui()->collection_view->clearSelection();
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(cu));
    const flatcam::BoundingBox before = cu->gerber().bounds();
    app.ui()->menuoptions_transform_flipy->trigger();
    const flatcam::BoundingBox after = cu->gerber().bounds();
    EXPECT_NEAR(after.ymin, before.ymin, 1e-9);
    EXPECT_NEAR(after.ymax, before.ymax, 1e-9);
}
