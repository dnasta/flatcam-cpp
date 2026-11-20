// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ThievingToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void ThievingToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Copper Thieving Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Copper Thieving Tool")));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    grbobj_label = new FCLabel(nullptr);
    grbobj_label->setText(trUi_(QString::fromUtf8("<b>GERBER:</b>")));
    grbobj_label->setToolTip(trUi_(QString::fromUtf8("Gerber Object to which will be added a copper thieving.")));
    lay3->addWidget(grbobj_label, 0, 0, 1, 1);
    grb_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(grb_object_combo, 1, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 2, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    copper_fill_label = new FCLabel(nullptr);
    copper_fill_label->setText(trUi_(QString::fromUtf8("<b>Parameters</b>")));
    copper_fill_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay5->addWidget(copper_fill_label, 0, 0, 1, 2);
    clearance_label = new FCLabel(nullptr);
    clearance_label->setText(trUi_(QString::fromUtf8("Clearance:")));
    clearance_label->setToolTip(trUi_(QString::fromUtf8("This set the distance between the copper thieving components\n(the polygon fill may be split in multiple polygons)\nand the copper traces in the Gerber file.")));
    lay5->addWidget(clearance_label, 2, 0, 1, 1);
    clearance_entry = new FCDoubleSpinner(nullptr);
    clearance_entry->setProperty("decimals", 4);
    clearance_entry->setProperty("maximum", 10000.0);
    clearance_entry->setProperty("singleStep", 0.1);
    clearance_entry->setProperty("value", 0.25);
    lay5->addWidget(clearance_entry, 2, 1, 1, 1);
    margin_label = new FCLabel(nullptr);
    margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay5->addWidget(margin_label, 4, 0, 1, 1);
    margin_entry = new FCDoubleSpinner(nullptr);
    margin_entry->setProperty("decimals", 4);
    margin_entry->setProperty("maximum", 10000.0);
    margin_entry->setProperty("singleStep", 0.1);
    margin_entry->setProperty("value", 1.0);
    lay5->addWidget(margin_entry, 4, 1, 1, 1);
    area_label = new FCLabel(nullptr);
    area_label->setText(trUi_(QString::fromUtf8("Area:")));
    area_label->setToolTip(trUi_(QString::fromUtf8("Thieving areas with area less then this value will not be added.")));
    lay5->addWidget(area_label, 6, 0, 1, 1);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    area_entry = new FCDoubleSpinner(nullptr);
    area_entry->setProperty("decimals", 4);
    area_entry->setProperty("maximum", 10000.0);
    area_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); area_entry->setSizePolicy(sp); }
    area_entry->setProperty("value", 0.1);
    lay6->addWidget(area_entry, 0);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(trUi_(QString::fromUtf8("mm<sup>2</sup>")));
    lay6->addWidget(w7, 0);
    lay5->addLayout(lay6, 6, 1, 1, 1);
    reference_label = new FCLabel(nullptr);
    reference_label->setText(trUi_(QString::fromUtf8("Reference:")));
    reference_label->setToolTip(trUi_(QString::fromUtf8("- 'Itself' - the copper thieving extent is based on the object extent.\n- 'Area Selection' - left mouse click to start selection of the area to be filled.\n- 'Reference Object' - will do copper thieving within the area specified by another object.")));
    lay5->addWidget(reference_label, 8, 0, 1, 1);
    reference_radio = new RadioSet({{trUi_(QString::fromUtf8("Itself")), QVariant(QString::fromUtf8("itself"))}, {trUi_(QString::fromUtf8("Area Selection")), QVariant(QString::fromUtf8("area"))}, {trUi_(QString::fromUtf8("Reference Object")), QVariant(QString::fromUtf8("box"))}}, Qt::Vertical, false, nullptr);
    reference_radio->setValue(QVariant(QString::fromUtf8("itself")));
    lay5->addWidget(reference_radio, 8, 1, 1, 1);
    ref_combo_type_label = new FCLabel(nullptr);
    ref_combo_type_label->setText(trUi_(QString::fromUtf8("Ref. Type:")));
    ref_combo_type_label->setToolTip(trUi_(QString::fromUtf8("The type of FlatCAM object to be used as copper thieving reference.\nIt can be Gerber, Excellon or Geometry.")));
    ref_combo_type_label->setHidden(true);
    lay5->addWidget(ref_combo_type_label, 10, 0, 1, 1);
    ref_combo_type = new FCComboBox(nullptr);
    ref_combo_type->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    ref_combo_type->setHidden(true);
    ref_combo_type->addItem(trUi_(QString::fromUtf8("Gerber")));
    ref_combo_type->addItem(trUi_(QString::fromUtf8("Excellon")));
    ref_combo_type->addItem(trUi_(QString::fromUtf8("Geometry")));
    lay5->addWidget(ref_combo_type, 10, 1, 1, 1);
    ref_combo_label = new FCLabel(nullptr);
    ref_combo_label->setText(trUi_(QString::fromUtf8("Ref. Object:")));
    ref_combo_label->setToolTip(trUi_(QString::fromUtf8("The FlatCAM object to be used as non copper clearing reference.")));
    ref_combo_label->setHidden(true);
    lay5->addWidget(ref_combo_label, 12, 0, 1, 1);
    ref_combo = new FCComboBox(nullptr);
    ref_combo->setHidden(true);
    lay5->addWidget(ref_combo, 12, 1, 1, 1);
    bbox_type_label = new FCLabel(nullptr);
    bbox_type_label->setText(trUi_(QString::fromUtf8("Box Type:")));
    bbox_type_label->setToolTip(trUi_(QString::fromUtf8("- 'Rectangular' - the bounding box will be of rectangular shape.\n- 'Minimal' - the bounding box will be the convex hull shape.")));
    lay5->addWidget(bbox_type_label, 14, 0, 1, 1);
    bbox_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Rectangular")), QVariant(QString::fromUtf8("rect"))}, {trUi_(QString::fromUtf8("Minimal")), QVariant(QString::fromUtf8("min"))}}, Qt::Horizontal, false, nullptr);
    bbox_type_radio->setValue(QVariant(QString::fromUtf8("rect")));
    lay5->addWidget(bbox_type_radio, 14, 1, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay5->addWidget(w8, 16, 0, 1, 2);
    fill_type_label = new FCLabel(nullptr);
    fill_type_label->setText(trUi_(QString::fromUtf8("Fill Type:")));
    fill_type_label->setToolTip(trUi_(QString::fromUtf8("- 'Solid' - copper thieving will be a solid polygon.\n- 'Dots Grid' - the empty area will be filled with a pattern of dots.\n- 'Squares Grid' - the empty area will be filled with a pattern of squares.\n- 'Lines Grid' - the empty area will be filled with a pattern of lines.")));
    lay5->addWidget(fill_type_label, 18, 0, 1, 1);
    fill_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Solid")), QVariant(QString::fromUtf8("solid"))}, {trUi_(QString::fromUtf8("Dots Grid")), QVariant(QString::fromUtf8("dot"))}, {trUi_(QString::fromUtf8("Squares Grid")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Lines Grid")), QVariant(QString::fromUtf8("line"))}}, Qt::Vertical, false, nullptr);
    fill_type_radio->setValue(QVariant(QString::fromUtf8("solid")));
    lay5->addWidget(fill_type_radio, 18, 1, 1, 1);
    lay5->setColumnStretch(1, 1);
    layout->addLayout(lay5, 0);
    dots_frame = new QFrame(nullptr);
    dots_frame->setHidden(true);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    dots_label = new FCLabel(nullptr);
    dots_label->setText(trUi_(QString::fromUtf8("<b>Dots Grid Parameters</b>:")));
    lay9->addWidget(dots_label, 0, 0, 1, 2);
    dotdia_label = new FCLabel(nullptr);
    dotdia_label->setText(trUi_(QString::fromUtf8("Dia:")));
    dotdia_label->setToolTip(trUi_(QString::fromUtf8("Dot diameter in Dots Grid.")));
    lay9->addWidget(dotdia_label, 1, 0, 1, 1);
    dot_dia_entry = new FCDoubleSpinner(nullptr);
    dot_dia_entry->setProperty("decimals", 4);
    dot_dia_entry->setProperty("maximum", 10000.0);
    dot_dia_entry->setProperty("singleStep", 0.1);
    dot_dia_entry->setProperty("value", 1.0);
    lay9->addWidget(dot_dia_entry, 1, 1, 1, 1);
    dotspacing_label = new FCLabel(nullptr);
    dotspacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    dotspacing_label->setToolTip(trUi_(QString::fromUtf8("Distance between each two dots in Dots Grid.")));
    lay9->addWidget(dotspacing_label, 2, 0, 1, 1);
    dot_spacing_entry = new FCDoubleSpinner(nullptr);
    dot_spacing_entry->setProperty("decimals", 4);
    dot_spacing_entry->setProperty("maximum", 10000.0);
    dot_spacing_entry->setProperty("singleStep", 0.1);
    dot_spacing_entry->setProperty("value", 2.0);
    lay9->addWidget(dot_spacing_entry, 2, 1, 1, 1);
    lay9->setColumnStretch(1, 1);
    dots_frame->setLayout(lay9);
    layout->addWidget(dots_frame, 0);
    squares_frame = new QFrame(nullptr);
    squares_frame->setHidden(true);
    auto* lay10 = new QGridLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setHorizontalSpacing(6);
    lay10->setVerticalSpacing(6);
    squares_label = new FCLabel(nullptr);
    squares_label->setText(trUi_(QString::fromUtf8("<b>Squares Grid Parameters</b>:")));
    lay10->addWidget(squares_label, 0, 0, 1, 2);
    square_size_label = new FCLabel(nullptr);
    square_size_label->setText(trUi_(QString::fromUtf8("Size:")));
    square_size_label->setToolTip(trUi_(QString::fromUtf8("Square side size in Squares Grid.")));
    lay10->addWidget(square_size_label, 1, 0, 1, 1);
    square_size_entry = new FCDoubleSpinner(nullptr);
    square_size_entry->setProperty("decimals", 4);
    square_size_entry->setProperty("maximum", 10000.0);
    square_size_entry->setProperty("singleStep", 0.1);
    square_size_entry->setProperty("value", 1.0);
    lay10->addWidget(square_size_entry, 1, 1, 1, 1);
    squares_spacing_label = new FCLabel(nullptr);
    squares_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    squares_spacing_label->setToolTip(trUi_(QString::fromUtf8("Distance between each two squares in Squares Grid.")));
    lay10->addWidget(squares_spacing_label, 2, 0, 1, 1);
    squares_spacing_entry = new FCDoubleSpinner(nullptr);
    squares_spacing_entry->setProperty("decimals", 4);
    squares_spacing_entry->setProperty("maximum", 10000.0);
    squares_spacing_entry->setProperty("singleStep", 0.1);
    squares_spacing_entry->setProperty("value", 2.0);
    lay10->addWidget(squares_spacing_entry, 2, 1, 1, 1);
    lay10->setColumnStretch(1, 1);
    squares_frame->setLayout(lay10);
    layout->addWidget(squares_frame, 0);
    lines_frame = new QFrame(nullptr);
    lines_frame->setHidden(true);
    auto* lay11 = new QGridLayout();
    lay11->setContentsMargins(0, 0, 0, 0);
    lay11->setHorizontalSpacing(6);
    lay11->setVerticalSpacing(6);
    lines_label = new FCLabel(nullptr);
    lines_label->setText(trUi_(QString::fromUtf8("<b>Lines Grid Parameters</b>:")));
    lay11->addWidget(lines_label, 0, 0, 1, 2);
    line_size_label = new FCLabel(nullptr);
    line_size_label->setText(trUi_(QString::fromUtf8("Size:")));
    line_size_label->setToolTip(trUi_(QString::fromUtf8("Line thickness size in Lines Grid.")));
    lay11->addWidget(line_size_label, 1, 0, 1, 1);
    line_size_entry = new FCDoubleSpinner(nullptr);
    line_size_entry->setProperty("decimals", 4);
    line_size_entry->setProperty("maximum", 10000.0);
    line_size_entry->setProperty("singleStep", 0.1);
    line_size_entry->setProperty("value", 0.25);
    lay11->addWidget(line_size_entry, 1, 1, 1, 1);
    lines_spacing_label = new FCLabel(nullptr);
    lines_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    lines_spacing_label->setToolTip(trUi_(QString::fromUtf8("Distance between each two lines in Lines Grid.")));
    lay11->addWidget(lines_spacing_label, 2, 0, 1, 1);
    lines_spacing_entry = new FCDoubleSpinner(nullptr);
    lines_spacing_entry->setProperty("decimals", 4);
    lines_spacing_entry->setProperty("maximum", 10000.0);
    lines_spacing_entry->setProperty("singleStep", 0.1);
    lines_spacing_entry->setProperty("value", 2.0);
    lay11->addWidget(lines_spacing_entry, 2, 1, 1, 1);
    lay11->setColumnStretch(1, 1);
    lines_frame->setLayout(lay11);
    layout->addWidget(lines_frame, 0);
    fill_button = new QPushButton(nullptr);
    { QFont f = fill_button->font(); f.setBold(true); fill_button->setFont(f); }
    fill_button->setIcon(QIcon(QString::fromUtf8(":/images/copperfill32.png")));
    fill_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    fill_button->setText(trUi_(QString::fromUtf8("Insert Copper thieving")));
    fill_button->setToolTip(trUi_(QString::fromUtf8("Will add a polygon (may be split in multiple parts)\nthat will surround the actual Gerber traces at a certain distance.")));
    layout->addWidget(fill_button, 0);
    auto* lay12 = new QGridLayout();
    lay12->setContentsMargins(0, 0, 0, 0);
    lay12->setHorizontalSpacing(6);
    lay12->setVerticalSpacing(6);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    lay12->addWidget(w13, 0, 0, 1, 3);
    auto* w14 = new FCLabel(nullptr);
    lay12->addWidget(w14, 1, 0, 1, 1);
    robber_bar_label = new FCLabel(nullptr);
    robber_bar_label->setText(trUi_(QString::fromUtf8("<b>Robber Bar Parameters</b>")));
    robber_bar_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for the robber bar.\nRobber bar = copper border to help in pattern hole plating.")));
    lay12->addWidget(robber_bar_label, 2, 0, 1, 3);
    rb_margin_label = new FCLabel(nullptr);
    rb_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    rb_margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin for robber bar.")));
    lay12->addWidget(rb_margin_label, 4, 0, 1, 1);
    rb_margin_entry = new FCDoubleSpinner(nullptr);
    rb_margin_entry->setProperty("decimals", 4);
    rb_margin_entry->setProperty("maximum", 10000.0);
    rb_margin_entry->setProperty("minimum", -10000.0);
    rb_margin_entry->setProperty("singleStep", 0.1);
    rb_margin_entry->setProperty("value", 1.0);
    lay12->addWidget(rb_margin_entry, 4, 1, 1, 2);
    rb_thickness_label = new FCLabel(nullptr);
    rb_thickness_label->setText(trUi_(QString::fromUtf8("Thickness:")));
    rb_thickness_label->setToolTip(trUi_(QString::fromUtf8("The robber bar thickness.")));
    lay12->addWidget(rb_thickness_label, 6, 0, 1, 1);
    rb_thickness_entry = new FCDoubleSpinner(nullptr);
    rb_thickness_entry->setProperty("decimals", 4);
    rb_thickness_entry->setProperty("maximum", 10000.0);
    rb_thickness_entry->setProperty("singleStep", 0.1);
    rb_thickness_entry->setProperty("value", 1.0);
    lay12->addWidget(rb_thickness_entry, 6, 1, 1, 2);
    rb_button = new QPushButton(nullptr);
    { QFont f = rb_button->font(); f.setBold(true); rb_button->setFont(f); }
    rb_button->setIcon(QIcon(QString::fromUtf8(":/images/robber32.png")));
    rb_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    rb_button->setText(trUi_(QString::fromUtf8("Insert Robber Bar")));
    rb_button->setToolTip(trUi_(QString::fromUtf8("Will add a polygon with a defined thickness\nthat will surround the actual Gerber object\nat a certain distance.\nRequired when doing holes pattern plating.")));
    lay12->addWidget(rb_button, 8, 0, 1, 3);
    auto* w15 = new QFrame(nullptr);
    w15->setProperty("frameShadow", 48);
    w15->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w15->setSizePolicy(sp); }
    lay12->addWidget(w15, 10, 0, 1, 3);
    patern_mask_label = new FCLabel(nullptr);
    patern_mask_label->setText(trUi_(QString::fromUtf8("<b>Pattern Plating Mask</b>")));
    patern_mask_label->setToolTip(trUi_(QString::fromUtf8("Generate a mask for pattern plating.")));
    lay12->addWidget(patern_mask_label, 12, 0, 1, 3);
    sm_obj_label = new FCLabel(nullptr);
    sm_obj_label->setText(trUi_(QString::fromUtf8("Select Soldermask object:")));
    sm_obj_label->setToolTip(trUi_(QString::fromUtf8("Gerber Object with the soldermask.\nIt will be used as a base for\nthe pattern plating mask.")));
    lay12->addWidget(sm_obj_label, 14, 0, 1, 3);
    sm_object_combo = new FCComboBox(nullptr);
    lay12->addWidget(sm_object_combo, 16, 0, 1, 3);
    clearance_ppm_label = new FCLabel(nullptr);
    clearance_ppm_label->setText(trUi_(QString::fromUtf8("Clearance:")));
    clearance_ppm_label->setToolTip(trUi_(QString::fromUtf8("The distance between the possible copper thieving elements\nand/or robber bar and the actual openings in the mask.")));
    lay12->addWidget(clearance_ppm_label, 18, 0, 1, 1);
    clearance_ppm_entry = new FCDoubleSpinner(nullptr);
    clearance_ppm_entry->setProperty("decimals", 4);
    clearance_ppm_entry->setProperty("maximum", 10000.0);
    clearance_ppm_entry->setProperty("minimum", -10000.0);
    clearance_ppm_entry->setProperty("singleStep", 0.1);
    lay12->addWidget(clearance_ppm_entry, 18, 1, 1, 2);
    plated_area_label = new FCLabel(nullptr);
    plated_area_label->setText(trUi_(QString::fromUtf8("Plated area:")));
    plated_area_label->setToolTip(trUi_(QString::fromUtf8("The area to be plated by pattern plating.\nBasically is made from the openings in the plating mask.\n\n<<WARNING>> - the calculated area is actually a bit larger\ndue of the fact that the soldermask openings are by design\na bit larger than the copper pads, and this area is\ncalculated from the soldermask openings.")));
    lay12->addWidget(plated_area_label, 20, 0, 1, 1);
    plated_area_entry = new FCEntry(nullptr);
    plated_area_entry->setEnabled(false);
    lay12->addWidget(plated_area_entry, 20, 1, 1, 1);
    units_area_label = new FCLabel(nullptr);
    units_area_label->setText(trUi_(QString::fromUtf8("mm<sup>2</sup>")));
    lay12->addWidget(units_area_label, 20, 2, 1, 1);
    ppm_choice_label = new FCLabel(nullptr);
    ppm_choice_label->setText(trUi_(QString::fromUtf8("Add:")));
    ppm_choice_label->setToolTip(trUi_(QString::fromUtf8("Choose which additional geometry to include, if available.")));
    lay12->addWidget(ppm_choice_label, 22, 0, 1, 1);
    ppm_choice_radio = new RadioSet({{trUi_(QString::fromUtf8("Both")), QVariant(QString::fromUtf8("b"))}, {trUi_(QString::fromUtf8("Thieving")), QVariant(QString::fromUtf8("t"))}, {trUi_(QString::fromUtf8("Robber bar")), QVariant(QString::fromUtf8("r"))}, {trUi_(QString::fromUtf8("None")), QVariant(QString::fromUtf8("n"))}}, Qt::Vertical, false, nullptr);
    ppm_choice_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay12->addWidget(ppm_choice_radio, 22, 1, 1, 2);
    ppm_button = new QPushButton(nullptr);
    { QFont f = ppm_button->font(); f.setBold(true); ppm_button->setFont(f); }
    ppm_button->setIcon(QIcon(QString::fromUtf8(":/images/pattern32.png")));
    ppm_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    ppm_button->setText(trUi_(QString::fromUtf8("Generate pattern plating mask")));
    ppm_button->setToolTip(trUi_(QString::fromUtf8("Will add to the soldermask gerber geometry\nthe geometries of the copper thieving and/or\nthe robber bar if those were generated.")));
    lay12->addWidget(ppm_button, 24, 0, 1, 3);
    lay12->setColumnStretch(1, 1);
    layout->addLayout(lay12, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> ThievingToolUIBase::membersByName() const {
    return {
        {"area_entry", area_entry},
        {"area_label", area_label},
        {"bbox_type_label", bbox_type_label},
        {"bbox_type_radio", bbox_type_radio},
        {"clearance_entry", clearance_entry},
        {"clearance_label", clearance_label},
        {"clearance_ppm_entry", clearance_ppm_entry},
        {"clearance_ppm_label", clearance_ppm_label},
        {"copper_fill_label", copper_fill_label},
        {"dot_dia_entry", dot_dia_entry},
        {"dot_spacing_entry", dot_spacing_entry},
        {"dotdia_label", dotdia_label},
        {"dots_frame", dots_frame},
        {"dots_label", dots_label},
        {"dotspacing_label", dotspacing_label},
        {"fill_button", fill_button},
        {"fill_type_label", fill_type_label},
        {"fill_type_radio", fill_type_radio},
        {"grb_object_combo", grb_object_combo},
        {"grbobj_label", grbobj_label},
        {"layout", layout},
        {"line_size_entry", line_size_entry},
        {"line_size_label", line_size_label},
        {"lines_frame", lines_frame},
        {"lines_label", lines_label},
        {"lines_spacing_entry", lines_spacing_entry},
        {"lines_spacing_label", lines_spacing_label},
        {"margin_entry", margin_entry},
        {"margin_label", margin_label},
        {"patern_mask_label", patern_mask_label},
        {"plated_area_entry", plated_area_entry},
        {"plated_area_label", plated_area_label},
        {"ppm_button", ppm_button},
        {"ppm_choice_label", ppm_choice_label},
        {"ppm_choice_radio", ppm_choice_radio},
        {"rb_button", rb_button},
        {"rb_margin_entry", rb_margin_entry},
        {"rb_margin_label", rb_margin_label},
        {"rb_thickness_entry", rb_thickness_entry},
        {"rb_thickness_label", rb_thickness_label},
        {"ref_combo", ref_combo},
        {"ref_combo_label", ref_combo_label},
        {"ref_combo_type", ref_combo_type},
        {"ref_combo_type_label", ref_combo_type_label},
        {"reference_label", reference_label},
        {"reference_radio", reference_radio},
        {"reset_button", reset_button},
        {"robber_bar_label", robber_bar_label},
        {"sm_obj_label", sm_obj_label},
        {"sm_object_combo", sm_object_combo},
        {"square_size_entry", square_size_entry},
        {"square_size_label", square_size_label},
        {"squares_frame", squares_frame},
        {"squares_label", squares_label},
        {"squares_spacing_entry", squares_spacing_entry},
        {"squares_spacing_label", squares_spacing_label},
        {"units_area_label", units_area_label},
    };
}

}  // namespace flatcam::gui
