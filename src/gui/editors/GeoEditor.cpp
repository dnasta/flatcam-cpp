#include "gui/editors/GeoEditor.h"

#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QHeaderView>
#include <QPainterPath>
#include <QScrollArea>
#include <QSplitter>
#include <QTabWidget>
#include <QTreeWidgetItemIterator>

#include "cam/PyNumber.h"
#include "core/ArcMath.h"
#include "core/GeometryError.h"
#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/GeometryObject.h"
#include "gui/tools/DistanceTool.h"
#include "gui/widgets/DialogBoxRadio.h"

namespace flatcam::gui {

namespace {

constexpr int kShapesLayer = -7;   ///< `shapes`: las figuras del editor.
constexpr int kToolLayer = -8;     ///< `tool_shape`: la vista previa de la herramienta.
constexpr int kSelRectLayer = -9;  ///< El rectángulo de selección que se arrastra.

QString geoTypeName(const Shape& g) {
    switch (g.type()) {
        case ShapeType::LinearRing: return tr_("Ring");
        case ShapeType::LineString: return tr_("Line");
        case ShapeType::Polygon: return tr_("Polygon");
        case ShapeType::MultiLineString: return tr_("Multi-Line");
        case ShapeType::MultiPolygon: return tr_("Multi-Polygon");
        default: return "None";
    }
}

void addOutline(const Shape& s, PlotGroup& g) {
    QRectF ignored;
    FlatCAMObj::addShapeTo(s, g, ignored, false);
}

}  // namespace

// ------------------------------------------------------------------------------------------------ herramienta activa

/// El estado de la herramienta activa (`DrawTool` y sus subclases).
struct GeoEditor::Tool {
    QString name;                     ///< `select`, `circle`, `arc`, ...
    std::vector<Point2D> points;      ///< `points`.
    bool complete{false};             ///< `complete`.
    geodraw::ArcMode arcMode{geodraw::ArcMode::C12};
    ArcDirection direction{ArcDirection::Clockwise};
    std::optional<Point2D> origin;    ///< `origin` (mover, copiar, goma).
    ShapeList geometry;               ///< `geometry`.
    bool geometryIsList{false};       ///< `self.geometry = [...]`.
    Shape selectionShape;             ///< `selection_shape` de mover.
    bool isShapeTool() const { return name != "select"; }  ///< `isinstance(..., FCShapeTool)`.
};

// ------------------------------------------------------------------------------------------------ paneles

GeoPaintTool::GeoPaintTool(GeoEditor* ed, QWidget* parent) : GeoPaintToolUIBase(parent), ed_(ed) {
    setupUi();
    connect(paint_button, &QPushButton::clicked, this, [this] { onPaint(); });
}

void GeoPaintTool::setToolUi() {
    const double dia = defNum("tools_paint_tooldia");
    painttooldia_entry->setValue(dia != 0.0 ? dia : 0.0);
    paintoverlap_entry->setValue(defNum("tools_paint_overlap"));
    paintmargin_entry->setValue(defNum("tools_paint_offset"));
    // tools_paint_method vale 0 de fábrica: `if 0:` es falso y queda "Seed".
    const QVariant m = defVariant("tools_paint_method");
    if (m.toString().isEmpty() || m.toString() == "0") paintmethod_combo->setValue(tr_("Seed"));
    else paintmethod_combo->setValue(m.toString());
    pathconnect_cb->setChecked(defBool("tools_paint_connect"));
    paintcontour_cb->setChecked(defBool("tools_paint_contour"));
}

void GeoPaintTool::onPaint() {
    App* app = ed_->app();
    GeoEditorModel& m = ed_->model();
    if (m.selected.empty()) {
        app->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled.") + " " + tr_("No shape selected."));
        return;
    }
    const double tooldia = painttooldia_entry->value();
    const double overlap = paintoverlap_entry->value() / 100.0;
    const double margin = paintmargin_entry->value();
    const QString method = paintmethod_combo->currentText();
    if (overlap >= 100) {
        app->ui()->inform("[ERROR_NOTCL] " + tr_("Could not do Paint. Overlap value has to be less than 100%%."));
    } else {
        ed_->paintTooldia = tooldia;
        const auto meth = method == tr_("Seed")    ? GeoEditorModel::PaintMethod::Seed
                          : method == tr_("Lines") ? GeoEditorModel::PaintMethod::Lines
                                                   : GeoEditorModel::PaintMethod::Standard;
        const auto err = m.paint(tooldia, overlap, margin, pathconnect_cb->isChecked(), paintcontour_cb->isChecked(),
                                 meth, defInt("geometry_circle_steps", 64));
        if (err) {
            app->ui()->inform("[ERROR] " +
                              tr_("Could not do Paint. Try a different combination of parameters. Or a different method of Paint") +
                              "\n" + QString::fromStdString(*err));
        } else {
            app->ui()->inform("[success] " + tr_("Done."));
            ed_->buildUi();
            ed_->replot();
        }
    }
    ed_->selectTool("select");
    app->ui()->notebook->setTabText(2, tr_("Tools"));
    app->ui()->notebook->setCurrentWidget(app->ui()->project_tab);
    app->ui()->splitter->setSizes({0, 1});
}

GeoTransformTool::GeoTransformTool(GeoEditor* ed, QWidget* parent) : GeoTransformToolUIBase(parent), ed_(ed) {
    setupUi();
    connect(ref_combo, &QComboBox::currentIndexChanged, this, [this](int i) {
        const bool point = i == 2;
        point_label->setVisible(point);
        point_entry->setVisible(point);
        point_button->setVisible(point);
    });
    connect(point_button, &QPushButton::clicked, this, [this] { point_entry->setValue(QApplication::clipboard()->text()); });
    connect(rotate_button, &QPushButton::clicked, this, [this] { onRotate(); });
    connect(skewx_button, &QPushButton::clicked, this, [this] { onSkewX(); });
    connect(skewy_button, &QPushButton::clicked, this, [this] { onSkewY(); });
    connect(scalex_button, &QPushButton::clicked, this, [this] { onScaleX(); });
    connect(scaley_button, &QPushButton::clicked, this, [this] { onScaleY(); });
    connect(offx_button, &QPushButton::clicked, this, [this] { onOffX(); });
    connect(offy_button, &QPushButton::clicked, this, [this] { onOffY(); });
    connect(flipx_button, &QPushButton::clicked, this, [this] { onFlipX(); });
    connect(flipy_button, &QPushButton::clicked, this, [this] { onFlipY(); });
    connect(buffer_button, &QPushButton::clicked, this, [this] { onBufferByDistance(); });
    connect(buffer_factor_button, &QPushButton::clicked, this, [this] { onBufferByFactor(); });
}

void GeoTransformTool::setToolUi() {
    QString ref = defStr("tools_transform_reference");
    if (ref == tr_("Object")) ref = tr_("Selection");
    ref_combo->setValue(ref);
    point_entry->setValue(defStr("tools_transform_ref_point"));
    rotate_entry->setValue(defNum("tools_transform_rotate"));
    skewx_entry->setValue(defNum("tools_transform_skew_x"));
    skewy_entry->setValue(defNum("tools_transform_skew_y"));
    skew_link_cb->setChecked(defBool("tools_transform_skew_link"));
    scalex_entry->setValue(defNum("tools_transform_scale_x"));
    scaley_entry->setValue(defNum("tools_transform_scale_y"));
    scale_link_cb->setChecked(defBool("tools_transform_scale_link"));
    offx_entry->setValue(defNum("tools_transform_offset_x"));
    offy_entry->setValue(defNum("tools_transform_offset_y"));
    buffer_entry->setValue(defNum("tools_transform_buffer_dis"));
    buffer_factor_entry->setValue(defNum("tools_transform_buffer_factor"));
    buffer_rounded_cb->setChecked(defBool("tools_transform_buffer_corner"));
    point_label->hide();
    point_entry->hide();
    point_button->hide();
}

std::optional<Point2D> GeoTransformTool::reference(std::optional<int> ref) {
    // `if ref_index:` es falso con 0: entonces vale el combo.
    const int r = ref && *ref ? *ref : ref_combo->currentIndex();
    MainGUI* ui = ed_->app()->ui();
    const auto& sel = ed_->model().selected;
    if (r == 0) return Point2D{0, 0};
    if (r == 2) {
        const auto v = point_entry->evaluated();
        if (!v || v->size() != 2) {
            ui->inform("[WARNING_NOTCL] " + tr_("Incorrect format for Point value. Needs format X,Y"));
            return std::nullopt;
        }
        return Point2D{(*v)[0], (*v)[1]};
    }
    if (sel.empty()) {
        ui->inform("[ERROR_NOTCL] " + tr_("No shape selected."));
        return std::nullopt;
    }
    const BoundingBox b = ed_->model().selectionBounds();
    switch (r) {
        case 1: return Point2D{(b.xmax + b.xmin) * 0.5, (b.ymax + b.ymin) * 0.5};
        case 3: return Point2D{b.xmin, b.ymin};
        case 4: return Point2D{b.xmax, b.ymin};
        case 5: return Point2D{b.xmax, b.ymax};
        default: return Point2D{b.xmin, b.ymax};
    }
}

void GeoTransformTool::done(const QString& msg) {
    ed_->replot();
    ed_->app()->ui()->inform(msg);
}

void GeoTransformTool::onRotate(std::optional<double> val, std::optional<int> ref) {
    const double v = val ? *val : rotate_entry->value();
    MainGUI* ui = ed_->app()->ui();
    if (v == 0) {
        ui->inform("[WARNING_NOTCL] " + tr_("Rotate transformation can not be done for a value of 0."));
        return;
    }
    const auto p = reference(ref);
    if (!p) return;
    if (ed_->model().selected.empty()) return ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().rotate(-v, *p);
    done("[success] " + tr_("Done."));
}

void GeoTransformTool::onFlipX() {
    const auto p = reference();
    if (!p) return;
    if (ed_->model().selected.empty()) return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().mirror('Y', *p);
    done("[success] " + tr_("Flip on X axis done"));
}

void GeoTransformTool::onFlipY() {
    const auto p = reference();
    if (!p) return;
    if (ed_->model().selected.empty()) return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().mirror('X', *p);
    done("[success] " + tr_("Flip on Y axis done") + "...");
}

void GeoTransformTool::onSkewX(std::optional<double> val, std::optional<int> ref) {
    const double x = val ? *val : skewx_entry->value();
    if (x == 0) return;
    const double y = skew_link_cb->isChecked() ? x : 0.0;
    const auto p = reference(ref);
    if (!p) return;
    if (ed_->model().selected.empty()) return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().skew(x, y, *p);
    done("[success] " + tr_("Skew on the X axis done") + "...");
}

void GeoTransformTool::onSkewY() {
    const double y = skewy_entry->value();
    if (y == 0) return;
    const auto p = reference();
    if (!p) return;
    if (ed_->model().selected.empty()) return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().skew(0, y, *p);
    done("[success] " + tr_("Skew on the Y axis done") + "...");
}

void GeoTransformTool::onScaleX() {
    const double x = scalex_entry->value();
    MainGUI* ui = ed_->app()->ui();
    if (x == 0 || x == 1) {
        ui->inform("[WARNING_NOTCL] " + tr_("Scale transformation can not be done for a factor of 0 or 1."));
        return;
    }
    const double y = scale_link_cb->isChecked() ? x : 1.0;
    const auto p = reference();
    if (!p) return;
    if (ed_->model().selected.empty()) return ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().scale(x, y, *p);
    done("[success] " + tr_("Scale on the X axis done") + "...");
}

void GeoTransformTool::onScaleY() {
    const double y = scaley_entry->value();
    MainGUI* ui = ed_->app()->ui();
    if (y == 0 || y == 1) {
        ui->inform("[WARNING_NOTCL] " + tr_("Scale transformation can not be done for a factor of 0 or 1."));
        return;
    }
    const auto p = reference();
    if (!p) return;
    if (ed_->model().selected.empty()) return ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().scale(1, y, *p);
    done("[success] " + tr_("Scale on the Y axis done") + "...");
}

void GeoTransformTool::onOffX(std::optional<double> val) {
    const double v = val ? *val : offx_entry->value();
    MainGUI* ui = ed_->app()->ui();
    if (v == 0) return ui->inform("[WARNING_NOTCL] " + tr_("Offset transformation can not be done for a value of 0."));
    if (ed_->model().selected.empty()) return ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().offset(v, 0);
    done("[success] " + tr_("Offset on the X axis done") + "...");
}

void GeoTransformTool::onOffY() {
    const double v = offy_entry->value();
    MainGUI* ui = ed_->app()->ui();
    if (v == 0) return ui->inform("[WARNING_NOTCL] " + tr_("Offset transformation can not be done for a value of 0."));
    if (ed_->model().selected.empty()) return ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().offset(0, v);
    done("[success] " + tr_("Offset on the Y axis done") + "...");
}

void GeoTransformTool::onBufferByDistance() {
    if (ed_->model().selected.empty())
        return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().buffer(buffer_entry->value(), buffer_rounded_cb->isChecked() ? 1 : 2, false);
    done("[success] " + tr_("Buffer done") + "...");
}

void GeoTransformTool::onBufferByFactor() {
    if (ed_->model().selected.empty())
        return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
    ed_->model().buffer(1 + buffer_factor_entry->value() / 100.0, buffer_rounded_cb->isChecked() ? 1 : 2, true);
    done("[success] " + tr_("Buffer done") + "...");
}

void GeoTransformTool::onRotateKey() {
    FCInputDoubleSpinner box(ed_->app()->ui(), tr_("Rotate ..."), tr_("Enter an Angle Value (degrees)") + ":",
                             -359.9999, 360.0, 1, 4, defNum("tools_transform_rotate"));
    box.setWindowIcon(QIcon(":/images/rotate.png"));
    if (box.exec() != QDialog::Accepted) return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("Rotate cancelled"));
    onRotate(box.wdg->value(), 1);
    ed_->app()->ui()->inform("[success] " + tr_("Rotate done") + "...");
}

