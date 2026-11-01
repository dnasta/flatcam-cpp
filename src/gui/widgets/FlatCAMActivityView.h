/**
 * @file FlatCAMActivityView.h
 * @brief Indicador de actividad de la barra de estado (port de `FlatCAMActivityView` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QLabel>
#include <QMovie>
#include <QWidget>

#include "gui/widgets/FCLabel.h"

namespace flatcam::gui {

/**
 * @brief Ícono animado y texto ("Ocioso." o la tarea en curso) a la derecha de la barra de estado.
 *
 * Usa el ícono "Ball green" (`global_activity_icon`, el valor por defecto de FlatCAM). Un clic en el ícono pide
 * redibujar (`on_toolbar_replot`), a través de la señal `clicked` del ícono.
 */
class FlatCAMActivityView : public QWidget {
    Q_OBJECT
public:
    /** @brief Crea el indicador en reposo. @param parent Widget padre. */
    explicit FlatCAMActivityView(QWidget* parent = nullptr);

    /** @brief Detiene la animación y muestra "Ocioso." (`set_idle`). */
    void setIdle();

    /**
     * @brief Muestra una tarea en curso (`set_busy`).
     * @param msg Texto de la tarea.
     * @param noMovie Si es `true`, no anima el ícono.
     */
    void setBusy(const QString& msg, bool noMovie = false);

    FCLabel* icon;  ///< Ícono (`icon`).
    QLabel* text;   ///< Texto (`text`).

private:
    QMovie* movie_;
};

}  // namespace flatcam::gui
