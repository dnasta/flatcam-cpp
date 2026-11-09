// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DistanceToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void DistanceToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    w1->setText(QString::fromUtf8("<font size=4><b>Herramienta de Dist</b></font><br>"));
    layout->addWidget(w1, 0);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    units_label = new FCLabel(nullptr);
    units_label->setText(QString::fromUtf8("Unidades:"));
    units_label->setToolTip(QString::fromUtf8("Esas son las unidades en las que se mide la distancia."));
    lay2->addWidget(units_label, 0, 0, 1, 1);
    units_value = new FCLabel(nullptr);
    units_value->setEnabled(false);
    units_value->setText(QString::fromUtf8("MÉTRICO (mm)"));
    lay2->addWidget(units_value, 0, 1, 1, 1);
    snap_center_cb = new FCCheckBox(nullptr);
    snap_center_cb->setText(QString::fromUtf8("Ajustar al centro"));
    snap_center_cb->setToolTip(QString::fromUtf8("El cursor del mouse se ajustará al centro de la almohadilla / taladro\ncuando se cierne sobre la geometría de la almohadilla / taladro."));
    snap_center_cb->setHidden(true);
    lay2->addWidget(snap_center_cb, 1, 0, 1, 2);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->addWidget(w3, 2, 0, 1, 2);
    start_label = new FCLabel(nullptr);
    start_label->setText(QString::fromUtf8("Iniciar coordenadas:"));
    start_label->setToolTip(QString::fromUtf8("Esto mide las coordenadas del punto de inicio."));
    lay2->addWidget(start_label, 3, 0, 1, 1);
    start_entry = new FCEntry(nullptr);
    start_entry->setProperty("alignment", 130);
    start_entry->setReadOnly(true);
    start_entry->setText(QString::fromUtf8("(0, 0)"));
    start_entry->setToolTip(QString::fromUtf8("Esto mide las coordenadas del punto de inicio."));
    lay2->addWidget(start_entry, 3, 1, 1, 1);
    stop_label = new FCLabel(nullptr);
    stop_label->setText(QString::fromUtf8("Detener coordenadas:"));
    stop_label->setToolTip(QString::fromUtf8("Estas son las coordenadas del punto de parada de medición."));
    lay2->addWidget(stop_label, 4, 0, 1, 1);
    stop_entry = new FCEntry(nullptr);
    stop_entry->setProperty("alignment", 130);
    stop_entry->setReadOnly(true);
    stop_entry->setText(QString::fromUtf8("(0, 0)"));
    stop_entry->setToolTip(QString::fromUtf8("Estas son las coordenadas del punto de parada de medición."));
    lay2->addWidget(stop_entry, 4, 1, 1, 1);
    distance_x_label = new FCLabel(nullptr);
    distance_x_label->setText(QString::fromUtf8("Dx:"));
    distance_x_label->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje X."));
    lay2->addWidget(distance_x_label, 5, 0, 1, 1);
    distance_x_entry = new FCEntry(nullptr);
    distance_x_entry->setProperty("alignment", 130);
    distance_x_entry->setReadOnly(true);
    distance_x_entry->setText(QString::fromUtf8("0.0"));
    distance_x_entry->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje X."));
    lay2->addWidget(distance_x_entry, 5, 1, 1, 1);
    distance_y_label = new FCLabel(nullptr);
    distance_y_label->setText(QString::fromUtf8("Dy:"));
    distance_y_label->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje Y."));
    lay2->addWidget(distance_y_label, 6, 0, 1, 1);
    distance_y_entry = new FCEntry(nullptr);
    distance_y_entry->setProperty("alignment", 130);
    distance_y_entry->setReadOnly(true);
    distance_y_entry->setText(QString::fromUtf8("0.0"));
    distance_y_entry->setToolTip(QString::fromUtf8("Esta es la distancia medida sobre el eje Y."));
    lay2->addWidget(distance_y_entry, 6, 1, 1, 1);
    angle_label = new FCLabel(nullptr);
    angle_label->setText(QString::fromUtf8("Ángulo:"));
    angle_label->setToolTip(QString::fromUtf8("Este es el ángulo de orientación de la línea de medición."));
    lay2->addWidget(angle_label, 7, 0, 1, 1);
    angle_entry = new FCEntry(nullptr);
    angle_entry->setProperty("alignment", 130);
    angle_entry->setReadOnly(true);
    angle_entry->setText(QString::fromUtf8("0.0"));
    angle_entry->setToolTip(QString::fromUtf8("Este es el ángulo de orientación de la línea de medición."));
    lay2->addWidget(angle_entry, 7, 1, 1, 1);
    total_distance_label = new FCLabel(nullptr);
    total_distance_label->setText(QString::fromUtf8("<b>DISTANCIA:</b>"));
    total_distance_label->setToolTip(QString::fromUtf8("Este es el punto a punto de la distancia euclidiana."));
    lay2->addWidget(total_distance_label, 8, 0, 1, 1);
    total_distance_entry = new FCEntry(nullptr);
    total_distance_entry->setProperty("alignment", 130);
    total_distance_entry->setReadOnly(true);
    total_distance_entry->setText(QString::fromUtf8("0.0"));
    total_distance_entry->setToolTip(QString::fromUtf8("Este es el punto a punto de la distancia euclidiana."));
    lay2->addWidget(total_distance_entry, 8, 1, 1, 1);
    lay2->setColumnStretch(1, 1);
    layout->addLayout(lay2, 0);
    measure_btn = new FCButton(nullptr);
    measure_btn->setEnabled(false);
    measure_btn->setText(QString::fromUtf8("Trabajando..."));
    layout->addWidget(measure_btn, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> DistanceToolUIBase::membersByName() const {
    return {
        {"angle_entry", angle_entry},
        {"angle_label", angle_label},
        {"distance_x_entry", distance_x_entry},
        {"distance_x_label", distance_x_label},
        {"distance_y_entry", distance_y_entry},
        {"distance_y_label", distance_y_label},
        {"layout", layout},
        {"measure_btn", measure_btn},
        {"snap_center_cb", snap_center_cb},
        {"start_entry", start_entry},
        {"start_label", start_label},
        {"stop_entry", stop_entry},
        {"stop_label", stop_label},
        {"total_distance_entry", total_distance_entry},
        {"total_distance_label", total_distance_label},
        {"units_label", units_label},
        {"units_value", units_value},
    };
}

}  // namespace flatcam::gui
