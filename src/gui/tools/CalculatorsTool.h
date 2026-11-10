/**
 * @file CalculatorsTool.h
 * @brief Herramienta Calculadoras (port de `appTools/ToolCalculators.py`): diámetro efectivo de una fresa en V,
 * milímetros a pulgadas y corriente y tiempo de un galvanizado.
 */
#pragma once

#include "gui/generated/CalculatorsToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;

/** @brief Panel y lógica de las calculadoras, como el original. */
class CalculatorsTool : public CalculatorsToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit CalculatorsTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /**
     * @brief `on_calculate_tool_dia`: diámetro de la punta + 2 * profundidad * tan(ángulo / 2), con los decimales de la
     * aplicación.
     */
    void calculateToolDia();
    /** @brief `on_calculate_inch_units`: los milímetros escritos, en pulgadas (con los decimales de la aplicación). */
    void calculateInch();
    /** @brief `on_calculate_mm_units`: las pulgadas escritas, en milímetros. */
    void calculateMm();
    /**
     * @brief `on_calculate_eplate`: corriente = área (dm², de largo por ancho en cm o del área) * densidad *
     * 0,0021527820833419 y tiempo = crecimiento * 2,142857142857143 * 20 / densidad.
     */
    void calculateElectroplating();

private:
    void onAreaRadio(const QString& v);
    App* app_;
};

}  // namespace flatcam::gui
