// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/FiducialsToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void FiducialsToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de Fiduciales"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de Fiduciales"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    points_label = new QLabel(nullptr);
    points_label->setText(QString::fromUtf8("<b>Coordenadas Fiduciales:</b>"));
    points_label->setToolTip(QString::fromUtf8("Una tabla con las coordenadas de los puntos fiduciales,\nen el formato (x, y)."));
    layout->addWidget(points_label, 0);
    points_table = new FCTable(nullptr);
    points_table->setProperty("columnCount", 3);
    points_table->setMaximumSize(QSize(16777215, 113));
    points_table->setMinimumSize(QSize(0, 113));
    points_table->setProperty("rowCount", 3);
    points_table->setProperty("selectionBehavior", 1);
    points_table->setProperty("sizeAdjustPolicy", 2);
    points_table->setProperty("verticalScrollBarPolicy", 1);
    points_table->setColumnCount(3);
    points_table->setHorizontalHeaderLabels({QString::fromUtf8("#"), QString::fromUtf8("Nombre"), QString::fromUtf8("Coordenadas")});
    points_table->verticalHeader()->setVisible(false);
    points_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    points_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(2));
    points_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode(1));
    layout->addWidget(points_table, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    layout->addWidget(w3, 0);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    param_label = new QLabel(nullptr);
    param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    param_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay4->addWidget(param_label, 0, 0, 1, 2);
    size_label = new QLabel(nullptr);
    size_label->setText(QString::fromUtf8("Tamaño:"));
    size_label->setToolTip(QString::fromUtf8("Esto establece el diámetro fiducial si el tipo fiducial es circular,\nde lo contrario es el tamaño del fiducial.\nLa apertura de la máscara de soldadura es el doble que eso."));
    lay4->addWidget(size_label, 1, 0, 1, 1);
    fid_size_entry = new FCDoubleSpinner(nullptr);
    fid_size_entry->setProperty("decimals", 4);
    fid_size_entry->setProperty("maximum", 3.0);
    fid_size_entry->setProperty("minimum", 1.0);
    fid_size_entry->setProperty("singleStep", 0.1);
    fid_size_entry->setProperty("value", 1.0);
    fid_size_entry->setWrapping(true);
    lay4->addWidget(fid_size_entry, 1, 1, 1, 1);
    margin_label = new QLabel(nullptr);
    margin_label->setText(QString::fromUtf8("Margen:"));
    margin_label->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay4->addWidget(margin_label, 2, 0, 1, 1);
    margin_entry = new FCDoubleSpinner(nullptr);
    margin_entry->setProperty("decimals", 4);
    margin_entry->setProperty("maximum", 10000.0);
    margin_entry->setProperty("minimum", -10000.0);
    margin_entry->setProperty("singleStep", 0.1);
    margin_entry->setProperty("value", 1.0);
    lay4->addWidget(margin_entry, 2, 1, 1, 1);
    mode_label = new QLabel(nullptr);
    mode_label->setText(QString::fromUtf8("Modo:"));
    mode_label->setToolTip(QString::fromUtf8("- 'Auto' - colocación automática de fiduciales en las esquinas del cuadro delimitador.\n- 'Manual' - colocación manual de fiduciales."));
    lay4->addWidget(mode_label, 3, 0, 1, 1);
    mode_radio = new RadioSet({{QString::fromUtf8("Auto"), QVariant(QString::fromUtf8("auto"))}, {QString::fromUtf8("Manual"), QVariant(QString::fromUtf8("manual"))}}, Qt::Horizontal, false, nullptr);
    mode_radio->setValue(QVariant(QString::fromUtf8("auto")));
    lay4->addWidget(mode_radio, 3, 1, 1, 1);
    pos_label = new QLabel(nullptr);
    pos_label->setText(QString::fromUtf8("Segundo fiducial:"));
    pos_label->setToolTip(QString::fromUtf8("La posición para el segundo fiducial.\n- 'Arriba' - el orden es: abajo a la izquierda, arriba a la izquierda, arriba a la derecha.\n- 'Abajo' - el orden es: abajo a la izquierda, abajo a la derecha, arriba a la derecha.\n- 'Ninguno' - no hay un segundo fiducial. El orden es: abajo a la izquierda, arriba a la derecha."));
    lay4->addWidget(pos_label, 4, 0, 1, 1);
    pos_radio = new RadioSet({{QString::fromUtf8("Arriba"), QVariant(QString::fromUtf8("up"))}, {QString::fromUtf8("Abajo"), QVariant(QString::fromUtf8("down"))}, {QString::fromUtf8("Ninguno"), QVariant(QString::fromUtf8("no"))}}, Qt::Horizontal, false, nullptr);
    pos_radio->setValue(QVariant(QString::fromUtf8("up")));
    lay4->addWidget(pos_radio, 4, 1, 1, 1);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay4->addWidget(w5, 5, 0, 1, 2);
    fid_type_label = new QLabel(nullptr);
    fid_type_label->setText(QString::fromUtf8("Tipo fiducial:"));
    fid_type_label->setToolTip(QString::fromUtf8("El tipo de fiducial.\n- 'Circular': este es el fiducial regular.\n- 'Cruce' - líneas cruzadas fiduciales.\n- 'Ajedrez' - patrón de ajedrez fiducial."));
    lay4->addWidget(fid_type_label, 6, 0, 1, 1);
    fid_type_radio = new RadioSet({{QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("circular"))}, {QString::fromUtf8("Cruce"), QVariant(QString::fromUtf8("cross"))}, {QString::fromUtf8("Ajedrez"), QVariant(QString::fromUtf8("chess"))}}, Qt::Horizontal, false, nullptr);
    fid_type_radio->setValue(QVariant(QString::fromUtf8("circular")));
    lay4->addWidget(fid_type_radio, 6, 1, 1, 1);
    line_thickness_label = new QLabel(nullptr);
    line_thickness_label->setEnabled(false);
    line_thickness_label->setText(QString::fromUtf8("Grosor de la línea:"));
    line_thickness_label->setToolTip(QString::fromUtf8("Espesor de la línea que hace al fiducial."));
    lay4->addWidget(line_thickness_label, 7, 0, 1, 1);
    line_thickness_entry = new FCDoubleSpinner(nullptr);
    line_thickness_entry->setProperty("decimals", 4);
    line_thickness_entry->setEnabled(false);
    line_thickness_entry->setProperty("maximum", 10000.0);
    line_thickness_entry->setProperty("singleStep", 0.1);
    line_thickness_entry->setProperty("value", 0.25);
    lay4->addWidget(line_thickness_entry, 7, 1, 1, 1);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay4->addWidget(w6, 8, 0, 1, 2);
    grbobj_label = new QLabel(nullptr);
    grbobj_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    grbobj_label->setToolTip(QString::fromUtf8("Gerber Objeto al que se agregará un Copper Thieving."));
    lay4->addWidget(grbobj_label, 9, 0, 1, 2);
    grb_object_combo = new FCComboBox(nullptr);
    lay4->addWidget(grb_object_combo, 10, 0, 1, 2);
    add_cfid_button = new QPushButton(nullptr);
    { QFont f = add_cfid_button->font(); f.setBold(true); add_cfid_button->setFont(f); }
    add_cfid_button->setIcon(QIcon(QString::fromUtf8(":/images/fiducials_32.png")));
    add_cfid_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    add_cfid_button->setText(QString::fromUtf8("Añadir Fiducial"));
    add_cfid_button->setToolTip(QString::fromUtf8("Agregará un polígono en la capa de cobre para servir como fiducial."));
    lay4->addWidget(add_cfid_button, 11, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay4->addWidget(w7, 12, 0, 1, 2);
    sm_object_label = new QLabel(nullptr);
    sm_object_label->setText(QString::fromUtf8("<b>Soldermask Gerber:</b>"));
    sm_object_label->setToolTip(QString::fromUtf8("El objeto Soldermask Gerber."));
    lay4->addWidget(sm_object_label, 13, 0, 1, 2);
    sm_object_combo = new FCComboBox(nullptr);
    lay4->addWidget(sm_object_combo, 14, 0, 1, 2);
    add_sm_opening_button = new QPushButton(nullptr);
    { QFont f = add_sm_opening_button->font(); f.setBold(true); add_sm_opening_button->setFont(f); }
    add_sm_opening_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    add_sm_opening_button->setText(QString::fromUtf8("Agregar apertura de Soldermask"));
    add_sm_opening_button->setToolTip(QString::fromUtf8("Agregará un polígono en la capa de máscara de soldadura\npara servir como apertura fiducial.\nEl diámetro siempre es el doble del diámetro.\npara el cobre fiducial."));
    lay4->addWidget(add_sm_opening_button, 15, 0, 1, 2);
    lay4->setColumnStretch(1, 1);
    layout->addLayout(lay4, 0);
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

std::map<std::string, QObject*> FiducialsToolUIBase::membersByName() const {
    return {
        {"add_cfid_button", add_cfid_button},
        {"add_sm_opening_button", add_sm_opening_button},
        {"fid_size_entry", fid_size_entry},
        {"fid_type_label", fid_type_label},
        {"fid_type_radio", fid_type_radio},
        {"grb_object_combo", grb_object_combo},
        {"grbobj_label", grbobj_label},
        {"layout", layout},
        {"line_thickness_entry", line_thickness_entry},
        {"line_thickness_label", line_thickness_label},
        {"margin_entry", margin_entry},
        {"margin_label", margin_label},
        {"mode_label", mode_label},
        {"mode_radio", mode_radio},
        {"param_label", param_label},
        {"points_label", points_label},
        {"points_table", points_table},
        {"pos_label", pos_label},
        {"pos_radio", pos_radio},
        {"reset_button", reset_button},
        {"size_label", size_label},
        {"sm_object_combo", sm_object_combo},
        {"sm_object_label", sm_object_label},
    };
}

}  // namespace flatcam::gui
