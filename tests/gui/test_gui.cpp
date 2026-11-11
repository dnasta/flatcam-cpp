// Tests de la interfaz gráfica (Qt6). Corren sin pantalla (plataforma "offscreen").
#include <gtest/gtest.h>

#include <QStandardItemModel>

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
#include "gui/ProjectIO.h"
#include "gui/CanvasPicker.h"
#include "gui/ExclusionAreasManager.h"
#include "gui/ToolsDBTab.h"
#include "gui/ToolsDatabase.h"
#include "tools/CutOut.h"
#include "tools/DoubleSided.h"
#include "tools/Isolation.h"
#include "tools/Ncc.h"
#include "tools/Paint.h"
#include <QFile>
#include <QMimeData>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QDropEvent>
#include "gui/widgets/KeySensitiveListView.h"
#include <QTemporaryDir>
#include <QTimer>
#include <QImage>
#include <QCloseEvent>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <set>
#include <QMenu>
#include <QMessageBox>
#include "gui/widgets/DialogBoxRadio.h"
#include <QClipboard>
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
    // Los recientes y demás datos de la aplicación van a una carpeta de prueba, no a la del usuario.
    QStandardPaths::setTestModeEnabled(true);
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
    // Propiedades: distancia recorrida y duración estimada (travel_distance, routing_time del CNC Job).
    {
        auto* cui = dynamic_cast<CNCObjectUI*>(cnc->propertiesUi());
        ASSERT_NE(cui, nullptr);
        EXPECT_FALSE(cui->t_distance_entry->isHidden());
        EXPECT_GT(cnc->job().travelDistance(), 0.0);
        EXPECT_EQ(cui->t_distance_entry->text(), QString::asprintf("%.4f", cnc->job().travelDistance()));
        EXPECT_EQ(cui->units_label->text(), "mm");
        const double r = cnc->job().routingTime();
        EXPECT_EQ(cui->units_time_label->text(), r > 1 ? "min" : "sec");
    }

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

TEST(GuiDblSided, MirrorAndAlignmentDrills) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* cu = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "B_Cu.gbr")));
    auto* drl = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_TRUE(edge && cu && drl);
    DblSidedTool* t = app.dblsidedTool;
    app.ui()->dblsided_btn->trigger();
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), t);
    // De fábrica: eje X, ubicación "Punto", diámetro 3,125; el combo muestra los Gerber.
    EXPECT_EQ(t->mirror_axis->value().toString(), "X");
    EXPECT_EQ(t->axis_location->value().toString(), "point");
    EXPECT_DOUBLE_EQ(t->drill_dia->value(), 3.125);
    EXPECT_EQ(t->object_combo->count(), 2);
    EXPECT_FALSE(t->box_combo->isVisibleTo(t));  // solo con la ubicación "Caja"
    EXPECT_TRUE(t->point_entry->isVisibleTo(t));

    // Sin punto: aviso. Con (0, 0) y eje Y se refleja B_Cu como en los .nc del usuario.
    t->object_combo->setValue(cu->name());
    EXPECT_FALSE(t->mirrorObject());
    t->point_entry->setText("(0, 0)");
    EXPECT_EQ(t->align_ref_label_val->text(), "(0, 0)");
    t->mirror_axis->setValue("Y");
    t->mirror_button->click();
    flatcam::Gerber ref;
    ASSERT_TRUE(ref.parseFile(dir + "B_Cu.gbr"));
    ref.mirror(flatcam::Axis::Y, {0, 0});
    EXPECT_NEAR(cu->gerber().bounds().xmin, ref.bounds().xmin, 1e-9);
    EXPECT_NEAR(cu->gerber().bounds().xmax, ref.bounds().xmax, 1e-9);

    // Ubicación "Caja": el centro del contorno (x = 0), así que reflejar de nuevo deja B_Cu como estaba.
    t->axis_location->setValue("box");
    EXPECT_TRUE(t->box_combo->isVisibleTo(t));
    EXPECT_FALSE(t->point_entry->isVisibleTo(t));
    EXPECT_EQ(t->align_ref_label_val->text(), "Box centroid");
    t->box_combo->setValue(edge->name());
    ASSERT_TRUE(t->mirrorObject());
    flatcam::Gerber orig;
    ASSERT_TRUE(orig.parseFile(dir + "B_Cu.gbr"));
    EXPECT_NEAR(cu->gerber().bounds().xmin, orig.bounds().xmin, 1e-6);

    // Límites de lo elegido -> centro -> punto.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    t->calculate_bb_button->click();
    EXPECT_EQ(t->axis_location->value().toString(), "point");
    EXPECT_NEAR(t->xmin_entry->value(), edge->bounds().left(), 1e-4);
    EXPECT_EQ(t->point_entry->text(), t->center_entry->text());
    t->xmax_btn->click();
    EXPECT_TRUE(t->point_entry->text().startsWith(QString("(%1,").arg(QString::number(edge->bounds().right(), 'f', 4))));

    // "Encajar en el agujero": un clic dentro de un taladro pone su centro como punto.
    t->axis_location->setValue("hole");
    t->exc_combo->setValue(drl->name());
    t->pick_hole_button->click();
    const flatcam::Point2D hole = drl->excellon().drillTools().begin()->second.drills.front();
    Q_EMIT app.ui()->plotCanvas()->mouseClicked(QPointF(hole.x + 0.05, hole.y - 0.05), Qt::LeftButton);
    EXPECT_EQ(t->axis_location->value().toString(), "point");
    EXPECT_EQ(t->point_entry->text(), QString("(%1, %2)").arg(QString::number(hole.x, 'f', 4), QString::number(hole.y, 'f', 4)));
    Q_EMIT app.ui()->plotCanvas()->mouseClicked(QPointF(0, 0), Qt::RightButton);  // cancela la elección

    // Agujeros de alineación del Alinea del usuario: (-38, 39.5) y (38, 39.5) con eje X por el origen -> 4 agujeros,
    // cada uno seguido de su reflejo, iguales a los del núcleo.
    t->setToolUi();
    t->point_entry->setText("(0, 0)");
    t->alignment_holes->setText("(-38, 39.5), (38, 39.5)");
    t->drill_dia->setValue(3.0);
    auto* al = dynamic_cast<ExcellonObject*>(t->createAlignmentHoles());
    ASSERT_NE(al, nullptr);
    EXPECT_EQ(al->name(), tr_("Alignment Drills"));
    const flatcam::Excellon expected =
        flatcam::DoubleSided::alignmentExcellon({{-38, 39.5}, {38, 39.5}}, flatcam::Axis::X, {0, 0}, 3.0);
    ASSERT_EQ(al->excellon().drillTools().size(), 1u);
    EXPECT_EQ(al->excellon().drillTools().at(1).drills, expected.drillTools().at(1).drills);
    EXPECT_EQ(al->excellon().drillTools().at(1).drills.size(), 4u);
    EXPECT_DOUBLE_EQ(al->excellon().drillTools().at(1).tooldia, 3.0);
    // Los botones de agregar usan el último clic en el canvas.
    Q_EMIT app.ui()->plotCanvas()->mouseClicked(QPointF(1.5, -2.25), Qt::LeftButton);
    t->add_drill_point_button->click();
    t->add_drill_point_button->click();
    EXPECT_EQ(t->alignment_holes->text(), "(1.5000, -2.2500),(1.5000, -2.2500),");
    t->delete_drill_point_button->click();
    EXPECT_EQ(t->alignment_holes->text(), "(1.5000, -2.2500),");
}

TEST(GuiIsolation, CombinedSeparateAndRest) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* cu = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "B_Cu.gbr")));
    ASSERT_NE(cu, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(cu));
    IsolationTool* t = app.isolationTool;
    app.ui()->isolation_btn->trigger();
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), t);
    EXPECT_EQ(t->object_combo->currentText(), cu->name());
    // De fábrica: una herramienta de 0,1, una pasada, 10 %, combinar; nivel Básico (sin resto ni tipo a la vista).
    ASSERT_EQ(t->tools_table->rowCount(), 1);
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "0.1000");
    EXPECT_EQ(t->passes_entry->value(), 1);
    EXPECT_DOUBLE_EQ(t->iso_overlap_entry->value(), 10.0);
    EXPECT_TRUE(t->combine_passes_cb->isChecked());
    EXPECT_FALSE(t->rest_cb->isVisibleTo(t));
    EXPECT_TRUE(t->tool_data_label->text().contains("Herramienta 1"));

    // Lo que da el núcleo con los mismos datos.
    flatcam::IsolationTool it;
    it.toolDia = 0.1;
    it.passes = 1;
    it.overlap = 0.1;
    flatcam::IsolationOptions o;
    flatcam::IsolationOutcome expected;
    std::string err;
    ASSERT_TRUE(flatcam::Isolation::isolate(cu->gerber().solidGeometry(), cu->gerber().followGeometry(), {it}, o,
                                            expected, err));

    t->generate_iso_button->click();
    auto* comb = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-B_Cu.gbr_iso_combined"));
    ASSERT_NE(comb, nullptr);
    ASSERT_EQ(comb->tools().size(), 1u);
    const flatcam::ShapeList& got = comb->tools().begin()->second.solidGeometry;
    ASSERT_EQ(got.size(), expected.sets.at(0).geometry.size());
    for (std::size_t i = 0; i < got.size(); ++i) EXPECT_NEAR(got[i].area(), expected.sets[0].geometry[i].area(), 1e-9);
    EXPECT_EQ(app.ui()->notebook->currentWidget(), app.ui()->properties_tab);

    // Dos pasadas sin combinar: un objeto por pasada con el nombre del original.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(cu));
    app.ui()->isolation_btn->trigger();  // el panel ya estaba: vuelve a la pestaña y lo prepara de nuevo
    t->setToolUi();
    t->passes_entry->setValue(2);
    EXPECT_EQ(t->tools().begin()->second.passes, 2);
    t->combine_passes_cb->setChecked(false);
    const std::vector<FlatCAMObj*> sep = t->isolate();
    ASSERT_EQ(sep.size(), 2u);
    EXPECT_EQ(sep[0]->name(), "luz de noche Pascual-B_Cu.gbr_0.1000_iso1");
    EXPECT_EQ(sep[1]->name(), "luz de noche Pascual-B_Cu.gbr_0.1000_iso2");

    // Mecanizado de resto (opción avanzada): obliga a combinar y crea `_iso_rest`.
    t->setToolUi();
    t->new_tooldia_entry->setValue(0.4);
    t->search_and_add_btn->click();
    ASSERT_EQ(t->tools_table->rowCount(), 2);
    EXPECT_EQ(t->tools_table->item(0, 1)->text(), "0.4000");  // orden "Atrás": la más grande primero
    t->combine_passes_cb->setChecked(false);
    t->rest_cb->setChecked(true);
    EXPECT_TRUE(t->combine_passes_cb->isChecked());
    EXPECT_FALSE(t->combine_passes_cb->isEnabled());
    t->tools_table->selectAll();
    const std::vector<FlatCAMObj*> rest = t->isolate();
    ASSERT_EQ(rest.size(), 1u);
    EXPECT_EQ(rest[0]->name(), "luz de noche Pascual-B_Cu.gbr_iso_rest");
    t->rest_cb->setChecked(false);
    EXPECT_FALSE(t->combine_passes_cb->isChecked());  // vuelve a como estaba
}

TEST(GuiProject, OpensTheUsersProject) {
    // Proyecto real del usuario (FlatCAM 2024.4), reducido a 7 objetos: Edge_Cuts, B_Cu y Logo (reflejados), PTH.drl,
    // la Geometría del corte y los CNC Job del corte y del taladro.
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/";
    ASSERT_TRUE(app.openProject(QString::fromStdString(dir + "luz_de_noche_reducido.FlatPrj")));
    EXPECT_EQ(app.collection()->names(),
              (QStringList{"luz de noche Pascual-Edge_Cuts.gbr", "luz de noche Pascual-B_Cu.gbr",
                           "luz de noche Pascual-Logo.gbr", "luz de noche Pascual-PTH.drl",
                           "luz de noche Pascual-Edge_Cuts.gbr_cutout", "luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc",
                           "luz de noche Pascual-PTH.drl_cnc"}));
    EXPECT_TRUE(app.ui()->windowTitle().endsWith("luz_de_noche_reducido.FlatPrj"));
    // Los Gerber quedan como estaban en el proyecto (B_Cu reflejado), no como el archivo original.
    auto* cu = dynamic_cast<GerberObject*>(app.collection()->byName("luz de noche Pascual-B_Cu.gbr"));
    ASSERT_NE(cu, nullptr);
    flatcam::Gerber ref;
    ASSERT_TRUE(ref.parseFile(dir + "luz de noche Pascual-B_Cu.gbr"));
    ref.mirror(flatcam::Axis::Y, {0, 0});
    EXPECT_NEAR(cu->gerber().bounds().xmin, ref.bounds().xmin, 1e-6);
    EXPECT_NEAR(cu->gerber().bounds().xmax, ref.bounds().xmax, 1e-6);
    EXPECT_EQ(cu->gerber().apertures().size(), ref.apertures().size());
    EXPECT_EQ(cu->fillColor, colorFromFlatCam("#BBF268bf"));
    // El Excellon, con sus 120 agujeros reflejados.
    auto* drl = dynamic_cast<ExcellonObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl"));
    ASSERT_NE(drl, nullptr);
    std::size_t holes = 0;
    for (const auto& [id, t] : drl->excellon().drillTools()) holes += t.drills.size();
    EXPECT_EQ(holes, 120u);
    // La Geometría del corte, con los parámetros que usó el usuario.
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_cutout"));
    ASSERT_NE(geo, nullptr);
    ASSERT_EQ(geo->tools().size(), 1u);
    const GeometryTool& t = geo->tools().at(1);
    EXPECT_DOUBLE_EQ(t.tooldia, 1.2);
    EXPECT_DOUBLE_EQ(t.data.feedrate, 20.0);
    EXPECT_DOUBLE_EQ(t.data.spindleSpeed, 10000.0);
    ASSERT_TRUE(t.data.endXY.has_value());
    EXPECT_FALSE(t.solidGeometry.empty());
    // Generar de nuevo su CNC Job da el .nc real del usuario (salvo la fecha).
    QString err;
    auto job = geo->makeCncJob({1}, err);
    ASSERT_TRUE(job) << err.toStdString();
    const QStringList real = readTextLines(QString::fromStdString(dir + "luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc.nc"));
    QStringList mine = QString::fromStdString(job->exportText()).split('\n');
    auto noDate = [](QStringList l) {
        l.replaceInStrings(QRegularExpression("^\\(Created on .*\\)$"), "(Created on)");
        return l;
    };
    EXPECT_EQ(noDate(mine), noDate(real));
    // Los CNC Job guardados se exportan iguales a los archivos reales, con fecha y todo: la cabecera viene del proyecto y
    // el código de inicio, que el original no guarda (y al reabrir exporta sin él), se reconstruye (pedido del usuario).
    auto firstDiff = [](const QStringList& a, const QStringList& b) {
        for (int i = 0; i < std::min(a.size(), b.size()); ++i)
            if (a[i] != b[i]) return QString("línea %1: '%2' != '%3'").arg(i + 1).arg(a[i], b[i]);
        return a.size() == b.size() ? QString() : QString("largo %1 != %2").arg(a.size()).arg(b.size());
    };
    auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc"));
    ASSERT_NE(cnc, nullptr);
    const QStringList restored = QString::fromStdString(cnc->exportText()).split('\n');
    EXPECT_EQ(firstDiff(restored, real), QString());
    const QStringList realDrill = readTextLines(QString::fromStdString(dir + "luz de noche Pascual-PTH.drl_cnc.nc"));
    auto* drillJob = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
    ASSERT_NE(drillJob, nullptr);
    EXPECT_EQ(firstDiff(QString::fromStdString(drillJob->exportText()).split('\n'), realDrill), QString());
    // El taladro guardado muestra su tabla de herramientas de Excellon.
    auto* drill = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
    ASSERT_NE(drill, nullptr);
    ASSERT_EQ(drill->tools().size(), 4u);
    EXPECT_EQ(drill->tools()[0].drills, 84);
    EXPECT_GT(drill->plotData().fill.size(), 1000u);
}

