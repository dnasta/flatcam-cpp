/**
 * @file VerticalScrollArea.h
 * @brief Área con desplazamiento solo vertical (port de `VerticalScrollArea` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QScrollArea>

namespace flatcam::gui {

/**
 * @brief `QScrollArea` que solo se desplaza en vertical y se ensancha para que entre su contenido.
 *
 * Es el contenedor de las pestañas Propiedades y Herramienta: cuando el contenido cambia de tamaño, el ancho mínimo del
 * área pasa a ser el ancho preferido del contenido más el de la barra de desplazamiento.
 */
class VerticalScrollArea : public QScrollArea {
    Q_OBJECT
public:
    /** @brief Crea el área. @param parent Widget padre. */
    explicit VerticalScrollArea(QWidget* parent = nullptr);

protected:
    /**
     * @brief Ajusta el ancho mínimo cuando cambia el tamaño del contenido.
     * @param source Objeto observado.
     * @param event Evento.
     * @return `false` (el evento sigue su curso).
     */
    bool eventFilter(QObject* source, QEvent* event) override;
};

}  // namespace flatcam::gui
