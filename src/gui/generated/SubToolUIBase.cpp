// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/SubToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void SubToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de Sustracción"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de Sustracción"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    delete_sources_cb = new FCCheckBox(nullptr);
    delete_sources_cb->setText(QString::fromUtf8("Eliminar fuente"));
    delete_sources_cb->setToolTip(QString::fromUtf8("Cuando está marcado, eliminará los objetos de origen\ndespués de una operación exitosa."));
    lay3->addWidget(delete_sources_cb, 0, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 2, 0, 1, 3);
    auto* w5 = new QLabel(nullptr);
    lay3->addWidget(w5, 4, 0, 1, 2);
    gerber_title = new QLabel(nullptr);
    gerber_title->setText(QString::fromUtf8("<b>GERBER</b>"));
    lay3->addWidget(gerber_title, 6, 0, 1, 2);
    target_gerber_label = new QLabel(nullptr);
    target_gerber_label->setText(QString::fromUtf8("Objetivo:"));
    target_gerber_label->setToolTip(QString::fromUtf8("Objeto de Gerber para restar\nEl sustractor del objeto Gerber."));
    lay3->addWidget(target_gerber_label, 8, 0, 1, 1);
    target_gerber_combo = new FCComboBox(nullptr);
    lay3->addWidget(target_gerber_combo, 8, 1, 1, 1);
    sub_gerber_label = new QLabel(nullptr);
    sub_gerber_label->setText(QString::fromUtf8("Sustractor:"));
    sub_gerber_label->setToolTip(QString::fromUtf8("Objeto de Gerber que se restará\ndel objeto objetivo de Gerber."));
    lay3->addWidget(sub_gerber_label, 10, 0, 1, 1);
    sub_gerber_combo = new FCComboBox(nullptr);
    lay3->addWidget(sub_gerber_combo, 10, 1, 1, 1);
    intersect_btn = new FCButton(nullptr);
    { QFont f = intersect_btn->font(); f.setBold(true); intersect_btn->setFont(f); }
    intersect_btn->setIcon(QIcon(QString::fromUtf8(":/images/subtract_btn32.png")));
    intersect_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    intersect_btn->setText(QString::fromUtf8("Restar Gerber"));
    intersect_btn->setToolTip(QString::fromUtf8("Eliminará el área ocupada por el sustractor\nGerber del objetivo Gerber.\nSe puede utilizar para eliminar la serigrafía superpuesta\nsobre la máscara de soldadura."));
    lay3->addWidget(intersect_btn, 12, 0, 1, 2);
    auto* w6 = new QLabel(nullptr);
    lay3->addWidget(w6, 14, 0, 1, 2);
    geo_title = new QLabel(nullptr);
    geo_title->setText(QString::fromUtf8("<b>GEOMETRÍA</b>"));
    lay3->addWidget(geo_title, 16, 0, 1, 2);
    target_geo_label = new QLabel(nullptr);
    target_geo_label->setText(QString::fromUtf8("Objetivo:"));
    target_geo_label->setToolTip(QString::fromUtf8("Objeto de Geometría del cual restar\nEl objeto de Geometría de sustractor."));
    lay3->addWidget(target_geo_label, 18, 0, 1, 1);
    target_geo_combo = new FCComboBox(nullptr);
    lay3->addWidget(target_geo_combo, 18, 1, 1, 1);
    sub_geo_label = new QLabel(nullptr);
    sub_geo_label->setText(QString::fromUtf8("Sustractor:"));
    sub_geo_label->setToolTip(QString::fromUtf8("Objeto de Geometría que se restará\ndel objeto de Geometría de destino."));
    lay3->addWidget(sub_geo_label, 20, 0, 1, 1);
    sub_geo_combo = new FCComboBox(nullptr);
    lay3->addWidget(sub_geo_combo, 20, 1, 1, 1);
    close_paths_cb = new FCCheckBox(nullptr);
    close_paths_cb->setChecked(true);
    close_paths_cb->setText(QString::fromUtf8("Caminos cercanos"));
    close_paths_cb->setToolTip(QString::fromUtf8("Marcar esto cerrará los caminos cortados por el objeto sustractor."));
    lay3->addWidget(close_paths_cb, 22, 0, 1, 2);
    intersect_geo_btn = new FCButton(nullptr);
    { QFont f = intersect_geo_btn->font(); f.setBold(true); intersect_geo_btn->setFont(f); }
    intersect_geo_btn->setIcon(QIcon(QString::fromUtf8(":/images/subtract_btn32.png")));
    intersect_geo_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    intersect_geo_btn->setText(QString::fromUtf8("Restar Geometría"));
    intersect_geo_btn->setToolTip(QString::fromUtf8("Eliminará el área ocupada por el sustractor\nGeometría de la Geometría Objetivo."));
    lay3->addWidget(intersect_geo_btn, 24, 0, 1, 2);
    auto* w7 = new QLabel(nullptr);
    lay3->addWidget(w7, 26, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    tools_box->addLayout(lay3, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    tools_box->addWidget(reset_button, 0);
    tools_frame->setLayout(tools_box);
    layout->addWidget(tools_frame, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> SubToolUIBase::membersByName() const {
    return {
        {"close_paths_cb", close_paths_cb},
        {"delete_sources_cb", delete_sources_cb},
        {"geo_title", geo_title},
        {"gerber_title", gerber_title},
        {"intersect_btn", intersect_btn},
        {"intersect_geo_btn", intersect_geo_btn},
        {"layout", layout},
        {"reset_button", reset_button},
        {"sub_geo_combo", sub_geo_combo},
        {"sub_geo_label", sub_geo_label},
        {"sub_gerber_combo", sub_gerber_combo},
        {"sub_gerber_label", sub_gerber_label},
        {"target_geo_combo", target_geo_combo},
        {"target_geo_label", target_geo_label},
        {"target_gerber_combo", target_gerber_combo},
        {"target_gerber_label", target_gerber_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
    };
}

}  // namespace flatcam::gui
