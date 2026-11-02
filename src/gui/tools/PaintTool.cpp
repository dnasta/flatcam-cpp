#include "gui/tools/PaintTool.h"

#include <QStandardItemModel>

#include "gui/App.h"
#include "gui/CanvasPicker.h"
#include "gui/Translation.h"
#include "gui/tools/ClearingTools.h"
#include "tools/Paint.h"

namespace flatcam::gui {

PaintTool::PaintTool(App* app, QWidget* parent) : PaintToolUIBase(parent), app_(app) {
    setupUi();
    connect(generate_paint_button, &QPushButton::clicked, this, [this] { paint(); });
    connect(type_obj_radio, &RadioSet::activatedCustom, this, [this] { onTypeObjChanged(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(order_radio, &RadioSet::activatedCustom, this, [this] { rebuild(); });
    connect(rest_cb, &QCheckBox::toggled, this, [this](bool on) { onRestMachiningCheck(on); });
    connect(reference_type_combo, &QComboBox::currentIndexChanged, this, [this] { fillReferenceCombo(); });
    connect(selectmethod_combo, &QComboBox::currentIndexChanged, this, [this] { onSelection(); });
    connect(tools_table, &QTableWidget::itemSelectionChanged, this, [this] {
        if (!muted_) updateUi();
    });
    connect(add_newtool_button, &QPushButton::clicked, this, [this] {
        if (addTool(new_tooldia_entry->value())) app_->ui()->inform("[success] " + tr_("Default tool added to Tool Table."));
    });
    connect(deltool_btn, &QPushButton::clicked, this, [this] {
        const std::vector<int> ids = selectedClearingTools(tools_table);
        if (ids.empty()) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Delete failed. Select a tool to delete."));
            return;
        }
        for (int id : ids) tools_.erase(id);
        rebuild();
        app_->ui()->inform("[success] " + tr_("Tool(s) deleted from Tool Table."));
    });
    // form_to_storage: lo que se cambia va a las herramientas elegidas.
    for (FCDoubleSpinner* w : {paintoverlap_entry, offset_entry})
        connect(w, &QDoubleSpinBox::valueChanged, this, [this] { formToStorage(); });
    connect(paintmethod_combo, &QComboBox::currentIndexChanged, this, [this] { formToStorage(); });
    for (FCCheckBox* w : {pathconnect_cb, paintcontour_cb})
        connect(w, &QCheckBox::toggled, this, [this] { formToStorage(); });
}

ToolInfo PaintTool::info() { return {tr_("Paint Tool"), tr_("Paint Tool")}; }

void PaintTool::onRestMachiningCheck(bool state) {
    // PaintUI.on_rest_machining_check
    if (state) order_radio->setValue("rev");
    order_label->setDisabled(state);
    order_radio->setDisabled(state);
    offset_label->setVisible(!state);
    offset_entry->setVisible(!state);
    rest_offset_label->setVisible(state);
    rest_offset_entry->setVisible(state);
}

void PaintTool::onSelection() {
    // PaintUI.on_selection
    const int sel = selectmethod_combo->indexValue();
    for (QWidget* w : std::initializer_list<QWidget*>{reference_combo, reference_type_combo, reference_type_label})
        w->setVisible(sel == 3);
    area_shape_label->setVisible(sel == 2);
    area_shape_radio->setVisible(sel == 2);
}

void PaintTool::fillReferenceCombo() {
    // on_reference_combo_changed: los objetos del tipo elegido (Gerber, Excellon o Geometría).
    const int t = reference_type_combo->indexValue();
    const ObjectKind kind = t == 1 ? ObjectKind::Excellon : t == 2 ? ObjectKind::Geometry : ObjectKind::Gerber;
    reference_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kind) reference_combo->addItem(o->name());
}

void PaintTool::onTypeObjChanged() {
    const bool gerber = type_obj_radio->value().toString() == "gerber";
    obj_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == (gerber ? ObjectKind::Gerber : ObjectKind::Geometry)) obj_combo->addItem(o->name());
    obj_combo->setCurrentIndex(0);
    // "Líneas láser" (índice 3) solo para Gerber; con una Geometría, si estaba elegido pasa al siguiente (Combo).
    if (auto* model = qobject_cast<QStandardItemModel*>(paintmethod_combo->model())) {
        if (QStandardItem* item = model->item(3)) item->setEnabled(gerber);
    }
    if (!gerber && paintmethod_combo->indexValue() == 3) paintmethod_combo->setIndexValue(4);
}

void PaintTool::setToolUi() {
    muted_ = true;
    // tools_paint_* de fábrica.
    order_radio->setValue("rev");
    offset_entry->setValue(0.0);
    paintmethod_combo->setIndexValue(0);
    selectmethod_combo->setIndexValue(0);
    area_shape_radio->setValue("square");
    pathconnect_cb->setChecked(true);
    paintcontour_cb->setChecked(true);
    paintoverlap_entry->setValue(20.0);
    new_tooldia_entry->setValue(0.1);
    rest_cb->setChecked(false);
    // El objeto activo, si es Gerber o Geometría; si no, Geometría.
    FlatCAMObj* active = app_->activeObject();
    const bool gerber = active && active->kind() == ObjectKind::Gerber;
    type_obj_radio->setValue(gerber ? "gerber" : "geometry");
    onTypeObjChanged();
    if (active && (active->kind() == ObjectKind::Gerber || active->kind() == ObjectKind::Geometry))
        obj_combo->setValue(active->name());
    tools_.clear();
    muted_ = false;
    addTool(0.3);  // tools_paint_tooldia
    onRestMachiningCheck(false);
    fillReferenceCombo();
    onSelection();
}

bool PaintTool::addTool(double dia) {
    // on_tool_add → sin base de datos de herramientas → on_tool_default_add.
    const double d = QString::number(dia, 'f', 4).toDouble();
    if (d == 0.0) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Please enter a tool diameter with non-zero value, in Float format."));
        return false;
    }
    for (const auto& [id, t] : tools_) {
        if (QString::number(t.geo.tooldia, 'f', 4).toDouble() == d) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled. Tool already in Tool Table."));
            return false;
        }
    }
    const int uid = tools_.empty() ? 1 : tools_.rbegin()->first + 1;
    ClearingTool t;
    t.geo = defaultGeometryTool();
    t.geo.tooldia = d;
    t.geo.offset = "Path";
    t.geo.type = "Iso";
    t.geo.toolType = "V";  // así lo pone on_tool_default_add de Paint
    t.geo.data.cutZ = -0.05;  // tools_paint_cutz
    // data = default_data: los tools_paint_* de fábrica (no lo que tenga el formulario).
    t.overlap = 20.0;
    t.offset = 0.0;
    t.method = 0;
    t.connect = true;
    t.contour = true;
    tools_[uid] = t;
    rebuild();
    // Queda elegida la herramienta nueva.
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (tools_table->item(r, 3)->text().toInt() == uid) tools_table->selectRow(r);
    updateUi();
    return true;
}

