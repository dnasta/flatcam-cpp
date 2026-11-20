// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/CNCObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void CNCObjectUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(9, 9, 9, 9);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/cnc32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>CNC Job Object</b></font>")));
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
    cncplot_method_label = new FCLabel(nullptr);
    cncplot_method_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    cncplot_method_label->setToolTip(trUi_(QString::fromUtf8("This selects the kind of geometries on the canvas to plot.\nThose can be either of type 'Travel' which means the moves\nabove the work piece or it can be of type 'Cut',\nwhich means the moves that cut into the material.")));
    lay2->addWidget(cncplot_method_label, 0, 0, 1, 1);
    cncplot_method_combo = new RadioSet({{trUi_(QString::fromUtf8("All")), QVariant(QString::fromUtf8("all"))}, {trUi_(QString::fromUtf8("Travel")), QVariant(QString::fromUtf8("travel"))}, {trUi_(QString::fromUtf8("Cut")), QVariant(QString::fromUtf8("cut"))}}, Qt::Horizontal, false, nullptr);
    cncplot_method_combo->setValue(QVariant(QString::fromUtf8("all")));
    lay2->addWidget(cncplot_method_combo, 0, 1, 1, 2);
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
    editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t\t\t   "));
    editor_button->setText(trUi_(QString::fromUtf8("GCode Editor")));
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
    annotation_cb = new FCCheckBox(nullptr);
    annotation_cb->setChecked(true);
    annotation_cb->setText(trUi_(QString::fromUtf8("Display Annotation")));
    annotation_cb->setToolTip(trUi_(QString::fromUtf8("This selects if to display text annotation on the plot.\nWhen checked it will display numbers in order for each end\nof a travel line.")));
    lay2->addWidget(annotation_cb, 8, 0, 1, 3);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay2->addWidget(w4, 10, 0, 1, 3);
    t_distance_label = new FCLabel(nullptr);
    t_distance_label->setText(trUi_(QString::fromUtf8("<b>Travelled distance:</b>")));
    t_distance_label->setToolTip(trUi_(QString::fromUtf8("This is the total travelled distance on X-Y plane.\nIn current units.")));
    t_distance_label->setHidden(true);
    lay2->addWidget(t_distance_label, 12, 0, 1, 1);
    t_distance_entry = new FCEntry(nullptr);
    t_distance_entry->setHidden(true);
    lay2->addWidget(t_distance_entry, 12, 1, 1, 1);
    units_label = new FCLabel(nullptr);
    lay2->addWidget(units_label, 12, 2, 1, 1);
    t_time_label = new FCLabel(nullptr);
    t_time_label->setText(trUi_(QString::fromUtf8("<b>Estimated time:</b>")));
    t_time_label->setToolTip(trUi_(QString::fromUtf8("This is the estimated time to do the routing/drilling,\nwithout the time spent in ToolChange events.")));
    t_time_label->setHidden(true);
    lay2->addWidget(t_time_label, 14, 0, 1, 1);
    t_time_entry = new FCEntry(nullptr);
    t_time_entry->setHidden(true);
    lay2->addWidget(t_time_entry, 14, 1, 1, 1);
    units_time_label = new FCLabel(nullptr);
    lay2->addWidget(units_time_label, 14, 2, 1, 1);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay2->addWidget(w5, 16, 0, 1, 3);
    lay2->setColumnStretch(1, 1);
    custom_box->addLayout(lay2, 0);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    cnc_tools_table_label = new FCLabel(nullptr);
    cnc_tools_table_label->setText(trUi_(QString::fromUtf8("<b>CNC Tools Table</b>")));
    cnc_tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools in this CNCJob object used for cutting.\nThe tool diameter is used for plotting on canvas.\nThe 'Offset' entry will set an offset for the cut.\n'Offset' can be inside, outside, on path (none) and custom.\n'Type' entry is only informative and it allow to know the \nintent of using the current tool. \nIt can be Rough(ing), Finish(ing) or Iso(lation).\nThe 'Tool type'(TT) can be circular with 1 to 4 teeths(C1..C4),\nball(B), or V-Shaped(V).")));
    lay6->addWidget(cnc_tools_table_label, 0);
    lay6->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    plot_cb = new FCCheckBox(nullptr);
    plot_cb->setChecked(true);
    plot_cb->setProperty("layoutDirection", 1);
    plot_cb->setText(trUi_(QString::fromUtf8("Plot Object")));
    plot_cb->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay6->addWidget(plot_cb, 0);
    custom_box->addLayout(lay6, 0);
    cnc_tools_table = new FCTable(nullptr);
    cnc_tools_table->setProperty("columnCount", 7);
    cnc_tools_table->setColumnCount(7);
    cnc_tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Dia")), trUi_(QString::fromUtf8("Offset")), trUi_(QString::fromUtf8("Type")), trUi_(QString::fromUtf8("TT")), trUi_(QString::fromUtf8("")), trUi_(QString::fromUtf8("P"))});
    cnc_tools_table->setColumnHidden(5, true);
    custom_box->addWidget(cnc_tools_table, 0);
    exc_cnc_tools_table = new FCTable(nullptr);
    exc_cnc_tools_table->setProperty("columnCount", 7);
    exc_cnc_tools_table->setColumnCount(7);
    exc_cnc_tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Dia")), trUi_(QString::fromUtf8("Drills")), trUi_(QString::fromUtf8("Slots")), trUi_(QString::fromUtf8("")), trUi_(QString::fromUtf8("Cut Z")), trUi_(QString::fromUtf8("P"))});
    exc_cnc_tools_table->setColumnHidden(4, true);
    custom_box->addWidget(exc_cnc_tools_table, 0);
    tooldia_entry = new FCDoubleSpinner(nullptr);
    tooldia_entry->setProperty("decimals", 4);
    tooldia_entry->setProperty("maximum", 10000.0);
    tooldia_entry->setProperty("singleStep", 0.1);
    tooldia_entry->setProperty("value", 1.0);
    custom_box->addWidget(tooldia_entry, 0);
    updateplot_button = new FCButton(nullptr);
    updateplot_button->setText(trUi_(QString::fromUtf8("Update Plot")));
    updateplot_button->setToolTip(trUi_(QString::fromUtf8("Update the plot.")));
    custom_box->addWidget(updateplot_button, 0);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    custom_box->addWidget(w7, 0);
    snippets_cb = new FCCheckBox(nullptr);
    snippets_cb->setText(trUi_(QString::fromUtf8("Use CNC Code Snippets")));
    snippets_cb->setToolTip(trUi_(QString::fromUtf8("When selected, it will include CNC Code snippets (append and prepend)\ndefined in the Preferences.")));
    custom_box->addWidget(snippets_cb, 0);
    sal_btn = new FCButton(nullptr);
    sal_btn->setCheckable(true);
    { QFont f = sal_btn->font(); f.setBold(true); sal_btn->setFont(f); }
    sal_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t  "));
    sal_btn->setText(trUi_(QString::fromUtf8("Autolevelling")));
    sal_btn->setToolTip(trUi_(QString::fromUtf8("Enable the autolevelling feature.")));
    sal_btn->setHidden(true);
    custom_box->addWidget(sal_btn, 0);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    custom_box->addWidget(w8, 0);
    al_frame = new QFrame(nullptr);
    al_frame->setHidden(true);
    al_box = new QVBoxLayout();
    al_box->setContentsMargins(0, 0, 0, 0);
    al_box->setSpacing(6);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    auto* w11 = new FCLabel(nullptr);
    w11->setText(trUi_(QString::fromUtf8("<b>Probe Points Table</b>")));
    w11->setToolTip(trUi_(QString::fromUtf8("Generate GCode that will obtain the height map")));
    lay10->addWidget(w11, 0);
    lay10->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    show_al_table = new FCCheckBox(nullptr);
    show_al_table->setChecked(true);
    show_al_table->setText(trUi_(QString::fromUtf8("Show")));
    show_al_table->setToolTip(trUi_(QString::fromUtf8("Toggle the display of the Probe Points table.")));
    lay10->addWidget(show_al_table, 0, Qt::Alignment(2));
    lay9->addLayout(lay10, 0, 0, 1, 2);
    al_probe_points_table = new FCTable(nullptr);
    al_probe_points_table->setProperty("columnCount", 3);
    al_probe_points_table->setMaximumSize(QSize(16777215, 24));
    al_probe_points_table->setMinimumSize(QSize(0, 24));
    al_probe_points_table->setProperty("verticalScrollBarPolicy", 1);
    al_probe_points_table->setColumnCount(3);
    al_probe_points_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("X-Y Coordinates")), trUi_(QString::fromUtf8("Height"))});
    al_probe_points_table->verticalHeader()->setVisible(false);
    al_probe_points_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    al_probe_points_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    al_probe_points_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode(3));
    lay9->addWidget(al_probe_points_table, 1, 0, 1, 2);
    plot_probing_pts_cb = new FCCheckBox(nullptr);
    plot_probing_pts_cb->setText(trUi_(QString::fromUtf8("Plot probing points")));
    plot_probing_pts_cb->setToolTip(trUi_(QString::fromUtf8("Plot the probing points in the table.\nIf a Voronoi method is used then\nthe Voronoi areas are also plotted.")));
    lay9->addWidget(plot_probing_pts_cb, 3, 0, 1, 2);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    lay9->addWidget(w12, 5, 0, 1, 2);
    probe_gc_label = new FCLabel(nullptr);
    probe_gc_label->setText(trUi_(QString::fromUtf8("<b>Probe GCode Generation</b>:")));
    probe_gc_label->setToolTip(trUi_(QString::fromUtf8("Will create a GCode which will be sent to the controller,\neither through a file or directly, with the intent to get the height map\nthat is to modify the original GCode to level the cutting height.")));
    lay9->addWidget(probe_gc_label, 7, 0, 1, 2);
    ptravelz_label = new FCLabel(nullptr);
    ptravelz_label->setText(trUi_(QString::fromUtf8("Probe Z travel:")));
    ptravelz_label->setToolTip(trUi_(QString::fromUtf8("The safe Z for probe travelling between probe points.")));
    lay9->addWidget(ptravelz_label, 9, 0, 1, 1);
    ptravelz_entry = new FCDoubleSpinner(nullptr);
    ptravelz_entry->setProperty("decimals", 4);
    ptravelz_entry->setProperty("maximum", 10000.0);
    ptravelz_entry->setProperty("value", 2.0);
    lay9->addWidget(ptravelz_entry, 9, 1, 1, 1);
    pdepth_label = new FCLabel(nullptr);
    pdepth_label->setText(trUi_(QString::fromUtf8("Probe Z depth:")));
    pdepth_label->setToolTip(trUi_(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units.")));
    lay9->addWidget(pdepth_label, 11, 0, 1, 1);
    pdepth_entry = new FCDoubleSpinner(nullptr);
    pdepth_entry->setProperty("decimals", 4);
    pdepth_entry->setProperty("maximum", 0.0);
    pdepth_entry->setProperty("minimum", -910000.0);
    pdepth_entry->setProperty("value", -1.0);
    lay9->addWidget(pdepth_entry, 11, 1, 1, 1);
    feedrate_probe_label = new FCLabel(nullptr);
    feedrate_probe_label->setText(trUi_(QString::fromUtf8("Probe Feedrate:")));
    feedrate_probe_label->setToolTip(trUi_(QString::fromUtf8("The feedrate used while the probe is probing.")));
    lay9->addWidget(feedrate_probe_label, 13, 0, 1, 1);
    feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    feedrate_probe_entry->setProperty("decimals", 4);
    feedrate_probe_entry->setProperty("maximum", 910000.0);
    feedrate_probe_entry->setProperty("value", 120.0);
    lay9->addWidget(feedrate_probe_entry, 13, 1, 1, 1);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    lay9->addWidget(w13, 15, 0, 1, 2);
    auto* w14 = new FCLabel(nullptr);
    w14->setText(trUi_(QString::fromUtf8("<b>Mode</b>:")));
    w14->setToolTip(trUi_(QString::fromUtf8("Choose a mode for height map generation.\n- Manual: will pick a selection of probe points by clicking on canvas\n- Grid: will automatically generate a grid of probe points")));
    lay9->addWidget(w14, 16, 0, 1, 1);
    al_mode_radio = new RadioSet({{trUi_(QString::fromUtf8("Manual")), QVariant(QString::fromUtf8("manual"))}, {trUi_(QString::fromUtf8("Grid")), QVariant(QString::fromUtf8("grid"))}}, Qt::Horizontal, true, nullptr);
    al_mode_radio->setValue(QVariant(QString::fromUtf8("grid")));
    lay9->addWidget(al_mode_radio, 16, 1, 1, 1);
    al_method_lbl = new FCLabel(nullptr);
    al_method_lbl->setText(trUi_(QString::fromUtf8("Method:")));
    al_method_lbl->setToolTip(trUi_(QString::fromUtf8("Choose a method for approximation of heights from autolevelling data.\n- Voronoi: will generate a Voronoi diagram\n- Bilinear: will use bilinear interpolation. Usable only for grid mode.")));
    lay9->addWidget(al_method_lbl, 17, 0, 1, 1);
    al_method_radio = new RadioSet({{trUi_(QString::fromUtf8("Voronoi")), QVariant(QString::fromUtf8("v"))}, {trUi_(QString::fromUtf8("Bilinear")), QVariant(QString::fromUtf8("b"))}}, Qt::Horizontal, true, nullptr);
    al_method_radio->setValue(QVariant(QString::fromUtf8("v")));
    lay9->addWidget(al_method_radio, 17, 1, 1, 1);
    al_columns_label = new FCLabel(nullptr);
    al_columns_label->setText(trUi_(QString::fromUtf8("Columns:")));
    al_columns_label->setToolTip(trUi_(QString::fromUtf8("The number of grid columns.")));
    lay9->addWidget(al_columns_label, 19, 0, 1, 1);
    al_columns_entry = new FCSpinner(nullptr);
    al_columns_entry->setProperty("minimum", 1);
    al_columns_entry->setProperty("value", 4);
    lay9->addWidget(al_columns_entry, 19, 1, 1, 1);
    al_rows_label = new FCLabel(nullptr);
    al_rows_label->setText(trUi_(QString::fromUtf8("Rows:")));
    al_rows_label->setToolTip(trUi_(QString::fromUtf8("The number of grid rows.")));
    lay9->addWidget(al_rows_label, 21, 0, 1, 1);
    al_rows_entry = new FCSpinner(nullptr);
    al_rows_entry->setProperty("minimum", 1);
    al_rows_entry->setProperty("value", 4);
    lay9->addWidget(al_rows_entry, 21, 1, 1, 1);
    al_add_button = new FCButton(nullptr);
    al_add_button->setText(trUi_(QString::fromUtf8("Add Probe Points")));
    lay9->addWidget(al_add_button, 23, 0, 1, 2);
    auto* w15 = new QFrame(nullptr);
    w15->setProperty("frameShadow", 48);
    w15->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w15->setSizePolicy(sp); }
    lay9->addWidget(w15, 25, 0, 1, 2);
    al_controller_label = new FCLabel(nullptr);
    al_controller_label->setText(trUi_(QString::fromUtf8("<b>Controller</b>:")));
    al_controller_label->setToolTip(trUi_(QString::fromUtf8("The kind of controller for which to generate\nheight map gcode.")));
    lay9->addWidget(al_controller_label, 27, 0, 1, 1);
    al_controller_combo = new FCComboBox(nullptr);
    al_controller_combo->setProperty("currentText", trUi_(QString::fromUtf8("MACH3")));
    al_controller_combo->addItem(trUi_(QString::fromUtf8("MACH3")));
    al_controller_combo->addItem(trUi_(QString::fromUtf8("MACH4")));
    al_controller_combo->addItem(trUi_(QString::fromUtf8("LinuxCNC")));
    al_controller_combo->addItem(trUi_(QString::fromUtf8("GRBL")));
    lay9->addWidget(al_controller_combo, 27, 1, 1, 1);
    grbl_frame = new QFrame(nullptr);
    grbl_frame->setHidden(true);
    grbl_box = new QVBoxLayout();
    grbl_box->setContentsMargins(0, 0, 0, 0);
    grbl_box->setSpacing(6);
    al_toolbar = new FCDetachableTab(nullptr);
    al_toolbar->setTabsClosable(false);
    gr_conn_tab = new QWidget(nullptr);
    gr_conn_tab->setProperty("objectName", QString::fromUtf8("connect_tab"));
    gr_conn_tab_layout = new QVBoxLayout();
    gr_conn_tab_layout->setContentsMargins(2, 2, 2, 2);
    gr_conn_tab_layout->setSpacing(6);
    auto* lay16 = new QGridLayout();
    lay16->setContentsMargins(0, 0, 0, 0);
    lay16->setHorizontalSpacing(6);
    lay16->setVerticalSpacing(6);
    com_list_label = new FCLabel(nullptr);
    com_list_label->setText(trUi_(QString::fromUtf8("COM list:")));
    com_list_label->setToolTip(trUi_(QString::fromUtf8("Lists the available serial ports.")));
    lay16->addWidget(com_list_label, 2, 0, 1, 1);
    com_list_combo = new FCComboBox(nullptr);
    lay16->addWidget(com_list_combo, 2, 1, 1, 1);
    com_search_button = new FCButton(nullptr);
    com_search_button->setText(trUi_(QString::fromUtf8("Search")));
    com_search_button->setToolTip(trUi_(QString::fromUtf8("Search for the available serial ports.")));
    lay16->addWidget(com_search_button, 2, 2, 1, 1);
    baudrates_list_label = new FCLabel(nullptr);
    baudrates_list_label->setText(trUi_(QString::fromUtf8("Baud rates:")));
    baudrates_list_label->setToolTip(trUi_(QString::fromUtf8("Lists the available serial ports.")));
    lay16->addWidget(baudrates_list_label, 4, 0, 1, 1);
    baudrates_list_combo = new FCComboBox(nullptr);
    baudrates_list_combo->setProperty("currentText", trUi_(QString::fromUtf8("115200")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("9600")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("19200")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("38400")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("57600")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("115200")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("230400")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("460800")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("500000")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("576000")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("921600")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("1000000")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("1152000")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("1500000")));
    baudrates_list_combo->addItem(trUi_(QString::fromUtf8("2000000")));
    baudrates_list_combo->setCurrentIndex(4);
    lay16->addWidget(baudrates_list_combo, 4, 1, 1, 1);
    new_bd_label = new FCLabel(nullptr);
    new_bd_label->setText(trUi_(QString::fromUtf8("New:")));
    new_bd_label->setToolTip(trUi_(QString::fromUtf8("New, custom baudrate.")));
    lay16->addWidget(new_bd_label, 6, 0, 1, 1);
    new_baudrate_entry = new FCSpinner(nullptr);
    new_baudrate_entry->setProperty("maximum", 9999999);
    new_baudrate_entry->setProperty("minimum", 40);
    new_baudrate_entry->setProperty("value", 40);
    lay16->addWidget(new_baudrate_entry, 6, 1, 1, 1);
    add_bd_button = new FCButton(nullptr);
    add_bd_button->setText(trUi_(QString::fromUtf8("Add")));
    add_bd_button->setToolTip(trUi_(QString::fromUtf8("Add the specified custom baudrate to the list.")));
    lay16->addWidget(add_bd_button, 6, 2, 1, 1);
    del_bd_button = new FCButton(nullptr);
    del_bd_button->setText(trUi_(QString::fromUtf8("Delete selected baudrate")));
    lay16->addWidget(del_bd_button, 8, 0, 1, 3);
    auto* w17 = new FCLabel(nullptr);
    lay16->addWidget(w17, 9, 0, 1, 3);
    auto* lay18 = new QHBoxLayout();
    lay18->setContentsMargins(0, 0, 0, 0);
    lay18->setSpacing(6);
    controller_reset_button = new FCButton(nullptr);
    controller_reset_button->setEnabled(false);
    controller_reset_button->setText(trUi_(QString::fromUtf8("Reset")));
    controller_reset_button->setToolTip(trUi_(QString::fromUtf8("Software reset of the controller.")));
    lay18->addWidget(controller_reset_button, 0);
    com_connect_button = new FCButton(nullptr);
    com_connect_button->setStyleSheet(QString::fromUtf8("QPushButton {background-color: red;}"));
    com_connect_button->setText(trUi_(QString::fromUtf8("Disconnected")));
    com_connect_button->setToolTip(trUi_(QString::fromUtf8("Connect to the selected port with the selected baud rate.")));
    lay18->addWidget(com_connect_button, 0);
    lay16->addLayout(lay18, 10, 0, 1, 3);
    lay16->setRowStretch(9, 1);
    lay16->setColumnStretch(1, 1);
    gr_conn_tab_layout->addLayout(lay16, 0);
    gr_conn_tab->setLayout(gr_conn_tab_layout);
    al_toolbar->addTab(gr_conn_tab, trUi_(QString::fromUtf8("Connect")));
    gr_ctrl_tab = new QWidget(nullptr);
    gr_ctrl_tab->setProperty("objectName", QString::fromUtf8("connect_tab"));
    gr_ctrl_tab->setHidden(true);
    gr_ctrl_tab_layout = new QVBoxLayout();
    gr_ctrl_tab_layout->setContentsMargins(2, 2, 2, 2);
    gr_ctrl_tab_layout->setSpacing(6);
    auto* lay19 = new QGridLayout();
    lay19->setContentsMargins(0, 0, 0, 0);
    lay19->setHorizontalSpacing(6);
    lay19->setVerticalSpacing(6);
    auto* w20 = new FCLabel(nullptr);
    { QFont f = w20->font(); f.setBold(true); w20->setFont(f); }
    w20->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tFCLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w20->setText(trUi_(QString::fromUtf8("Jog")));
    lay19->addWidget(w20, 0, 0, 1, 1);
    auto* w21 = new FCLabel(nullptr);
    { QFont f = w21->font(); f.setBold(true); w21->setFont(f); }
    w21->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tFCLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w21->setText(trUi_(QString::fromUtf8("Zero Axes")));
    lay19->addWidget(w21, 0, 2, 1, 1);
    jog_wdg = new QFrame(nullptr);
    jog_wdg->setProperty("frameShape", 1);
    { QSizePolicy sp(QSizePolicy::Policy(5), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); jog_wdg->setSizePolicy(sp); }
    jog_wdg->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\tFCJog\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tborder: 1px solid lightgray;\n\t\t\t\t\t\t\t\tborder-radius: 5px;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t"));
    auto* lay22 = new QGridLayout();
    lay22->setContentsMargins(2, 4, 2, 4);
    lay22->setHorizontalSpacing(6);
    lay22->setVerticalSpacing(6);
    lay22->setProperty("sizeConstraint", 2);
    auto* w23 = new QToolButton(nullptr);
    w23->setIcon(QIcon(QString::fromUtf8(":/images/up-arrow32.png")));
    w23->setToolTip(trUi_(QString::fromUtf8("Jog the Y axis.")));
    lay22->addWidget(w23, 2, 1, 1, 1);
    auto* w24 = new QToolButton(nullptr);
    w24->setIcon(QIcon(QString::fromUtf8(":/images/origin2_32.png")));
    w24->setToolTip(trUi_(QString::fromUtf8("Move to Origin")));
    lay22->addWidget(w24, 3, 1, 1, 1);
    auto* w25 = new QToolButton(nullptr);
    w25->setIcon(QIcon(QString::fromUtf8(":/images/down-arrow32.png")));
    w25->setToolTip(trUi_(QString::fromUtf8("Jog the Y axis.")));
    lay22->addWidget(w25, 4, 1, 1, 1);
    auto* w26 = new QToolButton(nullptr);
    w26->setIcon(QIcon(QString::fromUtf8(":/images/left_arrow32.png")));
    w26->setToolTip(trUi_(QString::fromUtf8("Jog the X axis.")));
    lay22->addWidget(w26, 3, 0, 1, 1);
    auto* w27 = new QToolButton(nullptr);
    w27->setIcon(QIcon(QString::fromUtf8(":/images/right_arrow32.png")));
    w27->setToolTip(trUi_(QString::fromUtf8("Jog the X axis.")));
    lay22->addWidget(w27, 3, 2, 1, 1);
    auto* w28 = new QToolButton(nullptr);
    w28->setIcon(QIcon(QString::fromUtf8(":/images/up-arrow32.png")));
    w28->setText(trUi_(QString::fromUtf8("Z")));
    w28->setProperty("toolButtonStyle", 2);
    w28->setToolTip(trUi_(QString::fromUtf8("Jog the Z axis.")));
    lay22->addWidget(w28, 2, 3, 1, 1);
    auto* w29 = new QToolButton(nullptr);
    w29->setIcon(QIcon(QString::fromUtf8(":/images/down-arrow32.png")));
    w29->setText(trUi_(QString::fromUtf8("Z")));
    w29->setProperty("toolButtonStyle", 2);
    w29->setToolTip(trUi_(QString::fromUtf8("Jog the Z axis.")));
    lay22->addWidget(w29, 4, 3, 1, 1);
    jog_wdg->setLayout(lay22);
    lay19->addWidget(jog_wdg, 2, 0, 1, 1);
    zero_axs_wdg = new QFrame(nullptr);
    zero_axs_wdg->setProperty("frameShape", 1);
    { QSizePolicy sp(QSizePolicy::Policy(5), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); zero_axs_wdg->setSizePolicy(sp); }
    zero_axs_wdg->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\tFCZeroAxes\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tborder: 1px solid lightgray;\n\t\t\t\t\t\t\t\tborder-radius: 5px\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t"));
    auto* lay30 = new QGridLayout();
    lay30->setContentsMargins(2, 4, 2, 4);
    lay30->setHorizontalSpacing(6);
    lay30->setVerticalSpacing(6);
    auto* w31 = new QToolButton(nullptr);
    w31->setText(trUi_(QString::fromUtf8("X")));
    w31->setToolTip(trUi_(QString::fromUtf8("Zero the CNC X axes at current position.")));
    lay30->addWidget(w31, 1, 0, 1, 1);
    auto* w32 = new QToolButton(nullptr);
    w32->setText(trUi_(QString::fromUtf8("Y")));
    w32->setToolTip(trUi_(QString::fromUtf8("Zero the CNC Y axes at current position.")));
    lay30->addWidget(w32, 2, 0, 1, 1);
    auto* w33 = new QToolButton(nullptr);
    w33->setText(trUi_(QString::fromUtf8("Z")));
    w33->setToolTip(trUi_(QString::fromUtf8("Zero the CNC Z axes at current position.")));
    lay30->addWidget(w33, 3, 0, 1, 1);
    auto* w34 = new QToolButton(nullptr);
    w34->setText(trUi_(QString::fromUtf8("Do Home")));
    w34->setToolTip(trUi_(QString::fromUtf8("Perform a homing cycle on all axis.")));
    lay30->addWidget(w34, 4, 0, 1, 2);
    auto* w35 = new QToolButton(nullptr);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(7)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w35->setSizePolicy(sp); }
    w35->setText(trUi_(QString::fromUtf8("All")));
    w35->setToolTip(trUi_(QString::fromUtf8("Zero all CNC axes at current position.")));
    lay30->addWidget(w35, 1, 1, 3, 1);
    zero_axs_wdg->setLayout(lay30);
    lay19->addWidget(zero_axs_wdg, 2, 2, 1, 1);
    auto* w36 = new QFrame(nullptr);
    { QSizePolicy sp(QSizePolicy::Policy(13), QSizePolicy::Policy(7)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w36->setSizePolicy(sp); }
    auto* lay37 = new QHBoxLayout();
    lay37->setContentsMargins(0, 0, 0, 0);
    lay37->setSpacing(6);
    pause_resume_button = new QToolButton(nullptr);
    pause_resume_button->setCheckable(true);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(7)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); pause_resume_button->setSizePolicy(sp); }
    pause_resume_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\tRotatedToolButton:checked\n\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\tbackground-color: red;\n\t\t\t\t\t\t\t\tcolor: white;\n\t\t\t\t\t\t\t\tborder: none;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t"));
    pause_resume_button->setText(trUi_(QString::fromUtf8("Pause/Resume")));
    lay37->addWidget(pause_resume_button, 0);
    w36->setLayout(lay37);
    lay19->addWidget(w36, 2, 1, 1, 1);
    lay19->setColumnStretch(1, 1);
    gr_ctrl_tab_layout->addLayout(lay19, 0);
    auto* lay38 = new QGridLayout();
    lay38->setContentsMargins(0, 0, 0, 0);
    lay38->setHorizontalSpacing(6);
    lay38->setVerticalSpacing(6);
    jog_step_label = new FCLabel(nullptr);
    jog_step_label->setText(trUi_(QString::fromUtf8("Step:")));
    jog_step_label->setToolTip(trUi_(QString::fromUtf8("Each jog action will move the axes with this value.")));
    lay38->addWidget(jog_step_label, 0, 0, 1, 1);
    jog_step_entry = new QFrame(nullptr);
    auto* lay39 = new QHBoxLayout();
    lay39->setContentsMargins(0, 0, 0, 0);
    lay39->setSpacing(6);
    auto* w40 = new QSlider(nullptr);
    w40->setProperty("maximum", 500000);
    w40->setProperty("singleStep", 1000);
    w40->setProperty("value", 5000);
    lay39->addWidget(w40, 0);
    auto* w41 = new FCDoubleSpinner(nullptr);
    w41->setProperty("decimals", 4);
    w41->setProperty("maximum", 500.0);
    w41->setMinimumSize(QSize(70, 0));
    w41->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(5), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w41->setSizePolicy(sp); }
    w41->setProperty("value", 5.0);
    lay39->addWidget(w41, 0);
    jog_step_entry->setLayout(lay39);
    lay38->addWidget(jog_step_entry, 0, 1, 1, 1);
    jog_fr_label = new FCLabel(nullptr);
    jog_fr_label->setText(trUi_(QString::fromUtf8("Feedrate:")));
    jog_fr_label->setToolTip(trUi_(QString::fromUtf8("Feedrate when jogging.")));
    lay38->addWidget(jog_fr_label, 1, 0, 1, 1);
    jog_fr_entry = new QFrame(nullptr);
    auto* lay42 = new QHBoxLayout();
    lay42->setContentsMargins(0, 0, 0, 0);
    lay42->setSpacing(6);
    auto* w43 = new QSlider(nullptr);
    w43->setProperty("maximum", 10000000);
    w43->setProperty("singleStep", 1000);
    w43->setProperty("value", 1500000);
    lay42->addWidget(w43, 0);
    auto* w44 = new FCDoubleSpinner(nullptr);
    w44->setProperty("decimals", 4);
    w44->setProperty("maximum", 10000.0);
    w44->setMinimumSize(QSize(70, 0));
    w44->setProperty("singleStep", 10.0);
    { QSizePolicy sp(QSizePolicy::Policy(5), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w44->setSizePolicy(sp); }
    w44->setProperty("value", 1500.0);
    lay42->addWidget(w44, 0);
    jog_fr_entry->setLayout(lay42);
    lay38->addWidget(jog_fr_entry, 1, 1, 1, 1);
    lay38->setColumnStretch(1, 1);
    gr_ctrl_tab_layout->addLayout(lay38, 0);
    gr_ctrl_tab_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    gr_ctrl_tab_layout->setStretch(gr_ctrl_tab_layout->count() - 1, 1);
    gr_ctrl_tab->setLayout(gr_ctrl_tab_layout);
    al_toolbar->addTab(gr_ctrl_tab, trUi_(QString::fromUtf8("Control")));
    gr_send_tab = new QWidget(nullptr);
    gr_send_tab->setProperty("objectName", QString::fromUtf8("connect_tab"));
    gr_send_tab->setHidden(true);
    gr_send_tab_layout = new QVBoxLayout();
    gr_send_tab_layout->setContentsMargins(2, 2, 2, 2);
    gr_send_tab_layout->setSpacing(6);
    auto* lay45 = new QGridLayout();
    lay45->setContentsMargins(0, 0, 0, 0);
    lay45->setHorizontalSpacing(6);
    lay45->setVerticalSpacing(6);
    grbl_command_label = new FCLabel(nullptr);
    grbl_command_label->setText(trUi_(QString::fromUtf8("Send Command:")));
    grbl_command_label->setToolTip(trUi_(QString::fromUtf8("Send a custom command to GRBL.")));
    lay45->addWidget(grbl_command_label, 2, 0, 1, 2);
    grbl_command_entry = new FCEntry(nullptr);
    grbl_command_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Type GRBL command ...")));
    lay45->addWidget(grbl_command_entry, 4, 0, 1, 1);
    grbl_send_button = new QToolButton(nullptr);
    grbl_send_button->setText(trUi_(QString::fromUtf8("Send")));
    grbl_send_button->setToolTip(trUi_(QString::fromUtf8("Send a custom command to GRBL.")));
    lay45->addWidget(grbl_send_button, 4, 1, 1, 1);
    grbl_get_param_label = new FCLabel(nullptr);
    grbl_get_param_label->setText(trUi_(QString::fromUtf8("Get Config parameter:")));
    grbl_get_param_label->setToolTip(trUi_(QString::fromUtf8("A GRBL configuration parameter.")));
    lay45->addWidget(grbl_get_param_label, 6, 0, 1, 2);
    grbl_parameter_entry = new FCEntry(nullptr);
    grbl_parameter_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Type GRBL parameter ...")));
    lay45->addWidget(grbl_parameter_entry, 8, 0, 1, 1);
    grbl_get_param_button = new QToolButton(nullptr);
    grbl_get_param_button->setText(trUi_(QString::fromUtf8("Get")));
    grbl_get_param_button->setToolTip(trUi_(QString::fromUtf8("Get the value of a specified GRBL parameter.")));
    lay45->addWidget(grbl_get_param_button, 8, 1, 1, 1);
    grbl_report_button = new FCButton(nullptr);
    grbl_report_button->setText(trUi_(QString::fromUtf8("Get Report")));
    grbl_report_button->setToolTip(trUi_(QString::fromUtf8("Print in shell the GRBL report.")));
    lay45->addWidget(grbl_report_button, 10, 0, 1, 2);
    auto* lay46 = new QHBoxLayout();
    lay46->setContentsMargins(0, 0, 0, 0);
    lay46->setSpacing(6);
    grbl_get_heightmap_button = new FCButton(nullptr);
    grbl_get_heightmap_button->setEnabled(false);
    grbl_get_heightmap_button->setText(trUi_(QString::fromUtf8("Apply AutoLevelling")));
    grbl_get_heightmap_button->setToolTip(trUi_(QString::fromUtf8("Will send the probing GCode to the GRBL controller,\nwait for the Z probing data and then apply this data\nover the original GCode therefore doing autolevelling.")));
    lay46->addWidget(grbl_get_heightmap_button, 1);
    grbl_save_height_map_button = new QToolButton(nullptr);
    grbl_save_height_map_button->setEnabled(false);
    grbl_save_height_map_button->setIcon(QIcon(QString::fromUtf8(":/images/save_as.png")));
    grbl_save_height_map_button->setToolTip(trUi_(QString::fromUtf8("Will save the GRBL height map.")));
    lay46->addWidget(grbl_save_height_map_button, 0, Qt::Alignment(2));
    lay45->addLayout(lay46, 12, 0, 1, 2);
    lay45->setRowStretch(9, 1);
    lay45->setColumnStretch(0, 1);
    gr_send_tab_layout->addLayout(lay45, 0);
    gr_send_tab->setLayout(gr_send_tab_layout);
    al_toolbar->addTab(gr_send_tab, trUi_(QString::fromUtf8("Sender")));
    grbl_box->addWidget(al_toolbar, 0);
    grbl_frame->setLayout(grbl_box);
    lay9->addWidget(grbl_frame, 29, 0, 1, 2);
    auto* lay47 = new QHBoxLayout();
    lay47->setContentsMargins(0, 0, 0, 0);
    lay47->setSpacing(6);
    h_gcode_button = new FCButton(nullptr);
    h_gcode_button->setEnabled(false);
    { QSizePolicy sp(QSizePolicy::Policy(7), QSizePolicy::Policy(3)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); h_gcode_button->setSizePolicy(sp); }
    h_gcode_button->setText(trUi_(QString::fromUtf8("Save Probing GCode")));
    h_gcode_button->setToolTip(trUi_(QString::fromUtf8("Will save the probing GCode.")));
    lay47->addWidget(h_gcode_button, 0);
    view_h_gcode_button = new QToolButton(nullptr);
    view_h_gcode_button->setEnabled(false);
    view_h_gcode_button->setIcon(QIcon(QString::fromUtf8(":/images/edit_file32.png")));
    view_h_gcode_button->setToolTip(trUi_(QString::fromUtf8("View/Edit the probing GCode.")));
    lay47->addWidget(view_h_gcode_button, 0);
    lay9->addLayout(lay47, 31, 0, 1, 2);
    import_heights_button = new FCButton(nullptr);
    import_heights_button->setText(trUi_(QString::fromUtf8("Import Height Map")));
    import_heights_button->setToolTip(trUi_(QString::fromUtf8("Import the file that has the Z heights\nobtained through probing and then apply this data\nover the original GCode therefore\ndoing autolevelling.")));
    lay9->addWidget(import_heights_button, 33, 0, 1, 2);
    auto* w48 = new QFrame(nullptr);
    w48->setProperty("frameShadow", 48);
    w48->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w48->setSizePolicy(sp); }
    lay9->addWidget(w48, 35, 0, 1, 2);
    lay9->setColumnStretch(1, 1);
    al_box->addLayout(lay9, 0);
    al_frame->setLayout(al_box);
    custom_box->addWidget(al_frame, 0);
    export_gcode_label = new FCLabel(nullptr);
    export_gcode_label->setText(trUi_(QString::fromUtf8("<b>Export CNC Code:</b>")));
    export_gcode_label->setToolTip(trUi_(QString::fromUtf8("Export and save G-Code to\nmake this object to a file.")));
    custom_box->addWidget(export_gcode_label, 0);
    auto* lay49 = new QHBoxLayout();
    lay49->setContentsMargins(0, 0, 0, 0);
    lay49->setSpacing(6);
    export_gcode_button = new FCButton(nullptr);
    export_gcode_button->setIcon(QIcon(QString::fromUtf8(":/images/save_as.png")));
    { QSizePolicy sp(QSizePolicy::Policy(7), QSizePolicy::Policy(1)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); export_gcode_button->setSizePolicy(sp); }
    export_gcode_button->setText(trUi_(QString::fromUtf8("Save CNC Code")));
    export_gcode_button->setToolTip(trUi_(QString::fromUtf8("Opens dialog to save G-Code\nfile.")));
    lay49->addWidget(export_gcode_button, 0);
    review_gcode_button = new QToolButton(nullptr);
    review_gcode_button->setIcon(QIcon(QString::fromUtf8(":/images/find32.png")));
    review_gcode_button->setToolTip(trUi_(QString::fromUtf8("Review CNC Code.")));
    lay49->addWidget(review_gcode_button, 0);
    custom_box->addLayout(lay49, 0);
    custom_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->addLayout(custom_box, 0);
    common_grid = new QGridLayout();
    common_grid->setContentsMargins(0, 0, 0, 0);
    common_grid->setHorizontalSpacing(6);
    common_grid->setVerticalSpacing(6);
    auto* w50 = new QFrame(nullptr);
    w50->setProperty("frameShadow", 48);
    w50->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w50->setSizePolicy(sp); }
    w50->setHidden(true);
    common_grid->addWidget(w50, 1, 0, 1, 2);
    transform_label = new FCLabel(nullptr);
    transform_label->setText(trUi_(QString::fromUtf8("<b>Transformations</b>")));
    transform_label->setToolTip(trUi_(QString::fromUtf8("Geometrical transformations of the current object.")));
    transform_label->setHidden(true);
    common_grid->addWidget(transform_label, 2, 0, 1, 2);
    scale_entry = new NumericalEvalEntry(nullptr);
    scale_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    scale_entry->setText(trUi_(QString::fromUtf8("1.0000")));
    scale_entry->setToolTip(trUi_(QString::fromUtf8("Factor by which to multiply\ngeometric features of this object.\nExpressions are allowed. E.g: 1/25.4")));
    scale_entry->setHidden(true);
    common_grid->addWidget(scale_entry, 3, 0, 1, 1);
    scale_button = new FCButton(nullptr);
    scale_button->setMinimumSize(QSize(70, 0));
    scale_button->setText(trUi_(QString::fromUtf8("Scale")));
    scale_button->setToolTip(trUi_(QString::fromUtf8("Perform scaling operation.")));
    scale_button->setHidden(true);
    common_grid->addWidget(scale_button, 3, 1, 1, 1);
    offsetvector_entry = new NumericalEvalTupleEntry(nullptr);
    offsetvector_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    offsetvector_entry->setText(trUi_(QString::fromUtf8("(0.0, 0.0)")));
    offsetvector_entry->setToolTip(trUi_(QString::fromUtf8("Amount by which to move the object\nin the x and y axes in (x, y) format.\nExpressions are allowed. E.g: (1/3.2, 0.5*3)")));
    offsetvector_entry->setHidden(true);
    common_grid->addWidget(offsetvector_entry, 4, 0, 1, 1);
    offset_button = new FCButton(nullptr);
    offset_button->setMinimumSize(QSize(70, 0));
    offset_button->setText(trUi_(QString::fromUtf8("Offset")));
    offset_button->setToolTip(trUi_(QString::fromUtf8("Perform the offset operation.")));
    offset_button->setHidden(true);
    common_grid->addWidget(offset_button, 4, 1, 1, 1);
    transformations_button = new FCButton(nullptr);
    transformations_button->setIcon(QIcon(QString::fromUtf8(":/images/transform.png")));
    transformations_button->setText(trUi_(QString::fromUtf8("Transformations")));
    transformations_button->setToolTip(trUi_(QString::fromUtf8("Geometrical transformations of the current object.")));
    transformations_button->setHidden(true);
    common_grid->addWidget(transformations_button, 5, 0, 1, 2);
    common_grid->setColumnStretch(0, 1);
    lay1->addLayout(common_grid, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(lay1);
}

