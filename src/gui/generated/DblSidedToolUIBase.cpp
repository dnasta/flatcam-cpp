// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DblSidedToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void DblSidedToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("2-Sided PCB"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("2-Sided PCB")));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    m_objects_label = new FCLabel(nullptr);
    m_objects_label->setText(trUi_(QString::fromUtf8("<b>Source Object:</b>")));
    m_objects_label->setToolTip(trUi_(QString::fromUtf8("Objects to be mirrored.")));
    lay3->addWidget(m_objects_label, 0, 0, 1, 2);
    type_obj_combo_label = new FCLabel(nullptr);
    type_obj_combo_label->setText(trUi_(QString::fromUtf8("Type:")));
    type_obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Select the type of application object to be processed in this tool.")));
    lay3->addWidget(type_obj_combo_label, 2, 0, 1, 1);
    object_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("grb"))}, {trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geo"))}, {trUi_(QString::fromUtf8("Excellon")), QVariant(QString::fromUtf8("exc"))}}, Qt::Horizontal, true, nullptr);
    object_type_radio->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(object_type_radio, 2, 1, 1, 1);
    object_combo = new FCComboBox(nullptr);
    lay3->addWidget(object_combo, 4, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 7, 0, 1, 2);
    lay3->setColumnStretch(0, 1);
    layout->addLayout(lay3, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    bv_label = new FCLabel(nullptr);
    bv_label->setText(trUi_(QString::fromUtf8("<b>Bounds Values:</b>")));
    bv_label->setToolTip(trUi_(QString::fromUtf8("Select on canvas the object(s)\nfor which to calculate bounds values.")));
    lay5->addWidget(bv_label, 6, 0, 1, 2);
    xmin_btn = new FCButton(nullptr);
    xmin_btn->setText(trUi_(QString::fromUtf8("X min:")));
    xmin_btn->setToolTip(trUi_(QString::fromUtf8("Minimum location.")));
    lay5->addWidget(xmin_btn, 7, 0, 1, 1);
    xmin_entry = new FCDoubleSpinner(nullptr);
    xmin_entry->setProperty("decimals", 4);
    xmin_entry->setProperty("maximum", 10000.0);
    xmin_entry->setProperty("minimum", -10000.0);
    xmin_entry->setReadOnly(true);
    lay5->addWidget(xmin_entry, 7, 1, 1, 1);
    ymin_btn = new FCButton(nullptr);
    ymin_btn->setText(trUi_(QString::fromUtf8("Y min:")));
    ymin_btn->setToolTip(trUi_(QString::fromUtf8("Minimum location.")));
    lay5->addWidget(ymin_btn, 8, 0, 1, 1);
    ymin_entry = new FCDoubleSpinner(nullptr);
    ymin_entry->setProperty("decimals", 4);
    ymin_entry->setProperty("maximum", 10000.0);
    ymin_entry->setProperty("minimum", -10000.0);
    ymin_entry->setReadOnly(true);
    lay5->addWidget(ymin_entry, 8, 1, 1, 1);
    xmax_btn = new FCButton(nullptr);
    xmax_btn->setText(trUi_(QString::fromUtf8("X max:")));
    xmax_btn->setToolTip(trUi_(QString::fromUtf8("Maximum location.")));
    lay5->addWidget(xmax_btn, 9, 0, 1, 1);
    xmax_entry = new FCDoubleSpinner(nullptr);
    xmax_entry->setProperty("decimals", 4);
    xmax_entry->setProperty("maximum", 10000.0);
    xmax_entry->setProperty("minimum", -10000.0);
    xmax_entry->setReadOnly(true);
    lay5->addWidget(xmax_entry, 9, 1, 1, 1);
    ymax_btn = new FCButton(nullptr);
    ymax_btn->setText(trUi_(QString::fromUtf8("Y max:")));
    ymax_btn->setToolTip(trUi_(QString::fromUtf8("Maximum location.")));
    lay5->addWidget(ymax_btn, 10, 0, 1, 1);
    ymax_entry = new FCDoubleSpinner(nullptr);
    ymax_entry->setProperty("decimals", 4);
    ymax_entry->setProperty("maximum", 10000.0);
    ymax_entry->setProperty("minimum", -10000.0);
    ymax_entry->setReadOnly(true);
    lay5->addWidget(ymax_entry, 10, 1, 1, 1);
    center_btn = new FCButton(nullptr);
    center_btn->setText(trUi_(QString::fromUtf8("Centroid:")));
    center_btn->setToolTip(trUi_(QString::fromUtf8("The center point location for the rectangular\nbounding shape. Centroid. Format is (x, y).")));
    lay5->addWidget(center_btn, 12, 0, 1, 1);
    center_entry = new NumericalEvalTupleEntry(nullptr);
    center_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Center point coordinates")));
    center_entry->setReadOnly(true);
    center_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay5->addWidget(center_entry, 12, 1, 1, 1);
    calculate_bb_button = new FCButton(nullptr);
    { QFont f = calculate_bb_button->font(); f.setBold(true); calculate_bb_button->setFont(f); }
    calculate_bb_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t\t\t\t\t   "));
    calculate_bb_button->setText(trUi_(QString::fromUtf8("Calculate Bounds Values")));
    calculate_bb_button->setToolTip(trUi_(QString::fromUtf8("Calculate the enveloping rectangular shape coordinates,\nfor the selection of objects.\nThe envelope shape is parallel with the X, Y axis.")));
    lay5->addWidget(calculate_bb_button, 13, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay5->addWidget(w6, 14, 0, 1, 2);
    lay5->setColumnStretch(1, 1);
    layout->addLayout(lay5, 0);
    auto* lay7 = new QGridLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    param_label = new FCLabel(nullptr);
    param_label->setText(trUi_(QString::fromUtf8("<b>Mirror Operation:</b>")));
    param_label->setToolTip(trUi_(QString::fromUtf8("Parameters for the mirror operation.")));
    lay7->addWidget(param_label, 0, 0, 1, 2);
    mirax_label = new FCLabel(nullptr);
    mirax_label->setText(trUi_(QString::fromUtf8("Axis:")));
    mirax_label->setToolTip(trUi_(QString::fromUtf8("Mirror vertically (X) or horizontally (Y).")));
    lay7->addWidget(mirax_label, 2, 0, 1, 1);
    mirror_axis = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}}, Qt::Vertical, false, nullptr);
    mirror_axis->setValue(QVariant(QString::fromUtf8("X")));
    lay7->addWidget(mirror_axis, 2, 1, 1, 2);
    axloc_label = new FCLabel(nullptr);
    axloc_label->setText(trUi_(QString::fromUtf8("Reference:")));
    axloc_label->setToolTip(trUi_(QString::fromUtf8("The coordinates used as reference for the mirror operation.\nCan be:\n- Point -> a set of coordinates (x,y) around which the object is mirrored\n- Box -> a set of coordinates (x, y) obtained from the center of the\nbounding box of another object selected below\n- Hole Snap -> a point defined by the center of a drill hole in a Excellon object")));
    lay7->addWidget(axloc_label, 4, 0, 1, 1);
    axis_location = new RadioSet({{trUi_(QString::fromUtf8("Point")), QVariant(QString::fromUtf8("point"))}, {trUi_(QString::fromUtf8("Box")), QVariant(QString::fromUtf8("box"))}, {trUi_(QString::fromUtf8("Hole Snap")), QVariant(QString::fromUtf8("hole"))}}, Qt::Horizontal, true, nullptr);
    axis_location->setValue(QVariant(QString::fromUtf8("point")));
    lay7->addWidget(axis_location, 4, 1, 1, 2);
    point_entry = new NumericalEvalTupleEntry(nullptr);
    point_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Point coordinates")));
    point_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay7->addWidget(point_entry, 7, 0, 1, 2);
    add_point_button = new FCButton(nullptr);
    { QFont f = add_point_button->font(); f.setBold(true); add_point_button->setFont(f); }
    add_point_button->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_point_button->setMinimumSize(QSize(60, 0));
    add_point_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t\t\t"));
    add_point_button->setText(trUi_(QString::fromUtf8("Add")));
    add_point_button->setToolTip(trUi_(QString::fromUtf8("Add the coordinates in format <b>(x, y)</b> through which the mirroring axis\n selected in 'MIRROR AXIS' pass.\nThe (x, y) coordinates are captured by pressing SHIFT key\nand left mouse button click on canvas or you can enter the coordinates manually.")));
    lay7->addWidget(add_point_button, 7, 2, 1, 1);
    exc_hole_lbl = new FCLabel(nullptr);
    exc_hole_lbl->setText(trUi_(QString::fromUtf8("Excellon:")));
    exc_hole_lbl->setToolTip(trUi_(QString::fromUtf8("Object that holds holes that can be picked as reference for mirroring.")));
    exc_hole_lbl->setHidden(true);
    lay7->addWidget(exc_hole_lbl, 10, 0, 1, 1);
    exc_combo = new FCComboBox(nullptr);
    exc_combo->setHidden(true);
    lay7->addWidget(exc_combo, 10, 1, 1, 2);
    pick_hole_button = new FCButton(nullptr);
    pick_hole_button->setText(trUi_(QString::fromUtf8("Pick hole")));
    pick_hole_button->setToolTip(trUi_(QString::fromUtf8("Click inside a drill hole that belong to the selected Excellon object,\nand the hole center coordinates will be copied to the Point field.")));
    pick_hole_button->setHidden(true);
    lay7->addWidget(pick_hole_button, 12, 0, 1, 3);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    box_type_label = new FCLabel(nullptr);
    box_type_label->setText(trUi_(QString::fromUtf8("Reference Object:")));
    box_type_label->setToolTip(trUi_(QString::fromUtf8("It can be of type: Gerber or Excellon or Geometry.\nThe coordinates of the center of the bounding box are used\nas reference for mirror operation.")));
    box_type_label->setHidden(true);
    lay8->addWidget(box_type_label, 0, 0, 1, 2);
    box_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("grb"))}, {trUi_(QString::fromUtf8("Excellon")), QVariant(QString::fromUtf8("exc"))}, {trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geo"))}}, Qt::Horizontal, true, nullptr);
    box_type_radio->setHidden(true);
    box_type_radio->setValue(QVariant(QString::fromUtf8("grb")));
    lay8->addWidget(box_type_radio, 1, 0, 1, 2);
    box_combo = new FCComboBox(nullptr);
    box_combo->setHidden(true);
    lay8->addWidget(box_combo, 3, 0, 1, 2);
    lay8->setColumnStretch(1, 1);
    lay7->addLayout(lay8, 14, 0, 1, 3);
    mirror_button = new FCButton(nullptr);
    { QFont f = mirror_button->font(); f.setBold(true); mirror_button->setFont(f); }
    mirror_button->setIcon(QIcon(QString::fromUtf8(":/images/doubleside16.png")));
    mirror_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    mirror_button->setText(trUi_(QString::fromUtf8("Mirror")));
    mirror_button->setToolTip(trUi_(QString::fromUtf8("Mirrors (flips) the specified object around \nthe specified axis. Does not create a new \nobject, but modifies it.")));
    lay7->addWidget(mirror_button, 16, 0, 1, 3);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay7->addWidget(w9, 18, 0, 1, 3);
    lay7->setColumnStretch(1, 1);
    layout->addLayout(lay7, 0);
    auto* lay10 = new QGridLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setHorizontalSpacing(6);
    lay10->setVerticalSpacing(6);
    alignment_label = new FCLabel(nullptr);
    alignment_label->setText(trUi_(QString::fromUtf8("<b>PCB Alignment:</b>")));
    alignment_label->setToolTip(trUi_(QString::fromUtf8("Creates an Excellon Object containing the\nspecified alignment holes and their mirror\nimages.")));
    lay10->addWidget(alignment_label, 0, 0, 1, 2);
    dt_label = new FCLabel(nullptr);
    dt_label->setText(trUi_(QString::fromUtf8("Drill Dia:")));
    dt_label->setToolTip(trUi_(QString::fromUtf8("Diameter of the drill for the alignment holes.")));
    lay10->addWidget(dt_label, 2, 0, 1, 1);
    drill_dia = new FCDoubleSpinner(nullptr);
    drill_dia->setProperty("decimals", 4);
    drill_dia->setProperty("maximum", 10000.0);
    drill_dia->setToolTip(trUi_(QString::fromUtf8("Diameter of the drill for the alignment holes.")));
    drill_dia->setProperty("value", 3.125);
    lay10->addWidget(drill_dia, 2, 1, 1, 1);
    align_ax_label = new FCLabel(nullptr);
    align_ax_label->setText(trUi_(QString::fromUtf8("Axis:")));
    align_ax_label->setToolTip(trUi_(QString::fromUtf8("Mirror vertically (X) or horizontally (Y).")));
    lay10->addWidget(align_ax_label, 4, 0, 1, 1);
    align_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}}, Qt::Vertical, false, nullptr);
    align_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay10->addWidget(align_axis_radio, 4, 1, 1, 1);
    align_ref_label = new FCLabel(nullptr);
    align_ref_label->setText(trUi_(QString::fromUtf8("Reference:")));
    align_ref_label->setToolTip(trUi_(QString::fromUtf8("The reference point used to create the second alignment drill\nfrom the first alignment drill, by doing mirror.\nIt can be modified in the Mirror Parameters -> Reference section")));
    lay10->addWidget(align_ref_label, 6, 0, 1, 1);
    align_ref_label_val = new NumericalEvalTupleEntry(nullptr);
    align_ref_label_val->setEnabled(false);
    align_ref_label_val->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    align_ref_label_val->setText(trUi_(QString::fromUtf8("0.0000")));
    align_ref_label_val->setToolTip(trUi_(QString::fromUtf8("The reference point used to create the second alignment drill\nfrom the first alignment drill, by doing mirror.\nIt can be modified in the Mirror Parameters -> Reference section")));
    lay10->addWidget(align_ref_label_val, 6, 1, 1, 1);
    lay10->setColumnStretch(1, 1);
    layout->addLayout(lay10, 0);
    auto* lay11 = new QGridLayout();
    lay11->setContentsMargins(0, 0, 0, 0);
    lay11->setHorizontalSpacing(6);
    lay11->setVerticalSpacing(6);
    ah_label = new FCLabel(nullptr);
    ah_label->setText(trUi_(QString::fromUtf8("Alignment Drill Coordinates:")));
    ah_label->setToolTip(trUi_(QString::fromUtf8("Alignment holes (x1, y1), (x2, y2), ... on one side of the mirror axis. For each set of (x, y) coordinates\nentered here, a pair of drills will be created:\n\n- one drill at the coordinates from the field\n- one drill in mirror position over the axis selected above in the 'Align Axis'.")));
    lay11->addWidget(ah_label, 0, 0, 1, 2);
    alignment_holes = new NumericalEvalTupleEntry(nullptr);
    alignment_holes->setProperty("placeholderText", trUi_(QString::fromUtf8("Drill coordinates")));
    alignment_holes->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay11->addWidget(alignment_holes, 1, 0, 1, 2);
    auto* lay12 = new QHBoxLayout();
    lay12->setContentsMargins(0, 0, 0, 0);
    lay12->setSpacing(6);
    add_drill_point_button = new FCButton(nullptr);
    add_drill_point_button->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_drill_point_button->setText(trUi_(QString::fromUtf8("Add")));
    add_drill_point_button->setToolTip(trUi_(QString::fromUtf8("Add alignment drill holes coordinates in the format: (x1, y1), (x2, y2), ... \non one side of the alignment axis.\n\nThe coordinates set can be obtained:\n- press SHIFT key and left mouse clicking on canvas. Then click Add.\n- press SHIFT key and left mouse clicking on canvas. Then Ctrl+V in the field.\n- press SHIFT key and left mouse clicking on canvas. Then RMB click in the field and click Paste.\n- by entering the coords manually in the format: (x1, y1), (x2, y2), ...")));
    lay12->addWidget(add_drill_point_button, 0);
    delete_drill_point_button = new FCButton(nullptr);
    delete_drill_point_button->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    delete_drill_point_button->setText(trUi_(QString::fromUtf8("Delete Last")));
    delete_drill_point_button->setToolTip(trUi_(QString::fromUtf8("Delete the last coordinates tuple in the list.")));
    lay12->addWidget(delete_drill_point_button, 0);
    lay11->addLayout(lay12, 2, 0, 1, 2);
    layout->addLayout(lay11, 0);
    create_alignment_hole_button = new FCButton(nullptr);
    { QFont f = create_alignment_hole_button->font(); f.setBold(true); create_alignment_hole_button->setFont(f); }
    create_alignment_hole_button->setIcon(QIcon(QString::fromUtf8(":/images/drill32.png")));
    create_alignment_hole_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    create_alignment_hole_button->setText(trUi_(QString::fromUtf8("Create Excellon Object")));
    create_alignment_hole_button->setToolTip(trUi_(QString::fromUtf8("Creates an Excellon Object containing the\nspecified alignment holes and their mirror\nimages.")));
    layout->addWidget(create_alignment_hole_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> DblSidedToolUIBase::membersByName() const {
    return {
        {"add_drill_point_button", add_drill_point_button},
        {"add_point_button", add_point_button},
        {"ah_label", ah_label},
        {"align_ax_label", align_ax_label},
        {"align_axis_radio", align_axis_radio},
        {"align_ref_label", align_ref_label},
        {"align_ref_label_val", align_ref_label_val},
        {"alignment_holes", alignment_holes},
        {"alignment_label", alignment_label},
        {"axis_location", axis_location},
        {"axloc_label", axloc_label},
        {"box_combo", box_combo},
        {"box_type_label", box_type_label},
        {"box_type_radio", box_type_radio},
        {"bv_label", bv_label},
        {"calculate_bb_button", calculate_bb_button},
        {"center_btn", center_btn},
        {"center_entry", center_entry},
        {"create_alignment_hole_button", create_alignment_hole_button},
        {"delete_drill_point_button", delete_drill_point_button},
        {"drill_dia", drill_dia},
        {"dt_label", dt_label},
        {"exc_combo", exc_combo},
        {"exc_hole_lbl", exc_hole_lbl},
        {"layout", layout},
        {"m_objects_label", m_objects_label},
        {"mirax_label", mirax_label},
        {"mirror_axis", mirror_axis},
        {"mirror_button", mirror_button},
        {"object_combo", object_combo},
        {"object_type_radio", object_type_radio},
        {"param_label", param_label},
        {"pick_hole_button", pick_hole_button},
        {"point_entry", point_entry},
        {"reset_button", reset_button},
        {"type_obj_combo_label", type_obj_combo_label},
        {"xmax_btn", xmax_btn},
        {"xmax_entry", xmax_entry},
        {"xmin_btn", xmin_btn},
        {"xmin_entry", xmin_entry},
        {"ymax_btn", ymax_btn},
        {"ymax_entry", ymax_entry},
        {"ymin_btn", ymin_btn},
        {"ymin_entry", ymin_entry},
    };
}

}  // namespace flatcam::gui
