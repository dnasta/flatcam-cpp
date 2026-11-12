/**
 * @file SolderPaste.h
 * @brief Pasta de soldar (port de `ToolSolderPaste.on_create_geo` y de `CNCjob.generate_gcode_from_solderpaste_geo` /
 * `create_soldepaste_gcode`): el recorrido de dispensado sobre cada pad y su G-code con el postprocesador Paste_1.
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "cam/Preprocessor.h"
#include "core/Shape.h"

namespace flatcam {

/** @brief Una boquilla de la Geometría de pasta: su número, su diámetro y los trazos. */
struct PasteTool {
    int uid;           ///< Número de la herramienta de la tabla.
    double dia;        ///< Diámetro (redondeado a los decimales).
    ShapeList paths;   ///< Trazos de dispensado.
};

/** @brief Resultado de @ref solderPasteGeometry. */
struct PasteGeometry {
    std::vector<PasteTool> tools;  ///< Las boquillas, de la más grande a la más chica.
    std::string error;             ///< Vacío si salió bien; si no, el mensaje del original (no se crea la Geometría).
};

/**
 * @brief `on_create_geo`: para cada pad (el exterior de cada polígono del Gerber) la diagonal más larga dentro del pad
 * o, si las dos miden lo mismo (redondeadas a 1 decimal en mm), la línea media del lado largo; recortada al pad
 * achicado medio diámetro. Las boquillas van de la más grande a la más chica; un pad donde no entra queda para la
 * siguiente.
 * @param solid La geometría sólida del Gerber. @param nozzles Número y diámetro de cada boquilla de la tabla, en
 * orden. @param decimals `app.decimals`. @param mm Unidades en mm.
 * @return Las boquillas con sus trazos, o el error.
 */
PasteGeometry solderPasteGeometry(const ShapeList& solid, const std::vector<std::pair<int, double>>& nozzles,
                                  int decimals, bool mm);

/**
 * @brief `generate_gcode_from_solderpaste_geo`: el G-code de una boquilla (el trazo más cercano cada vez).
 * @param paths Los trazos. @param p Los parámetros (los de pasta de la herramienta y `toolC`).
 * @param g90 `cncjob_coords_type` == `G90`. @return El G-code.
 */
std::string solderPasteGcode(const ShapeList& paths, PreprocessorParams p, bool g90);

}  // namespace flatcam
