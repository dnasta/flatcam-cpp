// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/FilmToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void FilmToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Film PCB"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Film PCB")));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    tf_type_obj_combo_label = new FCLabel(nullptr);
    tf_type_obj_combo_label->setText(trUi_(QString::fromUtf8("<b>Object</b>:")));
    tf_type_obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object for which to create the film.\nThe object can be of type: Gerber or Geometry.\nThe selection here decide the type of objects that will be\nin the Film Object combobox.")));
    lay3->addWidget(tf_type_obj_combo_label, 0, 0, 1, 1);
    tf_type_obj_combo = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("grb"))}, {trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geo"))}}, Qt::Horizontal, true, nullptr);
    tf_type_obj_combo->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(tf_type_obj_combo, 0, 1, 1, 1);
    tf_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(tf_object_combo, 1, 0, 1, 2);
    tf_type_box_combo_label = new FCLabel(nullptr);
    tf_type_box_combo_label->setText(trUi_(QString::fromUtf8("Box Type:")));
    tf_type_box_combo_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be used as an container for\nfilm creation. It can be: Gerber or Geometry type.The selection here decide the type of objects that will be\nin the Box Object combobox.")));
    lay3->addWidget(tf_type_box_combo_label, 2, 0, 1, 1);
    tf_type_box_combo = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("grb"))}, {trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geo"))}}, Qt::Horizontal, true, nullptr);
    tf_type_box_combo->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(tf_type_box_combo, 2, 1, 1, 1);
    tf_box_combo = new FCComboBox(nullptr);
    lay3->addWidget(tf_box_combo, 3, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 4, 0, 1, 2);
    film_adj_label = new FCLabel(nullptr);
    film_adj_label->setText(trUi_(QString::fromUtf8("<b>Film Adjustments</b>")));
    film_adj_label->setToolTip(trUi_(QString::fromUtf8("Sometime the printers will distort the print shape, especially the Laser types.\nThis section provide the tools to compensate for the print distortions.")));
    lay3->addWidget(film_adj_label, 5, 0, 1, 2);
    film_scale_cb = new FCCheckBox(nullptr);
    { QFont f = film_scale_cb->font(); f.setBold(true); film_scale_cb->setFont(f); }
    film_scale_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    film_scale_cb->setText(trUi_(QString::fromUtf8("Scale Film geometry")));
    film_scale_cb->setToolTip(trUi_(QString::fromUtf8("A value greater than 1 will stretch the film\nwhile a value less than 1 will jolt it.")));
    lay3->addWidget(film_scale_cb, 6, 0, 1, 2);
    film_scalex_label = new FCLabel(nullptr);
    film_scalex_label->setText(trUi_(QString::fromUtf8("X factor:")));
    film_scalex_label->setHidden(true);
    lay3->addWidget(film_scalex_label, 7, 0, 1, 1);
    film_scalex_entry = new FCDoubleSpinner(nullptr);
    film_scalex_entry->setProperty("decimals", 4);
    film_scalex_entry->setProperty("maximum", 999.9999);
    film_scalex_entry->setProperty("minimum", -999.9999);
    film_scalex_entry->setProperty("singleStep", 0.01);
    film_scalex_entry->setProperty("value", 1.0);
    film_scalex_entry->setHidden(true);
    lay3->addWidget(film_scalex_entry, 7, 1, 1, 1);
    film_scaley_label = new FCLabel(nullptr);
    film_scaley_label->setText(trUi_(QString::fromUtf8("Y factor:")));
    film_scaley_label->setHidden(true);
    lay3->addWidget(film_scaley_label, 8, 0, 1, 1);
    film_scaley_entry = new FCDoubleSpinner(nullptr);
    film_scaley_entry->setProperty("decimals", 4);
    film_scaley_entry->setProperty("maximum", 999.9999);
    film_scaley_entry->setProperty("minimum", -999.9999);
    film_scaley_entry->setProperty("singleStep", 0.01);
    film_scaley_entry->setProperty("value", 1.0);
    film_scaley_entry->setHidden(true);
    lay3->addWidget(film_scaley_entry, 8, 1, 1, 1);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay3->addWidget(w5, 9, 0, 1, 2);
    film_skew_cb = new FCCheckBox(nullptr);
    { QFont f = film_skew_cb->font(); f.setBold(true); film_skew_cb->setFont(f); }
    film_skew_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    film_skew_cb->setText(trUi_(QString::fromUtf8("Skew Film geometry")));
    film_skew_cb->setToolTip(trUi_(QString::fromUtf8("Positive values will skew to the right\nwhile negative values will skew to the left.")));
    lay3->addWidget(film_skew_cb, 10, 0, 1, 2);
    film_skewx_label = new FCLabel(nullptr);
    film_skewx_label->setText(trUi_(QString::fromUtf8("X angle:")));
    film_skewx_label->setHidden(true);
    lay3->addWidget(film_skewx_label, 11, 0, 1, 1);
    film_skewx_entry = new FCDoubleSpinner(nullptr);
    film_skewx_entry->setProperty("decimals", 4);
    film_skewx_entry->setProperty("maximum", 999.9999);
    film_skewx_entry->setProperty("minimum", -999.9999);
    film_skewx_entry->setProperty("singleStep", 0.01);
    film_skewx_entry->setHidden(true);
    lay3->addWidget(film_skewx_entry, 11, 1, 1, 1);
    film_skewy_label = new FCLabel(nullptr);
    film_skewy_label->setText(trUi_(QString::fromUtf8("Y angle:")));
    film_skewy_label->setHidden(true);
    lay3->addWidget(film_skewy_label, 12, 0, 1, 1);
    film_skewy_entry = new FCDoubleSpinner(nullptr);
    film_skewy_entry->setProperty("decimals", 4);
    film_skewy_entry->setProperty("maximum", 999.9999);
    film_skewy_entry->setProperty("minimum", -999.9999);
    film_skewy_entry->setProperty("singleStep", 0.01);
    film_skewy_entry->setHidden(true);
    lay3->addWidget(film_skewy_entry, 12, 1, 1, 1);
    film_skew_ref_label = new FCLabel(nullptr);
    film_skew_ref_label->setText(trUi_(QString::fromUtf8("Reference:")));
    film_skew_ref_label->setToolTip(trUi_(QString::fromUtf8("The reference point to be used as origin for the skew.\nIt can be one of the four points of the geometry bounding box.")));
    film_skew_ref_label->setHidden(true);
    lay3->addWidget(film_skew_ref_label, 13, 0, 1, 1);
    film_skew_reference = new RadioSet({{trUi_(QString::fromUtf8("Bottom Left")), QVariant(QString::fromUtf8("bottomleft"))}, {trUi_(QString::fromUtf8("Top Left")), QVariant(QString::fromUtf8("topleft"))}, {trUi_(QString::fromUtf8("Bottom Right")), QVariant(QString::fromUtf8("bottomright"))}, {trUi_(QString::fromUtf8("Top right")), QVariant(QString::fromUtf8("topright"))}}, Qt::Vertical, false, nullptr);
    film_skew_reference->setHidden(true);
    film_skew_reference->setValue(QVariant(QString::fromUtf8("bottomleft")));
    lay3->addWidget(film_skew_reference, 13, 1, 1, 1);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay3->addWidget(w6, 14, 0, 1, 2);
    film_mirror_cb = new FCCheckBox(nullptr);
    { QFont f = film_mirror_cb->font(); f.setBold(true); film_mirror_cb->setFont(f); }
    film_mirror_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    film_mirror_cb->setText(trUi_(QString::fromUtf8("Mirror Film geometry")));
    film_mirror_cb->setToolTip(trUi_(QString::fromUtf8("Mirror the film geometry on the selected axis or on both.")));
    lay3->addWidget(film_mirror_cb, 15, 0, 1, 2);
    film_mirror_axis_label = new FCLabel(nullptr);
    film_mirror_axis_label->setText(trUi_(QString::fromUtf8("Mirror Axis:")));
    film_mirror_axis_label->setHidden(true);
    lay3->addWidget(film_mirror_axis_label, 16, 0, 1, 1);
    film_mirror_axis = new RadioSet({{trUi_(QString::fromUtf8("None")), QVariant(QString::fromUtf8("none"))}, {trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("x"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("y"))}, {trUi_(QString::fromUtf8("Both")), QVariant(QString::fromUtf8("both"))}}, Qt::Horizontal, false, nullptr);
    film_mirror_axis->setHidden(true);
    film_mirror_axis->setValue(QVariant(QString::fromUtf8("none")));
    lay3->addWidget(film_mirror_axis, 16, 1, 1, 1);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay3->addWidget(w7, 17, 0, 1, 2);
    film_param_label = new FCLabel(nullptr);
    film_param_label->setText(trUi_(QString::fromUtf8("<b>Film Parameters</b>")));
    lay3->addWidget(film_param_label, 18, 0, 1, 2);
    film_scale_stroke_label = new FCLabel(nullptr);
    film_scale_stroke_label->setText(trUi_(QString::fromUtf8("Scale Stroke:")));
    film_scale_stroke_label->setToolTip(trUi_(QString::fromUtf8("Scale the line stroke thickness of each feature in the SVG file.\nIt means that the line that envelope each SVG feature will be thicker or thinner,\ntherefore the fine features may be more affected by this parameter.")));
    lay3->addWidget(film_scale_stroke_label, 19, 0, 1, 1);
    film_scale_stroke_entry = new FCDoubleSpinner(nullptr);
    film_scale_stroke_entry->setProperty("decimals", 4);
    film_scale_stroke_entry->setProperty("maximum", 999.9999);
    film_scale_stroke_entry->setProperty("minimum", -999.9999);
    film_scale_stroke_entry->setProperty("singleStep", 0.01);
    lay3->addWidget(film_scale_stroke_entry, 19, 1, 1, 1);
    film_type_label = new FCLabel(nullptr);
    film_type_label->setText(trUi_(QString::fromUtf8("Film Type:")));
    film_type_label->setToolTip(trUi_(QString::fromUtf8("Generate a Positive black film or a Negative film.\nPositive means that it will print the features\nwith black on a white canvas.\nNegative means that it will print the features\nwith white on a black canvas.\nThe Film format is SVG.")));
    lay3->addWidget(film_type_label, 21, 0, 1, 1);
    film_type = new RadioSet({{trUi_(QString::fromUtf8("Positive")), QVariant(QString::fromUtf8("pos"))}, {trUi_(QString::fromUtf8("Negative")), QVariant(QString::fromUtf8("neg"))}}, Qt::Horizontal, false, nullptr);
    film_type->setValue(QVariant(QString::fromUtf8("neg")));
    lay3->addWidget(film_type, 21, 1, 1, 1);
    boundary_label = new FCLabel(nullptr);
    boundary_label->setText(trUi_(QString::fromUtf8("Border:")));
    boundary_label->setToolTip(trUi_(QString::fromUtf8("Specify a border around the object.\nOnly for negative film.\nIt helps if we use as a Box Object the same \nobject as in Film Object. It will create a thick\nblack bar around the actual print allowing for a\nbetter delimitation of the outline features which are of\nwhite color like the rest and which may confound with the\nsurroundings if not for this border.")));
    lay3->addWidget(boundary_label, 22, 0, 1, 1);
    boundary_entry = new FCDoubleSpinner(nullptr);
    boundary_entry->setProperty("decimals", 4);
    boundary_entry->setProperty("maximum", 999.9999);
    boundary_entry->setProperty("minimum", -999.9999);
    boundary_entry->setProperty("singleStep", 0.01);
    boundary_entry->setProperty("value", 1.0);
    lay3->addWidget(boundary_entry, 22, 1, 1, 1);
    punch_cb = new FCCheckBox(nullptr);
    punch_cb->setText(trUi_(QString::fromUtf8("Punch drill holes")));
    punch_cb->setToolTip(trUi_(QString::fromUtf8("When checked the generated film will have holes in pads when\nthe generated film is positive. This is done to help drilling,\nwhen done manually.")));
    punch_cb->setHidden(true);
    lay3->addWidget(punch_cb, 23, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    punch_frame = new QFrame(nullptr);
    punch_frame->setHidden(true);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    source_label = new FCLabel(nullptr);
    source_label->setText(trUi_(QString::fromUtf8("Source:")));
    source_label->setToolTip(trUi_(QString::fromUtf8("The punch hole source can be:\n- Excellon -> an Excellon holes center will serve as reference.\n- Pad Center -> will try to use the pads center as reference.")));
    lay8->addWidget(source_label, 0, 0, 1, 1);
    source_punch = new RadioSet({{trUi_(QString::fromUtf8("Excellon")), QVariant(QString::fromUtf8("exc"))}, {trUi_(QString::fromUtf8("Pad center")), QVariant(QString::fromUtf8("pad"))}}, Qt::Horizontal, false, nullptr);
    source_punch->setValue(QVariant(QString::fromUtf8("exc")));
    lay8->addWidget(source_punch, 0, 1, 1, 1);
    exc_label = new FCLabel(nullptr);
    exc_label->setText(trUi_(QString::fromUtf8("Excellon Obj:")));
    exc_label->setToolTip(trUi_(QString::fromUtf8("Remove the geometry of Excellon from the Film to create the holes in pads.")));
    lay8->addWidget(exc_label, 1, 0, 1, 1);
    exc_combo = new FCComboBox(nullptr);
    lay8->addWidget(exc_combo, 1, 1, 1, 1);
    punch_size_label = new FCLabel(nullptr);
    punch_size_label->setText(trUi_(QString::fromUtf8("Punch Size:")));
    punch_size_label->setToolTip(trUi_(QString::fromUtf8("The value here will control how big is the punch hole in the pads.")));
    punch_size_label->setHidden(true);
    lay8->addWidget(punch_size_label, 2, 0, 1, 1);
    punch_size_spinner = new FCDoubleSpinner(nullptr);
    punch_size_spinner->setProperty("decimals", 4);
    punch_size_spinner->setProperty("maximum", 999.9999);
    punch_size_spinner->setProperty("singleStep", 0.1);
    punch_size_spinner->setHidden(true);
    lay8->addWidget(punch_size_spinner, 2, 1, 1, 1);
    lay8->setColumnStretch(1, 1);
    punch_frame->setLayout(lay8);
    layout->addWidget(punch_frame, 0);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay9->addWidget(w10, 0, 0, 1, 2);
    file_type_label = new FCLabel(nullptr);
    file_type_label->setText(trUi_(QString::fromUtf8("Film Type:")));
    file_type_label->setToolTip(trUi_(QString::fromUtf8("The file type of the saved film. Can be:\n- 'SVG' -> open-source vectorial format\n- 'PNG' -> raster image\n- 'PDF' -> portable document format")));
    lay9->addWidget(file_type_label, 1, 0, 1, 1);
    file_type_radio = new RadioSet({{trUi_(QString::fromUtf8("SVG")), QVariant(QString::fromUtf8("svg"))}, {trUi_(QString::fromUtf8("PNG")), QVariant(QString::fromUtf8("png"))}, {trUi_(QString::fromUtf8("PDF")), QVariant(QString::fromUtf8("pdf"))}}, Qt::Horizontal, false, nullptr);
    file_type_radio->setValue(QVariant(QString::fromUtf8("svg")));
    lay9->addWidget(file_type_radio, 1, 1, 1, 1);
    orientation_label = new FCLabel(nullptr);
    orientation_label->setText(trUi_(QString::fromUtf8("Page Orientation:")));
    orientation_label->setToolTip(trUi_(QString::fromUtf8("Can be:\n- Portrait\n- Landscape")));
    orientation_label->setHidden(true);
    lay9->addWidget(orientation_label, 2, 0, 1, 1);
    orientation_radio = new RadioSet({{trUi_(QString::fromUtf8("Portrait")), QVariant(QString::fromUtf8("p"))}, {trUi_(QString::fromUtf8("Landscape")), QVariant(QString::fromUtf8("l"))}}, Qt::Horizontal, false, nullptr);
    orientation_radio->setHidden(true);
    orientation_radio->setValue(QVariant(QString::fromUtf8("p")));
    lay9->addWidget(orientation_radio, 2, 1, 1, 1);
    pagesize_label = new FCLabel(nullptr);
    pagesize_label->setText(trUi_(QString::fromUtf8("Page Size:")));
    pagesize_label->setToolTip(trUi_(QString::fromUtf8("A selection of standard ISO 216 page sizes.")));
    pagesize_label->setHidden(true);
    lay9->addWidget(pagesize_label, 3, 0, 1, 1);
    pagesize_combo = new FCComboBox(nullptr);
    pagesize_combo->setProperty("currentText", trUi_(QString::fromUtf8("A4")));
    pagesize_combo->setHidden(true);
    pagesize_combo->addItem(trUi_(QString::fromUtf8("Bounds")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A0")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A1")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A2")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A3")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A4")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A5")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A6")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A7")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A8")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A9")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("A10")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B0")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B1")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B2")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B3")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B4")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B5")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B6")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B7")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B8")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B9")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("B10")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C0")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C1")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C2")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C3")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C4")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C5")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C6")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C7")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C8")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C9")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("C10")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("LETTER")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("LEGAL")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("ELEVENSEVENTEEN")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("JUNIOR_LEGAL")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("HALF_LETTER")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("GOV_LETTER")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("GOV_LEGAL")));
    pagesize_combo->addItem(trUi_(QString::fromUtf8("LEDGER")));
    pagesize_combo->setCurrentIndex(5);
    lay9->addWidget(pagesize_combo, 3, 1, 1, 1);
    png_dpi_label = new FCLabel(nullptr);
    png_dpi_label->setText(trUi_(QString::fromUtf8("PNG DPI:")));
    png_dpi_label->setToolTip(trUi_(QString::fromUtf8("Default value is 96 DPI. Change this value to scale the PNG file.")));
    png_dpi_label->setHidden(true);
    lay9->addWidget(png_dpi_label, 4, 0, 1, 1);
    png_dpi_spinner = new FCSpinner(nullptr);
    png_dpi_spinner->setProperty("maximum", 100000);
    png_dpi_spinner->setProperty("value", 96);
    png_dpi_spinner->setHidden(true);
    lay9->addWidget(png_dpi_spinner, 4, 1, 1, 1);
    film_object_button = new FCButton(nullptr);
    { QFont f = film_object_button->font(); f.setBold(true); film_object_button->setFont(f); }
    film_object_button->setIcon(QIcon(QString::fromUtf8(":/images/save_as.png")));
    film_object_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    film_object_button->setText(trUi_(QString::fromUtf8("Save Film")));
    film_object_button->setToolTip(trUi_(QString::fromUtf8("Create a Film for the selected object, within\nthe specified box. Does not create a new \n FlatCAM object, but directly save it in the\nselected format.")));
    lay9->addWidget(film_object_button, 6, 0, 1, 2);
    lay9->setColumnStretch(1, 1);
    layout->addLayout(lay9, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> FilmToolUIBase::membersByName() const {
    return {
        {"boundary_entry", boundary_entry},
        {"boundary_label", boundary_label},
        {"exc_combo", exc_combo},
        {"exc_label", exc_label},
        {"file_type_label", file_type_label},
        {"file_type_radio", file_type_radio},
        {"film_adj_label", film_adj_label},
        {"film_mirror_axis", film_mirror_axis},
        {"film_mirror_axis_label", film_mirror_axis_label},
        {"film_mirror_cb", film_mirror_cb},
        {"film_object_button", film_object_button},
        {"film_param_label", film_param_label},
        {"film_scale_cb", film_scale_cb},
        {"film_scale_stroke_entry", film_scale_stroke_entry},
        {"film_scale_stroke_label", film_scale_stroke_label},
        {"film_scalex_entry", film_scalex_entry},
        {"film_scalex_label", film_scalex_label},
        {"film_scaley_entry", film_scaley_entry},
        {"film_scaley_label", film_scaley_label},
        {"film_skew_cb", film_skew_cb},
        {"film_skew_ref_label", film_skew_ref_label},
        {"film_skew_reference", film_skew_reference},
        {"film_skewx_entry", film_skewx_entry},
        {"film_skewx_label", film_skewx_label},
        {"film_skewy_entry", film_skewy_entry},
        {"film_skewy_label", film_skewy_label},
        {"film_type", film_type},
        {"film_type_label", film_type_label},
        {"layout", layout},
        {"orientation_label", orientation_label},
        {"orientation_radio", orientation_radio},
        {"pagesize_combo", pagesize_combo},
        {"pagesize_label", pagesize_label},
        {"png_dpi_label", png_dpi_label},
        {"png_dpi_spinner", png_dpi_spinner},
        {"punch_cb", punch_cb},
        {"punch_frame", punch_frame},
        {"punch_size_label", punch_size_label},
        {"punch_size_spinner", punch_size_spinner},
        {"reset_button", reset_button},
        {"source_label", source_label},
        {"source_punch", source_punch},
        {"tf_box_combo", tf_box_combo},
        {"tf_object_combo", tf_object_combo},
        {"tf_type_box_combo", tf_type_box_combo},
        {"tf_type_box_combo_label", tf_type_box_combo_label},
        {"tf_type_obj_combo", tf_type_obj_combo},
        {"tf_type_obj_combo_label", tf_type_obj_combo_label},
    };
}

}  // namespace flatcam::gui
