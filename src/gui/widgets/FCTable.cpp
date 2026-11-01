#include "gui/widgets/FCTable.h"

#include <QHeaderView>
#include <QMouseEvent>

#include "gui/widgets/FCEntry.h"

namespace flatcam::gui {

FCTable::FCTable(QWidget* parent) : QTableWidget(parent) {
    QPalette pal;
    pal.setColor(QPalette::Inactive, QPalette::Highlight, pal.color(QPalette::Active, QPalette::Highlight));
    pal.setColor(QPalette::Inactive, QPalette::HighlightedText, pal.color(QPalette::Active, QPalette::HighlightedText));
    setPalette(pal);
}

QSize FCTable::sizeHint() const { return QSize(FCEntry::kEditSizeHint, QTableWidget::sizeHint().height()); }

int FCTable::contentHeight() const {
    int h = horizontalHeader()->height();
    for (int i = 0; i < rowCount(); ++i) h += rowHeight(i);
    return h;
}

int FCTable::contentWidth() const {
    int w = verticalHeader()->width();
    for (int i = 0; i < columnCount(); ++i) w += columnWidth(i);
    return w;
}

void FCTable::mousePressEvent(QMouseEvent* ev) {
    if (!itemAt(ev->pos())) {
        clearSelection();
        clearFocus();
    } else {
        QTableWidget::mousePressEvent(ev);
    }
}

void FCTable::focusOutEvent(QFocusEvent* ev) {
    Q_EMIT lostFocus();
    QTableWidget::focusOutEvent(ev);
}

}  // namespace flatcam::gui
