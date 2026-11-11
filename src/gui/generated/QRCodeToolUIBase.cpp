// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/QRCodeToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void QRCodeToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta QRCode"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta QRCode"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    grbobj_label = new QLabel(nullptr);
    grbobj_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    grbobj_label->setToolTip(QString::fromUtf8("Objeto Gerber al que se agregará el QRCode."));
    lay3->addWidget(grbobj_label, 0, 0, 1, 1);
    grb_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(grb_object_combo, 1, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 2, 0, 1, 2);
    text_label = new QLabel(nullptr);
    text_label->setText(QString::fromUtf8("<b>Datos de QRCode</b>:"));
    text_label->setToolTip(QString::fromUtf8("Datos de QRCode. Texto alfanumérico a codificar en el Código QR."));
    lay3->addWidget(text_label, 5, 0, 1, 1);
    text_data = new FCTextArea(nullptr);
    text_data->setProperty("placeholderText", QString::fromUtf8("Agregue aquí el texto que se incluirá en el QRCode ..."));
    lay3->addWidget(text_data, 6, 0, 1, 2);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay3->addWidget(w5, 7, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    auto* lay6 = new QGridLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setHorizontalSpacing(6);
    lay6->setVerticalSpacing(6);
    qrcode_label = new QLabel(nullptr);
    qrcode_label->setText(QString::fromUtf8("<b>Parámetros</b>"));
    qrcode_label->setToolTip(QString::fromUtf8("Los parámetros utilizados para dar forma al QRCode."));
    lay6->addWidget(qrcode_label, 0, 0, 1, 2);
    version_label = new QLabel(nullptr);
    version_label->setText(QString::fromUtf8("Versión:"));
    version_label->setToolTip(QString::fromUtf8("La versión de QRCode puede tener valores de 1 (21x21 elementos)\na 40 (177x177 elementos)."));
    lay6->addWidget(version_label, 1, 0, 1, 1);
    version_entry = new FCSpinner(nullptr);
    version_entry->setProperty("maximum", 40);
    version_entry->setProperty("minimum", 1);
    version_entry->setProperty("value", 1);
    version_entry->setWrapping(true);
    lay6->addWidget(version_entry, 1, 1, 1, 1);
    error_label = new QLabel(nullptr);
    error_label->setText(QString::fromUtf8("Corrección de error:"));
    error_label->setToolTip(QString::fromUtf8("Parámetro que controla la corrección de errores utilizada para el código QR.\nL = máximo 7 %% de errores pueden ser corregidos\nM = máximo 15%% de errores pueden ser corregidos\nQ = se puede corregir un máximo de 25%% de errores\nH = máximo 30 %% de errores pueden ser corregidos."));
    lay6->addWidget(error_label, 2, 0, 1, 1);
    error_radio = new RadioSet({{QString::fromUtf8("L"), QVariant(QString::fromUtf8("L"))}, {QString::fromUtf8("M"), QVariant(QString::fromUtf8("M"))}, {QString::fromUtf8("Q"), QVariant(QString::fromUtf8("Q"))}, {QString::fromUtf8("H"), QVariant(QString::fromUtf8("H"))}}, Qt::Horizontal, true, nullptr);
    error_radio->setToolTip(QString::fromUtf8("Parámetro que controla la corrección de errores utilizada para el código QR.\nL = máximo 7 %% de errores pueden ser corregidos\nM = máximo 15%% de errores pueden ser corregidos\nQ = se puede corregir un máximo de 25%% de errores\nH = máximo 30 %% de errores pueden ser corregidos."));
    error_radio->setValue(QVariant(QString::fromUtf8("L")));
    lay6->addWidget(error_radio, 2, 1, 1, 1);
    bsize_label = new QLabel(nullptr);
    bsize_label->setText(QString::fromUtf8("Tamaño de Elementos:"));
    bsize_label->setToolTip(QString::fromUtf8("El tamaño del elemento controla el tamaño general del código QR\najustando el tamaño de cada cuadro en el código."));
    lay6->addWidget(bsize_label, 3, 0, 1, 1);
    bsize_entry = new FCSpinner(nullptr);
    bsize_entry->setProperty("maximum", 9999);
    bsize_entry->setProperty("minimum", 1);
    bsize_entry->setProperty("value", 3);
    bsize_entry->setWrapping(true);
    lay6->addWidget(bsize_entry, 3, 1, 1, 1);
    border_size_label = new QLabel(nullptr);
    border_size_label->setText(QString::fromUtf8("Tamaño de borde:"));
    border_size_label->setToolTip(QString::fromUtf8("Tamaño del borde del código QR. Cuántos elementos tiene el borde.\nEl valor predeterminado es 4. El ancho del espacio libre alrededor del Código QR."));
    lay6->addWidget(border_size_label, 4, 0, 1, 1);
    border_size_entry = new FCSpinner(nullptr);
    border_size_entry->setProperty("maximum", 9999);
    border_size_entry->setProperty("minimum", 1);
    border_size_entry->setProperty("value", 4);
    border_size_entry->setWrapping(true);
    lay6->addWidget(border_size_entry, 4, 1, 1, 1);
    pol_label = new QLabel(nullptr);
    pol_label->setText(QString::fromUtf8("Polaridad:"));
    pol_label->setToolTip(QString::fromUtf8("Elija la polaridad del código QR.\nSe puede dibujar de forma negativa (los cuadrados son claros)\no de manera positiva (los cuadrados son opacos)."));
    lay6->addWidget(pol_label, 7, 0, 1, 1);
    pol_radio = new RadioSet({{QString::fromUtf8("Negativa"), QVariant(QString::fromUtf8("neg"))}, {QString::fromUtf8("Positivo"), QVariant(QString::fromUtf8("pos"))}}, Qt::Horizontal, true, nullptr);
    pol_radio->setToolTip(QString::fromUtf8("Elija el tipo de QRCode que se creará.\nSi se agrega en un archivo de Silkscreen Gerber, el QRCode puede\nser agregado como positivo Si se agrega a un cobre Gerber\nentonces quizás el QRCode se pueda agregar como negativo."));
    pol_radio->setValue(QVariant(QString::fromUtf8("pos")));
    lay6->addWidget(pol_radio, 7, 1, 1, 1);
    bb_label = new QLabel(nullptr);
    bb_label->setText(QString::fromUtf8("Cuadro delimitador:"));
    bb_label->setToolTip(QString::fromUtf8("El cuadro delimitador, que significa el espacio vacío que rodea\nLa geometría QRCode, puede tener una forma redondeada o cuadrada."));
    lay6->addWidget(bb_label, 8, 0, 1, 1);
    bb_radio = new RadioSet({{QString::fromUtf8("Redondeado"), QVariant(QString::fromUtf8("r"))}, {QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("s"))}}, Qt::Horizontal, true, nullptr);
    bb_radio->setToolTip(QString::fromUtf8("El cuadro delimitador, que significa el espacio vacío que rodea\nLa geometría QRCode, puede tener una forma redondeada o cuadrada."));
    bb_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay6->addWidget(bb_radio, 8, 1, 1, 1);
    export_cb = new FCCheckBox(nullptr);
    export_cb->setText(QString::fromUtf8("Exportar el código QR"));
    export_cb->setToolTip(QString::fromUtf8("Mostrar un conjunto de controles que permiten exportar el QRCode\na un archivo SVG o un archivo PNG."));
    lay6->addWidget(export_cb, 9, 0, 1, 2);
    lay6->setColumnStretch(1, 1);
    layout->addLayout(lay6, 0);
    export_frame = new QFrame(nullptr);
    export_frame->setHidden(true);
    export_lay = new QGridLayout();
    export_lay->setContentsMargins(0, 0, 0, 0);
    export_lay->setHorizontalSpacing(6);
    export_lay->setVerticalSpacing(6);
    fill_color_label = new QLabel(nullptr);
    fill_color_label->setText(QString::fromUtf8("Color de relleno:"));
    fill_color_label->setToolTip(QString::fromUtf8("Establezca el color de relleno del código QR (color de cuadrados / elementos)."));
    export_lay->addWidget(fill_color_label, 0, 0, 1, 1);
    auto* w7 = new QWidget(nullptr);
    auto* lay8 = new QHBoxLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setSpacing(6);
    fill_color_entry = new FCEntry(nullptr);
    fill_color_entry->setText(QString::fromUtf8("#000000"));
    lay8->addWidget(fill_color_entry, 0);
    fill_color_button = new QPushButton(nullptr);
    fill_color_button->setMaximumSize(QSize(15, 15));
    fill_color_button->setMinimumSize(QSize(15, 15));
    fill_color_button->setStyleSheet(QString::fromUtf8("background-color:#000000"));
    lay8->addWidget(fill_color_button, 0, Qt::Alignment(2));
    w7->setLayout(lay8);
    export_lay->addWidget(w7, 0, 1, 1, 1);
    transparent_cb = new FCCheckBox(nullptr);
    transparent_cb->setText(QString::fromUtf8("Color de fondo transparente"));
    export_lay->addWidget(transparent_cb, 1, 0, 1, 2);
    back_color_label = new QLabel(nullptr);
    back_color_label->setText(QString::fromUtf8("Color de fondo:"));
    back_color_label->setToolTip(QString::fromUtf8("Establece el color de fondo del QRCode."));
    export_lay->addWidget(back_color_label, 2, 0, 1, 1);
    auto* w9 = new QWidget(nullptr);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    back_color_entry = new FCEntry(nullptr);
    back_color_entry->setText(QString::fromUtf8("#FFFFFF"));
    lay10->addWidget(back_color_entry, 0);
    back_color_button = new QPushButton(nullptr);
    back_color_button->setMaximumSize(QSize(15, 15));
    back_color_button->setMinimumSize(QSize(15, 15));
    back_color_button->setStyleSheet(QString::fromUtf8("background-color:#FFFFFF"));
    lay10->addWidget(back_color_button, 0, Qt::Alignment(2));
    w9->setLayout(lay10);
    export_lay->addWidget(w9, 2, 1, 1, 1);
    export_svg_button = new QPushButton(nullptr);
    { QFont f = export_svg_button->font(); f.setBold(true); export_svg_button->setFont(f); }
    export_svg_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    export_svg_button->setText(QString::fromUtf8("Exportar el QRCode SVG"));
    export_svg_button->setToolTip(QString::fromUtf8("Exporte un archivo SVG con el contenido de QRCode."));
    export_lay->addWidget(export_svg_button, 3, 0, 1, 2);
    export_png_button = new QPushButton(nullptr);
    { QFont f = export_png_button->font(); f.setBold(true); export_png_button->setFont(f); }
    export_png_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    export_png_button->setText(QString::fromUtf8("Exportar el QRCode PNG"));
    export_png_button->setToolTip(QString::fromUtf8("Exporte un archivo de imagen PNG con el contenido de QRCode."));
    export_lay->addWidget(export_png_button, 4, 0, 1, 2);
    export_lay->setColumnStretch(1, 1);
    export_frame->setLayout(export_lay);
    layout->addWidget(export_frame, 0);
    qrcode_button = new QPushButton(nullptr);
    { QFont f = qrcode_button->font(); f.setBold(true); qrcode_button->setFont(f); }
    qrcode_button->setIcon(QIcon(QString::fromUtf8(":/images/qrcode32.png")));
    qrcode_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    qrcode_button->setText(QString::fromUtf8("Insertar QRCode"));
    qrcode_button->setToolTip(QString::fromUtf8("Crea el objeto QRCode."));
    layout->addWidget(qrcode_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> QRCodeToolUIBase::membersByName() const {
    return {
        {"back_color_button", back_color_button},
        {"back_color_entry", back_color_entry},
        {"back_color_label", back_color_label},
        {"bb_label", bb_label},
        {"bb_radio", bb_radio},
        {"border_size_entry", border_size_entry},
        {"border_size_label", border_size_label},
        {"bsize_entry", bsize_entry},
        {"bsize_label", bsize_label},
        {"error_label", error_label},
        {"error_radio", error_radio},
        {"export_cb", export_cb},
        {"export_frame", export_frame},
        {"export_lay", export_lay},
        {"export_png_button", export_png_button},
        {"export_svg_button", export_svg_button},
        {"fill_color_button", fill_color_button},
        {"fill_color_entry", fill_color_entry},
        {"fill_color_label", fill_color_label},
        {"grb_object_combo", grb_object_combo},
        {"grbobj_label", grbobj_label},
        {"layout", layout},
        {"pol_label", pol_label},
        {"pol_radio", pol_radio},
        {"qrcode_button", qrcode_button},
        {"qrcode_label", qrcode_label},
        {"reset_button", reset_button},
        {"text_data", text_data},
        {"text_label", text_label},
        {"transparent_cb", transparent_cb},
        {"version_entry", version_entry},
        {"version_label", version_label},
    };
}

}  // namespace flatcam::gui
