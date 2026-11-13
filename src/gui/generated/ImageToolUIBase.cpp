// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ImageToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void ImageToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Imagen como objeto"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Imagen como objeto"));
    layout->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    tf_type_obj_combo_label = new QLabel(nullptr);
    tf_type_obj_combo_label->setText(QString::fromUtf8("Tipo de objeto:"));
    tf_type_obj_combo_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto a crear a partir de la imagen.\nPuede ser de tipo: Gerber o Geometría."));
    lay2->setWidget(0, QFormLayout::ItemRole(0), tf_type_obj_combo_label);
    tf_type_obj_combo = new FCComboBox(nullptr);
    tf_type_obj_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    tf_type_obj_combo->addItem(QString::fromUtf8("Gerber"));
    tf_type_obj_combo->addItem(QString::fromUtf8("Geometría"));
    lay2->setWidget(0, QFormLayout::ItemRole(1), tf_type_obj_combo);
    dpi_label = new QLabel(nullptr);
    dpi_label->setText(QString::fromUtf8("Valor de DPI:"));
    dpi_label->setToolTip(QString::fromUtf8("Especifique un valor de DPI para la imagen."));
    lay2->setWidget(1, QFormLayout::ItemRole(0), dpi_label);
    dpi_entry = new FCSpinner(nullptr);
    dpi_entry->setProperty("maximum", 99999);
    dpi_entry->setProperty("value", 96);
    lay2->setWidget(1, QFormLayout::ItemRole(1), dpi_entry);
    layout->addLayout(lay2, 0);
    emty_lbl = new QLabel(nullptr);
    layout->addWidget(emty_lbl, 0);
    detail_label = new QLabel(nullptr);
    detail_label->setText(QString::fromUtf8("<font size=4><b>Nivel de detalle:</b></font>"));
    layout->addWidget(detail_label, 0);
    auto* lay3 = new QFormLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    image_type_label = new QLabel(nullptr);
    image_type_label->setText(QString::fromUtf8("<b>Tipo de imagen:</b>"));
    image_type_label->setToolTip(QString::fromUtf8("Elija un método para la interpretación de la imagen.\nB / N significa una imagen en blanco y negro. Color significa una imagen en color."));
    lay3->setWidget(0, QFormLayout::ItemRole(0), image_type_label);
    image_type = new RadioSet({{QString::fromUtf8("B/W"), QVariant(QString::fromUtf8("black"))}, {QString::fromUtf8("Color"), QVariant(QString::fromUtf8("color"))}}, Qt::Horizontal, true, nullptr);
    image_type->setValue(QVariant(QString::fromUtf8("black")));
    lay3->setWidget(0, QFormLayout::ItemRole(1), image_type);
    mask_bw_label = new QLabel(nullptr);
    mask_bw_label->setText(QString::fromUtf8("Valor de la máscara <b>B/W</b>:"));
    mask_bw_label->setToolTip(QString::fromUtf8("Máscara para imagen monocroma.\nToma valores entre [0 ... 255].\nDecide el nivel de detalles a incluir\nen la geometría resultante.\n0 significa sin detalles y 255 significa todo\n(que es totalmente negro)"));
    lay3->setWidget(1, QFormLayout::ItemRole(0), mask_bw_label);
    mask_bw_entry = new FCSpinner(nullptr);
    mask_bw_entry->setProperty("maximum", 255);
    mask_bw_entry->setProperty("value", 250);
    lay3->setWidget(1, QFormLayout::ItemRole(1), mask_bw_entry);
    mask_r_label = new QLabel(nullptr);
    mask_r_label->setEnabled(false);
    mask_r_label->setText(QString::fromUtf8("Valor de la máscara <b>R:</b>"));
    mask_r_label->setToolTip(QString::fromUtf8("Máscara para color ROJO.\nToma valores entre [0 ... 255].\nDecide el nivel de detalles a incluir\nen la geometría resultante."));
    lay3->setWidget(2, QFormLayout::ItemRole(0), mask_r_label);
    mask_r_entry = new FCSpinner(nullptr);
    mask_r_entry->setEnabled(false);
    mask_r_entry->setProperty("maximum", 255);
    mask_r_entry->setProperty("value", 250);
    lay3->setWidget(2, QFormLayout::ItemRole(1), mask_r_entry);
    mask_g_label = new QLabel(nullptr);
    mask_g_label->setEnabled(false);
    mask_g_label->setText(QString::fromUtf8("Valor de la máscara <b>G:</b>"));
    mask_g_label->setToolTip(QString::fromUtf8("Máscara para color VERDE.\nToma valores entre [0 ... 255].\nDecide el nivel de detalles a incluir\nen la geometría resultante."));
    lay3->setWidget(3, QFormLayout::ItemRole(0), mask_g_label);
    mask_g_entry = new FCSpinner(nullptr);
    mask_g_entry->setEnabled(false);
    mask_g_entry->setProperty("maximum", 255);
    mask_g_entry->setProperty("value", 250);
    lay3->setWidget(3, QFormLayout::ItemRole(1), mask_g_entry);
    mask_b_label = new QLabel(nullptr);
    mask_b_label->setEnabled(false);
    mask_b_label->setText(QString::fromUtf8("Valor de la máscara <b>B:</b>"));
    mask_b_label->setToolTip(QString::fromUtf8("Máscara para color AZUL.\nToma valores entre [0 ... 255].\nDecide el nivel de detalles a incluir\nen la geometría resultante."));
    lay3->setWidget(4, QFormLayout::ItemRole(0), mask_b_label);
    mask_b_entry = new FCSpinner(nullptr);
    mask_b_entry->setEnabled(false);
    mask_b_entry->setProperty("maximum", 255);
    mask_b_entry->setProperty("value", 250);
    lay3->setWidget(4, QFormLayout::ItemRole(1), mask_b_entry);
    layout->addLayout(lay3, 0);
    import_button = new QPushButton(nullptr);
    import_button->setText(QString::fromUtf8("Importar imagen"));
    import_button->setToolTip(QString::fromUtf8("Abra una imagen de tipo ráster y luego impórtela en FlatCAM."));
    layout->addWidget(import_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> ImageToolUIBase::membersByName() const {
    return {
        {"detail_label", detail_label},
        {"dpi_entry", dpi_entry},
        {"dpi_label", dpi_label},
        {"emty_lbl", emty_lbl},
        {"image_type", image_type},
        {"image_type_label", image_type_label},
        {"import_button", import_button},
        {"layout", layout},
        {"mask_b_entry", mask_b_entry},
        {"mask_b_label", mask_b_label},
        {"mask_bw_entry", mask_bw_entry},
        {"mask_bw_label", mask_bw_label},
        {"mask_g_entry", mask_g_entry},
        {"mask_g_label", mask_g_label},
        {"mask_r_entry", mask_r_entry},
        {"mask_r_label", mask_r_label},
        {"tf_type_obj_combo", tf_type_obj_combo},
        {"tf_type_obj_combo_label", tf_type_obj_combo_label},
    };
}

}  // namespace flatcam::gui
