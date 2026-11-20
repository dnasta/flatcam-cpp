// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PaintToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void PaintToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Paint Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Paint Tool")));
    layout->addWidget(w1, 0);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    type_obj_combo_label = new FCLabel(nullptr);
    type_obj_combo_label->setMinimumSize(QSize(60, 0));
    type_obj_combo_label->setText(trUi_(QString::fromUtf8("Obj Type:")));
    type_obj_combo_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be painted.\nIt can be of type: Gerber or Geometry.\nWhat is selected here will dictate the kind\nof objects that will populate the 'Object' combobox.")));
    lay2->addWidget(type_obj_combo_label, 1, 0, 1, 1);
    type_obj_radio = new RadioSet({{trUi_(QString::fromUtf8("Geometry")), QVariant(QString::fromUtf8("geometry"))}, {trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("gerber"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("geometry")));
    lay2->addWidget(type_obj_radio, 1, 1, 1, 1);
    object_label = new FCLabel(nullptr);
    object_label->setText(trUi_(QString::fromUtf8("Object:")));
    object_label->setToolTip(trUi_(QString::fromUtf8("Object to be painted.")));
    lay2->addWidget(object_label, 2, 0, 1, 1);
    obj_combo = new FCComboBox(nullptr);
    lay2->addWidget(obj_combo, 2, 1, 1, 1);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->addWidget(w3, 5, 0, 1, 2);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(trUi_(QString::fromUtf8("<b>Tools Table</b>")));
    tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools pool from which the algorithm\nwill pick the ones used for painting.")));
    lay2->addWidget(tools_table_label, 6, 0, 1, 2);
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
    lay2->addWidget(tools_table, 7, 0, 1, 2);
    order_label = new FCLabel(nullptr);
    order_label->setText(trUi_(QString::fromUtf8("<b>Tool order:</b>")));
    order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay2->addWidget(order_label, 9, 0, 1, 1);
    order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    order_radio->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay2->addWidget(order_radio, 9, 1, 1, 1);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay2->addWidget(w4, 10, 0, 1, 2);
    lay2->setColumnStretch(1, 1);
    tools_box->addLayout(lay2, 0);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(trUi_(QString::fromUtf8("<b>Add from DB</b>")));
    grid3->addWidget(tool_sel_label, 0, 0, 1, 2);
    new_tooldia_lbl = new FCLabel(nullptr);
    new_tooldia_lbl->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    new_tooldia_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool to add in the Tool Table.\nIf the tool is V-shape type then this value is automatically\ncalculated from the other parameters.")));
    grid3->addWidget(new_tooldia_lbl, 2, 0, 1, 1);
    new_tooldia_entry = new FCDoubleSpinner(nullptr);
    new_tooldia_entry->setProperty("decimals", 4);
    new_tooldia_entry->setProperty("maximum", 10000.0);
    new_tooldia_entry->setProperty("objectName", QString::fromUtf8("p_tool_dia"));
    new_tooldia_entry->setProperty("value", 0.1);
    grid3->addWidget(new_tooldia_entry, 2, 1, 1, 1);
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
    grid3->addLayout(lay5, 7, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    grid3->addWidget(w6, 8, 0, 1, 2);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    deltool_btn->setText(trUi_(QString::fromUtf8("Delete")));
    deltool_btn->setToolTip(trUi_(QString::fromUtf8("Delete a selection of tools in the Tool Table\nby first selecting a row in the Tool Table.")));
    grid3->addWidget(deltool_btn, 9, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    grid3->addWidget(w7, 11, 0, 1, 2);
    tool_data_label = new FCLabel(nullptr);
    tool_data_label->setText(trUi_(QString::fromUtf8("<b>Parameters for: <font color='#0000FF'>Tool 1</font></b>")));
    tool_data_label->setToolTip(trUi_(QString::fromUtf8("The data used for creating GCode.\nEach tool store it's own set of such data.")));
    grid3->addWidget(tool_data_label, 12, 0, 1, 2);
    grid3->setColumnStretch(1, 1);
    tools_box->addLayout(grid3, 0);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("Overlap:")));
    w9->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.\nAdjust the value starting with lower values\nand increasing it if areas that should be processed are still \nnot processed.\nLower values = faster processing, faster execution on CNC.\nHigher values = slow processing and slow execution on CNC\ndue of too many paths.")));
    lay8->addWidget(w9, 1, 0, 1, 1);
    paintoverlap_entry = new FCDoubleSpinner(nullptr);
    paintoverlap_entry->setProperty("decimals", 3);
    paintoverlap_entry->setProperty("maximum", 100.0);
    paintoverlap_entry->setProperty("objectName", QString::fromUtf8("p_overlap"));
    paintoverlap_entry->setProperty("singleStep", 0.1);
    paintoverlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    paintoverlap_entry->setProperty("value", 20.0);
    paintoverlap_entry->setWrapping(true);
    lay8->addWidget(paintoverlap_entry, 1, 1, 1, 1);
    offset_label = new FCLabel(nullptr);
    offset_label->setText(trUi_(QString::fromUtf8("Offset:")));
    offset_label->setToolTip(trUi_(QString::fromUtf8("Distance by which to avoid\nthe edges of the polygon to\nbe painted.")));
    lay8->addWidget(offset_label, 2, 0, 1, 1);
    offset_entry = new FCDoubleSpinner(nullptr);
    offset_entry->setProperty("decimals", 4);
    offset_entry->setProperty("maximum", 10000.0);
    offset_entry->setProperty("minimum", -10000.0);
    offset_entry->setProperty("objectName", QString::fromUtf8("p_offset"));
    lay8->addWidget(offset_entry, 2, 1, 1, 1);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Method:")));
    w10->setToolTip(trUi_(QString::fromUtf8("Algorithm for painting:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.\n- Laser-lines: Active only for Gerber objects.\nWill create lines that follow the traces.\n- Combo: In case of failure a new method will be picked from the above\nin the order specified.")));
    lay8->addWidget(w10, 7, 0, 1, 1);
    paintmethod_combo = new FCComboBox2(nullptr);
    paintmethod_combo->setProperty("currentText", trUi_(QString::fromUtf8("Standard")));
    paintmethod_combo->setProperty("objectName", QString::fromUtf8("p_method"));
    paintmethod_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    paintmethod_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    paintmethod_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    paintmethod_combo->addItem(trUi_(QString::fromUtf8("Laser_lines")));
    paintmethod_combo->addItem(trUi_(QString::fromUtf8("Combo")));
    lay8->addWidget(paintmethod_combo, 7, 1, 1, 1);
    pathconnect_cb = new FCCheckBox(nullptr);
    pathconnect_cb->setChecked(true);
    pathconnect_cb->setProperty("objectName", QString::fromUtf8("p_connect"));
    pathconnect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    pathconnect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    lay8->addWidget(pathconnect_cb, 10, 0, 1, 1);
    paintcontour_cb = new FCCheckBox(nullptr);
    paintcontour_cb->setChecked(true);
    paintcontour_cb->setProperty("objectName", QString::fromUtf8("p_contour"));
    paintcontour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    paintcontour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    lay8->addWidget(paintcontour_cb, 10, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    lay8->addWidget(w11, 11, 0, 1, 2);
    apply_param_to_all = new FCButton(nullptr);
    apply_param_to_all->setIcon(QIcon(QString::fromUtf8(":/images/param_all32.png")));
    apply_param_to_all->setText(trUi_(QString::fromUtf8("Apply parameters to all tools")));
    apply_param_to_all->setToolTip(trUi_(QString::fromUtf8("The parameters in the current form will be applied\non all the tools from the Tool Table.")));
    lay8->addWidget(apply_param_to_all, 12, 0, 1, 2);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    lay8->addWidget(w12, 13, 0, 1, 2);
    gen_param_label = new FCLabel(nullptr);
    gen_param_label->setText(trUi_(QString::fromUtf8("<b>Common Parameters</b>")));
    gen_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters that are common for all tools.")));
    lay8->addWidget(gen_param_label, 15, 0, 1, 2);
    rest_cb = new FCCheckBox(nullptr);
    rest_cb->setProperty("objectName", QString::fromUtf8("p_rest_machining"));
    rest_cb->setText(trUi_(QString::fromUtf8("Rest")));
    rest_cb->setToolTip(trUi_(QString::fromUtf8("If checked, use 'rest machining'.\nBasically it will process copper outside PCB features,\nusing the biggest tool and continue with the next tools,\nfrom bigger to smaller, to process the copper features that\ncould not be processed by previous tool, until there is\nnothing left to process or there are no more tools.\n\nIf not checked, use the standard algorithm.")));
    lay8->addWidget(rest_cb, 16, 0, 1, 2);
    rest_offset_label = new FCLabel(nullptr);
    rest_offset_label->setText(trUi_(QString::fromUtf8("Offset:")));
    rest_offset_label->setToolTip(trUi_(QString::fromUtf8("Distance by which to avoid\nthe edges of the polygon to\nbe painted.")));
    rest_offset_label->setHidden(true);
    lay8->addWidget(rest_offset_label, 17, 0, 1, 1);
    rest_offset_entry = new FCDoubleSpinner(nullptr);
    rest_offset_entry->setProperty("decimals", 4);
    rest_offset_entry->setProperty("maximum", 10000.0);
    rest_offset_entry->setProperty("minimum", -10000.0);
    rest_offset_entry->setHidden(true);
    lay8->addWidget(rest_offset_entry, 17, 1, 1, 1);
    auto* w13 = new FCLabel(nullptr);
    w13->setText(trUi_(QString::fromUtf8("Selection:")));
    w13->setToolTip(trUi_(QString::fromUtf8("Selection of area to be processed.\n- 'Polygon Selection' - left mouse click to add/remove polygons to be processed.\n- 'Area Selection' - left mouse click to start selection of the area to be processed.\nKeeping a modifier key pressed (CTRL or SHIFT) will allow to add multiple areas.\n- 'All Polygons' - the process will start after click.\n- 'Reference Object' - will process the area specified by another object.")));
    lay8->addWidget(w13, 18, 0, 1, 1);
    selectmethod_combo = new FCComboBox2(nullptr);
    selectmethod_combo->setProperty("currentText", trUi_(QString::fromUtf8("All")));
    selectmethod_combo->setProperty("objectName", QString::fromUtf8("p_selection"));
    selectmethod_combo->addItem(trUi_(QString::fromUtf8("All")));
    selectmethod_combo->addItem(trUi_(QString::fromUtf8("Polygon Selection")));
    selectmethod_combo->addItem(trUi_(QString::fromUtf8("Area Selection")));
    selectmethod_combo->addItem(trUi_(QString::fromUtf8("Reference Object")));
    lay8->addWidget(selectmethod_combo, 18, 1, 1, 1);
    reference_type_label = new FCLabel(nullptr);
    reference_type_label->setText(trUi_(QString::fromUtf8("Type:")));
    reference_type_label->setToolTip(trUi_(QString::fromUtf8("The type of FlatCAM object to be used as paint reference.\nIt can be Gerber, Excellon or Geometry.")));
    reference_type_label->setHidden(true);
    lay8->addWidget(reference_type_label, 20, 0, 1, 1);
    reference_type_combo = new FCComboBox2(nullptr);
    reference_type_combo->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    reference_type_combo->setHidden(true);
    reference_type_combo->addItem(trUi_(QString::fromUtf8("Gerber")));
    reference_type_combo->addItem(trUi_(QString::fromUtf8("Excellon")));
    reference_type_combo->addItem(trUi_(QString::fromUtf8("Geometry")));
    lay8->addWidget(reference_type_combo, 20, 1, 1, 1);
    reference_combo = new FCComboBox(nullptr);
    reference_combo->setHidden(true);
    lay8->addWidget(reference_combo, 22, 0, 1, 2);
    area_shape_label = new FCLabel(nullptr);
    area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    area_shape_label->setHidden(true);
    lay8->addWidget(area_shape_label, 24, 0, 1, 1);
    area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setHidden(true);
    area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay8->addWidget(area_shape_radio, 24, 1, 1, 1);
    lay8->setColumnStretch(1, 1);
    tools_box->addLayout(lay8, 0);
    generate_paint_button = new FCButton(nullptr);
    { QFont f = generate_paint_button->font(); f.setBold(true); generate_paint_button->setFont(f); }
    generate_paint_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_paint_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t\t QPushButton\n\t\t\t\t\t\t\t\t\t\t\t\t {\n\t\t\t\t\t\t\t\t\t\t\t\t\t font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t\t }\n\t\t\t\t\t\t\t\t\t\t\t\t "));
    generate_paint_button->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    generate_paint_button->setToolTip(trUi_(QString::fromUtf8("Create a Geometry Object which paints the polygons.")));
    tools_box->addWidget(generate_paint_button, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t\t\t"));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    tools_box->addWidget(reset_button, 0);
    tools_frame->setLayout(tools_box);
    layout->addWidget(tools_frame, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> PaintToolUIBase::membersByName() const {
    return {
        {"add_newtool_button", add_newtool_button},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_label", area_shape_label},
        {"area_shape_radio", area_shape_radio},
        {"deltool_btn", deltool_btn},
        {"gen_param_label", gen_param_label},
        {"generate_paint_button", generate_paint_button},
        {"grid3", grid3},
        {"layout", layout},
        {"new_tooldia_entry", new_tooldia_entry},
        {"new_tooldia_lbl", new_tooldia_lbl},
        {"obj_combo", obj_combo},
        {"object_label", object_label},
        {"offset_entry", offset_entry},
        {"offset_label", offset_label},
        {"order_label", order_label},
        {"order_radio", order_radio},
        {"paintcontour_cb", paintcontour_cb},
        {"paintmethod_combo", paintmethod_combo},
        {"paintoverlap_entry", paintoverlap_entry},
        {"pathconnect_cb", pathconnect_cb},
        {"reference_combo", reference_combo},
        {"reference_type_combo", reference_type_combo},
        {"reference_type_label", reference_type_label},
        {"reset_button", reset_button},
        {"rest_cb", rest_cb},
        {"rest_offset_entry", rest_offset_entry},
        {"rest_offset_label", rest_offset_label},
        {"selectmethod_combo", selectmethod_combo},
        {"tool_data_label", tool_data_label},
        {"tool_sel_label", tool_sel_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"type_obj_combo_label", type_obj_combo_label},
        {"type_obj_radio", type_obj_radio},
    };
}

}  // namespace flatcam::gui
