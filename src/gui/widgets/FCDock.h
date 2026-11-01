/**
 * @file FCDock.h
 * @brief Panel acoplable (port de `FCDock` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QDockWidget>
#include <functional>

namespace flatcam::gui {

/** @brief `QDockWidget` que avisa al cerrarse y que se vuelve a acoplar al mostrarse. */
class FCDock : public QDockWidget {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param parent Widget padre. */
    explicit FCDock(QWidget* parent = nullptr) : QDockWidget(parent) {}

    /** @brief Función que se llama al cerrar el panel (`close_callback`). @param cb Función. */
    void setCloseCallback(std::function<void()> cb) { closeCallback_ = std::move(cb); }

    /** @brief Muestra el panel, acoplándolo si estaba flotando (`show`). */
    void showDocked() {
        if (isFloating()) setFloating(false);
        show();
    }

protected:
    /** @brief Llama al aviso de cierre. @param event Evento. */
    void closeEvent(QCloseEvent* event) override {
        if (closeCallback_) closeCallback_();
        QDockWidget::closeEvent(event);
    }

private:
    std::function<void()> closeCallback_;
};

}  // namespace flatcam::gui
