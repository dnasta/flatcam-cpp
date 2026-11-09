#include "gui/App.h"

#include <QClipboard>
#include <QRegularExpression>
#include <QMessageBox>
#include <QPushButton>

#include <QFile>

#include "cam/PolygonClearing.h"

#include <QApplication>
#include <algorithm>
#include <QDateTime>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMimeData>
#include <QScrollArea>
#include <QSplitter>

#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/CanvasPicker.h"
#include "gui/ExclusionAreasManager.h"
#include "gui/ToolsDatabase.h"
#include "gui/ToolsDBTab.h"
#include "gui/tools/TransformTool.h"
#include "gui/tools/DistanceTool.h"
#include "gui/tools/MoveTool.h"
#include "gui/tools/PropertiesTool.h"
#include "gui/ProjectIO.h"
#include "gui/Preferences.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/RadioSet.h"
#include <QProcess>
#include "gui/tools/ClearingTools.h"
#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

namespace {

// AppIO.on_fileopengerber / on_fileopenexcellon
const char* kGerberFilter =
    "Gerber Files (*.gbr *.ger *.gtl *.gbl *.gts *.gbs *.gtp *.gbp *.gto *.gbo *.gm1 *.gml *.gm3 "
    "*.gko *.cmp *.sol *.stc *.sts *.plc *.pls *.crc *.crs *.tsm *.bsm *.ly2 *.ly15 *.dim *.mil *.grb "
    "*.top *.bot *.smt *.smb *.sst *.ssb *.spt *.spb *.pho *.gdo *.art *.gbd *.outline);;"
    "Protel Files (*.gtl *.gbl *.gts *.gbs *.gto *.gbo *.gtp *.gbp *.gml *.gm1 *.gm3 *.gko *.outline);;"
    "Eagle Files (*.cmp *.sol *.stc *.sts *.plc *.pls *.crc *.crs *.tsm *.bsm *.ly2 *.ly15 *.dim *.mil);;"
    "OrCAD Files (*.top *.bot *.smt *.smb *.sst *.ssb *.spt *.spb);;"
    "Allegro Files (*.art);;"
    "Mentor Files (*.pho *.gdo);;"
    "All Files (*.*)";
const char* kExcellonFilter = "Excellon Files (*.drl *.txt *.xln *.drd *.tap *.exc *.ncd);;All Files (*.*)";

// Extensiones de `global_gerber_extensions` y `global_excellon_extensions` (para arrastrar y soltar).
const QStringList kGerberExt = {"gbr", "ger", "gtl", "gbl", "gts", "gbs", "gtp", "gbp", "gto", "gbo", "gm1", "gml", "gm3",
                                "gko", "cmp", "sol", "stc", "sts", "plc", "pls", "crc", "crs", "tsm", "bsm", "ly2", "ly15",
                                "dim", "mil", "grb", "top", "bot", "smt", "smb", "sst", "ssb", "spt", "spb", "pho", "gdo",
                                "art", "gbd", "outline"};
const QStringList kExcellonExt = {"drl", "txt", "xln", "drd", "tap", "exc", "ncd"};

// Color con que AppObject.on_object_created anuncia cada tipo.
QString kindColor(ObjectKind k) {
    switch (k) {
        case ObjectKind::Gerber: return "green";
        case ObjectKind::Excellon: return "brown";
        case ObjectKind::CNCJob: return "blue";
        case ObjectKind::Geometry: return "red";
        case ObjectKind::Script: return "orange";
        default: return "darkCyan";
    }
}

QString kindName(ObjectKind k) {
    switch (k) {
        case ObjectKind::Gerber: return "Gerber";
        case ObjectKind::Excellon: return "Excellon";
        case ObjectKind::CNCJob: return "Cncjob";
        case ObjectKind::Geometry: return "Geometry";
        case ObjectKind::Script: return "Script";
        default: return "Document";
    }
}

}  // namespace

App::App(QObject* parent) : QObject(parent), ui_(std::make_unique<MainGUI>()) {
    collection_ = new ObjectCollection(this);
    ui_->collection_view->setModel(collection_);
    ui_->collection_view->setMinimumWidth(290);
    // Agregado del port (pedido del usuario): reordenar los objetos de un grupo arrastrándolos; el orden del árbol es
    // el de dibujo (el primero queda arriba). Los archivos se siguen pudiendo soltar para abrirlos.
    ui_->collection_view->setDragEnabled(true);
    ui_->collection_view->setDragDropMode(QAbstractItemView::DragDrop);
    ui_->collection_view->setDefaultDropAction(Qt::MoveAction);
    ui_->collection_view->setDropIndicatorShown(true);
    connect(collection_, &ObjectCollection::orderChanged, this,
            [this] { ui_->plotCanvas()->setLayerOrder(collection_->drawOrder()); });
    ui_->setAcceptDrops(true);
    ui_->installEventFilter(this);
    defaultProperties_ = ui_->properties_scroll_area->widget();
    // El selector con el mouse y las áreas de exclusión, antes que las herramientas que los usan.
    picker_ = new CanvasPicker(ui_.get(), this);
    excAreas_ = new ExclusionAreasManager(ui_.get(), picker_, this);
    setupPreferences();  // app.defaults: antes que las herramientas
    ToolsDatabase::ensureFile(dataPath());  // tools_db.FlatDB vacío si no existe
    // Las herramientas (install_tools): sus paneles viven mientras viva la ventana.
    nccTool = new NccTool(this, ui_.get());
    paintTool = new PaintTool(this, ui_.get());
    drillingTool = new DrillingTool(this, ui_.get());
    cutoutTool = new CutOutTool(this, ui_.get());
    dblsidedTool = new DblSidedTool(this, ui_.get());
    transformTool = new TransformTool(this, ui_.get());
    distanceTool = new DistanceTool(this, ui_.get());
    distanceMinTool = new DistanceMinTool(this, ui_.get());
    moveTool = new MoveTool(this, this);
    propertiesTool = new PropertiesTool(this, ui_.get());
    isolationTool = new IsolationTool(this, ui_.get());
    for (QWidget* w : std::initializer_list<QWidget*>{nccTool, paintTool, drillingTool, cutoutTool, dblsidedTool, transformTool,
                                                      isolationTool, distanceTool, distanceMinTool, propertiesTool})
        w->hide();
    connectSignals();
    setupProjectMenu();
    setupCanvasMenu();
    setupHelpMenu();
    // Archivo -> Salir y cerrar la ventana: final_save.
    disconnect(ui_->menufile_exit, nullptr, nullptr, nullptr);
    connect(ui_->menufile_exit, &QAction::triggered, this, [this] {
        if (finalSave()) QApplication::quit();
    });
    setupRecentItems();
}

