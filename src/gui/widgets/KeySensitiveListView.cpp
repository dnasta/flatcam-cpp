#include "gui/widgets/KeySensitiveListView.h"

#include <QKeyEvent>

namespace flatcam::gui {

KeySensitiveListView::KeySensitiveListView(QWidget* parent) : QTreeView(parent) {
    setHeaderHidden(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);  // no se editan los nombres en el árbol
    setAcceptDrops(true);
}

void KeySensitiveListView::keyPressEvent(QKeyEvent* event) { emit keyPressed(event->key()); }

}  // namespace flatcam::gui