void GeoTransformTool::onOffXKey() {
    FCInputDoubleSpinner box(ed_->app()->ui(), tr_("Offset on X axis ..."),
                             QString("%1: (%2)").arg(tr_("Enter a distance Value"), defStr("units").toLower()), -10000.0,
                             10000.0, 1, 4, defNum("tools_transform_offset_x"));
    box.setWindowIcon(QIcon(":/images/offsetx32.png"));
    if (box.exec() != QDialog::Accepted) return ed_->app()->ui()->inform("[WARNING_NOTCL] " + tr_("Offset X cancelled"));
    onOffX(box.wdg->value());
    ed_->app()->ui()->inform("[success] " + tr_("Offset on the X axis done"));
}

void GeoTransformTool::onOffYKey() {
    FCInputDoubleSpinner box(ed_->app()->ui(), tr_("Offset on Y axis ..."),
                             QString("%1: (%2)").arg(tr_("Enter a distance Value"), defStr("units").toLower()), -10000.0,
                             10000.0, 1, 4, defNum("tools_transform_offset_y"));
    box.setWindowIcon(QIcon(":/images/offsety32.png"));
    if (box.exec() != QDialog::Accepted)
        return ed_->app()->ui()->inform("[success] " + tr_("Offset on the Y axis canceled") + "...");
    onOffX(box.wdg->value());  // el original llama a on_offx
    ed_->app()->ui()->inform("[success] " + tr_("Offset on Y axis done") + "...");
}

