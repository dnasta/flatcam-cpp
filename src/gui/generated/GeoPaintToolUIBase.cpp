// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GeoPaintToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void GeoPaintToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de pintura"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(11, 11, 11, 11);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de pintura"));
    layout->addWidget(w1, 0);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    auto* w3 = new FCLabel(nullptr);
    w3->setText(QString::fromUtf8("Diá. de Herram:"));
    w3->setToolTip(QString::fromUtf8("Diámetro de la herramienta a utilizar en la operación."));
    lay2->addWidget(w3, 0, 0, 1, 1);
    painttooldia_entry = new FCDoubleSpinner(nullptr);
    painttooldia_entry->setProperty("decimals", 4);
    painttooldia_entry->setProperty("maximum", 10000.0);
    painttooldia_entry->setProperty("minimum", -10000.0);
    painttooldia_entry->setProperty("value", 0.3);
    lay2->addWidget(painttooldia_entry, 0, 1, 1, 1);
    auto* w4 = new FCLabel(nullptr);
    w4->setText(QString::fromUtf8("Superposición:"));
    w4->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta.\nAjustar el valor comenzando con valores más bajos\ny aumentarlo si las áreas que deben procesarse aún están\nno procesado.Valores más bajos = procesamiento más rápido, ejecución más rápida en CNC.\nValores más altos = procesamiento lento y ejecución lenta en CNC\ndebido a demasiados caminos."));
    lay2->addWidget(w4, 1, 0, 1, 1);
    paintoverlap_entry = new FCDoubleSpinner(nullptr);
    paintoverlap_entry->setProperty("decimals", 4);
    paintoverlap_entry->setProperty("maximum", 99.9999);
    paintoverlap_entry->setProperty("suffix", QString::fromUtf8(" %"));
    paintoverlap_entry->setProperty("value", 20.0);
    paintoverlap_entry->setWrapping(true);
    lay2->addWidget(paintoverlap_entry, 1, 1, 1, 1);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(QString::fromUtf8("Margen:"));
    w5->setToolTip(QString::fromUtf8("Distancia por la cual evitar\nlos bordes del polígono a\nser pintado."));
    lay2->addWidget(w5, 2, 0, 1, 1);
    paintmargin_entry = new FCDoubleSpinner(nullptr);
    paintmargin_entry->setProperty("decimals", 4);
    paintmargin_entry->setProperty("maximum", 10000.0);
    paintmargin_entry->setProperty("minimum", -10000.0);
    lay2->addWidget(paintmargin_entry, 2, 1, 1, 1);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(QString::fromUtf8("Método:"));
    w6->setToolTip(QString::fromUtf8("Algoritmo para pintar los polígonos:\n- Estándar: paso fijo hacia adentro.\n- Basado en semillas: hacia afuera de la semilla.\n- Basado en líneas: líneas paralelas."));
    lay2->addWidget(w6, 3, 0, 1, 1);
    paintmethod_combo = new FCComboBox(nullptr);
    paintmethod_combo->setProperty("currentText", QString::fromUtf8("Semilla"));
    paintmethod_combo->addItem(QString::fromUtf8("Estándar"));
    paintmethod_combo->addItem(QString::fromUtf8("Semilla"));
    paintmethod_combo->addItem(QString::fromUtf8("Líneas"));
    paintmethod_combo->setCurrentIndex(1);
    lay2->addWidget(paintmethod_combo, 3, 1, 1, 1);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(QString::fromUtf8("Conectar:"));
    w7->setToolTip(QString::fromUtf8("Dibuja líneas entre el resultado\nSegmentos para minimizar elevaciones de herramientas."));
    lay2->addWidget(w7, 4, 0, 1, 1);
    pathconnect_cb = new FCCheckBox(nullptr);
    pathconnect_cb->setChecked(true);
    lay2->addWidget(pathconnect_cb, 4, 1, 1, 1);
    auto* w8 = new FCLabel(nullptr);
    w8->setText(QString::fromUtf8("Contorno:"));
    w8->setToolTip(QString::fromUtf8("Corta todo el perímetro del polígono.\nPara recortar los bordes ásperos."));
    lay2->addWidget(w8, 5, 0, 1, 1);
    paintcontour_cb = new FCCheckBox(nullptr);
    paintcontour_cb->setChecked(true);
    lay2->addWidget(paintcontour_cb, 5, 1, 1, 1);
    lay2->setColumnStretch(1, 1);
    layout->addLayout(lay2, 0);
    auto* lay9 = new QHBoxLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setSpacing(6);
    paint_button = new FCButton(nullptr);
    paint_button->setText(QString::fromUtf8("Pintar"));
    lay9->addWidget(paint_button, 0);
    layout->addLayout(lay9, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> GeoPaintToolUIBase::membersByName() const {
    return {
        {"layout", layout},
        {"paint_button", paint_button},
        {"paintcontour_cb", paintcontour_cb},
        {"paintmargin_entry", paintmargin_entry},
        {"paintmethod_combo", paintmethod_combo},
        {"paintoverlap_entry", paintoverlap_entry},
        {"painttooldia_entry", painttooldia_entry},
        {"pathconnect_cb", pathconnect_cb},
    };
}

}  // namespace flatcam::gui
