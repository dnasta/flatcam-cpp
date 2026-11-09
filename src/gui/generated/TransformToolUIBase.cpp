// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/TransformToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void TransformToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Transform. de objetos"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Transform. de objetos"));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    auto* w4 = new FCLabel(nullptr);
    lay3->addWidget(w4, 0, 0, 1, 1);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(QString::fromUtf8("Referencia:"));
    w5->setToolTip(QString::fromUtf8("El punto de referencia para Rotate, Skew, Scale, Mirror.\nPuede ser:\n- Origen -> es el punto 0, 0\n- Selección -> el centro del cuadro delimitador de los objetos seleccionados\n- Punto -> un punto personalizado definido por coordenadas X, Y\n- Objeto -> el centro del cuadro delimitador de un objeto específico"));
    lay3->addWidget(w5, 0, 0, 1, 1);
    ref_combo = new FCComboBox(nullptr);
    ref_combo->setProperty("currentText", QString::fromUtf8("Selección"));
    ref_combo->addItem(QString::fromUtf8("Origen"));
    ref_combo->addItem(QString::fromUtf8("Selección"));
    ref_combo->addItem(QString::fromUtf8("Punto"));
    ref_combo->addItem(QString::fromUtf8("Objeto"));
    ref_combo->setCurrentIndex(1);
    lay3->addWidget(ref_combo, 0, 1, 1, 2);
    point_label = new FCLabel(nullptr);
    point_label->setText(QString::fromUtf8("Valor:"));
    point_label->setToolTip(QString::fromUtf8("Un punto de referencia en formato X, Y."));
    point_label->setHidden(true);
    lay3->addWidget(point_label, 1, 0, 1, 1);
    point_entry = new NumericalEvalTupleEntry(nullptr);
    point_entry->setText(QString::fromUtf8("0, 0"));
    point_entry->setHidden(true);
    lay3->addWidget(point_entry, 1, 1, 1, 2);
    point_button = new FCButton(nullptr);
    point_button->setText(QString::fromUtf8("Añadir"));
    point_button->setToolTip(QString::fromUtf8("Agregar coordenadas de puntos desde el portapapeles."));
    point_button->setHidden(true);
    lay3->addWidget(point_button, 2, 0, 1, 3);
    type_object_label = new FCLabel(nullptr);
    type_object_label->setText(QString::fromUtf8("Tipo:"));
    type_object_label->setToolTip(QString::fromUtf8("El tipo de objeto utilizado como referencia."));
    type_object_label->setHidden(true);
    lay3->addWidget(type_object_label, 3, 0, 1, 1);
    type_obj_combo = new FCComboBox(nullptr);
    type_obj_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    type_obj_combo->setHidden(true);
    type_obj_combo->addItem(QString::fromUtf8("Gerber"));
    type_obj_combo->addItem(QString::fromUtf8("Excellon"));
    type_obj_combo->addItem(QString::fromUtf8("Geometría"));
    lay3->addWidget(type_obj_combo, 3, 1, 1, 2);
    object_combo = new FCComboBox(nullptr);
    object_combo->setToolTip(QString::fromUtf8("El objeto utilizado como referencia.\nEl punto utilizado es el centro de su cuadro delimitador."));
    object_combo->setHidden(true);
    lay3->addWidget(object_combo, 4, 0, 1, 3);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay3->addWidget(w6, 5, 0, 1, 3);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(QString::fromUtf8("<font size=3><b>Girar</b></font>"));
    lay3->addWidget(w7, 6, 0, 1, 3);
    rotate_label = new FCLabel(nullptr);
    rotate_label->setText(QString::fromUtf8("Ángulo:"));
    rotate_label->setToolTip(QString::fromUtf8("Ángulo, en grados.\nNúmero Real entre -360 y 359.\nNúmeros positivos para el movimiento CW.\nNúmeros negativos para movimiento CCW."));
    lay3->addWidget(rotate_label, 7, 0, 1, 1);
    rotate_entry = new FCDoubleSpinner(nullptr);
    rotate_entry->setProperty("decimals", 4);
    rotate_entry->setProperty("maximum", 360.0);
    rotate_entry->setProperty("minimum", -360.0);
    rotate_entry->setProperty("singleStep", 45.0);
    rotate_entry->setProperty("value", 90.0);
    rotate_entry->setWrapping(true);
    lay3->addWidget(rotate_entry, 7, 1, 1, 1);
    rotate_button = new FCButton(nullptr);
    rotate_button->setMinimumSize(QSize(90, 0));
    rotate_button->setText(QString::fromUtf8("Girar"));
    rotate_button->setToolTip(QString::fromUtf8("Rotar los objetos seleccionados.\nEl punto de referencia es el medio de\nel cuadro delimitador para todos los objetos seleccionados."));
    lay3->addWidget(rotate_button, 7, 2, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay3->addWidget(w8, 8, 0, 1, 3);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(QString::fromUtf8("<font size=3><b>Sesgo / cizalla</b></font>"));
    lay3->addWidget(w9, 9, 0, 1, 2);
    skew_link_cb = new FCCheckBox(nullptr);
    skew_link_cb->setChecked(true);
    skew_link_cb->setText(QString::fromUtf8("Enlazar"));
    skew_link_cb->setToolTip(QString::fromUtf8("Enlace la entrada Y a la entrada X y copie su contenido."));
    lay3->addWidget(skew_link_cb, 9, 2, 1, 1);
    skewx_label = new FCLabel(nullptr);
    skewx_label->setText(QString::fromUtf8("Ángulo X:"));
    skewx_label->setToolTip(QString::fromUtf8("Ángulo para sesgo de acción, en grados.\nNúmero Real entre -360 y 360."));
    lay3->addWidget(skewx_label, 10, 0, 1, 1);
    skewx_entry = new FCDoubleSpinner(nullptr);
    skewx_entry->setProperty("decimals", 4);
    skewx_entry->setProperty("maximum", 360.0);
    skewx_entry->setProperty("minimum", -360.0);
    lay3->addWidget(skewx_entry, 10, 1, 1, 1);
    skewx_button = new FCButton(nullptr);
    skewx_button->setMinimumSize(QSize(90, 0));
    skewx_button->setText(QString::fromUtf8("Inclinar X"));
    skewx_button->setToolTip(QString::fromUtf8("Incline / corte los objetos seleccionados.\nEl punto de referencia es el medio de\nel cuadro delimitador para todos los objetos seleccionados."));
    lay3->addWidget(skewx_button, 10, 2, 1, 1);
    skewy_label = new FCLabel(nullptr);
    skewy_label->setEnabled(false);
    skewy_label->setText(QString::fromUtf8("Ángulo Y:"));
    skewy_label->setToolTip(QString::fromUtf8("Ángulo para sesgo de acción, en grados.\nNúmero Real entre -360 y 360."));
    lay3->addWidget(skewy_label, 12, 0, 1, 1);
    skewy_entry = new FCDoubleSpinner(nullptr);
    skewy_entry->setProperty("decimals", 4);
    skewy_entry->setEnabled(false);
    skewy_entry->setProperty("maximum", 360.0);
    skewy_entry->setProperty("minimum", -360.0);
    lay3->addWidget(skewy_entry, 12, 1, 1, 1);
    skewy_button = new FCButton(nullptr);
    skewy_button->setEnabled(false);
    skewy_button->setMinimumSize(QSize(90, 0));
    skewy_button->setText(QString::fromUtf8("Inclinar Y"));
    skewy_button->setToolTip(QString::fromUtf8("Incline / corte los objetos seleccionados.\nEl punto de referencia es el medio de\nel cuadro delimitador para todos los objetos seleccionados."));
    lay3->addWidget(skewy_button, 12, 2, 1, 1);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay3->addWidget(w10, 14, 0, 1, 3);
    auto* w11 = new FCLabel(nullptr);
    w11->setText(QString::fromUtf8("<font size=3><b>Escala</b></font>"));
    lay3->addWidget(w11, 15, 0, 1, 2);
    scale_link_cb = new FCCheckBox(nullptr);
    scale_link_cb->setChecked(true);
    scale_link_cb->setText(QString::fromUtf8("Enlazar"));
    scale_link_cb->setToolTip(QString::fromUtf8("Enlace la entrada Y a la entrada X y copie su contenido."));
    lay3->addWidget(scale_link_cb, 15, 2, 1, 1);
    scalex_label = new FCLabel(nullptr);
    scalex_label->setText(QString::fromUtf8("Factor X:"));
    scalex_label->setToolTip(QString::fromUtf8("Factor de escalado en eje X."));
    lay3->addWidget(scalex_label, 17, 0, 1, 1);
    scalex_entry = new FCDoubleSpinner(nullptr);
    scalex_entry->setProperty("decimals", 4);
    scalex_entry->setProperty("minimum", -1000000.0);
    scalex_entry->setProperty("value", 1.0);
    lay3->addWidget(scalex_entry, 17, 1, 1, 1);
    scalex_button = new FCButton(nullptr);
    scalex_button->setMinimumSize(QSize(90, 0));
    scalex_button->setText(QString::fromUtf8("Escala x"));
    scalex_button->setToolTip(QString::fromUtf8("Escala los objetos seleccionados.\nEl punto de referencia depende de\nel estado de la casilla de verificación Escalar referencia."));
    lay3->addWidget(scalex_button, 17, 2, 1, 1);
    scaley_label = new FCLabel(nullptr);
    scaley_label->setEnabled(false);
    scaley_label->setText(QString::fromUtf8("Factor Y:"));
    scaley_label->setToolTip(QString::fromUtf8("Factor de escalado en eje Y."));
    lay3->addWidget(scaley_label, 19, 0, 1, 1);
    scaley_entry = new FCDoubleSpinner(nullptr);
    scaley_entry->setProperty("decimals", 4);
    scaley_entry->setEnabled(false);
    scaley_entry->setProperty("minimum", -1000000.0);
    scaley_entry->setProperty("value", 1.0);
    lay3->addWidget(scaley_entry, 19, 1, 1, 1);
    scaley_button = new FCButton(nullptr);
    scaley_button->setEnabled(false);
    scaley_button->setMinimumSize(QSize(90, 0));
    scaley_button->setText(QString::fromUtf8("Escala Y"));
    scaley_button->setToolTip(QString::fromUtf8("Escala los objetos seleccionados.\nEl punto de referencia depende de\nel estado de la casilla de verificación Escalar referencia."));
    lay3->addWidget(scaley_button, 19, 2, 1, 1);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    lay3->addWidget(w12, 21, 0, 1, 3);
    auto* w13 = new FCLabel(nullptr);
    w13->setText(QString::fromUtf8("<font size=3><b>Espejo (Flip)</b></font>"));
    lay3->addWidget(w13, 23, 0, 1, 3);
    auto* lay14 = new QHBoxLayout();
    lay14->setContentsMargins(0, 0, 0, 0);
    lay14->setSpacing(6);
    flipx_button = new FCButton(nullptr);
    flipx_button->setText(QString::fromUtf8("Voltear en X"));
    flipx_button->setToolTip(QString::fromUtf8("Voltee los objetos seleccionados sobre el eje X."));
    lay14->addWidget(flipx_button, 0);
    flipy_button = new FCButton(nullptr);
    flipy_button->setText(QString::fromUtf8("Voltear en Y"));
    flipy_button->setToolTip(QString::fromUtf8("Voltee los objetos seleccionados sobre el eje X."));
    lay14->addWidget(flipy_button, 0);
    lay3->addLayout(lay14, 25, 0, 1, 3);
    auto* w15 = new QFrame(nullptr);
    w15->setProperty("frameShadow", 48);
    w15->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w15->setSizePolicy(sp); }
    lay3->addWidget(w15, 27, 0, 1, 3);
    auto* w16 = new FCLabel(nullptr);
    w16->setText(QString::fromUtf8("<font size=3><b>Compensar</b></font>"));
    lay3->addWidget(w16, 29, 0, 1, 3);
    offx_label = new FCLabel(nullptr);
    offx_label->setText(QString::fromUtf8("Valor X:"));
    offx_label->setToolTip(QString::fromUtf8("Distancia a desplazamiento en el eje X. En unidades actuales."));
    lay3->addWidget(offx_label, 31, 0, 1, 1);
    offx_entry = new FCDoubleSpinner(nullptr);
    offx_entry->setProperty("decimals", 4);
    offx_entry->setProperty("minimum", -1000000.0);
    lay3->addWidget(offx_entry, 31, 1, 1, 1);
    offx_button = new FCButton(nullptr);
    offx_button->setMinimumSize(QSize(90, 0));
    offx_button->setText(QString::fromUtf8("Offset X"));
    offx_button->setToolTip(QString::fromUtf8("Desplazar los objetos seleccionados.\nEl punto de referencia es el medio de\nel cuadro delimitador para todos los objetos seleccionados.\n"));
    lay3->addWidget(offx_button, 31, 2, 1, 1);
    offy_label = new FCLabel(nullptr);
    offy_label->setText(QString::fromUtf8("Valor Y:"));
    offy_label->setToolTip(QString::fromUtf8("Distancia a desplazamiento en el eje Y. En unidades actuales."));
    lay3->addWidget(offy_label, 32, 0, 1, 1);
    offy_entry = new FCDoubleSpinner(nullptr);
    offy_entry->setProperty("decimals", 4);
    offy_entry->setProperty("minimum", -1000000.0);
    lay3->addWidget(offy_entry, 32, 1, 1, 1);
    offy_button = new FCButton(nullptr);
    offy_button->setMinimumSize(QSize(90, 0));
    offy_button->setText(QString::fromUtf8("Offset Y"));
    offy_button->setToolTip(QString::fromUtf8("Desplazar los objetos seleccionados.\nEl punto de referencia es el medio de\nel cuadro delimitador para todos los objetos seleccionados.\n"));
    lay3->addWidget(offy_button, 32, 2, 1, 1);
    auto* w17 = new QFrame(nullptr);
    w17->setProperty("frameShadow", 48);
    w17->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w17->setSizePolicy(sp); }
    lay3->addWidget(w17, 34, 0, 1, 3);
    auto* w18 = new FCLabel(nullptr);
    w18->setText(QString::fromUtf8("<font size=3><b>Buffer</b></font>"));
    lay3->addWidget(w18, 35, 0, 1, 2);
    buffer_rounded_cb = new FCCheckBox(nullptr);
    buffer_rounded_cb->setChecked(true);
    buffer_rounded_cb->setText(QString::fromUtf8("Redondeado"));
    buffer_rounded_cb->setToolTip(QString::fromUtf8("Si se marca, el búfer rodeará la forma tamponada,\nCada rincón será redondeado.\nSi no está marcado, el búfer seguirá la geometría exacta\nde la forma amortiguada."));
    lay3->addWidget(buffer_rounded_cb, 35, 2, 1, 1);
    buffer_label = new FCLabel(nullptr);
    buffer_label->setText(QString::fromUtf8("Distancia:"));
    buffer_label->setToolTip(QString::fromUtf8("Un valor positivo creará el efecto de dilatación,\nmientras que un valor negativo creará el efecto de la erosión.\nCada elemento de geometría del objeto se incrementará\no disminuido con la 'distancia'."));
    lay3->addWidget(buffer_label, 37, 0, 1, 1);
    buffer_entry = new FCDoubleSpinner(nullptr);
    buffer_entry->setProperty("decimals", 4);
    buffer_entry->setProperty("maximum", 10000.0);
    buffer_entry->setProperty("minimum", -10000.0);
    buffer_entry->setProperty("singleStep", 0.1);
    buffer_entry->setWrapping(true);
    lay3->addWidget(buffer_entry, 37, 1, 1, 1);
    buffer_button = new FCButton(nullptr);
    buffer_button->setMinimumSize(QSize(90, 0));
    buffer_button->setText(QString::fromUtf8("Buffer D"));
    buffer_button->setToolTip(QString::fromUtf8("Crea el efecto de amortiguación en cada geometría,\nelemento del objeto seleccionado, utilizando la distancia."));
    lay3->addWidget(buffer_button, 37, 2, 1, 1);
    buffer_factor_label = new FCLabel(nullptr);
    buffer_factor_label->setText(QString::fromUtf8("Valor:"));
    buffer_factor_label->setToolTip(QString::fromUtf8("Un valor positivo creará el efecto de dilatación,\nmientras que un valor negativo creará el efecto de la erosión.\nCada elemento de geometría del objeto se incrementará\no disminuido para ajustarse al 'Valor'. El Valor es un porcentaje\nde la dimensión inicial."));
    lay3->addWidget(buffer_factor_label, 38, 0, 1, 1);
    buffer_factor_entry = new FCDoubleSpinner(nullptr);
    buffer_factor_entry->setProperty("decimals", 4);
    buffer_factor_entry->setProperty("maximum", 1000.0);
    buffer_factor_entry->setProperty("minimum", -100.0);
    buffer_factor_entry->setProperty("suffix", QString::fromUtf8(" %"));
    buffer_factor_entry->setProperty("value", 100.0);
    buffer_factor_entry->setWrapping(true);
    lay3->addWidget(buffer_factor_entry, 38, 1, 1, 1);
    buffer_factor_button = new FCButton(nullptr);
    buffer_factor_button->setMinimumSize(QSize(90, 0));
    buffer_factor_button->setText(QString::fromUtf8("Buffer F"));
    buffer_factor_button->setToolTip(QString::fromUtf8("Crea el efecto de amortiguación en cada geometría,\nelemento del objeto seleccionado, utilizando el factor."));
    lay3->addWidget(buffer_factor_button, 38, 2, 1, 1);
    auto* w19 = new FCLabel(nullptr);
    lay3->addWidget(w19, 42, 0, 1, 3);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
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

