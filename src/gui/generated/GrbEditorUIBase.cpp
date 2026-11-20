// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GrbEditorUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void GrbEditorUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Gerber Editor</b></font>")));
    title_box->addWidget(title_label, 1);
    lay1->addLayout(title_box, 0);
    name_box = new QHBoxLayout();
    name_box->setContentsMargins(0, 0, 0, 0);
    name_box->setSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(trUi_(QString::fromUtf8("Name:")));
    name_box->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_box->addWidget(name_entry, 0);
    lay1->addLayout(name_box, 0);
    custom_box = new QVBoxLayout();
    custom_box->setContentsMargins(0, 0, 0, 0);
    custom_box->setSpacing(6);
    apertures_table_label = new FCLabel(nullptr);
    apertures_table_label->setText(trUi_(QString::fromUtf8("<b>Apertures:</b>")));
    apertures_table_label->setToolTip(trUi_(QString::fromUtf8("Apertures Table for the Gerber Object.")));
    custom_box->addWidget(apertures_table_label, 0);
    apertures_table = new FCTable(nullptr);
    apertures_table->setProperty("columnCount", 5);
    apertures_table->setProperty("selectionBehavior", 1);
    apertures_table->setColumnCount(5);
    apertures_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Code")), trUi_(QString::fromUtf8("Type")), trUi_(QString::fromUtf8("Size")), trUi_(QString::fromUtf8("Dim"))});
    custom_box->addWidget(apertures_table, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    custom_box->addWidget(w3, 0);
    apertures_frame = new QFrame(nullptr);
    apertures_box = new QVBoxLayout();
    apertures_box->setContentsMargins(0, 0, 0, 0);
    apertures_box->setSpacing(6);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(trUi_(QString::fromUtf8("<b>Add/Delete Aperture:</b>")));
    w5->setToolTip(trUi_(QString::fromUtf8("Add/Delete an aperture in the aperture table")));
    lay4->addWidget(w5, 0, 0, 1, 2);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(trUi_(QString::fromUtf8("Aperture Code:")));
    w6->setToolTip(trUi_(QString::fromUtf8("Code for the new aperture")));
    lay4->addWidget(w6, 1, 0, 1, 1);
    apcode_entry = new FCSpinner(nullptr);
    apcode_entry->setProperty("maximum", 1000);
    apcode_entry->setWrapping(true);
    lay4->addWidget(apcode_entry, 1, 1, 1, 1);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(trUi_(QString::fromUtf8("Aperture Size:")));
    w7->setToolTip(trUi_(QString::fromUtf8("Size for the new aperture.\nIf aperture type is 'R' or 'O' then\nthis value is automatically\ncalculated as:\nsqrt(width**2 + height**2)")));
    lay4->addWidget(w7, 2, 0, 1, 1);
    apsize_entry = new FCDoubleSpinner(nullptr);
    apsize_entry->setProperty("decimals", 4);
    apsize_entry->setProperty("maximum", 10000.0);
    apsize_entry->setProperty("value", 0.8);
    lay4->addWidget(apsize_entry, 2, 1, 1, 1);
    auto* w8 = new FCLabel(nullptr);
    w8->setText(trUi_(QString::fromUtf8("Aperture Type:")));
    w8->setToolTip(trUi_(QString::fromUtf8("Select the type of new aperture. Can be:\nC = circular\nR = rectangular\nO = oblong")));
    lay4->addWidget(w8, 3, 0, 1, 1);
    aptype_cb = new FCComboBox(nullptr);
    aptype_cb->setProperty("currentText", trUi_(QString::fromUtf8("C")));
    aptype_cb->addItem(trUi_(QString::fromUtf8("C")));
    aptype_cb->addItem(trUi_(QString::fromUtf8("R")));
    aptype_cb->addItem(trUi_(QString::fromUtf8("O")));
    lay4->addWidget(aptype_cb, 3, 1, 1, 1);
    apdim_lbl = new FCLabel(nullptr);
    apdim_lbl->setText(trUi_(QString::fromUtf8("Aperture Dim:")));
    apdim_lbl->setToolTip(trUi_(QString::fromUtf8("Dimensions for the new aperture.\nActive only for rectangular apertures (type R).\nThe format is (width, height)")));
    apdim_lbl->setHidden(true);
    lay4->addWidget(apdim_lbl, 4, 0, 1, 1);
    apdim_entry = new QLineEdit(nullptr);
    apdim_entry->setText(trUi_(QString::fromUtf8("0.5, 0.5")));
    apdim_entry->setHidden(true);
    lay4->addWidget(apdim_entry, 4, 1, 1, 1);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay4->addWidget(w9, 6, 0, 1, 2);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    addaperture_btn = new FCButton(nullptr);
    addaperture_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    addaperture_btn->setText(trUi_(QString::fromUtf8("Add")));
    addaperture_btn->setToolTip(trUi_(QString::fromUtf8("Add a new aperture to the aperture list.")));
    lay10->addWidget(addaperture_btn, 0);
    delaperture_btn = new FCButton(nullptr);
    delaperture_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    delaperture_btn->setText(trUi_(QString::fromUtf8("Delete")));
    delaperture_btn->setToolTip(trUi_(QString::fromUtf8("Delete a aperture in the aperture list")));
    lay10->addWidget(delaperture_btn, 0);
    lay4->addLayout(lay10, 8, 0, 1, 2);
    lay4->setColumnStretch(1, 1);
    apertures_box->addLayout(lay4, 0);
    apertures_frame->setLayout(apertures_box);
    custom_box->addWidget(apertures_frame, 0);
    buffer_tool_frame = new QFrame(nullptr);
    buffer_tool_frame->setHidden(true);
    buffer_tools_box = new QVBoxLayout();
    buffer_tools_box->setContentsMargins(0, 0, 0, 0);
    buffer_tools_box->setSpacing(6);
    auto* w11 = new FCLabel(nullptr);
    w11->setText(trUi_(QString::fromUtf8("<b>Buffer Aperture:</b>")));
    w11->setToolTip(trUi_(QString::fromUtf8("Buffer a aperture in the aperture list")));
    buffer_tools_box->addWidget(w11, 0);
    auto* lay12 = new QFormLayout();
    lay12->setContentsMargins(0, 0, 0, 0);
    lay12->setHorizontalSpacing(6);
    lay12->setVerticalSpacing(6);
    auto* w13 = new QLabel(nullptr);
    w13->setText(trUi_(QString::fromUtf8("Buffer distance:")));
    lay12->setWidget(0, QFormLayout::ItemRole(0), w13);
    buffer_distance_entry = new FCDoubleSpinner(nullptr);
    buffer_distance_entry->setProperty("decimals", 4);
    buffer_distance_entry->setProperty("maximum", 10000.0);
    buffer_distance_entry->setProperty("minimum", -10000.0);
    buffer_distance_entry->setProperty("value", 0.1);
    lay12->setWidget(0, QFormLayout::ItemRole(1), buffer_distance_entry);
    buffer_corner_lbl = new FCLabel(nullptr);
    buffer_corner_lbl->setText(trUi_(QString::fromUtf8("Buffer corner:")));
    buffer_corner_lbl->setToolTip(trUi_(QString::fromUtf8("There are 3 types of corners:\n - 'Round': the corner is rounded.\n - 'Square': the corner is met in a sharp angle.\n - 'Beveled': the corner is a line that directly connects the features meeting in the corner")));
    lay12->setWidget(1, QFormLayout::ItemRole(0), buffer_corner_lbl);
    buffer_corner_cb = new FCComboBox(nullptr);
    buffer_corner_cb->setProperty("currentText", trUi_(QString::fromUtf8("Round")));
    buffer_corner_cb->addItem(trUi_(QString::fromUtf8("Round")));
    buffer_corner_cb->addItem(trUi_(QString::fromUtf8("Square")));
    buffer_corner_cb->addItem(trUi_(QString::fromUtf8("Beveled")));
    lay12->setWidget(1, QFormLayout::ItemRole(1), buffer_corner_cb);
    auto* w14 = new QFrame(nullptr);
    w14->setProperty("frameShadow", 48);
    w14->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w14->setSizePolicy(sp); }
    lay12->setWidget(2, QFormLayout::ItemRole(2), w14);
    buffer_tools_box->addLayout(lay12, 0);
    auto* lay15 = new QHBoxLayout();
    lay15->setContentsMargins(0, 0, 0, 0);
    lay15->setSpacing(6);
    buffer_button = new FCButton(nullptr);
    buffer_button->setIcon(QIcon(QString::fromUtf8(":/images/buffer16-2.png")));
    buffer_button->setText(trUi_(QString::fromUtf8("Buffer")));
    lay15->addWidget(buffer_button, 0);
    buffer_tools_box->addLayout(lay15, 0);
    buffer_tool_frame->setLayout(buffer_tools_box);
    custom_box->addWidget(buffer_tool_frame, 0);
    scale_tool_frame = new QFrame(nullptr);
    scale_tool_frame->setHidden(true);
    scale_tools_box = new QVBoxLayout();
    scale_tools_box->setContentsMargins(0, 0, 0, 0);
    scale_tools_box->setSpacing(6);
    auto* w16 = new FCLabel(nullptr);
    w16->setText(trUi_(QString::fromUtf8("<b>Scale Aperture:</b>")));
    w16->setToolTip(trUi_(QString::fromUtf8("Scale a aperture in the aperture list")));
    scale_tools_box->addWidget(w16, 0);
    auto* lay17 = new QFormLayout();
    lay17->setContentsMargins(0, 0, 0, 0);
    lay17->setHorizontalSpacing(6);
    lay17->setVerticalSpacing(6);
    scale_factor_lbl = new FCLabel(nullptr);
    scale_factor_lbl->setText(trUi_(QString::fromUtf8("Scale factor:")));
    scale_factor_lbl->setToolTip(trUi_(QString::fromUtf8("The factor by which to scale the selected aperture.\nValues can be between 0.0000 and 999.9999")));
    lay17->setWidget(0, QFormLayout::ItemRole(0), scale_factor_lbl);
    scale_factor_entry = new FCDoubleSpinner(nullptr);
    scale_factor_entry->setProperty("decimals", 4);
    scale_factor_entry->setProperty("maximum", 10000.0);
    scale_factor_entry->setProperty("value", 1.0);
    lay17->setWidget(0, QFormLayout::ItemRole(1), scale_factor_entry);
    auto* w18 = new QFrame(nullptr);
    w18->setProperty("frameShadow", 48);
    w18->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w18->setSizePolicy(sp); }
    lay17->setWidget(1, QFormLayout::ItemRole(2), w18);
    scale_tools_box->addLayout(lay17, 0);
    auto* lay19 = new QHBoxLayout();
    lay19->setContentsMargins(0, 0, 0, 0);
    lay19->setSpacing(6);
    scale_button = new FCButton(nullptr);
    scale_button->setIcon(QIcon(QString::fromUtf8(":/images/clean32.png")));
    scale_button->setText(trUi_(QString::fromUtf8("Scale")));
    lay19->addWidget(scale_button, 0);
    scale_tools_box->addLayout(lay19, 0);
    scale_tool_frame->setLayout(scale_tools_box);
    custom_box->addWidget(scale_tool_frame, 0);
    ma_tool_frame = new QFrame(nullptr);
    ma_tool_frame->setHidden(true);
    ma_tools_box = new QVBoxLayout();
    ma_tools_box->setContentsMargins(0, 0, 0, 0);
    ma_tools_box->setSpacing(6);
    auto* w20 = new QFrame(nullptr);
    w20->setProperty("frameShadow", 48);
    w20->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w20->setSizePolicy(sp); }
    ma_tools_box->addWidget(w20, 0);
    auto* w21 = new FCLabel(nullptr);
    w21->setText(trUi_(QString::fromUtf8("<b>Mark polygons:</b>")));
    w21->setToolTip(trUi_(QString::fromUtf8("Mark the polygon areas.")));
    ma_tools_box->addWidget(w21, 0);
    auto* lay22 = new QFormLayout();
    lay22->setContentsMargins(0, 0, 0, 0);
    lay22->setHorizontalSpacing(6);
    lay22->setVerticalSpacing(6);
    ma_lower_threshold_lbl = new FCLabel(nullptr);
    ma_lower_threshold_lbl->setText(trUi_(QString::fromUtf8("Area LOWER threshold:")));
    ma_lower_threshold_lbl->setToolTip(trUi_(QString::fromUtf8("The threshold value, all areas more than this are marked.\nCan have a value between 0.0000 and 10000.0000")));
    lay22->setWidget(0, QFormLayout::ItemRole(0), ma_lower_threshold_lbl);
    ma_lower_threshold_entry = new FCDoubleSpinner(nullptr);
    ma_lower_threshold_entry->setProperty("decimals", 4);
    ma_lower_threshold_entry->setProperty("maximum", 10000.0);
    lay22->setWidget(0, QFormLayout::ItemRole(1), ma_lower_threshold_entry);
    ma_upper_threshold_lbl = new FCLabel(nullptr);
    ma_upper_threshold_lbl->setText(trUi_(QString::fromUtf8("Area UPPER threshold:")));
    ma_upper_threshold_lbl->setToolTip(trUi_(QString::fromUtf8("The threshold value, all areas less than this are marked.\nCan have a value between 0.0000 and 10000.0000")));
    lay22->setWidget(1, QFormLayout::ItemRole(0), ma_upper_threshold_lbl);
    ma_upper_threshold_entry = new FCDoubleSpinner(nullptr);
    ma_upper_threshold_entry->setProperty("decimals", 4);
    ma_upper_threshold_entry->setProperty("maximum", 10000.0);
    ma_upper_threshold_entry->setProperty("value", 1.0);
    lay22->setWidget(1, QFormLayout::ItemRole(1), ma_upper_threshold_entry);
    ma_tools_box->addLayout(lay22, 0);
    auto* lay23 = new QHBoxLayout();
    lay23->setContentsMargins(0, 0, 0, 0);
    lay23->setSpacing(6);
    ma_threshold_button = new FCButton(nullptr);
    ma_threshold_button->setIcon(QIcon(QString::fromUtf8(":/images/markarea32.png")));
    ma_threshold_button->setText(trUi_(QString::fromUtf8("Mark")));
    ma_threshold_button->setToolTip(trUi_(QString::fromUtf8("Mark the polygons that fit within limits.")));
    lay23->addWidget(ma_threshold_button, 0);
    ma_delete_button = new FCButton(nullptr);
    ma_delete_button->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    ma_delete_button->setText(trUi_(QString::fromUtf8("Delete")));
    ma_delete_button->setToolTip(trUi_(QString::fromUtf8("Delete all the marked polygons.")));
    lay23->addWidget(ma_delete_button, 0);
    ma_clear_button = new FCButton(nullptr);
    ma_clear_button->setIcon(QIcon(QString::fromUtf8(":/images/clean32.png")));
    ma_clear_button->setText(trUi_(QString::fromUtf8("Clear")));
    ma_clear_button->setToolTip(trUi_(QString::fromUtf8("Clear all the markings.")));
    lay23->addWidget(ma_clear_button, 0);
    ma_tools_box->addLayout(lay23, 0);
    ma_tool_frame->setLayout(ma_tools_box);
    custom_box->addWidget(ma_tool_frame, 0);
    array_frame = new QFrame(nullptr);
    array_box = new QVBoxLayout();
    array_box->setContentsMargins(0, 0, 0, 0);
    array_box->setSpacing(6);
    auto* w24 = new QFrame(nullptr);
    w24->setProperty("frameShadow", 48);
    w24->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w24->setSizePolicy(sp); }
    array_box->addWidget(w24, 0);
    auto* lay25 = new QGridLayout();
    lay25->setContentsMargins(0, 0, 0, 0);
    lay25->setHorizontalSpacing(6);
    lay25->setVerticalSpacing(6);
    padarray_label = new FCLabel(nullptr);
    padarray_label->setText(trUi_(QString::fromUtf8("<b>Add Pad Array</b>")));
    padarray_label->setToolTip(trUi_(QString::fromUtf8("Add an array of pads (linear or circular array)")));
    lay25->addWidget(padarray_label, 0, 0, 1, 2);
    auto* w26 = new FCLabel(nullptr);
    w26->setText(trUi_(QString::fromUtf8("Type:")));
    w26->setToolTip(trUi_(QString::fromUtf8("Select the type of pads array to create.\nIt can be Linear X(Y) or Circular")));
    lay25->addWidget(w26, 2, 0, 1, 1);
    array_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Linear")), QVariant(QString::fromUtf8("linear"))}, {trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("circular"))}}, Qt::Horizontal, true, nullptr);
    array_type_radio->setValue(QVariant(QString::fromUtf8("linear")));
    lay25->addWidget(array_type_radio, 2, 1, 1, 1);
    auto* w27 = new FCLabel(nullptr);
    w27->setText(trUi_(QString::fromUtf8("Nr of pads:")));
    w27->setToolTip(trUi_(QString::fromUtf8("Specify how many pads to be in the array.")));
    lay25->addWidget(w27, 4, 0, 1, 1);
    pad_array_size_entry = new FCSpinner(nullptr);
    pad_array_size_entry->setProperty("maximum", 10000);
    pad_array_size_entry->setProperty("minimum", 1);
    pad_array_size_entry->setProperty("value", 5);
    lay25->addWidget(pad_array_size_entry, 4, 1, 1, 1);
    lin_separator_line = new QFrame(nullptr);
    lin_separator_line->setProperty("frameShadow", 48);
    lin_separator_line->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); lin_separator_line->setSizePolicy(sp); }
    lay25->addWidget(lin_separator_line, 6, 0, 1, 2);
    pad_axis_label = new FCLabel(nullptr);
    pad_axis_label->setText(trUi_(QString::fromUtf8("Direction:")));
    pad_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the linear array is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the array inclination")));
    lay25->addWidget(pad_axis_label, 8, 0, 1, 1);
    pad_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    pad_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay25->addWidget(pad_axis_radio, 8, 1, 1, 1);
    pad_pitch_label = new FCLabel(nullptr);
    pad_pitch_label->setText(trUi_(QString::fromUtf8("Pitch:")));
    pad_pitch_label->setToolTip(trUi_(QString::fromUtf8("Pitch = Distance between elements of the array.")));
    lay25->addWidget(pad_pitch_label, 10, 0, 1, 1);
    pad_pitch_entry = new FCDoubleSpinner(nullptr);
    pad_pitch_entry->setProperty("decimals", 4);
    pad_pitch_entry->setProperty("maximum", 10000.0);
    pad_pitch_entry->setProperty("singleStep", 0.1);
    pad_pitch_entry->setProperty("value", 0.1);
    lay25->addWidget(pad_pitch_entry, 10, 1, 1, 1);
    linear_angle_label = new FCLabel(nullptr);
    linear_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    linear_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which the linear array is placed.\nThe precision is of max 2 decimals.\nMin value is: -360.00 degrees.\nMax value is: 360.00 degrees.")));
    linear_angle_label->setHidden(true);
    lay25->addWidget(linear_angle_label, 12, 0, 1, 1);
    linear_angle_spinner = new FCDoubleSpinner(nullptr);
    linear_angle_spinner->setProperty("decimals", 4);
    linear_angle_spinner->setProperty("maximum", 360.0);
    linear_angle_spinner->setProperty("minimum", -360.0);
    linear_angle_spinner->setHidden(true);
    lay25->addWidget(linear_angle_spinner, 12, 1, 1, 1);
    circ_separator_line = new QFrame(nullptr);
    circ_separator_line->setProperty("frameShadow", 48);
    circ_separator_line->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); circ_separator_line->setSizePolicy(sp); }
    circ_separator_line->setHidden(true);
    lay25->addWidget(circ_separator_line, 14, 0, 1, 2);
    pad_direction_label = new FCLabel(nullptr);
    pad_direction_label->setText(trUi_(QString::fromUtf8("Direction:")));
    pad_direction_label->setToolTip(trUi_(QString::fromUtf8("Direction for circular array.\nCan be CW = clockwise or CCW = counter clockwise.")));
    pad_direction_label->setHidden(true);
    lay25->addWidget(pad_direction_label, 16, 0, 1, 1);
    pad_direction_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    pad_direction_radio->setHidden(true);
    pad_direction_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay25->addWidget(pad_direction_radio, 16, 1, 1, 1);
    pad_angle_label = new FCLabel(nullptr);
    pad_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    pad_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    pad_angle_label->setHidden(true);
    lay25->addWidget(pad_angle_label, 18, 0, 1, 1);
    pad_angle_entry = new FCDoubleSpinner(nullptr);
    pad_angle_entry->setProperty("decimals", 4);
    pad_angle_entry->setProperty("maximum", 360.0);
    pad_angle_entry->setProperty("minimum", -360.0);
    pad_angle_entry->setProperty("singleStep", 0.1);
    pad_angle_entry->setHidden(true);
    lay25->addWidget(pad_angle_entry, 18, 1, 1, 1);
    lay25->setColumnStretch(1, 1);
    array_box->addLayout(lay25, 0);
    array_frame->setLayout(array_box);
    custom_box->addWidget(array_frame, 0);
    custom_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->addLayout(custom_box, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    exit_editor_button = new FCButton(nullptr);
    { QFont f = exit_editor_button->font(); f.setBold(true); exit_editor_button->setFont(f); }
    exit_editor_button->setIcon(QIcon(QString::fromUtf8(":/images/power16.png")));
    exit_editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t\t\t  "));
    exit_editor_button->setText(trUi_(QString::fromUtf8("Exit Editor")));
    exit_editor_button->setToolTip(trUi_(QString::fromUtf8("Exit from Editor.")));
    lay1->addWidget(exit_editor_button, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> GrbEditorUIBase::membersByName() const {
    return {
        {"addaperture_btn", addaperture_btn},
        {"apcode_entry", apcode_entry},
        {"apdim_entry", apdim_entry},
        {"apdim_lbl", apdim_lbl},
        {"apertures_box", apertures_box},
        {"apertures_frame", apertures_frame},
        {"apertures_table", apertures_table},
        {"apertures_table_label", apertures_table_label},
        {"apsize_entry", apsize_entry},
        {"aptype_cb", aptype_cb},
        {"array_box", array_box},
        {"array_frame", array_frame},
        {"array_type_radio", array_type_radio},
        {"buffer_button", buffer_button},
        {"buffer_corner_cb", buffer_corner_cb},
        {"buffer_corner_lbl", buffer_corner_lbl},
        {"buffer_distance_entry", buffer_distance_entry},
        {"buffer_tool_frame", buffer_tool_frame},
        {"buffer_tools_box", buffer_tools_box},
        {"circ_separator_line", circ_separator_line},
        {"custom_box", custom_box},
        {"delaperture_btn", delaperture_btn},
        {"exit_editor_button", exit_editor_button},
        {"icon", icon},
        {"lin_separator_line", lin_separator_line},
        {"linear_angle_label", linear_angle_label},
        {"linear_angle_spinner", linear_angle_spinner},
        {"ma_clear_button", ma_clear_button},
        {"ma_delete_button", ma_delete_button},
        {"ma_lower_threshold_entry", ma_lower_threshold_entry},
        {"ma_lower_threshold_lbl", ma_lower_threshold_lbl},
        {"ma_threshold_button", ma_threshold_button},
        {"ma_tool_frame", ma_tool_frame},
        {"ma_tools_box", ma_tools_box},
        {"ma_upper_threshold_entry", ma_upper_threshold_entry},
        {"ma_upper_threshold_lbl", ma_upper_threshold_lbl},
        {"name_box", name_box},
        {"name_entry", name_entry},
        {"pad_angle_entry", pad_angle_entry},
        {"pad_angle_label", pad_angle_label},
        {"pad_array_size_entry", pad_array_size_entry},
        {"pad_axis_label", pad_axis_label},
        {"pad_axis_radio", pad_axis_radio},
        {"pad_direction_label", pad_direction_label},
        {"pad_direction_radio", pad_direction_radio},
        {"pad_pitch_entry", pad_pitch_entry},
        {"pad_pitch_label", pad_pitch_label},
        {"padarray_label", padarray_label},
        {"scale_button", scale_button},
        {"scale_factor_entry", scale_factor_entry},
        {"scale_factor_lbl", scale_factor_lbl},
        {"scale_tool_frame", scale_tool_frame},
        {"scale_tools_box", scale_tools_box},
        {"title_box", title_box},
        {"title_label", title_label},
    };
}

}  // namespace flatcam::gui