void GeoTransformTool::onSkewXKey() {
    FCInputDoubleSpinner box(ed_->app()->ui(), tr_("Skew on X axis ..."), tr_("Enter an Angle Value (degrees)") + ":",
                             -359.9999, 360.0, 1, 4, defNum("tools_transform_skew_x"));
    box.setWindowIcon(QIcon(":/images/skewX.png"));
    if (box.exec() != QDialog::Accepted)
        return ed_->app()->ui()->inform("[success] " + tr_("Skew on X axis canceled") + "...");
    onSkewX(box.wdg->value(), 3);
    ed_->app()->ui()->inform("[success] " + tr_("Skew on X axis done") + "...");
}

void GeoTransformTool::onSkewYKey() {
    FCInputDoubleSpinner box(ed_->app()->ui(), tr_("Skew on Y axis ..."), tr_("Enter an Angle Value (degrees)") + ":",
                             -359.9999, 360.0, 1, 4, defNum("tools_transform_skew_y"));
    box.setWindowIcon(QIcon(":/images/skewY.png"));
    if (box.exec() != QDialog::Accepted)
        return ed_->app()->ui()->inform("[success] " + tr_("Skew on Y axis canceled") + "...");
    onSkewX(box.wdg->value(), 3);  // el original llama a on_skewx
    ed_->app()->ui()->inform("[success] " + tr_("Skew on Y axis done") + "...");
}

GeoTextTool::GeoTextTool(GeoEditor* ed, QWidget* parent) : GeoTextToolUIBase(parent), ed_(ed) {
    setupUi();
    connect(apply_button, &QPushButton::clicked, this, [this] { onApply(); });
    connect(font_bold_tb, &QToolButton::clicked, this, [this] { fontBold = font_bold_tb->isChecked(); });
    connect(font_italic_tb, &QToolButton::clicked, this, [this] { fontItalic = font_italic_tb->isChecked(); });
}

void GeoTextTool::onApply() {
    // font_to_geometry: cada contorno de cada letra es un polígono aparte (también los agujeros), escalado a
    // 0,0080187969924812 mm por unidad 26.6 de FreeType con el tamaño en puntos (en pulgadas, 0,00031570066).
    // El port toma los contornos de Qt (curvas aplanadas) en vez de los puntos de control de FreeType.
    // El original elige 'bold' antes que 'bi': con negrita y cursiva sale negrita.
    QFont f = font_type_cb->currentFont();
    f.setBold(fontBold);
    f.setItalic(!fontBold && fontItalic);
    f.setPixelSize(1000);
    const double size = font_size_cb->currentText().toDouble();
    const double unit = defStr("units").toUpper() == "MM" ? 0.0080187969924812 : 0.00031570066;
    const double k = size * 64.0 / 1000.0 * unit;
    QPainterPath path;
    path.addText(0, 0, f, text_input_entry->toPlainText());
    textPath.clear();
    for (const QPolygonF& poly : path.toSubpathPolygons()) {
        if (poly.size() < 3) continue;
        std::vector<Point2D> pts;
        for (const QPointF& p : poly) pts.push_back({p.x() * k, -p.y() * k});
        if (!(pts.front() == pts.back())) pts.push_back(pts.front());
        if (pts.size() < 4) continue;
        try {
            textPath.push_back(Shape::polygon(pts));
        } catch (const GeometryError&) {
        }
    }
}

// ------------------------------------------------------------------------------------------------ editor

