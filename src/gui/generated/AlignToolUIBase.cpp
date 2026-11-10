// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/AlignToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void AlignToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Alinear objetos"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Alinear objetos"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    auto* w4 = new FCLabel(nullptr);
    w4->setText(QString::fromUtf8("<b>Objeto en movimiento:</b>"));
    w4->setToolTip(QString::fromUtf8("Especifique el tipo de objeto a alinear.\nPuede ser de tipo: Gerber o Excellon.\nLa selección aquí decide el tipo de objetos que serán\nen el cuadro combinado Objeto."));
    lay3->addWidget(w4, 0, 0, 1, 2);
    type_obj_radio = new RadioSet({{QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("grb"))}, {QString::fromUtf8("Excellon"), QVariant(QString::fromUtf8("exc"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(type_obj_radio, 3, 0, 1, 2);
    object_combo = new FCComboBox(nullptr);
    object_combo->setToolTip(QString::fromUtf8("Objeto a alinear."));
    lay3->addWidget(object_combo, 4, 0, 1, 2);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay3->addWidget(w5, 5, 0, 1, 2);
    auto* w6 = new FCLabel(nullptr);
    lay3->addWidget(w6, 6, 0, 1, 2);
    aligned_label = new FCLabel(nullptr);
    aligned_label->setText(QString::fromUtf8("<b>Objeto DESTINO:</b>"));
    aligned_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto a alinear.\nPuede ser de tipo: Gerber o Excellon.\nLa selección aquí decide el tipo de objetos que serán\nen el cuadro combinado Objeto."));
    lay3->addWidget(aligned_label, 7, 0, 1, 2);
    type_aligner_obj_radio = new RadioSet({{QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("grb"))}, {QString::fromUtf8("Excellon"), QVariant(QString::fromUtf8("exc"))}}, Qt::Horizontal, true, nullptr);
    type_aligner_obj_radio->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(type_aligner_obj_radio, 8, 0, 1, 2);
    aligner_object_combo = new FCComboBox(nullptr);
    aligner_object_combo->setToolTip(QString::fromUtf8("Objeto a alinear. Alineador."));
    lay3->addWidget(aligner_object_combo, 9, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay3->addWidget(w7, 10, 0, 1, 2);
    auto* w8 = new QLabel(nullptr);
    lay3->addWidget(w8, 11, 0, 1, 2);
    a_type_lbl = new FCLabel(nullptr);
    a_type_lbl->setText(QString::fromUtf8("<b>Tipo de alineación:</b>"));
    a_type_lbl->setToolTip(QString::fromUtf8("El tipo de alineación puede ser:\n- Punto único -> requiere un único punto de sincronización, la acción será una traducción\n- Punto doble -> requiere dos puntos de sincronización, la acción será traslación seguida de rotación"));
    lay3->addWidget(a_type_lbl, 12, 0, 1, 2);
    a_type_radio = new RadioSet({{QString::fromUtf8("Punto único"), QVariant(QString::fromUtf8("sp"))}, {QString::fromUtf8("Punto doble"), QVariant(QString::fromUtf8("dp"))}}, Qt::Horizontal, true, nullptr);
    a_type_radio->setValue(QVariant(QString::fromUtf8("sp")));
    lay3->addWidget(a_type_radio, 13, 0, 1, 2);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay3->addWidget(w9, 14, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    align_object_button = new FCButton(nullptr);
    { QFont f = align_object_button->font(); f.setBold(true); align_object_button->setFont(f); }
    align_object_button->setIcon(QIcon(QString::fromUtf8(":/images/align16.png")));
    align_object_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    align_object_button->setText(QString::fromUtf8("Alinear objeto"));
    align_object_button->setToolTip(QString::fromUtf8("Alinee el objeto especificado con el objeto alineador.\nSi solo se utiliza un punto, se supone que se traduce.\nSi se utilizan estos puntos, se supone traslación y rotación."));
    layout->addWidget(align_object_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> AlignToolUIBase::membersByName() const {
    return {
        {"a_type_lbl", a_type_lbl},
        {"a_type_radio", a_type_radio},
        {"align_object_button", align_object_button},
        {"aligned_label", aligned_label},
        {"aligner_object_combo", aligner_object_combo},
        {"layout", layout},
        {"object_combo", object_combo},
        {"reset_button", reset_button},
        {"type_aligner_obj_radio", type_aligner_obj_radio},
        {"type_obj_radio", type_obj_radio},
    };
}

}  // namespace flatcam::gui
