// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PcbWizardToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void PcbWizardToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herra. de import. PcbWizard"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herra. de import. PcbWizard"));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* w3 = new FCLabel(nullptr);
    w3->setText(QString::fromUtf8("<b>Cargar archivos:</b>"));
    layout->addWidget(w3, 0);
    auto* lay4 = new QFormLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    excellon_label = new FCLabel(nullptr);
    excellon_label->setText(QString::fromUtf8("Archivo Excellon:"));
    excellon_label->setToolTip(QString::fromUtf8("Cargue el archivo Excellon.\nPor lo general, tiene una extensión .DRL"));
    lay4->setWidget(0, QFormLayout::ItemRole(0), excellon_label);
    excellon_brn = new FCButton(nullptr);
    excellon_brn->setText(QString::fromUtf8("Abrir"));
    lay4->setWidget(0, QFormLayout::ItemRole(1), excellon_brn);
    inf_label = new FCLabel(nullptr);
    inf_label->setText(QString::fromUtf8("Archivo INF:"));
    inf_label->setToolTip(QString::fromUtf8("Cargue el archivo INF."));
    lay4->setWidget(1, QFormLayout::ItemRole(0), inf_label);
    inf_btn = new FCButton(nullptr);
    inf_btn->setText(QString::fromUtf8("Abrir"));
    lay4->setWidget(1, QFormLayout::ItemRole(1), inf_btn);
    layout->addLayout(lay4, 0);
    tools_table = new FCTable(nullptr);
    tools_table->setProperty("columnCount", 2);
    tools_table->setProperty("horizontalScrollBarPolicy", 1);
    tools_table->setMaximumSize(QSize(16777215, 24));
    tools_table->setMinimumSize(QSize(0, 24));
    tools_table->setProperty("verticalScrollBarPolicy", 1);
    tools_table->setHidden(true);
    tools_table->setColumnCount(2);
    tools_table->setHorizontalHeaderLabels({QString::fromUtf8("#Tool"), QString::fromUtf8("Diámetro")});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(3));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    layout->addWidget(tools_table, 0);
    auto* w5 = new FCLabel(nullptr);
    layout->addWidget(w5, 0);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(QString::fromUtf8("<b>Formato Excellon:</b>"));
    layout->addWidget(w6, 0);
    auto* lay7 = new QFormLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    int_label = new FCLabel(nullptr);
    int_label->setText(QString::fromUtf8("Dígitos enteros:"));
    int_label->setToolTip(QString::fromUtf8("El número de dígitos para la parte integral de las coordenadas."));
    lay7->setWidget(0, QFormLayout::ItemRole(0), int_label);
    int_entry = new FCSpinner(nullptr);
    int_entry->setProperty("maximum", 10);
    int_entry->setProperty("minimum", 1);
    int_entry->setProperty("value", 2);
    lay7->setWidget(0, QFormLayout::ItemRole(1), int_entry);
    frac_label = new FCLabel(nullptr);
    frac_label->setText(QString::fromUtf8("Dígitos Fraccio:"));
    frac_label->setToolTip(QString::fromUtf8("El número de dígitos para la parte fraccionaria de las coordenadas."));
    lay7->setWidget(1, QFormLayout::ItemRole(0), frac_label);
    frac_entry = new FCSpinner(nullptr);
    frac_entry->setProperty("maximum", 10);
    frac_entry->setProperty("minimum", 1);
    frac_entry->setProperty("value", 4);
    lay7->setWidget(1, QFormLayout::ItemRole(1), frac_entry);
    zeros_label = new FCLabel(nullptr);
    zeros_label->setText(QString::fromUtf8("Supresión de Ceros:"));
    zeros_label->setToolTip(QString::fromUtf8("El tipo de supresión de ceros utilizada.\nPuede ser de tipo:\n- LZ = los ceros iniciales se mantienen\n- TZ = los ceros finales se mantienen\n- Sin supresión = sin supresión de cero"));
    lay7->setWidget(2, QFormLayout::ItemRole(0), zeros_label);
    zeros_radio = new RadioSet({{QString::fromUtf8("LZ"), QVariant(QString::fromUtf8("LZ"))}, {QString::fromUtf8("TZ"), QVariant(QString::fromUtf8("TZ"))}, {QString::fromUtf8("Sin supresión"), QVariant(QString::fromUtf8("D"))}}, Qt::Horizontal, true, nullptr);
    zeros_radio->setValue(QVariant(QString::fromUtf8("LZ")));
    lay7->setWidget(2, QFormLayout::ItemRole(1), zeros_radio);
    units_label = new FCLabel(nullptr);
    units_label->setText(QString::fromUtf8("<b>Unidades:</b>"));
    units_label->setToolTip(QString::fromUtf8("El tipo de unidades que las coordenadas y la herramienta\ndiámetros están utilizando. Puede ser PULGADAS o MM."));
    lay7->setWidget(3, QFormLayout::ItemRole(0), units_label);
    units_radio = new RadioSet({{QString::fromUtf8("Pulgada"), QVariant(QString::fromUtf8("INCH"))}, {QString::fromUtf8("mm"), QVariant(QString::fromUtf8("METRIC"))}}, Qt::Horizontal, true, nullptr);
    units_radio->setValue(QVariant(QString::fromUtf8("INCH")));
    lay7->setWidget(3, QFormLayout::ItemRole(1), units_radio);
    layout->addLayout(lay7, 0);
    import_button = new QPushButton(nullptr);
    import_button->setText(QString::fromUtf8("Importar Excellon"));
    import_button->setToolTip(QString::fromUtf8("Importar un archivo Excellon\nque almacena su información en 2 archivos.\nUno generalmente tiene la extensión .DRL mientras\nel otro tiene extensión .INF."));
    layout->addWidget(import_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> PcbWizardToolUIBase::membersByName() const {
    return {
        {"excellon_brn", excellon_brn},
        {"excellon_label", excellon_label},
        {"frac_entry", frac_entry},
        {"frac_label", frac_label},
        {"import_button", import_button},
        {"inf_btn", inf_btn},
        {"inf_label", inf_label},
        {"int_entry", int_entry},
        {"int_label", int_label},
        {"layout", layout},
        {"tools_table", tools_table},
        {"units_label", units_label},
        {"units_radio", units_radio},
        {"zeros_label", zeros_label},
        {"zeros_radio", zeros_radio},
    };
}

}  // namespace flatcam::gui
