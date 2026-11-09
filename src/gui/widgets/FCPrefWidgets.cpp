#include "gui/widgets/FCPrefWidgets.h"

#include <QColorDialog>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>
#include <QTabBar>

namespace flatcam::gui {

FCColorEntry::FCColorEntry(QWidget* parent) : QFrame(parent) {
    entry = new FCEntry();
    entry->setValidator(new QRegularExpressionValidator(QRegularExpression("[#A-Fa-f0-9]*"), entry));
    button = new QPushButton();
    button->setFixedSize(15, 15);
    button->setStyleSheet("border-color: dimgray;");
    auto* lay = new QHBoxLayout();
    lay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(entry);
    lay->addWidget(button);
    setLayout(lay);
    connect(entry, &QLineEdit::editingFinished, this, [this] {
        syncButtonColor();
        Q_EMIT editingFinished();
    });
    connect(button, &QPushButton::clicked, this, [this] { onButtonClicked(); });
}

void FCColorEntry::setValue(const QString& v) {
    entry->setText(v);
    syncButtonColor();
}

void FCColorEntry::syncButtonColor() { button->setStyleSheet("background-color:" + value().left(7) + ";"); }

void FCColorEntry::onButtonClicked() {
    const QString v = value();
    const QColor c = QColorDialog::getColor(QColor(v.left(7)), this, QString(), QColorDialog::ShowAlphaChannel);
    if (!c.isValid()) return;
    setValue(c.name() + v.mid(7, 2));  // el color elegido y la transparencia escrita
}

FCSliderWithSpinner::FCSliderWithSpinner(int min, int max, int step, QWidget* parent) : QFrame(parent) {
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setSingleStep(step);
    spinner = new FCSpinner();
    spinner->setRange(min, max);
    spinner->setSingleStep(step);
    spinner->setMinimumWidth(70);
    spinner->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    auto* lay = new QHBoxLayout();
    lay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(slider);
    lay->addWidget(spinner);
    setLayout(lay);
    connect(slider, &QSlider::valueChanged, spinner, [this](int v) { spinner->setValue(v); });
    connect(spinner, &QSpinBox::valueChanged, this, [this](int v) {
        slider->setValue(v);
        Q_EMIT valueChanged(v);
    });
}

FCTab::FCTab(QWidget* parent) : QTabWidget(parent) {
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, [this](int i) { removeTab(i); });
}

void FCTab::protectTab(int index) { tabBar()->setTabButton(index, QTabBar::RightSide, nullptr); }

}  // namespace flatcam::gui
