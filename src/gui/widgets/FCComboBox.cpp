#include "gui/widgets/FCComboBox.h"

#include <QAbstractItemView>
#include <QMouseEvent>

namespace flatcam::gui {

FCComboBox::FCComboBox(QWidget* parent) : QComboBox(parent) {
    setFocusPolicy(Qt::StrongFocus);
    view()->viewport()->installEventFilter(this);
    view()->setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}

void FCComboBox::setValue(const QString& val) {
    const int idx = findText(val);
    setCurrentIndex(idx == -1 ? 0 : idx);
}

void FCComboBox::wheelEvent(QWheelEvent*) {}

bool FCComboBox::eventFilter(QObject* obj, QEvent* ev) {
    if (ev->type() == QEvent::MouseButtonRelease && static_cast<QMouseEvent*>(ev)->button() == Qt::RightButton) return true;
    return QComboBox::eventFilter(obj, ev);
}

}  // namespace flatcam::gui
