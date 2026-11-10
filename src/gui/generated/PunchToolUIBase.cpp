// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PunchToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void PunchToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Gerber Perforadora"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Gerber Perforadora"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    grb_label = new QLabel(nullptr);
    grb_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    grb_label->setToolTip(QString::fromUtf8("Gerber en el que hacer agujeros."));
    lay3->addWidget(grb_label, 0, 0, 1, 2);
    gerber_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(gerber_object_combo, 1, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 2, 0, 1, 2);
    padt_label = new QLabel(nullptr);
    padt_label->setText(QString::fromUtf8("<b>Tipo de almohadillas procesadas</b>"));
    padt_label->setToolTip(QString::fromUtf8("El tipo de forma de almohadillas que se procesará.\nSi la PCB tiene muchas almohadillas SMD con almohadillas rectangulares,\ndeshabilitar la apertura rectangular."));
    lay3->addWidget(padt_label, 3, 0, 1, 2);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    auto* lay6 = new QGridLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setHorizontalSpacing(6);
    lay6->setVerticalSpacing(6);
    select_all_cb = new FCCheckBox(nullptr);
    select_all_cb->setText(QString::fromUtf8("Todos"));
    lay6->addWidget(select_all_cb, 0, 0, 1, 1);
    circular_cb = new FCCheckBox(nullptr);
    circular_cb->setChecked(true);
    circular_cb->setText(QString::fromUtf8("Circular"));
    circular_cb->setToolTip(QString::fromUtf8("Proceso de Almohadillas Circulares."));
    lay6->addWidget(circular_cb, 1, 0, 1, 1);
    oblong_cb = new FCCheckBox(nullptr);
    oblong_cb->setText(QString::fromUtf8("Oblongo"));
    oblong_cb->setToolTip(QString::fromUtf8("Procesar almohadillas oblongas."));
    lay6->addWidget(oblong_cb, 2, 0, 1, 1);
    square_cb = new FCCheckBox(nullptr);
    square_cb->setChecked(true);
    square_cb->setText(QString::fromUtf8("Cuadrado"));
    square_cb->setToolTip(QString::fromUtf8("Procesar almohadillas cuadradas."));
    lay6->addWidget(square_cb, 3, 0, 1, 1);
    rectangular_cb = new FCCheckBox(nullptr);
    rectangular_cb->setText(QString::fromUtf8("Rectangular"));
    rectangular_cb->setToolTip(QString::fromUtf8("Proceso Almohadillas Rectangulares."));
    lay6->addWidget(rectangular_cb, 4, 0, 1, 1);
    other_cb = new FCCheckBox(nullptr);
    other_cb->setText(QString::fromUtf8("Otros"));
    other_cb->setToolTip(QString::fromUtf8("Procese los pads no en las categorías anteriores."));
    lay6->addWidget(other_cb, 5, 0, 1, 1);
    lay5->addLayout(lay6, 0, 0, 1, 1);
    apertures_table = new FCTable(nullptr);
    apertures_table->setProperty("columnCount", 4);
    apertures_table->setProperty("horizontalScrollBarPolicy", 1);
    apertures_table->setProperty("selectionMode", 2);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); apertures_table->setSizePolicy(sp); }
    apertures_table->setColumnCount(4);
    apertures_table->setHorizontalHeaderLabels({QString::fromUtf8("Código"), QString::fromUtf8("Tipo"), QString::fromUtf8("Tamaño"), QString::fromUtf8("M")});
    apertures_table->verticalHeader()->setVisible(false);
    apertures_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(3));
    apertures_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(3));
    apertures_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode(1));
    apertures_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode(2));
    lay5->addWidget(apertures_table, 0, 1, 1, 1);
    lay5->setColumnStretch(1, 1);
    lay3->addLayout(lay5, 5, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay3->addWidget(w7, 10, 0, 1, 2);
    lay3->setColumnStretch(0, 1);
    layout->addLayout(lay3, 0);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    method_label = new QLabel(nullptr);
    method_label->setText(QString::fromUtf8("<b>Método:</b>"));
    method_label->setToolTip(QString::fromUtf8("La fuente del orificio de perforación puede ser:\n- Objeto Excellon-> el centro de perforación de objetos Excellon servirá como referencia.\n- Diámetro fijo -> intentará usar el centro de las almohadillas como referencia agregando agujeros de diámetro fijo.\n- Anillo anular fijo -> intentará mantener un anillo anular establecido.\n- Proporcional -> hará un orificio de perforación Gerber con un diámetro del porcentaje del diámetro de la almohadilla."));
    lay8->addWidget(method_label, 0, 0, 1, 2);
    method_punch = new RadioSet({{QString::fromUtf8("Excellon"), QVariant(QString::fromUtf8("exc"))}, {QString::fromUtf8("Diámetro fijo"), QVariant(QString::fromUtf8("fixed"))}, {QString::fromUtf8("Proporcional"), QVariant(QString::fromUtf8("prop"))}, {QString::fromUtf8("Anillo anular fijo"), QVariant(QString::fromUtf8("ring"))}}, Qt::Vertical, false, nullptr);
    method_punch->setValue(QVariant(QString::fromUtf8("exc")));
    lay8->addWidget(method_punch, 1, 0, 1, 2);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay8->addWidget(w9, 2, 0, 1, 2);
    exc_label = new QLabel(nullptr);
    exc_label->setText(QString::fromUtf8("<b>Excellon</b>"));
    exc_label->setToolTip(QString::fromUtf8("Retire la geometría de Excellon del Gerber para crear los agujeros en las almohadillas."));
    lay8->addWidget(exc_label, 3, 0, 1, 2);
    exc_combo = new FCComboBox(nullptr);
    lay8->addWidget(exc_combo, 4, 0, 1, 2);
    fixed_label = new QLabel(nullptr);
    fixed_label->setText(QString::fromUtf8("<b>Diámetro fijo</b>"));
    fixed_label->setHidden(true);
    lay8->addWidget(fixed_label, 6, 0, 1, 2);
    dia_label = new QLabel(nullptr);
    dia_label->setText(QString::fromUtf8("Valor:"));
    dia_label->setToolTip(QString::fromUtf8("Diámetro fijo del agujero."));
    dia_label->setHidden(true);
    lay8->addWidget(dia_label, 8, 0, 1, 1);
    dia_entry = new FCDoubleSpinner(nullptr);
    dia_entry->setProperty("decimals", 4);
    dia_entry->setProperty("maximum", 10000.0);
    dia_entry->setProperty("value", 0.5);
    dia_entry->setHidden(true);
    lay8->addWidget(dia_entry, 8, 1, 1, 1);
    ring_frame = new QFrame(nullptr);
    ring_frame->setHidden(true);
    ring_box = new QVBoxLayout();
    ring_box->setContentsMargins(0, 0, 0, 0);
    ring_box->setSpacing(6);
    ring_label = new QLabel(nullptr);
    ring_label->setText(QString::fromUtf8("<b>Anillo anular fijo</b>"));
    ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular.\nLa astilla de cobre entre el exterior del agujero\ny el margen de la almohadilla de cobre."));
    ring_box->addWidget(ring_label, 0);
    grid1 = new QGridLayout();
    grid1->setContentsMargins(0, 0, 0, 0);
    grid1->setHorizontalSpacing(6);
    grid1->setVerticalSpacing(6);
    circular_ring_label = new QLabel(nullptr);
    circular_ring_label->setText(QString::fromUtf8("Circular:"));
    circular_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas circulares."));
    grid1->addWidget(circular_ring_label, 3, 0, 1, 1);
    circular_ring_entry = new FCDoubleSpinner(nullptr);
    circular_ring_entry->setProperty("decimals", 4);
    circular_ring_entry->setProperty("maximum", 10000.0);
    circular_ring_entry->setProperty("value", 0.2);
    grid1->addWidget(circular_ring_entry, 3, 1, 1, 1);
    oblong_ring_label = new QLabel(nullptr);
    oblong_ring_label->setText(QString::fromUtf8("Oblongo:"));
    oblong_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas oblongas."));
    grid1->addWidget(oblong_ring_label, 4, 0, 1, 1);
    oblong_ring_entry = new FCDoubleSpinner(nullptr);
    oblong_ring_entry->setProperty("decimals", 4);
    oblong_ring_entry->setEnabled(false);
    oblong_ring_entry->setProperty("maximum", 10000.0);
    oblong_ring_entry->setProperty("value", 0.2);
    grid1->addWidget(oblong_ring_entry, 4, 1, 1, 1);
    square_ring_label = new QLabel(nullptr);
    square_ring_label->setText(QString::fromUtf8("Cuadrado:"));
    square_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas cuadradas."));
    grid1->addWidget(square_ring_label, 5, 0, 1, 1);
    square_ring_entry = new FCDoubleSpinner(nullptr);
    square_ring_entry->setProperty("decimals", 4);
    square_ring_entry->setProperty("maximum", 10000.0);
    square_ring_entry->setProperty("value", 0.2);
    grid1->addWidget(square_ring_entry, 5, 1, 1, 1);
    rectangular_ring_label = new QLabel(nullptr);
    rectangular_ring_label->setText(QString::fromUtf8("Rectangular:"));
    rectangular_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas rectangulares."));
    grid1->addWidget(rectangular_ring_label, 6, 0, 1, 1);
    rectangular_ring_entry = new FCDoubleSpinner(nullptr);
    rectangular_ring_entry->setProperty("decimals", 4);
    rectangular_ring_entry->setEnabled(false);
    rectangular_ring_entry->setProperty("maximum", 10000.0);
    rectangular_ring_entry->setProperty("value", 0.2);
    grid1->addWidget(rectangular_ring_entry, 6, 1, 1, 1);
    other_ring_label = new QLabel(nullptr);
    other_ring_label->setText(QString::fromUtf8("Otros:"));
    other_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para otras almohadillas."));
    grid1->addWidget(other_ring_label, 7, 0, 1, 1);
    other_ring_entry = new FCDoubleSpinner(nullptr);
    other_ring_entry->setProperty("decimals", 4);
    other_ring_entry->setEnabled(false);
    other_ring_entry->setProperty("maximum", 10000.0);
    other_ring_entry->setProperty("value", 0.2);
    grid1->addWidget(other_ring_entry, 7, 1, 1, 1);
    grid1->setColumnStretch(1, 1);
    ring_box->addLayout(grid1, 0);
    ring_frame->setLayout(ring_box);
    lay8->addWidget(ring_frame, 10, 0, 1, 2);
    prop_label = new QLabel(nullptr);
    prop_label->setText(QString::fromUtf8("<b>Diá. proporcional</b>"));
    prop_label->setHidden(true);
    lay8->addWidget(prop_label, 12, 0, 1, 2);
    factor_label = new QLabel(nullptr);
    factor_label->setText(QString::fromUtf8("Valor:"));
    factor_label->setToolTip(QString::fromUtf8("Diámetro proporcional.\nEl diámetro del agujero será una fracción del tamaño de la almohadilla."));
    factor_label->setHidden(true);
    lay8->addWidget(factor_label, 13, 0, 1, 1);
    factor_entry = new FCDoubleSpinner(nullptr);
    factor_entry->setProperty("decimals", 4);
    factor_entry->setProperty("maximum", 100.0);
    factor_entry->setProperty("singleStep", 0.1);
    factor_entry->setProperty("suffix", QString::fromUtf8(" %"));
    factor_entry->setProperty("value", 80.0);
    factor_entry->setHidden(true);
    lay8->addWidget(factor_entry, 13, 1, 1, 1);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay8->addWidget(w10, 14, 0, 1, 2);
    lay8->setColumnStretch(1, 1);
    layout->addLayout(lay8, 0);
    punch_object_button = new QPushButton(nullptr);
    { QFont f = punch_object_button->font(); f.setBold(true); punch_object_button->setFont(f); }
    punch_object_button->setIcon(QIcon(QString::fromUtf8(":/images/punch32.png")));
    punch_object_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    punch_object_button->setText(QString::fromUtf8("Gerber Perforadora"));
    punch_object_button->setToolTip(QString::fromUtf8("Cree un objeto Gerber a partir del objeto seleccionado, dentro de\nEl cuadro especificado."));
    layout->addWidget(punch_object_button, 0);
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

std::map<std::string, QObject*> PunchToolUIBase::membersByName() const {
    return {
        {"apertures_table", apertures_table},
        {"circular_cb", circular_cb},
        {"circular_ring_entry", circular_ring_entry},
        {"circular_ring_label", circular_ring_label},
        {"dia_entry", dia_entry},
        {"dia_label", dia_label},
        {"exc_combo", exc_combo},
        {"exc_label", exc_label},
        {"factor_entry", factor_entry},
        {"factor_label", factor_label},
        {"fixed_label", fixed_label},
        {"gerber_object_combo", gerber_object_combo},
        {"grb_label", grb_label},
        {"grid1", grid1},
        {"layout", layout},
        {"method_label", method_label},
        {"method_punch", method_punch},
        {"oblong_cb", oblong_cb},
        {"oblong_ring_entry", oblong_ring_entry},
        {"oblong_ring_label", oblong_ring_label},
        {"other_cb", other_cb},
        {"other_ring_entry", other_ring_entry},
        {"other_ring_label", other_ring_label},
        {"padt_label", padt_label},
        {"prop_label", prop_label},
        {"punch_object_button", punch_object_button},
        {"rectangular_cb", rectangular_cb},
        {"rectangular_ring_entry", rectangular_ring_entry},
        {"rectangular_ring_label", rectangular_ring_label},
        {"reset_button", reset_button},
        {"ring_box", ring_box},
        {"ring_frame", ring_frame},
        {"ring_label", ring_label},
        {"select_all_cb", select_all_cb},
        {"square_cb", square_cb},
        {"square_ring_entry", square_ring_entry},
        {"square_ring_label", square_ring_label},
    };
}

}  // namespace flatcam::gui