TEST(GuiProject, SavesAndReopensWhatItOpened) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/";
    ASSERT_TRUE(app.openProject(QString::fromStdString(dir + "luz_de_noche_reducido.FlatPrj")));
    QTemporaryDir tmp;
    const QString saved = tmp.filePath("copia.FlatPrj");
    ASSERT_TRUE(app.saveProject(saved));
    if (qEnvironmentVariableIsSet("FLATCAM_SAVE_COPY2")) QFile::copy(saved, qEnvironmentVariable("FLATCAM_SAVE_COPY2"));
    EXPECT_TRUE(app.ui()->windowTitle().endsWith("copia.FlatPrj"));
    // Comprimido con XZ (global_save_compressed) y legible otra vez.
    QFile f(saved);
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    EXPECT_TRUE(f.read(6) == QByteArray("\xFD" "7zXZ\0", 6));
    f.close();
    App again;
    ASSERT_TRUE(again.openProject(saved));
    EXPECT_EQ(again.collection()->names(), app.collection()->names());
    // Mismas figuras, aperturas, agujeros y parámetros.
    auto* a = dynamic_cast<GerberObject*>(app.collection()->byName("luz de noche Pascual-B_Cu.gbr"));
    auto* b = dynamic_cast<GerberObject*>(again.collection()->byName("luz de noche Pascual-B_Cu.gbr"));
    ASSERT_TRUE(a && b);
    ASSERT_EQ(a->gerber().solidGeometry().size(), b->gerber().solidGeometry().size());
    double areaA = 0, areaB = 0;
    for (const auto& s : a->gerber().solidGeometry()) areaA += s.area();
    for (const auto& s : b->gerber().solidGeometry()) areaB += s.area();
    EXPECT_NEAR(areaA, areaB, 1e-9);
    EXPECT_EQ(a->gerber().apertures().size(), b->gerber().apertures().size());
    EXPECT_EQ(a->gerber().sourceFile(), b->gerber().sourceFile());
    auto* ga = dynamic_cast<GeometryObject*>(again.collection()->byName("luz de noche Pascual-Edge_Cuts.gbr_cutout"));
    ASSERT_NE(ga, nullptr);
    EXPECT_DOUBLE_EQ(ga->tools().at(1).data.feedrate, 20.0);
    // Lo que el port no usa del proyecto se conserva (por ejemplo, las opciones del corte en los datos de la herramienta).
    EXPECT_EQ(ga->tools().at(1).projectData["tools_cutout_gapsize"].toDouble(), 4.0);
    // El CNC Job se exporta igual que antes de guardar.
    auto* ca = dynamic_cast<CNCJobObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
    auto* cb = dynamic_cast<CNCJobObject*>(again.collection()->byName("luz de noche Pascual-PTH.drl_cnc"));
    ASSERT_TRUE(ca && cb);
    EXPECT_EQ(cb->exportText(), ca->exportText());
    EXPECT_EQ(cb->tools().size(), 4u);
}

TEST(GuiProject, SavesObjectsCreatedInThePort) {
    // Contorno -> Corte -> Geometría -> CNC Job, y el taladro: se guarda, se reabre y todo sigue igual.
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* drl = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_TRUE(edge && drl);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    app.ui()->cutout_btn->trigger();
    app.cutoutTool->dia->setValue(1.2);
    app.cutoutTool->mpass_cb->setChecked(false);
    auto* geo = dynamic_cast<GeometryObject*>(app.cutoutTool->freeformCutout());
    ASSERT_NE(geo, nullptr);
    QString err;
    FlatCAMObj* cnc = app.addObject(geo->makeCncJob({1}, err));
    ASSERT_NE(cnc, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(drl));
    app.ui()->drill_btn->trigger();
    app.drillingTool->tools_table->selectAll();
    FlatCAMObj* dcnc = app.drillingTool->generateCnc();
    ASSERT_NE(dcnc, nullptr);

    QTemporaryDir tmp;
    const QString saved = tmp.filePath("nuevo.FlatPrj");
    ASSERT_TRUE(app.saveProject(saved));
    // Para verificarlo con el FlatCAM original: tests/tools/check_project_in_original.py.
    if (qEnvironmentVariableIsSet("FLATCAM_SAVE_COPY")) QFile::copy(saved, qEnvironmentVariable("FLATCAM_SAVE_COPY"));
    // Las opciones de cada objeto son las de fábrica de su tipo (new_object), con su nombre y sus límites.
    flatcam::JsonValue root;
    std::string e;
    ASSERT_TRUE(flatcam::ProjectFile::read(saved.toStdString(), root, e)) << e;
    EXPECT_EQ(root["version"].text(), "2024.4");
    EXPECT_EQ(root["options"]["units"].text(), "MM");
    const flatcam::JsonValue& g0 = root["objs"].items()[0];
    EXPECT_EQ(g0["kind"].text(), "gerber");
    EXPECT_EQ(g0["options"]["name"].text(), "luz de noche Pascual-Edge_Cuts.gbr");
    EXPECT_TRUE(g0["options"].find("isotooldia") || g0["options"].find("tools_iso_passes"));
    const flatcam::JsonValue& c = root["objs"].items()[3];  // orden del árbol: Gerber, Excellon, Geometría, CNC Job
    EXPECT_EQ(c["options"]["name"].text(), "luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc");
    EXPECT_EQ(c["kind"].text(), "cncjob");
    EXPECT_FALSE(c["cnc_tools"]["1"]["gcode_parsed"].items().empty());
    EXPECT_EQ(c["cnc_tools"]["1"]["data"]["feedrate"].toDouble(), 120.0);
    const flatcam::JsonValue& d = root["objs"].items()[4];
    EXPECT_EQ(d["origin_kind"].text(), "excellon");
    // Sin cambio de herramienta (lo de fábrica) hay un solo G-code: va en la primera herramienta; todas llevan los agujeros.
    ASSERT_EQ(d["exc_cnc_tools"].members().size(), 4u);
    EXPECT_FALSE(d["exc_cnc_tools"].members().front().second["gcode"].text().empty());
    EXPECT_TRUE(d["exc_cnc_tools"].members().back().second["gcode"].text().empty());
    EXPECT_FALSE(d["exc_cnc_tools"].members().back().second["solid_geometry"].items().empty());

    App again;
    ASSERT_TRUE(again.openProject(saved));
    EXPECT_EQ(again.collection()->names(), app.collection()->names());
    auto* g2 = dynamic_cast<GeometryObject*>(again.collection()->byName(geo->name()));
    ASSERT_NE(g2, nullptr);
    auto job2 = g2->makeCncJob({1}, err);
    ASSERT_TRUE(job2);
    auto job1 = geo->makeCncJob({1}, err);
    auto skipDate = [](std::string s) { return QString::fromStdString(s).remove(QRegularExpression("\\(Created on [^)]*\\)")); };
    EXPECT_EQ(skipDate(job2->exportText()), skipDate(job1->exportText()));
    auto* d2 = dynamic_cast<CNCJobObject*>(again.collection()->byName(dcnc->name()));
    ASSERT_NE(d2, nullptr);
    // Con el código de inicio reconstruido, el taladro reabierto se exporta igual que antes de guardar.
    const QStringList x = skipDate(d2->exportText()).split('\n'), y = skipDate(static_cast<CNCJobObject*>(dcnc)->exportText()).split('\n');
    ASSERT_EQ(x.size(), y.size());
    for (int i = 0; i < x.size(); ++i) ASSERT_EQ(x[i], y[i]) << "línea " << i + 1;
}

TEST(GuiProjectMenu, ContextMenuActions) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/";
    ASSERT_TRUE(app.openProject(QString::fromStdString(dir + "luz_de_noche_reducido.FlatPrj")));
    ObjectCollection* col = app.collection();
    KeySensitiveListView* view = app.ui()->collection_view;
    view->expandAll();
    auto select = [&](const QStringList& names) {
        view->clearSelection();
        view->setCurrentIndex(col->indexOf(col->byName(names.first())));  // carga sus Propiedades
        for (const QString& n : names)
            view->selectionModel()->select(col->indexOf(col->byName(n)), QItemSelectionModel::Select);
    };
    const QString edge = "luz de noche Pascual-Edge_Cuts.gbr", cut = "luz de noche Pascual-Edge_Cuts.gbr_cutout";
    // Lo que se ve según lo elegido (on_menu_request): "Crear CNCJob" solo con Geometrías; color con Gerber o Excellon.
    select({edge});
    app.updateProjectMenu();
    EXPECT_FALSE(app.menuProjectGenerateCnc->isVisible());
    EXPECT_TRUE(app.menuProjectColor->isEnabled());
    EXPECT_TRUE(app.menuProjectViewSource->isVisible());
    EXPECT_EQ(app.menuProjectEnable->text(), QString::fromUtf8("Habilitar Parcela"));
    select({cut});
    app.updateProjectMenu();
    EXPECT_TRUE(app.menuProjectGenerateCnc->isVisible());
    EXPECT_FALSE(app.menuProjectColor->isEnabled());
    EXPECT_FALSE(app.menuProjectViewSource->isVisible());

    // Mostrar/ocultar.
    FlatCAMObj* geo = col->byName(cut);
    EXPECT_FALSE(geo->plotEnabled());  // así estaba en el proyecto del usuario
    app.menuProjectEnable->trigger();
    EXPECT_TRUE(geo->plotEnabled());
    app.menuProjectDisable->trigger();
    EXPECT_FALSE(geo->plotEnabled());

    // Colores: rojo con transparencia BF y contorno al 70 %; "Por defecto" vuelve a los de fábrica.
    select({edge});
    FlatCAMObj* g = col->byName(edge);
    app.setSelectedColor("Red");
    EXPECT_EQ(g->fillColor, colorFromFlatCam("#FF0000BF"));
    EXPECT_EQ(g->outlineColor, QColor(178, 0, 0));  // round(255 * 0.7): 178.49999999999997 -> 178, como Python
    app.setSelectedColor("Default");
    EXPECT_EQ(g->fillColor, colorFromFlatCam("#BBF268BF"));

    // Crear CNCJob desde el menú, con todas las herramientas de la Geometría.
    select({cut});
    app.menuProjectGenerateCnc->trigger();
    EXPECT_NE(col->byName(cut + "_cnc_1"), nullptr);  // ya había un "<geo>_cnc": el nuevo se renombra

    // Ver fuente: una pestaña "Editor de fuente" con el texto del Gerber.
    select({edge});
    const int tabs = app.ui()->plot_tab_area->count();
    app.menuProjectViewSource->trigger();
    ASSERT_EQ(app.ui()->plot_tab_area->count(), tabs + 1);
    EXPECT_EQ(app.ui()->plot_tab_area->tabText(tabs), QString::fromUtf8("Editor de fuente"));
    auto* src = qobject_cast<QPlainTextEdit*>(app.ui()->plot_tab_area->widget(tabs));
    ASSERT_NE(src, nullptr);
    EXPECT_TRUE(src->toPlainText().startsWith("%TF.GenerationSoftware,KiCad"));

    // Copiar: "<nombre>_copy", con la misma geometría.
    const std::vector<FlatCAMObj*> copies = app.copySelected();
    ASSERT_EQ(copies.size(), 1u);
    EXPECT_EQ(copies[0]->name(), edge + "_copy");
    EXPECT_NEAR(copies[0]->bounds().width(), g->bounds().width(), 1e-9);

    // Renombrar desde el campo Nombre de Propiedades (Enter).
    select({edge + "_copy"});
    auto* ui = qobject_cast<GerberObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    ui->name_entry->setText("contorno");
    Q_EMIT ui->name_entry->returnPressed();
    EXPECT_EQ(copies[0]->name(), "contorno");
    EXPECT_EQ(col->data(col->indexOf(copies[0]), Qt::DisplayRole).toString(), "contorno");

    // Borrar (sin preguntar) dos objetos; también con la tecla Supr desde el árbol.
    const int before = static_cast<int>(col->objects().size());
    select({"contorno", "luz de noche Pascual-Logo.gbr"});
    ASSERT_TRUE(app.deleteSelected(true));
    EXPECT_EQ(static_cast<int>(col->objects().size()), before - 2);
    EXPECT_EQ(col->byName("contorno"), nullptr);
    EXPECT_EQ(app.ui()->properties_scroll_area->widget()->objectName(), "default_properties");  // panel vacío
    view->clearSelection();
    EXPECT_FALSE(app.deleteSelected(true));
}

TEST(GuiRecent, RecentProjectsAndFiles) {
    QTemporaryDir data;
    App app;
    app.setDataPath(data.path());
    // Vacíos: solo "Borrar ...".
    ASSERT_EQ(app.ui()->recent_projects->actions().size(), 2);  // separador + borrar
    EXPECT_EQ(app.ui()->recent_projects->actions().last()->text(), QString::fromUtf8("Borrar proyectos recientes"));
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/";
    ASSERT_NE(app.openByExtension(QString::fromStdString(dir + "luz de noche Pascual-Logo.gbr")), nullptr);
    ASSERT_NE(app.openByExtension(QString::fromStdString(dir + "luz de noche Pascual-PTH.drl")), nullptr);
    ASSERT_TRUE(app.openProject(QString::fromStdString(dir + "luz_de_noche_reducido.FlatPrj")));
    // Proyectos: el abierto; archivos: el último primero.
    QList<QAction*> p = app.ui()->recent_projects->actions();
    ASSERT_EQ(p.size(), 3);
    EXPECT_EQ(p[0]->text(), "luz_de_noche_reducido.FlatPrj");
    QList<QAction*> f = app.ui()->recent->actions();
    ASSERT_EQ(f.size(), 4);
    EXPECT_EQ(f[0]->text(), "luz de noche Pascual-PTH.drl");
    EXPECT_EQ(f[1]->text(), "luz de noche Pascual-Logo.gbr");
    // El archivo es el mismo JSON que escribe el original.
    QFile json(data.filePath("recent.json"));
    ASSERT_TRUE(json.open(QIODevice::ReadOnly));
    EXPECT_TRUE(json.readAll().startsWith("[\n  {\n    \"filename\": "));
    json.close();
    // Repetir no duplica; elegir uno del menú lo abre.
    app.openByExtension(QString::fromStdString(dir + "luz de noche Pascual-Logo.gbr"));
    EXPECT_EQ(app.ui()->recent->actions().size(), 4);
    app.clearProject();
    app.ui()->recent->actions()[1]->trigger();
    EXPECT_NE(app.collection()->byName("luz de noche Pascual-Logo.gbr"), nullptr);
    app.ui()->recent_projects->actions()[0]->trigger();
    EXPECT_EQ(app.collection()->objects().size(), 7u);
    // Otra ventana (otra sesión) ve los mismos recientes.
    App other;
    other.setDataPath(data.path());
    EXPECT_EQ(other.ui()->recent_projects->actions().size(), 3);
    // Borrar.
    app.ui()->recent_projects->actions().last()->trigger();
    EXPECT_EQ(app.ui()->recent_projects->actions().size(), 2);
    app.ui()->recent->actions().last()->trigger();
    EXPECT_EQ(app.ui()->recent->actions().size(), 2);
}

TEST(GuiClearingMethods, PaintAndNccMethodsAndRest) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* logo = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    ASSERT_NE(logo, nullptr);
    const flatcam::ShapeList logoGeo = logo->gerber().solidGeometry();
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));

    // Paint con Semilla: lo mismo que el núcleo.
    app.ui()->paint_btn->trigger();
    PaintTool* p = app.paintTool;
    p->paintmethod_combo->setIndexValue(1);
    auto* seed = dynamic_cast<GeometryObject*>(p->paint());
    ASSERT_NE(seed, nullptr);
    flatcam::PaintOptions po;
    po.toolDia = 0.3;
    po.overlap = 0.2;
    po.method = flatcam::PolygonClearing::Method::Seed;
    flatcam::ShapeList expected;
    std::string err;
    ASSERT_TRUE(flatcam::Paint::paintAll(logoGeo, po, expected, err)) << err;
    EXPECT_EQ(seed->tools().begin()->second.solidGeometry.size(), expected.size());

    // Paint con resto: 0,3 y 0,1 (la más grande primero), método Líneas del formulario.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    app.ui()->paint_btn->trigger();
    app.ui()->paint_btn->trigger();  // abre/cierra/abre: queda abierta y preparada
    p->setToolUi();
    p->new_tooldia_entry->setValue(0.1);
    p->add_newtool_button->click();
    p->tools_table->selectAll();
    p->rest_cb->setChecked(true);
    EXPECT_FALSE(p->offset_entry->isVisibleTo(p));
    EXPECT_TRUE(p->rest_offset_entry->isVisibleTo(p));
    EXPECT_FALSE(p->order_radio->isEnabled());
    p->paintmethod_combo->setIndexValue(2);
    auto* rest = dynamic_cast<GeometryObject*>(p->paint());
    ASSERT_NE(rest, nullptr);
    std::vector<std::pair<std::size_t, flatcam::ShapeList>> perTool;
    ASSERT_TRUE(flatcam::Paint::paintRest(logoGeo, {{0.3, 0.2, true, true}, {0.1, 0.2, true, true}}, 0.0,
                                          flatcam::PolygonClearing::Method::Lines, 64, perTool, err))
        << err;
    ASSERT_EQ(rest->tools().size(), perTool.size());
    std::size_t lines = 0, expectedLines = 0;
    for (const auto& [uid, t] : rest->tools()) lines += t.solidGeometry.size();
    for (const auto& [i, l] : perTool) expectedLines += l.size();
    EXPECT_EQ(lines, expectedLines);

    // NCC con Líneas y con resto (1,0 y 0,5 de fábrica, margen 1).
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    app.ui()->ncc_btn->trigger();
    NccTool* n = app.nccTool;
    n->tools_table->selectAll();
    n->ncc_method_combo->setIndexValue(2);  // queda guardado en las dos herramientas elegidas
    n->ncc_rest_cb->setChecked(true);
    EXPECT_TRUE(n->rest_ncc_connect_cb->isVisibleTo(n));
    EXPECT_FALSE(n->ncc_connect_cb->isVisibleTo(n));
    auto* nrest = dynamic_cast<GeometryObject*>(n->clearCopper());
    ASSERT_NE(nrest, nullptr);
    flatcam::NccOptions common;
    common.margin = 1.0;
    std::vector<std::pair<std::size_t, flatcam::ShapeList>> nPerTool;
    flatcam::ShapeList left;
    ASSERT_TRUE(flatcam::Ncc::clearCopperRest(logoGeo,
                                              {{1.0, 0.4, flatcam::PolygonClearing::Method::Lines},
                                               {0.5, 0.4, flatcam::PolygonClearing::Method::Lines}},
                                              common, nPerTool, left, err))
        << err;
    ASSERT_EQ(nrest->tools().size(), nPerTool.size());
}