std::map<std::string, QObject*> TransformToolUIBase::membersByName() const {
    return {
        {"buffer_button", buffer_button},
        {"buffer_entry", buffer_entry},
        {"buffer_factor_button", buffer_factor_button},
        {"buffer_factor_entry", buffer_factor_entry},
        {"buffer_factor_label", buffer_factor_label},
        {"buffer_label", buffer_label},
        {"buffer_rounded_cb", buffer_rounded_cb},
        {"flipx_button", flipx_button},
        {"flipy_button", flipy_button},
        {"layout", layout},
        {"object_combo", object_combo},
        {"offx_button", offx_button},
        {"offx_entry", offx_entry},
        {"offx_label", offx_label},
        {"offy_button", offy_button},
        {"offy_entry", offy_entry},
        {"offy_label", offy_label},
        {"point_button", point_button},
        {"point_entry", point_entry},
        {"point_label", point_label},
        {"ref_combo", ref_combo},
        {"reset_button", reset_button},
        {"rotate_button", rotate_button},
        {"rotate_entry", rotate_entry},
        {"rotate_label", rotate_label},
        {"scale_link_cb", scale_link_cb},
        {"scalex_button", scalex_button},
        {"scalex_entry", scalex_entry},
        {"scalex_label", scalex_label},
        {"scaley_button", scaley_button},
        {"scaley_entry", scaley_entry},
        {"scaley_label", scaley_label},
        {"skew_link_cb", skew_link_cb},
        {"skewx_button", skewx_button},
        {"skewx_entry", skewx_entry},
        {"skewx_label", skewx_label},
        {"skewy_button", skewy_button},
        {"skewy_entry", skewy_entry},
        {"skewy_label", skewy_label},
        {"type_obj_combo", type_obj_combo},
        {"type_object_label", type_object_label},
    };
}

}  // namespace flatcam::gui
