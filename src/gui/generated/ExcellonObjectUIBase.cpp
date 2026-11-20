// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ExcellonObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void ExcellonObjectUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(9, 9, 9, 9);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/drill32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Excellon Object</b></font>")));
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
    plot_options_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    lay2->addWidget(plot_options_label, 0, 0, 1, 1);
    solid_cb = new FCCheckBox(nullptr);
    solid_cb->setChecked(true);
    solid_cb->setText(trUi_(QString::fromUtf8("Solid")));
    solid_cb->setToolTip(trUi_(QString::fromUtf8("Solid circles.")));
    lay2->addWidget(solid_cb, 0, 1, 1, 1);
    multicolored_cb = new FCCheckBox(nullptr);
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
    editor_button->setText(trUi_(QString::fromUtf8("Excellon Editor")));
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
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(trUi_(QString::fromUtf8("<b>Tools Table</b>")));
    tools_table_label->setToolTip(trUi_(QString::fromUtf8("Tools in this Excellon object\nwhen are used for drilling.")));
    lay2->addWidget(tools_table_label, 8, 0, 1, 1);
    table_visibility_cb = new FCCheckBox(nullptr);
    table_visibility_cb->setChecked(true);
    table_visibility_cb->setToolTip(trUi_(QString::fromUtf8("Toggle the display of the Tools Table.")));
    table_visibility_cb->setHidden(true);
    lay2->addWidget(table_visibility_cb, 8, 1, 1, 1);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    lay4->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    plot_cb = new FCCheckBox(nullptr);
    plot_cb->setChecked(true);
    plot_cb->setProperty("layoutDirection", 1);
    plot_cb->setText(trUi_(QString::fromUtf8("Plot")));
    plot_cb->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay4->addWidget(plot_cb, 0);
    lay2->addLayout(lay4, 8, 2, 1, 1);
    lay2->setColumnStretch(1, 1);
    custom_box->addLayout(lay2, 0);
    drills_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    tools_table = new FCTable(nullptr);
    tools_table->setProperty("columnCount", 6);
    tools_table->setProperty("selectionBehavior", 1);
    tools_table->setColumnCount(6);
    tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Diameter")), trUi_(QString::fromUtf8("Drills")), trUi_(QString::fromUtf8("Slots")), trUi_(QString::fromUtf8("C")), trUi_(QString::fromUtf8("P"))});
    tools_table->setColumnHidden(4, true);
    tools_table->setColumnHidden(5, true);
    tools_box->addWidget(tools_table, 0);
    autoload_db_cb = new FCCheckBox(nullptr);
    autoload_db_cb->setText(trUi_(QString::fromUtf8("Auto load from DB")));
    autoload_db_cb->setToolTip(trUi_(QString::fromUtf8("Automatic replacement of the tools from related application tools\nwith tools from DB that have a close diameter value.")));
    autoload_db_cb->setHidden(true);
    tools_box->addWidget(autoload_db_cb, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay5->addWidget(w6, 0, 0, 1, 2);
    tool_lbl = new FCLabel(nullptr);
    tool_lbl->setText(trUi_(QString::fromUtf8("<b>TOOLS</b>")));
    lay5->addWidget(tool_lbl, 2, 0, 1, 2);
    drill_button = new FCButton(nullptr);
    { QFont f = drill_button->font(); f.setBold(true); drill_button->setFont(f); }
    drill_button->setIcon(QIcon(QString::fromUtf8(":/images/drilling_tool32.png")));
    drill_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    drill_button->setText(trUi_(QString::fromUtf8("Drilling Tool")));
    drill_button->setToolTip(trUi_(QString::fromUtf8("Generate GCode from the drill holes in an Excellon object.")));
    lay5->addWidget(drill_button, 4, 0, 1, 2);
    milling_button = new FCButton(nullptr);
    milling_button->setEnabled(false);
    milling_button->setIcon(QIcon(QString::fromUtf8(":/images/milling_tool32.png")));
    milling_button->setText(trUi_(QString::fromUtf8("Milling Tool")));
    milling_button->setToolTip(trUi_(QString::fromUtf8("Generate a Geometry for milling drills or slots in an Excellon object.")));
    lay5->addWidget(milling_button, 6, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay5->addWidget(w7, 8, 0, 1, 2);
    util_button = new FCButton(nullptr);
    util_button->setCheckable(true);
    { QFont f = util_button->font(); f.setBold(true); util_button->setFont(f); }
    util_button->setIcon(QIcon(QString::fromUtf8(":/images/settings18.png")));
    util_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    util_button->setText(trUi_(QString::fromUtf8("UTILTIES")));
    util_button->setToolTip(trUi_(QString::fromUtf8("Show the Utilties.")));
    lay5->addWidget(util_button, 10, 0, 1, 2);
    util_frame = new QFrame(nullptr);
    util_frame->setHidden(true);
    util_box = new QVBoxLayout();
    util_box->setContentsMargins(0, 0, 0, 0);
    util_box->setSpacing(6);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    mill_hole_label = new FCLabel(nullptr);
    mill_hole_label->setText(trUi_(QString::fromUtf8("<b>Milling Geometry</b>")));
    mill_hole_label->setToolTip(trUi_(QString::fromUtf8("Create Geometry for milling holes.\nSelect from the Tools Table above the hole dias to be\nmilled. Use the # column to make the selection.")));
    lay8->addWidget(mill_hole_label, 0, 0, 1, 3);
    tdlabel = new FCLabel(nullptr);
    tdlabel->setText(trUi_(QString::fromUtf8("Milling Diameter:")));
    tdlabel->setToolTip(trUi_(QString::fromUtf8("Diameter of the cutting tool.")));
    lay8->addWidget(tdlabel, 2, 0, 1, 3);
    tooldia_entry = new FCDoubleSpinner(nullptr);
    tooldia_entry->setProperty("decimals", 4);
    tooldia_entry->setProperty("maximum", 10000.0);
    tooldia_entry->setProperty("singleStep", 0.1);
    tooldia_entry->setProperty("value", 0.8);
    lay8->addWidget(tooldia_entry, 4, 0, 1, 2);
    generate_milling_button = new FCButton(nullptr);
    { QFont f = generate_milling_button->font(); f.setBold(true); generate_milling_button->setFont(f); }
    generate_milling_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    generate_milling_button->setText(trUi_(QString::fromUtf8("Mill Drills")));
    generate_milling_button->setToolTip(trUi_(QString::fromUtf8("Create the Geometry Object\nfor milling drills.")));
    lay8->addWidget(generate_milling_button, 4, 2, 1, 1);
    slot_tooldia_entry = new FCDoubleSpinner(nullptr);
    slot_tooldia_entry->setProperty("decimals", 4);
    slot_tooldia_entry->setProperty("maximum", 10000.0);
    slot_tooldia_entry->setProperty("singleStep", 0.1);
    slot_tooldia_entry->setProperty("value", 1.8);
    lay8->addWidget(slot_tooldia_entry, 6, 0, 1, 2);
    generate_milling_slots_button = new FCButton(nullptr);
    { QFont f = generate_milling_slots_button->font(); f.setBold(true); generate_milling_slots_button->setFont(f); }
    generate_milling_slots_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    generate_milling_slots_button->setText(trUi_(QString::fromUtf8("Mill Slots")));
    generate_milling_slots_button->setToolTip(trUi_(QString::fromUtf8("Create the Geometry Object\nfor milling slots.")));
    lay8->addWidget(generate_milling_slots_button, 6, 2, 1, 1);
    lay8->setColumnStretch(1, 1);
    util_box->addLayout(lay8, 0);
    util_frame->setLayout(util_box);
    lay5->addWidget(util_frame, 12, 0, 1, 2);
    lay5->setColumnStretch(1, 1);
    tools_box->addLayout(lay5, 0);
    drills_frame->setLayout(tools_box);
    custom_box->addWidget(drills_frame, 0);
    lay1->addLayout(custom_box, 0);
    common_grid = new QGridLayout();
    common_grid->setContentsMargins(0, 0, 0, 0);
    common_grid->setHorizontalSpacing(6);
    common_grid->setVerticalSpacing(6);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    common_grid->addWidget(w9, 1, 0, 1, 2);
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

std::map<std::string, QObject*> ExcellonObjectUIBase::membersByName() const {
    return {
        {"autoload_db_cb", autoload_db_cb},
        {"common_grid", common_grid},
        {"custom_box", custom_box},
        {"drill_button", drill_button},
        {"drills_frame", drills_frame},
        {"editor_button", editor_button},
        {"generate_milling_button", generate_milling_button},
        {"generate_milling_slots_button", generate_milling_slots_button},
        {"icon", icon},
        {"level", level},
        {"mill_hole_label", mill_hole_label},
        {"milling_button", milling_button},
        {"multicolored_cb", multicolored_cb},
        {"name_entry", name_entry},
        {"name_hlay", name_hlay},
        {"offset_button", offset_button},
        {"offsetvector_entry", offsetvector_entry},
        {"plot_cb", plot_cb},
        {"plot_options_label", plot_options_label},
        {"properties_box", properties_box},
        {"properties_button", properties_button},
        {"properties_frame", properties_frame},
        {"scale_button", scale_button},
        {"scale_entry", scale_entry},
        {"slot_tooldia_entry", slot_tooldia_entry},
        {"solid_cb", solid_cb},
        {"table_visibility_cb", table_visibility_cb},
        {"tdlabel", tdlabel},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tool_lbl", tool_lbl},
        {"tooldia_entry", tooldia_entry},
        {"tools_box", tools_box},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"transform_label", transform_label},
        {"transformations_button", transformations_button},
        {"treeWidget", treeWidget},
        {"util_box", util_box},
        {"util_button", util_button},
        {"util_frame", util_frame},
    };
}

}  // namespace flatcam::gui
