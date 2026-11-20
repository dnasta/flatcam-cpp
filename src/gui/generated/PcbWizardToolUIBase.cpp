// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PcbWizardToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void PcbWizardToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("PcbWizard Import Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("PcbWizard Import Tool")));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* w3 = new FCLabel(nullptr);
    w3->setText(trUi_(QString::fromUtf8("<b>Load files:</b>")));
    layout->addWidget(w3, 0);
    auto* lay4 = new QFormLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    excellon_label = new FCLabel(nullptr);
    excellon_label->setText(trUi_(QString::fromUtf8("Excellon file:")));
    excellon_label->setToolTip(trUi_(QString::fromUtf8("Load the Excellon file.\nUsually it has a .DRL extension")));
    lay4->setWidget(0, QFormLayout::ItemRole(0), excellon_label);
    excellon_brn = new FCButton(nullptr);
    excellon_brn->setText(trUi_(QString::fromUtf8("Open")));
    lay4->setWidget(0, QFormLayout::ItemRole(1), excellon_brn);
    inf_label = new FCLabel(nullptr);
    inf_label->setText(trUi_(QString::fromUtf8("INF file:")));
    inf_label->setToolTip(trUi_(QString::fromUtf8("Load the INF file.")));
    lay4->setWidget(1, QFormLayout::ItemRole(0), inf_label);
    inf_btn = new FCButton(nullptr);
    inf_btn->setText(trUi_(QString::fromUtf8("Open")));
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
    tools_table->setHorizontalHeaderLabels({trUi_(QString::fromUtf8("#Tool")), trUi_(QString::fromUtf8("Diameter"))});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(3));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    layout->addWidget(tools_table, 0);
    auto* w5 = new FCLabel(nullptr);
    layout->addWidget(w5, 0);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(trUi_(QString::fromUtf8("<b>Excellon Format:</b>")));
    layout->addWidget(w6, 0);
    auto* lay7 = new QFormLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    int_label = new FCLabel(nullptr);
    int_label->setText(trUi_(QString::fromUtf8("Int. digits:")));
    int_label->setToolTip(trUi_(QString::fromUtf8("The number of digits for the integral part of the coordinates.")));
    lay7->setWidget(0, QFormLayout::ItemRole(0), int_label);
    int_entry = new FCSpinner(nullptr);
    int_entry->setProperty("maximum", 10);
    int_entry->setProperty("minimum", 1);
    int_entry->setProperty("value", 2);
    lay7->setWidget(0, QFormLayout::ItemRole(1), int_entry);
    frac_label = new FCLabel(nullptr);
    frac_label->setText(trUi_(QString::fromUtf8("Frac. digits:")));
    frac_label->setToolTip(trUi_(QString::fromUtf8("The number of digits for the fractional part of the coordinates.")));
    lay7->setWidget(1, QFormLayout::ItemRole(0), frac_label);
    frac_entry = new FCSpinner(nullptr);
    frac_entry->setProperty("maximum", 10);
    frac_entry->setProperty("minimum", 1);
    frac_entry->setProperty("value", 4);
    lay7->setWidget(1, QFormLayout::ItemRole(1), frac_entry);
    zeros_label = new FCLabel(nullptr);
    zeros_label->setText(trUi_(QString::fromUtf8("Zeros supp.:")));
    zeros_label->setToolTip(trUi_(QString::fromUtf8("The type of zeros suppression used.\nCan be of type:\n- LZ = leading zeros are kept\n- TZ = trailing zeros are kept\n- No Suppression = no zero suppression")));
    lay7->setWidget(2, QFormLayout::ItemRole(0), zeros_label);
    zeros_radio = new RadioSet({{trUi_(QString::fromUtf8("LZ")), QVariant(QString::fromUtf8("LZ"))}, {trUi_(QString::fromUtf8("TZ")), QVariant(QString::fromUtf8("TZ"))}, {trUi_(QString::fromUtf8("No Suppression")), QVariant(QString::fromUtf8("D"))}}, Qt::Horizontal, true, nullptr);
    zeros_radio->setValue(QVariant(QString::fromUtf8("LZ")));
    lay7->setWidget(2, QFormLayout::ItemRole(1), zeros_radio);
    units_label = new FCLabel(nullptr);
    units_label->setText(trUi_(QString::fromUtf8("<b>Units:</b>")));
    units_label->setToolTip(trUi_(QString::fromUtf8("The type of units that the coordinates and tool\ndiameters are using. Can be INCH or MM.")));
    lay7->setWidget(3, QFormLayout::ItemRole(0), units_label);
    units_radio = new RadioSet({{trUi_(QString::fromUtf8("Inch")), QVariant(QString::fromUtf8("INCH"))}, {trUi_(QString::fromUtf8("mm")), QVariant(QString::fromUtf8("METRIC"))}}, Qt::Horizontal, true, nullptr);
    units_radio->setValue(QVariant(QString::fromUtf8("INCH")));
    lay7->setWidget(3, QFormLayout::ItemRole(1), units_radio);
    layout->addLayout(lay7, 0);
    import_button = new QPushButton(nullptr);
    import_button->setText(trUi_(QString::fromUtf8("Import Excellon")));
    import_button->setToolTip(trUi_(QString::fromUtf8("Import an Excellon file\nthat store it's information's in 2 files.\nOne usually has .DRL extension while\nthe other has .INF extension.")));
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
