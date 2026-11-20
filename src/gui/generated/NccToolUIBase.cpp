// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/NccToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void NccToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Non-Copper Clearing"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Non-Copper Clearing")));
    tools_box->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    type_obj_combo_label = new FCLabel(nullptr);
    type_obj_combo_label->setMinimumSize(QSize(60, 0));
    type_obj_combo_label->setText(trUi_(QString::fromUtf8("Obj Type:")));
    type_obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be cleared of excess copper.\nIt can be of type: Gerber or Geometry.\nWhat is selected here will dictate the kind\nof objects that will populate the 'Object' combobox.")));
    lay2->setWidget(0, QFormLayout::ItemRole(0), type_obj_combo_label);
    type_obj_radio = new RadioSet({{trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geometry"))}, {trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("gerber"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    lay2->setWidget(0, QFormLayout::ItemRole(1), type_obj_radio);
    object_label = new FCLabel(nullptr);
    object_label->setText(trUi_(QString::fromUtf8("Object:")));
    object_label->setToolTip(trUi_(QString::fromUtf8("Object to be cleared of excess copper.")));
    lay2->setWidget(1, QFormLayout::ItemRole(0), object_label);
    object_combo = new FCComboBox(nullptr);
    lay2->setWidget(1, QFormLayout::ItemRole(1), object_combo);
    tools_box->addLayout(lay2, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    tools_box->addWidget(w3, 0);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(trUi_(QString::fromUtf8("<b>Tools Table</b>")));
    tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools pool from which the algorithm\nwill pick the ones used for copper clearing.")));
    tools_box->addWidget(tools_table_label, 0);
    tools_table = new FCTable(nullptr);
    tools_table->setAcceptDrops(true);
    tools_table->setProperty("columnCount", 4);
    tools_table->setProperty("contextMenuPolicy", 2);
    tools_table->setProperty("dragDropMode", 4);
    tools_table->setDragDropOverwriteMode(false);
    tools_table->setDragEnabled(true);
    tools_table->setMaximumSize(QSize(16777215, 53));
    tools_table->setMinimumSize(QSize(0, 53));
    tools_table->setProperty("rowCount", 1);
    tools_table->setProperty("selectionBehavior", 1);
    tools_table->setProperty("verticalScrollBarPolicy", 1);
    tools_table->setColumnCount(4);
    tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Diameter")), trUi_(QString::fromUtf8("TT")), trUi_(QString::fromUtf8(""))});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    tools_table->setColumnHidden(3, true);
    tools_box->addWidget(tools_table, 0);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    ncc_order_label = new FCLabel(nullptr);
    ncc_order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    ncc_order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay4->addWidget(ncc_order_label, 1, 0, 1, 1);
    ncc_order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    ncc_order_radio->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    ncc_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay4->addWidget(ncc_order_radio, 1, 1, 1, 1);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay4->addWidget(w5, 2, 0, 1, 2);
    lay4->setColumnStretch(1, 1);
    tools_box->addLayout(lay4, 0);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(trUi_(QString::fromUtf8("<b>Add from DB</b>")));
    grid3->addWidget(tool_sel_label, 0, 0, 1, 2);
    new_tooldia_lbl = new FCLabel(nullptr);
    new_tooldia_lbl->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    new_tooldia_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool")));
    grid3->addWidget(new_tooldia_lbl, 2, 0, 1, 1);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    new_tooldia_entry = new FCDoubleSpinner(nullptr);
    new_tooldia_entry->setProperty("decimals", 4);
    new_tooldia_entry->setProperty("maximum", 10000.0);
    new_tooldia_entry->setProperty("objectName", QString::fromUtf8("Tool Dia"));
    new_tooldia_entry->setProperty("value", 0.1);
    lay6->addWidget(new_tooldia_entry, 0);
    find_optimal_button = new QToolButton(nullptr);
    find_optimal_button->setIcon(QIcon(QString::fromUtf8(":/images/open_excellon32.png")));
    find_optimal_button->setText(trUi_(QString::fromUtf8("Optimal")));
    find_optimal_button->setProperty("toolButtonStyle", 2);
    find_optimal_button->setToolTip(trUi_(QString::fromUtf8("Find a tool diameter that is guaranteed\nto do a complete isolation.")));
    lay6->addWidget(find_optimal_button, 0);
    grid3->addLayout(lay6, 2, 1, 1, 1);
    auto* lay7 = new QHBoxLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setSpacing(6);
    add_newtool_button = new FCButton(nullptr);
    add_newtool_button->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_newtool_button->setText(trUi_(QString::fromUtf8("Search and Add")));
    add_newtool_button->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nwith the diameter specified above.\nThis is done by a background search\nin the Tools Database. If nothing is found\nin the Tools DB then a default tool is added.")));
    lay7->addWidget(add_newtool_button, 0);
    addtool_from_db_btn = new FCButton(nullptr);
    addtool_from_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    addtool_from_db_btn->setText(trUi_(QString::fromUtf8("Pick from DB")));
    addtool_from_db_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nfrom the Tools Database.\nTools database administration in in:\nMenu: Options -> Tools Database")));
    lay7->addWidget(addtool_from_db_btn, 0);
    grid3->addLayout(lay7, 7, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    grid3->addWidget(w8, 8, 0, 1, 2);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    deltool_btn->setText(trUi_(QString::fromUtf8("Delete")));
    deltool_btn->setToolTip(trUi_(QString::fromUtf8("Delete a selection of tools in the Tool Table\nby first selecting a row in the Tool Table.")));
    grid3->addWidget(deltool_btn, 9, 0, 1, 2);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    grid3->addWidget(w9, 11, 0, 1, 2);
    tool_data_label = new FCLabel(nullptr);
    tool_data_label->setText(trUi_(QString::fromUtf8("<b>Parameters for: <font color=\"#7777FF\">Tool 1</font></b>")));
    tool_data_label->setToolTip(trUi_(QString::fromUtf8("The data used for creating GCode.\nEach tool store it's own set of such data.")));
    grid3->addWidget(tool_data_label, 12, 0, 1, 2);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Operation:")));
    w10->setToolTip(trUi_(QString::fromUtf8("The 'Operation' can be:\n- Isolation -> will ensure that the non-copper clearing is always complete.\nIf it's not successful then the non-copper clearing will fail, too.\n- Clear -> the regular non-copper clearing.")));
    grid3->addWidget(w10, 13, 0, 1, 1);
    op_radio = new RadioSet({{trUi_(QString::fromUtf8("Clear")), QVariant(QString::fromUtf8("clear"))}, {trUi_(QString::fromUtf8("Isolation")), QVariant(QString::fromUtf8("iso"))}}, Qt::Horizontal, false, nullptr);
    op_radio->setProperty("objectName", QString::fromUtf8("n_operation"));
    op_radio->setValue(QVariant(QString::fromUtf8("clear")));
    grid3->addWidget(op_radio, 13, 1, 1, 1);
    milling_type_label = new FCLabel(nullptr);
    milling_type_label->setEnabled(false);
    milling_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    milling_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    grid3->addWidget(milling_type_label, 14, 0, 1, 1);
    milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    milling_type_radio->setEnabled(false);
    milling_type_radio->setProperty("objectName", QString::fromUtf8("n_milling_type"));
    milling_type_radio->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    grid3->addWidget(milling_type_radio, 14, 1, 1, 1);
    nccoverlabel = new FCLabel(nullptr);
    nccoverlabel->setText(trUi_(QString::fromUtf8("Overlap:")));
    nccoverlabel->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.\nAdjust the value starting with lower values\nand increasing it if areas that should be processed are still \nnot processed.\nLower values = faster processing, faster execution on CNC.\nHigher values = slow processing and slow execution on CNC\ndue of too many paths.")));
    grid3->addWidget(nccoverlabel, 15, 0, 1, 1);
    ncc_overlap_entry = new FCDoubleSpinner(nullptr);
    ncc_overlap_entry->setProperty("decimals", 4);
    ncc_overlap_entry->setProperty("maximum", 99.9999);
    ncc_overlap_entry->setProperty("objectName", QString::fromUtf8("n_overlap"));
    ncc_overlap_entry->setProperty("singleStep", 0.1);
    ncc_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    ncc_overlap_entry->setProperty("value", 40.0);
    ncc_overlap_entry->setWrapping(true);
    grid3->addWidget(ncc_overlap_entry, 15, 1, 1, 1);
    methodlabel = new FCLabel(nullptr);
    methodlabel->setText(trUi_(QString::fromUtf8("Method:")));
    methodlabel->setToolTip(trUi_(QString::fromUtf8("Algorithm for copper clearing:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.")));
    grid3->addWidget(methodlabel, 16, 0, 1, 1);
    ncc_method_combo = new FCComboBox2(nullptr);
    ncc_method_combo->setProperty("currentText", trUi_(QString::fromUtf8("Seed")));
    ncc_method_combo->setProperty("objectName", QString::fromUtf8("n_method"));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Combo")));
    ncc_method_combo->setCurrentIndex(1);
    grid3->addWidget(ncc_method_combo, 16, 1, 1, 1);
    nccmarginlabel = new FCLabel(nullptr);
    nccmarginlabel->setText(trUi_(QString::fromUtf8("Margin:")));
    nccmarginlabel->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    grid3->addWidget(nccmarginlabel, 17, 0, 1, 1);
    ncc_margin_entry = new FCDoubleSpinner(nullptr);
    ncc_margin_entry->setProperty("decimals", 4);
    ncc_margin_entry->setProperty("maximum", 10000.0);
    ncc_margin_entry->setProperty("minimum", -10000.0);
    ncc_margin_entry->setProperty("objectName", QString::fromUtf8("n_margin"));
    ncc_margin_entry->setProperty("value", 1.0);
    grid3->addWidget(ncc_margin_entry, 17, 1, 1, 1);
    ncc_connect_cb = new FCCheckBox(nullptr);
    ncc_connect_cb->setChecked(true);
    ncc_connect_cb->setProperty("objectName", QString::fromUtf8("n_connect"));
    ncc_connect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    ncc_connect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    grid3->addWidget(ncc_connect_cb, 18, 0, 1, 1);
    ncc_contour_cb = new FCCheckBox(nullptr);
    ncc_contour_cb->setChecked(true);
    ncc_contour_cb->setProperty("objectName", QString::fromUtf8("n_contour"));
    ncc_contour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    ncc_contour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    grid3->addWidget(ncc_contour_cb, 18, 1, 1, 1);
    ncc_choice_offset_cb = new FCCheckBox(nullptr);
    ncc_choice_offset_cb->setProperty("objectName", QString::fromUtf8("n_offset"));
    ncc_choice_offset_cb->setText(trUi_(QString::fromUtf8("Offset")));
    ncc_choice_offset_cb->setToolTip(trUi_(QString::fromUtf8("If used, it will add an offset to the copper features.\nThe copper clearing will finish to a distance\nfrom the copper features.")));
    grid3->addWidget(ncc_choice_offset_cb, 19, 0, 1, 1);
    ncc_offset_spinner = new FCDoubleSpinner(nullptr);
    ncc_offset_spinner->setProperty("decimals", 4);
    ncc_offset_spinner->setProperty("maximum", 10.0);
    ncc_offset_spinner->setProperty("objectName", QString::fromUtf8("n_offset_value"));
    ncc_offset_spinner->setProperty("singleStep", 0.1);
    ncc_offset_spinner->setWrapping(true);
    grid3->addWidget(ncc_offset_spinner, 19, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    grid3->addWidget(w11, 21, 0, 1, 2);
    apply_param_to_all = new FCButton(nullptr);
    apply_param_to_all->setIcon(QIcon(QString::fromUtf8(":/images/param_all32.png")));
    apply_param_to_all->setText(trUi_(QString::fromUtf8("Apply parameters to all tools")));
    apply_param_to_all->setToolTip(trUi_(QString::fromUtf8("The parameters in the current form will be applied\non all the tools from the Tool Table.")));
    grid3->addWidget(apply_param_to_all, 22, 0, 1, 2);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    grid3->addWidget(w12, 23, 0, 1, 2);
    gen_param_label = new FCLabel(nullptr);
    gen_param_label->setText(trUi_(QString::fromUtf8("<b>Common Parameters</b>")));
    gen_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters that are common for all tools.")));
    grid3->addWidget(gen_param_label, 24, 0, 1, 2);
    ncc_rest_cb = new FCCheckBox(nullptr);
    ncc_rest_cb->setProperty("objectName", QString::fromUtf8("n_rest_machining"));
    ncc_rest_cb->setText(trUi_(QString::fromUtf8("Rest")));
    ncc_rest_cb->setToolTip(trUi_(QString::fromUtf8("If checked, use 'rest machining'.\nBasically it will process copper outside PCB features,\nusing the biggest tool and continue with the next tools,\nfrom bigger to smaller, to process the copper features that\ncould not be processed by previous tool, until there is\nnothing left to process or there are no more tools.\n\nIf not checked, use the standard algorithm.")));
    grid3->addWidget(ncc_rest_cb, 25, 0, 1, 2);
    rest_nccmarginlabel = new FCLabel(nullptr);
    rest_nccmarginlabel->setText(trUi_(QString::fromUtf8("Margin:")));
    rest_nccmarginlabel->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    rest_nccmarginlabel->setHidden(true);
    grid3->addWidget(rest_nccmarginlabel, 26, 0, 1, 1);
    rest_ncc_margin_entry = new FCDoubleSpinner(nullptr);
    rest_ncc_margin_entry->setProperty("decimals", 4);
    rest_ncc_margin_entry->setProperty("maximum", 10000.0);
    rest_ncc_margin_entry->setProperty("minimum", -10000.0);
    rest_ncc_margin_entry->setProperty("objectName", QString::fromUtf8("n_margin"));
    rest_ncc_margin_entry->setProperty("value", 1.0);
    rest_ncc_margin_entry->setHidden(true);
    grid3->addWidget(rest_ncc_margin_entry, 26, 1, 1, 1);
    rest_ncc_connect_cb = new FCCheckBox(nullptr);
    rest_ncc_connect_cb->setChecked(true);
    rest_ncc_connect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    rest_ncc_connect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    rest_ncc_connect_cb->setHidden(true);
    grid3->addWidget(rest_ncc_connect_cb, 27, 0, 1, 1);
    rest_ncc_contour_cb = new FCCheckBox(nullptr);
    rest_ncc_contour_cb->setChecked(true);
    rest_ncc_contour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    rest_ncc_contour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    rest_ncc_contour_cb->setHidden(true);
    grid3->addWidget(rest_ncc_contour_cb, 27, 1, 1, 1);
    rest_ncc_choice_offset_cb = new FCCheckBox(nullptr);
    rest_ncc_choice_offset_cb->setText(trUi_(QString::fromUtf8("Offset")));
    rest_ncc_choice_offset_cb->setToolTip(trUi_(QString::fromUtf8("If used, it will add an offset to the copper features.\nThe copper clearing will finish to a distance\nfrom the copper features.")));
    rest_ncc_choice_offset_cb->setHidden(true);
    grid3->addWidget(rest_ncc_choice_offset_cb, 28, 0, 1, 1);
    rest_ncc_offset_spinner = new FCDoubleSpinner(nullptr);
    rest_ncc_offset_spinner->setProperty("decimals", 4);
    rest_ncc_offset_spinner->setEnabled(false);
    rest_ncc_offset_spinner->setProperty("maximum", 10.0);
    rest_ncc_offset_spinner->setProperty("singleStep", 0.1);
    rest_ncc_offset_spinner->setWrapping(true);
    rest_ncc_offset_spinner->setHidden(true);
    grid3->addWidget(rest_ncc_offset_spinner, 28, 1, 1, 1);
    select_label = new FCLabel(nullptr);
    select_label->setText(trUi_(QString::fromUtf8("Selection:")));
    select_label->setToolTip(trUi_(QString::fromUtf8("Selection of area to be processed.\n- 'Itself' - the processing extent is based on the object that is processed.\n - 'Area Selection' - left mouse click to start selection of the area to be processed.\n- 'Reference Object' - will process the area specified by another object.")));
    grid3->addWidget(select_label, 29, 0, 1, 1);
    select_combo = new FCComboBox2(nullptr);
    select_combo->setProperty("currentText", trUi_(QString::fromUtf8("Itself")));
    select_combo->setProperty("objectName", QString::fromUtf8("n_selection"));
    select_combo->addItem(trUi_(QString::fromUtf8("Itself")));
    select_combo->addItem(trUi_(QString::fromUtf8("Area Selection")));
    select_combo->addItem(trUi_(QString::fromUtf8("Reference Object")));
    grid3->addWidget(select_combo, 29, 1, 1, 1);
    reference_combo_type_label = new FCLabel(nullptr);
    reference_combo_type_label->setText(trUi_(QString::fromUtf8("Type:")));
    reference_combo_type_label->setToolTip(trUi_(QString::fromUtf8("The type of FlatCAM object to be used as non copper clearing reference.\nIt can be Gerber, Excellon or Geometry.")));
    reference_combo_type_label->setHidden(true);
    grid3->addWidget(reference_combo_type_label, 31, 0, 1, 1);
    reference_combo_type = new FCComboBox2(nullptr);
    reference_combo_type->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    reference_combo_type->setHidden(true);
    reference_combo_type->addItem(trUi_(QString::fromUtf8("Gerber")));
    reference_combo_type->addItem(trUi_(QString::fromUtf8("Excellon")));
    reference_combo_type->addItem(trUi_(QString::fromUtf8("Geometry")));
    grid3->addWidget(reference_combo_type, 31, 1, 1, 1);
    reference_combo = new FCComboBox(nullptr);
    reference_combo->setHidden(true);
    grid3->addWidget(reference_combo, 33, 0, 1, 2);
    area_shape_label = new FCLabel(nullptr);
    area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    area_shape_label->setHidden(true);
    grid3->addWidget(area_shape_label, 35, 0, 1, 1);
    area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setHidden(true);
    area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    grid3->addWidget(area_shape_radio, 35, 1, 1, 1);
    valid_cb = new FCCheckBox(nullptr);
    valid_cb->setChecked(true);
    valid_cb->setProperty("objectName", QString::fromUtf8("n_check"));
    valid_cb->setText(trUi_(QString::fromUtf8("Check validity")));
    valid_cb->setToolTip(trUi_(QString::fromUtf8("If checked then the tools diameters are verified\nif they will provide a complete isolation.")));
    grid3->addWidget(valid_cb, 37, 0, 1, 2);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    grid3->addWidget(w13, 39, 0, 1, 2);
    grid3->setColumnStretch(1, 1);
    tools_box->addLayout(grid3, 0);
    generate_ncc_button = new FCButton(nullptr);
    { QFont f = generate_ncc_button->font(); f.setBold(true); generate_ncc_button->setFont(f); }
    generate_ncc_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_ncc_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    generate_ncc_button->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    generate_ncc_button->setToolTip(trUi_(QString::fromUtf8("Create the Geometry Object\nfor non-copper routing.")));
    tools_box->addWidget(generate_ncc_button, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
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

std::map<std::string, QObject*> NccToolUIBase::membersByName() const {
    return {
        {"add_newtool_button", add_newtool_button},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_label", area_shape_label},
        {"area_shape_radio", area_shape_radio},
        {"deltool_btn", deltool_btn},
        {"find_optimal_button", find_optimal_button},
        {"gen_param_label", gen_param_label},
        {"generate_ncc_button", generate_ncc_button},
        {"grid3", grid3},
        {"layout", layout},
        {"methodlabel", methodlabel},
        {"milling_type_label", milling_type_label},
        {"milling_type_radio", milling_type_radio},
        {"ncc_choice_offset_cb", ncc_choice_offset_cb},
        {"ncc_connect_cb", ncc_connect_cb},
        {"ncc_contour_cb", ncc_contour_cb},
        {"ncc_margin_entry", ncc_margin_entry},
        {"ncc_method_combo", ncc_method_combo},
        {"ncc_offset_spinner", ncc_offset_spinner},
        {"ncc_order_label", ncc_order_label},
        {"ncc_order_radio", ncc_order_radio},
        {"ncc_overlap_entry", ncc_overlap_entry},
        {"ncc_rest_cb", ncc_rest_cb},
        {"nccmarginlabel", nccmarginlabel},
        {"nccoverlabel", nccoverlabel},
        {"new_tooldia_entry", new_tooldia_entry},
        {"new_tooldia_lbl", new_tooldia_lbl},
        {"object_combo", object_combo},
        {"object_label", object_label},
        {"op_radio", op_radio},
        {"reference_combo", reference_combo},
        {"reference_combo_type", reference_combo_type},
        {"reference_combo_type_label", reference_combo_type_label},
        {"reset_button", reset_button},
        {"rest_ncc_choice_offset_cb", rest_ncc_choice_offset_cb},
        {"rest_ncc_connect_cb", rest_ncc_connect_cb},
        {"rest_ncc_contour_cb", rest_ncc_contour_cb},
        {"rest_ncc_margin_entry", rest_ncc_margin_entry},
        {"rest_ncc_offset_spinner", rest_ncc_offset_spinner},
        {"rest_nccmarginlabel", rest_nccmarginlabel},
        {"select_combo", select_combo},
        {"select_label", select_label},
        {"tool_data_label", tool_data_label},
        {"tool_sel_label", tool_sel_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"type_obj_combo_label", type_obj_combo_label},
        {"type_obj_radio", type_obj_radio},
        {"valid_cb", valid_cb},
    };
}

}  // namespace flatcam::gui