void PaintTool::rebuild() {
    muted_ = true;
    buildClearingTable(tools_table, tools_, order_radio->value().toString());
    muted_ = false;
}

void PaintTool::updateUi() {
    const std::vector<int> ids = selectedClearingTools(tools_table);
    // En Paint el color está fijo en el código ('#0000FF', con comillas simples).
    auto label = [this](const QString& what) {
        tool_data_label->setText(QString("<b>%1: <font color='#0000FF'>%2</font></b>").arg(tr_("Parameters for"), what));
    };
    if (ids.empty()) {
        generate_paint_button->setDisabled(true);
        label(tr_("No Tool Selected"));
        return;
    }
    generate_paint_button->setDisabled(false);
    if (ids.size() > 1) {
        label(tr_("Multiple Tools"));
        return;
    }
    int row = 0;
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (tools_table->item(r, 3)->text().toInt() == ids.front()) row = r;
    label(QString("%1 %2").arg(tr_("Tool"), tools_table->item(row, 0)->text()));
    const ClearingTool& t = tools_.at(ids.front());
    muted_ = true;
    paintoverlap_entry->setValue(t.overlap);
    offset_entry->setValue(t.offset);
    paintmethod_combo->setIndexValue(t.method);
    pathconnect_cb->setChecked(t.connect);
    paintcontour_cb->setChecked(t.contour);
    muted_ = false;
}