App::~App() {
    // Los paneles de Propiedades son de los objetos: se saca el que esté a la vista antes de destruir nada. El de la
    // herramienta lo destruye la ventana (es su hijo o el de su área).
    if (ui_) ui_->properties_scroll_area->takeWidget();
}

void App::runTool(QWidget* panel, const ToolInfo& info, bool toggle) {
    QSplitter* sp = ui_->splitter;
    if (toggle && sp->sizes().value(0) != 0) {
        QWidget* cur = ui_->tool_scroll_area->widget();
        if (cur && cur->objectName() == info.toolName) {
            if (ui_->notebook->currentWidget() != ui_->tool_tab) ui_->notebook->setCurrentWidget(ui_->tool_tab);
            else sp->setSizes({0, 1});
            return;
        }
    }
    if (sp->sizes().value(0) == 0) sp->setSizes({1, 1});
    // AppTool.run: el panel anterior se saca (sigue siendo de la App) y se pone el nuevo.
    QWidget* old = ui_->tool_scroll_area->takeWidget();
    if (old && old != panel) {
        old->hide();
        old->setParent(ui_.get());
    }
    ui_->tool_scroll_area->setWidget(panel);
    ui_->notebook->setCurrentWidget(ui_->tool_tab);
    panel->setObjectName(info.toolName);
    panel->show();
    ui_->notebook->setTabText(2, info.tabText);
}

ToolsDBTab* App::openToolsDatabase(std::function<void(const JsonValue&)> onToolRequest) {
    const QString path = ToolsDatabase::fileName(dataPath());
    if (!QFile::exists(path)) {
        ui_->inform("[ERROR] " + tr_("Could not load the file."));
        return nullptr;
    }
    FCDetachableTab2* area = ui_->plot_tab_area;
    for (int i = 0; i < area->count(); ++i)
        if (area->tabText(i) == tr_("Tools Database")) return qobject_cast<ToolsDBTab*>(area->widget(i));
    if (!onToolRequest) {
        // on_geometry_tool_add_from_db_executed: a la Geometría activa (solo Fresado o General).
        onToolRequest = [this](const JsonValue& tool) {
            auto* geo = dynamic_cast<GeometryObject*>(activeObject());
            if (!geo) return;
            const double target = tool["data"]["tool_target"].toDouble(-1);
            if (target != 0 && target != 1) {
                closeToolsDatabase();
                ui_->inform("[ERROR_NOTCL] " + tr_("Selected tool can't be used here. Pick another."));
                return;
            }
            ClearingTool ct;
            ct.geo = defaultGeometryTool();
            ToolsDatabase::applyTo(tool, ct);
            ct.geo.solidGeometry = geo->solidGeometry();  // 'solid_geometry': self.solid_geometry
            const int uid = geo->tools().empty() ? 1 : geo->tools().rbegin()->first + 1;
            geo->tools()[uid] = ct.geo;
            closeToolsDatabase();
            showProperties(geo);
            ui_->inform("[success] " + tr_("Tool from DB added in Tool Table."));
        };
    }
    auto* tab = new ToolsDBTab(this, std::move(onToolRequest));
    area->addTab(tab, tr_("Tools Database"));
    tab->setObjectName("database_tab");
    ui_->coords_toolbar->hide();
    ui_->delta_coords_toolbar->hide();
    area->setCurrentWidget(tab);
    return tab;
}

void App::closeToolsDatabase() {
    FCDetachableTab2* area = ui_->plot_tab_area;
    for (int i = area->count() - 1; i >= 0; --i) {
        if (area->tabText(i) != tr_("Tools Database")) continue;
        QWidget* w = area->widget(i);
        area->removeTab(i);
        w->deleteLater();
    }
    ui_->coords_toolbar->show();
    ui_->delta_coords_toolbar->show();
}

