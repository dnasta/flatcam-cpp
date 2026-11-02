#include "gui/tools/IsolationTool.h"

#include "gui/App.h"
#include "gui/CanvasPicker.h"
#include "gui/Translation.h"
#include "tools/Isolation.h"

namespace flatcam::gui {

namespace {

IsolationType isoTypeOf(const QString& t) {
    if (t == "ext") return IsolationType::Exterior;
    if (t == "int") return IsolationType::Interior;
    return IsolationType::Full;
}

}  // namespace

IsolationTool::IsolationTool(App* app, QWidget* parent) : IsolationToolUIBase(parent), app_(app) {
    setupUi();
    connect(generate_iso_button, &QPushButton::clicked, this, [this] { isolate(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(order_radio, &RadioSet::activatedCustom, this, [this] { rebuild(); });
    connect(rest_cb, &QCheckBox::toggled, this, [this](bool on) { onRestMachiningCheck(on); });
    connect(select_combo, &QComboBox::currentIndexChanged, this, [this] { onToggleReference(); });
    connect(reference_combo_type, &QComboBox::currentIndexChanged, this, [this] { onToggleReference(); });
    connect(tools_table, &QTableWidget::itemSelectionChanged, this, [this] {
        if (!muted_) updateUi();
    });
    connect(search_and_add_btn, &QPushButton::clicked, this, [this] {
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
    connect(passes_entry, &QSpinBox::valueChanged, this, [this] { formToStorage(passes_entry); });
    connect(iso_overlap_entry, &QDoubleSpinBox::valueChanged, this, [this] { formToStorage(iso_overlap_entry); });
    for (FCCheckBox* w : {combine_passes_cb, follow_cb}) connect(w, &QCheckBox::toggled, this, [this, w] { formToStorage(w); });
    for (RadioSet* w : {milling_type_radio, iso_type_radio})
        connect(w, &RadioSet::activatedCustom, this, [this, w] { formToStorage(w); });
}

ToolInfo IsolationTool::info() { return {tr_("Isolation Tool"), tr_("Isolation Tool")}; }

void IsolationTool::setToolUi() {
    muted_ = true;
    object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) object_combo->addItem(o->name());
    if (FlatCAMObj* active = app_->activeObject(); active && active->kind() == ObjectKind::Gerber)
        object_combo->setValue(active->name());
    // Nivel Básico (global_app_level = 'b'): lo que esconde el original ya viene escondido en el panel generado.
    level->setText(QString("<span style=\"color:green;\"><b>%1</b></span>").arg(tr_("Basic")));
    iso_type_radio->setValue("full");
    follow_cb->setChecked(false);
    rest_cb->setChecked(false);
    except_cb->setChecked(false);
    select_combo->setCurrentIndex(0);
    // tools_iso_* de fábrica.
    order_radio->setValue("rev");
    passes_entry->setValue(1);
    iso_overlap_entry->setValue(10.0);
    milling_type_radio->setValue("cl");
    combine_passes_cb->setChecked(true);
    valid_cb->setChecked(false);
    area_shape_radio->setValue("square");
    poly_int_cb->setChecked(false);
    forced_rest_iso_cb->setChecked(true);
    new_tooldia_entry->setValue(0.1);
    tools_.clear();
    muted_ = false;
    oldCombineState_ = true;
    addTool(0.1);  // tools_iso_tooldia
    onRestMachiningCheck(false);
    onToggleReference();
}

void IsolationTool::onToggleReference() {
    // on_toggle_reference: Todos, Selección de área, Selección de polígono u Objeto de referencia.
    const int sel = select_combo->currentIndex();
    for (QWidget* w : std::initializer_list<QWidget*>{reference_combo, reference_combo_type, reference_combo_type_label})
        w->setVisible(sel == 3);
    area_shape_label->setVisible(sel == 1);
    area_shape_radio->setVisible(sel == 1);
    poly_int_cb->setVisible(sel == 2);
    if (sel == 1) {
        rest_cb->setChecked(false);
        rest_cb->setDisabled(true);
    } else if (sel != 2) {
        rest_cb->setDisabled(false);
    }
    const int t = reference_combo_type->indexValue();
    const ObjectKind kind = t == 1 ? ObjectKind::Excellon : t == 2 ? ObjectKind::Geometry : ObjectKind::Gerber;
    reference_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == kind) reference_combo->addItem(o->name());
}

void IsolationTool::onRestMachiningCheck(bool state) {
    order_label->setDisabled(state);
    order_radio->setDisabled(state);
    if (state) {
        order_radio->setValue("rev");
        oldCombineState_ = combine_passes_cb->isChecked();
        combine_passes_cb->setChecked(true);
    } else {
        combine_passes_cb->setChecked(oldCombineState_);
    }
    combine_passes_cb->setDisabled(state);
    forced_rest_iso_cb->setDisabled(!state);
}

bool IsolationTool::addTool(double dia) {
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
    ClearingTool t;  // data = default_data: geometry_* y tools_iso_* de fábrica
    t.geo = defaultGeometryTool();
    t.geo.tooldia = d;
    t.geo.offset = "Path";
    t.geo.type = "           Iso";  // así, con los espacios, en on_tool_default_add del original
    t.geo.toolType = "V";
    t.geo.data.cutZ = -0.05;  // tools_iso_tool_cutz
    t.passes = 1;
    t.overlap = 10.0;
    t.millingType = "cl";
    t.isoType = "full";
    t.follow = false;
    tools_[uid] = t;
    rebuild();
    for (int r = 0; r < tools_table->rowCount(); ++r)
        if (tools_table->item(r, 3)->text().toInt() == uid) tools_table->selectRow(r);
    updateUi();
    return true;
}

void IsolationTool::rebuild() {
    muted_ = true;
    buildClearingTable(tools_table, tools_, order_radio->value().toString());
    muted_ = false;
}

void IsolationTool::updateUi() {
    const std::vector<int> ids = selectedClearingTools(tools_table);
    if (ids.empty()) {
        generate_iso_button->setDisabled(true);
        // Así, con las comillas corridas, en el original.
        tool_data_label->setText(
            QString("<b>%1: <font color=#7777FF\"\">%2</font></b>").arg(tr_("Parameters for"), tr_("No Tool Selected")));
        return;
    }
    generate_iso_button->setDisabled(false);
    auto label = [this](const QString& what) {
        tool_data_label->setText(QString("<b>%1: <font color=\"#7777FF\">%2</font></b>").arg(tr_("Parameters for"), what));
    };
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
    passes_entry->setValue(t.passes);
    iso_overlap_entry->setValue(t.overlap);
    milling_type_radio->setValue(t.millingType);
    follow_cb->setChecked(t.follow);
    iso_type_radio->setValue(t.isoType);
    muted_ = false;
}

void IsolationTool::formToStorage(QObject* w) {
    if (muted_) return;
    // form_to_storage: solo el campo que cambió, en las herramientas elegidas.
    for (int id : selectedClearingTools(tools_table)) {
        ClearingTool& t = tools_.at(id);
        if (w == passes_entry) t.passes = passes_entry->value();
        else if (w == iso_overlap_entry) t.overlap = iso_overlap_entry->value();
        else if (w == milling_type_radio) t.millingType = milling_type_radio->value().toString();
        else if (w == follow_cb) t.follow = follow_cb->isChecked();
        else if (w == iso_type_radio) t.isoType = iso_type_radio->value().toString();
    }
}

std::vector<FlatCAMObj*> IsolationTool::isolate() {
    std::vector<FlatCAMObj*> created;
    const QString name = object_combo->currentText();
    auto* grb = dynamic_cast<GerberObject*>(app_->collection()->byName(name));
    if (!grb) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), name));
        return created;
    }
    // Las elegidas, en el orden de la tabla.
    const std::vector<int> ids = selectedClearingTools(tools_table);
    if (ids.empty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("There are no tools selected in the Tool Table."));
        return created;
    }
    const int selection = select_combo->currentIndex();
    const QString grbName = grb->name();
    if (selection == 1) {  // Selección de área: on_mouse_release -> isolate(limited_area=...)
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Click the start point of the area."));
        const auto mode = area_shape_radio->value().toString() == "square" ? CanvasPicker::Mode::Rectangles
                                                                             : CanvasPicker::Mode::Polygons;
        app_->picker()->start(mode, {}, [this, grbName, ids](const ShapeList& zones) {
            if (auto* g = dynamic_cast<GerberObject*>(app_->collection()->byName(grbName)))
                runIsolation(g, ids, nullptr, {Shape::unaryUnion(zones)}, false);
        });
        return created;
    }
    if (selection == 2) {  // Selección de polígono: on_poly_mouse_click_release -> isolate(geometry=...)
        // Con "Interiores" se eligen los agujeros de los polígonos y se aíslan hacia adentro (negative_dia).
        const bool interiors = poly_int_cb->isChecked();
        ShapeList candidates;
        for (const Shape& s : grb->gerber().solidGeometry()) {
            for (const Shape& p : s.numParts() > 1 ? s.parts() : ShapeList{s}) {
                if (p.type() != ShapeType::Polygon) continue;
                if (!interiors) candidates.push_back(p);
                else
                    for (const Shape& h : p.interiors()) candidates.push_back(Shape::polygon(h.coordinates()));
            }
        }
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Click on a polygon to isolate it."));
        app_->picker()->start(CanvasPicker::Mode::PickPolygons, candidates,
                              [this, grbName, ids, interiors](const ShapeList& polys) {
                                  if (auto* g = dynamic_cast<GerberObject*>(app_->collection()->byName(grbName)))
                                      runIsolation(g, ids, &polys, {}, interiors);
                              });
        return created;
    }
    ShapeList refGeometry;
    const ShapeList* geometry = nullptr;
    if (selection == 3) {  // objeto de referencia: el cobre menos la referencia
        FlatCAMObj* ref = app_->collection()->byName(reference_combo->currentText());
        ShapeList refShapes;
        if (auto* g = dynamic_cast<GerberObject*>(ref)) refShapes = g->gerber().solidGeometry();
        else if (auto* geo = dynamic_cast<GeometryObject*>(ref)) refShapes = geo->solidGeometry();
        refGeometry = flatcam::Isolation::referenceGeometry(grb->gerber().solidGeometry(), refShapes);
        geometry = &refGeometry;
    }
    return runIsolation(grb, ids, geometry, {}, false);
}

