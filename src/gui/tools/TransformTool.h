/**
 * @file TransformTool.h
 * @brief Herramienta "Transformar objeto" (port de `appTools/ToolTransform.py`): rotar, inclinar, escalar, desplazar,
 * reflejar y engrosar los objetos elegidos.
 */
#pragma once

#include <optional>

#include "core/Types.h"
#include "gui/generated/TransformToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;

/**
 * @brief Transformaciones de los objetos elegidos del Proyecto, alrededor de una referencia: el origen, el centro de la
 * selección, un punto o el centro de otro objeto. Los CNC Job no se transforman (avisa, como el original).
 */
class TransformTool : public TransformToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit TransformTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Valores de fábrica (`set_tool_ui`). */
    void setToolUi();

    /** @brief Rotar (`on_rotate`). */
    void onRotate();
    /** @brief Inclinar en X (`on_skewx`). */
    void onSkewX();
    /** @brief Inclinar en Y (`on_skewy`). */
    void onSkewY();
    /** @brief Escalar en X (`on_scalex`). */
    void onScaleX();
    /** @brief Escalar en Y (`on_scaley`). */
    void onScaleY();
    /** @brief Desplazar en X (`on_offx`). */
    void onOffsetX();
    /** @brief Desplazar en Y (`on_offy`). */
    void onOffsetY();
    /** @brief Reflejar las X (`on_flipx`, eje Y). */
    void onFlipX();
    /** @brief Reflejar las Y (`on_flipy`, eje X). */
    void onFlipY();
    /** @brief Engrosar una distancia (`on_buffer_by_distance`). */
    void onBufferDistance();
    /** @brief Engrosar un porcentaje (`on_buffer_by_factor`). */
    void onBufferFactor();

private:
    std::optional<Point2D> reference();
    void onReferenceChanged(int index);
    void fillObjects();
    void scaleAction(char axis, double fx, double fy);
    void skewAction(char axis, double ax, double ay);
    void bufferAction(double value, bool rounded, bool factor);
    App* app_;
};

}  // namespace flatcam::gui