std::map<std::string, QObject*> CNCObjectUIBase::membersByName() const {
    return {
        {"add_bd_button", add_bd_button},
        {"al_add_button", al_add_button},
        {"al_box", al_box},
        {"al_columns_entry", al_columns_entry},
        {"al_columns_label", al_columns_label},
        {"al_controller_combo", al_controller_combo},
        {"al_controller_label", al_controller_label},
        {"al_frame", al_frame},
        {"al_method_lbl", al_method_lbl},
        {"al_method_radio", al_method_radio},
        {"al_mode_radio", al_mode_radio},
        {"al_probe_points_table", al_probe_points_table},
        {"al_rows_entry", al_rows_entry},
        {"al_rows_label", al_rows_label},
        {"al_toolbar", al_toolbar},
        {"annotation_cb", annotation_cb},
        {"baudrates_list_combo", baudrates_list_combo},
        {"baudrates_list_label", baudrates_list_label},
        {"cnc_tools_table", cnc_tools_table},
        {"cnc_tools_table_label", cnc_tools_table_label},
        {"cncplot_method_combo", cncplot_method_combo},
        {"cncplot_method_label", cncplot_method_label},
        {"com_connect_button", com_connect_button},
        {"com_list_combo", com_list_combo},
        {"com_list_label", com_list_label},
        {"com_search_button", com_search_button},
        {"common_grid", common_grid},
        {"controller_reset_button", controller_reset_button},
        {"custom_box", custom_box},
        {"del_bd_button", del_bd_button},
        {"editor_button", editor_button},
        {"exc_cnc_tools_table", exc_cnc_tools_table},
        {"export_gcode_button", export_gcode_button},
        {"export_gcode_label", export_gcode_label},
        {"feedrate_probe_entry", feedrate_probe_entry},
        {"feedrate_probe_label", feedrate_probe_label},
        {"gr_conn_tab", gr_conn_tab},
        {"gr_conn_tab_layout", gr_conn_tab_layout},
        {"gr_ctrl_tab", gr_ctrl_tab},
        {"gr_ctrl_tab_layout", gr_ctrl_tab_layout},
        {"gr_send_tab", gr_send_tab},
        {"gr_send_tab_layout", gr_send_tab_layout},
        {"grbl_box", grbl_box},
        {"grbl_command_entry", grbl_command_entry},
        {"grbl_command_label", grbl_command_label},
        {"grbl_frame", grbl_frame},
        {"grbl_get_heightmap_button", grbl_get_heightmap_button},
        {"grbl_get_param_button", grbl_get_param_button},
        {"grbl_get_param_label", grbl_get_param_label},
        {"grbl_parameter_entry", grbl_parameter_entry},
        {"grbl_report_button", grbl_report_button},
        {"grbl_save_height_map_button", grbl_save_height_map_button},
        {"grbl_send_button", grbl_send_button},
        {"h_gcode_button", h_gcode_button},
        {"icon", icon},
        {"import_heights_button", import_heights_button},
        {"jog_fr_entry", jog_fr_entry},
        {"jog_fr_label", jog_fr_label},
        {"jog_step_entry", jog_step_entry},
        {"jog_step_label", jog_step_label},
        {"jog_wdg", jog_wdg},
        {"level", level},
        {"name_entry", name_entry},
        {"name_hlay", name_hlay},
        {"new_baudrate_entry", new_baudrate_entry},
        {"new_bd_label", new_bd_label},
        {"offset_button", offset_button},
        {"offsetvector_entry", offsetvector_entry},
        {"pause_resume_button", pause_resume_button},
        {"pdepth_entry", pdepth_entry},
        {"pdepth_label", pdepth_label},
        {"plot_cb", plot_cb},
        {"plot_probing_pts_cb", plot_probing_pts_cb},
        {"probe_gc_label", probe_gc_label},
        {"properties_box", properties_box},
        {"properties_button", properties_button},
        {"properties_frame", properties_frame},
        {"ptravelz_entry", ptravelz_entry},
        {"ptravelz_label", ptravelz_label},
        {"review_gcode_button", review_gcode_button},
        {"sal_btn", sal_btn},
        {"scale_button", scale_button},
        {"scale_entry", scale_entry},
        {"show_al_table", show_al_table},
        {"snippets_cb", snippets_cb},
        {"t_distance_entry", t_distance_entry},
        {"t_distance_label", t_distance_label},
        {"t_time_entry", t_time_entry},
        {"t_time_label", t_time_label},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tooldia_entry", tooldia_entry},
        {"transform_label", transform_label},
        {"transformations_button", transformations_button},
        {"treeWidget", treeWidget},
        {"units_label", units_label},
        {"units_time_label", units_time_label},
        {"updateplot_button", updateplot_button},
        {"view_h_gcode_button", view_h_gcode_button},
        {"zero_axs_wdg", zero_axs_wdg},
    };
}

}  // namespace flatcam::gui
