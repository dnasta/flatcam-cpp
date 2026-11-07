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

QTreeWidgetItem* FCTree::addParent(QTreeWidgetItem* parent, const QString& title, bool expanded, const QColor& color,
                                   const std::optional<QFont>& font) {
    auto* item = new QTreeWidgetItem(parent ? parent : invisibleRootItem(), QStringList{title});
    item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    item->setExpanded(expanded);
    if (color.isValid()) item->setForeground(0, QBrush(color));
    if (font) item->setFont(0, *font);
    return item;
}

void FCTree::addChild(QTreeWidgetItem* parent, const QStringList& title, bool column1, const std::optional<QFont>& font,
                      std::vector<int> fontItems) {
    auto* item = new QTreeWidgetItem(parent ? parent : invisibleRootItem());
    item->setText(0, title.value(0));
    if (column1) item->setText(1, title.value(1));
    if (font)
        for (int fi : fontItems) item->setFont(fi, *font);
}

void FCTree::onResize() {
    QHeaderView* h = header();
    for (int c = 0; c < h->count(); ++c) {
        h->setSectionResizeMode(c, QHeaderView::ResizeToContents);
        const int w = h->sectionSize(c);
        h->setSectionResizeMode(c, QHeaderView::Interactive);
        h->resizeSection(c, w);
    }
}

void FCTree::resizeEvent(QResizeEvent* event) {
    QTreeWidget::resizeEvent(event);
    onResize();
}

}  // namespace flatcam::gui
