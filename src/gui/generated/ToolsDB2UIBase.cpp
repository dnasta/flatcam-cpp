// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ToolsDB2UIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void ToolsDB2UIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    g_lay = new QGridLayout();
    g_lay->setContentsMargins(9, 9, 9, 9);
    g_lay->setHorizontalSpacing(6);
    g_lay->setVerticalSpacing(6);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(0, 0, 0, 0);
    lay1->setSpacing(6);
    tree_widget = new FCTree(nullptr);
    tree_widget->setProperty("contextMenuPolicy", 3);
    tree_widget->setHeaderHidden(false);
    tree_widget->setProperty("indentation", 0);
    tree_widget->setProperty("selectionMode", 3);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(3)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); tree_widget->setSizePolicy(sp); }
    lay1->addWidget(tree_widget, 0);
    auto* lay2 = new QHBoxLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setSpacing(6);
    add_tool_from_db = new FCButton(nullptr);
    { QFont f = add_tool_from_db->font(); f.setBold(true); add_tool_from_db->setFont(f); }
    add_tool_from_db->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t\tcolor: green;\n\t\t\t\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t\t\t\t"));
    add_tool_from_db->setText(trUi_(QString::fromUtf8("Transfer the Tool")));
    add_tool_from_db->setToolTip(trUi_(QString::fromUtf8("Insert a new tool in the Tools Table of the\nobject/application tool after selecting a tool\nin the Tools Database.")));
    add_tool_from_db->setHidden(true);
    lay2->addWidget(add_tool_from_db, 0);
    cancel_tool_from_db = new FCButton(nullptr);
    cancel_tool_from_db->setText(trUi_(QString::fromUtf8("Cancel")));
    cancel_tool_from_db->setHidden(true);
    lay2->addWidget(cancel_tool_from_db, 0);
    lay1->addLayout(lay2, 0);
    g_lay->addLayout(lay1, 0, 0, 1, 1);
    auto* w3 = new QScrollArea(nullptr);
    w3->setWidgetResizable(true);
    auto* w4 = new QWidget(nullptr);
    w4->setAutoFillBackground(true);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(9, 9, 9, 9);
    lay5->setSpacing(6);
    auto* lay6 = new QVBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    tool_description_box = new QGroupBox(nullptr);
    { QFont f = tool_description_box->font(); f.setBold(true); f.setPixelSize(16); tool_description_box->setFont(f); }
    tool_description_box->setMinimumSize(QSize(250, 0));
    tool_description_box->setStyleSheet(QString::fromUtf8("\n\t\tQGroupBox\n\t\t{\n\t\t\tfont-size: 16px;\n\t\t\tfont-weight: bold;\n\t\t}\n\t\t"));
    tool_description_box->setTitle(trUi_(QString::fromUtf8("Tool Description")));
    description_vlay = new QVBoxLayout();
    description_vlay->setContentsMargins(9, 9, 9, 9);
    description_vlay->setSpacing(6);
    grid_tool = new QGridLayout();
    grid_tool->setContentsMargins(0, 0, 0, 0);
    grid_tool->setHorizontalSpacing(6);
    grid_tool->setVerticalSpacing(6);
    name_label = new FCLabel(nullptr);
    name_label->setText(trUi_(QString::fromUtf8("<span style=\"color:red;\"><b>Name:</b></span>")));
    name_label->setToolTip(trUi_(QString::fromUtf8("Tool name.\nThis is not used in the app, it's function\nis to serve as a note for the user.")));
    grid_tool->addWidget(name_label, 0, 0, 1, 1);
    name_entry = new FCEntry(nullptr);
    name_entry->setProperty("objectName", QString::fromUtf8("gdb_name"));
    grid_tool->addWidget(name_entry, 0, 1, 1, 1);
    dia_label = new FCLabel(nullptr);
    dia_label->setText(trUi_(QString::fromUtf8("Diameter:")));
    dia_label->setToolTip(trUi_(QString::fromUtf8("Tool Diameter.")));
    grid_tool->addWidget(dia_label, 1, 0, 1, 1);
    dia_entry = new FCDoubleSpinner(nullptr);
    dia_entry->setProperty("decimals", 4);
    dia_entry->setProperty("maximum", 10000.0);
    dia_entry->setProperty("minimum", -10000.0);
    dia_entry->setProperty("objectName", QString::fromUtf8("gdb_dia"));
    grid_tool->addWidget(dia_entry, 1, 1, 1, 1);
    tol_label = new FCLabel(nullptr);
    tol_label->setText(trUi_(QString::fromUtf8("<b>Diameter Tolerance:</b>")));
    tol_label->setToolTip(trUi_(QString::fromUtf8("Tool tolerance. This tool will be used if the desired tool diameter\nis within the tolerance specified here.")));
    grid_tool->addWidget(tol_label, 2, 0, 1, 2);
    min_limit_label = new FCLabel(nullptr);
    min_limit_label->setText(trUi_(QString::fromUtf8("Min:")));
    min_limit_label->setToolTip(trUi_(QString::fromUtf8("Set the tool tolerance minimum.")));
    grid_tool->addWidget(min_limit_label, 4, 0, 1, 1);
    tol_min_entry = new FCDoubleSpinner(nullptr);
    tol_min_entry->setProperty("decimals", 4);
    tol_min_entry->setProperty("maximum", 10000.0);
    tol_min_entry->setProperty("objectName", QString::fromUtf8("gdb_tol_min"));
    tol_min_entry->setProperty("singleStep", 0.1);
    grid_tool->addWidget(tol_min_entry, 4, 1, 1, 1);
    max_limit_label = new FCLabel(nullptr);
    max_limit_label->setText(trUi_(QString::fromUtf8("Max:")));
    max_limit_label->setToolTip(trUi_(QString::fromUtf8("Set the tool tolerance maximum.")));
    grid_tool->addWidget(max_limit_label, 6, 0, 1, 1);
    tol_max_entry = new FCDoubleSpinner(nullptr);
    tol_max_entry->setProperty("decimals", 4);
    tol_max_entry->setProperty("maximum", 10000.0);
    tol_max_entry->setProperty("objectName", QString::fromUtf8("gdb_tol_max"));
    tol_max_entry->setProperty("singleStep", 0.1);
    grid_tool->addWidget(tol_max_entry, 6, 1, 1, 1);
    tool_op_label = new FCLabel(nullptr);
    tool_op_label->setText(trUi_(QString::fromUtf8("<b>Operation:</b>")));
    tool_op_label->setToolTip(trUi_(QString::fromUtf8("The kind of Application Tool where this tool is to be used.")));
    grid_tool->addWidget(tool_op_label, 8, 0, 1, 1);
    tool_op_combo = new FCComboBox2(nullptr);
    tool_op_combo->setProperty("currentText", trUi_(QString::fromUtf8("General")));
    tool_op_combo->setProperty("objectName", QString::fromUtf8("gdb_tool_target"));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("General")));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("Milling")));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("Drilling")));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("Isolation")));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("Paint")));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("NCC")));
    tool_op_combo->addItem(trUi_(QString::fromUtf8("Cutout")));
    grid_tool->addWidget(tool_op_combo, 8, 1, 1, 1);
    grid_tool->setColumnStretch(1, 1);
    description_vlay->addLayout(grid_tool, 0);
    description_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tool_description_box->setLayout(description_vlay);
    lay6->addWidget(tool_description_box, 0);
    auto* lay7 = new QVBoxLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setSpacing(6);
    iso_box = new QGroupBox(nullptr);
    { QFont f = iso_box->font(); f.setBold(true); f.setPixelSize(16); iso_box->setFont(f); }
    iso_box->setMinimumSize(QSize(250, 0));
    iso_box->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t QGroupBox\n\t\t\t\t\t {\n\t\t\t\t\t\t font-size: 16px;\n\t\t\t\t\t\t font-weight: bold;\n\t\t\t\t\t }\n\t\t\t\t\t "));
    iso_box->setTitle(trUi_(QString::fromUtf8("Isolation Parameters")));
    iso_vlay = new QVBoxLayout();
    iso_vlay->setContentsMargins(9, 9, 9, 9);
    iso_vlay->setSpacing(6);
    grid4 = new QGridLayout();
    grid4->setContentsMargins(0, 0, 0, 0);
    grid4->setHorizontalSpacing(6);
    grid4->setVerticalSpacing(6);
    auto* w8 = new FCLabel(nullptr);
    w8->setText(trUi_(QString::fromUtf8("Passes:")));
    w8->setToolTip(trUi_(QString::fromUtf8("Width of the isolation gap in\nnumber (integer) of tool widths.")));
    grid4->addWidget(w8, 0, 0, 1, 1);
    iso_passes_entry = new FCSpinner(nullptr);
    iso_passes_entry->setProperty("maximum", 999);
    iso_passes_entry->setProperty("minimum", 1);
    iso_passes_entry->setProperty("objectName", QString::fromUtf8("gdb_i_passes"));
    iso_passes_entry->setProperty("value", 1);
    grid4->addWidget(iso_passes_entry, 0, 1, 1, 1);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("Overlap:")));
    w9->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.")));
    grid4->addWidget(w9, 2, 0, 1, 1);
    iso_overlap_entry = new FCDoubleSpinner(nullptr);
    iso_overlap_entry->setProperty("decimals", 4);
    iso_overlap_entry->setProperty("maximum", 99.9999);
    iso_overlap_entry->setProperty("objectName", QString::fromUtf8("gdb_i_overlap"));
    iso_overlap_entry->setProperty("singleStep", 0.1);
    iso_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    iso_overlap_entry->setWrapping(true);
    grid4->addWidget(iso_overlap_entry, 2, 1, 1, 1);
    iso_milling_type_label = new FCLabel(nullptr);
    iso_milling_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    iso_milling_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    grid4->addWidget(iso_milling_type_label, 4, 0, 1, 1);
    iso_milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    iso_milling_type_radio->setProperty("objectName", QString::fromUtf8("gdb_i_milling_type"));
    iso_milling_type_radio->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    grid4->addWidget(iso_milling_type_radio, 4, 1, 1, 1);
    follow_label = new FCLabel(nullptr);
    follow_label->setText(trUi_(QString::fromUtf8("Follow:")));
    follow_label->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    grid4->addWidget(follow_label, 6, 0, 1, 1);
    iso_follow_cb = new FCCheckBox(nullptr);
    iso_follow_cb->setProperty("objectName", QString::fromUtf8("gdb_i_follow"));
    iso_follow_cb->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    grid4->addWidget(iso_follow_cb, 6, 1, 1, 1);
    iso_type_label = new FCLabel(nullptr);
    iso_type_label->setText(trUi_(QString::fromUtf8("Isolation Type:")));
    iso_type_label->setToolTip(trUi_(QString::fromUtf8("Choose how the isolation will be executed:\n- 'Full' -> complete isolation of polygons\n- 'Ext' -> will isolate only on the outside\n- 'Int' -> will isolate only on the inside\n'Exterior' isolation is almost always possible\n(with the right tool) but 'Interior'\nisolation can be done only when there is an opening\ninside of the polygon (e.g polygon is a 'doughnut' shape).")));
    grid4->addWidget(iso_type_label, 8, 0, 1, 1);
    iso_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Full")), QVariant(QString::fromUtf8("full"))}, {trUi_(QString::fromUtf8("Ext")), QVariant(QString::fromUtf8("ext"))}, {trUi_(QString::fromUtf8("Int")), QVariant(QString::fromUtf8("int"))}}, Qt::Horizontal, true, nullptr);
    iso_type_radio->setProperty("objectName", QString::fromUtf8("gdb_i_iso_type"));
    grid4->addWidget(iso_type_radio, 8, 1, 1, 1);
    grid4->setColumnStretch(1, 1);
    iso_vlay->addLayout(grid4, 0);
    iso_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    iso_box->setLayout(iso_vlay);
    lay7->addWidget(iso_box, 0);
    paint_box = new QGroupBox(nullptr);
    { QFont f = paint_box->font(); f.setBold(true); f.setPixelSize(16); paint_box->setFont(f); }
    paint_box->setMinimumSize(QSize(250, 0));
    paint_box->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQGroupBox\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    paint_box->setTitle(trUi_(QString::fromUtf8("Paint Parameters")));
    paint_vlay = new QVBoxLayout();
    paint_vlay->setContentsMargins(9, 9, 9, 9);
    paint_vlay->setSpacing(6);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Overlap:")));
    w10->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.\nAdjust the value starting with lower values\nand increasing it if areas that should be processed are still \nnot processed.\nLower values = faster processing, faster execution on CNC.\nHigher values = slow processing and slow execution on CNC\ndue of too many paths.")));
    grid3->addWidget(w10, 1, 0, 1, 1);
    paint_overlap_entry = new FCDoubleSpinner(nullptr);
    paint_overlap_entry->setProperty("decimals", 3);
    paint_overlap_entry->setProperty("maximum", 100.0);
    paint_overlap_entry->setProperty("objectName", QString::fromUtf8("gdb_p_overlap"));
    paint_overlap_entry->setProperty("singleStep", 0.1);
    paint_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    paint_overlap_entry->setWrapping(true);
    grid3->addWidget(paint_overlap_entry, 1, 1, 1, 1);
    auto* w11 = new FCLabel(nullptr);
    w11->setText(trUi_(QString::fromUtf8("Offset:")));
    w11->setToolTip(trUi_(QString::fromUtf8("Distance by which to avoid\nthe edges of the polygon to\nbe painted.")));
    grid3->addWidget(w11, 2, 0, 1, 1);
    paint_offset_entry = new FCDoubleSpinner(nullptr);
    paint_offset_entry->setProperty("decimals", 4);
    paint_offset_entry->setProperty("maximum", 10000.0);
    paint_offset_entry->setProperty("minimum", -10000.0);
    paint_offset_entry->setProperty("objectName", QString::fromUtf8("gdb_p_offset"));
    grid3->addWidget(paint_offset_entry, 2, 1, 1, 1);
    auto* w12 = new FCLabel(nullptr);
    w12->setText(trUi_(QString::fromUtf8("Method:")));
    w12->setToolTip(trUi_(QString::fromUtf8("Algorithm for painting:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.\n- Laser-lines: Active only for Gerber objects.\nWill create lines that follow the traces.\n- Combo: In case of failure a new method will be picked from the above\nin the order specified.")));
    grid3->addWidget(w12, 7, 0, 1, 1);
    paint_method_combo = new FCComboBox2(nullptr);
    paint_method_combo->setProperty("currentText", trUi_(QString::fromUtf8("Standard")));
    paint_method_combo->setProperty("objectName", QString::fromUtf8("gdb_p_method"));
    paint_method_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    paint_method_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    paint_method_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    paint_method_combo->addItem(trUi_(QString::fromUtf8("Laser_lines")));
    paint_method_combo->addItem(trUi_(QString::fromUtf8("Combo")));
    grid3->addWidget(paint_method_combo, 7, 1, 1, 1);
    path_connect_cb = new FCCheckBox(nullptr);
    path_connect_cb->setProperty("objectName", QString::fromUtf8("gdb_p_connect"));
    path_connect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    path_connect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    grid3->addWidget(path_connect_cb, 10, 0, 1, 1);
    paint_contour_cb = new FCCheckBox(nullptr);
    paint_contour_cb->setProperty("objectName", QString::fromUtf8("gdb_p_contour"));
    paint_contour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    paint_contour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    grid3->addWidget(paint_contour_cb, 10, 1, 1, 1);
    grid3->setColumnStretch(1, 1);
    paint_vlay->addLayout(grid3, 0);
    paint_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    paint_box->setLayout(paint_vlay);
    lay7->addWidget(paint_box, 0);
    ncc_box = new QGroupBox(nullptr);
    { QFont f = ncc_box->font(); f.setBold(true); f.setPixelSize(16); ncc_box->setFont(f); }
    ncc_box->setMinimumSize(QSize(250, 0));
    ncc_box->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQGroupBox\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    ncc_box->setTitle(trUi_(QString::fromUtf8("NCC Parameters")));
    ncc_vlay = new QVBoxLayout();
    ncc_vlay->setContentsMargins(9, 9, 9, 9);
    ncc_vlay->setSpacing(6);
    grid2 = new QGridLayout();
    grid2->setContentsMargins(0, 0, 0, 0);
    grid2->setHorizontalSpacing(6);
    grid2->setVerticalSpacing(6);
    auto* w13 = new FCLabel(nullptr);
    w13->setText(trUi_(QString::fromUtf8("Operation:")));
    w13->setToolTip(trUi_(QString::fromUtf8("The 'Operation' can be:\n- Isolation -> will ensure that the non-copper clearing is always complete.\nIf it's not successful then the non-copper clearing will fail, too.\n- Clear -> the regular non-copper clearing.")));
    grid2->addWidget(w13, 13, 0, 1, 1);
    ncc_op_radio = new RadioSet({{trUi_(QString::fromUtf8("Clear")), QVariant(QString::fromUtf8("clear"))}, {trUi_(QString::fromUtf8("Isolation")), QVariant(QString::fromUtf8("iso"))}}, Qt::Horizontal, false, nullptr);
    ncc_op_radio->setProperty("objectName", QString::fromUtf8("gdb_n_operation"));
    grid2->addWidget(ncc_op_radio, 13, 1, 1, 1);
    milling_type_label = new FCLabel(nullptr);
    milling_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    milling_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    grid2->addWidget(milling_type_label, 14, 0, 1, 1);
    ncc_milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    ncc_milling_type_radio->setProperty("objectName", QString::fromUtf8("gdb_n_milling_type"));
    ncc_milling_type_radio->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    grid2->addWidget(ncc_milling_type_radio, 14, 1, 1, 1);
    auto* w14 = new FCLabel(nullptr);
    w14->setText(trUi_(QString::fromUtf8("Overlap:")));
    w14->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.\nAdjust the value starting with lower values\nand increasing it if areas that should be processed are still \nnot processed.\nLower values = faster processing, faster execution on CNC.\nHigher values = slow processing and slow execution on CNC\ndue of too many paths.")));
    grid2->addWidget(w14, 15, 0, 1, 1);
    ncc_overlap_entry = new FCDoubleSpinner(nullptr);
    ncc_overlap_entry->setProperty("decimals", 4);
    ncc_overlap_entry->setProperty("maximum", 99.9999);
    ncc_overlap_entry->setProperty("objectName", QString::fromUtf8("gdb_n_overlap"));
    ncc_overlap_entry->setProperty("singleStep", 0.1);
    ncc_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    ncc_overlap_entry->setWrapping(true);
    grid2->addWidget(ncc_overlap_entry, 15, 1, 1, 1);
    auto* w15 = new FCLabel(nullptr);
    w15->setText(trUi_(QString::fromUtf8("Margin:")));
    w15->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    grid2->addWidget(w15, 16, 0, 1, 1);
    ncc_margin_entry = new FCDoubleSpinner(nullptr);
    ncc_margin_entry->setProperty("decimals", 4);
    ncc_margin_entry->setProperty("maximum", 10000.0);
    ncc_margin_entry->setProperty("minimum", -10000.0);
    ncc_margin_entry->setProperty("objectName", QString::fromUtf8("gdb_n_margin"));
    grid2->addWidget(ncc_margin_entry, 16, 1, 1, 1);
    auto* w16 = new FCLabel(nullptr);
    w16->setText(trUi_(QString::fromUtf8("Method:")));
    w16->setToolTip(trUi_(QString::fromUtf8("Algorithm for copper clearing:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.")));
    grid2->addWidget(w16, 17, 0, 1, 1);
    ncc_method_combo = new FCComboBox2(nullptr);
    ncc_method_combo->setProperty("currentText", trUi_(QString::fromUtf8("Standard")));
    ncc_method_combo->setProperty("objectName", QString::fromUtf8("gdb_n_method"));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    ncc_method_combo->addItem(trUi_(QString::fromUtf8("Combo")));
    grid2->addWidget(ncc_method_combo, 17, 1, 1, 1);
    ncc_connect_cb = new FCCheckBox(nullptr);
    ncc_connect_cb->setProperty("objectName", QString::fromUtf8("gdb_n_connect"));
    ncc_connect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    ncc_connect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    grid2->addWidget(ncc_connect_cb, 18, 0, 1, 1);
    ncc_contour_cb = new FCCheckBox(nullptr);
    ncc_contour_cb->setProperty("objectName", QString::fromUtf8("gdb_n_contour"));
    ncc_contour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    ncc_contour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    grid2->addWidget(ncc_contour_cb, 18, 1, 1, 1);
    ncc_choice_offset_cb = new FCCheckBox(nullptr);
    ncc_choice_offset_cb->setProperty("objectName", QString::fromUtf8("gdb_n_offset"));
    ncc_choice_offset_cb->setText(trUi_(QString::fromUtf8("Offset")));
    ncc_choice_offset_cb->setToolTip(trUi_(QString::fromUtf8("If used, it will add an offset to the copper features.\nThe copper clearing will finish to a distance\nfrom the copper features.")));
    grid2->addWidget(ncc_choice_offset_cb, 19, 0, 1, 1);
    ncc_offset_spinner = new FCDoubleSpinner(nullptr);
    ncc_offset_spinner->setProperty("decimals", 4);
    ncc_offset_spinner->setProperty("maximum", 10.0);
    ncc_offset_spinner->setProperty("objectName", QString::fromUtf8("gdb_n_offset_value"));
    ncc_offset_spinner->setProperty("singleStep", 0.1);
    ncc_offset_spinner->setWrapping(true);
    grid2->addWidget(ncc_offset_spinner, 19, 1, 1, 1);
    grid2->setColumnStretch(1, 1);
    ncc_vlay->addLayout(grid2, 0);
    ncc_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    ncc_box->setLayout(ncc_vlay);
    lay7->addWidget(ncc_box, 0);
    cutout_box = new QGroupBox(nullptr);
    { QFont f = cutout_box->font(); f.setBold(true); f.setPixelSize(16); cutout_box->setFont(f); }
    cutout_box->setMinimumSize(QSize(250, 0));
    cutout_box->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t QGroupBox\n\t\t\t\t\t {\n\t\t\t\t\t\t font-size: 16px;\n\t\t\t\t\t\t font-weight: bold;\n\t\t\t\t\t }\n\t\t\t\t\t "));
    cutout_box->setTitle(trUi_(QString::fromUtf8("Cutout Parameters")));
    cutout_vlay = new QVBoxLayout();
    cutout_vlay->setContentsMargins(9, 9, 9, 9);
    cutout_vlay->setSpacing(6);
    grid6 = new QGridLayout();
    grid6->setContentsMargins(0, 0, 0, 0);
    grid6->setHorizontalSpacing(6);
    grid6->setVerticalSpacing(6);
    cutout_margin_label = new FCLabel(nullptr);
    cutout_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    cutout_margin_label->setToolTip(trUi_(QString::fromUtf8("Margin over bounds. A positive value here\nwill make the cutout of the PCB further from\nthe actual PCB border")));
    grid6->addWidget(cutout_margin_label, 11, 0, 1, 1);
    cutout_margin_entry = new FCDoubleSpinner(nullptr);
    cutout_margin_entry->setProperty("decimals", 4);
    cutout_margin_entry->setProperty("maximum", 10000.0);
    cutout_margin_entry->setProperty("minimum", -10000.0);
    cutout_margin_entry->setProperty("objectName", QString::fromUtf8("gdb_ct_margin"));
    cutout_margin_entry->setProperty("singleStep", 0.1);
    grid6->addWidget(cutout_margin_entry, 11, 1, 1, 1);
    cutout_gapsize_label = new FCLabel(nullptr);
    cutout_gapsize_label->setText(trUi_(QString::fromUtf8("Gap size:")));
    cutout_gapsize_label->setToolTip(trUi_(QString::fromUtf8("The size of the bridge gaps in the cutout\nused to keep the board connected to\nthe surrounding material (the one \nfrom which the PCB is cutout).")));
    grid6->addWidget(cutout_gapsize_label, 13, 0, 1, 1);
    cutout_gapsize = new FCDoubleSpinner(nullptr);
    cutout_gapsize->setProperty("decimals", 4);
    cutout_gapsize->setProperty("objectName", QString::fromUtf8("gdb_ct_gapsize"));
    grid6->addWidget(cutout_gapsize, 13, 1, 1, 1);
    gaptype_label = new FCLabel(nullptr);
    gaptype_label->setText(trUi_(QString::fromUtf8("Gap type:")));
    gaptype_label->setToolTip(trUi_(QString::fromUtf8("The type of gap:\n- Bridge -> the cutout will be interrupted by bridges\n- Thin -> same as 'bridge' but it will be thinner by partially milling the gap\n- M-Bites -> 'Mouse Bites' - same as 'bridge' but covered with drill holes")));
    grid6->addWidget(gaptype_label, 15, 0, 1, 1);
    cutout_gaptype_radio = new RadioSet({{trUi_(QString::fromUtf8("Bridge")), QVariant(QString::fromUtf8("b"))}, {trUi_(QString::fromUtf8("Thin")), QVariant(QString::fromUtf8("bt"))}, {trUi_(QString::fromUtf8("M-Bites")), QVariant(QString::fromUtf8("mb"))}}, Qt::Horizontal, true, nullptr);
    cutout_gaptype_radio->setProperty("objectName", QString::fromUtf8("gdb_ct_gap_type"));
    grid6->addWidget(cutout_gaptype_radio, 15, 1, 1, 1);
    thin_depth_label = new FCLabel(nullptr);
    thin_depth_label->setText(trUi_(QString::fromUtf8("Depth:")));
    thin_depth_label->setToolTip(trUi_(QString::fromUtf8("The depth until the milling is done\nin order to thin the gaps.")));
    grid6->addWidget(thin_depth_label, 17, 0, 1, 1);
    cutout_thin_depth_entry = new FCDoubleSpinner(nullptr);
    cutout_thin_depth_entry->setProperty("decimals", 4);
    cutout_thin_depth_entry->setProperty("maximum", -0.0);
    cutout_thin_depth_entry->setProperty("minimum", -10000.0);
    cutout_thin_depth_entry->setProperty("objectName", QString::fromUtf8("gdb_ct_gap_depth"));
    cutout_thin_depth_entry->setProperty("singleStep", 0.1);
    grid6->addWidget(cutout_thin_depth_entry, 17, 1, 1, 1);
    mb_dia_label = new FCLabel(nullptr);
    mb_dia_label->setText(trUi_(QString::fromUtf8("Tool Diameter:")));
    mb_dia_label->setToolTip(trUi_(QString::fromUtf8("The drill hole diameter when doing mouse bites.")));
    grid6->addWidget(mb_dia_label, 19, 0, 1, 1);
    cutout_mb_dia_entry = new FCDoubleSpinner(nullptr);
    cutout_mb_dia_entry->setProperty("decimals", 4);
    cutout_mb_dia_entry->setProperty("maximum", 100.0);
    cutout_mb_dia_entry->setProperty("objectName", QString::fromUtf8("gdb_ct_mb_dia"));
    grid6->addWidget(cutout_mb_dia_entry, 19, 1, 1, 1);
    mb_spacing_label = new FCLabel(nullptr);
    mb_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    mb_spacing_label->setToolTip(trUi_(QString::fromUtf8("The spacing between drill holes when doing mouse bites.")));
    grid6->addWidget(mb_spacing_label, 21, 0, 1, 1);
    cutout_mb_spacing_entry = new FCDoubleSpinner(nullptr);
    cutout_mb_spacing_entry->setProperty("decimals", 4);
    cutout_mb_spacing_entry->setProperty("maximum", 100.0);
    cutout_mb_spacing_entry->setProperty("objectName", QString::fromUtf8("gdb_ct_mb_spacing"));
    grid6->addWidget(cutout_mb_spacing_entry, 21, 1, 1, 1);
    cutout_convex_box = new FCCheckBox(nullptr);
    cutout_convex_box->setProperty("objectName", QString::fromUtf8("gdb_ct_convex"));
    cutout_convex_box->setText(trUi_(QString::fromUtf8("Convex Shape")));
    cutout_convex_box->setToolTip(trUi_(QString::fromUtf8("Create a convex shape surrounding the entire PCB.\nUsed only if the source object type is Gerber.")));
    grid6->addWidget(cutout_convex_box, 23, 0, 1, 2);
    cutout_gaps_label = new FCLabel(nullptr);
    cutout_gaps_label->setText(trUi_(QString::fromUtf8("Gaps:")));
    cutout_gaps_label->setToolTip(trUi_(QString::fromUtf8("Number of gaps used for the Automatic cutout.\nThere can be maximum 8 bridges/gaps.\nThe choices are:\n- None  - no gaps\n- lr    - left + right\n- tb    - top + bottom\n- 4     - left + right +top + bottom\n- 2lr   - 2*left + 2*right\n- 2tb  - 2*top + 2*bottom\n- 8     - 2*left + 2*right +2*top + 2*bottom")));
    grid6->addWidget(cutout_gaps_label, 25, 0, 1, 1);
    cutout_gaps = new FCComboBox(nullptr);
    cutout_gaps->setProperty("currentText", trUi_(QString::fromUtf8("None")));
    cutout_gaps->setProperty("objectName", QString::fromUtf8("gdb_ct_gaps"));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("None")));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("LR")));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("TB")));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("4")));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("2LR")));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("2TB")));
    cutout_gaps->addItem(trUi_(QString::fromUtf8("8")));
    grid6->addWidget(cutout_gaps, 25, 1, 1, 1);
    grid6->setColumnStretch(1, 1);
    cutout_vlay->addLayout(grid6, 0);
    cutout_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    cutout_box->setLayout(cutout_vlay);
    lay7->addWidget(cutout_box, 0);
    lay7->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay6->addLayout(lay7, 0);
    lay6->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay5->addLayout(lay6, 0);
    auto* lay17 = new QVBoxLayout();
    lay17->setContentsMargins(0, 0, 0, 0);
    lay17->setSpacing(6);
    drill_box = new QGroupBox(nullptr);
    { QFont f = drill_box->font(); f.setBold(true); f.setPixelSize(16); drill_box->setFont(f); }
    drill_box->setMinimumSize(QSize(250, 0));
    drill_box->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t QGroupBox\n\t\t\t\t\t {\n\t\t\t\t\t\t font-size: 16px;\n\t\t\t\t\t\t font-weight: bold;\n\t\t\t\t\t }\n\t\t\t\t\t "));
    drill_box->setTitle(trUi_(QString::fromUtf8("Drilling Parameters")));
    drill_vlay = new QVBoxLayout();
    drill_vlay->setContentsMargins(9, 9, 9, 9);
    drill_vlay->setSpacing(6);
    grid5 = new QGridLayout();
    grid5->setContentsMargins(0, 0, 0, 0);
    grid5->setHorizontalSpacing(6);
    grid5->setVerticalSpacing(6);
    cutzlabel = new FCLabel(nullptr);
    cutzlabel->setText(trUi_(QString::fromUtf8("Cut Z:")));
    cutzlabel->setToolTip(trUi_(QString::fromUtf8("Drill depth (negative)\nbelow the copper surface.")));
    grid5->addWidget(cutzlabel, 4, 0, 1, 1);
    drill_cutz_entry = new FCDoubleSpinner(nullptr);
    drill_cutz_entry->setProperty("decimals", 4);
    drill_cutz_entry->setProperty("maximum", 0.0);
    drill_cutz_entry->setProperty("minimum", -10000.0);
    drill_cutz_entry->setProperty("objectName", QString::fromUtf8("gdb_e_cutz"));
    drill_cutz_entry->setProperty("singleStep", 0.1);
    grid5->addWidget(drill_cutz_entry, 4, 1, 1, 1);
    tool_offset_label = new FCLabel(nullptr);
    tool_offset_label->setText(trUi_(QString::fromUtf8("Offset Z:")));
    tool_offset_label->setToolTip(trUi_(QString::fromUtf8("Some drill bits (the larger ones) need to drill deeper\nto create the desired exit hole diameter due of the tip shape.\nThe value here can compensate the Cut Z parameter.")));
    grid5->addWidget(tool_offset_label, 6, 0, 1, 1);
    drill_offset_entry = new FCDoubleSpinner(nullptr);
    drill_offset_entry->setProperty("decimals", 4);
    drill_offset_entry->setProperty("maximum", 10000.0);
    drill_offset_entry->setProperty("minimum", -10000.0);
    drill_offset_entry->setProperty("objectName", QString::fromUtf8("gdb_e_offset"));
    grid5->addWidget(drill_offset_entry, 6, 1, 1, 1);
    multidepth_drill_label = new FCLabel(nullptr);
    multidepth_drill_label->setText(trUi_(QString::fromUtf8("MultiDepth:")));
    multidepth_drill_label->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    grid5->addWidget(multidepth_drill_label, 7, 0, 1, 1);
    drill_mpass_cb = new FCCheckBox(nullptr);
    drill_mpass_cb->setProperty("objectName", QString::fromUtf8("gdb_e_multidepth"));
    grid5->addWidget(drill_mpass_cb, 7, 1, 1, 1);
    dpp_drill_label = new FCLabel(nullptr);
    dpp_drill_label->setText(trUi_(QString::fromUtf8("DPP:")));
    dpp_drill_label->setToolTip(trUi_(QString::fromUtf8("DPP. Depth per Pass.\nThe value used to cut into material on each pass.")));
    grid5->addWidget(dpp_drill_label, 8, 0, 1, 1);
    drill_maxdepth_entry = new FCDoubleSpinner(nullptr);
    drill_maxdepth_entry->setProperty("decimals", 4);
    drill_maxdepth_entry->setProperty("maximum", 10000.0);
    drill_maxdepth_entry->setProperty("objectName", QString::fromUtf8("gdb_e_depthperpass"));
    drill_maxdepth_entry->setProperty("singleStep", 0.1);
    drill_maxdepth_entry->setToolTip(trUi_(QString::fromUtf8("Depth of each pass (positive).")));
    grid5->addWidget(drill_maxdepth_entry, 8, 1, 1, 1);
    travelzlabel = new FCLabel(nullptr);
    travelzlabel->setText(trUi_(QString::fromUtf8("Travel Z:")));
    travelzlabel->setToolTip(trUi_(QString::fromUtf8("Tool height when travelling\nacross the XY plane.")));
    grid5->addWidget(travelzlabel, 10, 0, 1, 1);
    drill_travelz_entry = new FCDoubleSpinner(nullptr);
    drill_travelz_entry->setProperty("decimals", 4);
    drill_travelz_entry->setProperty("maximum", 10000.0);
    drill_travelz_entry->setProperty("objectName", QString::fromUtf8("gdb_e_travelz"));
    drill_travelz_entry->setProperty("singleStep", 0.1);
    grid5->addWidget(drill_travelz_entry, 10, 1, 1, 1);
    auto* w18 = new QFrame(nullptr);
    w18->setProperty("frameShadow", 48);
    w18->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w18->setSizePolicy(sp); }
    grid5->addWidget(w18, 12, 0, 1, 2);
    frzlabel = new FCLabel(nullptr);
    frzlabel->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    frzlabel->setToolTip(trUi_(QString::fromUtf8("Tool speed while drilling\n(in units per minute).\nSo called 'Plunge' feedrate.\nThis is for linear move G01.")));
    grid5->addWidget(frzlabel, 14, 0, 1, 1);
    drill_feedrate_z_entry = new FCDoubleSpinner(nullptr);
    drill_feedrate_z_entry->setProperty("decimals", 4);
    drill_feedrate_z_entry->setProperty("maximum", 910000.0);
    drill_feedrate_z_entry->setProperty("objectName", QString::fromUtf8("gdb_e_feedratez"));
    drill_feedrate_z_entry->setProperty("singleStep", 0.1);
    grid5->addWidget(drill_feedrate_z_entry, 14, 1, 1, 1);
    feedrate_rapid_label = new FCLabel(nullptr);
    feedrate_rapid_label->setText(trUi_(QString::fromUtf8("Feedrate Rapids:")));
    feedrate_rapid_label->setToolTip(trUi_(QString::fromUtf8("Tool speed while drilling\n(in units per minute).\nThis is for the rapid move G00.\nIt is useful only for Marlin,\nignore for any other cases.")));
    grid5->addWidget(feedrate_rapid_label, 16, 0, 1, 1);
    drill_feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    drill_feedrate_rapid_entry->setProperty("decimals", 4);
    drill_feedrate_rapid_entry->setProperty("maximum", 910000.0);
    drill_feedrate_rapid_entry->setProperty("objectName", QString::fromUtf8("gdb_e_fr_rapid"));
    drill_feedrate_rapid_entry->setProperty("singleStep", 0.1);
    grid5->addWidget(drill_feedrate_rapid_entry, 16, 1, 1, 1);
    auto* w19 = new QFrame(nullptr);
    w19->setProperty("frameShadow", 48);
    w19->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w19->setSizePolicy(sp); }
    grid5->addWidget(w19, 18, 0, 1, 2);
    spindle_label = new FCLabel(nullptr);
    spindle_label->setText(trUi_(QString::fromUtf8("Spindle speed:")));
    spindle_label->setToolTip(trUi_(QString::fromUtf8("Speed of the spindle\nin RPM (optional)")));
    grid5->addWidget(spindle_label, 20, 0, 1, 1);
    drill_spindlespeed_entry = new FCSpinner(nullptr);
    drill_spindlespeed_entry->setProperty("maximum", 1000000);
    drill_spindlespeed_entry->setProperty("objectName", QString::fromUtf8("gdb_e_spindlespeed"));
    drill_spindlespeed_entry->setProperty("singleStep", 100);
    grid5->addWidget(drill_spindlespeed_entry, 20, 1, 1, 1);
    dwell_drill_label = new FCLabel(nullptr);
    dwell_drill_label->setText(trUi_(QString::fromUtf8("Dwell:")));
    dwell_drill_label->setToolTip(trUi_(QString::fromUtf8("Dwell.\nCheck this if a delay is needed to allow\nthe spindle motor to reach its set speed.")));
    grid5->addWidget(dwell_drill_label, 21, 0, 1, 1);
    drill_dwell_cb = new FCCheckBox(nullptr);
    drill_dwell_cb->setProperty("objectName", QString::fromUtf8("gdb_e_dwell"));
    grid5->addWidget(drill_dwell_cb, 21, 1, 1, 1);
    dwelltime_drill_lbl = new FCLabel(nullptr);
    dwelltime_drill_lbl->setText(trUi_(QString::fromUtf8("Dwelltime:")));
    dwelltime_drill_lbl->setToolTip(trUi_(QString::fromUtf8("Dwell Time.\nA delay used to allow the motor spindle reach its set speed.")));
    grid5->addWidget(dwelltime_drill_lbl, 22, 0, 1, 1);
    drill_dwelltime_entry = new FCDoubleSpinner(nullptr);
    drill_dwelltime_entry->setProperty("decimals", 4);
    drill_dwelltime_entry->setProperty("maximum", 10000.0);
    drill_dwelltime_entry->setProperty("objectName", QString::fromUtf8("gdb_e_dwelltime"));
    drill_dwelltime_entry->setProperty("singleStep", 0.1);
    grid5->addWidget(drill_dwelltime_entry, 22, 1, 1, 1);
    auto* w20 = new QFrame(nullptr);
    w20->setProperty("frameShadow", 48);
    w20->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w20->setSizePolicy(sp); }
    grid5->addWidget(w20, 24, 0, 1, 2);
    drill_slots_drill_lbl = new FCLabel(nullptr);
    drill_slots_drill_lbl->setText(trUi_(QString::fromUtf8("Drill slots:")));
    drill_slots_drill_lbl->setToolTip(trUi_(QString::fromUtf8("If the selected tool has slots then they will be drilled.")));
    grid5->addWidget(drill_slots_drill_lbl, 26, 0, 1, 1);
    drill_slots_cb = new FCCheckBox(nullptr);
    drill_slots_cb->setProperty("objectName", QString::fromUtf8("gdb_e_drill_slots"));
    grid5->addWidget(drill_slots_cb, 26, 1, 1, 1);
    drill_overlap_label = new FCLabel(nullptr);
    drill_overlap_label->setText(trUi_(QString::fromUtf8("Overlap:")));
    drill_overlap_label->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool diameter to overlap previous drill hole.")));
    grid5->addWidget(drill_overlap_label, 28, 0, 1, 1);
    drill_overlap_entry = new FCDoubleSpinner(nullptr);
    drill_overlap_entry->setProperty("decimals", 4);
    drill_overlap_entry->setProperty("maximum", 100.0);
    drill_overlap_entry->setProperty("objectName", QString::fromUtf8("gdb_e_drill_slots_over"));
    drill_overlap_entry->setProperty("singleStep", 0.1);
    drill_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    grid5->addWidget(drill_overlap_entry, 28, 1, 1, 1);
    last_drill_drill_lbl = new FCLabel(nullptr);
    last_drill_drill_lbl->setText(trUi_(QString::fromUtf8("Last drill:")));
    last_drill_drill_lbl->setToolTip(trUi_(QString::fromUtf8("If the slot length is not completely covered by drill holes,\nadd a drill hole on the slot end point.")));
    grid5->addWidget(last_drill_drill_lbl, 30, 0, 1, 2);
    drill_last_drill_cb = new FCCheckBox(nullptr);
    drill_last_drill_cb->setProperty("objectName", QString::fromUtf8("gdb_e_drill_last_drill"));
    grid5->addWidget(drill_last_drill_cb, 30, 1, 1, 1);
    grid5->setColumnStretch(1, 1);
    drill_vlay->addLayout(grid5, 0);
    drill_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    drill_box->setLayout(drill_vlay);
    lay17->addWidget(drill_box, 0);
    lay5->addLayout(lay17, 0);
    auto* lay21 = new QVBoxLayout();
    lay21->setContentsMargins(0, 0, 0, 0);
    lay21->setSpacing(6);
    milling_box = new QGroupBox(nullptr);
    { QFont f = milling_box->font(); f.setBold(true); f.setPixelSize(16); milling_box->setFont(f); }
    milling_box->setMinimumSize(QSize(250, 0));
    milling_box->setStyleSheet(QString::fromUtf8("\n\t\tQGroupBox\n\t\t{\n\t\t\tfont-size: 16px;\n\t\t\tfont-weight: bold;\n\t\t}\n\t\t"));
    milling_box->setTitle(trUi_(QString::fromUtf8("Milling Parameters")));
    milling_vlay = new QVBoxLayout();
    milling_vlay->setContentsMargins(9, 9, 9, 9);
    milling_vlay->setSpacing(6);
    grid0 = new QGridLayout();
    grid0->setContentsMargins(0, 0, 0, 0);
    grid0->setHorizontalSpacing(6);
    grid0->setVerticalSpacing(6);
    shape_label = new FCLabel(nullptr);
    shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    shape_label->setToolTip(trUi_(QString::fromUtf8("Tool Shape. \nCan be:\nC1 ... C4 = circular tool with x flutes\nB = ball tip milling tool\nV = v-shape milling tool")));
    grid0->addWidget(shape_label, 2, 0, 1, 1);
    mill_shape_combo = new FCComboBox(nullptr);
    mill_shape_combo->setProperty("currentText", trUi_(QString::fromUtf8("C1")));
    mill_shape_combo->setProperty("objectName", QString::fromUtf8("gdb_shape"));
    mill_shape_combo->addItem(trUi_(QString::fromUtf8("C1")));
    mill_shape_combo->addItem(trUi_(QString::fromUtf8("C2")));
    mill_shape_combo->addItem(trUi_(QString::fromUtf8("C3")));
    mill_shape_combo->addItem(trUi_(QString::fromUtf8("C4")));
    mill_shape_combo->addItem(trUi_(QString::fromUtf8("B")));
    mill_shape_combo->addItem(trUi_(QString::fromUtf8("V")));
    grid0->addWidget(mill_shape_combo, 2, 1, 1, 1);
    vdia_label = new FCLabel(nullptr);
    vdia_label->setText(trUi_(QString::fromUtf8("V-Dia:")));
    vdia_label->setToolTip(trUi_(QString::fromUtf8("V-Dia.\nDiameter of the tip for V-Shape Tools.")));
    grid0->addWidget(vdia_label, 4, 0, 1, 1);
    mill_vdia_entry = new FCDoubleSpinner(nullptr);
    mill_vdia_entry->setProperty("decimals", 4);
    mill_vdia_entry->setProperty("maximum", 10000.0);
    mill_vdia_entry->setProperty("objectName", QString::fromUtf8("gdb_vdia"));
    grid0->addWidget(mill_vdia_entry, 4, 1, 1, 1);
    vangle_label = new FCLabel(nullptr);
    vangle_label->setText(trUi_(QString::fromUtf8("V-Angle:")));
    vangle_label->setToolTip(trUi_(QString::fromUtf8("V-Agle.\nAngle at the tip for the V-Shape Tools.")));
    grid0->addWidget(vangle_label, 6, 0, 1, 1);
    mill_vangle_entry = new FCDoubleSpinner(nullptr);
    mill_vangle_entry->setProperty("decimals", 4);
    mill_vangle_entry->setProperty("maximum", 360.0);
    mill_vangle_entry->setProperty("minimum", -360.0);
    mill_vangle_entry->setProperty("objectName", QString::fromUtf8("gdb_vangle"));
    grid0->addWidget(mill_vangle_entry, 6, 1, 1, 1);
    auto* w22 = new QFrame(nullptr);
    w22->setProperty("frameShadow", 48);
    w22->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w22->setSizePolicy(sp); }
    grid0->addWidget(w22, 8, 0, 1, 2);
    type_label = new FCLabel(nullptr);
    type_label->setText(trUi_(QString::fromUtf8("Tool Type:")));
    type_label->setToolTip(trUi_(QString::fromUtf8("Tool Type.\nCan be:\nIso = isolation cut\nRough = rough cut, low feedrate, multiple passes\nFinish = finishing cut, high feedrate")));
    grid0->addWidget(type_label, 10, 0, 1, 1);
    mill_type_combo = new FCComboBox(nullptr);
    mill_type_combo->setProperty("currentText", trUi_(QString::fromUtf8("Iso")));
    mill_type_combo->setProperty("objectName", QString::fromUtf8("gdb_type"));
    mill_type_combo->addItem(trUi_(QString::fromUtf8("Iso")));
    mill_type_combo->addItem(trUi_(QString::fromUtf8("Rough")));
    mill_type_combo->addItem(trUi_(QString::fromUtf8("Finish")));
    grid0->addWidget(mill_type_combo, 10, 1, 1, 1);
    tooloffset_label = new FCLabel(nullptr);
    tooloffset_label->setText(trUi_(QString::fromUtf8("Tool Offset:")));
    tooloffset_label->setToolTip(trUi_(QString::fromUtf8("Tool Offset.\nCan be of a few types:\nPath = zero offset\nIn = offset inside by half of tool diameter\nOut = offset outside by half of tool diameter\nCustom = custom offset using the Custom Offset value")));
    grid0->addWidget(tooloffset_label, 12, 0, 1, 1);
    mill_tooloffset_combo = new FCComboBox(nullptr);
    mill_tooloffset_combo->setProperty("currentText", trUi_(QString::fromUtf8("Path")));
    mill_tooloffset_combo->setProperty("objectName", QString::fromUtf8("gdb_tool_offset"));
    mill_tooloffset_combo->addItem(trUi_(QString::fromUtf8("Path")));
    mill_tooloffset_combo->addItem(trUi_(QString::fromUtf8("In")));
    mill_tooloffset_combo->addItem(trUi_(QString::fromUtf8("Out")));
    mill_tooloffset_combo->addItem(trUi_(QString::fromUtf8("Custom")));
    grid0->addWidget(mill_tooloffset_combo, 12, 1, 1, 1);
    custom_offset_label = new FCLabel(nullptr);
    custom_offset_label->setText(trUi_(QString::fromUtf8("Custom Offset:")));
    custom_offset_label->setToolTip(trUi_(QString::fromUtf8("Custom Offset.\nA value to be used as offset from the current path.")));
    grid0->addWidget(custom_offset_label, 14, 0, 1, 1);
    mill_custom_offset_entry = new FCDoubleSpinner(nullptr);
    mill_custom_offset_entry->setProperty("decimals", 4);
    mill_custom_offset_entry->setProperty("maximum", 10000.0);
    mill_custom_offset_entry->setProperty("minimum", -10000.0);
    mill_custom_offset_entry->setProperty("objectName", QString::fromUtf8("gdb_custom_offset"));
    grid0->addWidget(mill_custom_offset_entry, 14, 1, 1, 1);
    auto* w23 = new QFrame(nullptr);
    w23->setProperty("frameShadow", 48);
    w23->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w23->setSizePolicy(sp); }
    grid0->addWidget(w23, 16, 0, 1, 2);
    cutz_label = new FCLabel(nullptr);
    cutz_label->setText(trUi_(QString::fromUtf8("Cut Z:")));
    cutz_label->setToolTip(trUi_(QString::fromUtf8("Cutting Depth.\nThe depth at which to cut into material.")));
    grid0->addWidget(cutz_label, 18, 0, 1, 1);
    mill_cutz_entry = new FCDoubleSpinner(nullptr);
    mill_cutz_entry->setProperty("decimals", 4);
    mill_cutz_entry->setProperty("maximum", 10000.0);
    mill_cutz_entry->setProperty("minimum", -10000.0);
    mill_cutz_entry->setProperty("objectName", QString::fromUtf8("gdb_cutz"));
    grid0->addWidget(mill_cutz_entry, 18, 1, 1, 1);
    multidepth_label = new FCLabel(nullptr);
    multidepth_label->setText(trUi_(QString::fromUtf8("MultiDepth:")));
    multidepth_label->setToolTip(trUi_(QString::fromUtf8("Multi Depth.\nSelecting this will allow cutting in multiple passes,\neach pass adding a DPP parameter depth.")));
    grid0->addWidget(multidepth_label, 20, 0, 1, 1);
    mill_multidepth_cb = new FCCheckBox(nullptr);
    mill_multidepth_cb->setProperty("objectName", QString::fromUtf8("gdb_multidepth"));
    grid0->addWidget(mill_multidepth_cb, 20, 1, 1, 1);
    dpp_label = new FCLabel(nullptr);
    dpp_label->setText(trUi_(QString::fromUtf8("DPP:")));
    dpp_label->setToolTip(trUi_(QString::fromUtf8("DPP. Depth per Pass.\nThe value used to cut into material on each pass.")));
    grid0->addWidget(dpp_label, 22, 0, 1, 1);
    mill_multidepth_entry = new FCDoubleSpinner(nullptr);
    mill_multidepth_entry->setProperty("decimals", 4);
    mill_multidepth_entry->setProperty("maximum", 10000.0);
    mill_multidepth_entry->setProperty("minimum", -10000.0);
    mill_multidepth_entry->setProperty("objectName", QString::fromUtf8("gdb_multidepth_entry"));
    grid0->addWidget(mill_multidepth_entry, 22, 1, 1, 1);
    travelz_label = new FCLabel(nullptr);
    travelz_label->setText(trUi_(QString::fromUtf8("Travel Z:")));
    travelz_label->setToolTip(trUi_(QString::fromUtf8("Clearance Height.\nHeight at which the milling bit will travel between cuts,\nabove the surface of the material, avoiding all fixtures.")));
    grid0->addWidget(travelz_label, 24, 0, 1, 1);
    mill_travelz_entry = new FCDoubleSpinner(nullptr);
    mill_travelz_entry->setProperty("decimals", 4);
    mill_travelz_entry->setProperty("maximum", 10000.0);
    mill_travelz_entry->setProperty("minimum", -10000.0);
    mill_travelz_entry->setProperty("objectName", QString::fromUtf8("gdb_travelz"));
    grid0->addWidget(mill_travelz_entry, 24, 1, 1, 1);
    ecut_label = new FCLabel(nullptr);
    ecut_label->setText(trUi_(QString::fromUtf8("ExtraCut:")));
    ecut_label->setToolTip(trUi_(QString::fromUtf8("Extra Cut.\nIf checked, after a isolation is finished an extra cut\nwill be added where the start and end of isolation meet\nsuch as that this point is covered by this extra cut to\nensure a complete isolation.")));
    grid0->addWidget(ecut_label, 26, 0, 1, 1);
    mill_ecut_cb = new FCCheckBox(nullptr);
    mill_ecut_cb->setProperty("objectName", QString::fromUtf8("gdb_ecut"));
    grid0->addWidget(mill_ecut_cb, 26, 1, 1, 1);
    ecut_length_label = new FCLabel(nullptr);
    ecut_length_label->setText(trUi_(QString::fromUtf8("E-Cut Length:")));
    ecut_length_label->setToolTip(trUi_(QString::fromUtf8("Extra Cut length.\nIf checked, after a isolation is finished an extra cut\nwill be added where the start and end of isolation meet\nsuch as that this point is covered by this extra cut to\nensure a complete isolation. This is the length of\nthe extra cut.")));
    grid0->addWidget(ecut_length_label, 28, 0, 1, 1);
    mill_ecut_length_entry = new FCDoubleSpinner(nullptr);
    mill_ecut_length_entry->setProperty("decimals", 4);
    mill_ecut_length_entry->setProperty("maximum", 10000.0);
    mill_ecut_length_entry->setProperty("objectName", QString::fromUtf8("gdb_ecut_length"));
    grid0->addWidget(mill_ecut_length_entry, 28, 1, 1, 1);
    auto* w24 = new QFrame(nullptr);
    w24->setProperty("frameShadow", 48);
    w24->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w24->setSizePolicy(sp); }
    grid0->addWidget(w24, 30, 0, 1, 2);
    frxy_label = new FCLabel(nullptr);
    frxy_label->setText(trUi_(QString::fromUtf8("Feedrate X-Y:")));
    frxy_label->setToolTip(trUi_(QString::fromUtf8("Feedrate X-Y. Feedrate\nThe speed on XY plane used while cutting into material.")));
    grid0->addWidget(frxy_label, 32, 0, 1, 1);
    mill_frxy_entry = new FCDoubleSpinner(nullptr);
    mill_frxy_entry->setProperty("decimals", 4);
    mill_frxy_entry->setProperty("maximum", 9910000.0);
    mill_frxy_entry->setProperty("minimum", -9910000.0);
    mill_frxy_entry->setProperty("objectName", QString::fromUtf8("gdb_frxy"));
    grid0->addWidget(mill_frxy_entry, 32, 1, 1, 1);
    frz_label = new FCLabel(nullptr);
    frz_label->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    frz_label->setToolTip(trUi_(QString::fromUtf8("Feedrate Z\nThe speed on Z plane.")));
    grid0->addWidget(frz_label, 34, 0, 1, 1);
    mill_frz_entry = new FCDoubleSpinner(nullptr);
    mill_frz_entry->setProperty("decimals", 4);
    mill_frz_entry->setProperty("maximum", 9910000.0);
    mill_frz_entry->setProperty("minimum", -9910000.0);
    mill_frz_entry->setProperty("objectName", QString::fromUtf8("gdb_frz"));
    grid0->addWidget(mill_frz_entry, 34, 1, 1, 1);
    frapids_label = new FCLabel(nullptr);
    frapids_label->setText(trUi_(QString::fromUtf8("FR Rapids:")));
    frapids_label->setToolTip(trUi_(QString::fromUtf8("FR Rapids. Feedrate Rapids\nSpeed used while moving as fast as possible.\nThis is used only by some devices that can't use\nthe G0 g-code command. Mostly 3D printers.")));
    grid0->addWidget(frapids_label, 36, 0, 1, 1);
    mill_frapids_entry = new FCDoubleSpinner(nullptr);
    mill_frapids_entry->setProperty("decimals", 4);
    mill_frapids_entry->setProperty("maximum", 10000.0);
    mill_frapids_entry->setProperty("objectName", QString::fromUtf8("gdb_frapids"));
    grid0->addWidget(mill_frapids_entry, 36, 1, 1, 1);
    auto* w25 = new QFrame(nullptr);
    w25->setProperty("frameShadow", 48);
    w25->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w25->setSizePolicy(sp); }
    grid0->addWidget(w25, 38, 0, 1, 2);
    auto* w26 = new FCLabel(nullptr);
    w26->setText(trUi_(QString::fromUtf8("Spindle Speed:")));
    w26->setToolTip(trUi_(QString::fromUtf8("Spindle Speed.\nIf it's left empty it will not be used.\nThe speed of the spindle in RPM.")));
    grid0->addWidget(w26, 40, 0, 1, 1);
    mill_spindle_entry = new FCDoubleSpinner(nullptr);
    mill_spindle_entry->setProperty("decimals", 4);
    mill_spindle_entry->setProperty("maximum", 9910000.0);
    mill_spindle_entry->setProperty("minimum", -9910000.0);
    mill_spindle_entry->setProperty("objectName", QString::fromUtf8("gdb_spindle"));
    grid0->addWidget(mill_spindle_entry, 40, 1, 1, 1);
    dwell_label = new FCLabel(nullptr);
    dwell_label->setText(trUi_(QString::fromUtf8("Dwell:")));
    dwell_label->setToolTip(trUi_(QString::fromUtf8("Dwell.\nCheck this if a delay is needed to allow\nthe spindle motor to reach its set speed.")));
    grid0->addWidget(dwell_label, 42, 0, 1, 1);
    mill_dwell_cb = new FCCheckBox(nullptr);
    mill_dwell_cb->setProperty("objectName", QString::fromUtf8("gdb_dwell"));
    grid0->addWidget(mill_dwell_cb, 42, 1, 1, 1);
    dwelltime_label = new FCLabel(nullptr);
    dwelltime_label->setText(trUi_(QString::fromUtf8("Dwelltime:")));
    dwelltime_label->setToolTip(trUi_(QString::fromUtf8("Dwell Time.\nA delay used to allow the motor spindle reach its set speed.")));
    grid0->addWidget(dwelltime_label, 44, 0, 1, 1);
    mill_dwelltime_entry = new FCDoubleSpinner(nullptr);
    mill_dwelltime_entry->setProperty("decimals", 4);
    mill_dwelltime_entry->setProperty("maximum", 10000.0);
    mill_dwelltime_entry->setProperty("objectName", QString::fromUtf8("gdb_dwelltime"));
    grid0->addWidget(mill_dwelltime_entry, 44, 1, 1, 1);
    grid0->setColumnStretch(1, 1);
    milling_vlay->addLayout(grid0, 0);
    milling_vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    milling_box->setLayout(milling_vlay);
    lay21->addWidget(milling_box, 0);
    lay21->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay5->addLayout(lay21, 0);
    lay5->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    w4->setLayout(lay5);
    w3->setWidget(w4);
    g_lay->addWidget(w3, 0, 1, 1, 1);
    auto* lay27 = new QVBoxLayout();
    lay27->setContentsMargins(0, 0, 0, 0);
    lay27->setSpacing(6);
    buttons_frame = new QFrame(nullptr);
    buttons_box = new QHBoxLayout();
    buttons_box->setContentsMargins(0, 0, 0, 0);
    buttons_box->setSpacing(6);
    add_entry_btn = new FCButton(nullptr);
    add_entry_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_entry_btn->setText(trUi_(QString::fromUtf8("Add Tool in DB")));
    add_entry_btn->setToolTip(trUi_(QString::fromUtf8("Add a new tool in the Tools Database.\nIt will be used in the Geometry UI.\nYou can edit it after it is added.")));
    buttons_box->addWidget(add_entry_btn, 0);
    remove_entry_btn = new FCButton(nullptr);
    remove_entry_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    remove_entry_btn->setText(trUi_(QString::fromUtf8("Delete Tool from DB")));
    remove_entry_btn->setToolTip(trUi_(QString::fromUtf8("Remove a selection of tools in the Tools Database.")));
    buttons_box->addWidget(remove_entry_btn, 0);
    export_db_btn = new FCButton(nullptr);
    export_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/export.png")));
    export_db_btn->setText(trUi_(QString::fromUtf8("Export DB")));
    export_db_btn->setToolTip(trUi_(QString::fromUtf8("Save the Tools Database to a custom text file.")));
    buttons_box->addWidget(export_db_btn, 0);
    import_db_btn = new FCButton(nullptr);
    import_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/import.png")));
    import_db_btn->setText(trUi_(QString::fromUtf8("Import DB")));
    import_db_btn->setToolTip(trUi_(QString::fromUtf8("Load the Tools Database information's from a custom text file.")));
    buttons_box->addWidget(import_db_btn, 0);
    save_db_btn = new FCButton(nullptr);
    save_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/save_as.png")));
    save_db_btn->setText(trUi_(QString::fromUtf8("Save DB")));
    save_db_btn->setToolTip(trUi_(QString::fromUtf8("Save the Tools Database information's.")));
    buttons_box->addWidget(save_db_btn, 0);
    buttons_frame->setLayout(buttons_box);
    lay27->addWidget(buttons_frame, 0);
    g_lay->addLayout(lay27, 1, 0, 1, 2);
    g_lay->setColumnStretch(1, 1);
    setLayout(g_lay);
}