void App::connectSignals() {
    connect(ui_->menuoptions_tools_db, &QAction::triggered, this, [this] { openToolsDatabase(); });
    for (QAction* a : {ui_->menufileopenproject, ui_->file_open_btn})
        connect(a, &QAction::triggered, this, [this] { onFileOpenProject(); });
    for (QAction* a : {ui_->menufilesaveproject, ui_->file_save_btn})
        connect(a, &QAction::triggered, this, [this] { onFileSaveProject(); });
    connect(ui_->menufilesaveprojectas, &QAction::triggered, this, [this] { onFileSaveProjectAs(); });
    connect(ui_->menufileopengerber, &QAction::triggered, this, [this] { onFileOpenGerber(); });
    connect(ui_->file_open_gerber_btn, &QAction::triggered, this, [this] { onFileOpenGerber(); });
    connect(ui_->menufileopenexcellon, &QAction::triggered, this, [this] { onFileOpenExcellon(); });
    connect(ui_->menufileopengcode, &QAction::triggered, this, [this] { onFileOpenGcode(); });
    connect(ui_->menufileexportgerber, &QAction::triggered, this, [this] { onFileExport(ObjectKind::Gerber); });
    connect(ui_->menufileexportexcellon, &QAction::triggered, this, [this] { onFileExport(ObjectKind::Excellon); });
    connect(ui_->menufileexportsvg, &QAction::triggered, this, [this] { onFileExportSvg(); });
    connect(ui_->menufileexportdxf, &QAction::triggered, this, [this] { onFileExportDxf(); });
    connect(ui_->menufileexportpng, &QAction::triggered, this, [this] { onFileExportPng(); });
    connect(ui_->menufileimportsvg, &QAction::triggered, this, [this] { onFileImportSvg(false); });
    connect(ui_->menufileimportsvg_as_gerber, &QAction::triggered, this, [this] { onFileImportSvg(true); });
    connect(ui_->menufileimportdxf, &QAction::triggered, this, [this] { onFileImportDxf(false); });
    connect(ui_->menufileimportdxf_as_gerber, &QAction::triggered, this, [this] { onFileImportDxf(true); });
    connect(ui_->menufileimport_hpgl2_as_geo, &QAction::triggered, this, [this] { onFileOpenHpgl2(); });
    connect(ui_->file_open_excellon_btn, &QAction::triggered, this, [this] { onFileOpenExcellon(); });
    for (QAction* a : {ui_->menuview_zoom_fit, ui_->zoom_fit_btn}) connect(a, &QAction::triggered, this, [this] { onZoomFit(); });
    // on_zoom_in / on_zoom_out: factor global_zoom_ratio (1,5) alrededor del centro de la vista.
    auto zoomBy = [this](double f) {
        PlotCanvas* c = ui_->plotCanvas();
        c->zoom(f, c->toWorld(c->viewRect().center()));
    };
    for (QAction* a : {ui_->menuview_zoom_in, ui_->zoom_in_btn}) connect(a, &QAction::triggered, this, [zoomBy] { zoomBy(defNum("global_zoom_ratio")); });
    for (QAction* a : {ui_->menuview_zoom_out, ui_->zoom_out_btn})
        connect(a, &QAction::triggered, this, [zoomBy] { zoomBy(1 / defNum("global_zoom_ratio")); });
    // Herramientas: menú Herramienta y barra de herramientas.
    for (QAction* a : {ui_->ncclear_tool_menuAction, ui_->ncc_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(nccTool, NccTool::info());
            nccTool->setToolUi();
        });
    for (QAction* a : {ui_->paint_tool_menuAction, ui_->paint_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(paintTool, PaintTool::info());
            paintTool->setToolUi();
        });
    for (QAction* a : {ui_->drilling_tool_menuAction, ui_->drill_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(drillingTool, DrillingTool::info());
            drillingTool->setToolUi();
        });
    for (QAction* a : {ui_->cutout_tool_menuAction, ui_->cutout_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(cutoutTool, CutOutTool::info());
            cutoutTool->setToolUi();
        });
    // Las teclas en el árbol del Proyecto van a los atajos de la ventana (view.keyPressed -> ui.keyPressEvent).
    connect(ui_->collection_view, &KeySensitiveListView::keyPressed, this,
            [this](int key) { handleKey(key, QApplication::keyboardModifiers()); });
    for (QAction* a : {ui_->isolation_tool_menuAction, ui_->isolation_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(isolationTool, IsolationTool::info());
            isolationTool->setToolUi();
        });
    for (QAction* a : {ui_->transform_tool_menuAction, ui_->transform_btn, ui_->grb_transform_btn,
                       ui_->grb_transform_menuitem, ui_->geo_transform_btn, ui_->geo_transform_menuitem})
        connect(a, &QAction::triggered, this, [this] {
            runTool(transformTool, TransformTool::info());
            transformTool->setToolUi();
        });
    // Distancia (Ctrl+M) y Distancia mínima (Shift+M): run(toggle=True).
    for (QAction* a : {ui_->distance_tool_menuAction, ui_->distance_btn})
        connect(a, &QAction::triggered, this, [this] { distanceTool->run(); });
    for (QAction* a : {ui_->distance_min_tool_menuAction, ui_->distance_min_btn})
        connect(a, &QAction::triggered, this, [this] { distanceMinTool->run(); });
    connect(ui_->move_tool_menuAction, &QAction::triggered, this, [this] { moveTool->toggle(); });
    // Propiedades: el menú llama run(toggle=False) (triggered pasa checked = False); la tecla P, run().
    connect(ui_->properties_tool_menuAction, &QAction::triggered, this, [this] { propertiesTool->run(false); });
    for (QAction* a : {ui_->dblsidedtool_menuAction, ui_->dblsided_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(dblsidedTool, DblSidedTool::info());
            dblsidedTool->setToolUi();
        });
    // Los manejadores del mouse de la aplicación (app.pos y la selección en el canvas: AppCanvasSelection.cpp).
    PlotCanvas* canvas = ui_->plotCanvas();
    connect(canvas, &PlotCanvas::mouseClicked, this, [this](QPointF w, Qt::MouseButton b) { onCanvasPress(w, b); });
    connect(canvas, &PlotCanvas::mouseMoved, this,
            [this](QPointF w) { onCanvasMove(w, QApplication::mouseButtons() & Qt::LeftButton); });
    connect(canvas, &PlotCanvas::mouseReleased, this, [this](QPointF w, Qt::MouseButton b, bool dragged) {
        onCanvasRelease(w, b, QApplication::keyboardModifiers(), dragged);
    });
    connect(canvas, &PlotCanvas::mouseDoubleClicked, this, [this](Qt::MouseButton b) { onCanvasDoubleClick(b); });
    // Opciones -> Voltear en el eje X / Y (on_flipx refleja con mirror('Y'), on_flipy con mirror('X')).
    connect(ui_->menuoptions_transform_flipx, &QAction::triggered, this, [this] { flip(Axis::Y); });
    connect(ui_->menuoptions_transform_flipy, &QAction::triggered, this, [this] { flip(Axis::X); });
    // ObjectCollection.on_list_selection_change: el primer objeto recién elegido carga sus Propiedades (item_selected ->
    // on_row_selected); si solo se quitaron objetos de la selección, la pestaña vuelve a la vacía. Activarlo (doble clic
    // o Enter) además abre la pestaña.
    connect(ui_->collection_view->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection& selected, const QItemSelection&) {
                const QModelIndexList idx = selected.indexes();
                FlatCAMObj* o = idx.isEmpty() ? nullptr : collection_->objectAt(idx.front());
                showProperties(o);
                if (o) {
                    ui_->inform(QString("[selected]<span style=\"color:%1;\">%2</span> %3")
                                    .arg(kindColor(o->kind()), o->name(), tr_("selected")));
                } else {
                    ui_->inform("");
                }
            });
    connect(ui_->collection_view, &QAbstractItemView::activated, this, [this](const QModelIndex& idx) {
        if (collection_->objectAt(idx)) ui_->notebook->setCurrentWidget(ui_->properties_tab);
    });
    connect(collection_, &ObjectCollection::objectRenamed, this, [this](FlatCAMObj* o, const QString& old) {
        shouldWeSave_ = true;
        ui_->inform(tr_("Object renamed from <b>{old}</b> to <b>{new}</b>").replace("{old}", old).replace("{new}", o->name()));
    });
}

QString App::lastFolder() const { return lastFolder_; }

void App::clearProject() {
    ui_->properties_scroll_area->takeWidget();
    showProperties(nullptr);
    for (FlatCAMObj* o : collection_->objects()) {
        ui_->plotCanvas()->removeLayer(o->id());
        collection_->remove(o);
    }
    projectFilename_.clear();
}

void App::onFileOpenProject() {
    const QString f = QFileDialog::getOpenFileName(ui_.get(), tr_("Open Project"), lastFolder(),
                                                   "FlatCAM Project (*.FlatPrj);;All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1").arg(tr_("Cancelled.")));
        return;
    }
    openProject(f);
}

