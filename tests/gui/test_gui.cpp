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
    qputenv("QT_QPA_PLATFORM", "offscreen");
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
