#include "gui/tools/ExtractDrillsTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/ExtractDrills.h"

namespace flatcam::gui {

ExtractDrillsTool::ExtractDrillsTool(App* app, QWidget* parent) : ExtractDrillsToolUIBase(parent), app_(app) {
    setupUi();
    connect(hole_size_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { onHoleSizeToggle(v); });
    connect(e_drills_button, &QPushButton::clicked, this, [this] { extract(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    const std::pair<FCCheckBox*, QWidget*> links[] = {{circular_cb, circular_ring_entry},
                                                      {oblong_cb, oblong_ring_entry},
                                                      {square_cb, square_ring_entry},
                                                      {rectangular_cb, rectangular_ring_entry},
                                                      {other_cb, other_ring_entry}};
    for (const auto& link : links) {
        QWidget* entry = link.second;
        connect(link.first, &QCheckBox::checkStateChanged, this,
                [entry](Qt::CheckState s) { entry->setDisabled(s == Qt::Unchecked); });
    }
}

ToolInfo ExtractDrillsTool::info() { return {tr_("Extract Drills"), tr_("Extract Drills Tool")}; }

void ExtractDrillsTool::setToolUi() {
    const QString cur = gerber_object_combo->currentText();
    gerber_object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) gerber_object_combo->addItem(o->name());
    if (!cur.isEmpty()) gerber_object_combo->setValue(cur);
    hole_size_radio->setValue(defVariant("tools_edrills_hole_type"));
    dia_entry->setValue(defNum("tools_edrills_hole_fixed_dia"));
    circular_ring_entry->setValue(defNum("tools_edrills_circular_ring"));
    oblong_ring_entry->setValue(defNum("tools_edrills_oblong_ring"));
    square_ring_entry->setValue(defNum("tools_edrills_square_ring"));
    rectangular_ring_entry->setValue(defNum("tools_edrills_rectangular_ring"));
    other_ring_entry->setValue(defNum("tools_edrills_others_ring"));
    circular_cb->setChecked(defBool("tools_edrills_circular"));
    oblong_cb->setChecked(defBool("tools_edrills_oblong"));
    square_cb->setChecked(defBool("tools_edrills_square"));
    rectangular_cb->setChecked(defBool("tools_edrills_rectangular"));
    other_cb->setChecked(defBool("tools_edrills_others"));
    factor_entry->setValue(defNum("tools_edrills_hole_prop_factor"));
}

void ExtractDrillsTool::onHoleSizeToggle(const QString& v) {
    const bool fixed = v == "fixed", ring = v == "ring", prop = v == "prop";
    for (QWidget* w : std::initializer_list<QWidget*>{fixed_label, dia_entry, dia_label}) w->setVisible(fixed);
    ring_frame->setVisible(ring);
    for (QWidget* w : std::initializer_list<QWidget*>{prop_label, factor_label, factor_entry}) w->setVisible(prop);
}

FlatCAMObj* ExtractDrillsTool::extract() {
    FlatCAMObj* src = app_->collection()->byName(gerber_object_combo->currentText());
    if (!src || src->kind() != ObjectKind::Gerber) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    ExtractDrillsSettings s;
    const QString m = hole_size_radio->value().toString();
    s.mode = m == "fixed" ? ExtractDrillsSettings::Mode::Fixed
           : m == "ring"  ? ExtractDrillsSettings::Mode::Ring
                          : ExtractDrillsSettings::Mode::Proportional;
    s.fixedDia = dia_entry->value();
    s.ringCircular = circular_ring_entry->value();
    s.ringOblong = oblong_ring_entry->value();
    s.ringSquare = square_ring_entry->value();
    s.ringRectangular = rectangular_ring_entry->value();
    s.ringOther = other_ring_entry->value();
    s.factor = factor_entry->value();
    s.circular = circular_cb->isChecked();
    s.oblong = oblong_cb->isChecked();
    s.square = square_cb->isChecked();
    s.rectangular = rectangular_cb->isChecked();
    s.other = other_cb->isChecked();
    s.decimals = appDecimals();
    const auto tools = extractDrills(static_cast<GerberObject*>(src)->gerber(), s);
    if (!tools) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("No drills extracted. Try different parameters."));
        return nullptr;
    }
    auto exc = std::make_unique<Excellon>();
    for (const ExtractedTool& t : *tools) exc->addDrillTool(t.id, t.dia, t.drills);
    exc->createGeometry();
    // fcobj.options['name'].rpartition('.')[0]
    const QString name = src->name();
    const int dot = name.lastIndexOf('.');
    return app_->addObject(std::make_unique<ExcellonObject>(dot < 0 ? QString() : name.left(dot), std::move(exc)));
}

}  // namespace flatcam::gui
