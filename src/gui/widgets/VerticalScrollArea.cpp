#include "gui/widgets/VerticalScrollArea.h"

#include <QEvent>
#include <QScrollBar>

namespace flatcam::gui {

VerticalScrollArea::VerticalScrollArea(QWidget* parent) : QScrollArea(parent) {
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

bool VerticalScrollArea::eventFilter(QObject* source, QEvent* event) {
    // QScrollArea instala este filtro sobre el contenido al llamar a setWidget().
    if (event->type() == QEvent::Resize && source == widget()) {
        setMinimumWidth(widget()->sizeHint().width() + verticalScrollBar()->sizeHint().width());
    }
    return QScrollArea::eventFilter(source, event);
}

}  // namespace flatcam::gui