void PaintTool::formToStorage() {
    if (muted_) return;
    for (int id : selectedClearingTools(tools_table)) {
        ClearingTool& t = tools_.at(id);
        t.overlap = paintoverlap_entry->value();
        t.offset = offset_entry->value();
        t.method = paintmethod_combo->indexValue();
        t.connect = pathconnect_cb->isChecked();
        t.contour = paintcontour_cb->isChecked();
    }
}

FlatCAMObj* PaintTool::paint() {
    const QString name = obj_combo->currentText();
    FlatCAMObj* src = app_->collection()->byName(name);
    if (!src) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), name));
        return nullptr;
    }
    if (src->kind() == ObjectKind::Geometry && static_cast<GeometryObject*>(src)->tools().size() > 1) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Can't do Paint on MultiGeo geometries") + "...");
        return nullptr;
    }
    const std::vector<int> ids = selectedClearingTools(tools_table);
    if (ids.empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("There are no tools selected in the Tool Table."));
        return nullptr;
    }
    // Fuente: la geometría sólida del Gerber, o la de la (única) herramienta de la Geometría.
    ShapeList geometry;
    if (src->kind() == ObjectKind::Gerber) {
        geometry = static_cast<GerberObject*>(src)->gerber().solidGeometry();
    } else {
        auto* g = static_cast<GeometryObject*>(src);
        geometry = g->tools().empty() ? g->solidGeometry() : g->tools().begin()->second.solidGeometry;
    }
    const int sel = selectmethod_combo->indexValue();
    const QString srcName = src->name();
    // paint_poly_area: el Gerber (o la Geometría) recortado a la zona elegida.
    auto inArea = [geometry](const Shape& zone) {
        return Paint::flattenToPolygons({Shape::collection(Paint::flattenToPolygons(geometry)).intersection(zone)});
    };
    if (sel == 1) {  // Selección de polígono
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Click on a polygon to paint it."));
        app_->picker()->start(CanvasPicker::Mode::PickPolygons, Paint::flattenToPolygons(geometry),
                              [this, srcName, ids](const ShapeList& polys) {
                                  if (FlatCAMObj* s = app_->collection()->byName(srcName)) paintGeometry(s, polys, ids);
                              });
        return nullptr;
    }
    if (sel == 2) {  // Selección de área
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Click the start point of the area."));
        const auto mode = area_shape_radio->value().toString() == "square" ? CanvasPicker::Mode::Rectangles
                                                                             : CanvasPicker::Mode::Polygons;
        app_->picker()->start(mode, {}, [this, srcName, ids, inArea](const ShapeList& zones) {
            if (FlatCAMObj* s = app_->collection()->byName(srcName))
                paintGeometry(s, inArea(Shape::unaryUnion(zones)), ids);
        });
        return nullptr;
    }
    if (sel == 3) {  // Objeto de referencia: paint_poly_ref
        FlatCAMObj* ref = app_->collection()->byName(reference_combo->currentText());
        if (!ref) {
            app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Could not retrieve object"), reference_combo->currentText()));
            return nullptr;
        }
        ShapeList refGeo;
        if (auto* g = dynamic_cast<GerberObject*>(ref)) refGeo = g->gerber().solidGeometry();
        else if (auto* e = dynamic_cast<ExcellonObject*>(ref)) refGeo = e->excellon().solidGeometry();
        else if (auto* gg = dynamic_cast<GeometryObject*>(ref)) refGeo = gg->solidGeometry();
        const Shape env = refGeo.size() == 1 && refGeo.front().type() == ShapeType::Polygon
                              ? Shape::unaryUnion(refGeo)
                              : Shape::unaryUnion(refGeo).convexHull();
        return paintGeometry(src, inArea(env.buffer(0.0000001, 16, JoinStyle::Mitre)), ids);
    }
    return paintGeometry(src, geometry, ids);
}

