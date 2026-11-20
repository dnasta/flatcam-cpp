// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GeometryObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void GeometryObjectUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(9, 9, 9, 9);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/geometry32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Geometry Object</b></font>")));
    title_box->addWidget(title_label, 1);
    level = new FCLabel(nullptr);
    level->setProperty("alignment", 130);
    level->setText(trUi_(QString::fromUtf8("<span style=\"color:green;\"><b>Basic</b></span>")));
    level->setToolTip(trUi_(QString::fromUtf8("BASIC is suitable for a beginner. Many parameters\nare hidden from the user in this mode.\nADVANCED mode will make available all parameters.\n\nTo change the application LEVEL, go to:\nEdit -> Preferences -> General and check:\n'APP. LEVEL' radio button.")));
    title_box->addWidget(level, 0);
    lay1->addLayout(title_box, 0);
    custom_box = new QVBoxLayout();
    custom_box->setContentsMargins(0, 0, 0, 0);
    custom_box->setSpacing(6);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    plot_options_label = new FCLabel(nullptr);
    plot_options_label->setMinimumSize(QSize(90, 0));
    plot_options_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    lay2->addWidget(plot_options_label, 0, 0, 1, 1);
    multicolored_cb = new FCCheckBox(nullptr);
    multicolored_cb->setMinimumSize(QSize(55, 0));
    multicolored_cb->setText(trUi_(QString::fromUtf8("Multi-Color")));
    multicolored_cb->setToolTip(trUi_(QString::fromUtf8("Draw polygons in different colors.")));
    lay2->addWidget(multicolored_cb, 0, 2, 1, 1);
    name_hlay = new QHBoxLayout();
    name_hlay->setContentsMargins(0, 0, 0, 0);
    name_hlay->setSpacing(6);
    auto* w3 = new FCLabel(nullptr);
    w3->setText(trUi_(QString::fromUtf8("<b>Name:</b>")));
    name_hlay->addWidget(w3, 0);
    name_entry = new FCEntry(nullptr);
    name_entry->setText(trUi_(QString::fromUtf8("x")));
    name_hlay->addWidget(name_entry, 0);
    lay2->addLayout(name_hlay, 2, 0, 1, 3);
    editor_button = new FCButton(nullptr);
    { QFont f = editor_button->font(); f.setBold(true); editor_button->setFont(f); }
    editor_button->setIcon(QIcon(QString::fromUtf8(":/images/edit_file32.png")));
    editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    editor_button->setText(trUi_(QString::fromUtf8("Geometry Editor")));
    editor_button->setToolTip(trUi_(QString::fromUtf8("Start the Object Editor")));
    lay2->addWidget(editor_button, 4, 0, 1, 3);
    properties_button = new FCButton(nullptr);
    properties_button->setCheckable(true);
    { QFont f = properties_button->font(); f.setBold(true); properties_button->setFont(f); }
    properties_button->setIcon(QIcon(QString::fromUtf8(":/images/properties32.png")));
    properties_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    properties_button->setText(trUi_(QString::fromUtf8("PROPERTIES")));
    properties_button->setToolTip(trUi_(QString::fromUtf8("Show the Properties.")));
    lay2->addWidget(properties_button, 6, 0, 1, 3);
    properties_frame = new QFrame(nullptr);
    properties_frame->setHidden(true);
    properties_box = new QVBoxLayout();
    properties_box->setContentsMargins(0, 0, 0, 0);
    properties_box->setSpacing(6);
    treeWidget = new FCTree(nullptr);
    properties_box->addWidget(treeWidget, 0);
    properties_frame->setLayout(properties_box);
    lay2->addWidget(properties_frame, 7, 0, 1, 3);
    lay2->setColumnStretch(1, 1);
    custom_box->addLayout(lay2, 0);
    geo_tools_frame = new QFrame(nullptr);
    geo_tools_box = new QVBoxLayout();
    geo_tools_box->setContentsMargins(0, 0, 0, 0);
    geo_tools_box->setSpacing(6);
    geo_table_frame = new QFrame(nullptr);
    geo_table_box = new QVBoxLayout();
    geo_table_box->setContentsMargins(0, 0, 0, 0);
    geo_table_box->setSpacing(6);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(trUi_(QString::fromUtf8("<b>Tools Table:</b>")));
    tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools in this Geometry object used for cutting.\nThe 'Offset' entry will set an offset for the cut.\n'Offset' can be inside, outside, on path (none) and custom.\n'Type' entry is only informative and it allow to know the \nintent of using the current tool. \nIt can be Rough(ing), Finish(ing) or Iso(lation).\nThe 'Tool type'(TT) can be circular with 1 to 4 teeths(C1..C4),\nball(B), or V-Shaped(V). \nWhen V-shaped is selected the 'Type' entry is automatically \nset to Isolation, the CutZ parameter in the UI form is\ngrayed out and Cut Z is automatically calculated from the newly \nshowed UI form entries named V-Tip Dia and V-Tip Angle.")));
    lay4->addWidget(tools_table_label, 0, 0, 1, 1);
    plot_cb = new FCCheckBox(nullptr);
    plot_cb->setChecked(true);
    plot_cb->setProperty("layoutDirection", 1);
    plot_cb->setText(trUi_(QString::fromUtf8("Plot Object")));
    plot_cb->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay4->addWidget(plot_cb, 0, 1, 1, 1);
    geo_tools_table = new FCTable(nullptr);
    geo_tools_table->setAcceptDrops(true);
    geo_tools_table->setProperty("columnCount", 7);
    geo_tools_table->setProperty("contextMenuPolicy", 2);
    geo_tools_table->setProperty("dragDropMode", 4);
    geo_tools_table->setDragDropOverwriteMode(false);
    geo_tools_table->setDragEnabled(true);
    geo_tools_table->setProperty("selectionBehavior", 1);
    geo_tools_table->setProperty("sizeAdjustPolicy", 2);
    geo_tools_table->setColumnCount(7);
    geo_tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Dia")), trUi_(QString::fromUtf8("Offset")), trUi_(QString::fromUtf8("Type")), trUi_(QString::fromUtf8("TT")), trUi_(QString::fromUtf8("")), trUi_(QString::fromUtf8("P"))});
    geo_tools_table->setColumnHidden(2, true);
    geo_tools_table->setColumnHidden(3, true);
    geo_tools_table->setColumnHidden(5, true);
    lay4->addWidget(geo_tools_table, 1, 0, 1, 2);
    lay4->setColumnStretch(1, 1);
    geo_table_box->addLayout(lay4, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    tool_offset_lbl = new FCLabel(nullptr);
    tool_offset_lbl->setText(trUi_(QString::fromUtf8("Tool Offset:")));
    tool_offset_lbl->setToolTip(trUi_(QString::fromUtf8("The value to offset the cut when \nthe Offset type selected is 'Offset'.\nThe value can be positive for 'outside'\ncut and negative for 'inside' cut.")));
    tool_offset_lbl->setHidden(true);
    lay5->addWidget(tool_offset_lbl, 0, 0, 1, 1);
    tool_offset_entry = new FCDoubleSpinner(nullptr);
    tool_offset_entry->setProperty("decimals", 4);
    tool_offset_entry->setProperty("maximum", 10000.0);
    tool_offset_entry->setProperty("minimum", -10000.0);
    tool_offset_entry->setProperty("singleStep", 0.1);
    tool_offset_entry->setHidden(true);
    lay5->addWidget(tool_offset_entry, 0, 1, 1, 1);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay5->addWidget(w6, 1, 0, 1, 2);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(trUi_(QString::fromUtf8("<b>Add from DB</b>")));
    lay5->addWidget(tool_sel_label, 2, 0, 1, 2);
    addtool_entry_lbl = new FCLabel(nullptr);
    addtool_entry_lbl->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    addtool_entry_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool")));
    addtool_entry_lbl->setHidden(true);
    lay5->addWidget(addtool_entry_lbl, 3, 0, 1, 1);
    addtool_entry = new FCDoubleSpinner(nullptr);
    addtool_entry->setProperty("decimals", 4);
    addtool_entry->setProperty("maximum", 10000.0);
    addtool_entry->setProperty("singleStep", 0.1);
    addtool_entry->setProperty("value", 2.4);
    addtool_entry->setHidden(true);
    lay5->addWidget(addtool_entry, 3, 1, 1, 1);
    auto* lay7 = new QHBoxLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setSpacing(6);
    search_and_add_btn = new FCButton(nullptr);
    search_and_add_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    search_and_add_btn->setText(trUi_(QString::fromUtf8("Search and Add")));
    search_and_add_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nwith the diameter specified above.")));
    search_and_add_btn->setHidden(true);
    lay7->addWidget(search_and_add_btn, 0);
    addtool_from_db_btn = new FCButton(nullptr);
    addtool_from_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    addtool_from_db_btn->setText(trUi_(QString::fromUtf8("Pick from DB")));
    addtool_from_db_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nfrom the Tools Database.\nTools database administration in in:\nMenu: Options -> Tools Database")));
    lay7->addWidget(addtool_from_db_btn, 0);
    lay5->addLayout(lay7, 5, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay5->addWidget(w8, 9, 0, 1, 2);
    lay5->setColumnStretch(1, 1);
    geo_table_box->addLayout(lay5, 0);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    deltool_btn->setText(trUi_(QString::fromUtf8("Delete")));
    deltool_btn->setToolTip(trUi_(QString::fromUtf8("Delete a selection of tools in the Tool Table\nby first selecting a row in the Tool Table.")));
    deltool_btn->setHidden(true);
    lay9->addWidget(deltool_btn, 0, 0, 1, 2);
    geo_table_box->addLayout(lay9, 0);
    geo_table_frame->setLayout(geo_table_box);
    geo_tools_box->addWidget(geo_table_frame, 0);
    geo_param_frame = new QFrame(nullptr);
    geo_param_box = new QVBoxLayout();
    geo_param_box->setContentsMargins(0, 0, 0, 0);
    geo_param_box->setSpacing(6);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    geo_param_box->addWidget(w10, 0);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    tool_data_label = new FCLabel(nullptr);
    tool_data_label->setText(trUi_(QString::fromUtf8("<b>Parameters for: <font color='#0000FF'>Tool 1</font></b>")));
    tool_data_label->setToolTip(trUi_(QString::fromUtf8("The data used for creating GCode.\nEach tool store it's own set of such data.")));
    grid3->addWidget(tool_data_label, 0, 0, 1, 2);
    tipdialabel = new FCLabel(nullptr);
    tipdialabel->setText(trUi_(QString::fromUtf8("V-Tip Dia:")));
    tipdialabel->setToolTip(trUi_(QString::fromUtf8("The tip diameter for V-Shape Tool")));
    tipdialabel->setHidden(true);
    grid3->addWidget(tipdialabel, 1, 0, 1, 1);
    tipdia_entry = new FCDoubleSpinner(nullptr);
    tipdia_entry->setProperty("decimals", 4);
    tipdia_entry->setProperty("maximum", 10000.0);
    tipdia_entry->setProperty("singleStep", 0.1);
    tipdia_entry->setProperty("value", 0.1);
    tipdia_entry->setHidden(true);
    grid3->addWidget(tipdia_entry, 1, 1, 1, 1);
    tipanglelabel = new FCLabel(nullptr);
    tipanglelabel->setText(trUi_(QString::fromUtf8("V-Tip Angle:")));
    tipanglelabel->setToolTip(trUi_(QString::fromUtf8("The tip angle for V-Shape Tool.\nIn degree.")));
    tipanglelabel->setHidden(true);
    grid3->addWidget(tipanglelabel, 2, 0, 1, 1);
    tipangle_entry = new FCDoubleSpinner(nullptr);
    tipangle_entry->setProperty("decimals", 4);
    tipangle_entry->setProperty("maximum", 180.0);
    tipangle_entry->setProperty("minimum", 1.0);
    tipangle_entry->setProperty("value", 30.0);
    tipangle_entry->setHidden(true);
    grid3->addWidget(tipangle_entry, 2, 1, 1, 1);
    cutzlabel = new FCLabel(nullptr);
    cutzlabel->setText(trUi_(QString::fromUtf8("Cut Z:")));
    cutzlabel->setToolTip(trUi_(QString::fromUtf8("Cutting depth (negative)\nbelow the copper surface.")));
    grid3->addWidget(cutzlabel, 3, 0, 1, 1);
    cutz_entry = new FCDoubleSpinner(nullptr);
    cutz_entry->setProperty("decimals", 4);
    cutz_entry->setProperty("maximum", 0.0);
    cutz_entry->setProperty("minimum", -10000.0);
    cutz_entry->setProperty("singleStep", 0.1);
    cutz_entry->setProperty("value", -2.4);
    grid3->addWidget(cutz_entry, 3, 1, 1, 1);
    mpass_cb = new FCCheckBox(nullptr);
    mpass_cb->setText(trUi_(QString::fromUtf8("Multi-Depth:")));
    mpass_cb->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    grid3->addWidget(mpass_cb, 4, 0, 1, 1);
    maxdepth_entry = new FCDoubleSpinner(nullptr);
    maxdepth_entry->setProperty("decimals", 4);
    maxdepth_entry->setEnabled(false);
    maxdepth_entry->setProperty("maximum", 10000.0);
    maxdepth_entry->setProperty("singleStep", 0.1);
    maxdepth_entry->setToolTip(trUi_(QString::fromUtf8("Depth of each pass (positive).")));
    maxdepth_entry->setProperty("value", 0.8);
    grid3->addWidget(maxdepth_entry, 4, 1, 1, 1);
    travelzlabel = new FCLabel(nullptr);
    travelzlabel->setText(trUi_(QString::fromUtf8("Travel Z:")));
    travelzlabel->setToolTip(trUi_(QString::fromUtf8("Height of the tool when\nmoving without cutting.")));
    grid3->addWidget(travelzlabel, 5, 0, 1, 1);
    travelz_entry = new FCDoubleSpinner(nullptr);
    travelz_entry->setProperty("decimals", 4);
    travelz_entry->setProperty("maximum", 10000.0);
    travelz_entry->setProperty("singleStep", 0.1);
    travelz_entry->setProperty("value", 2.0);
    grid3->addWidget(travelz_entry, 5, 1, 1, 1);
    frlabel = new FCLabel(nullptr);
    frlabel->setText(trUi_(QString::fromUtf8("Feedrate X-Y:")));
    frlabel->setToolTip(trUi_(QString::fromUtf8("Cutting speed in the XY\nplane in units per minute")));
    grid3->addWidget(frlabel, 10, 0, 1, 1);
    cncfeedrate_entry = new FCDoubleSpinner(nullptr);
    cncfeedrate_entry->setProperty("decimals", 4);
    cncfeedrate_entry->setProperty("maximum", 910000.0);
    cncfeedrate_entry->setProperty("singleStep", 0.1);
    cncfeedrate_entry->setProperty("value", 120.0);
    grid3->addWidget(cncfeedrate_entry, 10, 1, 1, 1);
    frzlabel = new FCLabel(nullptr);
    frzlabel->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    frzlabel->setToolTip(trUi_(QString::fromUtf8("Cutting speed in the XY\nplane in units per minute.\nIt is called also Plunge.")));
    grid3->addWidget(frzlabel, 11, 0, 1, 1);
    feedrate_z_entry = new FCDoubleSpinner(nullptr);
    feedrate_z_entry->setProperty("decimals", 4);
    feedrate_z_entry->setProperty("maximum", 910000.0);
    feedrate_z_entry->setProperty("singleStep", 0.1);
    feedrate_z_entry->setProperty("value", 60.0);
    grid3->addWidget(feedrate_z_entry, 11, 1, 1, 1);
    fr_rapidlabel = new FCLabel(nullptr);
    fr_rapidlabel->setText(trUi_(QString::fromUtf8("Feedrate Rapids:")));
    fr_rapidlabel->setToolTip(trUi_(QString::fromUtf8("Cutting speed in the XY plane\n(in units per minute).\nThis is for the rapid move G00.\nIt is useful only for Marlin,\nignore for any other cases.")));
    fr_rapidlabel->setHidden(true);
    grid3->addWidget(fr_rapidlabel, 12, 0, 1, 1);
    feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    feedrate_rapid_entry->setProperty("decimals", 4);
    feedrate_rapid_entry->setProperty("maximum", 910000.0);
    feedrate_rapid_entry->setProperty("singleStep", 0.1);
    feedrate_rapid_entry->setProperty("value", 1500.0);
    feedrate_rapid_entry->setHidden(true);
    grid3->addWidget(feedrate_rapid_entry, 12, 1, 1, 1);
    extracut_cb = new FCCheckBox(nullptr);
    extracut_cb->setText(trUi_(QString::fromUtf8("Re-cut:")));
    extracut_cb->setToolTip(trUi_(QString::fromUtf8("In order to remove possible\ncopper leftovers where first cut\nmeet with last cut, we generate an\nextended cut over the first cut section.")));
    extracut_cb->setHidden(true);
    grid3->addWidget(extracut_cb, 13, 0, 1, 1);
    e_cut_entry = new FCDoubleSpinner(nullptr);
    e_cut_entry->setProperty("decimals", 4);
    e_cut_entry->setEnabled(false);
    e_cut_entry->setProperty("maximum", 99999.0);
    e_cut_entry->setProperty("singleStep", 0.1);
    e_cut_entry->setToolTip(trUi_(QString::fromUtf8("In order to remove possible\ncopper leftovers where first cut\nmeet with last cut, we generate an\nextended cut over the first cut section.")));
    e_cut_entry->setProperty("value", 0.1);
    e_cut_entry->setWrapping(true);
    e_cut_entry->setHidden(true);
    grid3->addWidget(e_cut_entry, 13, 1, 1, 1);
    spindle_label = new FCLabel(nullptr);
    spindle_label->setText(trUi_(QString::fromUtf8("Spindle speed:")));
    spindle_label->setToolTip(trUi_(QString::fromUtf8("Speed of the spindle in RPM (optional).\nIf LASER preprocessor is used,\nthis value is the power of laser.")));
    grid3->addWidget(spindle_label, 14, 0, 1, 1);
    cncspindlespeed_entry = new FCSpinner(nullptr);
    cncspindlespeed_entry->setProperty("maximum", 1000000);
    cncspindlespeed_entry->setProperty("singleStep", 100);
    grid3->addWidget(cncspindlespeed_entry, 14, 1, 1, 1);
    dwell_cb = new FCCheckBox(nullptr);
    dwell_cb->setText(trUi_(QString::fromUtf8("Dwell:")));
    dwell_cb->setToolTip(trUi_(QString::fromUtf8("Pause to allow the spindle to reach its\nspeed before cutting.")));
    grid3->addWidget(dwell_cb, 15, 0, 1, 1);
    dwelltime_entry = new FCDoubleSpinner(nullptr);
    dwelltime_entry->setProperty("decimals", 4);
    dwelltime_entry->setEnabled(false);
    dwelltime_entry->setProperty("maximum", 10000.0);
    dwelltime_entry->setProperty("singleStep", 0.1);
    dwelltime_entry->setToolTip(trUi_(QString::fromUtf8("Number of time units for spindle to dwell.")));
    dwelltime_entry->setProperty("value", 1.0);
    grid3->addWidget(dwelltime_entry, 15, 1, 1, 1);
    pdepth_label = new FCLabel(nullptr);
    pdepth_label->setText(trUi_(QString::fromUtf8("Probe Z depth:")));
    pdepth_label->setToolTip(trUi_(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units.")));
    pdepth_label->setHidden(true);
    grid3->addWidget(pdepth_label, 17, 0, 1, 1);
    pdepth_entry = new FCDoubleSpinner(nullptr);
    pdepth_entry->setProperty("decimals", 4);
    pdepth_entry->setProperty("maximum", 10000.0);
    pdepth_entry->setProperty("minimum", -10000.0);
    pdepth_entry->setProperty("singleStep", 0.1);
    pdepth_entry->setProperty("value", -0.02);
    pdepth_entry->setHidden(true);
    grid3->addWidget(pdepth_entry, 17, 1, 1, 1);
    feedrate_probe_label = new FCLabel(nullptr);
    feedrate_probe_label->setText(trUi_(QString::fromUtf8("Feedrate Probe:")));
    feedrate_probe_label->setToolTip(trUi_(QString::fromUtf8("The feedrate used while the probe is probing.")));
    feedrate_probe_label->setHidden(true);
    grid3->addWidget(feedrate_probe_label, 18, 0, 1, 1);
    feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    feedrate_probe_entry->setProperty("decimals", 4);
    feedrate_probe_entry->setProperty("maximum", 10000.0);
    feedrate_probe_entry->setProperty("singleStep", 0.1);
    feedrate_probe_entry->setProperty("value", 75.0);
    feedrate_probe_entry->setHidden(true);
    grid3->addWidget(feedrate_probe_entry, 18, 1, 1, 1);
    grid3->setColumnStretch(1, 1);
    geo_param_box->addLayout(grid3, 0);
    grid4 = new QGridLayout();
    grid4->setContentsMargins(0, 0, 0, 0);
    grid4->setHorizontalSpacing(6);
    grid4->setVerticalSpacing(6);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    grid4->addWidget(w11, 0, 0, 1, 2);
    apply_param_to_all = new FCButton(nullptr);
    apply_param_to_all->setIcon(QIcon(QString::fromUtf8(":/images/param_all32.png")));
    apply_param_to_all->setText(trUi_(QString::fromUtf8("Apply parameters to all tools")));
    apply_param_to_all->setToolTip(trUi_(QString::fromUtf8("The parameters in the current form will be applied\non all the tools from the Tool Table.")));
    grid4->addWidget(apply_param_to_all, 1, 0, 1, 2);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    grid4->addWidget(w12, 2, 0, 1, 2);
    gen_param_label = new FCLabel(nullptr);
    gen_param_label->setText(trUi_(QString::fromUtf8("<b>Common Parameters</b>")));
    gen_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters that are common for all tools.")));
    grid4->addWidget(gen_param_label, 3, 0, 1, 2);
    toolchangeg_cb = new FCCheckBox(nullptr);
    toolchangeg_cb->setText(trUi_(QString::fromUtf8("Tool change Z:")));
    toolchangeg_cb->setToolTip(trUi_(QString::fromUtf8("Include tool-change sequence\nin the Machine Code (Pause for tool change).")));
    grid4->addWidget(toolchangeg_cb, 6, 0, 1, 1);
    toolchangez_entry = new FCDoubleSpinner(nullptr);
    toolchangez_entry->setProperty("decimals", 4);
    toolchangez_entry->setEnabled(false);
    toolchangez_entry->setProperty("maximum", 10000.0);
    toolchangez_entry->setProperty("singleStep", 0.1);
    toolchangez_entry->setToolTip(trUi_(QString::fromUtf8("Z-axis position (height) for\ntool change.")));
    toolchangez_entry->setProperty("value", 15.0);
    grid4->addWidget(toolchangez_entry, 6, 1, 1, 1);
    endz_label = new FCLabel(nullptr);
    endz_label->setText(trUi_(QString::fromUtf8("End move Z:")));
    endz_label->setToolTip(trUi_(QString::fromUtf8("Height of the tool after\nthe last move at the end of the job.")));
    grid4->addWidget(endz_label, 9, 0, 1, 1);
    endz_entry = new FCDoubleSpinner(nullptr);
    endz_entry->setProperty("decimals", 4);
    endz_entry->setProperty("maximum", 10000.0);
    endz_entry->setProperty("singleStep", 0.1);
    endz_entry->setProperty("value", 15.0);
    grid4->addWidget(endz_entry, 9, 1, 1, 1);
    auto* w13 = new FCLabel(nullptr);
    w13->setText(trUi_(QString::fromUtf8("End move X,Y:")));
    w13->setToolTip(trUi_(QString::fromUtf8("End move X,Y position. In format (x,y).\nIf no value is entered then there is no move\non X,Y plane at the end of the job.")));
    grid4->addWidget(w13, 10, 0, 1, 1);
    endxy_entry = new NumericalEvalTupleEntry(nullptr);
    endxy_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("X,Y coordinates")));
    endxy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    endxy_entry->setText(trUi_(QString::fromUtf8("None")));
    grid4->addWidget(endxy_entry, 10, 1, 1, 1);
    auto* w14 = new FCLabel(nullptr);
    w14->setText(trUi_(QString::fromUtf8("Preprocessor:")));
    w14->setToolTip(trUi_(QString::fromUtf8("The Preprocessor file that dictates\nthe Machine Code (like GCode, RML, HPGL) output.")));
    grid4->addWidget(w14, 11, 0, 1, 1);
    pp_geometry_name_cb = new FCComboBox(nullptr);
    grid4->addWidget(pp_geometry_name_cb, 11, 1, 1, 1);
    exclusion_cb = new FCCheckBox(nullptr);
    exclusion_cb->setText(trUi_(QString::fromUtf8("Add exclusion areas")));
    exclusion_cb->setToolTip(trUi_(QString::fromUtf8("Include exclusion areas.\nIn those areas the travel of the tools\nis forbidden.")));
    grid4->addWidget(exclusion_cb, 12, 0, 1, 2);
    exclusion_frame = new QFrame(nullptr);
    exclusion_frame->setHidden(true);
    exclusion_box = new QVBoxLayout();
    exclusion_box->setContentsMargins(0, 0, 0, 0);
    exclusion_box->setSpacing(6);
    exclusion_table = new FCTable(nullptr);
    exclusion_table->setProperty("columnCount", 4);
    exclusion_table->setProperty("selectionBehavior", 1);
    exclusion_table->setProperty("sizeAdjustPolicy", 2);
    exclusion_table->setColumnCount(4);
    exclusion_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Object")), trUi_(QString::fromUtf8("Strategy")), trUi_(QString::fromUtf8("Over Z"))});
    exclusion_box->addWidget(exclusion_table, 0);
    auto* lay15 = new QGridLayout();
    lay15->setContentsMargins(0, 0, 0, 0);
    lay15->setHorizontalSpacing(6);
    lay15->setVerticalSpacing(6);
    strategy_label = new FCLabel(nullptr);
    strategy_label->setText(trUi_(QString::fromUtf8("Strategy:")));
    strategy_label->setToolTip(trUi_(QString::fromUtf8("The strategy followed when encountering an exclusion area.\nCan be:\n- Over -> when encountering the area, the tool will go to a set height\n- Around -> will avoid the exclusion area by going around the area")));
    lay15->addWidget(strategy_label, 1, 0, 1, 1);
    strategy_radio = new RadioSet({{trUi_(QString::fromUtf8("Over")), QVariant(QString::fromUtf8("over"))}, {trUi_(QString::fromUtf8("Around")), QVariant(QString::fromUtf8("around"))}}, Qt::Horizontal, true, nullptr);
    strategy_radio->setValue(QVariant(QString::fromUtf8("over")));
    lay15->addWidget(strategy_radio, 1, 1, 1, 1);
    over_z_label = new FCLabel(nullptr);
    over_z_label->setText(trUi_(QString::fromUtf8("Over Z:")));
    over_z_label->setToolTip(trUi_(QString::fromUtf8("The height Z to which the tool will rise in order to avoid\nan interdiction area.")));
    lay15->addWidget(over_z_label, 2, 0, 1, 1);
    over_z_entry = new FCDoubleSpinner(nullptr);
    over_z_entry->setProperty("decimals", 4);
    over_z_entry->setProperty("maximum", 10000.0);
    over_z_entry->setProperty("value", 1.0);
    lay15->addWidget(over_z_entry, 2, 1, 1, 1);
    add_area_button = new FCButton(nullptr);
    add_area_button->setText(trUi_(QString::fromUtf8("Add Area:")));
    add_area_button->setToolTip(trUi_(QString::fromUtf8("Add an Exclusion Area.")));
    lay15->addWidget(add_area_button, 4, 0, 1, 1);
    area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    area_shape_radio->setValue(QVariant(QString::fromUtf8("polygon")));
    lay15->addWidget(area_shape_radio, 4, 1, 1, 1);
    lay15->setColumnStretch(1, 1);
    exclusion_box->addLayout(lay15, 0);
    auto* lay16 = new QHBoxLayout();
    lay16->setContentsMargins(0, 0, 0, 0);
    lay16->setSpacing(6);
    delete_area_button = new FCButton(nullptr);
    delete_area_button->setText(trUi_(QString::fromUtf8("Delete All")));
    delete_area_button->setToolTip(trUi_(QString::fromUtf8("Delete all exclusion areas.")));
    lay16->addWidget(delete_area_button, 0);
    delete_sel_area_button = new FCButton(nullptr);
    delete_sel_area_button->setText(trUi_(QString::fromUtf8("Delete Selected")));
    delete_sel_area_button->setToolTip(trUi_(QString::fromUtf8("Delete all exclusion areas that are selected in the table.")));
    lay16->addWidget(delete_sel_area_button, 0);
    exclusion_box->addLayout(lay16, 0);
    exclusion_frame->setLayout(exclusion_box);
    grid4->addWidget(exclusion_frame, 14, 0, 1, 2);
    polish_cb = new FCCheckBox(nullptr);
    polish_cb->setProperty("objectName", QString::fromUtf8("g_polish"));
    polish_cb->setText(trUi_(QString::fromUtf8("Add Polish")));
    polish_cb->setToolTip(trUi_(QString::fromUtf8("Will add a Paint section at the end of the GCode.\nA metallic brush will clean the material after milling.")));
    grid4->addWidget(polish_cb, 15, 0, 1, 2);
    polish_dia_lbl = new FCLabel(nullptr);
    polish_dia_lbl->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    polish_dia_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the polishing tool.")));
    polish_dia_lbl->setHidden(true);
    grid4->addWidget(polish_dia_lbl, 16, 0, 1, 1);
    polish_dia_entry = new FCDoubleSpinner(nullptr);
    polish_dia_entry->setProperty("decimals", 4);
    polish_dia_entry->setProperty("maximum", 10000.0);
    polish_dia_entry->setProperty("objectName", QString::fromUtf8("g_polish_dia"));
    polish_dia_entry->setProperty("value", 10.0);
    polish_dia_entry->setHidden(true);
    grid4->addWidget(polish_dia_entry, 16, 1, 1, 1);
    polish_travelz_lbl = new FCLabel(nullptr);
    polish_travelz_lbl->setText(trUi_(QString::fromUtf8("Travel Z:")));
    polish_travelz_lbl->setToolTip(trUi_(QString::fromUtf8("Height of the tool when\nmoving without cutting.")));
    polish_travelz_lbl->setHidden(true);
    grid4->addWidget(polish_travelz_lbl, 17, 0, 1, 1);
    polish_travelz_entry = new FCDoubleSpinner(nullptr);
    polish_travelz_entry->setProperty("decimals", 4);
    polish_travelz_entry->setProperty("maximum", 10000.0);
    polish_travelz_entry->setProperty("objectName", QString::fromUtf8("g_polish_travelz"));
    polish_travelz_entry->setProperty("singleStep", 0.1);
    polish_travelz_entry->setProperty("value", 2.0);
    polish_travelz_entry->setHidden(true);
    grid4->addWidget(polish_travelz_entry, 17, 1, 1, 1);
    polish_pressure_lbl = new FCLabel(nullptr);
    polish_pressure_lbl->setText(trUi_(QString::fromUtf8("Pressure:")));
    polish_pressure_lbl->setToolTip(trUi_(QString::fromUtf8("Negative value. The higher the absolute value\nthe stronger the pressure of the brush on the material.")));
    polish_pressure_lbl->setHidden(true);
    grid4->addWidget(polish_pressure_lbl, 18, 0, 1, 1);
    polish_pressure_entry = new FCDoubleSpinner(nullptr);
    polish_pressure_entry->setProperty("decimals", 4);
    polish_pressure_entry->setProperty("maximum", 10000.0);
    polish_pressure_entry->setProperty("minimum", -10000.0);
    polish_pressure_entry->setProperty("objectName", QString::fromUtf8("g_polish_pressure"));
    polish_pressure_entry->setProperty("value", -1.0);
    polish_pressure_entry->setHidden(true);
    grid4->addWidget(polish_pressure_entry, 18, 1, 1, 1);
    polish_margin_lbl = new FCLabel(nullptr);
    polish_margin_lbl->setText(trUi_(QString::fromUtf8("Margin:")));
    polish_margin_lbl->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    polish_margin_lbl->setHidden(true);
    grid4->addWidget(polish_margin_lbl, 20, 0, 1, 1);
    polish_margin_entry = new FCDoubleSpinner(nullptr);
    polish_margin_entry->setProperty("decimals", 4);
    polish_margin_entry->setProperty("maximum", 10000.0);
    polish_margin_entry->setProperty("minimum", -10000.0);
    polish_margin_entry->setProperty("objectName", QString::fromUtf8("g_polish_margin"));
    polish_margin_entry->setHidden(true);
    grid4->addWidget(polish_margin_entry, 20, 1, 1, 1);
    polish_over_lbl = new FCLabel(nullptr);
    polish_over_lbl->setText(trUi_(QString::fromUtf8("Overlap:")));
    polish_over_lbl->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.")));
    polish_over_lbl->setHidden(true);
    grid4->addWidget(polish_over_lbl, 22, 0, 1, 1);
    polish_over_entry = new FCDoubleSpinner(nullptr);
    polish_over_entry->setProperty("decimals", 4);
    polish_over_entry->setProperty("maximum", 99.9999);
    polish_over_entry->setProperty("objectName", QString::fromUtf8("g_polish_overlap"));
    polish_over_entry->setProperty("singleStep", 0.1);
    polish_over_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    polish_over_entry->setProperty("value", 5.0);
    polish_over_entry->setWrapping(true);
    polish_over_entry->setHidden(true);
    grid4->addWidget(polish_over_entry, 22, 1, 1, 1);
    polish_method_lbl = new FCLabel(nullptr);
    polish_method_lbl->setText(trUi_(QString::fromUtf8("Method:")));
    polish_method_lbl->setToolTip(trUi_(QString::fromUtf8("Algorithm for polishing:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.")));
    polish_method_lbl->setHidden(true);
    grid4->addWidget(polish_method_lbl, 24, 0, 1, 1);
    polish_method_combo = new FCComboBox2(nullptr);
    polish_method_combo->setProperty("currentText", trUi_(QString::fromUtf8("Standard")));
    polish_method_combo->setProperty("objectName", QString::fromUtf8("g_polish_method"));
    polish_method_combo->setHidden(true);
    polish_method_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    polish_method_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    polish_method_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    grid4->addWidget(polish_method_combo, 24, 1, 1, 1);
    auto* w17 = new QFrame(nullptr);
    w17->setProperty("frameShadow", 48);
    w17->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w17->setSizePolicy(sp); }
    grid4->addWidget(w17, 26, 0, 1, 2);
    generate_cnc_button = new FCButton(nullptr);
    { QFont f = generate_cnc_button->font(); f.setBold(true); generate_cnc_button->setFont(f); }
    generate_cnc_button->setIcon(QIcon(QString::fromUtf8(":/images/cnc16.png")));
    generate_cnc_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    generate_cnc_button->setText(trUi_(QString::fromUtf8("Generate CNCJob object")));
    generate_cnc_button->setToolTip(trUi_(QString::fromUtf8("Generate CNCJob object.\nAdd / Select at least one tool in the tool-table.\nClick the # header to select all, or Ctrl + LMB\nfor custom selection of tools.")));
    grid4->addWidget(generate_cnc_button, 28, 0, 1, 2);
    auto* w18 = new FCLabel(nullptr);
    grid4->addWidget(w18, 30, 0, 1, 2);
    tools_label = new FCLabel(nullptr);
    tools_label->setText(trUi_(QString::fromUtf8("<b>TOOLS</b>")));
    tools_label->setToolTip(trUi_(QString::fromUtf8("Launch Paint Tool in Tools Tab.")));
    grid4->addWidget(tools_label, 32, 0, 1, 2);
    milling_button = new FCButton(nullptr);
    milling_button->setEnabled(false);
    { QFont f = milling_button->font(); f.setBold(true); milling_button->setFont(f); }
    milling_button->setIcon(QIcon(QString::fromUtf8(":/images/milling_tool32.png")));
    milling_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    milling_button->setText(trUi_(QString::fromUtf8("Milling Tool")));
    milling_button->setToolTip(trUi_(QString::fromUtf8("Generate a CNCJob by milling a Geometry.")));
    grid4->addWidget(milling_button, 34, 0, 1, 2);
    paint_tool_button = new FCButton(nullptr);
    paint_tool_button->setIcon(QIcon(QString::fromUtf8(":/images/paint20_1.png")));
    paint_tool_button->setText(trUi_(QString::fromUtf8("Paint Tool")));
    paint_tool_button->setToolTip(trUi_(QString::fromUtf8("Creates tool paths to cover the\nwhole area of a polygon.")));
    grid4->addWidget(paint_tool_button, 36, 0, 1, 2);
    generate_ncc_button = new FCButton(nullptr);
    generate_ncc_button->setIcon(QIcon(QString::fromUtf8(":/images/eraser26.png")));
    generate_ncc_button->setText(trUi_(QString::fromUtf8("NCC Tool")));
    generate_ncc_button->setToolTip(trUi_(QString::fromUtf8("Create the Geometry Object\nfor non-copper routing.")));
    grid4->addWidget(generate_ncc_button, 38, 0, 1, 2);
    grid4->setColumnStretch(1, 1);
    geo_param_box->addLayout(grid4, 0);
    geo_param_frame->setLayout(geo_param_box);
    geo_tools_box->addWidget(geo_param_frame, 0);
    geo_tools_frame->setLayout(geo_tools_box);
    custom_box->addWidget(geo_tools_frame, 0);
    lay1->addLayout(custom_box, 0);
    common_grid = new QGridLayout();
    common_grid->setContentsMargins(0, 0, 0, 0);
    common_grid->setHorizontalSpacing(6);
    common_grid->setVerticalSpacing(6);
    auto* w19 = new QFrame(nullptr);
    w19->setProperty("frameShadow", 48);
    w19->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w19->setSizePolicy(sp); }
    common_grid->addWidget(w19, 1, 0, 1, 2);
    transform_label = new FCLabel(nullptr);
    transform_label->setText(trUi_(QString::fromUtf8("<b>Transformations</b>")));
    transform_label->setToolTip(trUi_(QString::fromUtf8("Geometrical transformations of the current object.")));
    common_grid->addWidget(transform_label, 2, 0, 1, 2);
    scale_entry = new NumericalEvalEntry(nullptr);
    scale_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    scale_entry->setText(trUi_(QString::fromUtf8("1.0000")));
    scale_entry->setToolTip(trUi_(QString::fromUtf8("Factor by which to multiply\ngeometric features of this object.\nExpressions are allowed. E.g: 1/25.4")));
    common_grid->addWidget(scale_entry, 3, 0, 1, 1);
    scale_button = new FCButton(nullptr);
    scale_button->setMinimumSize(QSize(70, 0));
    scale_button->setText(trUi_(QString::fromUtf8("Scale")));
    scale_button->setToolTip(trUi_(QString::fromUtf8("Perform scaling operation.")));
    common_grid->addWidget(scale_button, 3, 1, 1, 1);
    offsetvector_entry = new NumericalEvalTupleEntry(nullptr);
    offsetvector_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    offsetvector_entry->setText(trUi_(QString::fromUtf8("(0.0, 0.0)")));
    offsetvector_entry->setToolTip(trUi_(QString::fromUtf8("Amount by which to move the object\nin the x and y axes in (x, y) format.\nExpressions are allowed. E.g: (1/3.2, 0.5*3)")));
    common_grid->addWidget(offsetvector_entry, 4, 0, 1, 1);
    offset_button = new FCButton(nullptr);
    offset_button->setMinimumSize(QSize(70, 0));
    offset_button->setText(trUi_(QString::fromUtf8("Offset")));
    offset_button->setToolTip(trUi_(QString::fromUtf8("Perform the offset operation.")));
    common_grid->addWidget(offset_button, 4, 1, 1, 1);
    transformations_button = new FCButton(nullptr);
    transformations_button->setIcon(QIcon(QString::fromUtf8(":/images/transform.png")));
    transformations_button->setText(trUi_(QString::fromUtf8("Transformations")));
    transformations_button->setToolTip(trUi_(QString::fromUtf8("Geometrical transformations of the current object.")));
    common_grid->addWidget(transformations_button, 5, 0, 1, 2);
    common_grid->setColumnStretch(0, 1);
    lay1->addLayout(common_grid, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(lay1);
}

