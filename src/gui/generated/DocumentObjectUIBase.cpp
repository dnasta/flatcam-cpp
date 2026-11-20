// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/DocumentObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

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
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Document Object</b></font>")));
    title_box->addWidget(title_label, 1);
    level = new FCLabel(nullptr);
    level->setProperty("alignment", 130);
    level->setText(trUi_(QString::fromUtf8("<span style=\"color:green;\"><b>Basic</b></span>")));
    level->setToolTip(trUi_(QString::fromUtf8("BASIC is suitable for a beginner. Many parameters\nare hidden from the user in this mode.\nADVANCED mode will make available all parameters.\n\nTo change the application LEVEL, go to:\nEdit -> Preferences -> General and check:\n'APP. LEVEL' radio button.")));
    title_box->addWidget(level, 0);
    lay1->addLayout(title_box, 0);
    custom_box = new QVBoxLayout();
    custom_box->setContentsMargins(0, 0, 0, 0);
    custom_box->setSpacing(6);
    name_hlay = new QHBoxLayout();
    name_hlay->setContentsMargins(0, 0, 0, 0);
    name_hlay->setSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(trUi_(QString::fromUtf8("<b>Name:</b>")));
    name_hlay->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_entry->setText(trUi_(QString::fromUtf8("x")));
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
    autocomplete_cb->setText(trUi_(QString::fromUtf8("Auto Completer")));
    autocomplete_cb->setToolTip(trUi_(QString::fromUtf8("This selects if the auto completer is enabled in the Document Editor.")));
    lay3->addWidget(autocomplete_cb, 0);
    lay3->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    custom_box->addLayout(lay3, 0);
    form_box = new QFormLayout();
    form_box->setContentsMargins(0, 0, 0, 0);
    form_box->setHorizontalSpacing(6);
    form_box->setVerticalSpacing(6);
    font_type_label = new FCLabel(nullptr);
    font_type_label->setText(trUi_(QString::fromUtf8("Font Type:")));
    form_box->setWidget(0, QFormLayout::ItemRole(0), font_type_label);
    font_type_cb = new QFontComboBox(nullptr);
    // propiedad currentFont (QFont) sin traducir
    font_type_cb->setProperty("currentText", trUi_(QString::fromUtf8("Helvetica Neue")));
    form_box->setWidget(0, QFormLayout::ItemRole(1), font_type_cb);
    font_size_label = new FCLabel(nullptr);
    font_size_label->setText(trUi_(QString::fromUtf8("Font Size:")));
    form_box->setWidget(1, QFormLayout::ItemRole(0), font_size_label);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    font_size_cb = new FCComboBox(nullptr);
    font_size_cb->setProperty("currentText", trUi_(QString::fromUtf8("12")));
    font_size_cb->setEditable(true);
    font_size_cb->setProperty("focusPolicy", 15);
    font_size_cb->setMaximumSize(QSize(70, 16777215));
    font_size_cb->setProperty("minimumContentsLength", 3);
    font_size_cb->addItem(trUi_(QString::fromUtf8("6")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("7")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("8")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("9")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("10")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("11")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("12")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("13")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("14")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("15")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("16")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("18")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("20")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("22")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("24")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("26")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("28")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("32")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("36")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("40")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("44")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("48")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("54")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("60")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("66")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("72")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("80")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("88")));
    font_size_cb->addItem(trUi_(QString::fromUtf8("96")));
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
    alignment_label->setText(trUi_(QString::fromUtf8("Alignment:")));
    form_box->setWidget(2, QFormLayout::ItemRole(0), alignment_label);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    al_left_tb = new QToolButton(nullptr);
    al_left_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_left32.png")));
    al_left_tb->setToolTip(trUi_(QString::fromUtf8("Align Left")));
    lay5->addWidget(al_left_tb, 0);
    al_center_tb = new QToolButton(nullptr);
    al_center_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_center32.png")));
    al_center_tb->setToolTip(trUi_(QString::fromUtf8("Center")));
    lay5->addWidget(al_center_tb, 0);
    al_right_tb = new QToolButton(nullptr);
    al_right_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_right32.png")));
    al_right_tb->setToolTip(trUi_(QString::fromUtf8("Align Right")));
    lay5->addWidget(al_right_tb, 0);
    al_justify_tb = new QToolButton(nullptr);
    al_justify_tb->setIcon(QIcon(QString::fromUtf8(":/images/align_justify32.png")));
    al_justify_tb->setToolTip(trUi_(QString::fromUtf8("Justify")));
    lay5->addWidget(al_justify_tb, 0);
    form_box->setLayout(2, QFormLayout::ItemRole(1), lay5);
    font_color_label = new FCLabel(nullptr);
    font_color_label->setText(trUi_(QString::fromUtf8("Font Color:")));
    font_color_label->setToolTip(trUi_(QString::fromUtf8("Set the font color for the selected text")));
    form_box->setWidget(3, QFormLayout::ItemRole(0), font_color_label);
    form_box_child_1 = new QHBoxLayout();
    form_box_child_1->setContentsMargins(0, 0, 0, 0);
    form_box_child_1->setSpacing(6);
    font_color_entry = new FCEntry(nullptr);
    font_color_entry->setText(trUi_(QString::fromUtf8("#000000")));
    form_box_child_1->addWidget(font_color_entry, 0);
    font_color_button = new FCButton(nullptr);
    font_color_button->setMaximumSize(QSize(15, 15));
    font_color_button->setMinimumSize(QSize(15, 15));
    font_color_button->setStyleSheet(QString::fromUtf8("background-color:#000000"));
    form_box_child_1->addWidget(font_color_button, 0);
    form_box->setLayout(3, QFormLayout::ItemRole(1), form_box_child_1);
    sel_color_label = new FCLabel(nullptr);
    sel_color_label->setText(trUi_(QString::fromUtf8("Selection Color:")));
    sel_color_label->setToolTip(trUi_(QString::fromUtf8("Set the selection color when doing text selection.")));
    form_box->setWidget(4, QFormLayout::ItemRole(0), sel_color_label);
    form_box_child_2 = new QHBoxLayout();
    form_box_child_2->setContentsMargins(0, 0, 0, 0);
    form_box_child_2->setSpacing(6);
    sel_color_entry = new FCEntry(nullptr);
    sel_color_entry->setText(trUi_(QString::fromUtf8("#0055ff")));
    form_box_child_2->addWidget(sel_color_entry, 0);
    sel_color_button = new FCButton(nullptr);
    sel_color_button->setMaximumSize(QSize(15, 15));
    sel_color_button->setMinimumSize(QSize(15, 15));
    sel_color_button->setStyleSheet(QString::fromUtf8("background-color:#0055ff"));
    form_box_child_2->addWidget(sel_color_button, 0);
    form_box->setLayout(4, QFormLayout::ItemRole(1), form_box_child_2);
    tab_size_label = new FCLabel(nullptr);
    tab_size_label->setText(trUi_(QString::fromUtf8("Tab Size:")));
    tab_size_label->setToolTip(trUi_(QString::fromUtf8("Set the tab size. In pixels. Default value is 80 pixels.")));
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
