/**
 * @file FlatCAMInfoBar.h
 * @brief Zona de mensajes de la barra de estado (port de `FlatCAMInfoBar` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QLabel>
#include <QPixmap>
#include <QWidget>

namespace flatcam::gui {

/**
 * @brief Luz de color y texto del último mensaje de la aplicación.
 *
 * El color depende del nivel: rojo para errores, verde para éxito, amarillo para advertencias, azul para selección y
 * gris para lo demás.
 */
class FlatCAMInfoBar : public QWidget {
    Q_OBJECT
public:
    /** @brief Crea la zona con el mensaje "Aplicación iniciada ...". @param parent Widget padre. */
    explicit FlatCAMInfoBar(QWidget* parent = nullptr);

    /**
     * @brief Muestra un mensaje con su luz (`set_status`).
     * @param text Mensaje.
     * @param level `"info"`, `"success"`, `"selected"`, `"WARNING"`, `"WARNING_NOTCL"`, `"ERROR"` o `"ERROR_NOTCL"`.
     */
    void setStatus(const QString& text, const QString& level = "info");

    /**
     * @brief Cambia el texto (y su tooltip) sin tocar la luz (`set_text_`).
     * @param text Mensaje.
     * @param color Color CSS opcional del texto.
     */
    void setText(const QString& text, const QString& color = QString());

    /** @brief Si es `true`, la luz no cambia con los mensajes (`lock_pmaps`). @param lock Bloquear. */
    void setLockPixmaps(bool lock) { lockPmaps_ = lock; }

    QLabel* icon;  ///< Luz de color (`icon`).
    QLabel* text;  ///< Texto del mensaje (`text`).

private:
    QPixmap pmap_;
    bool lockPmaps_{false};
};

}  // namespace flatcam::gui