GeoEditor::GeoEditor(App* app) : QObject(app), app_(app) {
    MainGUI* ui = app_->ui();
    panel_ = new GeoEditorUIBase();
    panel_->setupUi();
    panel_->tw->setHeaderLabels({"ID", tr_("Type"), tr_("Name")});
    panel_->tw->setIndentation(0);
    panel_->tw->header()->setStretchLastSection(true);
    panel_->tw->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    paintTool_ = new GeoPaintTool(this);
    transformTool_ = new GeoTransformTool(this);
    textTool_ = new GeoTextTool(this);
    connect(panel_->exit_editor_button, &QPushButton::clicked, this, [this] { app_->editorToObject(); });
    connect(panel_->tw, &QTreeWidget::itemSelectionChanged, this, [this] {
        // on_tree_selection_change
        model_.selected.clear();
        const auto objs = model_.storage.objects();
        for (QTreeWidgetItem* it : panel_->tw->selectedItems()) {
            const auto i = it->data(0, Qt::UserRole).toInt();
            if (i >= 0 && i < static_cast<int>(objs.size())) model_.selected.push_back(objs[static_cast<std::size_t>(i)]);
        }
        replot();
    });
    buttons_ = {{"select", ui->geo_select_btn},        {"arc", ui->geo_add_arc_btn},
                {"circle", ui->geo_add_circle_btn},    {"path", ui->geo_add_path_btn},
                {"rectangle", ui->geo_add_rectangle_btn}, {"polygon", ui->geo_add_polygon_btn},
                {"text", ui->geo_add_text_btn},        {"buffer", ui->geo_add_buffer_btn},
                {"paint", ui->geo_add_paint_btn},      {"eraser", ui->geo_eraser_btn},
                {"move", ui->geo_move_btn},            {"transform", ui->geo_transform_btn},
                {"copy", ui->geo_copy_btn},            {"explode", ui->geo_explode_btn}};
    for (auto& [name, a] : buttons_) {
        a->setCheckable(true);
        const QString n = name;
        connect(a, &QAction::triggered, this, [this, n] { if (active_) onToolSelect(n); });
    }
    // El menú del editor y la barra.
    auto onMenu = [this](QAction* a, auto fn) { connect(a, &QAction::triggered, this, [this, fn] { if (active_) fn(); }); };
    onMenu(ui->geo_add_circle_menuitem, [this] { selectTool("circle"); });
    onMenu(ui->geo_add_arc_menuitem, [this] { selectTool("arc"); });
    onMenu(ui->geo_add_rectangle_menuitem, [this] { selectTool("rectangle"); });
    onMenu(ui->geo_add_polygon_menuitem, [this] { selectTool("polygon"); });
    onMenu(ui->geo_add_path_menuitem, [this] { selectTool("path"); });
    onMenu(ui->geo_add_text_menuitem, [this] { selectTool("text"); });
    onMenu(ui->geo_paint_menuitem, [this] {  // on_paint_tool: un PaintOptionsTool nuevo
        paintTool_->setToolUi();
        app_->runTool(paintTool_, {tr_("Paint Tool"), tr_("Paint Tool")}, false);
    });
    // on_buffer_tool: BufferSelectionTool falla al construirse ("%S:"): no hace nada.
    onMenu(ui->geo_buffer_menuitem, [] {});
    onMenu(ui->geo_transform_menuitem, [this] {
        transformTool_->setToolUi();
        app_->runTool(transformTool_, {tr_("Transform Tool"), tr_("Transform Tool")});
    });
    onMenu(ui->geo_delete_menuitem, [this] { deleteSelected(); });
    onMenu(ui->geo_union_menuitem, [this] { unite(); });
    onMenu(ui->geo_intersection_menuitem, [this] { intersection(); });
    onMenu(ui->geo_subtract_menuitem, [this] { subtract(); });
    onMenu(ui->geo_cutpath_menuitem, [this] { cutpath(); });
    onMenu(ui->geo_copy_menuitem, [this] { selectTool("copy"); });
    onMenu(ui->geo_union_btn, [this] { unite(); });
    onMenu(ui->geo_intersection_btn, [this] { intersection(); });
    onMenu(ui->geo_subtract_btn, [this] { subtract(); });
    onMenu(ui->geo_cutpath_btn, [this] { cutpath(); });
    onMenu(ui->geo_delete_btn, [this] { deleteSelected(); });
    onMenu(ui->geo_move_menuitem, [this] {
        app_->ui()->geo_move_btn->setChecked(true);
        onToolSelect("move");
    });
    onMenu(ui->geo_cornersnap_menuitem, [this] { app_->ui()->corner_snap_btn->trigger(); });
    ui->corner_snap_btn->setCheckable(true);
    connect(ui->corner_snap_btn, &QAction::triggered, this, [this](bool on) { cornerSnap_ = on; });
}

GeoEditor::~GeoEditor() {
    delete panel_;
    delete paintTool_;
    delete transformTool_;
    delete textTool_;
}

QString GeoEditor::activeToolName() const { return tool_ ? tool_->name : QString(); }

Point2D GeoEditor::snap(const Point2D& p) const {
    // AppGeoEditor.snap: la esquina más cercana si está a menos de global_snap_max, y la grilla si queda más cerca.
    Point2D out = p;
    double snapDistance = std::numeric_limits<double>::infinity();
    MainGUI* ui = app_->ui();
    if (cornerSnap_) {
        if (const auto n = model_.storage.nearest(p)) {
            const double d = distance(p, n->first);
            bool ok = false;
            const double maxd = QString(ui->snap_max_dist_entry->text()).replace(',', '.').toDouble(&ok);
            if (d <= (ok ? maxd : 0.05)) {
                snapDistance = d;
                out = n->first;
            }
        }
    }
    if (ui->grid_snap_btn->isChecked()) {
        const QPointF g = ui->snapToGrid({p.x, p.y});
        if (distance(p, {g.x(), g.y()}) < snapDistance) out = {g.x(), g.y()};
    }
    return out;
}

