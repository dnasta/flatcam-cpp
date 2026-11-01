#include "gui/widgets/FCLabel.h"

#include <QMouseEvent>

namespace flatcam::gui {

FCLabel::FCLabel(QWidget* parent) : QLabel(parent) {}

void FCLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        clickedState_ = !clickedState_;
        emit clicked(clickedState_);
    } else if (event->button() == Qt::RightButton) {
        rightClickedState_ = !rightClickedState_;
        emit rightClicked(true);
    } else if (event->button() == Qt::MiddleButton) {
        middleClickedState_ = !middleClickedState_;
        emit middleClicked(true);
    }
}

}  // namespace flatcam::gui
