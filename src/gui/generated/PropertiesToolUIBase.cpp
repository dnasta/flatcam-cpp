// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PropertiesToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void PropertiesToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    properties_frame = new QFrame(nullptr);
    properties_frame->setHidden(true);
    properties_box = new QVBoxLayout();
    properties_box->setContentsMargins(0, 0, 0, 0);
    properties_box->setSpacing(6);
    treeWidget = new FCTree(nullptr);
    { QSizePolicy sp(QSizePolicy::Policy(7), QSizePolicy::Policy(7)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); treeWidget->setSizePolicy(sp); }
    treeWidget->setStyleSheet(QString::fromUtf8("QTreeWidget {border: 0px;}"));
    properties_box->addWidget(treeWidget, 0);
    properties_frame->setLayout(properties_box);
    layout->addWidget(properties_frame, 0);
    setLayout(layout);
}

}  // namespace flatcam::gui