TEST(GuiCanvasSelection, AreaPolygonAndReferenceSelections) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* logo = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_TRUE(logo && edge);
    PlotCanvas* canvas = app.ui()->plotCanvas();
    auto click = [&](double x, double y, Qt::MouseButton b = Qt::LeftButton, bool dragged = false) {
        Q_EMIT canvas->mouseReleased(QPointF(x, y), b, dragged);
    };
    const flatcam::ShapeList logoPolys = flatcam::Paint::flattenToPolygons(logo->gerber().solidGeometry());
    ASSERT_GT(logoPolys.size(), 3u);
    const flatcam::Point2D inside = logoPolys[0].representativePoint();

    // ---- Paint, "Selección de polígono": marcar, desmarcar, marcar otra vez; el derecho pinta solo ese polígono.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    app.ui()->paint_btn->trigger();
    PaintTool* p = app.paintTool;
    p->deltool_btn->click();
    p->new_tooldia_entry->setValue(0.1);
    p->add_newtool_button->click();
    p->selectmethod_combo->setIndexValue(1);
    EXPECT_FALSE(p->area_shape_radio->isVisibleTo(p));
    EXPECT_EQ(p->paint(), nullptr);  // empieza a elegir
    EXPECT_TRUE(app.picker()->active());
    click(inside.x, inside.y);
    click(inside.x, inside.y);                        // desmarca
    click(-1000, -1000);                              // nada debajo
    click(inside.x, inside.y);
    click(inside.x, inside.y, Qt::RightButton, true);  // arrastre para desplazar: no termina
    EXPECT_TRUE(app.picker()->active());
    click(inside.x, inside.y, Qt::RightButton);
    EXPECT_FALSE(app.picker()->active());
    auto* one = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_mt_paint"));
    ASSERT_NE(one, nullptr);
    flatcam::PaintOptions po;
    po.toolDia = 0.1;
    po.overlap = 0.2;
    flatcam::ShapeList expected;
    std::string err;
    ASSERT_TRUE(flatcam::Paint::paintAll({logoPolys[0]}, po, expected, err)) << err;
    EXPECT_EQ(one->tools().begin()->second.solidGeometry.size(), expected.size());

    // ---- Paint, "Selección de área" con rectángulos: dos clics por zona y el derecho termina.
    p->selectmethod_combo->setIndexValue(2);
    EXPECT_TRUE(p->area_shape_radio->isVisibleTo(p));
    p->area_shape_radio->setValue("square");
    p->paint();
    const QRectF lb = logo->bounds();
    click(lb.left() - 1, lb.top() - 1);
    click(lb.center().x(), lb.bottom() + 1);
    click(0, 0, Qt::RightButton);
    auto* half = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_mt_paint_1"));
    ASSERT_NE(half, nullptr);
    EXPECT_LE(half->bounds().right(), lb.center().x() + 1e-6);

    // ---- Escape cancela una selección en curso.
    p->paint();
    click(1, 1);
    QKeyEvent esc(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    QApplication::sendEvent(app.ui(), &esc);
    EXPECT_FALSE(app.picker()->active());

    // ---- NCC, "Selección de área" con un polígono dibujado punto por punto.
    app.ui()->ncc_btn->trigger();
    NccTool* n = app.nccTool;
    n->tools_table->selectAll();
    n->deltool_btn->click();
    n->new_tooldia_entry->setValue(0.3);
    n->add_newtool_button->click();
    n->ncc_method_combo->setIndexValue(0);
    n->select_combo->setIndexValue(1);
    EXPECT_TRUE(n->area_shape_radio->isVisibleTo(n));
    n->area_shape_radio->setValue("polygon");
    n->object_combo->setValue(logo->name());
    n->clearCopper();
    const double cx = lb.center().x(), cy = lb.center().y();
    click(cx - 4, cy - 4);
    click(cx + 4, cy - 4);
    click(cx + 4, cy + 4);
    click(cx - 4, cy + 4, Qt::RightButton);  // cierra el polígono (con este punto)
    EXPECT_TRUE(app.picker()->active());
    click(0, 0, Qt::RightButton);  // termina
    auto* ncc = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_ncc"));
    ASSERT_NE(ncc, nullptr);
    flatcam::NccOptions no;
    no.toolDia = 0.3;
    no.overlap = 0.4;
    no.margin = 1.0;
    no.region = flatcam::Ncc::areaRegion(
        {flatcam::Shape::polygon({{cx - 4, cy - 4}, {cx + 4, cy - 4}, {cx + 4, cy + 4}, {cx - 4, cy + 4}})}, 1.0);
    flatcam::ShapeList nccExpected;
    ASSERT_TRUE(flatcam::Ncc::clearCopper(logo->gerber().solidGeometry(), no, nccExpected, err)) << err;
    EXPECT_EQ(ncc->tools().begin()->second.solidGeometry.size(), nccExpected.size());

    // ---- NCC, "Objeto de referencia": el contorno (Gerber) limita la región.
    n->select_combo->setIndexValue(2);
    EXPECT_TRUE(n->reference_combo->isVisibleTo(n));
    n->reference_combo->setValue(edge->name());
    EXPECT_NE(n->clearCopper(), nullptr);

    // ---- Isolation, "Selección de polígono" y "Selección de área" sobre el Logo.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    app.ui()->isolation_btn->trigger();
    IsolationTool* iso = app.isolationTool;
    iso->select_combo->setCurrentIndex(2);
    EXPECT_TRUE(iso->poly_int_cb->isVisibleTo(iso));
    iso->isolate();
    click(inside.x, inside.y);
    click(0, 0, Qt::RightButton);
    auto* isoOne = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_iso_combined"));
    ASSERT_NE(isoOne, nullptr);
    flatcam::IsolationTool it;
    it.toolDia = 0.1;
    flatcam::IsolationOptions io;
    flatcam::IsolationOutcome out;
    const flatcam::ShapeList chosen{logoPolys[0]};
    ASSERT_TRUE(flatcam::Isolation::isolate(logo->gerber().solidGeometry(), logo->gerber().followGeometry(), {it}, io, out,
                                            err, &chosen));
    EXPECT_EQ(isoOne->tools().begin()->second.solidGeometry.size(), out.sets.at(0).geometry.size());
    iso->select_combo->setCurrentIndex(1);
    EXPECT_FALSE(iso->rest_cb->isEnabled());
    iso->area_shape_radio->setValue("square");
    iso->isolate();
    click(lb.left() - 1, lb.top() - 1);
    click(lb.center().x(), lb.bottom() + 1);
    click(0, 0, Qt::RightButton);
    auto* isoArea = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_iso_combined_1"));
    ASSERT_NE(isoArea, nullptr);
    EXPECT_LE(isoArea->bounds().right(), lb.center().x() + 1e-6);
}

TEST(GuiPreprocessors, CombosAndHpglLikeTheOriginal) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    // Perforación: todos los postprocesadores menos hpgl, con default primero.
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(exc, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    DrillingTool* d = app.drillingTool;
    app.runTool(d, DrillingTool::info());
    d->setToolUi();
    EXPECT_EQ(d->pp_excellon_name_cb->count(), 19);
    EXPECT_EQ(d->pp_excellon_name_cb->itemText(0), "default");
    EXPECT_EQ(d->pp_excellon_name_cb->findText("hpgl"), -1);
    // Con la sonda aparecen sus campos y su valor llega al G-code.
    d->pp_excellon_name_cb->setValue("Toolchange_Probe_MACH3");
    EXPECT_FALSE(d->pdepth_entry->isHidden());
    d->pdepth_entry->setValue(-0.5);
    auto* cnc = dynamic_cast<CNCJobObject*>(d->generateCnc());
    ASSERT_NE(cnc, nullptr);
    EXPECT_NE(cnc->job().gcode().find("G31 Z-0.5000"), std::string::npos);

    // Geometría: también hpgl; al elegirlo se apaga la profundidad múltiple y se fuerza el cambio de herramienta.
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    app.ui()->cutout_btn->trigger();
    auto* geo = dynamic_cast<GeometryObject*>(app.cutoutTool->freeformCutout());
    ASSERT_NE(geo, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(geo));
    auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    EXPECT_EQ(ui->pp_geometry_name_cb->count(), 20);
    const bool multidepth = ui->mpass_cb->isChecked();
    ui->pp_geometry_name_cb->setValue("hpgl");
    EXPECT_FALSE(ui->mpass_cb->isChecked());
    EXPECT_FALSE(ui->mpass_cb->isEnabled());
    EXPECT_TRUE(ui->toolchangeg_cb->isChecked());
    ui->generate_cnc_button->click();
    auto* plot = dynamic_cast<CNCJobObject*>(app.collection()->byName(geo->name() + "_cnc"));
    ASSERT_NE(plot, nullptr);
    EXPECT_EQ(plot->job().startGcode().rfind("IN;\nPU;", 0), 0u);
    EXPECT_NE(plot->job().gcode().find("SP1;"), std::string::npos);
    // Al volver a otro postprocesador se restauran las casillas.
    ui->pp_geometry_name_cb->setValue("default");
    EXPECT_EQ(ui->mpass_cb->isChecked(), multidepth);
    EXPECT_TRUE(ui->mpass_cb->isEnabled());
}

TEST(GuiPaint, LaserLinesOnlyForGerber) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* logo = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    ASSERT_NE(logo, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    PaintTool* t = app.paintTool;
    app.ui()->paint_btn->trigger();
    auto* model = qobject_cast<QStandardItemModel*>(t->paintmethod_combo->model());
    ASSERT_NE(model, nullptr);
    EXPECT_TRUE(model->item(3)->isEnabled());
    // Líneas láser con una herramienta de 0,1: sigue las líneas centrales de los trazos del Logo.
    t->deltool_btn->click();
    t->new_tooldia_entry->setValue(0.1);
    t->add_newtool_button->click();
    t->paintmethod_combo->setIndexValue(3);
    EXPECT_EQ(t->tools().begin()->second.method, 3);
    t->generate_paint_button->click();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_mt_paint"));
    ASSERT_NE(geo, nullptr);
    std::size_t lines = 0;
    for (const auto& [id, tool] : geo->tools()) lines += tool.solidGeometry.size();
    EXPECT_GT(lines, 30000u);  // el original da 36.870 trazados con solape 20 %
    // Con una Geometría el método se deshabilita y, si estaba elegido, pasa a Combo.
    t->type_obj_radio->setValue("geometry");
    Q_EMIT t->type_obj_radio->activatedCustom("geometry");
    EXPECT_FALSE(model->item(3)->isEnabled());
    EXPECT_EQ(t->paintmethod_combo->indexValue(), 4);
}

TEST(GuiNcc, IsolationOperation) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* logo = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    ASSERT_NE(logo, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(logo));
    NccTool* t = app.nccTool;
    app.ui()->ncc_btn->trigger();
    ASSERT_EQ(t->tools_table->rowCount(), 2);  // 1,0 y 0,5 de fábrica
    // La de 0,5 (segunda fila) pasa a "Aislamiento": solo queda habilitado el tipo de fresado.
    t->tools_table->clearSelection();
    t->tools_table->selectRow(1);
    t->op_radio->setValue("iso");
    Q_EMIT t->op_radio->activatedCustom("iso");
    EXPECT_TRUE(t->milling_type_radio->isEnabled());
    EXPECT_FALSE(t->ncc_overlap_entry->isEnabled());
    // Solo con la de aislamiento no se puede: hace falta una de limpieza.
    t->generate_ncc_button->click();
    EXPECT_EQ(app.collection()->byName("luz de noche Pascual-Logo.gbr_ncc"), nullptr);
    EXPECT_TRUE(app.ui()->fcinfo->text->text().contains(QString::fromUtf8("limpieza")) ||
                app.ui()->fcinfo->text->text().contains("clearing"));
    // Con las dos: la de 1,0 limpia afuera de la envolvente de la de 0,5, que recorre el contorno del cobre.
    t->tools_table->selectAll();
    t->ncc_method_combo->setIndexValue(0);
    t->generate_ncc_button->click();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-Logo.gbr_ncc"));
    ASSERT_NE(geo, nullptr) << app.ui()->fcinfo->text->text().toStdString();
    ASSERT_EQ(geo->tools().size(), 2u);
    const GeometryTool* iso = nullptr;
    for (const auto& [id, tool] : geo->tools())
        if (std::abs(tool.tooldia - 0.5) < 1e-9) iso = &tool;
    ASSERT_NE(iso, nullptr);
    // Los mismos recorridos que el núcleo (verificado contra el original en test_ncc_isolation.cpp), con el margen de
    // fábrica (1,0) y en concordancia.
    flatcam::NccOptions o;
    o.margin = t->ncc_margin_entry->value();
    bool broken = false;
    const flatcam::ShapeList expected = flatcam::Ncc::isolationPaths(logo->gerber().solidGeometry(), 0.5, true,
                                                   flatcam::Ncc::clearingBox(logo->gerber().solidGeometry(), o), 64, broken);
    EXPECT_FALSE(expected.empty());
    EXPECT_EQ(iso->solidGeometry.size(), expected.size());
}

TEST(GuiNcc, OptimalToolDiameter) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* cu = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "B_Cu.gbr")));
    ASSERT_NE(cu, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(cu));
    NccTool* t = app.nccTool;
    app.ui()->ncc_btn->trigger();
    // "Óptimo": la menor distancia entre figuras del cobre (0,1994 mm en el original) va al campo de la herramienta nueva.
    t->find_optimal_button->click();
    EXPECT_DOUBLE_EQ(t->new_tooldia_entry->value(), 0.1994);
    EXPECT_TRUE(app.ui()->fcinfo->text->text().contains("0.1994"));
    // Isolation: lo mismo.
    IsolationTool* iso = app.isolationTool;
    app.ui()->isolation_btn->trigger();
    iso->find_optimal_button->click();
    EXPECT_DOUBLE_EQ(iso->new_tooldia_entry->value(), 0.1994);
}