bool App::openProject(const QString& filename) {
    ui_->setUiTitle(tr_("Loading Project ... Please Wait ..."));
    JsonValue root;
    std::string error;
    bool ok = false;
    {
        BusyScope busy(ui_.get());
        ok = ProjectFile::read(filename.toStdString(), root, error, ProjectIO::skipFn());
    }
    if (!ok || !root["objs"].isArray()) {
        ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open project file"), filename));
        ui_->setUiTitle(projectFilename_.isEmpty() ? tr_("New Project - Not saved") : projectFilename_);
        return false;
    }
    clearProject();
    projectOptions_ = root["options"];
    QStringList failed;
    {
        BusyScope busy(ui_.get());
        for (const JsonValue& d : root["objs"].items()) {
            QString warning;
            std::unique_ptr<FlatCAMObj> obj =
                ProjectIO::objectFrom(d, warning, [this](const QString& n) -> std::optional<BoundingBox> {
                    FlatCAMObj* o = collection_->byName(n);
                    if (!o) return std::nullopt;
                    const QRectF b = o->bounds();
                    return BoundingBox{b.left(), b.top(), b.right(), b.bottom()};
                });
            if (!obj) {
                failed << warning;
                continue;
            }
            addObject(std::move(obj));
        }
    }
    projectFilename_ = filename;
    lastFolder_ = QFileInfo(filename).absolutePath();
    ui_->setUiTitle(filename);
    registerRecent("project", filename);
    if (!failed.isEmpty()) ui_->inform("[WARNING_NOTCL] " + failed.join("; "));
    else ui_->inform(QString("[success] %1: %2").arg(tr_("Project loaded from"), filename));
    shouldWeSave_ = false;  // un proyecto recién abierto no tiene cambios
    return true;
}

bool App::saveProject(const QString& filename) {
    const JsonValue& options = appOptions();
    std::string error;
    bool ok = false;
    {
        BusyScope busy(ui_.get());
        const JsonValue root = ProjectIO::projectDict(collection_->objects(), options);
        ok = ProjectFile::write(filename.toStdString(), root, options["global_save_compressed"].toBool(true),
                                static_cast<int>(options["global_compression_level"].toDouble(3)), error);
    }
    if (!ok) {
        ui_->inform(QString("[ERROR_NOTCL] %1").arg(tr_("The object is used by another application.")));
        return false;
    }
    projectFilename_ = filename;
    shouldWeSave_ = false;
    lastFolder_ = QFileInfo(filename).absolutePath();
    ui_->setUiTitle(filename);
    registerRecent("project", filename);  // file_opened.emit("project", ...) al guardar
    ui_->inform(QString("[success] %1: %2").arg(tr_("Project saved to"), filename));
    return true;
}

void App::onFileSaveProject() {
    if (projectFilename_.isEmpty()) onFileSaveProjectAs();
    else saveProject(projectFilename_);
}

void App::onFileSaveProjectAs() {
    // Nombre propuesto: "<carpeta>/Proyecto_AAAAMMDD_HHMMSS".
    const QString date = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    const QString f = QFileDialog::getSaveFileName(ui_.get(), tr_("Save Project As ..."),
                                                   QString("%1/%2_%3").arg(lastFolder(), tr_("Project"), date),
                                                   "FlatCAM Project .FlatPrj (*.FlatPrj);; All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1").arg(tr_("Cancelled.")));
        return;
    }
    saveProject(f.endsWith(".FlatPrj", Qt::CaseInsensitive) ? f : f + ".FlatPrj");
}

void App::onFileOpenGerber() {
    QString selected = lastGerberFilter_;
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Open Gerber"), lastFolder(),
                                                            QString::fromUtf8(kGerberFilter), &selected);
    lastGerberFilter_ = selected;
    if (files.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1: %2").arg(tr_("Open Gerber"), tr_("Cancelled.")));
        return;
    }
    for (const QString& f : files) openGerber(f);
}

void App::onFileOpenExcellon() {
    QString selected = lastExcellonFilter_;
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Open Excellon"), lastFolder(),
                                                            QString::fromUtf8(kExcellonFilter), &selected);
    lastExcellonFilter_ = selected;
    if (files.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1").arg(tr_("Cancelled.")));
        return;
    }
    for (const QString& f : files) openExcellon(f);
}

