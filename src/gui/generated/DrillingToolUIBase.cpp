// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DrillingToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void DrillingToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Drilling Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Drilling Tool")));
    w1->setToolTip(trUi_(QString::fromUtf8("Create CNCJob with toolpaths for drilling or milling holes.")));
    title_box->addWidget(w1, 0);
    level = new QLabel(nullptr);
    level->setProperty("alignment", 130);
    level->setText(trUi_(QString::fromUtf8("<span style=\"color:green;\"><b>Basic</b></span>")));
    level->setToolTip(trUi_(QString::fromUtf8("BASIC is suitable for a beginner. Many parameters\nare hidden from the user in this mode.\nADVANCED mode will make available all parameters.\n\nTo change the application LEVEL, go to:\nEdit -> Preferences -> General and check:\n'APP. LEVEL' radio button.")));
    title_box->addWidget(level, 0);
    tools_box->addLayout(title_box, 0);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    obj_combo_label = new QLabel(nullptr);
    obj_combo_label->setText(trUi_(QString::fromUtf8("<b>EXCELLON</b>:")));
    obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Excellon object for drilling/milling operation.")));
    lay2->addWidget(obj_combo_label, 0, 0, 1, 2);
    object_combo = new FCComboBox(nullptr);
    lay2->addWidget(object_combo, 1, 0, 1, 2);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->addWidget(w3, 2, 0, 1, 2);
    tools_table = new FCTable(nullptr);
    tools_table->setAcceptDrops(true);
    tools_table->setProperty("columnCount", 5);
    tools_table->setProperty("dragDropMode", 4);
    tools_table->setDragDropOverwriteMode(false);
    tools_table->setDragEnabled(true);
    tools_table->setProperty("horizontalScrollBarPolicy", 1);
    tools_table->setMaximumSize(QSize(16777215, 72));
    tools_table->setMinimumSize(QSize(0, 72));
    tools_table->setProperty("rowCount", 2);
    tools_table->setProperty("selectionBehavior", 1);
    tools_table->setProperty("verticalScrollBarPolicy", 1);
    tools_table->setColumnCount(5);
    tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Diameter")), trUi_(QString::fromUtf8("Drills")), trUi_(QString::fromUtf8("")), trUi_(QString::fromUtf8("Slots"))});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    tools_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode(3));
    tools_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeMode(3));
    tools_table->setColumnHidden(3, true);
    lay2->addWidget(tools_table, 3, 0, 1, 2);
    order_label = new QLabel(nullptr);
    order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay2->addWidget(order_label, 4, 0, 1, 1);
    order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    order_radio->setValue(QVariant(QString::fromUtf8("no")));
    lay2->addWidget(order_radio, 4, 1, 1, 1);
    search_load_db_btn = new FCButton(nullptr);
    search_load_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    search_load_db_btn->setText(trUi_(QString::fromUtf8("Search DB")));
    search_load_db_btn->setToolTip(trUi_(QString::fromUtf8("Will search and try to replace the tools from Tools Table\nwith tools from DB that have a close diameter value.")));
    lay2->addWidget(search_load_db_btn, 5, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay2->addWidget(w4, 6, 0, 1, 2);
    tool_data_label = new QLabel(nullptr);
    tool_data_label->setText(trUi_(QString::fromUtf8("<b>Parameters for: <font color='#0000FF'>No Tool Selected</font></b>")));
    tool_data_label->setToolTip(trUi_(QString::fromUtf8("The data used for creating GCode.\nEach tool store it's own set of such data.")));
    lay2->addWidget(tool_data_label, 8, 0, 1, 2);
    exc_param_frame = new QFrame(nullptr);
    exc_param_frame->setEnabled(false);
    exc_tools_box = new QVBoxLayout();
    exc_tools_box->setContentsMargins(0, 0, 0, 0);
    exc_tools_box->setSpacing(6);
    grid1 = new QGridLayout();
    grid1->setContentsMargins(0, 0, 0, 0);
    grid1->setHorizontalSpacing(6);
    grid1->setVerticalSpacing(6);
    cutzlabel = new QLabel(nullptr);
    cutzlabel->setText(trUi_(QString::fromUtf8("Cut Z:")));
    cutzlabel->setToolTip(trUi_(QString::fromUtf8("Drill depth (negative)\nbelow the copper surface.")));
    grid1->addWidget(cutzlabel, 4, 0, 1, 1);
    cutz_entry = new FCDoubleSpinner(nullptr);
    cutz_entry->setProperty("decimals", 4);
    cutz_entry->setProperty("maximum", 0.0);
    cutz_entry->setProperty("minimum", -10000.0);
    cutz_entry->setProperty("objectName", QString::fromUtf8("e_cutz"));
    cutz_entry->setProperty("singleStep", 0.1);
    cutz_entry->setProperty("value", -1.7);
    grid1->addWidget(cutz_entry, 4, 1, 1, 1);
    mpass_cb = new FCCheckBox(nullptr);
    mpass_cb->setProperty("objectName", QString::fromUtf8("e_multidepth"));
    mpass_cb->setText(trUi_(QString::fromUtf8("Multi-Depth:")));
    mpass_cb->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    grid1->addWidget(mpass_cb, 5, 0, 1, 1);
    maxdepth_entry = new FCDoubleSpinner(nullptr);
    maxdepth_entry->setProperty("decimals", 4);
    maxdepth_entry->setProperty("maximum", 10000.0);
    maxdepth_entry->setProperty("objectName", QString::fromUtf8("e_depthperpass"));
    maxdepth_entry->setProperty("singleStep", 0.1);
    maxdepth_entry->setToolTip(trUi_(QString::fromUtf8("Depth of each pass (positive).")));
    maxdepth_entry->setProperty("value", 0.7);
    grid1->addWidget(maxdepth_entry, 5, 1, 1, 1);
    travelzlabel = new QLabel(nullptr);
    travelzlabel->setText(trUi_(QString::fromUtf8("Travel Z:")));
    travelzlabel->setToolTip(trUi_(QString::fromUtf8("Tool height when travelling\nacross the XY plane.")));
    grid1->addWidget(travelzlabel, 6, 0, 1, 1);
    travelz_entry = new FCDoubleSpinner(nullptr);
    travelz_entry->setProperty("decimals", 4);
    travelz_entry->setProperty("maximum", 10000.0);
    travelz_entry->setProperty("objectName", QString::fromUtf8("e_travelz"));
    travelz_entry->setProperty("singleStep", 0.1);
    travelz_entry->setProperty("value", 2.0);
    grid1->addWidget(travelz_entry, 6, 1, 1, 1);
    frzlabel = new QLabel(nullptr);
    frzlabel->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    frzlabel->setToolTip(trUi_(QString::fromUtf8("Tool speed while drilling\n(in units per minute).\nSo called 'Plunge' feedrate.\nThis is for linear move G01.")));
    grid1->addWidget(frzlabel, 14, 0, 1, 1);
    feedrate_z_entry = new FCDoubleSpinner(nullptr);
    feedrate_z_entry->setProperty("decimals", 4);
    feedrate_z_entry->setProperty("maximum", 910000.0);
    feedrate_z_entry->setProperty("objectName", QString::fromUtf8("e_feedratez"));
    feedrate_z_entry->setProperty("singleStep", 0.1);
    feedrate_z_entry->setProperty("value", 300.0);
    grid1->addWidget(feedrate_z_entry, 14, 1, 1, 1);
    feedrate_rapid_label = new QLabel(nullptr);
    feedrate_rapid_label->setText(trUi_(QString::fromUtf8("Feedrate Rapids:")));
    feedrate_rapid_label->setToolTip(trUi_(QString::fromUtf8("Tool speed while drilling\n(in units per minute).\nThis is for the rapid move G00.\nIt is useful only for Marlin,\nignore for any other cases.")));
    feedrate_rapid_label->setHidden(true);
    grid1->addWidget(feedrate_rapid_label, 16, 0, 1, 1);
    feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    feedrate_rapid_entry->setProperty("decimals", 4);
    feedrate_rapid_entry->setProperty("maximum", 910000.0);
    feedrate_rapid_entry->setProperty("objectName", QString::fromUtf8("e_fr_rapid"));
    feedrate_rapid_entry->setProperty("singleStep", 0.1);
    feedrate_rapid_entry->setProperty("value", 1500.0);
    feedrate_rapid_entry->setHidden(true);
    grid1->addWidget(feedrate_rapid_entry, 16, 1, 1, 1);
    spindle_label = new QLabel(nullptr);
    spindle_label->setText(trUi_(QString::fromUtf8("Spindle speed:")));
    spindle_label->setToolTip(trUi_(QString::fromUtf8("Speed of the spindle\nin RPM (optional)")));
    grid1->addWidget(spindle_label, 19, 0, 1, 1);
    spindlespeed_entry = new FCSpinner(nullptr);
    spindlespeed_entry->setProperty("maximum", 1000000);
    spindlespeed_entry->setProperty("objectName", QString::fromUtf8("e_spindlespeed"));
    spindlespeed_entry->setProperty("singleStep", 100);
    grid1->addWidget(spindlespeed_entry, 19, 1, 1, 1);
    dwell_cb = new FCCheckBox(nullptr);
    dwell_cb->setProperty("objectName", QString::fromUtf8("e_dwell"));
    dwell_cb->setText(trUi_(QString::fromUtf8("Dwell:")));
    dwell_cb->setToolTip(trUi_(QString::fromUtf8("Pause to allow the spindle to reach its\nspeed before cutting.")));
    grid1->addWidget(dwell_cb, 20, 0, 1, 1);
    dwelltime_entry = new FCDoubleSpinner(nullptr);
    dwelltime_entry->setProperty("decimals", 4);
    dwelltime_entry->setProperty("maximum", 10000.0);
    dwelltime_entry->setProperty("objectName", QString::fromUtf8("e_dwelltime"));
    dwelltime_entry->setProperty("singleStep", 0.1);
    dwelltime_entry->setToolTip(trUi_(QString::fromUtf8("Number of time units for spindle to dwell.")));
    dwelltime_entry->setProperty("value", 1.0);
    grid1->addWidget(dwelltime_entry, 20, 1, 1, 1);
    tool_offset_label = new QLabel(nullptr);
    tool_offset_label->setText(trUi_(QString::fromUtf8("Offset Z:")));
    tool_offset_label->setToolTip(trUi_(QString::fromUtf8("Some drill bits (the larger ones) need to drill deeper\nto create the desired exit hole diameter due of the tip shape.\nThe value here can compensate the Cut Z parameter.")));
    grid1->addWidget(tool_offset_label, 25, 0, 1, 1);
    offset_entry = new FCDoubleSpinner(nullptr);
    offset_entry->setProperty("decimals", 4);
    offset_entry->setProperty("maximum", 10000.0);
    offset_entry->setProperty("minimum", -10000.0);
    offset_entry->setProperty("objectName", QString::fromUtf8("e_offset"));
    grid1->addWidget(offset_entry, 25, 1, 1, 1);
    drill_slots_cb = new FCCheckBox(nullptr);
    drill_slots_cb->setProperty("objectName", QString::fromUtf8("e_drill_slots"));
    drill_slots_cb->setText(trUi_(QString::fromUtf8("Drill slots")));
    drill_slots_cb->setToolTip(trUi_(QString::fromUtf8("If the selected tool has slots then they will be drilled.")));
    drill_slots_cb->setHidden(true);
    grid1->addWidget(drill_slots_cb, 27, 0, 1, 2);
    drill_overlap_label = new QLabel(nullptr);
    drill_overlap_label->setText(trUi_(QString::fromUtf8("Overlap:")));
    drill_overlap_label->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool diameter to overlap previous drill hole.")));
    drill_overlap_label->setHidden(true);
    grid1->addWidget(drill_overlap_label, 28, 0, 1, 1);
    drill_overlap_entry = new FCDoubleSpinner(nullptr);
    drill_overlap_entry->setProperty("decimals", 4);
    drill_overlap_entry->setProperty("maximum", 100.0);
    drill_overlap_entry->setProperty("objectName", QString::fromUtf8("e_drill_slots_overlap"));
    drill_overlap_entry->setProperty("singleStep", 0.1);
    drill_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    drill_overlap_entry->setHidden(true);
    grid1->addWidget(drill_overlap_entry, 28, 1, 1, 1);
    last_drill_cb = new FCCheckBox(nullptr);
    last_drill_cb->setChecked(true);
    last_drill_cb->setProperty("objectName", QString::fromUtf8("e_drill_last_drill"));
    last_drill_cb->setText(trUi_(QString::fromUtf8("Last drill")));
    last_drill_cb->setToolTip(trUi_(QString::fromUtf8("If the slot length is not completely covered by drill holes,\nadd a drill hole on the slot end point.")));
    last_drill_cb->setHidden(true);
    grid1->addWidget(last_drill_cb, 30, 0, 1, 2);
    grid1->setColumnStretch(1, 1);
    exc_tools_box->addLayout(grid1, 0);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    grid3->addWidget(w5, 0, 0, 1, 2);
    apply_param_to_all = new FCButton(nullptr);
    apply_param_to_all->setIcon(QIcon(QString::fromUtf8(":/images/param_all32.png")));
    apply_param_to_all->setText(trUi_(QString::fromUtf8("Apply parameters to all tools")));
    apply_param_to_all->setToolTip(trUi_(QString::fromUtf8("The parameters in the current form will be applied\non all the tools from the Tool Table.")));
    grid3->addWidget(apply_param_to_all, 1, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    grid3->addWidget(w6, 2, 0, 1, 2);
    gen_param_label = new QLabel(nullptr);
    gen_param_label->setText(trUi_(QString::fromUtf8("<b>Common Parameters</b>")));
    gen_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters that are common for all tools.")));
    grid3->addWidget(gen_param_label, 3, 0, 1, 2);
    toolchange_cb = new FCCheckBox(nullptr);
    toolchange_cb->setProperty("objectName", QString::fromUtf8("e_toolchange"));
    toolchange_cb->setText(trUi_(QString::fromUtf8("Tool change")));
    toolchange_cb->setToolTip(trUi_(QString::fromUtf8("Include tool-change sequence\nin G-Code (Pause for tool change).")));
    grid3->addWidget(toolchange_cb, 5, 0, 1, 2);
    toolchangez_label = new QLabel(nullptr);
    toolchangez_label->setText(trUi_(QString::fromUtf8("Tool change Z:")));
    toolchangez_label->setToolTip(trUi_(QString::fromUtf8("Z-axis position (height) for\ntool change.")));
    grid3->addWidget(toolchangez_label, 7, 0, 1, 1);
    toolchangez_entry = new FCDoubleSpinner(nullptr);
    toolchangez_entry->setProperty("decimals", 4);
    toolchangez_entry->setProperty("maximum", 10000.0);
    toolchangez_entry->setProperty("objectName", QString::fromUtf8("e_toolchangez"));
    toolchangez_entry->setProperty("singleStep", 0.1);
    toolchangez_entry->setProperty("value", 15.0);
    grid3->addWidget(toolchangez_entry, 7, 1, 1, 1);
    estartz_label = new QLabel(nullptr);
    estartz_label->setText(trUi_(QString::fromUtf8("Start Z:")));
    estartz_label->setToolTip(trUi_(QString::fromUtf8("Height of the tool just after starting the work.\nDelete the value if you don't need this feature.")));
    estartz_label->setHidden(true);
    grid3->addWidget(estartz_label, 9, 0, 1, 1);
    estartz_entry = new NumericalEvalEntry(nullptr);
    estartz_entry->setProperty("objectName", QString::fromUtf8("e_startz"));
    estartz_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    estartz_entry->setHidden(true);
    grid3->addWidget(estartz_entry, 9, 1, 1, 1);
    endz_label = new QLabel(nullptr);
    endz_label->setText(trUi_(QString::fromUtf8("End move Z:")));
    endz_label->setToolTip(trUi_(QString::fromUtf8("Height of the tool after\nthe last move at the end of the job.")));
    grid3->addWidget(endz_label, 11, 0, 1, 1);
    endz_entry = new FCDoubleSpinner(nullptr);
    endz_entry->setProperty("decimals", 4);
    endz_entry->setProperty("maximum", 10000.0);
    endz_entry->setProperty("objectName", QString::fromUtf8("e_endz"));
    endz_entry->setProperty("singleStep", 0.1);
    endz_entry->setProperty("value", 0.5);
    grid3->addWidget(endz_entry, 11, 1, 1, 1);
    auto* w7 = new QLabel(nullptr);
    w7->setText(trUi_(QString::fromUtf8("End move X,Y:")));
    w7->setToolTip(trUi_(QString::fromUtf8("End move X,Y position. In format (x,y).\nIf no value is entered then there is no move\non X,Y plane at the end of the job.")));
    grid3->addWidget(w7, 12, 0, 1, 1);
    endxy_entry = new NumericalEvalTupleEntry(nullptr);
    endxy_entry->setProperty("objectName", QString::fromUtf8("e_endxy"));
    endxy_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("X,Y coordinates")));
    endxy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    endxy_entry->setText(trUi_(QString::fromUtf8("None")));
    grid3->addWidget(endxy_entry, 12, 1, 1, 1);
    pdepth_label = new QLabel(nullptr);
    pdepth_label->setText(trUi_(QString::fromUtf8("Probe Z depth:")));
    pdepth_label->setToolTip(trUi_(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units.")));
    pdepth_label->setHidden(true);
    grid3->addWidget(pdepth_label, 13, 0, 1, 1);
    pdepth_entry = new FCDoubleSpinner(nullptr);
    pdepth_entry->setProperty("decimals", 4);
    pdepth_entry->setProperty("maximum", 10000.0);
    pdepth_entry->setProperty("minimum", -10000.0);
    pdepth_entry->setProperty("objectName", QString::fromUtf8("e_depth_probe"));
    pdepth_entry->setProperty("singleStep", 0.1);
    pdepth_entry->setProperty("value", -0.02);
    pdepth_entry->setHidden(true);
    grid3->addWidget(pdepth_entry, 13, 1, 1, 1);
    feedrate_probe_label = new QLabel(nullptr);
    feedrate_probe_label->setText(trUi_(QString::fromUtf8("Feedrate Probe:")));
    feedrate_probe_label->setToolTip(trUi_(QString::fromUtf8("The feedrate used while the probe is probing.")));
    feedrate_probe_label->setHidden(true);
    grid3->addWidget(feedrate_probe_label, 14, 0, 1, 1);
    feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    feedrate_probe_entry->setProperty("decimals", 4);
    feedrate_probe_entry->setProperty("maximum", 10000.0);
    feedrate_probe_entry->setProperty("objectName", QString::fromUtf8("e_fr_probe"));
    feedrate_probe_entry->setProperty("singleStep", 0.1);
    feedrate_probe_entry->setProperty("value", 75.0);
    feedrate_probe_entry->setHidden(true);
    grid3->addWidget(feedrate_probe_entry, 14, 1, 1, 1);
    auto* w8 = new QLabel(nullptr);
    w8->setText(trUi_(QString::fromUtf8("Preprocessor:")));
    w8->setToolTip(trUi_(QString::fromUtf8("The preprocessor JSON file that dictates\nGcode output for Excellon Objects.")));
    grid3->addWidget(w8, 15, 0, 1, 1);
    pp_excellon_name_cb = new FCComboBox(nullptr);
    pp_excellon_name_cb->setProperty("objectName", QString::fromUtf8("e_pp"));
    grid3->addWidget(pp_excellon_name_cb, 15, 1, 1, 1);
    exclusion_cb = new FCCheckBox(nullptr);
    exclusion_cb->setProperty("objectName", QString::fromUtf8("e_area_exclusion"));
    exclusion_cb->setText(trUi_(QString::fromUtf8("Add exclusion areas")));
    exclusion_cb->setToolTip(trUi_(QString::fromUtf8("Include exclusion areas.\nIn those areas the travel of the tools\nis forbidden.")));
    grid3->addWidget(exclusion_cb, 20, 0, 1, 2);
    exclusion_frame = new QFrame(nullptr);
    exclusion_frame->setHidden(true);
    exclusion_box = new QVBoxLayout();
    exclusion_box->setContentsMargins(0, 0, 0, 0);
    exclusion_box->setSpacing(6);
    exclusion_table = new FCTable(nullptr);
    exclusion_table->setProperty("columnCount", 4);
    exclusion_table->setCornerButtonEnabled(false);
    exclusion_table->setProperty("horizontalScrollBarPolicy", 1);
    exclusion_table->setMaximumSize(QSize(16777215, 24));
    exclusion_table->setMinimumSize(QSize(0, 24));
    exclusion_table->setProperty("selectionBehavior", 1);
    exclusion_table->setProperty("sizeAdjustPolicy", 2);
    exclusion_table->setProperty("verticalScrollBarPolicy", 1);
    exclusion_table->setColumnCount(4);
    exclusion_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Object")), trUi_(QString::fromUtf8("Strategy")), trUi_(QString::fromUtf8("Over Z"))});
    exclusion_table->verticalHeader()->setVisible(false);
    exclusion_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    exclusion_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    exclusion_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode(3));
    exclusion_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode(3));
    exclusion_box->addWidget(exclusion_table, 0);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    strategy_label = new FCLabel(nullptr);
    strategy_label->setText(trUi_(QString::fromUtf8("Strategy:")));
    strategy_label->setToolTip(trUi_(QString::fromUtf8("The strategy followed when encountering an exclusion area.\nCan be:\n- Over -> when encountering the area, the tool will go to a set height\n- Around -> will avoid the exclusion area by going around the area")));
    lay9->addWidget(strategy_label, 1, 0, 1, 1);
    strategy_radio = new RadioSet({{trUi_(QString::fromUtf8("Over")), QVariant(QString::fromUtf8("over"))}, {trUi_(QString::fromUtf8("Around")), QVariant(QString::fromUtf8("around"))}}, Qt::Horizontal, true, nullptr);
    strategy_radio->setProperty("objectName", QString::fromUtf8("e_area_strategy"));
    strategy_radio->setValue(QVariant(QString::fromUtf8("over")));
    lay9->addWidget(strategy_radio, 1, 1, 1, 1);
    over_z_label = new FCLabel(nullptr);
    over_z_label->setText(trUi_(QString::fromUtf8("Over Z:")));
    over_z_label->setToolTip(trUi_(QString::fromUtf8("The height Z to which the tool will rise in order to avoid\nan interdiction area.")));
    lay9->addWidget(over_z_label, 2, 0, 1, 1);
    over_z_entry = new FCDoubleSpinner(nullptr);
    over_z_entry->setProperty("decimals", 4);
    over_z_entry->setProperty("maximum", 10000.0);
    over_z_entry->setProperty("objectName", QString::fromUtf8("e_area_overz"));
    over_z_entry->setProperty("value", 1.0);
    lay9->addWidget(over_z_entry, 2, 1, 1, 1);
    add_area_button = new QPushButton(nullptr);
    add_area_button->setText(trUi_(QString::fromUtf8("Add Area:")));
    add_area_button->setToolTip(trUi_(QString::fromUtf8("Add an Exclusion Area.")));
    lay9->addWidget(add_area_button, 4, 0, 1, 1);
    area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setProperty("objectName", QString::fromUtf8("e_area_shape"));
    area_shape_radio->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    area_shape_radio->setValue(QVariant(QString::fromUtf8("polygon")));
    lay9->addWidget(area_shape_radio, 4, 1, 1, 1);
    lay9->setColumnStretch(1, 1);
    exclusion_box->addLayout(lay9, 0);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    delete_area_button = new QPushButton(nullptr);
    delete_area_button->setText(trUi_(QString::fromUtf8("Delete All")));
    delete_area_button->setToolTip(trUi_(QString::fromUtf8("Delete all exclusion areas.")));
    lay10->addWidget(delete_area_button, 0);
    delete_sel_area_button = new QPushButton(nullptr);
    delete_sel_area_button->setText(trUi_(QString::fromUtf8("Delete Selected")));
    delete_sel_area_button->setToolTip(trUi_(QString::fromUtf8("Delete all exclusion areas that are selected in the table.")));
    lay10->addWidget(delete_sel_area_button, 0);
    exclusion_box->addLayout(lay10, 0);
    exclusion_frame->setLayout(exclusion_box);
    grid3->addWidget(exclusion_frame, 22, 0, 1, 2);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    grid3->addWidget(w11, 25, 0, 1, 2);
    grid3->setColumnStretch(1, 1);
    exc_tools_box->addLayout(grid3, 0);
    exc_param_frame->setLayout(exc_tools_box);
    lay2->addWidget(exc_param_frame, 10, 0, 1, 2);
    lay2->setColumnStretch(1, 1);
    tools_box->addLayout(lay2, 0);
    grid4 = new QGridLayout();
    grid4->setContentsMargins(0, 0, 0, 0);
    grid4->setHorizontalSpacing(6);
    grid4->setVerticalSpacing(6);
    generate_cnc_button = new QPushButton(nullptr);
    generate_cnc_button->setEnabled(false);
    { QFont f = generate_cnc_button->font(); f.setBold(true); generate_cnc_button->setFont(f); }
    generate_cnc_button->setIcon(QIcon(QString::fromUtf8(":/images/cnc16.png")));
    generate_cnc_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    generate_cnc_button->setText(trUi_(QString::fromUtf8("Generate CNCJob object")));
    generate_cnc_button->setToolTip(trUi_(QString::fromUtf8("Generate the CNC Job.\nIf milling then an additional Geometry object will be created.\nAdd / Select at least one tool in the tool-table.\nClick the # header to select all, or Ctrl + LMB\nfor custom selection of tools.")));
    grid4->addWidget(generate_cnc_button, 3, 0, 1, 3);
    grid4->setColumnStretch(1, 1);
    tools_box->addLayout(grid4, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    tools_box->addWidget(reset_button, 0);
    tools_frame->setLayout(tools_box);
    layout->addWidget(tools_frame, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> DrillingToolUIBase::membersByName() const {
    return {
        {"add_area_button", add_area_button},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_radio", area_shape_radio},
        {"cutz_entry", cutz_entry},
        {"cutzlabel", cutzlabel},
        {"delete_area_button", delete_area_button},
        {"delete_sel_area_button", delete_sel_area_button},
        {"drill_overlap_entry", drill_overlap_entry},
        {"drill_overlap_label", drill_overlap_label},
        {"drill_slots_cb", drill_slots_cb},
        {"dwell_cb", dwell_cb},
        {"dwelltime_entry", dwelltime_entry},
        {"endxy_entry", endxy_entry},
        {"endz_entry", endz_entry},
        {"endz_label", endz_label},
        {"estartz_entry", estartz_entry},
        {"estartz_label", estartz_label},
        {"exc_param_frame", exc_param_frame},
        {"exc_tools_box", exc_tools_box},
        {"exclusion_box", exclusion_box},
        {"exclusion_cb", exclusion_cb},
        {"exclusion_frame", exclusion_frame},
        {"exclusion_table", exclusion_table},
        {"feedrate_probe_entry", feedrate_probe_entry},
        {"feedrate_probe_label", feedrate_probe_label},
        {"feedrate_rapid_entry", feedrate_rapid_entry},
        {"feedrate_rapid_label", feedrate_rapid_label},
        {"feedrate_z_entry", feedrate_z_entry},
        {"frzlabel", frzlabel},
        {"gen_param_label", gen_param_label},
        {"generate_cnc_button", generate_cnc_button},
        {"grid1", grid1},
        {"grid3", grid3},
        {"grid4", grid4},
        {"last_drill_cb", last_drill_cb},
        {"layout", layout},
        {"level", level},
        {"maxdepth_entry", maxdepth_entry},
        {"mpass_cb", mpass_cb},
        {"obj_combo_label", obj_combo_label},
        {"object_combo", object_combo},
        {"offset_entry", offset_entry},
        {"order_label", order_label},
        {"order_radio", order_radio},
        {"over_z_entry", over_z_entry},
        {"over_z_label", over_z_label},
        {"pdepth_entry", pdepth_entry},
        {"pdepth_label", pdepth_label},
        {"pp_excellon_name_cb", pp_excellon_name_cb},
        {"reset_button", reset_button},
        {"search_load_db_btn", search_load_db_btn},
        {"spindle_label", spindle_label},
        {"spindlespeed_entry", spindlespeed_entry},
        {"strategy_label", strategy_label},
        {"strategy_radio", strategy_radio},
        {"title_box", title_box},
        {"tool_data_label", tool_data_label},
        {"tool_offset_label", tool_offset_label},
        {"toolchange_cb", toolchange_cb},
        {"toolchangez_entry", toolchangez_entry},
        {"toolchangez_label", toolchangez_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"tools_table", tools_table},
        {"travelz_entry", travelz_entry},
        {"travelzlabel", travelzlabel},
    };
}

}  // namespace flatcam::gui
