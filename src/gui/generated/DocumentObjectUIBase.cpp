// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DocumentObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void DocumentObjectUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(9, 9, 9, 9);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/notes16_1.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(QString::fromUtf8("<font size=5><b>Objeto de Documento</b></font>"));
    title_box->addWidget(title_label, 1);
    level = new FCLabel(nullptr);
    level->setProperty("alignment", 130);
    level->setText(QString::fromUtf8("<span style=\"color:green;\"><b>BASIC</b></span>"));
    level->setToolTip(QString::fromUtf8("BASIC es adecuado para un principiante. Muchos parámetros\nestán ocultos para el usuario en este modo.\nEl modo AVANZADO pondrá a disposición todos los parámetros.\n\nPara cambiar el NIVEL de la aplicación, vaya a:\nEditar -> Preferencias -> General y verificar:\n'APP. NIVEL 'botón de radio."));
    title_box->addWidget(level, 0);
    lay1->addLayout(title_box, 0);
    custom_box = new QVBoxLayout();
    custom_box->setContentsMargins(0, 0, 0, 0);
    custom_box->setSpacing(6);
    name_hlay = new QHBoxLayout();
    name_hlay->setContentsMargins(0, 0, 0, 0);
    name_hlay->setSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(QString::fromUtf8("<b>Nombre:</b>"));
    name_hlay->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_entry->setText(QString::fromUtf8("x"));
    name_hlay->addWidget(name_entry, 0);
    custom_box->addLayout(name_hlay, 0);
    plot_cb = new FCCheckBox(nullptr);
    plot_cb->setProperty("layoutDirection", 1);
    plot_cb->setHidden(true);
    custom_box->addWidget(plot_cb, 0);
    auto* lay3 = new QHBoxLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setSpacing(6);
    autocomplete_cb = new FCCheckBox(nullptr);
    { QFont f = autocomplete_cb->font(); f.setBold(true); autocomplete_cb->setFont(f); }
    autocomplete_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    autocomplete_cb->setText(QString::fromUtf8("Autocompletador"));
    autocomplete_cb->setToolTip(QString::fromUtf8("Esto selecciona si el autocompletador está habilitado en el Editor de Documentos."));
    lay3->addWidget(autocomplete_cb, 0);
    lay3->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    custom_box->addLayout(lay3, 0);
    form_box = new QFormLayout();
    form_box->setContentsMargins(0, 0, 0, 0);
    form_box->setHorizontalSpacing(6);
    form_box->setVerticalSpacing(6);
    font_type_label = new FCLabel(nullptr);
    font_type_label->setText(QString::fromUtf8("Tipo de Fuente:"));
    form_box->setWidget(0, QFormLayout::ItemRole(0), font_type_label);
    font_type_cb = new QFontComboBox(nullptr);
    // propiedad currentFont (QFont) sin traducir
    font_type_cb->setProperty("currentText", QString::fromUtf8("Helvetica Neue"));
    form_box->setWidget(0, QFormLayout::ItemRole(1), font_type_cb);
    font_size_label = new FCLabel(nullptr);
    font_size_label->setText(QString::fromUtf8("Tamaño de Fuente:"));
    form_box->setWidget(1, QFormLayout::ItemRole(0), font_size_label);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    font_size_cb = new FCComboBox(nullptr);
    font_size_cb->setProperty("currentText", QString::fromUtf8("12"));
    font_size_cb->setEditable(true);
    font_size_cb->setProperty("focusPolicy", 15);
    font_size_cb->setMaximumSize(QSize(70, 16777215));
    font_size_cb->setProperty("minimumContentsLength", 3);
    font_size_cb->addItem(QString::fromUtf8("6"));
    font_size_cb->addItem(QString::fromUtf8("7"));
    font_size_cb->addItem(QString::fromUtf8("8"));
    font_size_cb->addItem(QString::fromUtf8("9"));
    font_size_cb->addItem(QString::fromUtf8("10"));
    font_size_cb->addItem(QString::fromUtf8("11"));
    font_size_cb->addItem(QString::fromUtf8("12"));
    font_size_cb->addItem(QString::fromUtf8("13"));
    font_size_cb->addItem(QString::fromUtf8("14"));
    font_size_cb->addItem(QString::fromUtf8("15"));
    font_size_cb->addItem(QString::fromUtf8("16"));
    font_size_cb->addItem(QString::fromUtf8("18"));
    font_size_cb->addItem(QString::fromUtf8("20"));
    font_size_cb->addItem(QString::fromUtf8("22"));
    font_size_cb->addItem(QString::fromUtf8("24"));
    font_size_cb->addItem(QString::fromUtf8("26"));
    font_size_cb->addItem(QString::fromUtf8("28"));
    font_size_cb->addItem(QString::fromUtf8("32"));
    font_size_cb->addItem(QString::fromUtf8("36"));
    font_size_cb->addItem(QString::fromUtf8("40"));
    font_size_cb->addItem(QString::fromUtf8("44"));
    font_size_cb->addItem(QString::fromUtf8("48"));
    font_size_cb->addItem(QString::fromUtf8("54"));
    font_size_cb->addItem(QString::fromUtf8("60"));
    font_size_cb->addItem(QString::fromUtf8("66"));
    font_size_cb->addItem(QString::fromUtf8("72"));
    font_size_cb->addItem(QString::fromUtf8("80"));
    font_size_cb->addItem(QString::fromUtf8("88"));
    font_size_cb->addItem(QString::fromUtf8("96"));
    font_size_cb->setCurrentIndex(6);
    lay4->addWidget(font_size_cb, 0);
    lay4->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    font_bold_tb = new QToolButton(nullptr);
    font_bold_tb->setCheckable(true);
    font_bold_tb->setIcon(QIcon(QString::fromUtf8(":/images/bold32.png")));
    lay4->addWidget(font_bold_tb, 0);
    font_italic_tb = new QToolButton(nullptr);
    font_italic_tb->setCheckable(true);
    font_italic_tb->setIcon(QIcon(QString::fromUtf8(":/images/italic32.png")));
    lay4->addWidget(font_italic_tb, 0);
    font_under_tb = new QToolButton(nullptr);
    font_under_tb->setCheckable(true);
    font_under_tb->setIcon(QIcon(QString::fromUtf8(":/images/underline32.png")));
    lay4->addWidget(font_under_tb, 0);
    form_box->setLayout(1, QFormLayout::ItemRole(1), lay4);
    alignment_label = new FCLabel(nullptr);
    alignment_label->setText(QString::fromUtf8("Alineación:"));
    form_box->setWidget(2, QFormLayout::ItemRole(0), alignment_label);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    al_left_tb = new QToolButton(nullptr);
    al_left_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_left32.png")));
    al_left_tb->setToolTip(QString::fromUtf8("Alinear a la izquierda"));
    lay5->addWidget(al_left_tb, 0);
    al_center_tb = new QToolButton(nullptr);
    al_center_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_center32.png")));
    al_center_tb->setToolTip(QString::fromUtf8("Centrar"));
    lay5->addWidget(al_center_tb, 0);
    al_right_tb = new QToolButton(nullptr);
    al_right_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_right32.png")));
    al_right_tb->setToolTip(QString::fromUtf8("Alinear a la derecha"));
    lay5->addWidget(al_right_tb, 0);
    al_justify_tb = new QToolButton(nullptr);
    al_justify_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_justify32.png")));
    al_justify_tb->setToolTip(QString::fromUtf8("Alinear Justificar"));
    lay5->addWidget(al_justify_tb, 0);
    form_box->setLayout(2, QFormLayout::ItemRole(1), lay5);
    font_color_label = new FCLabel(nullptr);
    font_color_label->setText(QString::fromUtf8("Color de Fuente:"));
    font_color_label->setToolTip(QString::fromUtf8("Establecer el color de fuente para el texto seleccionado"));
    form_box->setWidget(3, QFormLayout::ItemRole(0), font_color_label);
    form_box_child_1 = new QHBoxLayout();
    form_box_child_1->setContentsMargins(0, 0, 0, 0);
    form_box_child_1->setSpacing(6);
    font_color_entry = new FCEntry(nullptr);
    font_color_entry->setText(QString::fromUtf8("#000000"));
    form_box_child_1->addWidget(font_color_entry, 0);
    font_color_button = new FCButton(nullptr);
    font_color_button->setMaximumSize(QSize(15, 15));
    font_color_button->setMinimumSize(QSize(15, 15));
    font_color_button->setStyleSheet(QString::fromUtf8("background-color:#000000"));
    form_box_child_1->addWidget(font_color_button, 0);
    form_box->setLayout(3, QFormLayout::ItemRole(1), form_box_child_1);
    sel_color_label = new FCLabel(nullptr);
    sel_color_label->setText(QString::fromUtf8("Color de seleccion:"));
    sel_color_label->setToolTip(QString::fromUtf8("Establezca el color de selección al hacer la selección de texto."));
    form_box->setWidget(4, QFormLayout::ItemRole(0), sel_color_label);
    form_box_child_2 = new QHBoxLayout();
    form_box_child_2->setContentsMargins(0, 0, 0, 0);
    form_box_child_2->setSpacing(6);
    sel_color_entry = new FCEntry(nullptr);
    sel_color_entry->setText(QString::fromUtf8("#0055ff"));
    form_box_child_2->addWidget(sel_color_entry, 0);
    sel_color_button = new FCButton(nullptr);
    sel_color_button->setMaximumSize(QSize(15, 15));
    sel_color_button->setMinimumSize(QSize(15, 15));
    sel_color_button->setStyleSheet(QString::fromUtf8("background-color:#0055ff"));
    form_box_child_2->addWidget(sel_color_button, 0);
    form_box->setLayout(4, QFormLayout::ItemRole(1), form_box_child_2);
    tab_size_label = new FCLabel(nullptr);
    tab_size_label->setText(QString::fromUtf8("Tamaño de Pestaña:"));
    tab_size_label->setToolTip(QString::fromUtf8("Establece el tamaño de la pestaña. En píxeles El valor predeterminado es 80 píxeles."));
    form_box->setWidget(5, QFormLayout::ItemRole(0), tab_size_label);
    tab_size_spinner = new FCSpinner(nullptr);
    tab_size_spinner->setProperty("maximum", 1000);
    tab_size_spinner->setProperty("value", 80);
    form_box->setWidget(5, QFormLayout::ItemRole(1), tab_size_spinner);
    custom_box->addLayout(form_box, 0);
    custom_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->addLayout(custom_box, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(lay1);
}

