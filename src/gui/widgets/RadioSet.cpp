#include "gui/widgets/RadioSet.h"

#include <QBoxLayout>
#include <QButtonGroup>

namespace flatcam::gui {

RadioSet::RadioSet(std::vector<Choice> choices, Qt::Orientation orientation, bool stretch, QWidget* parent)
    : QWidget(parent), choices_(std::move(choices)) {
    auto* layout = orientation == Qt::Horizontal ? static_cast<QBoxLayout*>(new QHBoxLayout())
                                                 : static_cast<QBoxLayout*>(new QVBoxLayout());
    auto* group = new QButtonGroup(this);
    for (const Choice& c : choices_) {
        auto* rb = new QRadioButton(c.label);
        group->addButton(rb);
        layout->addWidget(rb, 0);
        buttons_.push_back(rb);
        connect(rb, &QRadioButton::toggled, this, [this](bool on) {
            if (on) Q_EMIT activatedCustom(value().toString());
        });
    }
    layout->setContentsMargins(0, 0, 0, 0);
    if (stretch) layout->addStretch();
    setLayout(layout);
}

QVariant RadioSet::value() const {
    for (std::size_t i = 0; i < choices_.size(); ++i)
        if (buttons_[i]->isChecked()) return choices_[i].value;
    return {};
}

void RadioSet::setValue(const QVariant& val) {
    for (std::size_t i = 0; i < choices_.size(); ++i) {
        if (choices_[i].value == val) {
            buttons_[i]->setChecked(true);
            return;
        }
    }
}

void RadioSet::setOptionsDisabled(const QStringList& labels, bool disabled) {
    for (std::size_t i = 0; i < choices_.size(); ++i)
        if (labels.contains(choices_[i].label)) buttons_[i]->setDisabled(disabled);
}

}  // namespace flatcam::gui