std::map<std::string, QObject*> ToolsDB2UIBase::membersByName() const {
    return {
        {"add_entry_btn", add_entry_btn},
        {"add_tool_from_db", add_tool_from_db},
        {"buttons_box", buttons_box},
        {"buttons_frame", buttons_frame},
        {"cancel_tool_from_db", cancel_tool_from_db},
        {"custom_offset_label", custom_offset_label},
        {"cutout_box", cutout_box},
        {"cutout_convex_box", cutout_convex_box},
        {"cutout_gaps", cutout_gaps},
        {"cutout_gaps_label", cutout_gaps_label},
        {"cutout_gapsize", cutout_gapsize},
        {"cutout_gapsize_label", cutout_gapsize_label},
        {"cutout_gaptype_radio", cutout_gaptype_radio},
        {"cutout_margin_entry", cutout_margin_entry},
        {"cutout_margin_label", cutout_margin_label},
        {"cutout_mb_dia_entry", cutout_mb_dia_entry},
        {"cutout_mb_spacing_entry", cutout_mb_spacing_entry},
        {"cutout_thin_depth_entry", cutout_thin_depth_entry},
        {"cutout_vlay", cutout_vlay},
        {"cutz_label", cutz_label},
        {"cutzlabel", cutzlabel},
        {"description_vlay", description_vlay},
        {"dia_entry", dia_entry},
        {"dia_label", dia_label},
        {"dpp_drill_label", dpp_drill_label},
        {"dpp_label", dpp_label},
        {"drill_box", drill_box},
        {"drill_cutz_entry", drill_cutz_entry},
        {"drill_dwell_cb", drill_dwell_cb},
        {"drill_dwelltime_entry", drill_dwelltime_entry},
        {"drill_feedrate_rapid_entry", drill_feedrate_rapid_entry},
        {"drill_feedrate_z_entry", drill_feedrate_z_entry},
        {"drill_last_drill_cb", drill_last_drill_cb},
        {"drill_maxdepth_entry", drill_maxdepth_entry},
        {"drill_mpass_cb", drill_mpass_cb},
        {"drill_offset_entry", drill_offset_entry},
        {"drill_overlap_entry", drill_overlap_entry},
        {"drill_overlap_label", drill_overlap_label},
        {"drill_slots_cb", drill_slots_cb},
        {"drill_slots_drill_lbl", drill_slots_drill_lbl},
        {"drill_spindlespeed_entry", drill_spindlespeed_entry},
        {"drill_travelz_entry", drill_travelz_entry},
        {"drill_vlay", drill_vlay},
        {"dwell_drill_label", dwell_drill_label},
        {"dwell_label", dwell_label},
        {"dwelltime_drill_lbl", dwelltime_drill_lbl},
        {"dwelltime_label", dwelltime_label},
        {"ecut_label", ecut_label},
        {"ecut_length_label", ecut_length_label},
        {"export_db_btn", export_db_btn},
        {"feedrate_rapid_label", feedrate_rapid_label},
        {"follow_label", follow_label},
        {"frapids_label", frapids_label},
        {"frxy_label", frxy_label},
        {"frz_label", frz_label},
        {"frzlabel", frzlabel},
        {"g_lay", g_lay},
        {"gaptype_label", gaptype_label},
        {"grid0", grid0},
        {"grid2", grid2},
        {"grid3", grid3},
        {"grid4", grid4},
        {"grid5", grid5},
        {"grid6", grid6},
        {"grid_tool", grid_tool},
        {"import_db_btn", import_db_btn},
        {"iso_box", iso_box},
        {"iso_follow_cb", iso_follow_cb},
        {"iso_milling_type_label", iso_milling_type_label},
        {"iso_milling_type_radio", iso_milling_type_radio},
        {"iso_overlap_entry", iso_overlap_entry},
        {"iso_passes_entry", iso_passes_entry},
        {"iso_type_label", iso_type_label},
        {"iso_type_radio", iso_type_radio},
        {"iso_vlay", iso_vlay},
        {"last_drill_drill_lbl", last_drill_drill_lbl},
        {"max_limit_label", max_limit_label},
        {"mb_dia_label", mb_dia_label},
        {"mb_spacing_label", mb_spacing_label},
        {"mill_custom_offset_entry", mill_custom_offset_entry},
        {"mill_cutz_entry", mill_cutz_entry},
        {"mill_dwell_cb", mill_dwell_cb},
        {"mill_dwelltime_entry", mill_dwelltime_entry},
        {"mill_ecut_cb", mill_ecut_cb},
        {"mill_ecut_length_entry", mill_ecut_length_entry},
        {"mill_frapids_entry", mill_frapids_entry},
        {"mill_frxy_entry", mill_frxy_entry},
        {"mill_frz_entry", mill_frz_entry},
        {"mill_multidepth_cb", mill_multidepth_cb},
        {"mill_multidepth_entry", mill_multidepth_entry},
        {"mill_shape_combo", mill_shape_combo},
        {"mill_spindle_entry", mill_spindle_entry},
        {"mill_tooloffset_combo", mill_tooloffset_combo},
        {"mill_travelz_entry", mill_travelz_entry},
        {"mill_type_combo", mill_type_combo},
        {"mill_vangle_entry", mill_vangle_entry},
        {"mill_vdia_entry", mill_vdia_entry},
        {"milling_box", milling_box},
        {"milling_type_label", milling_type_label},
        {"milling_vlay", milling_vlay},
        {"min_limit_label", min_limit_label},
        {"multidepth_drill_label", multidepth_drill_label},
        {"multidepth_label", multidepth_label},
        {"name_entry", name_entry},
        {"name_label", name_label},
        {"ncc_box", ncc_box},
        {"ncc_choice_offset_cb", ncc_choice_offset_cb},
        {"ncc_connect_cb", ncc_connect_cb},
        {"ncc_contour_cb", ncc_contour_cb},
        {"ncc_margin_entry", ncc_margin_entry},
        {"ncc_method_combo", ncc_method_combo},
        {"ncc_milling_type_radio", ncc_milling_type_radio},
        {"ncc_offset_spinner", ncc_offset_spinner},
        {"ncc_op_radio", ncc_op_radio},
        {"ncc_overlap_entry", ncc_overlap_entry},
        {"ncc_vlay", ncc_vlay},
        {"paint_box", paint_box},
        {"paint_contour_cb", paint_contour_cb},
        {"paint_method_combo", paint_method_combo},
        {"paint_offset_entry", paint_offset_entry},
        {"paint_overlap_entry", paint_overlap_entry},
        {"paint_vlay", paint_vlay},
        {"path_connect_cb", path_connect_cb},
        {"remove_entry_btn", remove_entry_btn},
        {"save_db_btn", save_db_btn},
        {"shape_label", shape_label},
        {"spindle_label", spindle_label},
        {"thin_depth_label", thin_depth_label},
        {"tol_label", tol_label},
        {"tol_max_entry", tol_max_entry},
        {"tol_min_entry", tol_min_entry},
        {"tool_description_box", tool_description_box},
        {"tool_offset_label", tool_offset_label},
        {"tool_op_combo", tool_op_combo},
        {"tool_op_label", tool_op_label},
        {"tooloffset_label", tooloffset_label},
        {"travelz_label", travelz_label},
        {"travelzlabel", travelzlabel},
        {"tree_widget", tree_widget},
        {"type_label", type_label},
        {"vangle_label", vangle_label},
        {"vdia_label", vdia_label},
    };
}

}  // namespace flatcam::gui
