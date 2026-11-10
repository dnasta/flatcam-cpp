/**
 * @file ExcellonMilling.h
 * @brief "Geometría de fresado" del panel de un Excellon: fresar los agujeros o las ranuras en vez de taladrarlos
 * (port de `ExcellonObject.generate_milling_drills` y `generate_milling_slots`, `appObjects/FlatCAMExcellon.py`).
 */
#pragma once

#include <string>
#include <vector>

#include "core/Shape.h"
#include "parsers/Excellon.h"

namespace flatcam {

/** @brief Resultado de fresar agujeros o ranuras. */
struct MillingResult {
    /** @brief Por qué no se generó. */
    enum class Error {
        None,            ///< Se generó.
        NoTools,         ///< No hay herramientas elegidas.
        ToolTooLarge     ///< La fresa es más grande que un agujero o una ranura.
    };
    Error error{Error::None};  ///< El error, si hubo.
    int tool{0};               ///< La herramienta del agujero más chico que la fresa (agujeros).
    ShapeList rings;           ///< Los recorridos (anillos): la `solid_geometry` de la Geometría nueva.
    /** @brief Si se generó. @return `true` si sí. */
    bool ok() const { return error == Error::None; }
};

/**
 * @brief `generate_milling_drills`: el recorrido de la fresa dentro de cada agujero de las herramientas elegidas, el
 * contorno de `Point.buffer(radio del agujero - radio de la fresa)` (16 segmentos por cuarto de círculo, el valor por
 * defecto de Shapely); si los radios son iguales, `buffer(1e-7)`.
 * @param exc El Excellon. @param tools Las herramientas, en el orden de la tabla. @param tooldia Diámetro de la fresa.
 * @return El resultado.
 */
MillingResult millDrills(const Excellon& exc, const std::vector<int>& tools, double tooldia);

/**
 * @brief `generate_milling_slots`: el recorrido dentro de cada ranura, el contorno de la línea de la ranura engrosada
 * `radio de la ranura - radio de la fresa + 0,0001` (los diámetros redondeados a @p decimals; con @p stepsPerCircle
 * segmentos por cuarto de círculo, como el original con `geo_steps_per_circle`).
 * @param exc El Excellon. @param tools Las herramientas. @param tooldia Diámetro de la fresa.
 * @param decimals `app.decimals`. @param stepsPerCircle `geometry_circle_steps`.
 * @return El resultado.
 */
MillingResult millSlots(const Excellon& exc, const std::vector<int>& tools, double tooldia, int decimals = 4,
                        int stepsPerCircle = 64);

}  // namespace flatcam