FlatCAMObj* PaintTool::paintGeometry(FlatCAMObj* src, const ShapeList& geometry, const std::vector<int>& ids) {
    if (geometry.empty()) {
        app_->ui()->inform("[WARNING] " + tr_("No polygon found."));
        return nullptr;
    }
    app_->ui()->inform(QString("%1 %2").arg(tr_("Paint Tool."), tr_("Paint all polygons task started.")));
    // El original calcula en un hilo de trabajo; acá se calcula en el de la interfaz con el indicador de actividad.
    BusyScope busy(app_->ui());
    auto obj = std::make_unique<GeometryObject>(src->name() + "_mt_paint");
    const int formMethod = paintmethod_combo->indexValue();
    auto methodOf = [](int m) {
        return m == 1 ? PolygonClearing::Method::Seed
             : m == 2 ? PolygonClearing::Method::Lines
             : m == 3 ? PolygonClearing::Method::LaserLines
             : m == 4 ? PolygonClearing::Method::Combo
                      : PolygonClearing::Method::Standard;
    };
    // "Líneas láser" usa las aperturas del Gerber pintado.
    auto* gerberObj = dynamic_cast<GerberObject*>(src);
    const Gerber* gerber = gerberObj ? &gerberObj->gerber() : nullptr;
    if (rest_cb->isChecked()) {
        // job_rest_clear: el método es el del formulario; el resto, de cada herramienta.
        std::vector<PaintRestTool> rt;
        for (int id : ids) {
            const ClearingTool& t = tools_.at(id);
            rt.push_back({t.geo.tooldia, t.overlap / 100.0, t.connect, t.contour});
        }
        std::vector<std::pair<std::size_t, ShapeList>> perTool;
        std::string error;
        if (!Paint::paintRest(geometry, rt, rest_offset_entry->value(), methodOf(formMethod), 64, perTool, error,
                              gerber)) {
            app_->ui()->inform("[ERROR] " + tr_("There is no Painting Geometry in the file.\n"
                                                "Usually it means that the tool diameter is too big for the painted geometry.\n"
                                                "Change the painting parameters and try again."));
            return nullptr;
        }
        for (const auto& [idx, paths] : perTool) {
            GeometryTool gt = tools_.at(ids[idx]).geo;
            gt.solidGeometry = paths;
            obj->tools()[ids[idx]] = gt;
            obj->solidGeometry().insert(obj->solidGeometry().end(), paths.begin(), paths.end());
        }
        FlatCAMObj* added = app_->addObject(std::move(obj));
        app_->ui()->inform("[success] " + tr_("Done."));
        return added;
    }
    // job_normal_clear: cada herramienta pinta todos los polígonos, en el orden de la tabla.
    for (int id : orderedClearingTools(tools_, ids, order_radio->value().toString())) {
        const ClearingTool& t = tools_.at(id);
        // paintmethod_combo: Estándar, Semilla, Líneas, Líneas láser, Combo.
        PaintOptions o;
        o.toolDia = t.geo.tooldia;
        o.overlap = t.overlap / 100.0;
        o.connect = t.connect;
        o.offset = t.offset;
        o.contour = t.contour;
        o.method = methodOf(t.method);
        ShapeList paths;
        std::string error;
        if (!Paint::paintAll(geometry, o, paths, error, gerber) || paths.empty()) continue;
        GeometryTool gt = t.geo;
        gt.solidGeometry = paths;
        obj->tools()[id] = gt;
        obj->solidGeometry().insert(obj->solidGeometry().end(), paths.begin(), paths.end());
    }
    if (obj->tools().empty()) {
        app_->ui()->inform("[ERROR] " + tr_("There is no Painting Geometry in the file.\n"
                                            "Usually it means that the tool diameter is too big for the painted geometry.\n"
                                            "Change the painting parameters and try again."));
        return nullptr;
    }
    FlatCAMObj* added = app_->addObject(std::move(obj));
    app_->ui()->inform("[success] " + tr_("Done."));
    return added;
}

}  // namespace flatcam::gui
