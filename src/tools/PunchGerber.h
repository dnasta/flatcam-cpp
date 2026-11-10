/**
 * @file PunchGerber.h
 * @brief Perforar un Gerber: agujeros en los pads (port de `ToolPunchGerber.on_generate_object` y sus métodos
 * `on_excellon_method`, `on_fixed_method`, `on_ring_method` y `on_proportional_method`, `appTools/ToolPunchGerber.py`).
 */
#pragma once

#include <map>
#include <set>
#include <string>

#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Parámetros de Perforar Gerber (`tools_punch_*`). */
struct PunchSettings {
    /** @brief Cómo se hacen los agujeros. */
    enum class Method {
        Excellon,     ///< Con los agujeros de un Excellon (`exc`).
        Fixed,        ///< Un diámetro fijo (`fixed`).
        Ring,         ///< Dejando un anillo (`ring`).
        Proportional  ///< Una proporción del pad (`prop`).
    };
    Method method{Method::Excellon};  ///< Método.
    double fixedDia{0.5};             ///< Diámetro fijo.
    double ringCircular{0.2};         ///< Anillo de los pads circulares.
    double ringOblong{0.2};           ///< Anillo de los oblongos.
    double ringSquare{0.2};           ///< Anillo de los cuadrados.
    double ringRectangular{0.2};      ///< Anillo de los rectangulares.
    double ringOther{0.2};            ///< Anillo de los demás.
    double factor{50.0};              ///< Proporción, en %.
    bool circular{true};              ///< Pads circulares (C).
    bool oblong{true};                ///< Oblongos (O).
    bool square{true};                ///< Cuadrados (R con lados iguales).
    bool rectangular{true};           ///< Rectangulares.
    bool other{true};                 ///< Los demás.
    int decimals{4};                  ///< `app.decimals`.
};

/** @brief El Gerber perforado. */
struct PunchResult {
    /** @brief Por qué no se creó. */
    enum class Error {
        None,           ///< Se creó.
        ZeroDiameter,   ///< El diámetro fijo es 0 ("The value of the fixed diameter is 0.0").
        HoleTooBig,     ///< El agujero fijo es más grande que algún pad elegido.
        SameGeometry,   ///< No se perforó nada ("The new object geometry is the same...").
        OriginalFails   ///< El original falla con una excepción (no crea nada; en la GUI solo queda en el registro).
    };
    Error error{Error::None};                    ///< El error.
    ShapeList solid;                             ///< Geometría sólida perforada.
    std::map<std::string, Aperture> apertures;   ///< Las aperturas del Gerber más las nuevas con los agujeros (`clear`).
    std::vector<std::string> newApertures;       ///< Los números de las aperturas nuevas, en orden.
    /** @brief Si se creó. @return `true` si sí. */
    bool ok() const { return error == Error::None; }
};

/**
 * @brief Perfora los pads (flashes) de las aperturas elegidas. Las aperturas nuevas (circulares, con un agujero `clear`
 * por perforación que cae dentro de un pad) se numeran desde la mayor, una por diámetro, como el original.
 *
 * Errores del original que se reproducen: con el diámetro fijo, una macro elegida con "otros" activo hace fallar todo
 * (`KeyError: 'size'`); en anillo y proporcional, una apertura de un tipo con su filtro apagado igual se perfora por
 * "otros" si ese filtro está activo.
 * @param g El Gerber. @param selected Las aperturas elegidas en la tabla. @param s Parámetros.
 * @param exc El Excellon (método `exc`).
 * @return El resultado.
 */
PunchResult punchGerber(const Gerber& g, const std::set<std::string>& selected, const PunchSettings& s,
                        const Excellon* exc = nullptr);

}  // namespace flatcam