void GeoEditor::edit(GeometryObject* obj, int tool) {
    MainGUI* ui = app_->ui();
    deactivate();
    // activate
    active_ = true;
    toolbarOldState_ = ui->geo_edit_toolbar->isVisible();
    ui->menueditedit->setDisabled(true);
    ui->menueditok->setDisabled(false);
    model_ = GeoEditorModel();
    ui->corner_snap_btn->setVisible(true);
    ui->snap_magnet->setVisible(true);
    ui->geo_editor_menu->setDisabled(false);
    ui->geo_editor_menu->menuAction()->setVisible(true);
    ui->update_obj_btn->setEnabled(true);
    ui->geo_edit_toolbar->setDisabled(false);
    ui->geo_edit_toolbar->setVisible(true);
    ui->status_toolbar->setDisabled(false);
    panel_->geo_frame->show();
    ui->setCanvasHandlersEnabled(false);
    ui->plotCanvas()->setSnapper([this](QPointF p) {
        const Point2D s = app_->ui()->grid_snap_btn->isChecked() ? snap({p.x(), p.y()}) : Point2D{p.x(), p.y()};
        return QPointF(s.x, s.y);
    });
    // set_ui
    ui->properties_scroll_area->takeWidget();
    ui->properties_scroll_area->setWidget(panel_);
    ui->notebook->setCurrentWidget(ui->properties_tab);
    tool_id_ = tool;
    ui->plotCanvas()->setLayerVisible(obj->id(), false);  // fcgeometry.visible = False
    model_.storage.tolerance = 0.005 / 20.0 * 10.0;        // drawing_tolerance * 10
    selectTool("select");
    // geometry_spindledir CW con fresado por ascenso: orientación 1 (antihoraria).
    const bool cw = defStr("geometry_spindledir") == "CW";
    const bool climb = defStr("geometry_editor_milling_type") == "cl";
    const int orient = (cw == climb) ? 1 : -1;
    const auto it = obj->tools().find(tool);
    if (it != obj->tools().end()) {
        model_.load(it->second.solidGeometry, orient);
        ui->inform(QString("[WARNING_NOTCL] %1: %2 %3: %4")
                       .arg(tr_("Editing MultiGeo Geometry, tool"))
                       .arg(tool)
                       .arg(tr_("with diameter"))
                       .arg(QString::fromStdString(pyFloatRepr(it->second.tooldia))));
    } else {
        model_.load(obj->solidGeometry(), orient);
    }
    buildUi();
    replot();
    if (!ui->grid_snap_btn->isChecked()) ui->grid_snap_btn->trigger();
}

void GeoEditor::save(GeometryObject* obj) {
    const ShapeList r = model_.result();
    auto it = obj->tools().find(tool_id_);
    if (it != obj->tools().end()) it->second.solidGeometry = r;
    obj->solidGeometry() = r;
    deactivate();
}

void GeoEditor::deactivate() {
    MainGUI* ui = app_->ui();
    const bool wasActive = active_;
    ui->menueditedit->setDisabled(false);
    ui->menueditok->setDisabled(true);
    if (wasActive) {
        ui->setCanvasHandlersEnabled(true);
        ui->plotCanvas()->setSnapper([ui](QPointF p) { return ui->snapToGrid(p); });
    }
    tool_.reset();
    model_.selected.clear();
    ui->plotCanvas()->removeLayer(kShapesLayer);
    ui->plotCanvas()->removeLayer(kToolLayer);
    ui->plotCanvas()->removeLayer(kSelRectLayer);
    ui->geo_edit_toolbar->setDisabled(true);
    ui->corner_snap_btn->setVisible(false);
    ui->snap_magnet->setVisible(false);
    ui->geo_edit_toolbar->setVisible(toolbarOldState_);
    ui->geo_editor_menu->setDisabled(true);
    ui->geo_editor_menu->menuAction()->setVisible(false);
    ui->update_obj_btn->setEnabled(false);
    panel_->tw->clear();
    panel_->geo_frame->hide();
    for (auto& [n, a] : buttons_) a->setChecked(false);
    active_ = false;
}

void GeoEditor::selectTool(const QString& name) {
    auto it = buttons_.find(name);
    if (it == buttons_.end()) return;
    it->second->setChecked(true);
    onToolSelect(name);
}

void GeoEditor::onToolSelect(const QString& name) {
    if (!buttons_.count(name)) return;
    if (buttons_[name]->isChecked()) {
        for (auto& [n, a] : buttons_)
            if (n != name) a->setChecked(false);
        newTool(name);
    } else {
        for (auto& [n, a] : buttons_) a->setChecked(false);
        selectTool("select");
        newTool("select");
    }
}

void GeoEditor::newTool(const QString& name) {
    MainGUI* ui = app_->ui();
    auto t = std::make_unique<Tool>();
    t->name = name;
    if (name == "circle" || name == "arc") {
        ui->inform(tr_("Click on Center point ..."));
    } else if (name == "rectangle" || name == "polygon" || name == "path") {
        ui->inform(tr_("Click on 1st corner ..."));
    } else if (name == "text") {
        ui->inform(tr_("Click on 1st point ..."));
        textTool_->textPath.clear();
        textTool_->text_tool_frame->show();
        app_->runTool(textTool_, {tr_("Text Input Tool"), tr_("Text Tool")}, false);
    } else if (name == "buffer") {
        // FCBuffer: avisa y falla al construir BufferSelectionTool ('%S:' no es un formato válido).
        ui->inform(tr_("Create buffer geometry ..."));
        return;
    } else if (name == "paint") {
        ui->inform(tr_("Create Paint geometry ..."));
        paintTool_->setToolUi();
        app_->runTool(paintTool_, {tr_("Paint Tool"), tr_("Paint Tool")}, false);
    } else if (name == "eraser") {
        ui->inform(model_.selected.empty() ? tr_("Select a shape to act as deletion area ...")
                                           : tr_("Click to pick-up the erase shape..."));
        ui->notebook->setCurrentWidget(ui->properties_tab);
    } else if (name == "move" || name == "copy") {
        if (model_.selected.empty()) ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
        else ui->inform(tr_("Click on reference location ..."));
    } else if (name == "transform") {
        ui->inform(tr_("Shape transformations ..."));
        transformTool_->setToolUi();
        app_->runTool(transformTool_, {tr_("Transform Tool"), tr_("Transform Tool")});
    } else if (name == "explode") {
        tool_ = std::move(t);
        if (model_.selected.empty()) {
            ui->inform("[WARNING_NOTCL] " + tr_("No shape selected."));
            return;
        }
        if (auto lines = model_.explode()) {
            tool_->geometry = *lines;
            tool_->geometryIsList = true;
            onShapeComplete();
            ui->inform("[success] " + tr_("Done.") + "...");
        }
        return;
    }
    tool_ = std::move(t);
}

void GeoEditor::cleanUpActive() {
    if (!tool_) return;
    if (tool_->name != "select") {
        model_.selected.clear();
        replot();
    }
}

void GeoEditor::onShapeComplete() {
    if (!tool_) return;
    model_.complete(tool_->geometry, tool_->geometryIsList, defStr("geometry_editor_milling_type") == "cl");
    buildUi();
    deleteUtility();
    replot();
}

