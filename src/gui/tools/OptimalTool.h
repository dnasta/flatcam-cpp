/**
 * @file OptimalTool.h
 * @brief Herramienta Óptimo (port de `appTools/ToolOptimal.py`): la distancia mínima entre las figuras de cobre.
 */
#pragma once

#include <map>
#include <utility>
#include <vector>

#include "core/Types.h"
#include "gui/generated/OptimalToolUIBase.h"
#include "gui/tools/ToolInfo.h"

class QPlainTextEdit;

namespace flatcam::gui {

class App;

/**
 * @brief Panel y lógica de Óptimo: "Encontrar la distancia mínima" calcula la distancia más chica entre las figuras de
 * cobre del Gerber elegido, cuántas veces aparece y dónde; la lista de las demás distancias muestra, al elegir una, sus
 * puntos; "Saltar a la posición elegida" centra la vista en el punto medio de la línea elegida.
 */
class OptimalTool : public OptimalToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit OptimalTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief `find_minimum_distance`. @return Si se calculó. */
    bool findMinimumDistance();
    /**
     * @brief `on_locate_position`: salta al punto medio de los puntos de la línea @p text (`(x0, y0), (x1, y1)`).
     * @param text La línea elegida. @return Si se pudo.
     */
    bool locate(const QString& text);

private:
    QString selectLine(QPlainTextEdit* box);
    App* app_;
    int decimals_{4};
    QString selectedText_;
    QString selectedSecText_;
    std::map<double, std::vector<std::pair<Point2D, Point2D>>> others_;
};

}  // namespace flatcam::gui
