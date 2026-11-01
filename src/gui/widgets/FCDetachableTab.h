/**
 * @file FCDetachableTab.h
 * @brief Pestañas que se cierran y se desprenden (port de `FCDetachableTab` y `FCDetachableTab2` de `GUIElements.py`).
 */
#pragma once

#include <QIcon>
#include <QMainWindow>
#include <QPointer>
#include <QTabBar>
#include <QTabWidget>

namespace flatcam::gui {

/**
 * @brief Barra de pestañas de @ref FCDetachableTab (`FCDetachableTab.FCTabBar`).
 *
 * Avisa del doble clic (desprender), del clic del medio (cerrar) y del clic derecho.
 */
class FCTabBar : public QTabBar {
    Q_OBJECT
public:
    /** @brief Crea la barra. @param parent Widget padre. */
    explicit FCTabBar(QWidget* parent = nullptr);

signals:
    /** @brief Doble clic sobre una pestaña. @param index Pestaña. @param globalPos Posición en pantalla. */
    void onDetachTabSignal(int index, QPoint globalPos);
    /** @brief Clic del medio sobre una pestaña. @param index Pestaña. */
    void onCloseTabSignal(int index);
    /** @brief Clic derecho sobre una pestaña. @param index Pestaña. */
    void rightClick(int index);

protected:
    /** @brief Pide desprender la pestaña. @param event Evento. */
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    /** @brief Clic del medio y derecho. @param event Evento. */
    void mousePressEvent(QMouseEvent* event) override;
};

/**
 * @brief Pestañas de FlatCAM: se cierran (salvo las protegidas), con el botón del medio o el de la pestaña, y se
 * desprenden con doble clic a una ventana propia que, al cerrarse, devuelve la pestaña.
 *
 * Desvío consciente: el original además permite arrastrar una pestaña fuera de la barra y soltarla en otra ventana de
 * pestañas; acá se desprende con doble clic (lo mismo que hace el original con doble clic).
 */
class FCDetachableTab : public QTabWidget {
    Q_OBJECT
public:
    /**
     * @brief Crea las pestañas.
     * @param parent Widget padre.
     * @param protect Si es `true`, ninguna pestaña se puede cerrar.
     */
    explicit FCDetachableTab(QWidget* parent = nullptr, bool protect = false);

    /** @brief Quita el botón de cerrar de una pestaña (`protectTab`). @param index Pestaña. */
    void protectTab(int index);

    /** @brief Si la pestaña tiene botón de cerrar (no está protegida). @param index Pestaña. @return `true` si se puede cerrar. */
    bool isClosable(int index) const;

    /** @brief Cierra y destruye el contenido de una pestaña (`deleteTab`). @param index Pestaña. */
    void deleteTab(int index);

    /** @brief Habilita o no desprender pestañas (`set_detachable`). @param val Habilitar. */
    void setDetachable(bool val) { detachable_ = val; }

    /** @brief La barra de pestañas propia. @return La barra. */
    FCTabBar* fcTabBar() const { return bar_; }

public slots:
    /** @brief Cierra una pestaña (`closeTab`). @param index Pestaña. */
    virtual void closeTab(int index);

signals:
    /** @brief Se desprendió una pestaña. @param name Nombre de la pestaña. */
    void tabDetached(const QString& name);
    /** @brief Volvió una pestaña desprendida. @param name Nombre de la pestaña. */
    void tabAttached(const QString& name);

protected:
    /** @brief Clic del medio: cierra si la pestaña no está protegida. @param index Pestaña. */
    virtual void onCloseTabMiddleButton(int index);

private:
    void detachTab(int index, QPoint globalPos);

    FCTabBar* bar_;
    bool protect_{false};
    bool detachable_{true};
};

/** @brief Igual que @ref FCDetachableTab, pero avisa con el nombre del objeto de la pestaña al cerrarla. */
class FCDetachableTab2 : public FCDetachableTab {
    Q_OBJECT
public:
    /** @brief Crea las pestañas. @param parent Widget padre. @param protect Ninguna pestaña se puede cerrar. */
    explicit FCDetachableTab2(QWidget* parent = nullptr, bool protect = false) : FCDetachableTab(parent, protect) {}

public slots:
    /** @brief Avisa con @ref tabClosed y cierra. @param index Pestaña. */
    void closeTab(int index) override;

signals:
    /** @brief Se cerró una pestaña. @param objectName Nombre del widget de la pestaña. @param index Pestaña. */
    void tabClosed(const QString& objectName, int index);

protected:
    /** @brief Clic del medio: cierra (con aviso) si no está protegida. @param index Pestaña. */
    void onCloseTabMiddleButton(int index) override;
};

}  // namespace flatcam::gui
