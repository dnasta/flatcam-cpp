// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GeoEditorUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void GeoEditorUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    geo_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(QString::fromUtf8("<font size=5><b>Editor de geometría</b></font>"));
    title_box->addWidget(title_label, 1);
    auto* w2 = new FCLabel(nullptr);
    title_box->addWidget(w2, 0);
    tools_box->addLayout(title_box, 0);
    tw = new FCTree(nullptr);
    tw->setProperty("columnCount", 3);
    tw->setHeaderHidden(false);
    tw->setProperty("indentation", 0);
    tw->setProperty("selectionMode", 3);
    tools_box->addWidget(tw, 0);
    geo_frame->setLayout(tools_box);
    lay1->addWidget(geo_frame, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    exit_editor_button = new FCButton(nullptr);
    { QFont f = exit_editor_button->font(); f.setBold(true); exit_editor_button->setFont(f); }
    exit_editor_button->setIcon(QIcon(QString::fromUtf8(":/images/power16.png")));
    exit_editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    exit_editor_button->setText(QString::fromUtf8("Salir del editor"));
    exit_editor_button->setToolTip(QString::fromUtf8("Salida del editor."));
    lay1->addWidget(exit_editor_button, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> GeoEditorUIBase::membersByName() const {
    return {
        {"exit_editor_button", exit_editor_button},
        {"geo_frame", geo_frame},
        {"icon", icon},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tools_box", tools_box},
        {"tw", tw},
    };
}

}  // namespace flatcam::gui