TEST(GuiCanvas, GridSnapLikeTheOriginal) {
    App app;
    MainGUI* ui = app.ui();
    // De fábrica: ajuste activo, grilla de 1,0 enlazada.
    ASSERT_TRUE(ui->grid_snap_btn->isChecked());
    EXPECT_EQ(ui->snapToGrid({1.4, -2.6}), QPointF(1.0, -3.0));
    EXPECT_EQ(ui->snapToGrid({2.5, 0.5}), QPointF(2.0, 0.0));  // round() de Python: al par en los empates
    // Con "enlazar", la Y copia la X; sin enlace cada una la suya.
    ui->grid_gap_x_entry->setText("0.25");
    EXPECT_EQ(ui->grid_gap_y_entry->text(), "0.2500");
    EXPECT_EQ(ui->snapToGrid({1.13, 1.13}), QPointF(1.25, 1.25));
    ui->grid_gap_link_cb->setChecked(false);
    ui->grid_gap_y_entry->setText("0");
    EXPECT_EQ(ui->snapToGrid({1.13, 1.13}), QPointF(1.25, 1.13));  // grilla 0: no ajusta
    // Las señales del canvas salen ajustadas.
    ui->grid_gap_link_cb->setChecked(true);
    ui->grid_gap_x_entry->setText("1.0");
    PlotCanvas* canvas = ui->plotCanvas();
    canvas->resize(400, 300);
    QPointF released;
    QObject::connect(canvas, &PlotCanvas::mouseReleased, [&](QPointF w, Qt::MouseButton, bool) { released = w; });
    const QPointF pixel = canvas->toPixel({3.3, 4.6});
    QMouseEvent press(QEvent::MouseButtonPress, pixel, canvas->mapToGlobal(pixel), Qt::LeftButton, Qt::LeftButton,
                      Qt::NoModifier);
    QMouseEvent release(QEvent::MouseButtonRelease, pixel, canvas->mapToGlobal(pixel), Qt::LeftButton, Qt::NoButton,
                        Qt::NoModifier);
    QApplication::sendEvent(canvas, &press);
    QApplication::sendEvent(canvas, &release);
    EXPECT_NEAR(released.x(), 3.0, 1e-12);
    EXPECT_NEAR(released.y(), 5.0, 1e-12);
    // Sin ajuste, la posición tal cual.
    ui->grid_snap_btn->trigger();
    QApplication::sendEvent(canvas, &press);
    QApplication::sendEvent(canvas, &release);
    EXPECT_NEAR(released.x(), 3.3, 1e-6);
    // Elegir polígonos apaga el ajuste y lo restaura al terminar.
    ui->grid_snap_btn->trigger();
    ASSERT_TRUE(ui->grid_snap_btn->isChecked());
    app.picker()->start(CanvasPicker::Mode::PickPolygons, {}, [](const flatcam::ShapeList&) {});
    EXPECT_FALSE(ui->grid_snap_btn->isChecked());
    app.picker()->cancel();
    EXPECT_TRUE(ui->grid_snap_btn->isChecked());
}

TEST(GuiDrilling, ExclusionAreas) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(exc, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    DrillingTool* t = app.drillingTool;
    app.runTool(t, DrillingTool::info());
    t->setToolUi();
    // La casilla muestra la sección.
    t->exclusion_cb->setEnabled(true);
    t->exclusion_cb->setChecked(true);
    EXPECT_FALSE(t->exclusion_frame->isHidden());
    // "Alrededor" deshabilita la Z.
    t->strategy_radio->setValue("around");
    Q_EMIT t->strategy_radio->activatedCustom("around");
    EXPECT_FALSE(t->over_z_entry->isEnabled());
    t->strategy_radio->setValue("over");
    Q_EMIT t->strategy_radio->activatedCustom("over");
    t->over_z_entry->setValue(7.5);
    // Un rectángulo lejos de la placa (dos clics y clic derecho), con la grilla de 1 mm.
    t->area_shape_radio->setValue("square");
    PlotCanvas* canvas = app.ui()->plotCanvas();
    auto click = [&](double x, double y, Qt::MouseButton b) { Q_EMIT canvas->mouseReleased(QPointF(x, y), b, false); };
    t->add_area_button->click();
    click(100, 100, Qt::LeftButton);
    click(110, 110, Qt::LeftButton);
    click(110, 110, Qt::RightButton);
    ASSERT_EQ(app.excAreas()->areas().size(), 1u);
    EXPECT_EQ(app.excAreas()->areas()[0].strategy, "over");
    EXPECT_DOUBLE_EQ(app.excAreas()->areas()[0].overZ, 7.5);
    ASSERT_EQ(t->exclusion_table->rowCount(), 1);
    EXPECT_EQ(t->exclusion_table->item(0, 1)->text(), "excellon");
    EXPECT_EQ(t->exclusion_table->item(0, 3)->text(), "7.5");
    EXPECT_TRUE(t->generate_cnc_button->styleSheet().contains("orange"));
    // El CNC Job se genera con las áreas (lejos de los agujeros: no cambian los viajes).
    EXPECT_NE(t->generateCnc(), nullptr);
    // Borrar las elegidas sin elegir nada avisa; con la fila elegida, se borra.
    t->exclusion_table->clearSelection();
    t->delete_sel_area_button->click();
    EXPECT_EQ(app.excAreas()->areas().size(), 1u);
    t->exclusion_table->selectRow(0);
    t->delete_sel_area_button->click();
    EXPECT_TRUE(app.excAreas()->areas().empty());
    EXPECT_EQ(t->exclusion_table->rowCount(), 0);
    EXPECT_FALSE(t->generate_cnc_button->styleSheet().contains("orange"));
    // Un área sobre la placa corta el Excellon: se borran todas.
    t->add_area_button->click();
    click(-10, -10, Qt::LeftButton);
    click(10, 10, Qt::LeftButton);
    click(10, 10, Qt::RightButton);
    EXPECT_TRUE(app.excAreas()->areas().empty());
    EXPECT_TRUE(app.ui()->fcinfo->text->text().contains("Exclus") || app.ui()->fcinfo->text->text().contains("exclus"));
}

TEST(GuiGeometry, PolishAddsAToolOverTheObjectBox) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    app.ui()->cutout_btn->trigger();
    auto* geo = dynamic_cast<GeometryObject*>(app.cutoutTool->freeformCutout());
    ASSERT_NE(geo, nullptr);
    ASSERT_EQ(geo->tools().size(), 1u);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(geo));
    auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    // La casilla muestra los parámetros del pulido.
    EXPECT_TRUE(ui->polish_dia_entry->isHidden());
    ui->polish_cb->setChecked(true);
    EXPECT_FALSE(ui->polish_dia_entry->isHidden());
    ui->polish_dia_entry->setValue(10.0);
    ui->polish_over_entry->setValue(5.0);
    ui->polish_margin_entry->setValue(1.0);
    ui->generate_cnc_button->click();
    // Se agrega la herramienta "Pulir" (2) y el CNC Job lleva las dos.
    ASSERT_EQ(geo->tools().size(), 2u);
    const GeometryTool& polish = geo->tools().at(2);
    EXPECT_DOUBLE_EQ(polish.tooldia, 10.0);
    EXPECT_EQ(polish.type, QString::fromUtf8("Limpiar"));  // tr_("Polish")
    EXPECT_DOUBLE_EQ(polish.data.cutZ, -1.0);              // geometry_polish_pressure
    EXPECT_FALSE(polish.solidGeometry.empty());
    auto* cnc = dynamic_cast<CNCJobObject*>(app.collection()->byName(geo->name() + "_cnc"));
    ASSERT_NE(cnc, nullptr);
    EXPECT_EQ(cnc->tools().size(), 2u);
    EXPECT_NE(cnc->job().gcode().find("T2"), std::string::npos);
}

TEST(GuiToolsDatabase, EditSaveAndUseFromTheTools) {
    QTemporaryDir data;
    App app;
    app.setDataPath(data.path());
    flatcam::gui::ToolsDatabase::ensureFile(data.path());
    const QString path = flatcam::gui::ToolsDatabase::fileName(data.path());
    QFile f0(path);
    ASSERT_TRUE(f0.open(QIODevice::ReadOnly));
    EXPECT_EQ(f0.readAll(), "{}");  // vacía, como la crea el original
    f0.close();
    // Opciones -> Base de datos de herramientas.
    app.ui()->menuoptions_tools_db->trigger();
    FCDetachableTab2* area = app.ui()->plot_tab_area;
    auto* tab = qobject_cast<ToolsDBTab*>(area->currentWidget());
    ASSERT_NE(tab, nullptr);
    EXPECT_EQ(area->tabText(area->currentIndex()), QString::fromUtf8("Base de Datos de Herramientas"));
    EXPECT_FALSE(tab->milling_box->isEnabled());  // base vacía: todo deshabilitado
    // Una herramienta nueva: new_tool_1 de 2,4 con los valores de fábrica; como Aislamiento de 0,15 con 3 pasadas.
    tab->addTool();
    ASSERT_EQ(tab->tree_widget->topLevelItemCount(), 1);
    EXPECT_EQ(tab->tree_widget->topLevelItem(0)->text(1), "new_tool_1");
    EXPECT_DOUBLE_EQ(tab->dia_entry->value(), 2.4);
    tab->tool_op_combo->setIndexValue(3);
    EXPECT_TRUE(tab->iso_box->isVisible() || !tab->iso_box->isHidden());
    EXPECT_TRUE(tab->ncc_box->isHidden());
    tab->dia_entry->setValue(0.15);
    tab->iso_passes_entry->setValue(3);
    EXPECT_TRUE(tab->changed());
    tab->save();
    QFile f(path);
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    const QByteArray saved = f.readAll();
    EXPECT_TRUE(saved.startsWith("{\n  \"1\": {\n    \"name\": \"new_tool_1\"")) << saved.left(120).toStdString();
    EXPECT_TRUE(saved.contains("\"tool_target\": 3"));
    EXPECT_TRUE(saved.contains("\"tools_iso_passes\": 3"));
    app.closeToolsDatabase();
    EXPECT_EQ(qobject_cast<ToolsDBTab*>(area->currentWidget()), nullptr);

    // Isolation: "Buscar y agregar" de 0,15 la toma de la base (3 pasadas).
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* cu = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    ASSERT_NE(cu, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(cu));
    IsolationTool* iso = app.isolationTool;
    app.ui()->isolation_btn->trigger();
    const std::size_t before = iso->tools().size();
    iso->new_tooldia_entry->setValue(0.15);
    iso->search_and_add_btn->click();
    ASSERT_EQ(iso->tools().size(), before + 1);
    EXPECT_EQ(iso->tools().rbegin()->second.passes, 3);
    EXPECT_EQ(app.ui()->fcinfo->text->text(), tr_("New tool added to Tool Table from Tools Database."));
    // NCC compara con un texto: nunca la encuentra y agrega la de fábrica.
    NccTool* ncc = app.nccTool;
    app.ui()->ncc_btn->trigger();
    const std::size_t nccBefore = ncc->tools().size();
    ncc->new_tooldia_entry->setValue(0.15);
    ncc->add_newtool_button->click();
    ASSERT_EQ(ncc->tools().size(), nccBefore + 1);
    EXPECT_DOUBLE_EQ(ncc->tools().rbegin()->second.overlap, 40.0);  // la de fábrica
    // "Agregar desde la base" en NCC: el objetivo es Aislamiento (3), no sirve.
    ncc->addtool_from_db_btn->click();
    tab = qobject_cast<ToolsDBTab*>(area->currentWidget());
    ASSERT_NE(tab, nullptr);
    tab->tree_widget->topLevelItem(0)->setSelected(true);
    tab->requestTool();
    EXPECT_EQ(ncc->tools().size(), nccBefore + 1);
    EXPECT_EQ(qobject_cast<ToolsDBTab*>(area->currentWidget()), nullptr);  // la pestaña se cierra
    // En Isolation sí: entra con sus datos.
    iso->tools_table->selectAll();
    iso->deltool_btn->click();
    iso->addtool_from_db_btn->click();
    tab = qobject_cast<ToolsDBTab*>(area->currentWidget());
    ASSERT_NE(tab, nullptr);
    tab->tree_widget->topLevelItem(0)->setSelected(true);
    tab->requestTool();
    ASSERT_EQ(iso->tools().size(), 1u);
    EXPECT_DOUBLE_EQ(iso->tools().begin()->second.geo.tooldia, 0.15);
    EXPECT_EQ(iso->tools().begin()->second.passes, 3);
}

TEST(GuiGeometry, AddAndDeleteTools) {
    QTemporaryDir data;
    App app;
    app.setDataPath(data.path());
    flatcam::gui::ToolsDatabase::ensureFile(data.path());
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    app.ui()->cutout_btn->trigger();
    auto* geo = dynamic_cast<GeometryObject*>(app.cutoutTool->freeformCutout());
    ASSERT_NE(geo, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(geo));
    auto* ui = qobject_cast<GeometryObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    // No está en la base (vacía): copia de la última herramienta con el diámetro nuevo.
    ui->addtool_entry->setValue(0.8);
    ui->search_and_add_btn->click();
    ASSERT_EQ(geo->tools().size(), 2u);
    EXPECT_DOUBLE_EQ(geo->tools().at(2).tooldia, 0.8);
    EXPECT_EQ(geo->tools().at(2).solidGeometry.size(), geo->tools().at(1).solidGeometry.size());
    EXPECT_EQ(ui->geo_tools_table->rowCount(), 2);
    EXPECT_EQ(app.ui()->fcinfo->text->text(), tr_("Tool added in Tool Table."));
    // Borrar sin elegir avisa; con la fila elegida, se borra.
    ui->geo_tools_table->clearSelection();
    ui->deltool_btn->click();
    EXPECT_EQ(geo->tools().size(), 2u);
    ui->geo_tools_table->selectRow(1);
    ui->deltool_btn->click();
    EXPECT_EQ(geo->tools().size(), 1u);
}

TEST(GuiTransform, RotateScaleOffsetFlipAndBuffer) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(exc, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    app.ui()->transform_btn->trigger();
    TransformTool* t = app.transformTool;
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), t);
    EXPECT_EQ(t->ref_combo->currentIndex(), 1);   // Selección
    EXPECT_TRUE(t->point_entry->isHidden());
    EXPECT_FALSE(t->scaley_entry->isEnabled());  // enlazado
    const QRectF b0 = edge->bounds();
    // Desplazar en X.
    t->offx_entry->setValue(5.0);
    t->offx_button->click();
    EXPECT_NEAR(edge->bounds().left(), b0.left() + 5, 1e-9);
    EXPECT_NEAR(edge->bounds().width(), b0.width(), 1e-9);
    // Rotar 90 alrededor del centro de la selección: ancho y alto se intercambian.
    const QRectF b1 = edge->bounds();
    t->rotate_entry->setValue(90);
    t->rotate_button->click();
    EXPECT_NEAR(edge->bounds().width(), b1.height(), 1e-6);
    EXPECT_NEAR(edge->bounds().height(), b1.width(), 1e-6);
    EXPECT_NEAR(edge->bounds().center().x(), b1.center().x(), 1e-6);
    // Escalar por 2 (enlazado) desde el origen.
    t->ref_combo->setCurrentIndex(0);
    const QRectF b2 = edge->bounds();
    t->scalex_entry->setValue(2.0);
    t->scalex_button->click();
    EXPECT_NEAR(edge->bounds().left(), 2 * b2.left(), 1e-6);
    EXPECT_NEAR(edge->bounds().top(), 2 * b2.top(), 1e-6);
    // Reflejar las X alrededor de un punto.
    t->ref_combo->setCurrentIndex(2);
    EXPECT_FALSE(t->point_entry->isHidden());
    t->point_entry->setText("10, 0");
    const QRectF b3 = edge->bounds();
    t->flipx_button->click();
    EXPECT_NEAR(edge->bounds().left(), 20 - b3.right(), 1e-6);
    // Engrosar el Excellon 0,2: el diámetro de cada herramienta crece 0,2.
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(exc));
    const double d0 = exc->excellon().drillTools().begin()->second.tooldia;
    t->buffer_entry->setValue(0.2);
    t->buffer_button->click();
    EXPECT_NEAR(exc->excellon().drillTools().begin()->second.tooldia, d0 + 0.2, 1e-12);
    // Sin nada elegido avisa.
    app.ui()->collection_view->clearSelection();
    t->offy_entry->setValue(1.0);
    t->offy_button->click();
    EXPECT_EQ(app.ui()->fcinfo->text->text(), tr_("No object is selected."));
}