std::map<std::string, QObject*> DocumentObjectUIBase::membersByName() const {
    return {
        {"al_center_tb", al_center_tb},
        {"al_justify_tb", al_justify_tb},
        {"al_left_tb", al_left_tb},
        {"al_right_tb", al_right_tb},
        {"alignment_label", alignment_label},
        {"autocomplete_cb", autocomplete_cb},
        {"custom_box", custom_box},
        {"font_bold_tb", font_bold_tb},
        {"font_color_button", font_color_button},
        {"font_color_entry", font_color_entry},
        {"font_color_label", font_color_label},
        {"font_italic_tb", font_italic_tb},
        {"font_size_cb", font_size_cb},
        {"font_size_label", font_size_label},
        {"font_type_cb", font_type_cb},
        {"font_type_label", font_type_label},
        {"font_under_tb", font_under_tb},
        {"form_box", form_box},
        {"form_box_child_1", form_box_child_1},
        {"form_box_child_2", form_box_child_2},
        {"icon", icon},
        {"level", level},
        {"name_entry", name_entry},
        {"name_hlay", name_hlay},
        {"plot_cb", plot_cb},
        {"sel_color_button", sel_color_button},
        {"sel_color_entry", sel_color_entry},
        {"sel_color_label", sel_color_label},
        {"tab_size_label", tab_size_label},
        {"tab_size_spinner", tab_size_spinner},
        {"title_box", title_box},
        {"title_label", title_label},
    };
}

}  // namespace flatcam::gui
