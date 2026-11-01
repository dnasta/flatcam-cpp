#include "gui/widgets/FCLabel.h"

#include <QMouseEvent>

namespace flatcam::gui {

FCLabel::FCLabel(QWidget* parent) : QLabel(parent) {}

void FCLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        clickedState_ = !clickedState_;
        Q_EMIT clicked(clickedState_);
    } else if (event->button() == Qt::RightButton) {
        rightClickedState_ = !rightClickedState_;
        Q_EMIT rightClicked(true);
    } else if (event->button() == Qt::MiddleButton) {
        middleClickedState_ = !middleClickedState_;
        Q_EMIT middleClicked(true);
    }
}

}  // namespace flatcam::gui
