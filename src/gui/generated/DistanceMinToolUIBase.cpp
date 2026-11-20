// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DistanceMinToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void DistanceMinToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    w1->setText(trUi_(QString::fromUtf8("<font size=4><b>Minimum Distance Tool</b></font><br>")));
    layout->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    units_label = new QLabel(nullptr);
    units_label->setText(trUi_(QString::fromUtf8("Units:")));
    units_label->setToolTip(trUi_(QString::fromUtf8("Those are the units in which the distance is measured.")));
    lay2->setWidget(0, QFormLayout::ItemRole(0), units_label);
    units_value = new QLabel(nullptr);
    units_value->setEnabled(false);
    units_value->setText(trUi_(QString::fromUtf8("METRIC (mm)")));
    lay2->setWidget(0, QFormLayout::ItemRole(1), units_value);
    start_label = new QLabel(nullptr);
    start_label->setText(trUi_(QString::fromUtf8("First object point:")));
    start_label->setToolTip(trUi_(QString::fromUtf8("This is first object point coordinates.\nThis is the start point for measuring distance.")));
    lay2->setWidget(1, QFormLayout::ItemRole(0), start_label);
    start_entry = new FCEntry(nullptr);
    start_entry->setProperty("alignment", 130);
    start_entry->setReadOnly(true);
    start_entry->setText(trUi_(QString::fromUtf8("(0, 0)")));
    start_entry->setToolTip(trUi_(QString::fromUtf8("This is first object point coordinates.\nThis is the start point for measuring distance.")));
    lay2->setWidget(1, QFormLayout::ItemRole(1), start_entry);
    stop_label = new QLabel(nullptr);
    stop_label->setText(trUi_(QString::fromUtf8("Second object point:")));
    stop_label->setToolTip(trUi_(QString::fromUtf8("This is second object point coordinates.\nThis is the end point for measuring distance.")));
    lay2->setWidget(2, QFormLayout::ItemRole(0), stop_label);
    stop_entry = new FCEntry(nullptr);
    stop_entry->setProperty("alignment", 130);
    stop_entry->setReadOnly(true);
    stop_entry->setText(trUi_(QString::fromUtf8("(0, 0)")));
    stop_entry->setToolTip(trUi_(QString::fromUtf8("This is second object point coordinates.\nThis is the end point for measuring distance.")));
    lay2->setWidget(2, QFormLayout::ItemRole(1), stop_entry);
    distance_x_label = new QLabel(nullptr);
    distance_x_label->setText(trUi_(QString::fromUtf8("Dx:")));
    distance_x_label->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the X axis.")));
    lay2->setWidget(3, QFormLayout::ItemRole(0), distance_x_label);
    distance_x_entry = new FCEntry(nullptr);
    distance_x_entry->setProperty("alignment", 130);
    distance_x_entry->setReadOnly(true);
    distance_x_entry->setText(trUi_(QString::fromUtf8("0.0")));
    distance_x_entry->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the X axis.")));
    lay2->setWidget(3, QFormLayout::ItemRole(1), distance_x_entry);
    distance_y_label = new QLabel(nullptr);
    distance_y_label->setText(trUi_(QString::fromUtf8("Dy:")));
    distance_y_label->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the Y axis.")));
    lay2->setWidget(4, QFormLayout::ItemRole(0), distance_y_label);
    distance_y_entry = new FCEntry(nullptr);
    distance_y_entry->setProperty("alignment", 130);
    distance_y_entry->setReadOnly(true);
    distance_y_entry->setText(trUi_(QString::fromUtf8("0.0")));
    distance_y_entry->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the Y axis.")));
    lay2->setWidget(4, QFormLayout::ItemRole(1), distance_y_entry);
    angle_label = new QLabel(nullptr);
    angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    angle_label->setToolTip(trUi_(QString::fromUtf8("This is orientation angle of the measuring line.")));
    lay2->setWidget(5, QFormLayout::ItemRole(0), angle_label);
    angle_entry = new FCEntry(nullptr);
    angle_entry->setProperty("alignment", 130);
    angle_entry->setReadOnly(true);
    angle_entry->setText(trUi_(QString::fromUtf8("0.0")));
    angle_entry->setToolTip(trUi_(QString::fromUtf8("This is orientation angle of the measuring line.")));
    lay2->setWidget(5, QFormLayout::ItemRole(1), angle_entry);
    total_distance_label = new QLabel(nullptr);
    total_distance_label->setText(trUi_(QString::fromUtf8("<b>DISTANCE:</b>")));
    total_distance_label->setToolTip(trUi_(QString::fromUtf8("This is the point to point Euclidean distance.")));
    lay2->setWidget(6, QFormLayout::ItemRole(0), total_distance_label);
    total_distance_entry = new FCEntry(nullptr);
    total_distance_entry->setProperty("alignment", 130);
    total_distance_entry->setReadOnly(true);
    total_distance_entry->setText(trUi_(QString::fromUtf8("0.0")));
    total_distance_entry->setToolTip(trUi_(QString::fromUtf8("This is the point to point Euclidean distance.")));
    lay2->setWidget(6, QFormLayout::ItemRole(1), total_distance_entry);
    half_point_label = new QLabel(nullptr);
    half_point_label->setText(trUi_(QString::fromUtf8("<b>Half Point:</b>")));
    half_point_label->setToolTip(trUi_(QString::fromUtf8("This is the middle point of the point to point Euclidean distance.")));
    lay2->setWidget(7, QFormLayout::ItemRole(0), half_point_label);
    half_point_entry = new FCEntry(nullptr);
    half_point_entry->setProperty("alignment", 130);
    half_point_entry->setReadOnly(true);
    half_point_entry->setText(trUi_(QString::fromUtf8("(0, 0)")));
    half_point_entry->setToolTip(trUi_(QString::fromUtf8("This is the middle point of the point to point Euclidean distance.")));
    lay2->setWidget(7, QFormLayout::ItemRole(1), half_point_entry);
    layout->addLayout(lay2, 0);
    measure_btn = new QPushButton(nullptr);
    measure_btn->setText(trUi_(QString::fromUtf8("Measure")));
    layout->addWidget(measure_btn, 0);
    jump_hp_btn = new QPushButton(nullptr);
    jump_hp_btn->setEnabled(false);
    jump_hp_btn->setText(trUi_(QString::fromUtf8("Jump to Half Point")));
    layout->addWidget(jump_hp_btn, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> DistanceMinToolUIBase::membersByName() const {
    return {
        {"angle_entry", angle_entry},
        {"angle_label", angle_label},
        {"distance_x_entry", distance_x_entry},
        {"distance_x_label", distance_x_label},
        {"distance_y_entry", distance_y_entry},
        {"distance_y_label", distance_y_label},
        {"half_point_entry", half_point_entry},
        {"half_point_label", half_point_label},
        {"jump_hp_btn", jump_hp_btn},
        {"layout", layout},
        {"measure_btn", measure_btn},
        {"start_entry", start_entry},
        {"start_label", start_label},
        {"stop_entry", stop_entry},
        {"stop_label", stop_label},
        {"total_distance_entry", total_distance_entry},
        {"total_distance_label", total_distance_label},
        {"units_label", units_label},
        {"units_value", units_value},
    };
}

}  // namespace flatcam::gui