// Distancia (ToolDistance): dos clics, el resultado y que se restauren la grilla y los manejadores del mouse.
TEST(GuiDistance, MeasuresBetweenTwoClicksAndCancels) {
    App app;
    MainGUI* ui = app.ui();
    const bool grid = ui->grid_snap_btn->isChecked();
    ui->distance_btn->trigger();
    DistanceTool* t = app.distanceTool;
    ASSERT_TRUE(t->active());
    EXPECT_EQ(ui->tool_scroll_area->widget(), t);
    EXPECT_FALSE(t->measure_btn->isEnabled());
    EXPECT_TRUE(t->snap_center_cb->isHidden());
    EXPECT_FALSE(ui->canvasHandlersEnabled());
    t->onRelease({1, 1}, Qt::LeftButton, false);
    EXPECT_EQ(t->start_entry->text(), "(1.0000, 1.0000)");
    t->onMove({4, 1});
    EXPECT_EQ(t->angle_entry->text(), "0.0000");  // ángulo provisorio
    t->onRelease({4, 5}, Qt::LeftButton, false);
    EXPECT_FALSE(t->active());
    EXPECT_EQ(t->stop_entry->text(), "(4.0000, 5.0000)");
    EXPECT_EQ(t->distance_x_entry->text(), "3.0000");
    EXPECT_EQ(t->distance_y_entry->text(), "4.0000");
    EXPECT_EQ(t->angle_entry->text(), "53.1301");
    EXPECT_EQ(t->total_distance_entry->text(), "5.0000");
    EXPECT_TRUE(ui->fcinfo->text->text().contains("5.000000")) << ui->fcinfo->text->text().toStdString();
    EXPECT_TRUE(t->measure_btn->isEnabled());
    EXPECT_TRUE(ui->canvasHandlersEnabled());
    EXPECT_EQ(ui->grid_snap_btn->isChecked(), grid);
    // Con el botón "Medida" se vuelve a activar; el clic derecho (sin arrastre) cancela, uno con arrastre no.
    t->measure_btn->click();
    ASSERT_TRUE(t->active());
    t->onRelease({0, 0}, Qt::RightButton, true);
    EXPECT_TRUE(t->active());
    t->onRelease({0, 0}, Qt::RightButton, false);
    EXPECT_FALSE(t->active());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Distance Tool cancelled."));
    // Ctrl+M dos veces: activa y termina.
    QKeyEvent ctrlM(QEvent::KeyPress, Qt::Key_M, Qt::ControlModifier);
    QApplication::sendEvent(ui, &ctrlM);
    EXPECT_TRUE(t->active());
    QApplication::sendEvent(ui, &ctrlM);
    EXPECT_FALSE(t->active());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Distance Tool finished."));
}

// Distancia mínima (ToolDistanceMin): los puntos más cercanos entre los dos objetos elegidos.
TEST(GuiDistance, MinimumDistanceBetweenTwoObjects) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(exc, nullptr);
    MainGUI* ui = app.ui();
    ui->distance_min_btn->trigger();
    DistanceMinTool* t = app.distanceMinTool;
    EXPECT_EQ(ui->tool_scroll_area->widget(), t);
    EXPECT_FALSE(t->jump_hp_btn->isEnabled());
    // Con un solo objeto elegido avisa.
    auto* view = ui->collection_view;
    view->selectionModel()->clearSelection();
    view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    t->measure_btn->click();
    EXPECT_TRUE(ui->fcinfo->text->text().endsWith(" 1")) << ui->fcinfo->text->text().toStdString();
    view->selectionModel()->select(app.collection()->indexOf(exc), QItemSelectionModel::Select);
    t->measure_btn->click();
    const double expected =
        flatcam::Shape::unaryUnion(edge->solidShapes()).distance(flatcam::Shape::unaryUnion(exc->solidShapes()));
    ASSERT_GT(expected, 0.1);
    EXPECT_EQ(t->total_distance_entry->text(), QString::asprintf("%.4f", expected));
    const QPointF h = t->halfPoint();
    EXPECT_EQ(t->half_point_entry->text(), QString::asprintf("(%.4f, %.4f)", h.x(), h.y()));
    EXPECT_TRUE(t->jump_hp_btn->isEnabled());
    t->jump_hp_btn->click();
    EXPECT_TRUE(ui->position_label->text().contains(QString::asprintf("%.4f", h.x())));
    EXPECT_TRUE(ui->fcinfo->text->text().contains(QString::asprintf("(%.4f, %.4f)", h.x(), h.y())));
}

// Mover (ToolMove): sin objetos avisa; con dos clics desplaza los elegidos; Escape cancela.
TEST(GuiMove, MovesTheSelectedObjectsWithTwoClicks) {
    App app;
    MainGUI* ui = app.ui();
    MoveTool* m = app.moveTool;
    m->toggle();
    EXPECT_FALSE(m->active());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Cancelled. No object(s) to move."));
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(exc, nullptr);
    auto* view = ui->collection_view;
    view->selectionModel()->clearSelection();
    view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    view->selectionModel()->select(app.collection()->indexOf(exc), QItemSelectionModel::Select);
    const QRectF be = edge->bounds(), bx = exc->bounds();
    QKeyEvent keyM(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier);
    QApplication::sendEvent(ui, &keyM);
    ASSERT_TRUE(m->active());
    m->onClick({1, 2}, Qt::LeftButton);
    m->onMove({5, 5});
    m->onClick({11, -3}, Qt::LeftButton);
    EXPECT_FALSE(m->active());
    EXPECT_NEAR(edge->bounds().left(), be.left() + 10, 1e-9);
    EXPECT_NEAR(edge->bounds().top(), be.top() - 5, 1e-9);
    EXPECT_NEAR(exc->bounds().left(), bx.left() + 10, 1e-9);
    EXPECT_TRUE(ui->fcinfo->text->text().startsWith("Excellon ")) << ui->fcinfo->text->text().toStdString();
    // Escape cancela sin mover.
    m->toggle();
    ASSERT_TRUE(m->active());
    m->onClick({0, 0}, Qt::LeftButton);
    QKeyEvent esc(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    QApplication::sendEvent(ui, &esc);
    EXPECT_FALSE(m->active());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Cancelled."));
    EXPECT_NEAR(edge->bounds().left(), be.left() + 10, 1e-9);
}

// Propiedades (ToolProperties): el árbol con los datos de los objetos elegidos.
TEST(GuiPropertiesTool, ShowsTheSelectedObjects) {
    App app;
    MainGUI* ui = app.ui();
    PropertiesTool* t = app.propertiesTool;
    // Sin objetos: avisa y vuelve a la pestaña Proyecto.
    ui->properties_tool_menuAction->trigger();
    EXPECT_EQ(ui->notebook->currentWidget(), ui->project_tab);
    EXPECT_TRUE(t->properties_frame->isHidden());
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(exc, nullptr);
    auto* view = ui->collection_view;
    view->selectionModel()->clearSelection();
    view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    view->selectionModel()->select(app.collection()->indexOf(exc), QItemSelectionModel::Select);
    QKeyEvent keyP(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier);
    QApplication::sendEvent(ui, &keyP);
    EXPECT_EQ(ui->tool_scroll_area->widget(), t);
    EXPECT_EQ(ui->notebook->tabText(2), tr_("Properties Tool"));
    QTreeWidget* tree = t->treeWidget;
    // Dos objetos: TIPO, NOMBRE, Dimensiones, Unidades, Opciones, Aperturas | Herramientas [, Taladros, Ranuras], separador.
    ASSERT_EQ(tree->topLevelItemCount(), 7 + 9);
    auto find = [](QTreeWidgetItem* parent, const QString& text) -> QTreeWidgetItem* {
        for (int i = 0; i < parent->childCount(); ++i)
            if (parent->child(i)->text(0) == text) return parent->child(i);
        return nullptr;
    };
    QTreeWidgetItem* type = tree->topLevelItem(0);
    ASSERT_GE(type->childCount(), 2);
    EXPECT_EQ(type->child(0)->text(1), "GERBER");
    EXPECT_EQ(tree->topLevelItem(1)->child(0)->text(0), edge->name());
    QTreeWidgetItem* dims = tree->topLevelItem(2);
    QTreeWidgetItem* len = find(dims, tr_("Length") + ":");
    ASSERT_NE(len, nullptr);
    EXPECT_EQ(len->text(1), QString::asprintf("%.4f mm", edge->bounds().width()));
    QTreeWidgetItem* copper = find(dims, tr_("Copper Area") + ":");
    ASSERT_NE(copper, nullptr);
    EXPECT_NE(copper->text(1), "0.0000 cm2");
    QTreeWidgetItem* opts = tree->topLevelItem(4);
    EXPECT_GT(opts->childCount(), 10);
    QTreeWidgetItem* plot = find(opts, "plot");
    ASSERT_NE(plot, nullptr);
    EXPECT_EQ(plot->text(1), "True");
    QTreeWidgetItem* aps = tree->topLevelItem(5);
    EXPECT_EQ(aps->childCount(), static_cast<int>(edge->gerber().apertureOrder().size()));
    // El Excellon: herramientas con diámetro y cantidad de agujeros, y el total.
    EXPECT_EQ(tree->topLevelItem(7)->child(0)->text(1), "EXCELLON");
    QTreeWidgetItem* drills = tree->topLevelItem(7 + 6);
    ASSERT_EQ(drills->childCount(), 1);
    int total = 0;
    for (const auto& [id, tool] : exc->excellon().drillTools()) total += static_cast<int>(tool.drills.size());
    EXPECT_EQ(drills->child(0)->text(1), QString::number(total));
    // P otra vez con el panel a la vista: lo esconde.
    QApplication::sendEvent(ui, &keyP);
    EXPECT_EQ(ui->splitter->sizes().value(0), 0);
}

// Propiedades de un objeto: escala y desplazamiento (on_scale_button_click, on_offset_button_click) y "Transformaciones".
TEST(GuiTransform, ScaleAndOffsetFromThePropertiesPanel) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(edge));
    auto* p = qobject_cast<GerberObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(p, nullptr);
    const QRectF b0 = edge->bounds();
    p->offsetvector_entry->setText("(1, 2*3)");
    p->offset_button->click();
    EXPECT_NEAR(edge->bounds().left(), b0.left() + 1, 1e-9);
    EXPECT_NEAR(edge->bounds().top(), b0.top() + 6, 1e-9);
    const QRectF b1 = edge->bounds();
    p->scale_entry->setText("1/2");
    p->scale_button->click();
    EXPECT_NEAR(edge->bounds().left(), b1.left() / 2, 1e-9);
    EXPECT_NEAR(edge->bounds().width(), b1.width() / 2, 1e-9);
    EXPECT_EQ(app.ui()->fcinfo->text->text(), tr_("Scale done."));
    p->transformations_button->click();
    EXPECT_EQ(app.ui()->tool_scroll_area->widget(), app.transformTool);
}

// Elegir objetos con el mouse en el canvas (select_objects, selection_area_handler, Shift para copiar coordenadas).
TEST(GuiCanvasSelection, ClickAndAreaSelection) {
    App app;
    MainGUI* ui = app.ui();
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(exc, nullptr);
    auto sel = [&] { return app.selectedObjects(); };
    auto click = [&](QPointF p, Qt::KeyboardModifiers m = Qt::NoModifier) {
        app.onCanvasPress(p, Qt::LeftButton);
        app.onCanvasMove(p, false);
        app.onCanvasRelease(p, Qt::LeftButton, m);
    };
    ui->collection_view->selectionModel()->clearSelection();
    // Fuera de todo: nada elegido.
    click({500, 500});
    EXPECT_TRUE(sel().empty());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Objects selection is cleared."));
    // Un punto del borde de la placa, fuera de la caja del Excellon: solo el contorno.
    const QRectF be = edge->bounds(), bx = exc->bounds();
    const QPointF onlyEdge(be.left() + 0.01, be.center().y());
    ASSERT_FALSE(bx.contains(onlyEdge));
    click(onlyEdge);
    ASSERT_EQ(sel().size(), 1u);
    EXPECT_EQ(sel().front(), edge);
    EXPECT_EQ(ui->properties_scroll_area->widget(), edge->propertiesUi());
    // Un punto dentro de las dos cajas: cada clic pasa al siguiente objeto.
    const QPointF both = bx.center();
    click(both);
    ASSERT_EQ(sel().size(), 1u);
    FlatCAMObj* first = sel().front();
    click(both);
    ASSERT_EQ(sel().size(), 1u);
    EXPECT_NE(sel().front(), first);
    // Rectángulo de izquierda a derecha que encierra solo el Excellon.
    app.onCanvasPress(bx.topLeft() - QPointF(0.5, 0.5), Qt::LeftButton);
    app.onCanvasMove(bx.bottomRight() + QPointF(0.5, 0.5), true);
    app.onCanvasRelease(bx.bottomRight() + QPointF(0.5, 0.5), Qt::LeftButton, Qt::NoModifier);
    ASSERT_EQ(sel().size(), 1u);
    EXPECT_EQ(sel().front(), exc);
    // De derecha a izquierda, tocando: los dos (como el original, se comparan las cajas de los objetos).
    app.onCanvasPress({be.right() + 1, be.center().y()}, Qt::LeftButton);
    app.onCanvasMove({bx.center().x(), be.center().y() + 0.1}, true);
    app.onCanvasRelease({bx.center().x(), be.center().y() + 0.1}, Qt::LeftButton, Qt::NoModifier);
    EXPECT_EQ(sel().size(), 2u);
    // Shift copia las coordenadas del clic.
    click({1.23456, -7}, Qt::ShiftModifier);
    EXPECT_EQ(QApplication::clipboard()->text(), "(1.2346, -7.0000)");
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Coordinates copied to clipboard."));
    // Un clic sobre un objeto ya elegido quita la selección (como el original); otro lo vuelve a elegir.
    click(onlyEdge);
    if (!sel().empty()) click(onlyEdge);
    EXPECT_TRUE(sel().empty());
    click(onlyEdge);
    ASSERT_EQ(sel().size(), 1u);
    // Doble clic con algo elegido: la pestaña Propiedades.
    ui->notebook->setCurrentWidget(ui->project_tab);
    app.onCanvasDoubleClick(Qt::LeftButton);
    click(onlyEdge);
    EXPECT_EQ(ui->notebook->currentWidget(), ui->properties_tab);
}

// Menús Editar y Ver, el menú del canvas y Archivo -> Nuevo proyecto.
TEST(GuiEditViewMenus, EditViewAndCanvasMenu) {
    App app;
    MainGUI* ui = app.ui();
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* exc = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(exc, nullptr);
    auto* view = ui->collection_view;
    // Ver: ocultar los no elegidos y mostrar todos.
    view->selectionModel()->clearSelection();
    view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    ui->menuviewdisableother->trigger();
    EXPECT_TRUE(edge->plotEnabled());
    EXPECT_FALSE(exc->plotEnabled());
    ui->menuviewenable->trigger();
    EXPECT_TRUE(exc->plotEnabled());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("All plots enabled."));
    // Editar -> Elegir todo.
    ui->menueditselectall->trigger();
    EXPECT_EQ(app.selectedObjects().size(), 2u);
    // Mover al origen: la esquina inferior izquierda de la caja de los dos va a (0, 0).
    ui->menuedit_move2origin->trigger();
    const QRectF b = edge->bounds().united(exc->bounds());
    EXPECT_NEAR(b.left(), 0, 1e-9);
    EXPECT_NEAR(b.top(), 0, 1e-9);
    // Fijar el origen con un clic: todo se mueve.
    const QRectF be = edge->bounds();
    ui->menueditorigin->trigger();
    Q_EMIT ui->plotCanvas()->mouseClicked(QPointF(2, 3), Qt::RightButton);  // otro botón no cuenta
    EXPECT_NEAR(edge->bounds().left(), be.left(), 1e-9);
    Q_EMIT ui->plotCanvas()->mouseClicked(QPointF(2, 3), Qt::LeftButton);
    EXPECT_NEAR(edge->bounds().left(), be.left() - 2, 1e-9);
    EXPECT_NEAR(edge->bounds().top(), be.top() - 3, 1e-9);
    Q_EMIT ui->plotCanvas()->mouseClicked(QPointF(2, 3), Qt::LeftButton);  // ya no espera el clic
    EXPECT_NEAR(edge->bounds().left(), be.left() - 2, 1e-9);
    // Saltar a: el diálogo con el portapapeles y la posición escrita.
    QApplication::clipboard()->setText("(1.5, 2)");
    QTimer::singleShot(0, [] {
        auto* d = qobject_cast<DialogBoxRadio*>(QApplication::activeModalWidget());
        ASSERT_NE(d, nullptr);
        EXPECT_EQ(d->lineEdit->text(), "1.5, 2");
        d->lineEdit->setText("10, 2*3");
        d->accept();
    });
    ui->menueditjump->trigger();
    EXPECT_TRUE(ui->position_label->text().contains("10.0000"));
    EXPECT_TRUE(ui->position_label->text().contains("6.0000"));
    // Ubicar: la esquina superior derecha del objeto activo.
    view->selectionModel()->clearSelection();
    view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    QTimer::singleShot(0, [] {
        auto* d = qobject_cast<DialogBoxChoice*>(QApplication::activeModalWidget());
        ASSERT_NE(d, nullptr);
        d->ref_radio->setValue("tr");
        d->accept();
    });
    ui->menueditlocate->trigger();
    EXPECT_TRUE(ui->position_label->text().contains(QString::asprintf("%.4f", edge->bounds().right())));
    // Menú del canvas: clic derecho sin arrastrar; las grillas de la lista cambian la grilla.
    app.onCanvasRelease({0, 0}, Qt::RightButton, Qt::NoModifier, true);
    EXPECT_FALSE(app.canvasMenu()->isVisible());
    app.onCanvasRelease({0, 0}, Qt::RightButton, Qt::NoModifier, false);
    QMenu* grids = nullptr;
    for (QAction* a : app.canvasMenu()->actions())
        if (a->menu() && a->text() == tr_("Grids")) grids = a->menu();
    ASSERT_NE(grids, nullptr);
    QAction* g254 = nullptr;
    for (QAction* a : grids->actions())
        if (a->text() == "2.54") g254 = a;
    ASSERT_NE(g254, nullptr);
    g254->trigger();
    EXPECT_EQ(ui->grid_gap_x_entry->text(), "2.54");
    app.canvasMenu()->hide();
    // Ver -> área de trabajo y área de dibujo.
    ui->menuview_toggle_workspace->trigger();
    EXPECT_EQ(ui->wplace_label->text(), "A4");
    ui->menuview_toggle_parea->trigger();
    EXPECT_NE(ui->plot_tab_area->indexOf(ui->plot_tab), 0);
    ui->menuview_toggle_parea->trigger();
    EXPECT_EQ(ui->plot_tab_area->indexOf(ui->plot_tab), 0);
    // Archivo -> Nuevo proyecto, contestando "No" (no guardar).
    QTimer::singleShot(0, [] {
        auto* box = qobject_cast<QMessageBox*>(QApplication::activeModalWidget());
        ASSERT_NE(box, nullptr);
        for (QAbstractButton* bt : box->buttons())
            if (bt->text() == tr_("No")) bt->click();
    });
    ui->menufilenewproject->trigger();
    EXPECT_TRUE(app.collection()->objects().empty());
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("New Project created") + "...");
}

