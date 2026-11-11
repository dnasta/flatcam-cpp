/**
 * @file CopperThieving.h
 * @brief Robo de cobre: relleno de las zonas libres, barra de robo y máscara para el galvanizado (port de
 * `ToolCopperThieving.copper_thieving`, `on_add_robber_bar_click` y `on_new_pattern_plating_object`,
 * `appTools/ToolCopperThieving.py`).
 */
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Tipo de relleno (`fill_type_radio`). */
enum class ThievingFill {
    Solid,   ///< `solid`: todo el espacio libre.
    Dot,     ///< `dot`: una grilla de círculos.
    Square,  ///< `square`: una grilla de cuadrados.
    Line     ///< `line`: una grilla de líneas, con un contorno alrededor del cobre y de la caja.
};

/** @brief Zona que se rellena (`reference_radio`). */
enum class ThievingReference {
    Itself,  ///< `itself`: la caja del propio Gerber.
    Area,    ///< `area`: rectángulos elegidos con el mouse.
    Box      ///< `box`: otro objeto.
};

/** @brief Parámetros del relleno (los campos del panel). */
struct ThievingParams {
    double clearance{0.25};                       ///< `clearance_entry`: separación del cobre.
    double margin{1.0};                           ///< `margin_entry`: margen alrededor de la zona.
    double minArea{0.1};                          ///< `area_entry`: las zonas más chicas no se rellenan.
    ThievingReference reference{ThievingReference::Itself};  ///< Zona.
    bool minimalBox{false};                       ///< `bbox_type_radio` = `min` (envolvente convexa) o `rect`.
    ThievingFill fill{ThievingFill::Solid};       ///< Tipo de relleno.
    double dotDia{1.0};                           ///< `dot_dia_entry`.
    double dotSpacing{2.0};                       ///< `dot_spacing_entry`.
    double squareSize{1.0};                       ///< `square_size_entry`.
    double squareSpacing{2.0};                    ///< `squares_spacing_entry`.
    double lineSize{0.25};                        ///< `line_size_entry`.
    double lineSpacing{2.0};                      ///< `lines_spacing_entry`.
    int circleSteps{64};                          ///< `tools_copper_thieving_circle_steps`.
};

/** @brief El objeto de referencia del modo `box`. */
struct ThievingBoxReference {
    std::string kind;  ///< `gerber`, `geometry` o `excellon` (este no se admite, como en el original).
    ShapeList solid;   ///< Su geometría sólida.
};

/** @brief Resultado de @ref copperThieving. */
struct ThievingResult {
    bool ok{false};                             ///< `false` si el original falla o no crea nada.
    std::string error;                          ///< Mensaje que muestra el original al fallar (vacío si no muestra).
    /// `thief_solid_geometry`: lo agregado (la usa la máscara); vacío si el original no llegó a asignarlo (sí lo
    /// asigna aunque falle después, por ejemplo con una grilla en la que no entra ningún punto).
    std::optional<ShapeList> thieving;
    ShapeList solid;                            ///< Geometría sólida del Gerber nuevo.
    std::map<std::string, Aperture> apertures;  ///< Aperturas: lo agregado va a la `0`.
};

/**
 * @brief `copper_thieving`: el Gerber con el cobre agregado en los espacios libres.
 *
 * Rarezas del original que se reproducen: en el modo de líneas la lista donde se aplanan las figuras
 * (`flat_geometry`) no se vacía nunca, así que cada relleno con líneas agrega también los de antes (@p flatGeometry);
 * la grilla de puntos, cuadrados o líneas se arma siempre sobre la caja del Gerber más el margen, sea cual sea la
 * referencia; con un solo polígono, la caja "mínima" es el propio polígono.
 * @param g El Gerber. @param p Parámetros. @param area Rectángulos elegidos (modo `area`).
 * @param box Objeto de referencia (modo `box`). @param flatGeometry `flat_geometry` de la herramienta (se acumula).
 * @return El resultado.
 */
ThievingResult copperThieving(const Gerber& g, const ThievingParams& p, const ShapeList& area,
                              const ThievingBoxReference& box, ShapeList& flatGeometry);

/** @brief Resultado de @ref robberBar. */
struct RobberBarResult {
    Shape line;                                 ///< `robber_line`: el contorno exterior de la barra.
    Shape bar;                                  ///< `robber_geo`: la barra.
    ShapeList solid;                            ///< Geometría sólida del Gerber nuevo (`unary_union`).
    ShapeList follow;                           ///< Seguimiento: el del Gerber más la línea.
    std::map<std::string, Aperture> apertures;  ///< Aperturas con la barra.
    std::string newAperture;                    ///< La apertura creada (vacío si se usó una C del mismo grosor).
};

/**
 * @brief `on_add_robber_bar_click`: una barra alrededor de la caja del Gerber, a @p margin del cobre.
 * @param g El Gerber. @param margin Margen. @param thickness Grosor. @return El resultado.
 */
RobberBarResult robberBar(const Gerber& g, double margin, double thickness);

/** @brief Resultado de @ref platingMask. */
struct PlatingMaskResult {
    bool ok{false};                             ///< `false` si el original falla.
    ShapeList solid;                            ///< Geometría sólida de la máscara.
    ShapeList follow;                           ///< Seguimiento: el de la máscara (y la línea de la barra).
    std::map<std::string, Aperture> apertures;  ///< Aperturas.
    std::vector<std::string> added;             ///< Aperturas que no estaban en la máscara, en orden.
    double platedArea{0.0};                     ///< El área galvanizada que se muestra en el panel.
};

/**
 * @brief `on_new_pattern_plating_object`: la máscara de soldadura con aberturas sobre el relleno y la barra.
 *
 * Rarezas del original que se reproducen: el área galvanizada suma el relleno y la barra sin agrandar; si la barra va
 * a una apertura que ya existe, entra sin agrandar; el número de la apertura nueva sale del mayor de los números como
 * texto.
 * @param sm La máscara. @param clearance `clearance_ppm_entry` (negativo achica también la máscara).
 * @param choice `ppm_choice_radio`: `b` (los dos), `t` (relleno) o `r` (barra).
 * @param thieving El último relleno. @param robber La última barra (`robber_geo`) y su línea, si hay.
 * @param robberLine `robber_line`. @param rbThickness El grosor de la última barra.
 * @return El resultado.
 */
PlatingMaskResult platingMask(const Gerber& sm, double clearance, char choice, const ShapeList& thieving,
                              const std::optional<Shape>& robber, const std::optional<Shape>& robberLine,
                              double rbThickness);

}  // namespace flatcam
