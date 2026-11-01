/**
 * @file KeySensitiveListView.h
 * @brief Vista del árbol de objetos del proyecto (port de `KeySensitiveListView` de `appObjects/ObjectCollection.py`).
 */
#pragma once

#include <QTreeView>

namespace flatcam::gui {

/**
 * @brief `QTreeView` de la pestaña Proyecto: sin cabecera, sin edición y que avisa de cada tecla presionada.
 *
 * Acepta archivos arrastrados (la ventana principal los abre).
 */
class KeySensitiveListView : public QTreeView {
    Q_OBJECT
public:
    /** @brief Crea la vista. @param parent Widget padre. */
    explicit KeySensitiveListView(QWidget* parent = nullptr);

signals:
    /** @brief Se presionó una tecla. @param key Código de la tecla (`Qt::Key`). */
    void keyPressed(int key);

protected:
    /** @brief Emite @ref keyPressed en vez de procesar la tecla. @param event Evento. */
    void keyPressEvent(QKeyEvent* event) override;
};

}  // namespace flatcam::gui