// Editar -> Unir: Gerber, Excellon y Geometría (on_edit_join_grb, on_edit_join_exc, on_edit_join).
TEST(GuiEditViewMenus, JoinObjects) {
    App app;
    MainGUI* ui = app.ui();
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* logo = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Logo.gbr")));
    auto* pth = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    // El NPTH.drl de la placa está vacío (no se crea el objeto): el segundo Excellon es el taladro del dosificador.
    auto* npth = static_cast<ExcellonObject*>(
        app.openByExtension(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Drill.drl")));
    ASSERT_TRUE(edge && logo && pth && npth);
    auto* view = ui->collection_view;
    auto select = [&](std::initializer_list<FlatCAMObj*> objs) {
        view->selectionModel()->clearSelection();
        for (FlatCAMObj* o : objs) view->selectionModel()->select(app.collection()->indexOf(o), QItemSelectionModel::Select);
    };
    // Con un objeto avisa.
    select({edge});
    EXPECT_EQ(app.joinGerber(), nullptr);
    EXPECT_TRUE(ui->fcinfo->text->text().endsWith(": 1"));
    // Gerber: la geometría de los dos y todas las aperturas (las repetidas, con otro número).
    select({edge, logo});
    ASSERT_TRUE(ui->menuedit_join_grb2grb);
    ui->menuedit_join_grb2grb->trigger();
    auto* g = dynamic_cast<GerberObject*>(app.collection()->byName("Combo_Gerber"));
    ASSERT_NE(g, nullptr);
    EXPECT_EQ(g->gerber().solidGeometry().size(), edge->gerber().solidGeometry().size() + logo->gerber().solidGeometry().size());
    EXPECT_EQ(g->gerber().apertures().size(), edge->gerber().apertures().size() + logo->gerber().apertures().size());
    // Excellon: los agujeros de los dos; las herramientas de igual diámetro, juntas.
    select({pth, npth});
    EXPECT_EQ(app.joinGerber(), nullptr);  // no son Gerber
    ui->menuedit_join_exc2exc->trigger();
    auto* e = dynamic_cast<ExcellonObject*>(app.collection()->byName("Combo_Excellon"));
    ASSERT_NE(e, nullptr);
    std::size_t drills = 0, expected = 0;
    std::set<double> dias;
    for (const auto& [id, t] : e->excellon().drillTools()) drills += t.drills.size();
    for (ExcellonObject* x : {pth, npth})
        for (const auto& [id, t] : x->excellon().drillTools()) {
            expected += t.drills.size();
            dias.insert(std::round(t.tooldia * 1e4) / 1e4);
        }
    EXPECT_EQ(drills, expected);
    EXPECT_EQ(e->excellon().drillTools().size(), dias.size());
    // Geometría desde un Gerber y un Excellon (los dos de una sola herramienta).
    select({edge, pth});
    ui->menuedit_join2geo->trigger();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("Combo_SingleGeo"));
    ASSERT_NE(geo, nullptr);
    EXPECT_EQ(geo->solidGeometry().size(), edge->solidShapes().size() + pth->solidShapes().size());
    EXPECT_EQ(geo->tools().size(), 1u);
}

// Editar -> Convertir: Excellon -> Geometría -> Excellon, y Excellon -> Gerber.
TEST(GuiEditViewMenus, ConvertObjects) {
    App app;
    MainGUI* ui = app.ui();
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* pth = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_NE(pth, nullptr);
    auto* view = ui->collection_view;
    auto selectOnly = [&](FlatCAMObj* o) {
        view->selectionModel()->clearSelection();
        view->selectionModel()->select(app.collection()->indexOf(o), QItemSelectionModel::Select);
    };
    std::size_t drills = 0;
    for (const auto& [id, t] : pth->excellon().drillTools()) drills += t.drills.size();
    selectOnly(pth);
    ui->menueditconvert_any2geo->trigger();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName(pth->name() + "_conv"));
    ASSERT_NE(geo, nullptr);
    EXPECT_EQ(geo->solidGeometry().size(), drills);
    // De la Geometría a Excellon: un agujero por círculo, en su centro, con el diámetro del círculo.
    selectOnly(geo);
    ui->menueditconvert_any2excellon->trigger();
    auto* back = dynamic_cast<ExcellonObject*>(app.collection()->byName(geo->name() + "_conv"));
    ASSERT_NE(back, nullptr);
    // Como el original, el diámetro se compara exacto (varía en la última cifra según dónde está el círculo) y el agujero
    // se agrega a todas las herramientas con el mismo diámetro redondeado: se repiten agujeros. Las posiciones son las 120.
    std::set<std::pair<double, double>> backDrills, orig;
    for (const auto& [id, t] : back->excellon().drillTools())
        for (const flatcam::Point2D& p : t.drills) backDrills.insert({std::round(p.x * 1e6) / 1e6, std::round(p.y * 1e6) / 1e6});
    for (const auto& [id, t] : pth->excellon().drillTools())
        for (const flatcam::Point2D& p : t.drills) orig.insert({std::round(p.x * 1e6) / 1e6, std::round(p.y * 1e6) / 1e6});
    EXPECT_EQ(backDrills.size(), drills);
    EXPECT_EQ(backDrills, orig);
    for (const auto& [id, t] : back->excellon().drillTools()) {
        bool near = false;
        for (const auto& [id2, t2] : pth->excellon().drillTools()) near = near || std::abs(t.tooldia - t2.tooldia) < 1e-3;
        EXPECT_TRUE(near) << t.tooldia;
    }
    // Del Excellon a Gerber: una apertura por herramienta, desde la 10.
    selectOnly(pth);
    const std::vector<FlatCAMObj*> conv = app.convertToGerber();
    ASSERT_EQ(conv.size(), 1u);
    auto* grb = dynamic_cast<GerberObject*>(conv.front());
    ASSERT_NE(grb, nullptr);
    EXPECT_EQ(grb->gerber().apertures().size(), pth->excellon().drillTools().size());
    EXPECT_TRUE(grb->gerber().apertures().count("10"));
}

// Opciones -> Girar la selección e Inclinar en X (on_rotate, on_skewx) con su diálogo.
TEST(GuiEditViewMenus, RotateAndSkewSelection) {
    App app;
    MainGUI* ui = app.ui();
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    ui->collection_view->selectionModel()->clearSelection();
    ui->collection_view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    const QRectF b0 = edge->bounds();
    QTimer::singleShot(0, [] {
        auto* d = qobject_cast<FCInputDoubleSpinner*>(QApplication::activeModalWidget());
        ASSERT_NE(d, nullptr);
        EXPECT_DOUBLE_EQ(d->wdg->value(), 90.0);  // tools_transform_rotate
        d->accept();
    });
    ui->menuoptions_transform_rotate->trigger();
    EXPECT_NEAR(edge->bounds().width(), b0.height(), 1e-6);
    EXPECT_NEAR(edge->bounds().center().x(), b0.center().x(), 1e-6);
    EXPECT_EQ(ui->fcinfo->text->text(), tr_("Rotation done."));
    const QRectF b1 = edge->bounds();
    QTimer::singleShot(0, [] {
        auto* d = qobject_cast<FCInputDoubleSpinner*>(QApplication::activeModalWidget());
        ASSERT_NE(d, nullptr);
        d->wdg->setValue(45);
        d->accept();
    });
    ui->menuoptions_transform_skewx->trigger();
    // tan(45) = 1: el ancho crece en el alto (las esquinas redondeadas del contorno lo cambian en centésimas).
    EXPECT_NEAR(edge->bounds().width(), b1.width() + b1.height(), 0.1);
    EXPECT_NEAR(edge->bounds().left(), b1.left(), 0.1);
    EXPECT_GT(edge->bounds().width(), b1.width() + 0.9 * b1.height());
}

// Atajos de teclado de la aplicación (MainGUI.keyPressEvent).
TEST(GuiEditViewMenus, KeyboardShortcuts) {
    App app;
    MainGUI* ui = app.ui();
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    ASSERT_NE(edge, nullptr);
    auto key = [ui](int k, Qt::KeyboardModifiers m = Qt::NoModifier) {
        QKeyEvent ev(QEvent::KeyPress, k, m);
        QApplication::sendEvent(ui, &ev);
    };
    ui->collection_view->selectionModel()->clearSelection();
    ui->collection_view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    key(Qt::Key_C, Qt::ShiftModifier);  // copiar el nombre
    EXPECT_EQ(QApplication::clipboard()->text(), edge->name());
    key(Qt::Key_Space);  // "Dibujar" del elegido
    EXPECT_FALSE(edge->plotEnabled());
    key(Qt::Key_1, Qt::AltModifier);  // mostrar todos
    EXPECT_TRUE(edge->plotEnabled());
    key(Qt::Key_2, Qt::AltModifier);  // ocultar todos
    EXPECT_FALSE(edge->plotEnabled());
    key(Qt::Key_H, Qt::ShiftModifier);  // HUD
    EXPECT_FALSE(ui->plotCanvas()->hudEnabled());
    key(Qt::Key_A, Qt::ControlModifier);  // elegir todo
    EXPECT_EQ(app.selectedObjects().size(), 1u);
    key(Qt::Key_Escape);  // quitar la selección
    EXPECT_TRUE(app.selectedObjects().empty());
    // 1: el panel está escondido al empezar -> se muestra en Proyecto; otra vez 1 -> se esconde.
    ui->splitter->setSizes({0, 1});
    key(Qt::Key_1);
    EXPECT_NE(ui->splitter->sizes().value(0), 0);
    EXPECT_EQ(ui->notebook->currentWidget(), ui->project_tab);
    key(Qt::Key_1);
    EXPECT_EQ(ui->splitter->sizes().value(0), 0);
    key(Qt::Key_T, Qt::AltModifier);  // Transformaciones
    EXPECT_EQ(ui->tool_scroll_area->widget(), app.transformTool);
}

// Ayuda: Acerca de (generado del original, con las bibliotecas del port), Cómo empezar y la lista de atajos.
TEST(GuiEditViewMenus, HelpMenu) {
    App app;
    MainGUI* ui = app.ui();
    auto about = app.aboutDialog();
    EXPECT_FALSE(about->windowTitle().isEmpty());
    bool geos = false;
    for (QLabel* l : about->findChildren<QLabel*>()) geos = geos || l->text().contains("<td>GEOS</td>");
    EXPECT_TRUE(geos);
    auto* tabs = about->findChild<QTabWidget*>();
    ASSERT_NE(tabs, nullptr);
    EXPECT_GE(tabs->count(), 6);
    auto howto = app.howtoDialog();
    EXPECT_NE(howto->findChild<QTabWidget*>(), nullptr);
    // F3: la pestaña de atajos, con el texto del original.
    QKeyEvent f3(QEvent::KeyPress, Qt::Key_F3, Qt::NoModifier);
    QApplication::sendEvent(ui, &f3);
    QWidget* cur = ui->plot_tab_area->currentWidget();
    ASSERT_EQ(cur, app.shortcutsTab());
    EXPECT_EQ(ui->plot_tab_area->tabText(ui->plot_tab_area->currentIndex()), tr_("Key Shortcut List"));
    const auto edits = cur->findChildren<QTextEdit*>();
    ASSERT_EQ(edits.size(), 3);
    EXPECT_TRUE(edits[1]->toPlainText().contains("F3"));
}

