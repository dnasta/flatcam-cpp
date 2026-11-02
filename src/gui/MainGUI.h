/**
 * @file MainGUI.h
 * @brief Ventana principal de FlatCAM: la interfaz generada más su lógica (port de `MainGUI.py` y de `app_Main.py`).
 */
#pragma once

#include "gui/generated/MainGUIBase.h"

namespace flatcam::gui {

class PlotCanvas;

/**
 * @brief Ventana principal: arma la interfaz idéntica a la del original (@ref MainGUIBase) y le agrega el canvas y el
 * comportamiento.
 *
 * Por ahora cubre el arranque (geometría de la primera ejecución, panel izquierdo oculto, ícono de la ventana), el canvas
 * en "Área de la parcela", las coordenadas del cursor en la barra de estado y el recuadro HUD, y los botones
 * "Panel de palanca", HUD, ejes y salir. Abrir archivos y las herramientas vienen después.
 */
class MainGUI : public MainGUIBase {
    Q_OBJECT
public:
    /** @brief Crea la ventana. @param parent Widget padre. */
    explicit MainGUI(QWidget* parent = nullptr);

    /** @brief El canvas del área de dibujo. @return El canvas. */
    PlotCanvas* plotCanvas() const { return plotcanvas_; }

    /** @brief Muestra u oculta el panel izquierdo (`on_toggle_notebook`). */
    void onToggleNotebook();

    /**
     * @brief Cambia el título de la ventana (`set_ui_title`).
     * @param name Nombre del proyecto (por ejemplo "Proyecto nuevo: no guardado").
     */
    void setUiTitle(const QString& name);

    /**
     * @brief Muestra un mensaje en la barra de estado (`App.inform` → `FlatCAMInfoBar.set_status`).
     * @param msg Mensaje; si empieza con `[success]`, `[WARNING]`, `[ERROR]`, etc. se usa como nivel, como en el original.
     */
    void inform(const QString& msg);

    /**
     * @brief Ajusta una posición a la grilla si está activo "Ajustar a la grilla" (`App.grid_status` y
     * `AppGeoEditor.snap` con solo la grilla): `round(x / gx) * gx` con el redondeo de Python (al par en los empates);
     * con "enlazar" la Y usa la grilla X; una grilla 0 no ajusta.
     * @param p Posición.
     * @return La posición ajustada.
     */
    QPointF snapToGrid(const QPointF& p) const;

private:
    void updateCoordinates(QPointF world);
    void connectSignals();

    PlotCanvas* plotcanvas_{nullptr};
    QPointF relPoint1_{0, 0};  ///< Punto desde donde se miden Dx y Dy (`rel_point1`).
};

}  // namespace flatcam::gui
