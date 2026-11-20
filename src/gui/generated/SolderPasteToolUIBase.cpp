// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/SolderPasteToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void SolderPasteToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Solder Paste Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QLabel\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-size: 16px;\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    w1->setText(trUi_(QString::fromUtf8("Solder Paste Tool")));
    layout->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    object_label = new QLabel(nullptr);
    object_label->setText(trUi_(QString::fromUtf8("<b>GERBER</b>:")));
    object_label->setToolTip(trUi_(QString::fromUtf8("Gerber Solderpaste object.")));
    lay2->setWidget(0, QFormLayout::ItemRole(2), object_label);
    obj_combo = new FCComboBox(nullptr);
    lay2->setWidget(1, QFormLayout::ItemRole(2), obj_combo);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->setWidget(2, QFormLayout::ItemRole(2), w3);
    layout->addLayout(lay2, 0);
    tools_table_label = new QLabel(nullptr);
    tools_table_label->setText(trUi_(QString::fromUtf8("<b>Tools Table</b>")));
    tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools pool from which the algorithm\nwill pick the ones used for dispensing solder paste.")));
    layout->addWidget(tools_table_label, 0);
    tools_table = new FCTable(nullptr);
    tools_table->setProperty("columnCount", 3);
    tools_table->setProperty("contextMenuPolicy", 2);
    tools_table->setMaximumSize(QSize(16777215, 72));
    tools_table->setMinimumSize(QSize(0, 72));
    tools_table->setProperty("rowCount", 2);
    tools_table->setProperty("verticalScrollBarPolicy", 1);
    tools_table->setColumnCount(3);
    tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Diameter")), trUi_(QString::fromUtf8(""))});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    tools_table->setColumnHidden(2, true);
    layout->addWidget(tools_table, 0);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    addtool_entry_lbl = new QLabel(nullptr);
    addtool_entry_lbl->setText(trUi_(QString::fromUtf8("<b>New Nozzle Tool:</b>")));
    addtool_entry_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool to add in the Tool Table")));
    lay4->addWidget(addtool_entry_lbl, 0);
    addtool_entry = new FCDoubleSpinner(nullptr);
    addtool_entry->setProperty("decimals", 4);
    addtool_entry->setProperty("maximum", 10000.0);
    addtool_entry->setProperty("singleStep", 0.1);
    addtool_entry->setProperty("value", 0.3);
    lay4->addWidget(addtool_entry, 0);
    layout->addLayout(lay4, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    addtool_btn = new QPushButton(nullptr);
    addtool_btn->setText(trUi_(QString::fromUtf8("Add")));
    addtool_btn->setToolTip(trUi_(QString::fromUtf8("Add a new nozzle tool to the Tool Table\nwith the diameter specified above.")));
    lay5->addWidget(addtool_btn, 0, 0, 1, 1);
    deltool_btn = new QPushButton(nullptr);
    deltool_btn->setText(trUi_(QString::fromUtf8("Delete")));
    deltool_btn->setToolTip(trUi_(QString::fromUtf8("Delete a selection of tools in the Tool Table\nby first selecting a row in the Tool Table.")));
    lay5->addWidget(deltool_btn, 0, 2, 1, 1);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay5->addWidget(w6, 1, 0, 1, 3);
    layout->addLayout(lay5, 0);
    auto* lay7 = new QGridLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    auto* w8 = new QLabel(nullptr);
    w8->setText(trUi_(QString::fromUtf8("<b>STEP 1:</b>")));
    w8->setToolTip(trUi_(QString::fromUtf8("First step is to select a number of nozzle tools for usage\nand then optionally modify the GCode parameters below.")));
    lay7->addWidget(w8, 0, 0, 1, 1, Qt::Alignment(32));
    auto* w9 = new QLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("Select tools.\nModify parameters.")));
    lay7->addWidget(w9, 0, 2, 1, 1, Qt::Alignment(64));
    layout->addLayout(lay7, 0);
    gcode_frame = new QFrame(nullptr);
    gcode_box = new QVBoxLayout();
    gcode_box->setContentsMargins(0, 0, 0, 0);
    gcode_box->setSpacing(6);
    gcode_form_layout = new QFormLayout();
    gcode_form_layout->setContentsMargins(0, 0, 0, 0);
    gcode_form_layout->setHorizontalSpacing(6);
    gcode_form_layout->setVerticalSpacing(6);
    z_start_label = new QLabel(nullptr);
    z_start_label->setText(trUi_(QString::fromUtf8("Z Dispense Start:")));
    z_start_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) when solder paste dispensing starts.")));
    gcode_form_layout->setWidget(0, QFormLayout::ItemRole(0), z_start_label);
    z_start_entry = new FCDoubleSpinner(nullptr);
    z_start_entry->setProperty("decimals", 4);
    z_start_entry->setProperty("maximum", 10000.0);
    z_start_entry->setProperty("singleStep", 0.1);
    z_start_entry->setProperty("value", 0.05);
    gcode_form_layout->setWidget(0, QFormLayout::ItemRole(1), z_start_entry);
    z_dispense_label = new QLabel(nullptr);
    z_dispense_label->setText(trUi_(QString::fromUtf8("Z Dispense:")));
    z_dispense_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) when doing solder paste dispensing.")));
    gcode_form_layout->setWidget(1, QFormLayout::ItemRole(0), z_dispense_label);
    z_dispense_entry = new FCDoubleSpinner(nullptr);
    z_dispense_entry->setProperty("decimals", 4);
    z_dispense_entry->setProperty("maximum", 10000.0);
    z_dispense_entry->setProperty("singleStep", 0.1);
    z_dispense_entry->setProperty("value", 0.1);
    gcode_form_layout->setWidget(1, QFormLayout::ItemRole(1), z_dispense_entry);
    z_stop_label = new QLabel(nullptr);
    z_stop_label->setText(trUi_(QString::fromUtf8("Z Dispense Stop:")));
    z_stop_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) when solder paste dispensing stops.")));
    gcode_form_layout->setWidget(2, QFormLayout::ItemRole(0), z_stop_label);
    z_stop_entry = new FCDoubleSpinner(nullptr);
    z_stop_entry->setProperty("decimals", 4);
    z_stop_entry->setProperty("maximum", 10000.0);
    z_stop_entry->setProperty("singleStep", 0.1);
    z_stop_entry->setProperty("value", 0.05);
    gcode_form_layout->setWidget(2, QFormLayout::ItemRole(1), z_stop_entry);
    z_travel_label = new QLabel(nullptr);
    z_travel_label->setText(trUi_(QString::fromUtf8("Z Travel:")));
    z_travel_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) for travel between pads\n(without dispensing solder paste).")));
    gcode_form_layout->setWidget(3, QFormLayout::ItemRole(0), z_travel_label);
    z_travel_entry = new FCDoubleSpinner(nullptr);
    z_travel_entry->setProperty("decimals", 4);
    z_travel_entry->setProperty("maximum", 10000.0);
    z_travel_entry->setProperty("singleStep", 0.1);
    z_travel_entry->setProperty("value", 0.1);
    gcode_form_layout->setWidget(3, QFormLayout::ItemRole(1), z_travel_entry);
    z_toolchange_label = new QLabel(nullptr);
    z_toolchange_label->setText(trUi_(QString::fromUtf8("Z Toolchange:")));
    z_toolchange_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) for tool (nozzle) change.")));
    gcode_form_layout->setWidget(4, QFormLayout::ItemRole(0), z_toolchange_label);
    z_toolchange_entry = new FCDoubleSpinner(nullptr);
    z_toolchange_entry->setProperty("decimals", 4);
    z_toolchange_entry->setProperty("maximum", 10000.0);
    z_toolchange_entry->setProperty("singleStep", 0.1);
    z_toolchange_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(4, QFormLayout::ItemRole(1), z_toolchange_entry);
    xy_toolchange_label = new QLabel(nullptr);
    xy_toolchange_label->setText(trUi_(QString::fromUtf8("Toolchange X-Y:")));
    xy_toolchange_label->setToolTip(trUi_(QString::fromUtf8("The X,Y location for tool (nozzle) change.\nThe format is (x, y) where x and y are real numbers.")));
    gcode_form_layout->setWidget(5, QFormLayout::ItemRole(0), xy_toolchange_label);
    xy_toolchange_entry = new FCEntry(nullptr);
    xy_toolchange_entry->setText(trUi_(QString::fromUtf8("0.0, 0.0")));
    gcode_form_layout->setWidget(5, QFormLayout::ItemRole(1), xy_toolchange_entry);
    frxy_label = new QLabel(nullptr);
    frxy_label->setText(trUi_(QString::fromUtf8("Feedrate X-Y:")));
    frxy_label->setToolTip(trUi_(QString::fromUtf8("Feedrate (speed) while moving on the X-Y plane.")));
    gcode_form_layout->setWidget(6, QFormLayout::ItemRole(0), frxy_label);
    frxy_entry = new FCDoubleSpinner(nullptr);
    frxy_entry->setProperty("decimals", 4);
    frxy_entry->setProperty("maximum", 910000.0);
    frxy_entry->setProperty("singleStep", 0.1);
    frxy_entry->setProperty("value", 150.0);
    gcode_form_layout->setWidget(6, QFormLayout::ItemRole(1), frxy_entry);
    frz_label = new QLabel(nullptr);
    frz_label->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    frz_label->setToolTip(trUi_(QString::fromUtf8("Feedrate (speed) while moving vertically\n(on Z plane).")));
    gcode_form_layout->setWidget(7, QFormLayout::ItemRole(0), frz_label);
    frz_entry = new FCDoubleSpinner(nullptr);
    frz_entry->setProperty("decimals", 4);
    frz_entry->setProperty("maximum", 910000.0);
    frz_entry->setProperty("singleStep", 0.1);
    frz_entry->setProperty("value", 150.0);
    gcode_form_layout->setWidget(7, QFormLayout::ItemRole(1), frz_entry);
    frz_dispense_label = new QLabel(nullptr);
    frz_dispense_label->setText(trUi_(QString::fromUtf8("Feedrate Z Dispense:")));
    frz_dispense_label->setToolTip(trUi_(QString::fromUtf8("Feedrate (speed) while moving up vertically\nto Dispense position (on Z plane).")));
    gcode_form_layout->setWidget(8, QFormLayout::ItemRole(0), frz_dispense_label);
    frz_dispense_entry = new FCDoubleSpinner(nullptr);
    frz_dispense_entry->setProperty("decimals", 4);
    frz_dispense_entry->setProperty("maximum", 910000.0);
    frz_dispense_entry->setProperty("singleStep", 0.1);
    frz_dispense_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(8, QFormLayout::ItemRole(1), frz_dispense_entry);
    speedfwd_label = new QLabel(nullptr);
    speedfwd_label->setText(trUi_(QString::fromUtf8("Spindle Speed FWD:")));
    speedfwd_label->setToolTip(trUi_(QString::fromUtf8("The dispenser speed while pushing solder paste\nthrough the dispenser nozzle.")));
    gcode_form_layout->setWidget(9, QFormLayout::ItemRole(0), speedfwd_label);
    speedfwd_entry = new FCSpinner(nullptr);
    speedfwd_entry->setProperty("maximum", 999999);
    speedfwd_entry->setProperty("singleStep", 1000);
    speedfwd_entry->setProperty("value", 300);
    gcode_form_layout->setWidget(9, QFormLayout::ItemRole(1), speedfwd_entry);
    dwellfwd_label = new QLabel(nullptr);
    dwellfwd_label->setText(trUi_(QString::fromUtf8("Dwell FWD:")));
    dwellfwd_label->setToolTip(trUi_(QString::fromUtf8("Pause after solder dispensing.")));
    gcode_form_layout->setWidget(10, QFormLayout::ItemRole(0), dwellfwd_label);
    dwellfwd_entry = new FCDoubleSpinner(nullptr);
    dwellfwd_entry->setProperty("decimals", 4);
    dwellfwd_entry->setProperty("maximum", 10000.0);
    dwellfwd_entry->setProperty("singleStep", 0.1);
    dwellfwd_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(10, QFormLayout::ItemRole(1), dwellfwd_entry);
    speedrev_label = new QLabel(nullptr);
    speedrev_label->setText(trUi_(QString::fromUtf8("Spindle Speed REV:")));
    speedrev_label->setToolTip(trUi_(QString::fromUtf8("The dispenser speed while retracting solder paste\nthrough the dispenser nozzle.")));
    gcode_form_layout->setWidget(11, QFormLayout::ItemRole(0), speedrev_label);
    speedrev_entry = new FCSpinner(nullptr);
    speedrev_entry->setProperty("maximum", 999999);
    speedrev_entry->setProperty("singleStep", 1000);
    speedrev_entry->setProperty("value", 200);
    gcode_form_layout->setWidget(11, QFormLayout::ItemRole(1), speedrev_entry);
    dwellrev_label = new QLabel(nullptr);
    dwellrev_label->setText(trUi_(QString::fromUtf8("Dwell REV:")));
    dwellrev_label->setToolTip(trUi_(QString::fromUtf8("Pause after solder paste dispenser retracted,\nto allow pressure equilibrium.")));
    gcode_form_layout->setWidget(12, QFormLayout::ItemRole(0), dwellrev_label);
    dwellrev_entry = new FCDoubleSpinner(nullptr);
    dwellrev_entry->setProperty("decimals", 4);
    dwellrev_entry->setProperty("maximum", 10000.0);
    dwellrev_entry->setProperty("singleStep", 0.1);
    dwellrev_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(12, QFormLayout::ItemRole(1), dwellrev_entry);
    auto* w10 = new QLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Preprocessor:")));
    w10->setToolTip(trUi_(QString::fromUtf8("Files that control the GCode generation.")));
    gcode_form_layout->setWidget(13, QFormLayout::ItemRole(0), w10);
    pp_combo = new FCComboBox(nullptr);
    gcode_form_layout->setWidget(13, QFormLayout::ItemRole(1), pp_combo);
    gcode_box->addLayout(gcode_form_layout, 0);
    gcode_frame->setLayout(gcode_box);
    layout->addWidget(gcode_frame, 0);
    generation_frame = new QFrame(nullptr);
    generation_box = new QVBoxLayout();
    generation_box->setContentsMargins(0, 0, 0, 0);
    generation_box->setSpacing(6);
    auto* lay11 = new QGridLayout();
    lay11->setContentsMargins(0, 0, 0, 0);
    lay11->setHorizontalSpacing(6);
    lay11->setVerticalSpacing(6);
    auto* w12 = new QLabel(nullptr);
    w12->setText(trUi_(QString::fromUtf8("<b>STEP 2:</b>")));
    w12->setToolTip(trUi_(QString::fromUtf8("Second step is to create a solder paste dispensing\ngeometry out of an Solder Paste Mask Gerber file.")));
    lay11->addWidget(w12, 0, 0, 1, 1);
    soldergeo_btn = new QPushButton(nullptr);
    { QFont f = soldergeo_btn->font(); f.setBold(true); soldergeo_btn->setFont(f); }
    soldergeo_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t\t\t\t\t   "));
    soldergeo_btn->setText(trUi_(QString::fromUtf8("Generate Geo")));
    soldergeo_btn->setToolTip(trUi_(QString::fromUtf8("Generate solder paste dispensing geometry.")));
    lay11->addWidget(soldergeo_btn, 0, 2, 1, 1);
    generation_box->addLayout(lay11, 0);
    auto* lay13 = new QFormLayout();
    lay13->setContentsMargins(0, 0, 0, 0);
    lay13->setHorizontalSpacing(6);
    lay13->setVerticalSpacing(6);
    geo_object_label = new QLabel(nullptr);
    geo_object_label->setText(trUi_(QString::fromUtf8("Geo Result:")));
    geo_object_label->setToolTip(trUi_(QString::fromUtf8("Geometry Solder Paste object.\nThe name of the object has to end in:\n'_solderpaste' as a protection.")));
    lay13->setWidget(0, QFormLayout::ItemRole(0), geo_object_label);
    geo_obj_combo = new FCComboBox(nullptr);
    lay13->setWidget(0, QFormLayout::ItemRole(1), geo_obj_combo);
    generation_box->addLayout(lay13, 0);
    auto* lay14 = new QGridLayout();
    lay14->setContentsMargins(0, 0, 0, 0);
    lay14->setHorizontalSpacing(6);
    lay14->setVerticalSpacing(6);
    auto* w15 = new QLabel(nullptr);
    w15->setText(trUi_(QString::fromUtf8("<b>STEP 3:</b>")));
    w15->setToolTip(trUi_(QString::fromUtf8("Third step is to select a solder paste dispensing geometry,\nand then generate a CNCJob object.\n\nREMEMBER: if you want to create a CNCJob with new parameters,\nfirst you need to generate a geometry with those new params,\nand only after that you can generate an updated CNCJob.")));
    lay14->addWidget(w15, 0, 0, 1, 1);
    solder_gcode_btn = new QPushButton(nullptr);
    { QFont f = solder_gcode_btn->font(); f.setBold(true); solder_gcode_btn->setFont(f); }
    solder_gcode_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    solder_gcode_btn->setText(trUi_(QString::fromUtf8("Generate GCode")));
    solder_gcode_btn->setToolTip(trUi_(QString::fromUtf8("Generate GCode for Solder Paste dispensing\non PCB pads.")));
    lay14->addWidget(solder_gcode_btn, 0, 2, 1, 1);
    generation_box->addLayout(lay14, 0);
    auto* lay16 = new QFormLayout();
    lay16->setContentsMargins(0, 0, 0, 0);
    lay16->setHorizontalSpacing(6);
    lay16->setVerticalSpacing(6);
    cnc_object_label = new QLabel(nullptr);
    cnc_object_label->setText(trUi_(QString::fromUtf8("CNC Result:")));
    cnc_object_label->setToolTip(trUi_(QString::fromUtf8("CNCJob Solder paste object.\nIn order to enable the GCode save section,\nthe name of the object has to end in:\n'_solderpaste' as a protection.")));
    lay16->setWidget(0, QFormLayout::ItemRole(0), cnc_object_label);
    cnc_obj_combo = new FCComboBox(nullptr);
    lay16->setWidget(0, QFormLayout::ItemRole(1), cnc_obj_combo);
    generation_box->addLayout(lay16, 0);
    auto* lay17 = new QGridLayout();
    lay17->setContentsMargins(0, 0, 0, 0);
    lay17->setHorizontalSpacing(6);
    lay17->setVerticalSpacing(6);
    auto* w18 = new QLabel(nullptr);
    w18->setText(trUi_(QString::fromUtf8("<b>STEP 4:</b>")));
    w18->setToolTip(trUi_(QString::fromUtf8("Fourth step (and last) is to select a CNCJob made from \na solder paste dispensing geometry, and then view/save it's GCode.")));
    lay17->addWidget(w18, 0, 0, 1, 1);
    solder_gcode_view_btn = new QPushButton(nullptr);
    { QFont f = solder_gcode_view_btn->font(); f.setBold(true); solder_gcode_view_btn->setFont(f); }
    solder_gcode_view_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    solder_gcode_view_btn->setText(trUi_(QString::fromUtf8("View GCode")));
    solder_gcode_view_btn->setToolTip(trUi_(QString::fromUtf8("View the generated GCode for Solder Paste dispensing\non PCB pads.")));
    lay17->addWidget(solder_gcode_view_btn, 0, 2, 1, 1);
    solder_gcode_save_btn = new QPushButton(nullptr);
    { QFont f = solder_gcode_save_btn->font(); f.setBold(true); solder_gcode_save_btn->setFont(f); }
    solder_gcode_save_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    solder_gcode_save_btn->setText(trUi_(QString::fromUtf8("Save GCode")));
    solder_gcode_save_btn->setToolTip(trUi_(QString::fromUtf8("Save the generated GCode for Solder Paste dispensing\non PCB pads, to a file.")));
    lay17->addWidget(solder_gcode_save_btn, 1, 0, 1, 3);
    generation_box->addLayout(lay17, 0);
    generation_frame->setLayout(generation_box);
    layout->addWidget(generation_frame, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> SolderPasteToolUIBase::membersByName() const {
    return {
        {"addtool_btn", addtool_btn},
        {"addtool_entry", addtool_entry},
        {"addtool_entry_lbl", addtool_entry_lbl},
        {"cnc_obj_combo", cnc_obj_combo},
        {"cnc_object_label", cnc_object_label},
        {"deltool_btn", deltool_btn},
        {"dwellfwd_entry", dwellfwd_entry},
        {"dwellfwd_label", dwellfwd_label},
        {"dwellrev_entry", dwellrev_entry},
        {"dwellrev_label", dwellrev_label},
        {"frxy_entry", frxy_entry},
        {"frxy_label", frxy_label},
        {"frz_dispense_entry", frz_dispense_entry},
        {"frz_dispense_label", frz_dispense_label},
        {"frz_entry", frz_entry},
        {"frz_label", frz_label},
        {"gcode_box", gcode_box},
        {"gcode_form_layout", gcode_form_layout},
        {"gcode_frame", gcode_frame},
        {"generation_box", generation_box},
        {"generation_frame", generation_frame},
        {"geo_obj_combo", geo_obj_combo},
        {"geo_object_label", geo_object_label},
        {"layout", layout},
        {"obj_combo", obj_combo},
        {"object_label", object_label},
        {"pp_combo", pp_combo},
        {"reset_button", reset_button},
        {"solder_gcode_btn", solder_gcode_btn},
        {"solder_gcode_save_btn", solder_gcode_save_btn},
        {"solder_gcode_view_btn", solder_gcode_view_btn},
        {"soldergeo_btn", soldergeo_btn},
        {"speedfwd_entry", speedfwd_entry},
        {"speedfwd_label", speedfwd_label},
        {"speedrev_entry", speedrev_entry},
        {"speedrev_label", speedrev_label},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"xy_toolchange_entry", xy_toolchange_entry},
        {"xy_toolchange_label", xy_toolchange_label},
        {"z_dispense_entry", z_dispense_entry},
        {"z_dispense_label", z_dispense_label},
        {"z_start_entry", z_start_entry},
        {"z_start_label", z_start_label},
        {"z_stop_entry", z_stop_entry},
        {"z_stop_label", z_stop_label},
        {"z_toolchange_entry", z_toolchange_entry},
        {"z_toolchange_label", z_toolchange_label},
        {"z_travel_entry", z_travel_entry},
        {"z_travel_label", z_travel_label},
    };
}

}  // namespace flatcam::gui
