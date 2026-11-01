#include "gui/App.h"

#include <QApplication>
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
    ui_->setAcceptDrops(true);
    ui_->installEventFilter(this);
    defaultProperties_ = ui_->properties_scroll_area->widget();
    // Las herramientas (install_tools): sus paneles viven mientras viva la ventana.
    nccTool = new NccTool(ui_.get());
    paintTool = new PaintTool(ui_.get());
    drillingTool = new DrillingTool(ui_.get());
    cutoutTool = new CutOutTool(ui_.get());
    for (QWidget* w : std::initializer_list<QWidget*>{nccTool, paintTool, drillingTool, cutoutTool}) w->hide();
    connectSignals();
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
        connect(a, &QAction::triggered, this, [this] { runTool(nccTool, NccTool::info()); });
    for (QAction* a : {ui_->paint_tool_menuAction, ui_->paint_btn})
        connect(a, &QAction::triggered, this, [this] { runTool(paintTool, PaintTool::info()); });
    for (QAction* a : {ui_->drilling_tool_menuAction, ui_->drill_btn})
        connect(a, &QAction::triggered, this, [this] { runTool(drillingTool, DrillingTool::info()); });
    for (QAction* a : {ui_->cutout_tool_menuAction, ui_->cutout_btn})
        connect(a, &QAction::triggered, this, [this] { runTool(cutoutTool, CutOutTool::info()); });
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
    FlatCAMObj* obj = onObjectCreated(std::make_unique<GerberObject>(QFileInfo(filename).fileName(), std::move(gerber)));
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
    FlatCAMObj* obj = onObjectCreated(std::make_unique<ExcellonObject>(QFileInfo(filename).fileName(), std::move(exc)));
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

FlatCAMObj* App::openByExtension(const QString& filename) {
    const QString ext = QFileInfo(filename).suffix().toLower();
    if (kGerberExt.contains(ext)) return openGerber(filename);
    if (kExcellonExt.contains(ext)) return openExcellon(filename);
    return nullptr;
}

FlatCAMObj* App::onObjectCreated(std::unique_ptr<FlatCAMObj> o) {
    FlatCAMObj* obj = collection_->append(std::move(o));
    // ObjectCollection.append: se expande el grupo y, con global_project_autohide, se abre el panel izquierdo.
    ui_->collection_view->setExpanded(collection_->groupIndex(obj->kind()), true);
    ui_->splitter->setSizes({1, 1});
    ui_->menu_toggle_nb->setChecked(true);
    // AppObject.on_object_created: aviso, dibujo y encuadre (object_plotted -> on_zoom_fit).
    ui_->inform(QString("[selected] %1 %2: <span style=\"color:%3;\">%4</span>")
                    .arg(kindName(obj->kind()), tr_("created/selected"), kindColor(obj->kind()), obj->name()));
    ui_->plotCanvas()->setLayer(obj->id(), obj->plotData());
    onZoomFit();
    return obj;
}

void App::onZoomFit() { ui_->plotCanvas()->fitView(); }

void App::showProperties(FlatCAMObj* obj) {
    QScrollArea* area = ui_->properties_scroll_area;
    area->takeWidget();  // no se destruye: es del objeto o el panel vacío
    area->setWidget(obj ? obj->propertiesUi() : defaultProperties_);
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
            if (k->modifiers() == Qt::NoModifier) {
                switch (k->key()) {
                    case Qt::Key_V: onZoomFit(); return true;
                    case Qt::Key_1: ui_->notebook->setCurrentWidget(ui_->project_tab); return true;
                    case Qt::Key_2: ui_->notebook->setCurrentWidget(ui_->properties_tab); return true;
                    case Qt::Key_3: ui_->notebook->setCurrentWidget(ui_->tool_tab); return true;
                    default: break;
                }
            }
        }
    }
    return QObject::eventFilter(obj, ev);
}

}  // namespace flatcam::gui
