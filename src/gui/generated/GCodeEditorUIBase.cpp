// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GCodeEditorUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void GCodeEditorUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    edit_frame = new QFrame(nullptr);
    edit_box = new QVBoxLayout();
    edit_box->setContentsMargins(0, 0, 0, 0);
    edit_box->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new QLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new QLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>GCode Editor</b></font>")));
    title_box->addWidget(title_label, 1);
    edit_box->addLayout(title_box, 0);
    name_box = new QHBoxLayout();
    name_box->setContentsMargins(0, 0, 0, 0);
    name_box->setSpacing(6);
    auto* w2 = new QLabel(nullptr);
    w2->setText(trUi_(QString::fromUtf8("Name:")));
    name_box->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_box->addWidget(name_entry, 0);
    edit_box->addLayout(name_box, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    edit_box->addWidget(w3, 0);
    cnc_tools_table = new FCTable(nullptr);
    cnc_tools_table->setProperty("columnCount", 6);
    cnc_tools_table->setProperty("selectionBehavior", 1);
    cnc_tools_table->setColumnCount(6);
    cnc_tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Dia")), trUi_(QString::fromUtf8("Offset")), trUi_(QString::fromUtf8("Type")), trUi_(QString::fromUtf8("TT")), trUi_(QString::fromUtf8(""))});
    cnc_tools_table->setColumnHidden(5, true);
    edit_box->addWidget(cnc_tools_table, 0);
    exc_cnc_tools_table = new FCTable(nullptr);
    exc_cnc_tools_table->setProperty("columnCount", 6);
    exc_cnc_tools_table->setProperty("selectionBehavior", 1);
    exc_cnc_tools_table->setColumnCount(6);
    exc_cnc_tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#")), trUi_(QString::fromUtf8("Dia")), trUi_(QString::fromUtf8("Drills")), trUi_(QString::fromUtf8("Slots")), trUi_(QString::fromUtf8("")), trUi_(QString::fromUtf8("Cut Z"))});
    exc_cnc_tools_table->setColumnHidden(4, true);
    edit_box->addWidget(exc_cnc_tools_table, 0);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    edit_box->addWidget(w4, 0);
    auto* w5 = new QLabel(nullptr);
    w5->setText(trUi_(QString::fromUtf8("CNC Code Snippet 1:")));
    w5->setToolTip(trUi_(QString::fromUtf8("Code snippet defined in Preferences.")));
    edit_box->addWidget(w5, 0);
    prepend_text = new FCTextArea(nullptr);
    prepend_text->setProperty("placeholderText", trUi_(QString::fromUtf8("Type here any G-Code commands you would\nlike to insert at the cursor location.")));
    edit_box->addWidget(prepend_text, 0);
    update_gcode_button = new FCButton(nullptr);
    update_gcode_button->setText(trUi_(QString::fromUtf8("Insert Code")));
    update_gcode_button->setToolTip(trUi_(QString::fromUtf8("Insert the code above at the cursor location.")));
    edit_box->addWidget(update_gcode_button, 0);
    auto* w6 = new QLabel(nullptr);
    w6->setText(trUi_(QString::fromUtf8("CNC Code Snippet 2:")));
    w6->setToolTip(trUi_(QString::fromUtf8("Code snippet defined in Preferences.")));
    edit_box->addWidget(w6, 0);
    append_text = new FCTextArea(nullptr);
    append_text->setProperty("placeholderText", trUi_(QString::fromUtf8("Type here any G-Code commands you would\nlike to insert at the cursor location.")));
    edit_box->addWidget(append_text, 0);
    update_gcode_sec_button = new FCButton(nullptr);
    update_gcode_sec_button->setText(trUi_(QString::fromUtf8("Insert Code")));
    update_gcode_sec_button->setToolTip(trUi_(QString::fromUtf8("Insert the code above at the cursor location.")));
    edit_box->addWidget(update_gcode_sec_button, 0);
    edit_frame->setLayout(edit_box);
    lay1->addWidget(edit_frame, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    exit_editor_button = new FCButton(nullptr);
    { QFont f = exit_editor_button->font(); f.setBold(true); exit_editor_button->setFont(f); }
    exit_editor_button->setIcon(QIcon(QString::fromUtf8(":/images/power16.png")));
    exit_editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t\t  "));
    exit_editor_button->setText(trUi_(QString::fromUtf8("Exit Editor")));
    exit_editor_button->setToolTip(trUi_(QString::fromUtf8("Exit from Editor.")));
    lay1->addWidget(exit_editor_button, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> GCodeEditorUIBase::membersByName() const {
    return {
        {"append_text", append_text},
        {"cnc_tools_table", cnc_tools_table},
        {"edit_box", edit_box},
        {"edit_frame", edit_frame},
        {"exc_cnc_tools_table", exc_cnc_tools_table},
        {"exit_editor_button", exit_editor_button},
        {"icon", icon},
        {"name_box", name_box},
        {"name_entry", name_entry},
        {"prepend_text", prepend_text},
        {"title_box", title_box},
        {"title_label", title_label},
        {"update_gcode_button", update_gcode_button},
        {"update_gcode_sec_button", update_gcode_sec_button},
    };
}

}  // namespace flatcam::gui
