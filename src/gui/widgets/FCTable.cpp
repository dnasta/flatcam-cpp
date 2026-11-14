#include "gui/widgets/FCTable.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QHeaderView>
#include <set>

#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
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
        rowsDragged_.clear();
        for (const QTableWidgetItem* it : selectedItems()) rowsDragged_ << it->row();
        QTableWidget::mousePressEvent(ev);
    }
}

void FCTable::setDragDrop(bool on) {
    dragDrop_ = on;
    if (!on) return;
    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    setDragDropOverwriteMode(false);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void FCTable::dragEnterEvent(QDragEnterEvent* e) {
    if (!dragDrop_) return QTableWidget::dragEnterEvent(e);
    // Como el original: las señales quedan bloqueadas hasta soltar (si se cancela el arrastre, siguen bloqueadas).
    if (e->source() == this) {
        blockSignals(true);
        e->accept();
    } else {
        e->ignore();
    }
}

int FCTable::dropOn(const QPoint& pos) const {
    const QModelIndex index = indexAt(pos);
    if (!index.isValid()) return rowCount();
    // is_below
    const QRect rect = visualRect(index);
    constexpr int margin = 2;
    bool below;
    if (pos.y() - rect.top() < margin) below = false;
    else if (rect.bottom() - pos.y() < margin) below = true;
    else below = rect.contains(pos, true) && !(model()->flags(index) & Qt::ItemIsDropEnabled) &&
                 pos.y() >= rect.center().y();
    return below ? index.row() + 1 : index.row();
}

void FCTable::moveSelectedRows(int dropRow) {
    std::set<int> rowSet;
    for (const QTableWidgetItem* it : selectedItems()) rowSet.insert(it->row());
    const std::vector<int> rows(rowSet.begin(), rowSet.end());
    struct Cell {
        QTableWidgetItem* item{nullptr};
        QWidget* widget{nullptr};
    };
    std::vector<std::vector<Cell>> moved;
    for (int r : rows) {
        std::vector<Cell> line;
        for (int c = 0; c < columnCount(); ++c) {
            Cell cell;
            if (const QTableWidgetItem* it = item(r, c)) {
                cell.item = new QTableWidgetItem(*it);
            } else if (auto* cb = qobject_cast<QComboBox*>(cellWidget(r, c))) {
                auto* n = new FCComboBox();
                for (int i = 0; i < cb->count(); ++i) n->addItem(cb->itemText(i));
                n->setCurrentIndex(cb->currentIndex());
                cell.widget = n;
            } else if (auto* ck = qobject_cast<QCheckBox*>(cellWidget(r, c))) {
                auto* n = new FCCheckBox();
                n->setChecked(ck->isChecked());
                n->setText(ck->text());
                cell.widget = n;
            }
            line.push_back(cell);
        }
        moved.push_back(std::move(line));
    }
    for (auto it = rows.rbegin(); it != rows.rend(); ++it) {
        removeRow(*it);
        if (*it < dropRow) --dropRow;
    }
    for (std::size_t i = 0; i < moved.size(); ++i) {
        const int r = dropRow + static_cast<int>(i);
        insertRow(r);
        for (int c = 0; c < static_cast<int>(moved[i].size()); ++c) {
            if (moved[i][c].item) setItem(r, c, moved[i][c].item);
            else if (moved[i][c].widget) setCellWidget(r, c, moved[i][c].widget);
        }
    }
    for (std::size_t i = 0; i < moved.size(); ++i) {
        const int r = dropRow + static_cast<int>(i);
        if (QTableWidgetItem* a = item(r, 0)) a->setSelected(true);
        if (QTableWidgetItem* b = item(r, 1)) b->setSelected(true);
    }
    blockSignals(false);
    Q_EMIT dragDropped(rowsDragged_, dropRow);
}

void FCTable::dropEvent(QDropEvent* e) {
    if (!dragDrop_) return QTableWidget::dropEvent(e);
    if (!e->isAccepted() && e->source() == this) {
        moveSelectedRows(dropOn(e->position().toPoint()));
        e->accept();
    }
    blockSignals(false);
    resizeRowsToContents();
    // Desvío: el original llama además a QTableWidget.dropEvent con el evento ya aceptado; acá no, para que Qt no mueva
    // las celdas otra vez encima de las ya movidas.
}

void FCTable::focusOutEvent(QFocusEvent* ev) {
    Q_EMIT lostFocus();
    QTableWidget::focusOutEvent(ev);
}

}  // namespace flatcam::gui
