// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DistanceToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void DistanceToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    w1->setText(trUi_(QString::fromUtf8("<font size=4><b>Distance Tool</b></font><br>")));
    layout->addWidget(w1, 0);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    units_label = new FCLabel(nullptr);
    units_label->setText(trUi_(QString::fromUtf8("Units:")));
    units_label->setToolTip(trUi_(QString::fromUtf8("Those are the units in which the distance is measured.")));
    lay2->addWidget(units_label, 0, 0, 1, 1);
    units_value = new FCLabel(nullptr);
    units_value->setEnabled(false);
    units_value->setText(trUi_(QString::fromUtf8("METRIC (mm)")));
    lay2->addWidget(units_value, 0, 1, 1, 1);
    snap_center_cb = new FCCheckBox(nullptr);
    snap_center_cb->setText(trUi_(QString::fromUtf8("Snap to center")));
    snap_center_cb->setToolTip(trUi_(QString::fromUtf8("Mouse cursor will snap to the center of the pad/drill\nwhen it is hovering over the geometry of the pad/drill.")));
    snap_center_cb->setHidden(true);
    lay2->addWidget(snap_center_cb, 1, 0, 1, 2);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->addWidget(w3, 2, 0, 1, 2);
    start_label = new FCLabel(nullptr);
    start_label->setText(trUi_(QString::fromUtf8("Start Coords:")));
    start_label->setToolTip(trUi_(QString::fromUtf8("This is measuring Start point coordinates.")));
    lay2->addWidget(start_label, 3, 0, 1, 1);
    start_entry = new FCEntry(nullptr);
    start_entry->setProperty("alignment", 130);
    start_entry->setReadOnly(true);
    start_entry->setText(trUi_(QString::fromUtf8("(0, 0)")));
    start_entry->setToolTip(trUi_(QString::fromUtf8("This is measuring Start point coordinates.")));
    lay2->addWidget(start_entry, 3, 1, 1, 1);
    stop_label = new FCLabel(nullptr);
    stop_label->setText(trUi_(QString::fromUtf8("Stop Coords:")));
    stop_label->setToolTip(trUi_(QString::fromUtf8("This is the measuring Stop point coordinates.")));
    lay2->addWidget(stop_label, 4, 0, 1, 1);
    stop_entry = new FCEntry(nullptr);
    stop_entry->setProperty("alignment", 130);
    stop_entry->setReadOnly(true);
    stop_entry->setText(trUi_(QString::fromUtf8("(0, 0)")));
    stop_entry->setToolTip(trUi_(QString::fromUtf8("This is the measuring Stop point coordinates.")));
    lay2->addWidget(stop_entry, 4, 1, 1, 1);
    distance_x_label = new FCLabel(nullptr);
    distance_x_label->setText(trUi_(QString::fromUtf8("Dx:")));
    distance_x_label->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the X axis.")));
    lay2->addWidget(distance_x_label, 5, 0, 1, 1);
    distance_x_entry = new FCEntry(nullptr);
    distance_x_entry->setProperty("alignment", 130);
    distance_x_entry->setReadOnly(true);
    distance_x_entry->setText(trUi_(QString::fromUtf8("0.0")));
    distance_x_entry->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the X axis.")));
    lay2->addWidget(distance_x_entry, 5, 1, 1, 1);
    distance_y_label = new FCLabel(nullptr);
    distance_y_label->setText(trUi_(QString::fromUtf8("Dy:")));
    distance_y_label->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the Y axis.")));
    lay2->addWidget(distance_y_label, 6, 0, 1, 1);
    distance_y_entry = new FCEntry(nullptr);
    distance_y_entry->setProperty("alignment", 130);
    distance_y_entry->setReadOnly(true);
    distance_y_entry->setText(trUi_(QString::fromUtf8("0.0")));
    distance_y_entry->setToolTip(trUi_(QString::fromUtf8("This is the distance measured over the Y axis.")));
    lay2->addWidget(distance_y_entry, 6, 1, 1, 1);
    angle_label = new FCLabel(nullptr);
    angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    angle_label->setToolTip(trUi_(QString::fromUtf8("This is orientation angle of the measuring line.")));
    lay2->addWidget(angle_label, 7, 0, 1, 1);
    angle_entry = new FCEntry(nullptr);
    angle_entry->setProperty("alignment", 130);
    angle_entry->setReadOnly(true);
    angle_entry->setText(trUi_(QString::fromUtf8("0.0")));
    angle_entry->setToolTip(trUi_(QString::fromUtf8("This is orientation angle of the measuring line.")));
    lay2->addWidget(angle_entry, 7, 1, 1, 1);
    total_distance_label = new FCLabel(nullptr);
    total_distance_label->setText(trUi_(QString::fromUtf8("<b>DISTANCE:</b>")));
    total_distance_label->setToolTip(trUi_(QString::fromUtf8("This is the point to point Euclidian distance.")));
    lay2->addWidget(total_distance_label, 8, 0, 1, 1);
    total_distance_entry = new FCEntry(nullptr);
    total_distance_entry->setProperty("alignment", 130);
    total_distance_entry->setReadOnly(true);
    total_distance_entry->setText(trUi_(QString::fromUtf8("0.0")));
    total_distance_entry->setToolTip(trUi_(QString::fromUtf8("This is the point to point Euclidian distance.")));
    lay2->addWidget(total_distance_entry, 8, 1, 1, 1);
    lay2->setColumnStretch(1, 1);
    layout->addLayout(lay2, 0);
    measure_btn = new FCButton(nullptr);
    measure_btn->setEnabled(false);
    measure_btn->setText(trUi_(QString::fromUtf8("Working...")));
    layout->addWidget(measure_btn, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> DistanceToolUIBase::membersByName() const {
    return {
        {"angle_entry", angle_entry},
        {"angle_label", angle_label},
        {"distance_x_entry", distance_x_entry},
        {"distance_x_label", distance_x_label},
        {"distance_y_entry", distance_y_entry},
        {"distance_y_label", distance_y_label},
        {"layout", layout},
        {"measure_btn", measure_btn},
        {"snap_center_cb", snap_center_cb},
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