std::map<std::string, QObject*> GeometryObjectUIBase::membersByName() const {
    return {
        {"add_area_button", add_area_button},
        {"addtool_entry", addtool_entry},
        {"addtool_entry_lbl", addtool_entry_lbl},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_radio", area_shape_radio},
        {"cncfeedrate_entry", cncfeedrate_entry},
        {"cncspindlespeed_entry", cncspindlespeed_entry},
        {"common_grid", common_grid},
        {"custom_box", custom_box},
        {"cutz_entry", cutz_entry},
        {"cutzlabel", cutzlabel},
        {"delete_area_button", delete_area_button},
        {"delete_sel_area_button", delete_sel_area_button},
        {"deltool_btn", deltool_btn},
        {"dwell_cb", dwell_cb},
        {"dwelltime_entry", dwelltime_entry},
        {"e_cut_entry", e_cut_entry},
        {"editor_button", editor_button},
        {"endxy_entry", endxy_entry},
        {"endz_entry", endz_entry},
        {"endz_label", endz_label},
        {"exclusion_box", exclusion_box},
        {"exclusion_cb", exclusion_cb},
        {"exclusion_frame", exclusion_frame},
        {"exclusion_table", exclusion_table},
        {"extracut_cb", extracut_cb},
        {"feedrate_probe_entry", feedrate_probe_entry},
        {"feedrate_probe_label", feedrate_probe_label},
        {"feedrate_rapid_entry", feedrate_rapid_entry},
        {"feedrate_z_entry", feedrate_z_entry},
        {"fr_rapidlabel", fr_rapidlabel},
        {"frlabel", frlabel},
        {"frzlabel", frzlabel},
        {"gen_param_label", gen_param_label},
        {"generate_cnc_button", generate_cnc_button},
        {"generate_ncc_button", generate_ncc_button},
        {"geo_param_box", geo_param_box},
        {"geo_param_frame", geo_param_frame},
        {"geo_table_box", geo_table_box},
        {"geo_table_frame", geo_table_frame},
        {"geo_tools_box", geo_tools_box},
        {"geo_tools_frame", geo_tools_frame},
        {"geo_tools_table", geo_tools_table},
        {"grid3", grid3},
        {"grid4", grid4},
        {"icon", icon},
        {"level", level},
        {"maxdepth_entry", maxdepth_entry},
        {"milling_button", milling_button},
        {"mpass_cb", mpass_cb},
        {"multicolored_cb", multicolored_cb},
        {"name_entry", name_entry},
        {"name_hlay", name_hlay},
        {"offset_button", offset_button},
        {"offsetvector_entry", offsetvector_entry},
        {"over_z_entry", over_z_entry},
        {"over_z_label", over_z_label},
        {"paint_tool_button", paint_tool_button},
        {"pdepth_entry", pdepth_entry},
        {"pdepth_label", pdepth_label},
        {"plot_cb", plot_cb},
        {"plot_options_label", plot_options_label},
        {"polish_cb", polish_cb},
        {"polish_dia_entry", polish_dia_entry},
        {"polish_dia_lbl", polish_dia_lbl},
        {"polish_margin_entry", polish_margin_entry},
        {"polish_margin_lbl", polish_margin_lbl},
        {"polish_method_combo", polish_method_combo},
        {"polish_method_lbl", polish_method_lbl},
        {"polish_over_entry", polish_over_entry},
        {"polish_over_lbl", polish_over_lbl},
        {"polish_pressure_entry", polish_pressure_entry},
        {"polish_pressure_lbl", polish_pressure_lbl},
        {"polish_travelz_entry", polish_travelz_entry},
        {"polish_travelz_lbl", polish_travelz_lbl},
        {"pp_geometry_name_cb", pp_geometry_name_cb},
        {"properties_box", properties_box},
        {"properties_button", properties_button},
        {"properties_frame", properties_frame},
        {"scale_button", scale_button},
        {"scale_entry", scale_entry},
        {"search_and_add_btn", search_and_add_btn},
        {"spindle_label", spindle_label},
        {"strategy_label", strategy_label},
        {"strategy_radio", strategy_radio},
        {"tipangle_entry", tipangle_entry},
        {"tipanglelabel", tipanglelabel},
        {"tipdia_entry", tipdia_entry},
        {"tipdialabel", tipdialabel},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tool_data_label", tool_data_label},
        {"tool_offset_entry", tool_offset_entry},
        {"tool_offset_lbl", tool_offset_lbl},
        {"tool_sel_label", tool_sel_label},
        {"toolchangeg_cb", toolchangeg_cb},
        {"toolchangez_entry", toolchangez_entry},
        {"tools_label", tools_label},
        {"tools_table_label", tools_table_label},
        {"transform_label", transform_label},
        {"transformations_button", transformations_button},
        {"travelz_entry", travelz_entry},
        {"travelzlabel", travelzlabel},
        {"treeWidget", treeWidget},
    };
}

}  // namespace flatcam::gui
