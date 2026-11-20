// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/IsolationToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void IsolationToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Isolation Tool"));
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
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Isolation Tool")));
    w1->setToolTip(trUi_(QString::fromUtf8("Create a Geometry object with\ntoolpaths to cut around polygons.")));
    title_box->addWidget(w1, 0);
    level = new FCLabel(nullptr);
    level->setProperty("alignment", 130);
    level->setText(trUi_(QString::fromUtf8("<span style=\"color:green;\"><b>Basic</b></span>")));
    level->setToolTip(trUi_(QString::fromUtf8("BASIC is suitable for a beginner. Many parameters\nare hidden from the user in this mode.\nADVANCED mode will make available all parameters.\n\nTo change the application LEVEL, go to:\nEdit -> Preferences -> General and check:\n'APP. LEVEL' radio button.")));
    title_box->addWidget(level, 0);
    tools_box->addLayout(title_box, 0);
    obj_combo_label = new FCLabel(nullptr);
    obj_combo_label->setText(trUi_(QString::fromUtf8("<b>GERBER</b>:")));
    obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Gerber object for isolation routing.")));
    tools_box->addWidget(obj_combo_label, 0);
    object_combo = new FCComboBox(nullptr);
    tools_box->addWidget(object_combo, 0);
    auto* w2 = new QFrame(nullptr);
    w2->setProperty("frameShadow", 48);
    w2->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w2->setSizePolicy(sp); }
    tools_box->addWidget(w2, 0);
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
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    order_label = new FCLabel(nullptr);
    order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay3->addWidget(order_label, 1, 0, 1, 1);
    order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay3->addWidget(order_radio, 1, 1, 1, 1);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 2, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    tools_box->addLayout(lay3, 0);
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
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    new_tooldia_entry = new FCDoubleSpinner(nullptr);
    new_tooldia_entry->setProperty("decimals", 4);
    new_tooldia_entry->setProperty("maximum", 10000.0);
    new_tooldia_entry->setProperty("objectName", QString::fromUtf8("i_new_tooldia"));
    new_tooldia_entry->setProperty("value", 0.1);
    lay5->addWidget(new_tooldia_entry, 0);
    find_optimal_button = new QToolButton(nullptr);
    find_optimal_button->setIcon(QIcon(QString::fromUtf8(":/images/open_excellon32.png")));
    find_optimal_button->setText(trUi_(QString::fromUtf8("Optimal")));
    find_optimal_button->setProperty("toolButtonStyle", 2);
    find_optimal_button->setToolTip(trUi_(QString::fromUtf8("Find a tool diameter that is guaranteed\nto do a complete isolation.")));
    lay5->addWidget(find_optimal_button, 0);
    grid3->addLayout(lay5, 2, 1, 1, 1);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    search_and_add_btn = new FCButton(nullptr);
    search_and_add_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    search_and_add_btn->setText(trUi_(QString::fromUtf8("Search and Add")));
    search_and_add_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nwith the diameter specified above.\nThis is done by a background search\nin the Tools Database. If nothing is found\nin the Tools DB then a default tool is added.")));
    lay6->addWidget(search_and_add_btn, 0);
    addtool_from_db_btn = new FCButton(nullptr);
    addtool_from_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    addtool_from_db_btn->setText(trUi_(QString::fromUtf8("Pick from DB")));
    addtool_from_db_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool to the Tool Table\nfrom the Tools Database.\nTools database administration in in:\nMenu: Options -> Tools Database")));
    lay6->addWidget(addtool_from_db_btn, 0);
    grid3->addLayout(lay6, 7, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    grid3->addWidget(w7, 8, 0, 1, 2);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    deltool_btn->setText(trUi_(QString::fromUtf8("Delete")));
    deltool_btn->setToolTip(trUi_(QString::fromUtf8("Delete a selection of tools in the Tool Table\nby first selecting a row in the Tool Table.")));
    grid3->addWidget(deltool_btn, 9, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    grid3->addWidget(w8, 11, 0, 1, 2);
    tool_data_label = new FCLabel(nullptr);
    tool_data_label->setText(trUi_(QString::fromUtf8("<b>Parameters for: <font color=\"#7777FF\">Tool 1</font></b>")));
    tool_data_label->setToolTip(trUi_(QString::fromUtf8("The data used for creating GCode.\nEach tool store it's own set of such data.")));
    grid3->addWidget(tool_data_label, 12, 0, 1, 2);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("Passes:")));
    w9->setToolTip(trUi_(QString::fromUtf8("Width of the isolation gap in\nnumber (integer) of tool widths.")));
    grid3->addWidget(w9, 13, 0, 1, 1);
    passes_entry = new FCSpinner(nullptr);
    passes_entry->setProperty("maximum", 999);
    passes_entry->setProperty("minimum", 1);
    passes_entry->setProperty("objectName", QString::fromUtf8("i_passes"));
    passes_entry->setProperty("value", 1);
    grid3->addWidget(passes_entry, 13, 1, 1, 1);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Overlap:")));
    w10->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.")));
    grid3->addWidget(w10, 14, 0, 1, 1);
    iso_overlap_entry = new FCDoubleSpinner(nullptr);
    iso_overlap_entry->setProperty("decimals", 4);
    iso_overlap_entry->setProperty("maximum", 99.9999);
    iso_overlap_entry->setProperty("objectName", QString::fromUtf8("i_overlap"));
    iso_overlap_entry->setProperty("singleStep", 0.1);
    iso_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    iso_overlap_entry->setProperty("value", 10.0);
    iso_overlap_entry->setWrapping(true);
    grid3->addWidget(iso_overlap_entry, 14, 1, 1, 1);
    milling_type_label = new FCLabel(nullptr);
    milling_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    milling_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    milling_type_label->setHidden(true);
    grid3->addWidget(milling_type_label, 15, 0, 1, 1);
    milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    milling_type_radio->setProperty("objectName", QString::fromUtf8("i_milling_type"));
    milling_type_radio->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    milling_type_radio->setHidden(true);
    milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    grid3->addWidget(milling_type_radio, 15, 1, 1, 1);
    follow_label = new FCLabel(nullptr);
    follow_label->setText(trUi_(QString::fromUtf8("Follow:")));
    follow_label->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    follow_label->setHidden(true);
    grid3->addWidget(follow_label, 16, 0, 1, 1);
    follow_cb = new FCCheckBox(nullptr);
    follow_cb->setProperty("objectName", QString::fromUtf8("i_follow"));
    follow_cb->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    follow_cb->setHidden(true);
    grid3->addWidget(follow_cb, 16, 1, 1, 1);
    iso_type_label = new FCLabel(nullptr);
    iso_type_label->setText(trUi_(QString::fromUtf8("Isolation Type:")));
    iso_type_label->setToolTip(trUi_(QString::fromUtf8("Choose how the isolation will be executed:\n- 'Full' -> complete isolation of polygons\n- 'Ext' -> will isolate only on the outside\n- 'Int' -> will isolate only on the inside\n'Exterior' isolation is almost always possible\n(with the right tool) but 'Interior'\nisolation can be done only when there is an opening\ninside of the polygon (e.g polygon is a 'doughnut' shape).")));
    iso_type_label->setHidden(true);
    grid3->addWidget(iso_type_label, 17, 0, 1, 1);
    iso_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Full")), QVariant(QString::fromUtf8("full"))}, {trUi_(QString::fromUtf8("Ext")), QVariant(QString::fromUtf8("ext"))}, {trUi_(QString::fromUtf8("Int")), QVariant(QString::fromUtf8("int"))}}, Qt::Horizontal, true, nullptr);
    iso_type_radio->setProperty("objectName", QString::fromUtf8("i_iso_type"));
    iso_type_radio->setHidden(true);
    iso_type_radio->setValue(QVariant(QString::fromUtf8("full")));
    grid3->addWidget(iso_type_radio, 17, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    grid3->addWidget(w11, 18, 0, 1, 2);
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
    rest_cb = new FCCheckBox(nullptr);
    rest_cb->setProperty("objectName", QString::fromUtf8("i_rest"));
    rest_cb->setText(trUi_(QString::fromUtf8("Rest")));
    rest_cb->setToolTip(trUi_(QString::fromUtf8("If checked, use 'rest machining'.\nBasically it will process copper outside PCB features,\nusing the biggest tool and continue with the next tools,\nfrom bigger to smaller, to process the copper features that\ncould not be processed by previous tool, until there is\nnothing left to process or there are no more tools.\n\nIf not checked, use the standard algorithm.")));
    rest_cb->setHidden(true);
    grid3->addWidget(rest_cb, 25, 0, 1, 1);
    forced_rest_iso_cb = new FCCheckBox(nullptr);
    forced_rest_iso_cb->setChecked(true);
    forced_rest_iso_cb->setEnabled(false);
    forced_rest_iso_cb->setText(trUi_(QString::fromUtf8("Forced Rest")));
    forced_rest_iso_cb->setToolTip(trUi_(QString::fromUtf8("When checked the isolation will be done with the current tool even if\ninteriors of a polygon (holes in the polygon) could not be isolated.\nWorks when 'rest machining' is used.")));
    forced_rest_iso_cb->setHidden(true);
    grid3->addWidget(forced_rest_iso_cb, 25, 1, 1, 1);
    combine_passes_cb = new FCCheckBox(nullptr);
    combine_passes_cb->setChecked(true);
    combine_passes_cb->setProperty("objectName", QString::fromUtf8("i_combine"));
    combine_passes_cb->setText(trUi_(QString::fromUtf8("Combine")));
    combine_passes_cb->setToolTip(trUi_(QString::fromUtf8("Combine all passes into one object")));
    grid3->addWidget(combine_passes_cb, 26, 0, 1, 2);
    valid_cb = new FCCheckBox(nullptr);
    valid_cb->setProperty("objectName", QString::fromUtf8("i_check"));
    valid_cb->setText(trUi_(QString::fromUtf8("Check validity")));
    valid_cb->setToolTip(trUi_(QString::fromUtf8("If checked then the tools diameters are verified\nif they will provide a complete isolation.")));
    grid3->addWidget(valid_cb, 28, 0, 1, 2);
    except_cb = new FCCheckBox(nullptr);
    except_cb->setProperty("objectName", QString::fromUtf8("i_except"));
    except_cb->setText(trUi_(QString::fromUtf8("Except")));
    except_cb->setToolTip(trUi_(QString::fromUtf8("When the isolation geometry is generated,\nby checking this, the area of the object below\nwill be subtracted from the isolation geometry.")));
    except_cb->setHidden(true);
    grid3->addWidget(except_cb, 30, 0, 1, 1);
    type_excobj_radio = new RadioSet({{trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geometry"))}, {trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("gerber"))}}, Qt::Horizontal, true, nullptr);
    type_excobj_radio->setEnabled(false);
    type_excobj_radio->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be excepted from isolation.\nIt can be of type: Gerber or Geometry.\nWhat is selected here will dictate the kind\nof objects that will populate the 'Object' combobox.")));
    type_excobj_radio->setHidden(true);
    type_excobj_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    grid3->addWidget(type_excobj_radio, 30, 1, 1, 1);
    exc_obj_combo = new FCComboBox(nullptr);
    exc_obj_combo->setEnabled(false);
    exc_obj_combo->setToolTip(trUi_(QString::fromUtf8("Object whose area will be removed from isolation geometry.")));
    exc_obj_combo->setHidden(true);
    grid3->addWidget(exc_obj_combo, 32, 0, 1, 2);
    select_label = new FCLabel(nullptr);
    select_label->setText(trUi_(QString::fromUtf8("Selection:")));
    select_label->setToolTip(trUi_(QString::fromUtf8("Isolation scope. Choose what to isolate:\n- 'All' -> Isolate all the polygons in the object\n- 'Area Selection' -> Isolate polygons within a selection area.\n- 'Polygon Selection' -> Isolate a selection of polygons.\n- 'Reference Object' - will process the area specified by another object.")));
    select_label->setHidden(true);
    grid3->addWidget(select_label, 34, 0, 1, 1);
    select_combo = new FCComboBox2(nullptr);
    select_combo->setProperty("currentText", trUi_(QString::fromUtf8("All")));
    select_combo->setProperty("objectName", QString::fromUtf8("i_selection"));
    select_combo->setHidden(true);
    select_combo->addItem(trUi_(QString::fromUtf8("All")));
    select_combo->addItem(trUi_(QString::fromUtf8("Area Selection")));
    select_combo->addItem(trUi_(QString::fromUtf8("Polygon Selection")));
    select_combo->addItem(trUi_(QString::fromUtf8("Reference Object")));
    grid3->addWidget(select_combo, 34, 1, 1, 1);
    reference_combo_type_label = new FCLabel(nullptr);
    reference_combo_type_label->setText(trUi_(QString::fromUtf8("Type:")));
    reference_combo_type_label->setHidden(true);
    grid3->addWidget(reference_combo_type_label, 36, 0, 1, 1);
    reference_combo_type = new FCComboBox2(nullptr);
    reference_combo_type->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    reference_combo_type->setHidden(true);
    reference_combo_type->addItem(trUi_(QString::fromUtf8("Gerber")));
    reference_combo_type->addItem(trUi_(QString::fromUtf8("Excellon")));
    reference_combo_type->addItem(trUi_(QString::fromUtf8("Geometry")));
    grid3->addWidget(reference_combo_type, 36, 1, 1, 1);
    reference_combo = new FCComboBox(nullptr);
    reference_combo->setHidden(true);
    grid3->addWidget(reference_combo, 38, 0, 1, 2);
    poly_int_cb = new FCCheckBox(nullptr);
    poly_int_cb->setText(trUi_(QString::fromUtf8("Interiors")));
    poly_int_cb->setToolTip(trUi_(QString::fromUtf8("When checked the user can select interiors of a polygon.\n(holes in the polygon).")));
    poly_int_cb->setHidden(true);
    grid3->addWidget(poly_int_cb, 40, 0, 1, 1);
    area_shape_label = new FCLabel(nullptr);
    area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    area_shape_label->setHidden(true);
    grid3->addWidget(area_shape_label, 42, 0, 1, 1);
    area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setHidden(true);
    area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    grid3->addWidget(area_shape_radio, 42, 1, 1, 1);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    grid3->addWidget(w13, 44, 0, 1, 2);
    grid3->setColumnStretch(1, 1);
    tools_box->addLayout(grid3, 0);
    generate_iso_button = new FCButton(nullptr);
    { QFont f = generate_iso_button->font(); f.setBold(true); generate_iso_button->setFont(f); }
    generate_iso_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_iso_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    generate_iso_button->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    generate_iso_button->setToolTip(trUi_(QString::fromUtf8("Create a Geometry object with toolpaths to cut \nisolation outside, inside or on both sides of the\nobject. For a Gerber object outside means outside\nof the Gerber feature and inside means inside of\nthe Gerber feature, if possible at all. This means\nthat only if the Gerber feature has openings inside, they\nwill be isolated. If what is wanted is to cut isolation\ninside the actual Gerber feature, use a negative tool\ndiameter above.")));
    tools_box->addWidget(generate_iso_button, 0);
    create_buffer_button = new FCButton(nullptr);
    create_buffer_button->setText(trUi_(QString::fromUtf8("Buffer Solid Geometry")));
    create_buffer_button->setToolTip(trUi_(QString::fromUtf8("This button is shown only when the Gerber file\nis loaded without buffering.\nClicking this will create the buffered geometry\nrequired for isolation.")));
    create_buffer_button->setHidden(true);
    tools_box->addWidget(create_buffer_button, 0);
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

std::map<std::string, QObject*> IsolationToolUIBase::membersByName() const {
    return {
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_label", area_shape_label},
        {"area_shape_radio", area_shape_radio},
        {"combine_passes_cb", combine_passes_cb},
        {"create_buffer_button", create_buffer_button},
        {"deltool_btn", deltool_btn},
        {"exc_obj_combo", exc_obj_combo},
        {"except_cb", except_cb},
        {"find_optimal_button", find_optimal_button},
        {"follow_cb", follow_cb},
        {"follow_label", follow_label},
        {"forced_rest_iso_cb", forced_rest_iso_cb},
        {"gen_param_label", gen_param_label},
        {"generate_iso_button", generate_iso_button},
        {"grid3", grid3},
        {"iso_overlap_entry", iso_overlap_entry},
        {"iso_type_label", iso_type_label},
        {"iso_type_radio", iso_type_radio},
        {"layout", layout},
        {"level", level},
        {"milling_type_label", milling_type_label},
        {"milling_type_radio", milling_type_radio},
        {"new_tooldia_entry", new_tooldia_entry},
        {"new_tooldia_lbl", new_tooldia_lbl},
        {"obj_combo_label", obj_combo_label},
        {"object_combo", object_combo},
        {"order_label", order_label},
        {"order_radio", order_radio},
        {"passes_entry", passes_entry},
        {"poly_int_cb", poly_int_cb},
        {"reference_combo", reference_combo},
        {"reference_combo_type", reference_combo_type},
        {"reference_combo_type_label", reference_combo_type_label},
        {"reset_button", reset_button},
        {"rest_cb", rest_cb},
        {"search_and_add_btn", search_and_add_btn},
        {"select_combo", select_combo},
        {"select_label", select_label},
        {"title_box", title_box},
        {"tool_data_label", tool_data_label},
        {"tool_sel_label", tool_sel_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"type_excobj_radio", type_excobj_radio},
        {"valid_cb", valid_cb},
    };
}

}  // namespace flatcam::gui
