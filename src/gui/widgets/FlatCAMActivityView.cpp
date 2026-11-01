#include "gui/widgets/FlatCAMActivityView.h"

#include <QHBoxLayout>

#include "gui/Translation.h"

namespace flatcam::gui {

FlatCAMActivityView::FlatCAMActivityView(QWidget* parent) : QWidget(parent) {
    // global_activity_icon = "Ball green"
    const QString iconPath = ":/images/active_2_static.png";
    setMinimumWidth(200);

    icon = new FCLabel(this);
    icon->setGeometry(0, 0, 16, 12);
    movie_ = new QMovie(":/images/active_2.gif", QByteArray(), this);
    icon->setMovie(movie_);

    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);
    layout->addWidget(icon);

    text = new QLabel(this);
    text->setText(tr_("Idle."));
    icon->setPixmap(QPixmap(iconPath));
    layout->addWidget(text);
}

void FlatCAMActivityView::setIdle() {
    movie_->stop();
    text->setText(tr_("Idle."));
}

void FlatCAMActivityView::setBusy(const QString& msg, bool noMovie) {
    if (!noMovie) {
        icon->setMovie(movie_);
        movie_->start();
    }
    text->setText(msg);
}

}  // namespace flatcam::gui