void GeoEditor::onPress(const QPointF& world, Qt::MouseButton button) {
    if (!active_) return;
    MainGUI* ui = app_->ui();
    pos_ = {world.x(), world.y()};
    if (button != Qt::LeftButton) return;
    ui->rel_position_label->setText("<b>Dx</b>: 0.0000&nbsp;&nbsp;  <b>Dy</b>: 0.0000&nbsp;&nbsp;&nbsp;&nbsp;");
    if (QApplication::keyboardModifiers() == Qt::ShiftModifier) {
        QApplication::clipboard()->setText(QString("(%1, %2)").arg(pos_.x, 0, 'f', 4).arg(pos_.y, 0, 'f', 4));
        return;
    }
    if (!tool_) return;
    const Point2D p = snap(pos_);
    Tool& t = *tool_;
    const QString n = t.name;
    const int steps = defInt("geometry_circle_steps", 64);
    if (n == "circle" || n == "rectangle") {
        t.points.push_back(p);
        if (t.points.size() == 1) {
            ui->inform(n == "circle" ? tr_("Click on Perimeter point to complete ...")
                                     : tr_("Click on opposite corner to complete ..."));
        } else {
            t.geometry = {n == "circle" ? geodraw::circle(t.points[0], t.points[1], steps)
                                        : geodraw::rectangle(t.points[0], t.points[1])};
            t.complete = true;
            ui->inform("[success] " + tr_("Done."));
        }
    } else if (n == "arc") {
        t.points.push_back(p);
        if (t.points.size() == 1) {
            ui->inform(t.arcMode == geodraw::ArcMode::C12   ? tr_("Click on Start point ...")
                       : t.arcMode == geodraw::ArcMode::P132 ? tr_("Click on Point3 ...")
                                                             : tr_("Click on Stop point ..."));
        } else if (t.points.size() == 2) {
            ui->inform(t.arcMode == geodraw::ArcMode::C12   ? tr_("Click on Stop point to complete ...")
                       : t.arcMode == geodraw::ArcMode::P132 ? tr_("Click on Point2 to complete ...")
                                                             : tr_("Click on Center point to complete ..."));
        } else if (t.points.size() == 3) {
            const auto g = geodraw::arc(t.arcMode, t.points[0], t.points[1], t.points[2], t.direction, steps);
            if (!g) return;  // tres puntos alineados: TypeError
            t.geometry = {*g};
            t.complete = true;
            ui->inform("[success] " + tr_("Done."));
        }
    } else if (n == "polygon" || n == "path") {
        inAction_ = true;
        t.points.push_back(p);
        ui->inform(tr_("Click on next Point or click right mouse button to complete ..."));
    } else if (n == "move" || n == "copy") {
        if (model_.selected.empty()) {
            model_.clickSelect(p, QApplication::keyboardModifiers() ==
                                      (defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier));
            replot();
            ui->inform(tr_("Click on reference location ..."));
            return;
        }
        if (!t.origin) {
            ui->inform(tr_("Click on destination point ..."));
            t.origin = p;
            return;
        }
        const double dx = p.x - t.origin->x, dy = p.y - t.origin->y;
        t.geometry.clear();
        for (const auto& s : model_.selected) t.geometry.push_back(s->geo.translate(dx, dy));
        t.geometryIsList = true;
        if (n == "move") model_.deleteSelected();
        t.complete = true;
        ui->inform("[success] " + tr_("Done."));
    } else if (n == "eraser") {
        if (model_.selected.empty()) {
            // Recorre el almacén y agrega el más cercano una vez por cada figura.
            const auto count = model_.storage.size();
            if (const auto nn = model_.storage.nearest(p))
                for (std::size_t i = 0; i < count; ++i) model_.selected.push_back(nn->second);
        }
        if (model_.selected.empty()) return;
        ui->inform(tr_("Click to pick-up the erase shape..."));
        if (!t.origin) {
            t.origin = p;
            ui->inform(tr_("Click to erase ..."));
            return;
        }
        try {
            model_.erase(*t.origin, p);
        } catch (const GeometryError&) {
            return;
        }
        deleteUtility();
        replot();
        ui->inform("[success] " + tr_("Done."));
        return;
    } else if (n == "text") {
        if (textTool_->textPath.empty()) {
            ui->inform("[WARNING_NOTCL] " + tr_("No text to add."));
            return;
        }
        t.geometry = {Shape::collection(textTool_->textPath).translate(p.x, p.y)};
        textTool_->textPath.clear();
        textTool_->text_tool_frame->hide();
        ui->notebook->setCurrentWidget(ui->properties_tab);
        ui->notebook->setTabText(2, tr_("Tool"));
        t.complete = true;
        ui->inform("[success]" + tr_("Done."));
    }
    if (t.isShapeTool() && t.complete) {
        onShapeComplete();
        if (n == "text") selectTool("select");
        else selectTool(n);
    }
    if (tool_ && tool_->name == "select") replot();
}

void GeoEditor::onMove(const QPointF& world, bool leftDragging) {
    if (!active_) return;
    MainGUI* ui = app_->ui();
    const Point2D p{world.x(), world.y()};
    mouse_ = p;
    if (!tool_) return;
    const double dx = p.x - pos_.x, dy = p.y - pos_.y;
    ui->position_label->setText(QString("&nbsp;<b>X</b>: %1&nbsp;&nbsp;   <b>Y</b>: %2&nbsp;")
                                    .arg(p.x, 0, 'f', 4)
                                    .arg(p.y, 0, 'f', 4));
    ui->rel_position_label->setText(QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;")
                                        .arg(dx, 0, 'f', 4)
                                        .arg(dy, 0, 'f', 4));
    if (!(leftDragging && tool_->name == "eraser")) updateUtility(p);
    if (leftDragging) {
        PlotLayerData d;
        const Shape r = Shape::polygon({pos_, {p.x, pos_.y}, p, {pos_.x, p.y}});
        PlotGroup g;
        QRectF ignored;
        FlatCAMObj::addShapeTo(r, g, ignored, true);
        const bool left = dx < 0;
        g.lineColor = colorFromFlatCam(defStr(left ? "global_alt_sel_line" : "global_sel_line"));
        g.fillColor = colorFromFlatCam(defStr(left ? "global_alt_sel_fill" : "global_sel_fill").chopped(2) + "4c");
        d.groups.push_back(g);
        ui->plotCanvas()->setLayer(kSelRectLayer, std::move(d));
        selectionType_ = !left;
    } else {
        selectionType_.reset();
    }
}

