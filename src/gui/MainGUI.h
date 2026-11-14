/**
 * @file MainGUI.h
 * @brief Ventana principal de FlatCAM: la interfaz generada más su lógica (port de `MainGUI.py` y de `app_Main.py`).
 */
#pragma once

#include <QRect>
#include <QToolBar>
#include <vector>

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
    /** @brief Pantalla completa sin marco, sin barras de herramientas (`on_fullscreen`). @param disable Salir de ella. */
    void onFullscreen(bool disable = false);
    /** @brief Saca o vuelve a poner la pestaña "Área de dibujo" (`on_toggle_plotarea`). */
    void onTogglePlotArea();
    /** @brief Ajuste a la grilla (`on_toggle_grid`: dispara el botón de la barra). */
    void onToggleGrid() { grid_snap_btn->trigger(); }
    /** @brief Líneas de la grilla (`on_toggle_grid_lines`). */
    void onToggleGridLines();
    /** @brief Líneas por el origen (`on_toggle_axis`), con el estilo del botón de la barra de estado. */
    void onToggleAxis();
    /** @brief Recuadro HUD (`on_toggle_hud`), con el estilo del botón de la barra de estado. */
    void onToggleHud();

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

    /**
     * @brief Conecta o desconecta los manejadores del mouse de la aplicación (`on_mouse_move_over_plot`,
     * `on_mouse_click_over_plot`): las herramientas que usan el mouse por su cuenta, como Distancia, los desconectan.
     * @param on Conectados.
     */
    void setCanvasHandlersEnabled(bool on) { canvasHandlers_ = on; }
    /** @brief Si los manejadores del mouse de la aplicación están conectados. @return El estado. */
    bool canvasHandlersEnabled() const { return canvasHandlers_; }

    /**
     * @brief Escribe las etiquetas de posición de la barra de estado como `on_mouse_move_over_plot` (y `on_jump_to`):
     * la posición y la distancia a `rel_point1`.
     * @param world Posición en mm.
     */
    void updateCoordinates(QPointF world);

Q_SIGNALS:
    /** @brief Se mostró un mensaje con @ref inform (el texto completo, con su nivel). @param msg Mensaje. */
    void informed(const QString& msg);

private:
    void connectSignals();

    PlotCanvas* plotcanvas_{nullptr};
    bool canvasHandlers_{true};
    bool fullscreen_{false};          ///< `toggle_fscreen`.
    QRect normalGeometry_;            ///< Geometría antes de la pantalla completa.
    std::vector<QToolBar*> hiddenToolbars_;  ///< Barras visibles antes de la pantalla completa.
    QPointF relPoint1_{0, 0};  ///< Punto desde donde se miden Dx y Dy (`rel_point1`).
};

}  // namespace flatcam::gui
