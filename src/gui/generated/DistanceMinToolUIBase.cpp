// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DistanceMinToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void DistanceMinToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    w1->setText(QString::fromUtf8("<font size=4><b>Herramienta de Distancia Mínima</b></font><br>"));
    layout->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    units_label = new QLabel(nullptr);
    units_label->setText(QString::fromUtf8("Unidades:"));
    units_label->setToolTip(QString::fromUtf8("Esas son las unidades en las que se mide la distancia."));
    lay2->setWidget(0, QFormLayout::ItemRole(0), units_label);
    units_value = new QLabel(nullptr);
    units_value->setEnabled(false);
    units_value->setText(QString::fromUtf8("MÉTRICO (mm)"));
    lay2->setWidget(0, QFormLayout::ItemRole(1), units_value);
    start_label = new QLabel(nullptr);
    start_label->setText(QString::fromUtf8("Primer punto:"));
    start_label->setToolTip(QString::fromUtf8("Este es el primer objeto de coordenadas de puntos.\nEste es el punto de partida para medir la distancia."));
    lay2->setWidget(1, QFormLayout::ItemRole(0), start_label);
    start_entry = new FCEntry(nullptr);
    start_entry->setProperty("alignment", 130);
    start_entry->setReadOnly(true);
    start_entry->setText(QString::fromUtf8("(0, 0)"));
    start_entry->setToolTip(QString::fromUtf8("Este es el primer objeto de coordenadas de puntos.\nEste es el punto de partida para medir la distancia."));
    lay2->setWidget(1, QFormLayout::ItemRole(1), start_entry);
    stop_label = new QLabel(nullptr);
    stop_label->setText(QString::fromUtf8("Segundo punto:"));
    stop_label->setToolTip(QString::fromUtf8("Este es el segundo objeto de coordenadas de puntos.\nEste es el punto final para medir la distancia."));
    lay2->setWidget(2, QFormLayout::ItemRole(0), stop_label);
    stop_entry = new FCEntry(nullptr);
    stop_entry->setProperty("alignment", 130);
    stop_entry->setReadOnly(true);
    stop_entry->setText(QString::fromUtf8("(0, 0)"));
    stop_entry->setToolTip(QString::fromUtf8("Este es el segundo objeto de coordenadas de puntos.\nEste es el punto final para medir la distancia."));
    lay2->setWidget(2, QFormLayout::ItemRole(1), stop_entry);
    distance_x_label = new QLabel(nullptr);
    distance_x_label->setText(QString::fromUtf8("Dx:"));
    distance_x_label->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje X."));
    lay2->setWidget(3, QFormLayout::ItemRole(0), distance_x_label);
    distance_x_entry = new FCEntry(nullptr);
    distance_x_entry->setProperty("alignment", 130);
    distance_x_entry->setReadOnly(true);
    distance_x_entry->setText(QString::fromUtf8("0.0"));
    distance_x_entry->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje X."));
    lay2->setWidget(3, QFormLayout::ItemRole(1), distance_x_entry);
    distance_y_label = new QLabel(nullptr);
    distance_y_label->setText(QString::fromUtf8("Dy:"));
    distance_y_label->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje Y."));
    lay2->setWidget(4, QFormLayout::ItemRole(0), distance_y_label);
    distance_y_entry = new FCEntry(nullptr);
    distance_y_entry->setProperty("alignment", 130);
    distance_y_entry->setReadOnly(true);
    distance_y_entry->setText(QString::fromUtf8("0.0"));
    distance_y_entry->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje Y."));
    lay2->setWidget(4, QFormLayout::ItemRole(1), distance_y_entry);
    angle_label = new QLabel(nullptr);
    angle_label->setText(QString::fromUtf8("Ángulo:"));
    angle_label->setToolTip(QString::fromUtf8("Este es el ángulo de orientación de la línea de medición."));
    lay2->setWidget(5, QFormLayout::ItemRole(0), angle_label);
    angle_entry = new FCEntry(nullptr);
    angle_entry->setProperty("alignment", 130);
    angle_entry->setReadOnly(true);
    angle_entry->setText(QString::fromUtf8("0.0"));
    angle_entry->setToolTip(QString::fromUtf8("Este es el ángulo de orientación de la línea de medición."));
    lay2->setWidget(5, QFormLayout::ItemRole(1), angle_entry);
    total_distance_label = new QLabel(nullptr);
    total_distance_label->setText(QString::fromUtf8("<b>DISTANCIA:</b>"));
    total_distance_label->setToolTip(QString::fromUtf8("Este es el punto a punto de la distancia euclidiana."));
    lay2->setWidget(6, QFormLayout::ItemRole(0), total_distance_label);
    total_distance_entry = new FCEntry(nullptr);
    total_distance_entry->setProperty("alignment", 130);
    total_distance_entry->setReadOnly(true);
    total_distance_entry->setText(QString::fromUtf8("0.0"));
    total_distance_entry->setToolTip(QString::fromUtf8("Este es el punto a punto de la distancia euclidiana."));
    lay2->setWidget(6, QFormLayout::ItemRole(1), total_distance_entry);
    half_point_label = new QLabel(nullptr);
    half_point_label->setText(QString::fromUtf8("<b>Punto Medio:</b>"));
    half_point_label->setToolTip(QString::fromUtf8("Este es el punto medio de la distancia euclidiana punto a punto."));
    lay2->setWidget(7, QFormLayout::ItemRole(0), half_point_label);
    half_point_entry = new FCEntry(nullptr);
    half_point_entry->setProperty("alignment", 130);
    half_point_entry->setReadOnly(true);
    half_point_entry->setText(QString::fromUtf8("(0, 0)"));
    half_point_entry->setToolTip(QString::fromUtf8("Este es el punto medio de la distancia euclidiana punto a punto."));
    lay2->setWidget(7, QFormLayout::ItemRole(1), half_point_entry);
    layout->addLayout(lay2, 0);
    measure_btn = new QPushButton(nullptr);
    measure_btn->setText(QString::fromUtf8("Medida"));
    layout->addWidget(measure_btn, 0);
    jump_hp_btn = new QPushButton(nullptr);
    jump_hp_btn->setEnabled(false);
    jump_hp_btn->setText(QString::fromUtf8("Saltar a Medio Punto"));
    layout->addWidget(jump_hp_btn, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

}  // namespace flatcam::gui
