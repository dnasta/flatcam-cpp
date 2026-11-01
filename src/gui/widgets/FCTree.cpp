#include "gui/widgets/FCTree.h"

#include <QHeaderView>

namespace flatcam::gui {

FCTree::FCTree(QWidget* parent, int columns, bool headerHidden, bool extendedSel, int protectedColumn)
    : QTreeWidget(parent), protectedColumn_(protectedColumn) {
    setColumnCount(columns);
    setHeaderHidden(headerHidden);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    // La selección se ve igual con la ventana activa o inactiva.
    QPalette pal;
    pal.setColor(QPalette::Inactive, QPalette::Highlight, pal.color(QPalette::Active, QPalette::Highlight));
    setPalette(pal);

    if (extendedSel) setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &FCTree::onDoubleClick);
}

void FCTree::onDoubleClick(QTreeWidgetItem* item, int column) {
    // Solo una columna editable: https://stackoverflow.com/questions/2801959
    const Qt::ItemFlags flags = item->flags();
    const bool editable = protectedColumn_ < 0 || column != protectedColumn_;
    if (editable) item->setFlags(flags | Qt::ItemIsEditable);
    else if (flags & Qt::ItemIsEditable) item->setFlags(flags ^ Qt::ItemIsEditable);
}

QTreeWidgetItem* FCTree::addItem(QTreeWidgetItem* parent, const QStringList& texts, bool bold) {
    auto* item = parent ? new QTreeWidgetItem(parent, texts) : new QTreeWidgetItem(this, texts);
    if (bold) {
        QFont f = item->font(0);
        f.setBold(true);
        item->setFont(0, f);
    }
    return item;
}

}  // namespace flatcam::gui
