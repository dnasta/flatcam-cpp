/**
 * @file ExtractDrills.h
 * @brief Extraer taladros de los pads de un Gerber (port de `ToolExtractDrills.on_extract_drills_click`,
 * `appTools/ToolExtractDrills.py`).
 */
#pragma once

#include <optional>
#include <vector>

#include "core/Types.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Los parámetros de Extraer taladros (`tools_edrills_*`). */
struct ExtractDrillsSettings {
    /** @brief Cómo se calcula el diámetro de cada agujero. */
    enum class Mode {
        Fixed,        ///< Un diámetro fijo para todos (`fixed`).
        Ring,         ///< El pad menos dos veces el anillo de su tipo (`ring`).
        Proportional  ///< Una proporción del pad (`prop`).
    };
    Mode mode{Mode::Fixed};      ///< Modo.
    double fixedDia{0.5};        ///< Diámetro fijo.
    double ringCircular{0.2};    ///< Anillo de los pads circulares.
    double ringOblong{0.2};      ///< Anillo de los oblongos.
    double ringSquare{0.2};      ///< Anillo de los cuadrados.
    double ringRectangular{0.2}; ///< Anillo de los rectangulares.
    double ringOther{0.2};       ///< Anillo de los demás.
    double factor{50.0};         ///< Proporción, en %.
    bool circular{true};         ///< Incluir pads circulares (C).
    bool oblong{true};           ///< Incluir oblongos (O).
    bool square{true};           ///< Incluir cuadrados (R con lados iguales).
    bool rectangular{true};      ///< Incluir rectangulares.
    bool other{true};            ///< Incluir los demás (P, macros...).
    int decimals{4};             ///< `app.decimals`.
};

/** @brief Una herramienta del Excellon extraído. */
struct ExtractedTool {
    int id{1};                     ///< Número.
    double dia{0.0};               ///< Diámetro.
    std::vector<Point2D> drills;   ///< Agujeros (los flashes de los pads).
};

/**
 * @brief Las herramientas del Excellon con los agujeros de los flashes de los pads elegidos, en el orden de las
 * aperturas del archivo. Con los modos de anillo y proporcional los diámetros iguales (a @c decimals cifras) van en la
 * misma herramienta.
 *
 * Error del original que se reproduce: el primer agujero de cada herramienta no cuenta para saber si se extrajo algo
 * (el original crea la lista `solid_geometry` vacía con el primero y agrega desde el segundo), así que si ninguna
 * herramienta (ninguna apertura, en los modos de anillo y proporcional) tiene dos o más agujeros, no se crea nada.
 * @param g El Gerber. @param s Parámetros.
 * @return Las herramientas, o vacío si no se extrajo nada ("No drills extracted").
 */
std::optional<std::vector<ExtractedTool>> extractDrills(const Gerber& g, const ExtractDrillsSettings& s);

}  // namespace flatcam
