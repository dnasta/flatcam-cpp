#include "gui/App.h"

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
#include "gui/ProjectIO.h"
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
    // Las herramientas (install_tools): sus paneles viven mientras viva la ventana.
    nccTool = new NccTool(this, ui_.get());
    paintTool = new PaintTool(this, ui_.get());
    drillingTool = new DrillingTool(this, ui_.get());
    cutoutTool = new CutOutTool(this, ui_.get());
    dblsidedTool = new DblSidedTool(this, ui_.get());
    isolationTool = new IsolationTool(this, ui_.get());
    for (QWidget* w : std::initializer_list<QWidget*>{nccTool, paintTool, drillingTool, cutoutTool, dblsidedTool,
                                                      isolationTool})
        w->hide();
    picker_ = new CanvasPicker(ui_.get(), this);
    connectSignals();
    setupProjectMenu();
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

void App::connectSignals() {
    for (QAction* a : {ui_->menufileopenproject, ui_->file_open_btn})
        connect(a, &QAction::triggered, this, [this] { onFileOpenProject(); });
    for (QAction* a : {ui_->menufilesaveproject, ui_->file_save_btn})
        connect(a, &QAction::triggered, this, [this] { onFileSaveProject(); });
    connect(ui_->menufilesaveprojectas, &QAction::triggered, this, [this] { onFileSaveProjectAs(); });
    connect(ui_->menufileopengerber, &QAction::triggered, this, [this] { onFileOpenGerber(); });
    connect(ui_->file_open_gerber_btn, &QAction::triggered, this, [this] { onFileOpenGerber(); });
    connect(ui_->menufileopenexcellon, &QAction::triggered, this, [this] { onFileOpenExcellon(); });
    connect(ui_->file_open_excellon_btn, &QAction::triggered, this, [this] { onFileOpenExcellon(); });
    for (QAction* a : {ui_->menuview_zoom_fit, ui_->zoom_fit_btn}) connect(a, &QAction::triggered, this, [this] { onZoomFit(); });
    // on_zoom_in / on_zoom_out: factor global_zoom_ratio (1,5) alrededor del centro de la vista.
    auto zoomBy = [this](double f) {
        PlotCanvas* c = ui_->plotCanvas();
        c->zoom(f, c->toWorld(c->viewRect().center()));
    };
    for (QAction* a : {ui_->menuview_zoom_in, ui_->zoom_in_btn}) connect(a, &QAction::triggered, this, [zoomBy] { zoomBy(1.5); });
    for (QAction* a : {ui_->menuview_zoom_out, ui_->zoom_out_btn})
        connect(a, &QAction::triggered, this, [zoomBy] { zoomBy(1 / 1.5); });
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
    for (QAction* a : {ui_->dblsidedtool_menuAction, ui_->dblsided_btn})
        connect(a, &QAction::triggered, this, [this] {
            runTool(dblsidedTool, DblSidedTool::info());
            dblsidedTool->setToolUi();
        });
    // app.pos: el último clic izquierdo en el canvas.
    connect(ui_->plotCanvas(), &PlotCanvas::mouseClicked, this, [this](QPointF w, Qt::MouseButton b) {
        if (b == Qt::LeftButton) lastClick_ = w;
    });
    // Opciones -> Voltear en el eje X / Y (on_flipx refleja con mirror('Y'), on_flipy con mirror('X')).
    connect(ui_->menuoptions_transform_flipx, &QAction::triggered, this, [this] { flip(Axis::Y); });
    connect(ui_->menuoptions_transform_flipy, &QAction::triggered, this, [this] { flip(Axis::X); });
    // ObjectCollection: elegir un objeto carga sus Propiedades; activarlo (doble clic o Enter) además abre la pestaña.
    connect(ui_->collection_view->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex& cur, const QModelIndex&) {
                FlatCAMObj* o = collection_->objectAt(cur);
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
    return true;
}

bool App::saveProject(const QString& filename) {
    const JsonValue& options = projectOptions_.isObject() ? projectOptions_ : ProjectIO::factoryDefaults();
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
    FlatCAMObj* obj = addObject(std::make_unique<ExcellonObject>(QFileInfo(filename).fileName(), std::move(exc)));
    registerRecent("excellon", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

FlatCAMObj* App::openByExtension(const QString& filename) {
    const QString ext = QFileInfo(filename).suffix().toLower();
    if (kGerberExt.contains(ext)) return openGerber(filename);
    if (kExcellonExt.contains(ext)) return openExcellon(filename);
    if (ext == "flatprj") {
        openProject(filename);
        return nullptr;
    }
    return nullptr;
}

void App::objectChanged(FlatCAMObj* obj) {
    ui_->plotCanvas()->setLayer(obj->id(), obj->plotData());
    if (obj == activeObject()) showProperties(obj);  // las tablas y los límites cambiaron
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
    return collection_->objectAt(ui_->collection_view->currentIndex());
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

FlatCAMObj* App::generateCncJob(GeometryObject* geo, const std::vector<int>& toolIds) {
    QString error;
    std::unique_ptr<CNCJobObject> job = geo->makeCncJob(toolIds, error);
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
        gui->onGenerateCnc = [this, geo, gui] { generateCncJob(geo, gui->toolsForCnc()); };
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
    }
}

bool App::handleKey(int key, Qt::KeyboardModifiers modifiers) {
    // MainGUI.keyPressEvent sin modificadores: V encuadra; X e Y reflejan; 1, 2 y 3 eligen la pestaña del panel izquierdo.
    if (modifiers != Qt::NoModifier) return false;
    switch (key) {
        case Qt::Key_V: onZoomFit(); return true;
        case Qt::Key_X: flip(Axis::Y); return true;  // on_flipx
        case Qt::Key_Y: flip(Axis::X); return true;  // on_flipy
        case Qt::Key_1: ui_->notebook->setCurrentWidget(ui_->project_tab); return true;
        case Qt::Key_2: ui_->notebook->setCurrentWidget(ui_->properties_tab); return true;
        case Qt::Key_3: ui_->notebook->setCurrentWidget(ui_->tool_tab); return true;
        case Qt::Key_Delete: deleteSelected(); return true;  // on_delete_keypress
        case Qt::Key_Escape:
            // on_key_press de las herramientas: Escape cancela la selección de zonas o polígonos.
            if (picker_->cancel()) {
                ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled by user request."));
                return true;
            }
            return false;
        default: return false;
    }
}

bool App::eventFilter(QObject* obj, QEvent* ev) {
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