void GeoEditor::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!active_ || !tool_) return;
    MainGUI* ui = app_->ui();
    (void)world;
    const Qt::KeyboardModifier mselect = defStr("global_mselect_key") == "Control" ? Qt::ControlModifier : Qt::ShiftModifier;
    if (button == Qt::LeftButton) {
        if (selectionType_) {
            ui->plotCanvas()->removeLayer(kSelRectLayer);
            const Point2D end = ui->grid_snap_btn->isChecked() ? snap({world.x(), world.y()}) : Point2D{world.x(), world.y()};
            model_.areaSelect(pos_, end, *selectionType_, QApplication::keyboardModifiers() == mselect);
            selectionType_.reset();
            syncTreeSelection();
            replot();
        } else if (tool_->name == "select") {
            // FCSelect.click_release con la posición del clic.
            model_.clickSelect(pos_, QApplication::keyboardModifiers() == mselect);
            syncTreeSelection();
            replot();
        }
        return;
    }
    if (button != Qt::RightButton || dragged) return;
    if (!inAction_) {
        if (!tool_->complete && tool_->name != "select") {
            tool_->complete = true;
            inAction_ = false;
            deleteUtility();
            ui->inform("[success] " + tr_("Done."));
            selectTool("select");
        } else {
            app_->popupCanvasMenu();
        }
        return;
    }
    // Polígono o trayecto en curso: el punto del clic derecho y terminar.
    Tool& t = *tool_;
    t.points.push_back(snap(mouse_));
    t.geometry = {t.name == "path" ? Shape::lineString(t.points) : Shape::polygon(t.points)};
    inAction_ = false;
    t.complete = true;
    onShapeComplete();
    ui->inform("[success] " + tr_("Done."));
    selectTool(t.name);
}

void GeoEditor::updateUtility(const Point2D& data) {
    if (!tool_) return;
    const Tool& t = *tool_;
    const int steps = defInt("geometry_circle_steps", 64);
    ShapeList geo;
    const QString n = t.name;
    if (n == "circle" && t.points.size() == 1) {
        geo = {Shape::point(t.points[0]).buffer(distance(t.points[0], data), steps / 4)};
    } else if (n == "rectangle" && t.points.size() == 1) {
        const Point2D a = t.points[0];
        geo = {Shape::linearRing({a, {data.x, a.y}, data, {a.x, data.y}, a})};
    } else if (n == "arc") {
        if (t.points.size() == 1) {
            geo = {Shape::lineString({t.points[0], data})};
        } else if (t.points.size() == 2) {
            const auto g = geodraw::arc(t.arcMode, t.points[0], t.points[1], data, t.direction, steps);
            if (!g) return;
            geo = {*g};
            if (const auto c = geodraw::arcCenter(t.arcMode, t.points[0], t.points[1], data)) geo.push_back(Shape::point(*c));
            if (t.arcMode == geodraw::ArcMode::P132) {
                geo.push_back(Shape::point(t.points[0]));
                geo.push_back(Shape::point(t.points[1]));
            }
        }
    } else if (n == "polygon" && !t.points.empty()) {
        std::vector<Point2D> tmp = t.points;
        tmp.push_back(data);
        if (tmp.size() == 2) geo = {Shape::lineString(tmp)};
        else {
            tmp.push_back(tmp.front());
            try {
                geo = {Shape::linearRing(tmp)};
            } catch (const GeometryError&) {
                geo = {Shape::lineString(tmp)};
            }
        }
    } else if (n == "path" && !t.points.empty()) {
        std::vector<Point2D> tmp = t.points;
        tmp.push_back(data);
        geo = {Shape::lineString(tmp)};
    } else if ((n == "move" || n == "copy" || n == "eraser") && t.origin && !model_.selected.empty()) {
        const double dx = data.x - t.origin->x, dy = data.y - t.origin->y;
        const int limit = defInt("geometry_editor_sel_limit", 30);
        if (n == "eraser" || static_cast<int>(model_.selected.size()) <= limit) {
            for (const auto& s : model_.selected) geo.push_back(s->geo.translate(dx, dy));
        } else {
            const BoundingBox b = model_.selectionBounds();
            geo = {Shape::polygon({{b.xmin, b.ymin}, {b.xmax, b.ymin}, {b.xmax, b.ymax}, {b.xmin, b.ymax}})
                       .translate(dx, dy)};
        }
    } else if (n == "text" && !textTool_->textPath.empty()) {
        geo = {Shape::collection(textTool_->textPath).translate(data.x, data.y)};
    }
    if (!geo.empty()) drawUtility(geo);
}

void GeoEditor::drawUtility(const ShapeList& geo) {
    PlotLayerData d;
    PlotGroup g;
    for (const Shape& s : geo) addOutline(s, g);
    g.lineColor = colorFromFlatCam(defStr("global_draw_color") + "80");
    d.groups.push_back(g);
    app_->ui()->plotCanvas()->setLayer(kToolLayer, std::move(d));
}

void GeoEditor::deleteUtility() { app_->ui()->plotCanvas()->removeLayer(kToolLayer); }

void GeoEditor::replot() {
    PlotLayerData d;
    PlotGroup normal, sel;
    for (const auto& s : model_.storage.objects()) {
        if (s->geo.isEmpty()) continue;
        const bool isSel = std::find(model_.selected.begin(), model_.selected.end(), s) != model_.selected.end();
        addOutline(s->geo, isSel ? sel : normal);
    }
    normal.lineColor = colorFromFlatCam(defStr("global_draw_color") + "FF");
    sel.lineColor = colorFromFlatCam(defStr("global_sel_draw_color") + "FF");
    d.groups.push_back(normal);
    d.groups.push_back(sel);
    app_->ui()->plotCanvas()->setLayer(kShapesLayer, std::move(d));
}

void GeoEditor::buildUi() {
    // build_ui: una fila por figura (el id de Python es la dirección del objeto; el port guarda su posición).
    QSignalBlocker block(panel_->tw);
    panel_->tw->clear();
    QFont bold;
    bold.setBold(true);
    int i = 0;
    for (const auto& s : model_.storage.objects()) {
        auto* it = new QTreeWidgetItem(panel_->tw, {QString::number(reinterpret_cast<quintptr>(s.get())),
                                                    geoTypeName(s->geo), tr_("Geo Elem")});
        it->setData(0, Qt::UserRole, i++);
        it->setFont(2, bold);
        it->setFlags(it->flags() | Qt::ItemIsEditable);
    }
}

void GeoEditor::syncTreeSelection() {
    // El original busca el id en la columna 1 (la del tipo): nunca coincide y la lista no muestra lo elegido.
    QSignalBlocker block(panel_->tw);
    panel_->tw->selectionModel()->clearSelection();
}

void GeoEditor::unite() {
    model_.unite();
    buildUi();
    replot();
}

void GeoEditor::intersection() {
    if (!model_.intersection()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("A selection of minimum two items is required to do Intersection."));
        selectTool("select");
        return;
    }
    buildUi();
    replot();
}

void GeoEditor::subtract() {
    try {
        model_.subtract();
    } catch (const GeometryError&) {
    }
    buildUi();
    replot();
}