std::vector<FlatCAMObj*> IsolationTool::runIsolation(GerberObject* grb, const std::vector<int>& ids,
                                                     const ShapeList* geometry, const ShapeList& limitedArea,
                                                     bool negativeDia) {
    std::vector<FlatCAMObj*> created;
    std::vector<flatcam::IsolationTool> isoTools;
    for (int id : ids) {
        const ClearingTool& t = tools_.at(id);
        flatcam::IsolationTool it;
        it.toolDia = t.geo.tooldia;
        it.passes = t.passes;
        it.overlap = t.overlap / 100.0;
        it.milling = t.millingType == "cl" ? MillingDirection::Climb : MillingDirection::Conventional;
        it.type = isoTypeOf(t.isoType);
        it.follow = t.follow;
        isoTools.push_back(it);
    }
    IsolationOptions o;
    o.combine = combine_passes_cb->isChecked();
    o.rest = rest_cb->isChecked();
    o.forcedRest = forced_rest_iso_cb->isChecked();
    const QString order = order_radio->value().toString();
    o.order = order == "fwd" ? IsolationToolOrder::Forward : order == "rev" ? IsolationToolOrder::Reverse
                                                                            : IsolationToolOrder::Default;
    if (except_cb->isChecked()) {
        o.useExcept = true;
        if (FlatCAMObj* ex = app_->collection()->byName(exc_obj_combo->currentText())) {
            if (auto* g = dynamic_cast<GerberObject*>(ex)) o.exceptGeometry = g->gerber().solidGeometry();
            else if (auto* geo = dynamic_cast<GeometryObject*>(ex)) o.exceptGeometry = geo->solidGeometry();
        }
    }
    o.limitedArea = limitedArea;
    o.negativeDia = negativeDia;
    IsolationOutcome out;
    std::string error;
    {
        BusyScope busy(app_->ui());
        if (!flatcam::Isolation::isolate(grb->gerber().solidGeometry(), grb->gerber().followGeometry(), isoTools, o, out,
                                         error, geometry)) {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("Isolation geometry could not be generated.") +
                               (error.empty() ? QString() : " " + QString::fromStdString(error)));
            return created;
        }
    }
    auto geoTool = [&](std::size_t index) { return tools_.at(ids[index]).geo; };
    if (o.combine) {
        // combined_normal / combined_rest: un objeto con una herramienta por diámetro.
        auto obj = std::make_unique<GeometryObject>(grb->name() + (o.rest ? "_iso_rest" : "_iso_combined"));
        for (const IsolationPathSet& s : out.sets) {
            GeometryTool g = geoTool(s.toolIndex);
            g.solidGeometry = s.geometry;
            obj->tools()[ids[s.toolIndex]] = g;
            obj->solidGeometry().insert(obj->solidGeometry().end(), s.geometry.begin(), s.geometry.end());
        }
        app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Isolation geometry created"), obj->name()));
        created.push_back(app_->addObject(std::move(obj)));
        if (o.rest && !out.notIsolated.empty()) {
            app_->ui()->inform("[WARNING] " + tr_("Partial failure. The geometry was processed with all tools.\n"
                                                  "But there are still not-isolated geometry elements. "
                                                  "Try to include a tool with smaller diameter."));
        }
    } else {
        // isolate sin combinar: un objeto por pasada, con la herramienta como '1'.
        for (const IsolationPathSet& s : out.sets) {
            const flatcam::IsolationTool& it = isoTools[s.toolIndex];
            const QString objName = QString::fromStdString(flatcam::Isolation::objectName(
                grb->name().toStdString(), it.toolDia, it.type, s.pass, it.passes));
            auto obj = std::make_unique<GeometryObject>(objName);
            GeometryTool g = geoTool(s.toolIndex);
            g.type = "Rough";
            g.solidGeometry = s.geometry;
            obj->tools()[1] = g;
            obj->solidGeometry() = s.geometry;
            app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Isolation geometry created"), objName));
            created.push_back(app_->addObject(std::move(obj)));
        }
    }
    app_->ui()->notebook->setCurrentWidget(app_->ui()->properties_tab);
    return created;
}

}  // namespace flatcam::gui
