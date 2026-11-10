/**
 * @file EtchTool.h
 * @brief Herramienta Compensación de grabado (port de `appTools/ToolEtchCompensation.py`).
 */
#pragma once

#include "gui/generated/EtchToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Compensación de grabado: un Gerber `<nombre>_comp` con el cobre agrandado lo que se come el
 * grabado lateral (por factor, por grabador o escrito a mano), y los conversores de onzas y mils a micras.
 */
class EtchTool : public EtchToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit EtchTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`): espesor 18 y "Factor de grabado". */
    void setToolUi();
    /** @brief "Compensar" (`on_compensate`). @return El objeto nuevo, o `nullptr`. */
    FlatCAMObj* compensate();

private:
    /** @brief Muestra los campos del tipo elegido (`on_ratio_change`). @param val `factor`, `etch_list` o `manual`. */
    void onRatioChange(const QString& val);
    /** @brief Convierte un campo con @p factor a micras en @p out (`on_oz_conversion`, `on_mils_conversion`). */
    void convert(const QString& txt, double factor, FCEntry* out);

    App* app_;
};

}  // namespace flatcam::gui