void GeoEditor::cutpath() {
    if (!model_.selected.empty() && !model_.cutpath()) return;  // "Not implemented": solo en el registro
    buildUi();
    replot();
}

void GeoEditor::deleteSelected() {
    model_.deleteSelected();
    buildUi();
    replot();
}

void GeoEditor::onMoveClick() {
    // on_move_click: el origen es la posición del mouse.
    app_->ui()->geo_move_btn->setChecked(true);
    onToolSelect("move");
    if (tool_) {
        app_->ui()->inform(tr_("Click on destination point ..."));
        tool_->origin = snap(mouse_);
    }
}

void GeoEditor::onCopyClick() {
    if (model_.selected.empty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled.") + " " + tr_("No shape selected."));
        return;
    }
    app_->ui()->geo_copy_btn->setChecked(true);
    onToolSelect("copy");
    if (tool_) {
        app_->ui()->inform(tr_("Click on destination point ..."));
        tool_->origin = snap(mouse_);
    }
    app_->ui()->inform(tr_("Click on target point."));
}

bool GeoEditor::handleKey(int key, Qt::KeyboardModifiers mods) {
    MainGUI* ui = app_->ui();
    if (mods == Qt::ControlModifier) {
        if (key == Qt::Key_S) {
            app_->editorToObject();
            return true;
        }
        if (key == Qt::Key_M) {
            app_->distanceTool->run();
            return true;
        }
        if (key == Qt::Key_X) {
            cutpath();
            return true;
        }
        return false;
    }
    if (mods == Qt::ShiftModifier) {
        if (key == Qt::Key_X) transformTool_->onSkewXKey();
        else if (key == Qt::Key_Y) transformTool_->onSkewYKey();
        else return false;
        return true;
    }
    if (mods == Qt::AltModifier) {
        if (key == Qt::Key_R) selectTool("transform");
        else if (key == Qt::Key_X) transformTool_->onOffXKey();
        else if (key == Qt::Key_Y) transformTool_->onOffYKey();
        else return false;
        return true;
    }
    if (mods != Qt::NoModifier && mods != Qt::KeypadModifier) return false;
    if (key == Qt::Key_QuoteLeft) ui->onToggleNotebook();
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && tool_ && tool_->isShapeTool()) {
        Tool& t = *tool_;
        // click(snap(x, y)) + make(): solo el polígono y el trayecto terminan así.
        if (t.name == "polygon" || t.name == "path") {
            t.points.push_back(snap(mouse_));
            t.geometry = {t.name == "path" ? Shape::lineString(t.points) : Shape::polygon(t.points)};
            inAction_ = false;
            t.complete = true;
            onShapeComplete();
            ui->inform("[success] " + tr_("Done."));
        }
        selectTool("select");
    }
    if (key == Qt::Key_Escape) {
        ui->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        deleteUtility();
        cleanUpActive();
        inAction_ = false;
        selectTool("select");
        ui->splitter->setSizes({0, 1});
        return true;
    }
    if (key == Qt::Key_Delete) {
        deleteSelected();
    }
    if (key == Qt::Key_Space) transformTool_->onRotateKey();
    if (key == Qt::Key_Minus) ui->plotCanvas()->zoom(1.0 / defNum("global_zoom_ratio", 1.5), {mouse_.x, mouse_.y});
    if (key == Qt::Key_Equal) ui->plotCanvas()->zoom(defNum("global_zoom_ratio", 1.5), {mouse_.x, mouse_.y});
    if (key == Qt::Key_1) ui->notebook->setCurrentWidget(ui->project_tab);
    if (key == Qt::Key_2) ui->notebook->setCurrentWidget(ui->properties_tab);
    if (key == Qt::Key_3) ui->notebook->setCurrentWidget(ui->tool_tab);
    if (key == Qt::Key_G) ui->grid_snap_btn->trigger();
    if (key == Qt::Key_K) ui->corner_snap_btn->trigger();
    if (key == Qt::Key_V) app_->onZoomFit();
    if (tool_ && !ui->geo_select_btn->isChecked()) {
        // active_tool.on_key
        Tool& t = *tool_;
        if (key == Qt::Key_J) app_->onJumpTo();
        if (t.name == "arc") {
            if (key == Qt::Key_D) {
                t.direction = t.direction == ArcDirection::CounterClockwise ? ArcDirection::Clockwise
                                                                            : ArcDirection::CounterClockwise;
                ui->inform(QString("%1: %2").arg(tr_("Direction"),
                                                 t.direction == ArcDirection::Clockwise ? "CW" : "CCW"));
            }
            if (key == Qt::Key_M) {
                t.points.clear();
                deleteUtility();
                if (t.arcMode == geodraw::ArcMode::C12) {
                    t.arcMode = geodraw::ArcMode::P12C;
                    ui->inform(tr_("Mode: Start -> Stop -> Center. Click on Start point ..."));
                } else if (t.arcMode == geodraw::ArcMode::P12C) {
                    t.arcMode = geodraw::ArcMode::P132;
                    ui->inform(tr_("Mode: Point1 -> Point3 -> Point2. Click on Point1 ..."));
                } else {
                    t.arcMode = geodraw::ArcMode::C12;
                    ui->inform(tr_("Mode: Center -> Start -> Stop. Click on Center point ..."));
                }
            }
        }
        if ((t.name == "polygon" || t.name == "path") && key == Qt::Key_Backspace && !t.points.empty()) {
            t.points.pop_back();
            deleteUtility();
            updateUtility(mouse_);
            ui->inform(tr_("Backtracked one point ..."));
        }
    } else {
        switch (key) {
            case Qt::Key_A: selectTool("arc"); break;
            case Qt::Key_B: selectTool("buffer"); break;
            case Qt::Key_C: onCopyClick(); break;
            case Qt::Key_E: intersection(); break;
            case Qt::Key_I: selectTool("paint"); break;
            case Qt::Key_J: app_->onJumpTo(); break;
            case Qt::Key_M: onMoveClick(); break;
            case Qt::Key_N: selectTool("polygon"); break;
            case Qt::Key_O: selectTool("circle"); break;
            case Qt::Key_P: selectTool("path"); break;
            case Qt::Key_R: selectTool("rectangle"); break;
            case Qt::Key_S: subtract(); break;
            case Qt::Key_T: selectTool("text"); break;
            case Qt::Key_U: unite(); break;
            case Qt::Key_X: transformTool_->onFlipX(); break;
            case Qt::Key_Y: transformTool_->onFlipY(); break;
            default: break;
        }
    }
    if (key == Qt::Key_F3) app_->onShortcutList();
    return true;
}

}  // namespace flatcam::gui
