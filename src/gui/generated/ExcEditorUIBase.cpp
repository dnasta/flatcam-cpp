// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ExcEditorUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void ExcEditorUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    drills_frame = new QFrame(nullptr);
    ui_vertical_lay = new QVBoxLayout();
    ui_vertical_lay->setContentsMargins(0, 0, 0, 0);
    ui_vertical_lay->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Excellon Editor</b></font>")));
    title_box->addWidget(title_label, 1);
    ui_vertical_lay->addLayout(title_box, 0);
    name_box = new QHBoxLayout();
    name_box->setContentsMargins(0, 0, 0, 0);
    name_box->setSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(trUi_(QString::fromUtf8("Name:")));
    name_box->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_box->addWidget(name_entry, 0);
    ui_vertical_lay->addLayout(name_box, 0);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(trUi_(QString::fromUtf8("<b>Tools Table</b>")));
    tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools in this Excellon object\nwhen are used for drilling.")));
    ui_vertical_lay->addWidget(tools_table_label, 0);
    tools_table_exc = new FCTable(nullptr);
    tools_table_exc->setProperty("columnCount", 4);
    tools_table_exc->setProperty("selectionBehavior", 1);
    tools_table_exc->setColumnCount(4);
    tools_table_exc->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Diameter")), trUi_(QString::fromUtf8("D")), trUi_(QString::fromUtf8("S"))});
    ui_vertical_lay->addWidget(tools_table_exc, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    ui_vertical_lay->addWidget(w3, 0);
    convert_slots_btn = new FCButton(nullptr);
    convert_slots_btn->setIcon(QIcon(QString::fromUtf8(":/images/convert32.png")));
    convert_slots_btn->setText(trUi_(QString::fromUtf8("Convert Slots")));
    convert_slots_btn->setToolTip(trUi_(QString::fromUtf8("Convert the slots in the selected tools to drills.")));
    ui_vertical_lay->addWidget(convert_slots_btn, 0);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    ui_vertical_lay->addWidget(w4, 0);
    addtool_label = new FCLabel(nullptr);
    addtool_label->setText(trUi_(QString::fromUtf8("<b>Add/Delete Tool</b>")));
    addtool_label->setToolTip(trUi_(QString::fromUtf8("Add/Delete a tool to the tool list\nfor this Excellon object.")));
    ui_vertical_lay->addWidget(addtool_label, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    w6->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool")));
    lay5->addWidget(w6, 0, 0, 1, 1);
    auto* lay7 = new QHBoxLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setSpacing(6);
    addtool_entry = new FCDoubleSpinner(nullptr);
    addtool_entry->setProperty("decimals", 4);
    addtool_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); addtool_entry->setSizePolicy(sp); }
    lay7->addWidget(addtool_entry, 0);
    addtool_btn = new FCButton(nullptr);
    addtool_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    addtool_btn->setText(trUi_(QString::fromUtf8("Add")));
    addtool_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the tool list\nwith the diameter specified above.")));
    lay7->addWidget(addtool_btn, 0);
    lay5->addLayout(lay7, 0, 1, 1, 1);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    deltool_btn->setText(trUi_(QString::fromUtf8("Delete Tool")));
    deltool_btn->setToolTip(trUi_(QString::fromUtf8("Delete a tool in the tool list\nby selecting a row in the tool table.")));
    lay5->addWidget(deltool_btn, 2, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay5->addWidget(w8, 4, 0, 1, 2);
    lay5->setColumnStretch(1, 1);
    ui_vertical_lay->addLayout(lay5, 0);
    resize_frame = new QFrame(nullptr);
    resize_frame->setHidden(true);
    resize_grid = new QGridLayout();
    resize_grid->setContentsMargins(0, 0, 0, 0);
    resize_grid->setHorizontalSpacing(6);
    resize_grid->setVerticalSpacing(6);
    drillresize_label = new FCLabel(nullptr);
    drillresize_label->setText(trUi_(QString::fromUtf8("<b>Resize Tool</b>")));
    drillresize_label->setToolTip(trUi_(QString::fromUtf8("Resize a drill or a selection of drills.")));
    resize_grid->addWidget(drillresize_label, 0, 0, 1, 2);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("Resize Dia:")));
    w9->setToolTip(trUi_(QString::fromUtf8("Diameter to resize to.")));
    resize_grid->addWidget(w9, 2, 0, 1, 1);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    resdrill_entry = new FCDoubleSpinner(nullptr);
    resdrill_entry->setProperty("decimals", 4);
    resdrill_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); resdrill_entry->setSizePolicy(sp); }
    lay10->addWidget(resdrill_entry, 0);
    resize_btn = new FCButton(nullptr);
    resize_btn->setIcon(QIcon(QString::fromUtf8(":/images/resize16.png")));
    resize_btn->setText(trUi_(QString::fromUtf8("Resize")));
    resize_btn->setToolTip(trUi_(QString::fromUtf8("Resize drill(s)")));
    lay10->addWidget(resize_btn, 0);
    resize_grid->addLayout(lay10, 2, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    resize_grid->addWidget(w11, 6, 0, 1, 2);
    resize_grid->setColumnStretch(1, 1);
    resize_frame->setLayout(resize_grid);
    ui_vertical_lay->addWidget(resize_frame, 0);
    array_frame = new QFrame(nullptr);
    array_frame->setHidden(true);
    array_grid = new QGridLayout();
    array_grid->setContentsMargins(0, 0, 0, 0);
    array_grid->setHorizontalSpacing(6);
    array_grid->setVerticalSpacing(6);
    drill_array_label = new FCLabel(nullptr);
    drill_array_label->setText(trUi_(QString::fromUtf8("<b>Add Drill Array</b>")));
    drill_array_label->setToolTip(trUi_(QString::fromUtf8("Add an array of drills (linear or circular array)")));
    array_grid->addWidget(drill_array_label, 0, 0, 1, 2);
    auto* w12 = new FCLabel(nullptr);
    w12->setText(trUi_(QString::fromUtf8("Type:")));
    w12->setToolTip(trUi_(QString::fromUtf8("Select the type of drills array to create.\nIt can be Linear X(Y) or Circular")));
    array_grid->addWidget(w12, 2, 0, 1, 1);
    array_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Linear")), QVariant(QString::fromUtf8("linear"))}, {trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("circular"))}}, Qt::Horizontal, true, nullptr);
    array_grid->addWidget(array_type_radio, 2, 1, 1, 1);
    drill_array_size_label = new FCLabel(nullptr);
    drill_array_size_label->setText(trUi_(QString::fromUtf8("Number:")));
    drill_array_size_label->setToolTip(trUi_(QString::fromUtf8("Specify how many drills to be in the array.")));
    array_grid->addWidget(drill_array_size_label, 4, 0, 1, 1);
    drill_array_size_entry = new FCSpinner(nullptr);
    drill_array_size_entry->setProperty("maximum", 10000);
    drill_array_size_entry->setProperty("minimum", 1);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); drill_array_size_entry->setSizePolicy(sp); }
    drill_array_size_entry->setProperty("value", 1);
    array_grid->addWidget(drill_array_size_entry, 4, 1, 1, 1);
    array_linear_frame = new QFrame(nullptr);
    array_linear_frame->setHidden(true);
    lin_grid = new QGridLayout();
    lin_grid->setContentsMargins(0, 0, 0, 0);
    lin_grid->setHorizontalSpacing(6);
    lin_grid->setVerticalSpacing(6);
    drill_axis_label = new FCLabel(nullptr);
    drill_axis_label->setText(trUi_(QString::fromUtf8("Direction:")));
    drill_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the linear array is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the array inclination")));
    lin_grid->addWidget(drill_axis_label, 0, 0, 1, 1);
    drill_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    lin_grid->addWidget(drill_axis_radio, 0, 1, 1, 1);
    drill_pitch_label = new FCLabel(nullptr);
    drill_pitch_label->setText(trUi_(QString::fromUtf8("Pitch:")));
    drill_pitch_label->setToolTip(trUi_(QString::fromUtf8("Pitch = Distance between elements of the array.")));
    lin_grid->addWidget(drill_pitch_label, 2, 0, 1, 1);
    drill_pitch_entry = new FCDoubleSpinner(nullptr);
    drill_pitch_entry->setProperty("decimals", 4);
    drill_pitch_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); drill_pitch_entry->setSizePolicy(sp); }
    lin_grid->addWidget(drill_pitch_entry, 2, 1, 1, 1);
    linear_angle_label = new FCLabel(nullptr);
    linear_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    linear_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which the linear array is placed.\nThe precision is of max 2 decimals.\nMin value is: -360.00 degrees.\nMax value is: 360.00 degrees.")));
    linear_angle_label->setHidden(true);
    lin_grid->addWidget(linear_angle_label, 4, 0, 1, 1);
    linear_angle_spinner = new FCDoubleSpinner(nullptr);
    linear_angle_spinner->setProperty("decimals", 4);
    linear_angle_spinner->setProperty("maximum", 360.0);
    linear_angle_spinner->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); linear_angle_spinner->setSizePolicy(sp); }
    linear_angle_spinner->setHidden(true);
    lin_grid->addWidget(linear_angle_spinner, 4, 1, 1, 1);
    lin_grid->setColumnStretch(1, 1);
    array_linear_frame->setLayout(lin_grid);
    array_grid->addWidget(array_linear_frame, 6, 0, 1, 2);
    array_circular_frame = new QFrame(nullptr);
    array_circular_frame->setHidden(true);
    circ_grid = new QGridLayout();
    circ_grid->setContentsMargins(0, 0, 0, 0);
    circ_grid->setHorizontalSpacing(6);
    circ_grid->setVerticalSpacing(6);
    drill_array_dir_lbl = new FCLabel(nullptr);
    drill_array_dir_lbl->setText(trUi_(QString::fromUtf8("Direction:")));
    drill_array_dir_lbl->setToolTip(trUi_(QString::fromUtf8("Direction for circular array.\nCan be CW = clockwise or CCW = counter clockwise.")));
    circ_grid->addWidget(drill_array_dir_lbl, 0, 0, 1, 1);
    drill_array_dir_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    circ_grid->addWidget(drill_array_dir_radio, 0, 1, 1, 1);
    drill_array_angle_lbl = new FCLabel(nullptr);
    drill_array_angle_lbl->setText(trUi_(QString::fromUtf8("Angle:")));
    drill_array_angle_lbl->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    circ_grid->addWidget(drill_array_angle_lbl, 2, 0, 1, 1);
    drill_angle_entry = new FCDoubleSpinner(nullptr);
    drill_angle_entry->setProperty("decimals", 4);
    drill_angle_entry->setProperty("maximum", 360.0);
    drill_angle_entry->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); drill_angle_entry->setSizePolicy(sp); }
    circ_grid->addWidget(drill_angle_entry, 2, 1, 1, 1);
    circ_grid->setColumnStretch(1, 1);
    array_circular_frame->setLayout(circ_grid);
    array_grid->addWidget(array_circular_frame, 8, 0, 1, 2);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    array_grid->addWidget(w13, 10, 0, 1, 2);
    array_grid->setColumnStretch(1, 1);
    array_frame->setLayout(array_grid);
    ui_vertical_lay->addWidget(array_frame, 0);
    slot_frame = new QFrame(nullptr);
    slot_frame->setHidden(true);
    slot_grid = new QGridLayout();
    slot_grid->setContentsMargins(0, 0, 0, 0);
    slot_grid->setHorizontalSpacing(6);
    slot_grid->setVerticalSpacing(6);
    slot_label = new FCLabel(nullptr);
    slot_label->setText(trUi_(QString::fromUtf8("<b>Slot Parameters</b>")));
    slot_label->setToolTip(trUi_(QString::fromUtf8("Parameters for adding a slot (hole with oval shape)\neither single or as an part of an array.")));
    slot_grid->addWidget(slot_label, 0, 0, 1, 2);
    slot_length_label = new FCLabel(nullptr);
    slot_length_label->setText(trUi_(QString::fromUtf8("Length:")));
    slot_length_label->setToolTip(trUi_(QString::fromUtf8("Length. The length of the slot.")));
    slot_grid->addWidget(slot_length_label, 2, 0, 1, 1);
    slot_length_entry = new FCDoubleSpinner(nullptr);
    slot_length_entry->setProperty("decimals", 4);
    slot_length_entry->setProperty("maximum", 10000.0);
    slot_length_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_length_entry->setSizePolicy(sp); }
    slot_grid->addWidget(slot_length_entry, 2, 1, 1, 1);
    slot_axis_label = new FCLabel(nullptr);
    slot_axis_label->setText(trUi_(QString::fromUtf8("Direction:")));
    slot_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the slot is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the slot inclination")));
    slot_grid->addWidget(slot_axis_label, 4, 0, 1, 1);
    slot_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    slot_grid->addWidget(slot_axis_radio, 4, 1, 1, 1);
    slot_angle_label = new FCLabel(nullptr);
    slot_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    slot_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which the slot is placed.\nThe precision is of max 2 decimals.\nMin value is: -360.00 degrees.\nMax value is: 360.00 degrees.")));
    slot_grid->addWidget(slot_angle_label, 6, 0, 1, 1);
    slot_angle_spinner = new FCDoubleSpinner(nullptr);
    slot_angle_spinner->setProperty("decimals", 4);
    slot_angle_spinner->setProperty("maximum", 360.0);
    slot_angle_spinner->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_angle_spinner->setSizePolicy(sp); }
    slot_angle_spinner->setWrapping(true);
    slot_grid->addWidget(slot_angle_spinner, 6, 1, 1, 1);
    auto* w14 = new QFrame(nullptr);
    w14->setProperty("frameShadow", 48);
    w14->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w14->setSizePolicy(sp); }
    slot_grid->addWidget(w14, 8, 0, 1, 2);
    slot_grid->setColumnStretch(1, 1);
    slot_frame->setLayout(slot_grid);
    ui_vertical_lay->addWidget(slot_frame, 0);
    slot_array_frame = new QFrame(nullptr);
    slot_array_frame->setHidden(true);
    slot_array_grid = new QGridLayout();
    slot_array_grid->setContentsMargins(0, 0, 0, 0);
    slot_array_grid->setHorizontalSpacing(6);
    slot_array_grid->setVerticalSpacing(6);
    slot_array_label = new FCLabel(nullptr);
    slot_array_label->setText(trUi_(QString::fromUtf8("<b>Slot Array Parameters</b>")));
    slot_array_label->setToolTip(trUi_(QString::fromUtf8("Parameters for the array of slots (linear or circular array)")));
    slot_array_grid->addWidget(slot_array_label, 0, 0, 1, 2);
    auto* w15 = new FCLabel(nullptr);
    w15->setText(trUi_(QString::fromUtf8("Type:")));
    w15->setToolTip(trUi_(QString::fromUtf8("Select the type of slot array to create.\nIt can be Linear X(Y) or Circular")));
    slot_array_grid->addWidget(w15, 2, 0, 1, 1);
    slot_array_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Linear")), QVariant(QString::fromUtf8("linear"))}, {trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("circular"))}}, Qt::Horizontal, true, nullptr);
    slot_array_grid->addWidget(slot_array_type_radio, 2, 1, 1, 1);
    slot_array_size_label = new FCLabel(nullptr);
    slot_array_size_label->setText(trUi_(QString::fromUtf8("Number:")));
    slot_array_size_label->setToolTip(trUi_(QString::fromUtf8("Specify how many slots to be in the array.")));
    slot_array_grid->addWidget(slot_array_size_label, 4, 0, 1, 1);
    slot_array_size_entry = new FCSpinner(nullptr);
    slot_array_size_entry->setProperty("maximum", 10000);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_size_entry->setSizePolicy(sp); }
    slot_array_grid->addWidget(slot_array_size_entry, 4, 1, 1, 1);
    slot_array_linear_frame = new QFrame(nullptr);
    slot_array_lin_grid = new QGridLayout();
    slot_array_lin_grid->setContentsMargins(0, 0, 0, 0);
    slot_array_lin_grid->setHorizontalSpacing(6);
    slot_array_lin_grid->setVerticalSpacing(6);
    slot_array_axis_label = new FCLabel(nullptr);
    slot_array_axis_label->setText(trUi_(QString::fromUtf8("Direction:")));
    slot_array_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the linear array is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the array inclination")));
    slot_array_lin_grid->addWidget(slot_array_axis_label, 0, 0, 1, 1);
    slot_array_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    slot_array_lin_grid->addWidget(slot_array_axis_radio, 0, 1, 1, 1);
    slot_array_pitch_label = new FCLabel(nullptr);
    slot_array_pitch_label->setText(trUi_(QString::fromUtf8("Pitch:")));
    slot_array_pitch_label->setToolTip(trUi_(QString::fromUtf8("Pitch = Distance between elements of the array.")));
    slot_array_lin_grid->addWidget(slot_array_pitch_label, 2, 0, 1, 1);
    slot_array_pitch_entry = new FCDoubleSpinner(nullptr);
    slot_array_pitch_entry->setProperty("decimals", 4);
    slot_array_pitch_entry->setProperty("maximum", 10000.0);
    slot_array_pitch_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_pitch_entry->setSizePolicy(sp); }
    slot_array_lin_grid->addWidget(slot_array_pitch_entry, 2, 1, 1, 1);
    slot_array_linear_angle_label = new FCLabel(nullptr);
    slot_array_linear_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    slot_array_linear_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which the linear array is placed.\nThe precision is of max 2 decimals.\nMin value is: -360.00 degrees.\nMax value is: 360.00 degrees.")));
    slot_array_linear_angle_label->setHidden(true);
    slot_array_lin_grid->addWidget(slot_array_linear_angle_label, 4, 0, 1, 1);
    slot_array_linear_angle_spinner = new FCDoubleSpinner(nullptr);
    slot_array_linear_angle_spinner->setProperty("decimals", 4);
    slot_array_linear_angle_spinner->setProperty("maximum", 360.0);
    slot_array_linear_angle_spinner->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_linear_angle_spinner->setSizePolicy(sp); }
    slot_array_linear_angle_spinner->setHidden(true);
    slot_array_lin_grid->addWidget(slot_array_linear_angle_spinner, 4, 1, 1, 1);
    slot_array_lin_grid->setColumnStretch(1, 1);
    slot_array_linear_frame->setLayout(slot_array_lin_grid);
    slot_array_grid->addWidget(slot_array_linear_frame, 6, 0, 1, 2);
    slot_array_circular_frame = new QFrame(nullptr);
    slot_array_circ_grid = new QGridLayout();
    slot_array_circ_grid->setContentsMargins(0, 0, 0, 0);
    slot_array_circ_grid->setHorizontalSpacing(6);
    slot_array_circ_grid->setVerticalSpacing(6);
    slot_array_direction_label = new FCLabel(nullptr);
    slot_array_direction_label->setText(trUi_(QString::fromUtf8("Direction:")));
    slot_array_direction_label->setToolTip(trUi_(QString::fromUtf8("Direction for circular array.\nCan be CW = clockwise or CCW = counter clockwise.")));
    slot_array_circ_grid->addWidget(slot_array_direction_label, 0, 0, 1, 1);
    slot_array_direction_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    slot_array_circ_grid->addWidget(slot_array_direction_radio, 0, 1, 1, 1);
    slot_array_angle_label = new FCLabel(nullptr);
    slot_array_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    slot_array_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    slot_array_circ_grid->addWidget(slot_array_angle_label, 2, 0, 1, 1);
    slot_array_angle_entry = new FCDoubleSpinner(nullptr);
    slot_array_angle_entry->setProperty("decimals", 4);
    slot_array_angle_entry->setProperty("maximum", 360.0);
    slot_array_angle_entry->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_angle_entry->setSizePolicy(sp); }
    slot_array_circ_grid->addWidget(slot_array_angle_entry, 2, 1, 1, 1);
    slot_array_circ_grid->setColumnStretch(1, 1);
    slot_array_circular_frame->setLayout(slot_array_circ_grid);
    slot_array_grid->addWidget(slot_array_circular_frame, 8, 0, 1, 2);
    slot_array_grid->setColumnStretch(1, 1);
    slot_array_frame->setLayout(slot_array_grid);
    ui_vertical_lay->addWidget(slot_array_frame, 0);
    ui_vertical_lay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    drills_frame->setLayout(ui_vertical_lay);
    lay1->addWidget(drills_frame, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->setStretch(lay1->count() - 1, 1);
    exit_editor_button = new FCButton(nullptr);
    { QFont f = exit_editor_button->font(); f.setBold(true); exit_editor_button->setFont(f); }
    exit_editor_button->setIcon(QIcon(QString::fromUtf8(":/images/power16.png")));
    exit_editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    exit_editor_button->setText(trUi_(QString::fromUtf8("Exit Editor")));
    exit_editor_button->setToolTip(trUi_(QString::fromUtf8("Exit from Editor.")));
    lay1->addWidget(exit_editor_button, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> ExcEditorUIBase::membersByName() const {
    return {
        {"addtool_btn", addtool_btn},
        {"addtool_entry", addtool_entry},
        {"addtool_label", addtool_label},
        {"array_circular_frame", array_circular_frame},
        {"array_frame", array_frame},
        {"array_grid", array_grid},
        {"array_linear_frame", array_linear_frame},
        {"array_type_radio", array_type_radio},
        {"circ_grid", circ_grid},
        {"convert_slots_btn", convert_slots_btn},
        {"deltool_btn", deltool_btn},
        {"drill_angle_entry", drill_angle_entry},
        {"drill_array_angle_lbl", drill_array_angle_lbl},
        {"drill_array_dir_lbl", drill_array_dir_lbl},
        {"drill_array_dir_radio", drill_array_dir_radio},
        {"drill_array_label", drill_array_label},
        {"drill_array_size_entry", drill_array_size_entry},
        {"drill_array_size_label", drill_array_size_label},
        {"drill_axis_label", drill_axis_label},
        {"drill_axis_radio", drill_axis_radio},
        {"drill_pitch_entry", drill_pitch_entry},
        {"drill_pitch_label", drill_pitch_label},
        {"drillresize_label", drillresize_label},
        {"drills_frame", drills_frame},
        {"exit_editor_button", exit_editor_button},
        {"icon", icon},
        {"lin_grid", lin_grid},
        {"linear_angle_label", linear_angle_label},
        {"linear_angle_spinner", linear_angle_spinner},
        {"name_box", name_box},
        {"name_entry", name_entry},
        {"resdrill_entry", resdrill_entry},
        {"resize_btn", resize_btn},
        {"resize_frame", resize_frame},
        {"resize_grid", resize_grid},
        {"slot_angle_label", slot_angle_label},
        {"slot_angle_spinner", slot_angle_spinner},
        {"slot_array_angle_entry", slot_array_angle_entry},
        {"slot_array_angle_label", slot_array_angle_label},
        {"slot_array_axis_label", slot_array_axis_label},
        {"slot_array_axis_radio", slot_array_axis_radio},
        {"slot_array_circ_grid", slot_array_circ_grid},
        {"slot_array_circular_frame", slot_array_circular_frame},
        {"slot_array_direction_label", slot_array_direction_label},
        {"slot_array_direction_radio", slot_array_direction_radio},
        {"slot_array_frame", slot_array_frame},
        {"slot_array_grid", slot_array_grid},
        {"slot_array_label", slot_array_label},
        {"slot_array_lin_grid", slot_array_lin_grid},
        {"slot_array_linear_angle_label", slot_array_linear_angle_label},
        {"slot_array_linear_angle_spinner", slot_array_linear_angle_spinner},
        {"slot_array_linear_frame", slot_array_linear_frame},
        {"slot_array_pitch_entry", slot_array_pitch_entry},
        {"slot_array_pitch_label", slot_array_pitch_label},
        {"slot_array_size_entry", slot_array_size_entry},
        {"slot_array_size_label", slot_array_size_label},
        {"slot_array_type_radio", slot_array_type_radio},
        {"slot_axis_label", slot_axis_label},
        {"slot_axis_radio", slot_axis_radio},
        {"slot_frame", slot_frame},
        {"slot_grid", slot_grid},
        {"slot_label", slot_label},
        {"slot_length_entry", slot_length_entry},
        {"slot_length_label", slot_length_label},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tools_table_exc", tools_table_exc},
        {"tools_table_label", tools_table_label},
        {"ui_vertical_lay", ui_vertical_lay},
    };
}

}  // namespace flatcam::gui