FlatCAMObj* App::openGerber(const QString& filename) {
    ui_->activity_view->setBusy(tr_("Opening ..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto gerber = std::make_unique<Gerber>();
    const bool ok = gerber->parseFile(filename.toStdString());
    QApplication::restoreOverrideCursor();
    ui_->activity_view->setIdle();
    if (!ok || gerber->solidGeometry().empty()) {
        ui_->inform(QString("[ERROR_NOTCL]%1").arg(tr_("Open Gerber failed. Probable not a Gerber file.")));
        return nullptr;
    }
    lastFolder_ = QFileInfo(filename).absolutePath();
    // new_object: si el objeto no está en las unidades de la aplicación, se convierte (convert_units).
    if (gerber->units() != appUnitsKind()) {
        ui_->inform(tr_("Converting units to ") + ": " + appUnits());
        gerber->convertUnits(appUnitsKind());
    }
    FlatCAMObj* obj = addObject(std::make_unique<GerberObject>(QFileInfo(filename).fileName(), std::move(gerber)));
    registerRecent("gerber", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

FlatCAMObj* App::openExcellon(const QString& filename) {
    ui_->activity_view->setBusy(tr_("Opening ..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto exc = std::make_unique<Excellon>();
    const bool ok = exc->parseFile(filename.toStdString());
    QApplication::restoreOverrideCursor();
    ui_->activity_view->setIdle();
    bool hasGeometry = false;
    for (const auto& [id, t] : exc->drillTools()) hasGeometry = hasGeometry || !t.solidGeometry.empty();
    if (!ok || !hasGeometry) {
        ui_->inform(QString("[ERROR_NOTCL] %1").arg(tr_("Open Excellon file failed. Probable not an Excellon file.")));
        return nullptr;
    }
    lastFolder_ = QFileInfo(filename).absolutePath();
    if (exc->units() != appUnitsKind()) {
        ui_->inform(tr_("Converting units to ") + ": " + appUnits());
        exc->convertUnits(appUnitsKind());
    }
    FlatCAMObj* obj = addObject(std::make_unique<ExcellonObject>(QFileInfo(filename).fileName(), std::move(exc)));
    registerRecent("excellon", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

FlatCAMObj* App::openGcode(const QString& filename) {
    // open_gcode: el texto tal cual en un CNC Job; gcode_parse falla si alguna línea tiene '%' (o MOIN/MOMM).
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open"), filename));
        return nullptr;
    }
    const QString text = QString::fromUtf8(f.readAll());
    QString units = "MM";
    static const QRegularExpression g2x(R"(^\s*G\s*(20|21)(\.0*)?\b)");
    for (const QString& line : text.split('\n')) {
        if (line.contains('%') || line.contains("MOIN") || line.contains("MOMM")) {
            ui_->inform("[ERROR_NOTCL] " + tr_("This is not GCODE"));
            ui_->inform("[ERROR_NOTCL] " +
                        tr_("Failed to create CNCJob Object. Probable not a GCode file. Try to load it from File menu.\n "
                            "Attempting to create a FlatCAM CNCJob Object from G-Code file failed during processing"));
            return nullptr;
        }
        const auto m = g2x.match(line);
        if (m.hasMatch()) units = m.captured(1) == "20" ? "IN" : "MM";
    }
    auto job = std::make_unique<CncJob>();
    job->restore(std::string(), {text.toStdString()});
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    h.type = "Geometry";  // options['type'] de un CNCJobObject nuevo
    h.units = units.toStdString();
    h.name = QFileInfo(filename).fileName().toStdString();
    lastFolder_ = QFileInfo(filename).absolutePath();
    FlatCAMObj* obj = addObject(std::make_unique<CNCJobObject>(QFileInfo(filename).fileName(), std::move(job), h,
                                                               std::vector<CncToolRow>{}));
    registerRecent("cncjob", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

void App::onFileOpenGcode() {
    QString selected = lastGcodeFilter_;
    const QStringList files = QFileDialog::getOpenFileNames(
        ui_.get(), tr_("Open G-Code"), lastFolder(),
        "G-Code Files (*.txt *.nc *.ncc *.tap *.gcode *.cnc *.ecs *.fnc *.dnc *.ncg *.gc *.fan *.fgc *.din *.xpi *.hnc "
        "*.h *.i *.ncp *.min *.gcd *.rol *.mpr *.ply *.out *.eia *.sbp *.mpf);;All Files (*.*)",
        &selected);
    lastGcodeFilter_ = selected;
    if (files.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    for (const QString& f : files) openGcode(f);
}

FlatCAMObj* App::openByExtension(const QString& filename) {
    const QString ext = QFileInfo(filename).suffix().toLower();
    // dropEvent de MainGUI: cada lista por separado (".tap" está en la de Excellon y en la de G-code: abre los dos).
    static const QStringList kGcodeExt = {"cnc", "din", "dnc", "ecs", "eia", "fan", "fgc", "fnc", "gc", "gcd", "gcode",
                                          "h", "hnc", "i", "min", "mpf", "mpr", "nc", "ncc", "ncg", "ngc", "ncp", "out",
                                          "ply", "rol", "sbp", "tap", "xpi"};
    if (kGerberExt.contains(ext)) return openGerber(filename);
    if (ext == "tap") {
        FlatCAMObj* e = openExcellon(filename);
        FlatCAMObj* g = openGcode(filename);
        return g ? g : e;
    }
    if (kExcellonExt.contains(ext)) return openExcellon(filename);
    if (kGcodeExt.contains(ext)) return openGcode(filename);
    if (ext == "svg") return importSvg(filename, false);
    if (ext == "dxf") return importDxf(filename, false);
    if (ext == "flatprj") {
        openProject(filename);
        return nullptr;
    }
    return nullptr;
}

void App::objectChanged(FlatCAMObj* obj) {
    shouldWeSave_ = true;
    ui_->plotCanvas()->setLayer(obj->id(), obj->plotData());
    if (obj == activeObject()) showProperties(obj);  // las tablas y los límites cambiaron
}

const JsonValue& App::appOptions() const {
    return projectOptions_.isObject() ? projectOptions_ : defaults();
}

const JsonValue& App::defaults() const { return prefs_->defaults(); }

void App::setupPreferences() {
    prefs_ = new Preferences(ui_.get(), this);
    prefs_->setup(dataPath());
    // on_options_app2project: leer el formulario y copiar las preferencias a las opciones del proyecto.
    connect(prefs_, &Preferences::optionsToProject, this, [this] {
        prefs_->readForm();
        if (projectOptions_.isObject())
            for (const auto& [k, v] : prefs_->defaults().members()) projectOptions_.set(k, v);
    });
    // on_app_restart: vuelve a abrir la aplicación con los mismos argumentos.
    prefs_->onRestart = [this] {
        if (!finalSave()) return;
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QCoreApplication::arguments().mid(1));
        QApplication::quit();
    };
    connect(ui_->menueditpreferences, &QAction::triggered, prefs_, &Preferences::open);
    connect(ui_->menuedittoggleunits, &QAction::triggered, this, [this] { onToggleUnitsClick(); });
    connect(prefs_->tab()->general_app_group_units_radio, &RadioSet::activatedCustom, this, [this] {
        if (!unitsRadioMuted_) onToggleUnits(false);
    });
    setScreenUnits(appUnits());
    connect(ui_->pref_status_label, &FCLabel::clicked, prefs_, [this] { prefs_->toggle(); });
    PreferencesTab* t = prefs_->tab();
    connect(t->pref_save_button, &QPushButton::clicked, prefs_, [this] { prefs_->onSave(true); });
    connect(t->pref_apply_button, &QPushButton::clicked, prefs_, [this] { prefs_->onSave(false); });
    connect(t->pref_close_button, &QPushButton::clicked, prefs_, &Preferences::onCloseButton);
    connect(t->pref_defaults_button, &QPushButton::clicked, prefs_, &Preferences::onRestoreDefaults);
    connect(t->pref_open_button, &QPushButton::clicked, prefs_, &Preferences::openFolder);
    connect(t->clear_btn, &QPushButton::clicked, prefs_, [this] { prefs_->clearGuiSettings(false); });
}

void App::jumpTo(const QPointF& location, bool fitCenter) {
    ui_->plotCanvas()->jumpTo(location, fitCenter);
    ui_->updateCoordinates(location);
}

std::vector<FlatCAMObj*> App::selectedObjects() const {
    std::vector<FlatCAMObj*> out;
    QModelIndexList rows = ui_->collection_view->selectionModel()->selectedIndexes();
    std::sort(rows.begin(), rows.end(), [](const QModelIndex& a, const QModelIndex& b) {
        return a.parent().row() != b.parent().row() ? a.parent().row() < b.parent().row() : a.row() < b.row();
    });
    for (const QModelIndex& i : rows)
        if (FlatCAMObj* o = collection_->objectAt(i)) out.push_back(o);
    return out;
}

bool App::flip(Axis axis) {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    if (objs.empty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return false;
    }
    // Centro de la caja que contiene a todos los elegidos.
    QRectF box;
    for (FlatCAMObj* o : objs) box = box.isNull() ? o->bounds() : box.united(o->bounds());
    const Point2D center{0.5 * (box.left() + box.right()), 0.5 * (box.top() + box.bottom())};
    for (FlatCAMObj* o : objs) {
        if (!o->mirror(axis, center)) {
            ui_->inform(QString("[ERROR_NOTCL] %1: %2.").arg(tr_("Action was not executed"), o->name()));
            return false;
        }
        objectChanged(o);
    }
    ui_->inform(QString("[success] %1.").arg(axis == Axis::Y ? tr_("Flip on X axis done") : tr_("Flip on Y axis done")));
    return true;
}

FlatCAMObj* App::activeObject() const {
    // get_active: el primero de los elegidos (selectedIndexes), o ninguno.
    const QModelIndexList sel = ui_->collection_view->selectionModel()->selectedIndexes();
    return sel.isEmpty() ? nullptr : collection_->objectAt(sel.front());
}

bool App::exportGcode(CNCJobObject* job, QString filename) {
    if (filename.isEmpty()) {
        // cncjob_save_filters
        filename = QFileDialog::getSaveFileName(ui_.get(), tr_("Export Code ..."), lastFolder() + "/" + job->name(),
                                                "G-Code Files .nc (*.nc);;G-Code Files .cnc (*.cnc);;"
                                                "G-Code Files .gcode (*.gcode);;G-Code Files .ngc (*.ngc);;"
                                                "G-Code Files .tap (*.tap);;All Files (*.*)");
    }
    if (filename.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1").arg(tr_("Export cancelled ...")));
        return false;
    }
    // export_gcode_handler: el objeto pasa a llamarse como el archivo (sin la extensión).
    const QString newName = QFileInfo(filename).completeBaseName();
    if (newName != job->name()) {
        const QModelIndex idx = collection_->indexOf(job);
        collection_->setData(idx, newName, Qt::EditRole);
    }
    if (!job->saveTo(filename)) {
        ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open file"), filename));
        return false;
    }
    lastFolder_ = QFileInfo(filename).absolutePath();
    ui_->inform(QString("[success] %1: %2").arg(tr_("File saved to"), filename));
    return true;
}

FlatCAMObj* App::addObject(std::unique_ptr<FlatCAMObj> o) {
    FlatCAMObj* obj = collection_->append(std::move(o));
    shouldWeSave_ = true;
    // ObjectCollection.append: se expande el grupo y, con global_project_autohide, se abre el panel izquierdo.
    ui_->collection_view->setExpanded(collection_->groupIndex(obj->kind()), true);
    ui_->splitter->setSizes({1, 1});
    ui_->menu_toggle_nb->setChecked(true);
    // AppObject.on_object_created: aviso, dibujo y encuadre (object_plotted -> on_zoom_fit).
    ui_->inform(QString("[selected] %1 %2: <span style=\"color:%3;\">%4</span>")
                    .arg(kindName(obj->kind()), tr_("created/selected"), kindColor(obj->kind()), obj->name()));
    ui_->plotCanvas()->setLayer(obj->id(), obj->plotData());
    ui_->plotCanvas()->setLayerVisible(obj->id(), obj->plotEnabled());
    ui_->plotCanvas()->setLayerOrder(collection_->drawOrder());
    onZoomFit();
    return obj;
}

FlatCAMObj* App::generateCncJob(GeometryObject* geo, const std::vector<int>& toolIds, GeometryObjectUI* polish) {
    QString error;
    std::vector<int> ids = toolIds;
    if (polish && polish->polish_cb->isChecked() && !ids.empty()) {
        // on_polish: una herramienta nueva ("Pulir") que recorre la caja del objeto, agrandada el margen, con el método
        // elegido (contorno y unir siempre); se agrega a la Geometría y al CNC Job.
        const double margin = polish->polish_margin_entry->value();
        const double dia = std::round(polish->polish_dia_entry->value() * 1e4) / 1e4;  // dec_format
        const QRectF b = geo->bounds();
        const Shape box = Shape::box(b.left() - margin, b.top() - margin, b.right() + margin, b.bottom() + margin);
        const int m = polish->polish_method_combo->currentIndex();
        const auto method = m == 1 ? PolygonClearing::Method::Seed
                          : m == 2 ? PolygonClearing::Method::Lines
                                   : PolygonClearing::Method::Standard;
        const auto lines = PolygonClearing::clear(box, polish->polish_dia_entry->value(), 64,
                                                  polish->polish_over_entry->value() / 100.0, true, true, method);
        if (!lines) {
            ui_->inform("[ERROR_NOTCL] " + tr_("Geometry could not be painted completely"));
            return nullptr;
        }
        GeometryTool t = defaultGeometryTool();
        t.tooldia = dia;
        t.offset = "Path";
        t.offsetValue = 0.0;
        t.type = tr_("Polish");
        t.toolType = "C1";
        t.data.travelZ = polish->polish_travelz_entry->value();
        t.data.cutZ = polish->polish_pressure_entry->value();
        for (const auto& l : *lines) {
            const Shape s = PolygonClearing::toShape(l);
            if (!s.isEmpty()) t.solidGeometry.push_back(s);
        }
        const int uid = geo->tools().empty() ? 1 : geo->tools().rbegin()->first + 1;
        geo->tools()[uid] = t;
        ids.push_back(uid);
    }
    std::unique_ptr<CNCJobObject> job = geo->makeCncJob(ids, error, excAreas_->areas());
    if (!job) {
        ui_->inform(error);
        return nullptr;
    }
    FlatCAMObj* obj = addObject(std::move(job));
    ui_->inform("[success] " + tr_("Finished G-Code processing") + "...");
    return obj;
}

void App::onZoomFit() { ui_->plotCanvas()->fitView(); }

void App::showProperties(FlatCAMObj* obj) {
    QScrollArea* area = ui_->properties_scroll_area;
    area->takeWidget();  // no se destruye: es del objeto o el panel vacío
    QWidget* panel = obj ? obj->propertiesUi() : defaultProperties_;
    if (obj && obj->kind() == ObjectKind::CNCJob) {
        auto* job = static_cast<CNCJobObject*>(obj);
        static_cast<CNCObjectUI*>(panel)->onExport = [this, job] { exportGcode(job); };
    } else if (obj && obj->kind() == ObjectKind::Geometry) {
        auto* geo = static_cast<GeometryObject*>(obj);
        auto* gui = static_cast<GeometryObjectUI*>(panel);
        gui->onGenerateCnc = [this, geo, gui] { generateCncJob(geo, gui->toolsForCnc(), gui); };
        gui->inform = [this](const QString& m) { ui_->inform(m); };
        gui->dataPath = dataPath();
        gui->onAddFromDb = [this] { openToolsDatabase(); };
        gui->onToolsChanged = [this, geo] {
            ui_->plotCanvas()->setLayer(geo->id(), geo->plotData());
        };
        // Áreas de exclusión (una sola vez por panel).
        if (!gui->property("exclusionConnected").toBool()) {
            gui->setProperty("exclusionConnected", true);
            excAreas_->bindPanel(gui->exclusion_cb, gui->exclusion_frame, gui->area_shape_radio, gui->strategy_radio,
                                 gui->over_z_label, gui->over_z_entry, gui->add_area_button, gui->delete_area_button,
                                 gui->delete_sel_area_button, gui->generate_cnc_button, gui->exclusion_table,
                                 [geo] { return geo->solidGeometry(); }, "geometry");
        }
    }
    area->setWidget(panel);
    // FlatCAMObj.set_ui: Enter en el campo Nombre renombra el objeto (on_name_activate, sin mensaje).
    if (obj && !panel->property("nameConnected").toBool()) {
        FCEntry* entry = nullptr;
        if (auto* p = qobject_cast<GerberObjectUI*>(panel)) entry = p->name_entry;
        else if (auto* p2 = qobject_cast<ExcellonObjectUI*>(panel)) entry = p2->name_entry;
        else if (auto* p3 = qobject_cast<GeometryObjectUI*>(panel)) entry = p3->name_entry;
        else if (auto* p4 = qobject_cast<CNCObjectUI*>(panel)) entry = p4->name_entry;
        if (entry) {
            connect(entry, &QLineEdit::returnPressed, this, [this, obj, entry] { collection_->rename(obj, entry->text()); });
            panel->setProperty("nameConnected", true);
        }
        // FlatCAMObj.set_ui: escala, desplazamiento y el botón Transformaciones del panel.
        if (auto* p = qobject_cast<GerberObjectUI*>(panel)) connectTransformations(p, obj);
        else if (auto* p2 = qobject_cast<ExcellonObjectUI*>(panel)) connectTransformations(p2, obj);
        else if (auto* p3 = qobject_cast<GeometryObjectUI*>(panel)) connectTransformations(p3, obj);
        else if (auto* p4 = qobject_cast<CNCObjectUI*>(panel)) connectTransformations(p4, obj);
    }
}

template <typename Panel>
void App::connectTransformations(Panel* p, FlatCAMObj* obj) {
    // on_offset_button_click: el vector (x, y) de la fórmula; si no se puede leer, el original falla sin mensaje.
    auto offset = [this, p, obj] {
        const auto v = p->offsetvector_entry->evaluated();
        if (!v || v->size() != 2) return;
        if (obj->offset((*v)[0], (*v)[1])) objectChanged(obj);
    };
    // on_scale_button_click: el mismo factor en X e Y, desde el origen.
    auto scale = [this, p, obj] {
        const auto f = p->scale_entry->evaluated();
        if (!f) {
            ui_->inform("[ERROR_NOTCL] " + tr_("Scaling could not be executed."));
            return;
        }
        if (*f == 1.0) {
            ui_->inform("[success] " + tr_("Scale done."));
            return;
        }
        if (obj->scale(*f, *f, {0, 0})) objectChanged(obj);
        ui_->inform("[success] " + tr_("Scale done."));
    };
    connect(p->offset_button, &QPushButton::clicked, this, offset);
    connect(p->offsetvector_entry, &QLineEdit::returnPressed, this, offset);
    connect(p->scale_button, &QPushButton::clicked, this, scale);
    connect(p->scale_entry, &QLineEdit::returnPressed, this, scale);
    connect(p->transformations_button, &QPushButton::clicked, this, [this] {
        runTool(transformTool, TransformTool::info());
        transformTool->setToolUi();
    });
}

bool App::handleKey(int key, Qt::KeyboardModifiers modifiers) {
    // MainGUI.keyPressEvent con call_source == 'app' (en el Mac, Ctrl es la tecla Cmd). Los atajos de herramientas o
    // funciones que el port todavía no tiene no hacen nada.
    MainGUI* ui = ui_.get();
    auto tool = [](QAction* a) {
        a->trigger();
        return true;
    };
    if (modifiers == (Qt::ControlModifier | Qt::ShiftModifier)) {
        if (key == Qt::Key_S) return tool(ui->menufilesaveprojectas);
        return false;
    }
    if (modifiers == Qt::ControlModifier) {
        switch (key) {
            case Qt::Key_A: onSelectAll(); return true;
            case Qt::Key_C: copySelected(); return true;
            case Qt::Key_D: openToolsDatabase(); return true;
            case Qt::Key_E: onFileOpenExcellon(); return true;
            case Qt::Key_G: onFileOpenGerber(); return true;
            case Qt::Key_M: distanceTool->run(); return true;
            case Qt::Key_N: onFileNewClick(); return true;
            case Qt::Key_O: onFileOpenProject(); return true;
            case Qt::Key_S:
                // En la pestaña Preferencias, Ctrl+S aplica (on_save_button(save_to_file=False)).
                if (ui->plot_tab_area->currentWidget() && ui->plot_tab_area->currentWidget()->objectName() == "preferences_tab") {
                    prefs_->onSave(false);
                    return true;
                }
                onFileSaveProject();
                return true;
            case Qt::Key_F10: ui->onTogglePlotArea(); return true;
            default: return false;
        }
    }
    if (modifiers == Qt::ShiftModifier) {
        switch (key) {
            case Qt::Key_A: ui->onToggleAxis(); return true;
            case Qt::Key_C: {  // on_copy_name
                FlatCAMObj* o = activeObject();
                if (!o) {
                    ui->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
                    return true;
                }
                QApplication::clipboard()->setText(o->name());
                ui->inform(tr_("Name copied to clipboard ..."));
                return true;
            }
            case Qt::Key_G: ui->onToggleGridLines(); return true;
            case Qt::Key_H: ui->onToggleHud(); return true;
            case Qt::Key_J: onLocate(); return true;
            case Qt::Key_M: distanceMinTool->run(); return true;
            case Qt::Key_P: prefs_->open(); return true;
            case Qt::Key_R: onRotateSelection(-defNum("tools_transform_rotate")); return true;
            case Qt::Key_W: onWorkspaceToggle(); return true;
            case Qt::Key_X: onSkewSelection('X'); return true;
            case Qt::Key_Y: onSkewSelection('Y'); return true;
            default: return false;
        }
    }
    if (modifiers == Qt::AltModifier) {
        switch (key) {
            case Qt::Key_1: ui->menuviewenable->trigger(); return true;
            case Qt::Key_2: ui->menuviewdisableall->trigger(); return true;
            case Qt::Key_3: ui->menuviewenableother->trigger(); return true;
            case Qt::Key_4: ui->menuviewdisableother->trigger(); return true;
            case Qt::Key_D: return tool(ui->dblsided_btn);
            case Qt::Key_I: return tool(ui->isolation_btn);
            case Qt::Key_N: return tool(ui->ncc_btn);
            case Qt::Key_P: return tool(ui->paint_btn);
            case Qt::Key_S: viewSource(activeObject()); return true;
            case Qt::Key_T: return tool(ui->transform_btn);
            case Qt::Key_X: return tool(ui->cutout_btn);
            case Qt::Key_F10: ui->onFullscreen(); return true;
            default: return false;
        }
    }
    if (modifiers != Qt::NoModifier) return false;
    // on_select_tab: si el panel está escondido lo muestra; si ya estaba en esa pestaña, lo esconde.
    auto selectTab = [ui](QWidget* tab) {
        if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
        else if (ui->notebook->currentWidget() == tab) ui->splitter->setSizes({0, 1});
        ui->notebook->setCurrentWidget(tab);
        return true;
    };
    PlotCanvas* canvas = ui->plotCanvas();
    switch (key) {
        case Qt::Key_F1: openHelpLink(0); return true;
        case Qt::Key_F3: onShortcutList(); return true;
        case Qt::Key_F4: openHelpLink(1); return true;
        case Qt::Key_F5: plotAll(); return true;
        case Qt::Key_1: return selectTab(ui->project_tab);
        case Qt::Key_2: return selectTab(ui->properties_tab);
        case Qt::Key_3: return selectTab(ui->tool_tab);
        case Qt::Key_Delete: deleteSelected(); return true;  // on_delete_keypress
        case Qt::Key_Escape:
            // on_key_press de las herramientas: Escape cancela Mover o la selección de zonas o polígonos.
            if (moveTool->active()) {
                moveTool->cancel();
                return true;
            }
            if (picker_->cancel()) {
                ui->inform("[WARNING_NOTCL] " + tr_("Cancelled by user request."));
                return true;
            }
            // on_deselect_all, salir de la pantalla completa y dejar de esperar el clic de "Fijar el origen".
            for (FlatCAMObj* o : collection_->objects()) setActive(o, false);
            deleteSelectionShape();
            ui->onFullscreen(true);
            if (setOriginConnection_) {
                disconnect(setOriginConnection_);
                setOriginConnection_ = {};
            }
            ui->inform("");
            return true;
        case Qt::Key_Space: {
            // El "Dibujar" de cada elegido cambia.
            const std::vector<FlatCAMObj*> sel = selectedObjects();
            for (FlatCAMObj* o : sel) {
                o->setPlotEnabled(!o->plotEnabled());
                canvas->setLayerVisible(o->id(), o->plotEnabled());
                if (o == activeObject()) showProperties(o);
            }
            deleteSelectionShape();
            return true;
        }
        case Qt::Key_Up:
        case Qt::Key_Down: {
            // Solo con el foco en el árbol del Proyecto: el anterior o el siguiente (dando la vuelta).
            if (QApplication::focusWidget() != ui->collection_view) return false;
            FlatCAMObj* active = activeObject();
            const std::vector<FlatCAMObj*> all = collection_->objects();
            for (FlatCAMObj* o : all) setActive(o, false);
            if (!active || all.empty()) return true;
            const auto it = std::find(all.begin(), all.end(), active);
            const long idx = it - all.begin(), n = static_cast<long>(all.size());
            setActive(all[static_cast<std::size_t>(key == Qt::Key_Up ? (idx + n - 1) % n : (idx + 1) % n)], true);
            return true;
        }
        case Qt::Key_G: ui->grid_snap_btn->trigger(); return true;
        case Qt::Key_J: onJumpTo(); return true;
        case Qt::Key_M: moveTool->toggle(); return true;
        case Qt::Key_O: onSetOrigin(); return true;
        case Qt::Key_P: propertiesTool->run(true); return true;
        case Qt::Key_Q: onToggleUnitsClick(); return true;  // cambiar las unidades
        case Qt::Key_R: onRotateSelection(defNum("tools_transform_rotate")); return true;
        case Qt::Key_V: onZoomFit(); return true;
        case Qt::Key_X: flip(Axis::Y); return true;  // on_flipx
        case Qt::Key_Y: flip(Axis::X); return true;  // on_flipy
        // Acercar y alejar alrededor del mouse (global_zoom_ratio, 1,5 de fábrica).
        case Qt::Key_Equal: canvas->zoom(defNum("global_zoom_ratio"), mouse_); return true;
        case Qt::Key_Minus: canvas->zoom(1 / defNum("global_zoom_ratio"), mouse_); return true;
        case Qt::Key_QuoteLeft: ui->onToggleNotebook(); return true;
        default: return false;
    }
}

bool App::finalSave() {
    if (!shouldWeSave_ || collection_->objects().empty()) return true;
    QMessageBox box(ui_.get());
    box.setText(tr_("There are files/objects modified in FlatCAM. \nDo you want to Save the project?"));
    box.setWindowTitle(tr_("Save changes"));
    box.setWindowIcon(QIcon(":/images/save_as.png"));
    box.setIcon(QMessageBox::Question);
    QPushButton* yes = box.addButton(tr_("Yes"), QMessageBox::YesRole);
    QPushButton* no = box.addButton(tr_("No"), QMessageBox::NoRole);
    box.addButton(tr_("Cancel"), QMessageBox::RejectRole);
    box.setDefaultButton(yes);
    box.exec();
    QAbstractButton* r = box.clickedButton();
    if (r == yes) {
        onFileSaveProjectAs();  // on_file_saveprojectas(quit_action=True): sale si se guardó
        return !shouldWeSave_;
    }
    return r == no;
}

bool App::eventFilter(QObject* obj, QEvent* ev) {
    if (obj == ui_.get() && ev->type() == QEvent::Close) {
        // closeEvent de MainGUI -> final_save: con "Cancelar" la ventana no se cierra.
        if (!finalSave()) {
            ev->ignore();
            return true;
        }
        return false;
    }
    if (obj == ui_.get()) {
        if (ev->type() == QEvent::DragEnter || ev->type() == QEvent::DragMove) {
            auto* e = static_cast<QDropEvent*>(ev);
            if (e->mimeData()->hasUrls()) {
                e->acceptProposedAction();
                return true;
            }
        } else if (ev->type() == QEvent::Drop) {
            auto* e = static_cast<QDropEvent*>(ev);
            for (const QUrl& u : e->mimeData()->urls())
                if (u.isLocalFile()) openByExtension(u.toLocalFile());
            e->acceptProposedAction();
            return true;
        } else if (ev->type() == QEvent::KeyPress) {
            // MainGUI.keyPressEvent sin modificadores: V encuadra; 1, 2 y 3 eligen la pestaña del panel izquierdo.
            auto* k = static_cast<QKeyEvent*>(ev);
            if (handleKey(k->key(), k->modifiers())) return true;
        }
    }
    return QObject::eventFilter(obj, ev);
}

}  // namespace flatcam::gui
