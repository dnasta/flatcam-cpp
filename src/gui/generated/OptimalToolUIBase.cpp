// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/OptimalToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void OptimalToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de Óptima"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de Óptima"));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QFormLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    gerber_object_label = new FCLabel(nullptr);
    gerber_object_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    gerber_object_label->setToolTip(QString::fromUtf8("Gerber object for which to find the minimum distance between copper features."));
    lay3->setWidget(0, QFormLayout::ItemRole(2), gerber_object_label);
    gerber_object_combo = new FCComboBox(nullptr);
    lay3->setWidget(1, QFormLayout::ItemRole(2), gerber_object_combo);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->setWidget(2, QFormLayout::ItemRole(2), w4);
    precision_label = new FCLabel(nullptr);
    precision_label->setText(QString::fromUtf8("Precisión:"));
    precision_label->setToolTip(QString::fromUtf8("Número de decimales guardados para distancias encontradas."));
    lay3->setWidget(3, QFormLayout::ItemRole(0), precision_label);
    precision_spinner = new FCSpinner(nullptr);
    precision_spinner->setProperty("maximum", 10);
    precision_spinner->setProperty("minimum", 2);
    precision_spinner->setProperty("value", 4);
    precision_spinner->setWrapping(true);
    lay3->setWidget(3, QFormLayout::ItemRole(1), precision_spinner);
    title_res_label = new FCLabel(nullptr);
    title_res_label->setText(QString::fromUtf8("<b>Distancia minima:</b>"));
    title_res_label->setToolTip(QString::fromUtf8("Mostrar la distancia mínima entre las características de cobre."));
    lay3->setWidget(4, QFormLayout::ItemRole(2), title_res_label);
    result_label = new FCLabel(nullptr);
    result_label->setText(QString::fromUtf8("Determinado:"));
    lay3->setWidget(5, QFormLayout::ItemRole(0), result_label);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    result_entry = new FCEntry(nullptr);
    result_entry->setReadOnly(true);
    result_entry->setText(QString::fromUtf8("0.0000"));
    lay5->addWidget(result_entry, 0);
    units_lbl = new FCLabel(nullptr);
    units_lbl->setEnabled(false);
    units_lbl->setText(QString::fromUtf8("mm"));
    lay5->addWidget(units_lbl, 0);
    lay3->setLayout(5, QFormLayout::ItemRole(1), lay5);
    freq_label = new FCLabel(nullptr);
    freq_label->setText(QString::fromUtf8("Ocurriendo:"));
    freq_label->setToolTip(QString::fromUtf8("Cuántas veces se encuentra este mínimo."));
    lay3->setWidget(6, QFormLayout::ItemRole(0), freq_label);
    freq_entry = new FCEntry(nullptr);
    freq_entry->setReadOnly(true);
    freq_entry->setText(QString::fromUtf8("0"));
    lay3->setWidget(6, QFormLayout::ItemRole(1), freq_entry);
    locations_cb = new FCCheckBox(nullptr);
    locations_cb->setText(QString::fromUtf8("Coordenadas de puntos mínimos"));
    locations_cb->setToolTip(QString::fromUtf8("Coordenadas para los puntos donde se encontró la distancia mínima."));
    lay3->setWidget(7, QFormLayout::ItemRole(2), locations_cb);
    locations_textb = new FCTextArea(nullptr);
    locations_textb->setProperty("placeholderText", QString::fromUtf8("Coordenadas para los puntos donde se encontró la distancia mínima."));
    locations_textb->setReadOnly(true);
    locations_textb->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQTextEdit { selection-background-color:blue;\n\t\t\t\t\t\t\t\t\t\t\tselection-color:white;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t "));
    locations_textb->setProperty("textInteractionFlags", 1);
    locations_textb->setHidden(true);
    lay3->setWidget(8, QFormLayout::ItemRole(2), locations_textb);
    locate_button = new FCButton(nullptr);
    locate_button->setEnabled(false);
    locate_button->setMinimumSize(QSize(60, 0));
    locate_button->setText(QString::fromUtf8("Saltar a la posición seleccionada"));
    locate_button->setToolTip(QString::fromUtf8("Seleccione una posición en el cuadro de texto Ubicaciones y luego\nhaga clic en este botón."));
    locate_button->setHidden(true);
    lay3->setWidget(9, QFormLayout::ItemRole(2), locate_button);
    title_second_res_label = new FCLabel(nullptr);
    title_second_res_label->setText(QString::fromUtf8("<b>Otras distancias:</b>"));
    title_second_res_label->setToolTip(QString::fromUtf8("Mostrará otras distancias en el archivo Gerber ordenado a\nel mínimo al máximo, sin incluir el mínimo absoluto."));
    lay3->setWidget(10, QFormLayout::ItemRole(2), title_second_res_label);
    sec_locations_cb = new FCCheckBox(nullptr);
    sec_locations_cb->setText(QString::fromUtf8("Otras distancias puntos coordenadas"));
    sec_locations_cb->setToolTip(QString::fromUtf8("Otras distancias y las coordenadas de los puntos.\ndonde se encontró la distancia."));
    lay3->setWidget(11, QFormLayout::ItemRole(2), sec_locations_cb);
    layout->addLayout(lay3, 0);
    sec_locations_frame = new QFrame(nullptr);
    sec_locations_frame->setHidden(true);
    distances_box = new QVBoxLayout();
    distances_box->setContentsMargins(0, 0, 0, 0);
    distances_box->setSpacing(6);
    distances_label = new FCLabel(nullptr);
    distances_label->setText(QString::fromUtf8("Distancias de Gerber"));
    distances_label->setToolTip(QString::fromUtf8("Otras distancias y las coordenadas de los puntos.\ndonde se encontró la distancia."));
    distances_box->addWidget(distances_label, 0);
    distances_textb = new FCTextArea(nullptr);
    distances_textb->setProperty("placeholderText", QString::fromUtf8("Otras distancias y las coordenadas de los puntos.\ndonde se encontró la distancia."));
    distances_textb->setReadOnly(true);
    distances_textb->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQTextEdit { selection-background-color:blue;\n\t\t\t\t\t\t\t\t\t\t\tselection-color:white;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t "));
    distances_textb->setProperty("textInteractionFlags", 1);
    distances_box->addWidget(distances_textb, 0);
    auto* w6 = new FCLabel(nullptr);
    distances_box->addWidget(w6, 0);
    locations_label = new FCLabel(nullptr);
    locations_label->setText(QString::fromUtf8("Coordenadas de puntos"));
    locations_label->setToolTip(QString::fromUtf8("Otras distancias y las coordenadas de los puntos.\ndonde se encontró la distancia."));
    distances_box->addWidget(locations_label, 0);
    locations_sec_textb = new FCTextArea(nullptr);
    locations_sec_textb->setProperty("placeholderText", QString::fromUtf8("Otras distancias y las coordenadas de los puntos.\ndonde se encontró la distancia."));
    locations_sec_textb->setReadOnly(true);
    locations_sec_textb->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQTextEdit { selection-background-color:blue;\n\t\t\t\t\t\t\t\t\t\t\tselection-color:white;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t "));
    locations_sec_textb->setProperty("textInteractionFlags", 1);
    distances_box->addWidget(locations_sec_textb, 0);
    locate_sec_button = new FCButton(nullptr);
    locate_sec_button->setEnabled(false);
    locate_sec_button->setMinimumSize(QSize(60, 0));
    locate_sec_button->setText(QString::fromUtf8("Saltar a la posición seleccionada"));
    locate_sec_button->setToolTip(QString::fromUtf8("Seleccione una posición en el cuadro de texto Ubicaciones y luego\nhaga clic en este botón."));
    distances_box->addWidget(locate_sec_button, 0);
    sec_locations_frame->setLayout(distances_box);
    layout->addWidget(sec_locations_frame, 0);
    calculate_button = new FCButton(nullptr);
    { QFont f = calculate_button->font(); f.setBold(true); calculate_button->setFont(f); }
    calculate_button->setIcon(QIcon(QString::fromUtf8(":/images/open_excellon32.png")));
    calculate_button->setMinimumSize(QSize(60, 0));
    calculate_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    calculate_button->setText(QString::fromUtf8("Encuentra mínimo"));
    calculate_button->setToolTip(QString::fromUtf8("Calcule la distancia mínima entre las características de cobre,\nesto permitirá determinar la herramienta adecuada para\nutilizar para aislamiento o limpieza de cobre."));
    layout->addWidget(calculate_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> OptimalToolUIBase::membersByName() const {
    return {
        {"calculate_button", calculate_button},
        {"distances_box", distances_box},
        {"distances_label", distances_label},
        {"distances_textb", distances_textb},
        {"freq_entry", freq_entry},
        {"freq_label", freq_label},
        {"gerber_object_combo", gerber_object_combo},
        {"gerber_object_label", gerber_object_label},
        {"layout", layout},
        {"locate_button", locate_button},
        {"locate_sec_button", locate_sec_button},
        {"locations_cb", locations_cb},
        {"locations_label", locations_label},
        {"locations_sec_textb", locations_sec_textb},
        {"locations_textb", locations_textb},
        {"precision_label", precision_label},
        {"precision_spinner", precision_spinner},
        {"reset_button", reset_button},
        {"result_entry", result_entry},
        {"result_label", result_label},
        {"sec_locations_cb", sec_locations_cb},
        {"sec_locations_frame", sec_locations_frame},
        {"title_res_label", title_res_label},
        {"title_second_res_label", title_second_res_label},
        {"units_lbl", units_lbl},
    };
}

}  // namespace flatcam::gui
