// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GerberObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void GerberObjectUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(9, 9, 9, 9);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Gerber Object</b></font>")));
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
    solid_cb->setToolTip(trUi_(QString::fromUtf8("Solid color polygons.")));
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
    lay2->addLayout(name_hlay, 1, 0, 1, 3);
    plot_lbl = new FCLabel(nullptr);
    plot_lbl->setText(trUi_(QString::fromUtf8("Plot:")));
    plot_lbl->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay2->addWidget(plot_lbl, 2, 0, 1, 1);
    plot_cb = new FCCheckBox(nullptr);
    plot_cb->setChecked(true);
    lay2->addWidget(plot_cb, 2, 1, 1, 1);
    follow_cb = new FCCheckBox(nullptr);
    follow_cb->setText(trUi_(QString::fromUtf8("Follow")));
    follow_cb->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    follow_cb->setHidden(true);
    lay2->addWidget(follow_cb, 2, 2, 1, 1);
    editor_button = new FCButton(nullptr);
    { QFont f = editor_button->font(); f.setBold(true); editor_button->setFont(f); }
    editor_button->setIcon(QIcon(QString::fromUtf8(":/images/edit_file32.png")));
    editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    editor_button->setText(trUi_(QString::fromUtf8("Gerber Editor")));
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
    apertures_table_label = new FCLabel(nullptr);
    apertures_table_label->setText(trUi_(QString::fromUtf8("Apertures:")));
    apertures_table_label->setToolTip(trUi_(QString::fromUtf8("Apertures Table for the Gerber Object.")));
    apertures_table_label->setHidden(true);
    lay2->addWidget(apertures_table_label, 8, 0, 1, 1);
    aperture_table_visibility_cb = new FCCheckBox(nullptr);
    aperture_table_visibility_cb->setToolTip(trUi_(QString::fromUtf8("Toggle the display of the Tools Table.")));
    aperture_table_visibility_cb->setHidden(true);
    lay2->addWidget(aperture_table_visibility_cb, 8, 1, 1, 1);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    lay4->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    mark_all_cb = new FCCheckBox(nullptr);
    mark_all_cb->setProperty("layoutDirection", 1);
    mark_all_cb->setText(trUi_(QString::fromUtf8("Mark All")));
    mark_all_cb->setToolTip(trUi_(QString::fromUtf8("When checked it will display all the apertures.\nWhen unchecked, it will delete all mark shapes\nthat are drawn on canvas.")));
    mark_all_cb->setHidden(true);
    lay4->addWidget(mark_all_cb, 0);
    lay2->addLayout(lay4, 8, 2, 1, 1);
    apertures_table = new FCTable(nullptr);
    apertures_table->setProperty("columnCount", 6);
    apertures_table->setHidden(true);
    apertures_table->setColumnCount(6);
    apertures_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Code")), trUi_(QString::fromUtf8("Type")), trUi_(QString::fromUtf8("Size")), trUi_(QString::fromUtf8("Dim")), trUi_(QString::fromUtf8("M"))});
    lay2->addWidget(apertures_table, 10, 0, 1, 3);
    create_buffer_button = new FCButton(nullptr);
    create_buffer_button->setText(trUi_(QString::fromUtf8("Buffer Solid Geometry")));
    create_buffer_button->setToolTip(trUi_(QString::fromUtf8("This button is shown only when the Gerber file\nis loaded without buffering.\nClicking this will create the buffered geometry\nrequired for isolation.")));
    create_buffer_button->setHidden(true);
    lay2->addWidget(create_buffer_button, 12, 0, 1, 3);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay2->addWidget(w5, 13, 0, 1, 3);
    tool_lbl = new FCLabel(nullptr);
    tool_lbl->setText(trUi_(QString::fromUtf8("<b>TOOLS</b>")));
    lay2->addWidget(tool_lbl, 14, 0, 1, 3);
    iso_button = new FCButton(nullptr);
    { QFont f = iso_button->font(); f.setBold(true); iso_button->setFont(f); }
    iso_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    iso_button->setText(trUi_(QString::fromUtf8("Isolation Routing")));
    iso_button->setToolTip(trUi_(QString::fromUtf8("Create a Geometry object with\ntoolpaths to cut around polygons.")));
    lay2->addWidget(iso_button, 16, 0, 1, 3);
    generate_ncc_button = new FCButton(nullptr);
    generate_ncc_button->setIcon(QIcon(QString::fromUtf8(":/images/eraser26.png")));
    generate_ncc_button->setText(trUi_(QString::fromUtf8("NCC Tool")));
    generate_ncc_button->setToolTip(trUi_(QString::fromUtf8("Create the Geometry Object\nfor non-copper routing.")));
    lay2->addWidget(generate_ncc_button, 18, 0, 1, 3);
    generate_cutout_button = new FCButton(nullptr);
    generate_cutout_button->setIcon(QIcon(QString::fromUtf8(":/images/cut32_bis.png")));
    generate_cutout_button->setText(trUi_(QString::fromUtf8("Cutout Tool")));
    generate_cutout_button->setToolTip(trUi_(QString::fromUtf8("Generate the geometry for\nthe board cutout.")));
    lay2->addWidget(generate_cutout_button, 20, 0, 1, 3);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay2->addWidget(w6, 22, 0, 1, 3);
    util_button = new FCButton(nullptr);
    util_button->setCheckable(true);
    { QFont f = util_button->font(); f.setBold(true); util_button->setFont(f); }
    util_button->setIcon(QIcon(QString::fromUtf8(":/images/settings18.png")));
    util_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    util_button->setText(trUi_(QString::fromUtf8("UTILTIES")));
    util_button->setToolTip(trUi_(QString::fromUtf8("Show the Utilties.")));
    lay2->addWidget(util_button, 24, 0, 1, 3);
    util_frame = new QFrame(nullptr);
    util_frame->setHidden(true);
    util_box = new QVBoxLayout();
    util_box->setContentsMargins(0, 0, 0, 0);
    util_box->setSpacing(6);
    auto* lay7 = new QGridLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    noncopper_label = new FCLabel(nullptr);
    noncopper_label->setText(trUi_(QString::fromUtf8("<b>Non-copper regions</b>")));
    noncopper_label->setToolTip(trUi_(QString::fromUtf8("Create polygons covering the\nareas without copper on the PCB.\nEquivalent to the inverse of this\nobject. Can be used to remove all\ncopper from a specified region.")));
    lay7->addWidget(noncopper_label, 0, 0, 1, 3);
    auto* w8 = new FCLabel(nullptr);
    w8->setText(trUi_(QString::fromUtf8("Boundary Margin:")));
    w8->setToolTip(trUi_(QString::fromUtf8("Specify the edge of the PCB\nby drawing a box around all\nobjects with this minimum\ndistance.")));
    lay7->addWidget(w8, 2, 0, 1, 1);
    noncopper_margin_entry = new FCDoubleSpinner(nullptr);
    noncopper_margin_entry->setProperty("decimals", 4);
    noncopper_margin_entry->setProperty("maximum", 10000.0);
    noncopper_margin_entry->setProperty("minimum", -10000.0);
    noncopper_margin_entry->setProperty("singleStep", 0.1);
    noncopper_margin_entry->setProperty("value", 0.1);
    lay7->addWidget(noncopper_margin_entry, 2, 1, 1, 2);
    noncopper_rounded_cb = new FCCheckBox(nullptr);
    noncopper_rounded_cb->setText(trUi_(QString::fromUtf8("Rounded")));
    noncopper_rounded_cb->setToolTip(trUi_(QString::fromUtf8("Resulting geometry will have rounded corners.")));
    lay7->addWidget(noncopper_rounded_cb, 4, 0, 1, 1);
    generate_noncopper_button = new FCButton(nullptr);
    generate_noncopper_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_noncopper_button->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    lay7->addWidget(generate_noncopper_button, 4, 1, 1, 2);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay7->addWidget(w9, 6, 0, 1, 3);
    boundingbox_label = new FCLabel(nullptr);
    boundingbox_label->setText(trUi_(QString::fromUtf8("<b>Bounding Box</b>")));
    boundingbox_label->setToolTip(trUi_(QString::fromUtf8("Create a geometry surrounding the Gerber object.\nSquare shape.")));
    lay7->addWidget(boundingbox_label, 8, 0, 1, 3);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(trUi_(QString::fromUtf8("Boundary Margin:")));
    w10->setToolTip(trUi_(QString::fromUtf8("Distance of the edges of the box\nto the nearest polygon.")));
    lay7->addWidget(w10, 10, 0, 1, 1);
    bbmargin_entry = new FCDoubleSpinner(nullptr);
    bbmargin_entry->setProperty("decimals", 4);
    bbmargin_entry->setProperty("maximum", 10000.0);
    bbmargin_entry->setProperty("minimum", -10000.0);
    bbmargin_entry->setProperty("singleStep", 0.1);
    bbmargin_entry->setProperty("value", 0.1);
    lay7->addWidget(bbmargin_entry, 10, 1, 1, 2);
    bbrounded_cb = new FCCheckBox(nullptr);
    bbrounded_cb->setText(trUi_(QString::fromUtf8("Rounded")));
    bbrounded_cb->setToolTip(trUi_(QString::fromUtf8("If the bounding box is \nto have rounded corners\ntheir radius is equal to\nthe margin.")));
    lay7->addWidget(bbrounded_cb, 12, 0, 1, 1);
    generate_bb_button = new FCButton(nullptr);
    generate_bb_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_bb_button->setText(trUi_(QString::fromUtf8("Generate Geometry")));
    generate_bb_button->setToolTip(trUi_(QString::fromUtf8("Generate the Geometry object.")));
    lay7->addWidget(generate_bb_button, 12, 1, 1, 2);
    lay7->setColumnStretch(1, 1);
    util_box->addLayout(lay7, 0);
    util_frame->setLayout(util_box);
    lay2->addWidget(util_frame, 25, 0, 1, 3);
    lay2->setColumnStretch(1, 1);
    custom_box->addLayout(lay2, 0);
    lay1->addLayout(custom_box, 0);
    common_grid = new QGridLayout();
    common_grid->setContentsMargins(0, 0, 0, 0);
    common_grid->setHorizontalSpacing(6);
    common_grid->setVerticalSpacing(6);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    common_grid->addWidget(w11, 1, 0, 1, 2);
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

