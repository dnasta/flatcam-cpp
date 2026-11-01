#include "gui/widgets/FlatCAMInfoBar.h"

#include <QHBoxLayout>

#include "gui/Translation.h"

namespace flatcam::gui {

FlatCAMInfoBar::FlatCAMInfoBar(QWidget* parent) : QWidget(parent) {
    icon = new QLabel(this);
    icon->setGeometry(0, 0, 12, 12);
    pmap_ = QPixmap(":/images/graylight12.png");
    icon->setPixmap(pmap_);

    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(5, 0, 5, 0);
    setLayout(layout);
    layout->addWidget(icon);

    text = new QLabel(this);
    text->setText(tr_("Application started ..."));
    text->setToolTip(tr_("Hello!"));
    layout->addWidget(text);
    layout->addStretch();
}

void FlatCAMInfoBar::setText(const QString& t, const QString& color) {
    text->setText(t);
    text->setToolTip(t);
    if (!color.isEmpty()) text->setStyleSheet("color: " + color);
}

void FlatCAMInfoBar::setStatus(const QString& t, const QString& level) {
    if (!lockPmaps_) {
        if (level == "ERROR" || level == "ERROR_NOTCL") pmap_ = QPixmap(":/images/redlight12.png");
        else if (level.toLower() == "success") pmap_ = QPixmap(":/images/greenlight12.png");
        else if (level == "WARNING" || level == "WARNING_NOTCL") pmap_ = QPixmap(":/images/yellowlight12.png");
        else if (level.toLower() == "selected") pmap_ = QPixmap(":/images/bluelight12.png");
        else pmap_ = QPixmap(":/images/graylight12.png");
    }
    setText(t);
    icon->setPixmap(pmap_);
}

}  // namespace flatcam::gui
