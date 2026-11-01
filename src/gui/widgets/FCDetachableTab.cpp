#include "gui/widgets/FCDetachableTab.h"

#include <QCloseEvent>
#include <QMouseEvent>
#include <QVBoxLayout>

namespace flatcam::gui {

namespace {

/// Ventana de una pestaña desprendida (`FCDetachableTab.FCDetachedTab`): al cerrarse devuelve el contenido.
class DetachedTab : public QMainWindow {
public:
    DetachedTab(const QString& name, QWidget* content, FCDetachableTab* owner, const QIcon& icon, const QString& text)
        : owner_(owner), content_(content), icon_(icon), text_(text) {
        setObjectName(name);
        setWindowTitle(name);
        setCentralWidget(content);
        content->show();
    }

protected:
    void closeEvent(QCloseEvent* event) override {
        if (owner_ && content_) {
            takeCentralWidget();
            owner_->addTab(content_, icon_, text_);
            owner_->setCurrentWidget(content_);
            Q_EMIT owner_->tabAttached(text_);
        }
        event->accept();
        deleteLater();
    }

private:
    QPointer<FCDetachableTab> owner_;
    QPointer<QWidget> content_;
    QIcon icon_;
    QString text_;
};

}  // namespace

FCTabBar::FCTabBar(QWidget* parent) : QTabBar(parent) {
    setAcceptDrops(true);
    setElideMode(Qt::ElideRight);
    setSelectionBehaviorOnRemove(QTabBar::SelectLeftTab);
}

void FCTabBar::mouseDoubleClickEvent(QMouseEvent* event) {
    event->accept();
    Q_EMIT onDetachTabSignal(tabAt(event->pos()), event->globalPosition().toPoint());
}

void FCTabBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) Q_EMIT onCloseTabSignal(tabAt(event->pos()));
    else if (event->button() == Qt::RightButton) Q_EMIT rightClick(tabAt(event->pos()));
    QTabBar::mousePressEvent(event);
}

FCDetachableTab::FCDetachableTab(QWidget* parent, bool protect) : QTabWidget(parent), protect_(protect) {
    bar_ = new FCTabBar(this);
    setTabBar(bar_);
    connect(bar_, &FCTabBar::onCloseTabSignal, this, [this](int i) { onCloseTabMiddleButton(i); });
    connect(bar_, &FCTabBar::onDetachTabSignal, this, [this](int i, QPoint p) {
        if (detachable_) detachTab(i, p);
    });
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, [this](int i) { closeTab(i); });
}

void FCDetachableTab::protectTab(int index) {
    // El original quita el botón de la derecha (Windows y Linux); en macOS el estilo lo pone a la izquierda.
    bar_->setTabButton(index, QTabBar::RightSide, nullptr);
    bar_->setTabButton(index, QTabBar::LeftSide, nullptr);
}

bool FCDetachableTab::isClosable(int index) const {
    return bar_->tabButton(index, QTabBar::RightSide) != nullptr || bar_->tabButton(index, QTabBar::LeftSide) != nullptr;
}

void FCDetachableTab::deleteTab(int index) {
    if (QWidget* w = widget(index)) w->deleteLater();
    removeTab(index);
}

void FCDetachableTab::closeTab(int index) {
    if (protect_) return;
    removeTab(index);
}

void FCDetachableTab::onCloseTabMiddleButton(int index) {
    // Las pestañas protegidas no tienen botón de cerrar.
    if (index >= 0 && isClosable(index)) removeTab(index);
}

void FCDetachableTab::detachTab(int index, QPoint globalPos) {
    if (index < 0) return;
    const QString text = tabText(index);
    QIcon icon = tabIcon(index);
    if (icon.isNull()) icon = window()->windowIcon();
    QWidget* content = widget(index);
    const QRect rect = content->frameGeometry();
    removeTab(index);
    auto* win = new DetachedTab("FlatCAM " + text, content, this, icon, text);
    win->setWindowIcon(icon);
    win->setGeometry(QRect(globalPos, rect.size()));
    win->show();
    Q_EMIT tabDetached(text);
}

void FCDetachableTab2::closeTab(int index) {
    QWidget* w = widget(index);
    Q_EMIT tabClosed(w ? w->objectName() : QString(), index);
    removeTab(index);
}

void FCDetachableTab2::onCloseTabMiddleButton(int index) {
    if (index >= 0 && isClosable(index)) closeTab(index);
}

}  // namespace flatcam::gui