std::map<std::string, QObject*> GerberObjectUIBase::membersByName() const {
    return {
        {"aperture_table_visibility_cb", aperture_table_visibility_cb},
        {"apertures_table", apertures_table},
        {"apertures_table_label", apertures_table_label},
        {"bbmargin_entry", bbmargin_entry},
        {"bbrounded_cb", bbrounded_cb},
        {"boundingbox_label", boundingbox_label},
        {"common_grid", common_grid},
        {"create_buffer_button", create_buffer_button},
        {"custom_box", custom_box},
        {"editor_button", editor_button},
        {"follow_cb", follow_cb},
        {"generate_bb_button", generate_bb_button},
        {"generate_cutout_button", generate_cutout_button},
        {"generate_ncc_button", generate_ncc_button},
        {"generate_noncopper_button", generate_noncopper_button},
        {"icon", icon},
        {"iso_button", iso_button},
        {"level", level},
        {"mark_all_cb", mark_all_cb},
        {"multicolored_cb", multicolored_cb},
        {"name_entry", name_entry},
        {"name_hlay", name_hlay},
        {"noncopper_label", noncopper_label},
        {"noncopper_margin_entry", noncopper_margin_entry},
        {"noncopper_rounded_cb", noncopper_rounded_cb},
        {"offset_button", offset_button},
        {"offsetvector_entry", offsetvector_entry},
        {"plot_cb", plot_cb},
        {"plot_lbl", plot_lbl},
        {"plot_options_label", plot_options_label},
        {"properties_box", properties_box},
        {"properties_button", properties_button},
        {"properties_frame", properties_frame},
        {"scale_button", scale_button},
        {"scale_entry", scale_entry},
        {"solid_cb", solid_cb},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tool_lbl", tool_lbl},
        {"transform_label", transform_label},
        {"transformations_button", transformations_button},
        {"treeWidget", treeWidget},
        {"util_box", util_box},
        {"util_button", util_button},
        {"util_frame", util_frame},
    };
}

}  // namespace flatcam::gui
