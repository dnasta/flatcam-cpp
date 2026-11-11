/**
 * @file ThievingTool.h
 * @brief Robo de cobre (port de `ToolCopperThieving`, Alt+J) sobre @ref flatcam::copperThieving, @ref flatcam::robberBar
 * y @ref flatcam::platingMask.
 */
#pragma once

#include <optional>

#include "core/Shape.h"
#include "gui/generated/ThievingToolUIBase.h"
#include "gui/objects/FlatCAMObj.h"
#include "parsers/Gerber.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;

/**
 * @brief Panel de Robo de cobre: rellena con cobre los espacios libres de un Gerber (sólido, puntos, cuadrados o
 * líneas), agrega una barra de robo alrededor y arma la máscara para el galvanizado.
 *
 * Como el original, el último relleno y la última barra quedan guardados para la máscara (hasta "Reiniciar"), y la
 * lista donde se aplanan los rellenos con líneas no se vacía nunca.
 */
class ThievingTool : public ThievingToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Widget padre. */
    explicit ThievingTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y texto de la pestaña. @return Los textos. */
    static ToolInfo info();

    /** @brief `set_tool_ui`: los valores de las preferencias y el estado inicial. */
    void setToolUi();

    /** @brief `on_add_copper_thieving_click`: rellena (en el modo área, empieza a elegir los rectángulos). */
    void addCopperThieving();

    /** @brief `on_add_robber_bar_click`: crea `<nombre>_robber`. @return El objeto creado, o nulo. */
    FlatCAMObj* addRobberBar();

    /** @brief `on_new_pattern_plating_object`: crea la máscara para el galvanizado. @return El objeto, o nulo. */
    FlatCAMObj* addPlatingMask();

    /** @brief El último objeto creado (para los tests). @return El objeto, o nulo. */
    FlatCAMObj* lastCreated() const { return lastCreated_; }

private:
    void onRefComboTypeChange();
    void onToggleReference();
    void onThievingType(const QString& choice);
    void fillCombo(FCComboBox* combo, ObjectKind kind, bool last);
    void copperThievingRun(const ShapeList& area, FlatCAMObj* ref);
    FlatCAMObj* createGerber(FlatCAMObj* src, const QString& name, ShapeList solid, ShapeList follow,
                             std::map<std::string, Aperture> apertures, const std::vector<std::string>& added);
    void onExit();

    App* app_;
    FlatCAMObj* grb_{nullptr};
    FlatCAMObj* lastCreated_{nullptr};
    double clearance_{0.0};
    double margin_{0.0};
    ShapeList thieving_;           ///< `thief_solid_geometry`.
    ShapeList flat_;               ///< `flat_geometry` (no se vacía nunca, como en el original).
    std::optional<Shape> robber_;  ///< `robber_geo`.
    std::optional<Shape> robberLine_;  ///< `robber_line`.
    double rbThickness_{0.0};      ///< `rb_thickness`.
};

}  // namespace flatcam::gui
