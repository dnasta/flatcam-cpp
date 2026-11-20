// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/CutOutToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void CutOutToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Cutout PCB"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Cutout PCB")));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    object_label = new QLabel(nullptr);
    object_label->setText(trUi_(QString::fromUtf8("<b>Source Object:</b>")));
    object_label->setToolTip(trUi_(QString::fromUtf8("Object to be cutout.")));
    lay3->addWidget(object_label, 0, 0, 1, 2);
    kindlabel = new QLabel(nullptr);
    kindlabel->setText(trUi_(QString::fromUtf8("Kind:")));
    kindlabel->setToolTip(trUi_(QString::fromUtf8("Choice of what kind the object we want to cutout is.\n- Single: contain a single PCB Gerber outline object.\n- Panel: a panel PCB Gerber object, which is made\nout of many individual PCB outlines.")));
    lay3->addWidget(kindlabel, 2, 0, 1, 1);
    obj_kind_combo = new RadioSet({{trUi_(QString::fromUtf8("Single")), QVariant(QString::fromUtf8("single"))}, {trUi_(QString::fromUtf8("Panel")), QVariant(QString::fromUtf8("panel"))}}, Qt::Horizontal, true, nullptr);
    obj_kind_combo->setValue(QVariant(QString::fromUtf8("single")));
    lay3->addWidget(obj_kind_combo, 2, 1, 1, 1);
    type_obj_combo_label = new QLabel(nullptr);
    type_obj_combo_label->setText(trUi_(QString::fromUtf8("Type:")));
    type_obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be cutout.\nIt can be of type: Gerber or Geometry.\nWhat is selected here will dictate the kind\nof objects that will populate the 'Object' combobox.")));
    lay3->addWidget(type_obj_combo_label, 4, 0, 1, 1);
    type_obj_radio = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("grb"))}, {trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geo"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(type_obj_radio, 4, 1, 1, 1);
    obj_combo = new FCComboBox(nullptr);
    lay3->addWidget(obj_combo, 6, 0, 1, 2);
    convex_box_label = new QLabel(nullptr);
    convex_box_label->setText(trUi_(QString::fromUtf8("Convex Shape:")));
    convex_box_label->setToolTip(trUi_(QString::fromUtf8("Create a convex shape surrounding the entire PCB.\nUsed only if the source object type is Gerber.")));
    lay3->addWidget(convex_box_label, 8, 0, 1, 1);
    convex_box_cb = new FCCheckBox(nullptr);
    convex_box_cb->setToolTip(trUi_(QString::fromUtf8("Create a convex shape surrounding the entire PCB.\nUsed only if the source object type is Gerber.")));
    lay3->addWidget(convex_box_cb, 8, 1, 1, 1);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 10, 0, 1, 2);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(trUi_(QString::fromUtf8("<b>Cutout Tool</b>")));
    lay3->addWidget(tool_sel_label, 12, 0, 1, 2);
    dia_label = new QLabel(nullptr);
    dia_label->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    dia_label->setToolTip(trUi_(QString::fromUtf8("Diameter of the tool used to cutout\nthe PCB shape out of the surrounding material.")));
    lay3->addWidget(dia_label, 14, 0, 1, 1);
    dia = new FCDoubleSpinner(nullptr);
    dia->setProperty("decimals", 4);
    dia->setProperty("maximum", 10000.0);
    dia->setProperty("value", 2.4);
    lay3->addWidget(dia, 14, 1, 1, 1);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    add_newtool_button = new FCButton(nullptr);
    add_newtool_button->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_newtool_button->setText(trUi_(QString::fromUtf8("Search and Add")));
    add_newtool_button->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nwith the diameter specified above.\nThis is done by a background search\nin the Tools Database. If nothing is found\nin the Tools DB then a default tool is added.")));
    lay5->addWidget(add_newtool_button, 0);
    addtool_from_db_btn = new FCButton(nullptr);
    addtool_from_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    addtool_from_db_btn->setText(trUi_(QString::fromUtf8("Pick from DB")));
    addtool_from_db_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nfrom the Tools Database.\nTools database administration in in:\nMenu: Options -> Tools Database")));
    lay5->addWidget(addtool_from_db_btn, 0);
    lay3->addLayout(lay5, 16, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay3->addWidget(w6, 18, 0, 1, 2);
    param_label = new QLabel(nullptr);
    param_label->setText(trUi_(QString::fromUtf8("<b>Tool Parameters:</b>")));
    lay3->addWidget(param_label, 20, 0, 1, 2);
    auto* w7 = new QLabel(nullptr);
    w7->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w7->setToolTip(trUi_(QString::fromUtf8("Cutting depth (negative)\nbelow the copper surface.")));
    lay3->addWidget(w7, 22, 0, 1, 1);
    cutz_entry = new FCDoubleSpinner(nullptr);
    cutz_entry->setProperty("decimals", 4);
    cutz_entry->setProperty("maximum", -0.0);
    cutz_entry->setProperty("minimum", -10000.0);
    cutz_entry->setProperty("singleStep", 0.1);
    cutz_entry->setProperty("value", -1.8);
    lay3->addWidget(cutz_entry, 22, 1, 1, 1);
    mpass_cb = new FCCheckBox(nullptr);
    mpass_cb->setChecked(true);
    mpass_cb->setText(trUi_(QString::fromUtf8("Multi-Depth:")));
    mpass_cb->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    lay3->addWidget(mpass_cb, 24, 0, 1, 1);
    maxdepth_entry = new FCDoubleSpinner(nullptr);
    maxdepth_entry->setProperty("decimals", 4);
    maxdepth_entry->setProperty("maximum", 10000.0);
    maxdepth_entry->setProperty("singleStep", 0.1);
    maxdepth_entry->setToolTip(trUi_(QString::fromUtf8("Depth of each pass (positive).")));
    maxdepth_entry->setProperty("value", 0.6);
    lay3->addWidget(maxdepth_entry, 24, 1, 1, 1);
    margin_label = new QLabel(nullptr);
    margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    margin_label->setToolTip(trUi_(QString::fromUtf8("Margin over bounds. A positive value here\nwill make the cutout of the PCB further from\nthe actual PCB border")));
    lay3->addWidget(margin_label, 26, 0, 1, 1);
    margin = new FCDoubleSpinner(nullptr);
    margin->setProperty("decimals", 4);
    margin->setProperty("maximum", 10000.0);
    margin->setProperty("minimum", -10000.0);
    margin->setProperty("singleStep", 0.1);
    margin->setProperty("value", 0.1);
    lay3->addWidget(margin, 26, 1, 1, 1);
    gapsize_label = new QLabel(nullptr);
    gapsize_label->setText(trUi_(QString::fromUtf8("Gap size:")));
    gapsize_label->setToolTip(trUi_(QString::fromUtf8("The size of the bridge gaps in the cutout\nused to keep the board connected to\nthe surrounding material (the one \nfrom which the PCB is cutout).")));
    lay3->addWidget(gapsize_label, 28, 0, 1, 1);
    gapsize = new FCDoubleSpinner(nullptr);
    gapsize->setProperty("decimals", 4);
    gapsize->setProperty("value", 4.0);
    lay3->addWidget(gapsize, 28, 1, 1, 1);
    gaptype_label = new FCLabel(nullptr);
    gaptype_label->setText(trUi_(QString::fromUtf8("Gap type:")));
    gaptype_label->setToolTip(trUi_(QString::fromUtf8("The type of gap:\n- Bridge -> the cutout will be interrupted by bridges\n- Thin -> same as 'bridge' but it will be thinner by partially milling the gap\n- M-Bites -> 'Mouse Bites' - same as 'bridge' but covered with drill holes")));
    lay3->addWidget(gaptype_label, 30, 0, 1, 1);
    gaptype_radio = new RadioSet({{trUi_(QString::fromUtf8("Bridge")), QVariant(QString::fromUtf8("b"))}, {trUi_(QString::fromUtf8("Thin")), QVariant(QString::fromUtf8("bt"))}, {trUi_(QString::fromUtf8("M-Bites")), QVariant(QString::fromUtf8("mb"))}}, Qt::Horizontal, true, nullptr);
    gaptype_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay3->addWidget(gaptype_radio, 30, 1, 1, 1);
    thin_depth_label = new FCLabel(nullptr);
    thin_depth_label->setText(trUi_(QString::fromUtf8("Depth:")));
    thin_depth_label->setToolTip(trUi_(QString::fromUtf8("The depth until the milling is done\nin order to thin the gaps.")));
    thin_depth_label->setHidden(true);
    lay3->addWidget(thin_depth_label, 32, 0, 1, 1);
    thin_depth_entry = new FCDoubleSpinner(nullptr);
    thin_depth_entry->setProperty("decimals", 4);
    thin_depth_entry->setProperty("maximum", -0.0);
    thin_depth_entry->setProperty("minimum", -10000.0);
    thin_depth_entry->setProperty("singleStep", 0.1);
    thin_depth_entry->setProperty("value", -1.0);
    thin_depth_entry->setHidden(true);
    lay3->addWidget(thin_depth_entry, 32, 1, 1, 1);
    mb_dia_label = new FCLabel(nullptr);
    mb_dia_label->setText(trUi_(QString::fromUtf8("Tool Diameter:")));
    mb_dia_label->setToolTip(trUi_(QString::fromUtf8("The drill hole diameter when doing mouse bites.")));
    mb_dia_label->setHidden(true);
    lay3->addWidget(mb_dia_label, 34, 0, 1, 1);
    mb_dia_entry = new FCDoubleSpinner(nullptr);
    mb_dia_entry->setProperty("decimals", 4);
    mb_dia_entry->setProperty("maximum", 100.0);
    mb_dia_entry->setProperty("value", 0.6);
    mb_dia_entry->setHidden(true);
    lay3->addWidget(mb_dia_entry, 34, 1, 1, 1);
    mb_spacing_label = new FCLabel(nullptr);
    mb_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    mb_spacing_label->setToolTip(trUi_(QString::fromUtf8("The spacing between drill holes when doing mouse bites.")));
    mb_spacing_label->setHidden(true);
    lay3->addWidget(mb_spacing_label, 36, 0, 1, 1);
    mb_spacing_entry = new FCDoubleSpinner(nullptr);
    mb_spacing_entry->setProperty("decimals", 4);
    mb_spacing_entry->setProperty("maximum", 100.0);
    mb_spacing_entry->setProperty("value", 0.3);
    mb_spacing_entry->setHidden(true);
    lay3->addWidget(mb_spacing_entry, 36, 1, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay3->addWidget(w8, 38, 0, 1, 2);
    auto* w9 = new QLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("<b>Automatic Bridge Gaps</b>:")));
    w9->setToolTip(trUi_(QString::fromUtf8("This section handle creation of automatic bridge gaps.")));
    lay3->addWidget(w9, 40, 0, 1, 2);
    auto* w10 = new QLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Gaps:")));
    w10->setToolTip(trUi_(QString::fromUtf8("Number of gaps used for the Automatic cutout.\nThere can be maximum 8 bridges/gaps.\nThe choices are:\n- None  - no gaps\n- lr    - left + right\n- tb    - top + bottom\n- 4     - left + right +top + bottom\n- 2lr   - 2*left + 2*right\n- 2tb  - 2*top + 2*bottom\n- 8     - 2*left + 2*right +2*top + 2*bottom")));
    lay3->addWidget(w10, 42, 0, 1, 1);
    gaps = new FCComboBox(nullptr);
    gaps->setProperty("currentText", trUi_(QString::fromUtf8("4")));
    gaps->addItem(trUi_(QString::fromUtf8("None")));
    gaps->addItem(trUi_(QString::fromUtf8("LR")));
    gaps->addItem(trUi_(QString::fromUtf8("TB")));
    gaps->addItem(trUi_(QString::fromUtf8("4")));
    gaps->addItem(trUi_(QString::fromUtf8("2LR")));
    gaps->addItem(trUi_(QString::fromUtf8("2TB")));
    gaps->addItem(trUi_(QString::fromUtf8("8")));
    gaps->setCurrentIndex(3);
    lay3->addWidget(gaps, 42, 1, 1, 1);
    ff_cutout_object_btn = new FCButton(nullptr);
    { QFont f = ff_cutout_object_btn->font(); f.setBold(true); ff_cutout_object_btn->setFont(f); }
    ff_cutout_object_btn->setIcon(QIcon(QString::fromUtf8(":/images/irregular32.png")));
    ff_cutout_object_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    ff_cutout_object_btn->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    ff_cutout_object_btn->setToolTip(trUi_(QString::fromUtf8("Cutout the selected object.\nThe cutout shape can be of any shape.\nUseful when the PCB has a non-rectangular shape.")));
    lay3->addWidget(ff_cutout_object_btn, 44, 0, 1, 2);
    rect_cutout_object_btn = new FCButton(nullptr);
    { QFont f = rect_cutout_object_btn->font(); f.setBold(true); rect_cutout_object_btn->setFont(f); }
    rect_cutout_object_btn->setIcon(QIcon(QString::fromUtf8(":/images/rectangle32.png")));
    rect_cutout_object_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    rect_cutout_object_btn->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    rect_cutout_object_btn->setToolTip(trUi_(QString::fromUtf8("Cutout the selected object.\nThe resulting cutout shape is\nalways a rectangle shape and it will be\nthe bounding box of the Object.")));
    lay3->addWidget(rect_cutout_object_btn, 46, 0, 1, 2);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    lay3->addWidget(w11, 48, 0, 1, 2);
    auto* w12 = new QLabel(nullptr);
    w12->setText(trUi_(QString::fromUtf8("<b>Manual Bridge Gaps</b>:")));
    w12->setToolTip(trUi_(QString::fromUtf8("This section handle creation of manual bridge gaps.\nThis is done by mouse clicking on the perimeter of the\nGeometry object that is used as a cutout object. ")));
    lay3->addWidget(w12, 50, 0, 1, 2);
    auto* w13 = new QLabel(nullptr);
    w13->setText(trUi_(QString::fromUtf8("Big cursor:")));
    w13->setToolTip(trUi_(QString::fromUtf8("Use a big cursor when adding manual gaps.")));
    lay3->addWidget(w13, 52, 0, 1, 1);
    big_cursor_cb = new FCCheckBox(nullptr);
    big_cursor_cb->setChecked(true);
    lay3->addWidget(big_cursor_cb, 52, 1, 1, 1);
    man_geo_creation_btn = new FCButton(nullptr);
    man_geo_creation_btn->setIcon(QIcon(QString::fromUtf8(":/images/rectangle32.png")));
    man_geo_creation_btn->setText(trUi_(QString::fromUtf8("Generate Manual Geometry")));
    man_geo_creation_btn->setToolTip(trUi_(QString::fromUtf8("If the object to be cutout is a Gerber\nfirst create a Geometry that surrounds it,\nto be used as the cutout, if one doesn't exist yet.\nSelect the source Gerber file in the top object combobox.")));
    lay3->addWidget(man_geo_creation_btn, 54, 0, 1, 2);
    man_object_label = new QLabel(nullptr);
    man_object_label->setText(trUi_(QString::fromUtf8("Manual cutout Geometry:")));
    man_object_label->setToolTip(trUi_(QString::fromUtf8("Geometry object used to create the manual cutout.")));
    lay3->addWidget(man_object_label, 56, 0, 1, 2);
    man_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(man_object_combo, 56, 0, 1, 2);
    man_gaps_creation_btn = new FCButton(nullptr);
    { QFont f = man_gaps_creation_btn->font(); f.setBold(true); man_gaps_creation_btn->setFont(f); }
    man_gaps_creation_btn->setIcon(QIcon(QString::fromUtf8(":/images/gaps32.png")));
    man_gaps_creation_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    man_gaps_creation_btn->setText(trUi_(QString::fromUtf8("Manual Add Bridge Gaps")));
    man_gaps_creation_btn->setToolTip(trUi_(QString::fromUtf8("Use the left mouse button (LMB) click\nto create a bridge gap to separate the PCB from\nthe surrounding material.\nThe LMB click has to be done on the perimeter of\nthe Geometry object used as a cutout geometry.")));
    lay3->addWidget(man_gaps_creation_btn, 58, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
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

std::map<std::string, QObject*> CutOutToolUIBase::membersByName() const {
    return {
        {"add_newtool_button", add_newtool_button},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"big_cursor_cb", big_cursor_cb},
        {"convex_box_cb", convex_box_cb},
        {"convex_box_label", convex_box_label},
        {"cutz_entry", cutz_entry},
        {"dia", dia},
        {"dia_label", dia_label},
        {"ff_cutout_object_btn", ff_cutout_object_btn},
        {"gaps", gaps},
        {"gapsize", gapsize},
        {"gapsize_label", gapsize_label},
        {"gaptype_label", gaptype_label},
        {"gaptype_radio", gaptype_radio},
        {"kindlabel", kindlabel},
        {"layout", layout},
        {"man_gaps_creation_btn", man_gaps_creation_btn},
        {"man_geo_creation_btn", man_geo_creation_btn},
        {"man_object_combo", man_object_combo},
        {"man_object_label", man_object_label},
        {"margin", margin},
        {"margin_label", margin_label},
        {"maxdepth_entry", maxdepth_entry},
        {"mb_dia_entry", mb_dia_entry},
        {"mb_dia_label", mb_dia_label},
        {"mb_spacing_entry", mb_spacing_entry},
        {"mb_spacing_label", mb_spacing_label},
        {"mpass_cb", mpass_cb},
        {"obj_combo", obj_combo},
        {"obj_kind_combo", obj_kind_combo},
        {"object_label", object_label},
        {"param_label", param_label},
        {"rect_cutout_object_btn", rect_cutout_object_btn},
        {"reset_button", reset_button},
        {"thin_depth_entry", thin_depth_entry},
        {"thin_depth_label", thin_depth_label},
        {"tool_sel_label", tool_sel_label},
        {"type_obj_combo_label", type_obj_combo_label},
        {"type_obj_radio", type_obj_radio},
    };
}

}  // namespace flatcam::gui
