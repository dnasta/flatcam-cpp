// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ScriptObjectUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void ScriptObjectUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(9, 9, 9, 9);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/script_new24.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(trUi_(QString::fromUtf8("<font size=5><b>Script Object</b></font>")));
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
    auto* lay3 = new QHBoxLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setSpacing(6);
    autocomplete_cb = new FCCheckBox(nullptr);
    autocomplete_cb->setChecked(true);
    { QFont f = autocomplete_cb->font(); f.setBold(true); autocomplete_cb->setFont(f); }
    autocomplete_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    autocomplete_cb->setText(trUi_(QString::fromUtf8("Auto Completer")));
    autocomplete_cb->setToolTip(trUi_(QString::fromUtf8("This selects if the auto completer is enabled in the Script Editor.")));
    lay3->addWidget(autocomplete_cb, 0);
    lay3->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    custom_box->addLayout(lay3, 0);
    plot_cb = new FCCheckBox(nullptr);
    plot_cb->setProperty("layoutDirection", 1);
    plot_cb->setHidden(true);
    custom_box->addWidget(plot_cb, 0);
    custom_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->addLayout(custom_box, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(lay1);
}

std::map<std::string, QObject*> ScriptObjectUIBase::membersByName() const {
    return {
        {"autocomplete_cb", autocomplete_cb},
        {"custom_box", custom_box},
        {"icon", icon},
        {"level", level},
        {"name_entry", name_entry},
        {"name_hlay", name_hlay},
        {"plot_cb", plot_cb},
        {"title_box", title_box},
        {"title_label", title_label},
    };
}

}  // namespace flatcam::gui