// Salir (final_save): con cambios sin guardar pregunta; "Cancelar" no cierra; después de guardar no pregunta.
TEST(GuiEditViewMenus, FinalSaveAsksBeforeQuitting) {
    App app;
    EXPECT_TRUE(app.finalSave());  // sin objetos
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    ASSERT_NE(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")), nullptr);
    EXPECT_TRUE(app.shouldWeSave());
    QTimer::singleShot(0, [] {
        auto* box = qobject_cast<QMessageBox*>(QApplication::activeModalWidget());
        ASSERT_NE(box, nullptr);
        for (QAbstractButton* bt : box->buttons())
            if (bt->text() == tr_("Cancel")) bt->click();
    });
    QCloseEvent close;
    QApplication::sendEvent(app.ui(), &close);
    EXPECT_FALSE(close.isAccepted());
    QTemporaryDir tmp;
    ASSERT_TRUE(app.saveProject(tmp.filePath("p.FlatPrj")));
    EXPECT_FALSE(app.shouldWeSave());
    EXPECT_TRUE(app.finalSave());
}

// Abrir un G-code como CNC Job (open_gcode): el .nc del contorno de la placa; al exportarlo queda el mismo cuerpo.
TEST(GuiFiles, OpenGcodeAsCncJob) {
    App app;
    QTemporaryDir data;
    app.setDataPath(data.path());
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    const QString nc = QString::fromStdString(dir + "Edge_Cuts.gbr_cutout_cnc.nc");
    auto* cnc = dynamic_cast<CNCJobObject*>(app.openByExtension(nc));
    ASSERT_NE(cnc, nullptr);
    EXPECT_EQ(cnc->name(), "luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc.nc");
    EXPECT_GT(cnc->plotData().lines.size() + cnc->plotData().fill.size(), 100u);
    EXPECT_GT(cnc->bounds().width(), 60.0);
    QTemporaryDir tmp;
    const QString out = tmp.filePath("x.nc");
    ASSERT_TRUE(app.exportGcode(cnc, out));
    QFile a(nc), b(out);
    ASSERT_TRUE(a.open(QIODevice::ReadOnly | QIODevice::Text) && b.open(QIODevice::ReadOnly | QIODevice::Text));
    const QString orig = QString::fromUtf8(a.readAll()), mine = QString::fromUtf8(b.readAll());
    EXPECT_TRUE(mine.contains(orig.trimmed())) << "el texto original va entero después de la cabecera";
    // Un Gerber no es G-code.
    EXPECT_EQ(app.openGcode(QString::fromStdString(dir + "Edge_Cuts.gbr")), nullptr);
    EXPECT_TRUE(app.ui()->fcinfo->text->text().contains("CNCJob"));
}

// Exportar Gerber y Excellon (en pulgadas 2:4, de fábrica) y volver a abrirlos; mover regenera el texto fuente.
TEST(GuiFiles, ExportGerberAndExcellon) {
    App app;
    QTemporaryDir data;
    app.setDataPath(data.path());
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = static_cast<GerberObject*>(app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr")));
    auto* pth = static_cast<ExcellonObject*>(app.openByExtension(QString::fromStdString(dir + "PTH.drl")));
    ASSERT_TRUE(edge && pth);
    QTemporaryDir tmp;
    ASSERT_TRUE(app.exportObject(edge, tmp.filePath("e.gbr")));
    ASSERT_TRUE(app.exportObject(pth, tmp.filePath("p.drl")));
    auto* e2 = app.openGerber(tmp.filePath("e.gbr"));
    auto* p2 = app.openExcellon(tmp.filePath("p.drl"));
    ASSERT_TRUE(e2 && p2);
    // El factor a pulgadas es 0,03937 (no 1/25,4), como el original: el tamaño vuelve 0,999998 veces.
    EXPECT_NEAR(e2->bounds().width(), edge->bounds().width() * 0.03937 * 25.4, 0.01);  // 4 decimales en pulgadas
    std::size_t d1 = 0, d2 = 0;
    for (const auto& [id, t] : pth->excellon().drillTools()) d1 += t.drills.size();
    for (const auto& [id, t] : static_cast<ExcellonObject*>(p2)->excellon().drillTools()) d2 += t.drills.size();
    EXPECT_EQ(d1, d2);
    // Al moverlo, el texto fuente es el exportado (MOIN).
    EXPECT_EQ(edge->gerber().sourceFile().find("%MOIN*%"), std::string::npos);
    app.ui()->collection_view->selectionModel()->clearSelection();
    app.ui()->collection_view->selectionModel()->select(app.collection()->indexOf(edge), QItemSelectionModel::Select);
    app.ui()->menuedit_move2origin->trigger();
    EXPECT_NE(edge->gerber().sourceFile().find("%MOIN*%"), std::string::npos);
}

// Exportar a SVG desde la aplicación.
TEST(GuiFiles, ExportSvg) {
    App app;
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    auto* edge = app.openByExtension(QString::fromStdString(dir + "Edge_Cuts.gbr"));
    ASSERT_NE(edge, nullptr);
    QTemporaryDir tmp;
    ASSERT_TRUE(app.exportSvgFile(edge, tmp.filePath("e.svg")));
    QFile f(tmp.filePath("e.svg"));
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    const QString text = QString::fromUtf8(f.readAll());
    EXPECT_TRUE(text.startsWith("<?xml version=\"1.0\" ?>\n<svg xmlns="));
    EXPECT_TRUE(text.contains("<g transform=\"scale(1,-1)\">"));
}

// Importar SVG como Geometría (soltando el archivo) y como Gerber.
TEST(GuiFiles, ImportSvg) {
    App app;
    QTemporaryDir data;
    app.setDataPath(data.path());
    const QString f = QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/svg/paths.svg");
    auto* geo = dynamic_cast<GeometryObject*>(app.openByExtension(f));
    ASSERT_NE(geo, nullptr);
    EXPECT_EQ(geo->name(), "paths.svg");
    EXPECT_EQ(geo->solidGeometry().size(), 7u);
    ASSERT_EQ(geo->tools().size(), 1u);
    EXPECT_DOUBLE_EQ(geo->tools().begin()->second.tooldia, 2.4);
    auto* grb = dynamic_cast<GerberObject*>(app.importSvg(f, true));
    ASSERT_NE(grb, nullptr);
    ASSERT_TRUE(grb->gerber().apertures().count("0"));
    EXPECT_EQ(grb->gerber().apertures().at("0").type, "REG");
    EXPECT_EQ(grb->gerber().apertures().at("0").geometry.size(), 8u);
    // Un SVG con una línea no se puede importar como Gerber.
    EXPECT_EQ(app.importSvg(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/svg/shapes.svg"), true), nullptr);
}

// Importar DXF como Geometría y exportar una Geometría a DXF (y volver a importarla).
TEST(GuiFiles, ImportAndExportDxf) {
    App app;
    QTemporaryDir data;
    app.setDataPath(data.path());
    auto* geo = dynamic_cast<GeometryObject*>(
        app.openByExtension(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/dxf/entities.dxf")));
    ASSERT_NE(geo, nullptr);
    EXPECT_EQ(geo->solidGeometry().size(), 29u);
    QTemporaryDir tmp;
    ASSERT_TRUE(app.exportDxfFile(geo, tmp.filePath("g.dxf")));
    auto* back = dynamic_cast<GeometryObject*>(app.importDxf(tmp.filePath("g.dxf"), false));
    ASSERT_NE(back, nullptr);
    EXPECT_FALSE(back->solidGeometry().empty());
    // Como Gerber falla si hay polígonos (círculos), como el original.
    EXPECT_EQ(app.importDxf(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/dxf/entities.dxf"), true),
              nullptr);
    EXPECT_NE(app.importDxf(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/dxf/lines_r12.dxf"), true),
              nullptr);
}

// Exportar PNG: lo que muestra el canvas (en "offscreen" QOpenGLWidget no dibuja: entonces no se escribe nada).
TEST(GuiFiles, ExportPng) {
    App app;
    QTemporaryDir data;
    app.setDataPath(data.path());
    QTemporaryDir tmp;
    const QString f = tmp.filePath("c.png");
    const bool ok = app.exportPngFile(f);
    EXPECT_EQ(ok, QFile::exists(f));
    if (ok) {
        const QImage img(f);
        EXPECT_FALSE(img.hasAlphaChannel());
        EXPECT_EQ(img.size(), app.ui()->plotCanvas()->grabFramebuffer().size());
    }
}

// Importar HPGL2 como Geometría: una herramienta por pluma.
TEST(GuiFiles, OpenHpgl2) {
    App app;
    QTemporaryDir data;
    app.setDataPath(data.path());
    auto* geo = dynamic_cast<GeometryObject*>(
        app.openHpgl2(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/hpgl2/basic.plt")));
    ASSERT_NE(geo, nullptr);
    EXPECT_EQ(geo->tools().size(), 2u);
    EXPECT_EQ(geo->tools().begin()->second.type, "Iso");
    // Varias órdenes por línea: el original no lo lee; el port sí (corrección consciente).
    EXPECT_NE(app.openHpgl2(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/hpgl2/multi_cmd.plt")), nullptr);
    // Bajar la pluma sin trazo empezado sigue fallando, como en el original.
    EXPECT_EQ(app.openHpgl2(QString::fromStdString(std::string(FLATCAM_TEST_DATA_DIR) + "/hpgl2/pen_down_first.plt")),
              nullptr);
}

// "Geometría de fresado" del panel del Excellon: Fresar agujeros y Fresar ranuras (generate_milling_drills / _slots).
TEST(GuiMilling, MillDrillsAndSlotsFromTheExcellonPanel) {
    App app;
    const QString data = QString(FLATCAM_TEST_DATA_DIR);
    FlatCAMObj* pth = app.openExcellon(data + "/luz_de_noche/luz de noche Pascual-PTH.drl");
    ASSERT_NE(pth, nullptr);
    app.showProperties(pth);
    auto* ui = qobject_cast<ExcellonObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui, nullptr);
    EXPECT_EQ(ui->tooldia_entry->value(), 0.8);  // excellon_tooldia
    // Como el original: selectColumn(0) no elige nada en una tabla que elige filas, así que sin herramientas elegidas
    // los botones quedan deshabilitados.
    EXPECT_TRUE(ui->selectedTools().empty());
    EXPECT_FALSE(ui->generate_milling_button->isEnabled());
    // Se eligen las cuatro herramientas (un clic en la tabla actualiza la sección de fresado).
    auto pick = [](ExcellonObjectUI* u, int rows) {
        for (int r = 0; r < rows; ++r) u->tools_table->selectionModel()->select(
            u->tools_table->model()->index(r, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
        Q_EMIT u->tools_table->clicked(u->tools_table->model()->index(0, 0));
    };
    pick(ui, 4);
    EXPECT_TRUE(ui->generate_milling_button->isEnabled());
    EXPECT_FALSE(ui->generate_milling_slots_button->isEnabled());  // sin ranuras
    // La fresa de 0,8 (igual a T1).
    EXPECT_EQ(ui->selectedTools(), (std::vector<int>{1, 2, 3, 4}));
    ui->generate_milling_button->click();
    auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName("luz de noche Pascual-PTH.drl_mill"));
    ASSERT_NE(geo, nullptr);
    ASSERT_EQ(geo->tools().size(), 1u);
    EXPECT_EQ(geo->tools().at(1).tooldia, 0.8);
    EXPECT_EQ(geo->tools().at(1).solidGeometry.size(), 120u);  // los 120 agujeros
    // Una fresa más grande que T1: falla con el mensaje del original.
    app.showProperties(pth);
    pick(ui, 4);
    ui->tooldia_entry->setValue(1.2);
    ui->generate_milling_button->click();
    EXPECT_TRUE(app.ui()->fcinfo->text->text().contains(tr_("Milling tool for DRILLS is larger than hole size. Cancelled.")));
    // Ranuras.
    FlatCAMObj* sl = app.openExcellon(data + "/excellon_samples/slots_g85.drl");
    ASSERT_NE(sl, nullptr);
    app.showProperties(sl);
    auto* ui2 = qobject_cast<ExcellonObjectUI*>(app.ui()->properties_scroll_area->widget());
    ASSERT_NE(ui2, nullptr);
    pick(ui2, 1);
    EXPECT_TRUE(ui2->generate_milling_slots_button->isEnabled());
    ui2->slot_tooldia_entry->setValue(0.5);
    ui2->generate_milling_slots_button->click();
    auto* g2 = dynamic_cast<GeometryObject*>(app.collection()->byName("slots_g85.drl_mill_slot"));
    ASSERT_NE(g2, nullptr);
    EXPECT_EQ(g2->tools().at(1).solidGeometry.size(), 3u);
}

// Panelizar desde el panel: Gerber, Excellon y Geometría (on_panelize).
TEST(GuiPanelize, PanelizesGerberExcellonAndGeometry) {
    App app;
    const QString dir = QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    FlatCAMObj* edge = app.openGerber(dir + "Edge_Cuts.gbr");
    FlatCAMObj* pth = app.openExcellon(dir + "PTH.drl");
    ASSERT_NE(edge, nullptr);
    ASSERT_NE(pth, nullptr);
    PanelizeTool* t = app.panelizeTool;
    app.ui()->panelize_btn->trigger();
    EXPECT_EQ(t->columns->value(), 1);  // tools_panelize_columns
    t->columns->setValue(2);
    t->rows->setValue(2);
    t->spacing_columns->setValue(3.0);
    t->type_obj_combo->setCurrentIndex(0);
    t->object_combo->setValue(edge->name());
    FlatCAMObj* panel = t->panelize();
    ASSERT_NE(panel, nullptr);
    EXPECT_EQ(panel->name(), edge->name() + "_panelized");
    EXPECT_EQ(panel->kind(), ObjectKind::Gerber);
    const QRectF b = edge->bounds(), pb = panel->bounds();
    EXPECT_NEAR(pb.width(), 2 * b.width() + 3.0, 1e-9);
    EXPECT_NEAR(pb.height(), 2 * b.height(), 1e-9);
    EXPECT_EQ(static_cast<GerberObject*>(panel)->gerber().solidGeometry().size(),
              4 * static_cast<GerberObject*>(edge)->gerber().solidGeometry().size());
    // Un Excellon: el tipo de panel se deshabilita y sale un Excellon con los agujeros de las cuatro copias.
    t->type_obj_combo->setCurrentIndex(1);
    EXPECT_FALSE(t->panel_type_radio->isEnabled());
    t->object_combo->setValue(pth->name());
    FlatCAMObj* ep = t->panelize();
    ASSERT_NE(ep, nullptr);
    EXPECT_EQ(ep->kind(), ObjectKind::Excellon);
    std::size_t holes = 0;
    for (const auto& [id, tool] : static_cast<ExcellonObject*>(ep)->excellon().drillTools()) holes += tool.drills.size();
    EXPECT_EQ(holes, 4u * 120u);
    // Restringir a un área donde entra una sola copia.
    t->type_obj_combo->setCurrentIndex(0);
    t->object_combo->setValue(edge->name());
    t->constrain_cb->setChecked(true);
    t->x_width_entry->setValue(b.width() + 1);
    t->y_height_entry->setValue(b.height() + 1);
    FlatCAMObj* one = t->panelize();
    ASSERT_NE(one, nullptr);
    EXPECT_NEAR(one->bounds().width(), b.width(), 1e-9);
}

// Invertir Gerber desde el panel (Alt+G).
TEST(GuiInvert, InvertsTheGerber) {
    App app;
    FlatCAMObj* edge =
        app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr");
    ASSERT_NE(edge, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_G, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    InvertTool* t = app.invertTool;
    EXPECT_EQ(t->gerber_combo->currentText(), edge->name());
    EXPECT_EQ(t->margin_entry->value(), 0.1);  // tools_invert_margin
    t->margin_entry->setValue(2.0);
    FlatCAMObj* inv = t->invert();
    ASSERT_NE(inv, nullptr);
    EXPECT_EQ(inv->name(), edge->name() + "_inverted");
    const auto& g = static_cast<GerberObject*>(inv)->gerber();
    ASSERT_TRUE(g.apertures().count("0"));
    EXPECT_EQ(g.apertures().at("0").type, "C");
    EXPECT_EQ(g.apertures().at("0").geometry.size(), g.solidGeometry().size());
    EXPECT_NEAR(inv->bounds().width(), edge->bounds().width() + 4.0, 1e-9);  // la caja con el margen (ingletes)
}

// Calculadoras (Alt+C): los mismos resultados y formatos que el original (calculados con sus fórmulas en Python).
TEST(GuiCalculators, ComputeLikeTheOriginal) {
    App app;
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_C, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    CalculatorsTool* t = app.calculatorsTool;
    EXPECT_EQ(t->mm_entry->text(), "0.0000");
    t->calculateToolDia();  // punta 0,2, 30°, profundidad 0,05
    EXPECT_DOUBLE_EQ(t->effectiveToolDia_entry->value(), 0.2268);
    t->mm_entry->setText("12.7");
    t->calculateInch();
    EXPECT_EQ(t->inch_entry->text(), "0.5000");
    t->inch_entry->setText("1.5");
    t->calculateMm();
    EXPECT_EQ(t->mm_entry->text(), "38.1000");
    t->calculateElectroplating();  // 10 x 10 cm, 13 A/dm², 10 um
    EXPECT_DOUBLE_EQ(t->cvalue_entry->value(), 2.80);
    EXPECT_DOUBLE_EQ(t->time_entry->value(), 33.0);
    t->area_sel_radio->setValue(QString("a"));
    Q_EMIT t->area_sel_radio->activatedCustom("a");
    EXPECT_FALSE(t->pcbwidth_entry->isVisibleTo(t));
    EXPECT_TRUE(t->area_entry->isVisibleTo(t));
    t->calculateElectroplating();  // 100 cm²
    EXPECT_DOUBLE_EQ(t->cvalue_entry->value(), 2.80);
}

// Marcas de esquina (Alt+M): el Gerber con las marcas y el Excellon con los agujeros.
TEST(GuiCorners, AddsMarkersAndDrills) {
    App app;
    FlatCAMObj* edge =
        app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr");
    ASSERT_NE(edge, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_M, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    CornersTool* t = app.cornersTool;
    EXPECT_EQ(t->object_combo->currentText(), edge->name());
    EXPECT_EQ(t->addMarkers(), nullptr);  // sin esquinas elegidas
    t->toggle_all_cb->setChecked(true);
    EXPECT_TRUE(t->tl_cb->isChecked() && t->br_cb->isChecked());
    FlatCAMObj* c = t->addMarkers();
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->name(), edge->name() + "_corners");
    const auto& g = static_cast<GerberObject*>(c)->gerber();
    ASSERT_TRUE(g.apertures().count("11"));  // apertura nueva de 0,1 (la del archivo es de 0,05)
    EXPECT_EQ(g.apertures().at("11").geometry.size(), 8u);
    EXPECT_GT(c->bounds().width(), edge->bounds().width());
    FlatCAMObj* d = t->createDrills();
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(static_cast<ExcellonObject*>(d)->excellon().drillTools().at(1).drills.size(), 4u);
}

// Extraer taladros (Alt+E): un Excellon con los flashes de los pads del cobre del usuario.
TEST(GuiExtractDrills, ExtractsTheUserPads) {
    App app;
    FlatCAMObj* cu = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/PCB principal-B_Cu.gbl");
    ASSERT_NE(cu, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_E, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    ExtractDrillsTool* t = app.extractDrillsTool;
    EXPECT_EQ(t->gerber_object_combo->currentText(), cu->name());
    EXPECT_EQ(t->hole_size_radio->value().toString(), "fixed");  // tools_edrills_hole_type
    FlatCAMObj* e = t->extract();
    ASSERT_NE(e, nullptr);
    EXPECT_EQ(e->name(), "PCB principal-B_Cu");  // sin la extensión
    const auto& tools = static_cast<ExcellonObject*>(e)->excellon().drillTools();
    ASSERT_EQ(tools.size(), 1u);
    // De fábrica solo los pads circulares: los mismos 59 que el original con ese filtro (oracle_extract_drills.py).
    EXPECT_EQ(tools.at(1).drills.size(), 59u);
}

// Perforar Gerber (Alt+H) con el taladro del dosificador: los mismos agujeros "clear" que el original (oracle_punch.py).
TEST(GuiPunch, PunchesThePadsWithTheDrills) {
    App app;
    const QString data = QString(FLATCAM_TEST_DATA_DIR);
    FlatCAMObj* cu = app.openGerber(data + "/PCB principal-B_Cu.gbl");
    FlatCAMObj* drl = app.openExcellon(data + "/PCB principal-Drill.drl");
    ASSERT_NE(cu, nullptr);
    ASSERT_NE(drl, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_H, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    PunchTool* t = app.punchTool;
    EXPECT_EQ(t->method_punch->value().toString(), "exc");  // tools_punch_hole_type
    t->select_all_cb->setChecked(true);
    t->buildToolUi();
    ASSERT_GT(t->apertures_table->rowCount(), 0);
    // Como el original, selectColumn(0) deja elegidas todas las aperturas de la tabla.
    EXPECT_EQ(t->apertures_table->selectedItems().size(), t->apertures_table->rowCount());
    FlatCAMObj* p = t->punch();
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->name(), "PCB principal-B_Cu_punched");
    const auto& aps = static_cast<GerberObject*>(p)->gerber().apertures();
    ASSERT_TRUE(aps.count("28") && aps.count("31"));
    EXPECT_EQ(aps.at("28").geometry.size(), 90u);  // agujeros de 0,8 dentro de pads
    EXPECT_DOUBLE_EQ(aps.at("31").size.value_or(0), 3.4);
}

// Restar (Alt+W): cobre menos máscara del dosificador; con "Borrar fuentes" se borran los dos. Geometría menos Geometría
// no crea nada, como el original.
TEST(GuiSub, SubtractsGerbers) {
    App app;
    const QString data = QString(FLATCAM_TEST_DATA_DIR);
    FlatCAMObj* cu = app.openGerber(data + "/PCB principal-B_Cu.gbl");
    FlatCAMObj* mask = app.openGerber(data + "/PCB principal-B_Mask.gbs");
    ASSERT_NE(cu, nullptr);
    ASSERT_NE(mask, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_W, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    SubTool* t = app.subTool;
    t->target_gerber_combo->setValue(cu->name());
    t->sub_gerber_combo->setValue(mask->name());
    t->delete_sources_cb->setChecked(true);
    FlatCAMObj* r = t->subtractGerber();
    ASSERT_NE(r, nullptr);
    EXPECT_EQ(r->name(), "PCB principal-B_Cu.gbl_sub");
    EXPECT_EQ(static_cast<GerberObject*>(r)->gerber().solidGeometry().size(), 179u);  // como el original (oracle_sub.py)
    EXPECT_EQ(app.collection()->byName("PCB principal-B_Cu.gbl"), nullptr);
    EXPECT_EQ(app.collection()->byName("PCB principal-B_Mask.gbs"), nullptr);
    EXPECT_EQ(t->subtractGeometry(), nullptr);
}

// Óptimo (Alt+O): la distancia mínima del cobre de "luz de noche" (la misma que el original, oracle_optimal.py) y saltar a
// la posición.
TEST(GuiOptimal, FindsTheMinimumDistance) {
    App app;
    FlatCAMObj* cu = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr");
    ASSERT_NE(cu, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_O, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    OptimalTool* t = app.optimalTool;
    EXPECT_EQ(t->precision_spinner->value(), 4);  // tools_opt_precision
    ASSERT_TRUE(t->findMinimumDistance());
    EXPECT_EQ(t->result_entry->text(), "0.1994");
    EXPECT_EQ(t->freq_entry->text(), "1");
    EXPECT_FALSE(t->distances_textb->toPlainText().isEmpty());
    const QString line = t->locations_textb->toPlainText().section('\n', 0, 0);
    EXPECT_TRUE(t->locate(line));
    EXPECT_FALSE(t->locate("nada"));
}

TEST(GuiEtch, CompensatesTheCopper) {
    App app;
    FlatCAMObj* cu = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr");
    ASSERT_NE(cu, nullptr);
    app.ui()->etch_tool_menuAction->trigger();
    EtchTool* t = app.etchTool;
    EXPECT_EQ(t->thick_entry->value(), 18.0);
    EXPECT_EQ(t->ratio_radio->value().toString(), "factor");
    EXPECT_FALSE(t->factor_entry->isHidden());
    EXPECT_TRUE(t->etchants_combo->isHidden());
    // Conversores: 1 oz = 34,798 micras; 2 mils = 50,8 micras; un texto inválido deja el resultado vacío.
    t->oz_entry->setText("1");
    EXPECT_EQ(t->oz_to_um_entry->text(), "34.7980");
    t->mils_entry->setText("2");
    EXPECT_EQ(t->mils_to_um_entry->text(), "50.8000");
    t->mils_entry->setText("2*");
    EXPECT_EQ(t->mils_to_um_entry->text(), "");
    // Con factor 0 avisa y no crea nada, como el original (que además falla después).
    t->factor_entry->setText("0");
    EXPECT_EQ(t->compensate(), nullptr);
    t->factor_entry->setText("2");
    FlatCAMObj* out = t->compensate();
    ASSERT_NE(out, nullptr);
    EXPECT_EQ(out->name(), cu->name() + "_comp");
    // El mismo resultado que el núcleo, verificado contra el original (golden_etch.h: 48 polígonos).
    EXPECT_EQ(static_cast<GerberObject*>(out)->gerber().solidGeometry().size(), 48u);
    // Grabador elegido: los campos visibles cambian.
    t->ratio_radio->setValue(QString("etch_list"));
    Q_EMIT t->ratio_radio->activatedCustom("etch_list");
    EXPECT_FALSE(t->etchants_combo->isHidden());
    EXPECT_TRUE(t->factor_entry->isHidden());
}

TEST(GuiAlign, MovesTheAlignedObjectFromADrillToAPad) {
    App app;
    const QString dir = QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/";
    FlatCAMObj* cu = app.openGerber(dir + "luz de noche Pascual-B_Cu.gbr");
    FlatCAMObj* drl = app.openExcellon(dir + "luz de noche Pascual-PTH.drl");
    ASSERT_NE(cu, nullptr);
    ASSERT_NE(drl, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_A, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    AlignTool* t = app.alignTool;
    EXPECT_EQ(t->a_type_radio->value().toString(), "sp");  // tools_align_objects_align_type
    EXPECT_EQ(t->object_combo->currentText(), cu->name());
    t->type_obj_radio->setValue(QString("exc"));
    Q_EMIT t->type_obj_radio->activatedCustom("exc");
    EXPECT_EQ(t->object_combo->currentText(), drl->name());
    const bool grid = app.ui()->grid_snap_btn->isChecked();
    const QRectF before = drl->bounds();
    ASSERT_TRUE(t->align());
    EXPECT_TRUE(t->isPicking());
    EXPECT_FALSE(app.ui()->grid_snap_btn->isChecked());
    // Un clic fuera de los agujeros no elige nada; uno dentro elige su centro.
    t->onRelease({1000, 1000}, Qt::LeftButton, false);
    EXPECT_TRUE(t->clickedPoints().empty());
    t->onRelease({-32.445, 21.515}, Qt::LeftButton, false);
    ASSERT_EQ(t->clickedPoints().size(), 1u);
    EXPECT_EQ(t->clickedPoints()[0].x, -32.545);
    // Un trazo del cobre no sirve: solo los pads.
    t->onRelease({-2.28, -10.56}, Qt::LeftButton, false);
    EXPECT_EQ(t->clickedPoints().size(), 1u);
    t->onRelease({-25.09, -29.21}, Qt::LeftButton, false);
    EXPECT_FALSE(t->isPicking());
    EXPECT_EQ(app.ui()->grid_snap_btn->isChecked(), grid);
    const QRectF after = drl->bounds();
    EXPECT_NEAR(after.left() - before.left(), -25.14 + 32.545, 1e-9);
    EXPECT_NEAR(after.top() - before.top(), -29.18 - 21.415, 1e-9);
    // Clic derecho cancela.
    ASSERT_TRUE(t->align());
    t->onRelease({-32.445, 21.515}, Qt::LeftButton, false);
    t->onRelease({0, 0}, Qt::RightButton, false);
    EXPECT_FALSE(t->isPicking());
    EXPECT_TRUE(t->clickedPoints().empty());
}

TEST(GuiFiducials, AddsFiducialsAndMaskOpenings) {
    App app;
    FlatCAMObj* cu = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr");
    ASSERT_NE(cu, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_F, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    FiducialsTool* t = app.fiducialsTool;
    EXPECT_EQ(t->fid_size_entry->value(), 1.0);
    EXPECT_EQ(t->mode_radio->value().toString(), "auto");
    EXPECT_FALSE(t->line_thickness_entry->isEnabled());  // solo para la cruz
    // Automático, circular, segundo arriba: los mismos puntos y textos que el original (golden_fiducials.h).
    t->addFiducials();
    FlatCAMObj* fid = t->lastCreated();
    ASSERT_NE(fid, nullptr);
    EXPECT_EQ(fid->name(), cu->name() + "_fid");
    EXPECT_EQ(t->bottom_left_coords_entry->text(), "(-35.4995, -32.9995)");
    EXPECT_EQ(t->top_right_coords_entry->text(), "(35.4995, 32.9995)");
    EXPECT_EQ(t->sec_points_coords_entry->text(), "(-35.4995, 32.9995)");
    EXPECT_EQ(static_cast<GerberObject*>(fid)->gerber().solidGeometry().size(), 51u);
    // La abertura de la máscara usa los mismos puntos, con el doble del tamaño (la apertura C de 2,0).
    t->sm_object_combo->setValue(cu->name());
    FlatCAMObj* sm = t->addSoldermaskOpening();
    ASSERT_NE(sm, nullptr);
    EXPECT_EQ(static_cast<GerberObject*>(sm)->gerber().apertures().at("12").geometry.size(), 37u);
    // Manual, sin segundo punto: dos clics.
    t->grb_object_combo->setValue(cu->name());
    t->mode_radio->setValue(QString("manual"));
    t->pos_radio->setValue(QString("no"));
    Q_EMIT t->pos_radio->activatedCustom("no");
    EXPECT_FALSE(t->sec_points_coords_entry->isEnabled());
    t->addFiducials();
    EXPECT_TRUE(t->isPicking());
    t->onRelease({1.23456, -2.5}, Qt::LeftButton);
    EXPECT_EQ(t->bottom_left_coords_entry->text(), "(1.2346, -2.5)");
    t->onRelease({3, 4}, Qt::LeftButton);
    EXPECT_EQ(t->top_right_coords_entry->text(), "(3.0, 4.0)");
    EXPECT_FALSE(t->isPicking());
    EXPECT_NE(t->lastCreated(), sm);
}

TEST(GuiThieving, FillsAddsTheRobberBarAndThePlatingMask) {
    App app;
    FlatCAMObj* cu = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr");
    ASSERT_NE(cu, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_J, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    ThievingTool* t = app.thievingTool;
    EXPECT_EQ(t->clearance_entry->value(), 0.25);
    EXPECT_EQ(t->reference_radio->value().toString(), "itself");
    EXPECT_EQ(t->grb_object_combo->currentText(), cu->name());
    // Sólido sobre el propio Gerber (golden_thieving.h: 19 figuras agregadas a la apertura 0, que ya tenía 5).
    t->addCopperThieving();
    FlatCAMObj* th = t->lastCreated();
    ASSERT_NE(th, nullptr);
    EXPECT_EQ(th->name(), cu->name() + "_thief");
    EXPECT_EQ(static_cast<GerberObject*>(th)->gerber().apertures().at("0").geometry.size(), 24u);
    // Las líneas solo van con la referencia "itself".
    t->fill_type_radio->setValue(QString("line"));
    t->reference_radio->setValue(QString("area"));
    EXPECT_EQ(t->reference_radio->value().toString(), "itself");
    t->fill_type_radio->setValue(QString("solid"));
    t->reference_radio->setValue(QString("area"));
    EXPECT_TRUE(t->bbox_type_radio->isHidden());
    // Área: dos rectángulos; Escape no cancela (el original falla) y el clic derecho termina.
    t->addCopperThieving();
    CanvasPicker* pk = app.picker();
    ASSERT_TRUE(pk->active());
    pk->onRelease({-30, -30}, Qt::LeftButton, false);
    pk->onRelease({0, 0}, Qt::LeftButton, false);
    pk->onRelease({10, 10}, Qt::LeftButton, false);
    pk->onRelease({-10, -10}, Qt::LeftButton, false);
    QKeyEvent esc(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    QApplication::sendEvent(app.ui(), &esc);
    EXPECT_TRUE(pk->active());
    pk->onRelease({0, 0}, Qt::RightButton, false);
    EXPECT_FALSE(pk->active());
    ASSERT_NE(t->lastCreated(), th);
    EXPECT_EQ(static_cast<GerberObject*>(t->lastCreated())->gerber().apertures().at("0").geometry.size(), 15u);
    // La barra de robo de 1 mm: una apertura nueva, la 25.
    FlatCAMObj* rb = t->addRobberBar();
    ASSERT_NE(rb, nullptr);
    EXPECT_EQ(rb->name(), cu->name() + "_robber");
    EXPECT_EQ(static_cast<GerberObject*>(rb)->gerber().apertures().at("25").geometry.size(), 1u);
    // La máscara para el galvanizado, con el último relleno y la barra.
    FlatCAMObj* mask = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/PCB principal-B_Mask.gbs");
    ASSERT_NE(mask, nullptr);
    t->sm_object_combo->addItem(mask->name());
    t->sm_object_combo->setValue(mask->name());
    FlatCAMObj* pm = t->addPlatingMask();
    ASSERT_NE(pm, nullptr);
    EXPECT_EQ(pm->name(), "PCB principal-B_Mask_plating_mask.gbs");
    EXPECT_FALSE(t->plated_area_entry->text().isEmpty());
    const auto& aps = static_cast<GerberObject*>(pm)->gerber().apertures();
    EXPECT_EQ(aps.at("0").geometry.size(), 10u);  // las 10 figuras del relleno del área
    EXPECT_EQ(aps.at("26").size.value_or(0), 1.0);  // la barra de 1 mm
}

TEST(GuiQRCode, InsertsTheCodeAndExportsIt) {
    App app;
    FlatCAMObj* cu = app.openGerber(QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-B_Cu.gbr");
    ASSERT_NE(cu, nullptr);
    QKeyEvent alt(QEvent::KeyPress, Qt::Key_Q, Qt::AltModifier);
    QApplication::sendEvent(app.ui(), &alt);
    QRCodeTool* t = app.qrcodeTool;
    EXPECT_EQ(t->bsize_entry->value(), 3);
    EXPECT_EQ(t->error_radio->value().toString(), "L");
    // Sin texto no hace nada.
    EXPECT_FALSE(t->execute());
    t->text_data->setValue("https://flatcam.org");
    ASSERT_TRUE(t->execute());
    EXPECT_TRUE(t->isPlacing());
    t->onMove({3, 4});
    t->onRelease({0, 0}, Qt::RightButton);  // no cancela: solo el izquierdo cuenta
    EXPECT_TRUE(t->isPlacing());
    t->onRelease({0, 0}, Qt::LeftButton);
    EXPECT_FALSE(t->isPlacing());
    // El Gerber de origen queda modificado, como en el original (golden_qrcode.h: 59 polígonos, apertura 25).
    const flatcam::Gerber& g = static_cast<GerberObject*>(cu)->gerber();
    EXPECT_EQ(g.solidGeometry().size(), 59u);
    ASSERT_TRUE(g.apertures().count("25"));
    EXPECT_EQ(g.apertures().at("25").geometry.size(), 332u);
    EXPECT_EQ(g.apertureOrder().back(), "25");
    // Exportar: el SVG de la biblioteca y un PNG de 33 x 3 píxeles de lado (versión 2 con borde 4).
    QTemporaryDir dir;
    ASSERT_TRUE(t->exportSvg(dir.filePath("qr.svg")));
    QFile f(dir.filePath("qr.svg"));
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    const QByteArray svg = f.readAll();
    EXPECT_TRUE(svg.startsWith("<?xml version='1.0' encoding='UTF-8'?>\n<svg width=\"9.9mm\""));
    EXPECT_TRUE(svg.contains("fill_color=\"#000000\" back_color=\"#FFFFFF\""));
    ASSERT_TRUE(t->exportPng(dir.filePath("qr.png")));
    const QImage img(dir.filePath("qr.png"));
    EXPECT_EQ(img.width(), 99);
    EXPECT_EQ(img.pixelColor(0, 0), QColor("#FFFFFF"));
    EXPECT_EQ(img.pixelColor(12, 12), QColor("#000000"));  // esquina del primer patrón de posición
    // Fondo transparente.
    t->transparent_cb->setChecked(true);
    EXPECT_EQ(t->back_color_entry->value(), "transparent");
    ASSERT_TRUE(t->exportPng(dir.filePath("qr2.png")));
    EXPECT_EQ(QImage(dir.filePath("qr2.png")).pixelColor(0, 0).alpha(), 0);
    t->transparent_cb->setChecked(false);
    EXPECT_EQ(t->back_color_entry->value(), "#FFFFFF");
}
