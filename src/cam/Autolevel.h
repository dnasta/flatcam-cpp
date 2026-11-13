/**
 * @file Autolevel.h
 * @brief Autonivelación del CNC Job (port de la parte de `FlatCAMCNCJob.py` que la arma): los puntos de sondeo en
 * grilla, las celdas de Voronoi de cada punto, el G-code de sondeo para MACH3/MACH4/LinuxCNC, el mapa de alturas y los
 * comandos que se le mandan a GRBL.
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Un punto de sondeo (`al_voronoi_geo_storage[id]`). */
struct ProbePoint {
    Point2D point;                ///< `point`.
    std::optional<Shape> cell;    ///< `geo`: la celda de Voronoi (vacío: `None`).
    double height{0.0};           ///< `height`.
};

/** @brief `dec_format`: el valor escrito con @p dec decimales y vuelto a leer. @param v Valor. @param dec Decimales.
 *  @return Redondeado. */
double decFormat(double v, int dec);

/**
 * @brief Los puntos de la grilla de `on_add_al_probepoints`, fila por fila desde abajo a la izquierda.
 * @param b Límites de lo que corta. @param rows Filas. @param cols Columnas. @param decimals `app.decimals`.
 * @return Los puntos, redondeados.
 */
std::vector<Point2D> autolevelGrid(const BoundingBox& b, int rows, int cols, int decimals);

/**
 * @brief `generate_voronoi_geometry`: la celda de cada punto (recortada a la caja de lo que corta agrandada 1 mm o
 * 0,039 pulgadas). Los puntos que no caen dentro de ninguna celda quedan sin celda.
 * @param points Los puntos (se les completa `cell`). @param b Límites de lo que corta. @param mm Unidades en mm.
 * @return `false` si GEOS no pudo armar el diagrama.
 */
bool autolevelVoronoi(std::vector<ProbePoint>& points, const BoundingBox& b, bool mm);

/** @brief Los parámetros del G-code de sondeo. */
struct ProbingGcodeParams {
    std::string controller{"MACH3"};  ///< `MACH3`, `MACH4`, `LinuxCNC` o `GRBL`.
    std::string units{"MM"};          ///< Unidades del CNC Job.
    std::string name;                 ///< Nombre del objeto.
    std::string version;              ///< Versión de FlatCAM.
    std::string versionDate;          ///< Su fecha.
    std::string timeStr;              ///< "Created on".
    double travelZ{2.0};              ///< `ptravelz_entry`.
    double probeDepth{-1.0};          ///< `pdepth_entry`.
    double probeFeedrate{120};        ///< `feedrate_probe_entry`.
    int decimals{4};                  ///< `app.decimals`.
    int coordsDecimals{4};            ///< `coords_decimals`.
    int frDecimals{2};                ///< `fr_decimals`.
};

/**
 * @brief `probing_gcode`: el G-code que sondea cada punto y guarda las alturas en un archivo del controlador.
 * @param points Los puntos. @param p Parámetros.
 * @return El G-code (vacío con GRBL o un controlador desconocido).
 */
std::string autolevelProbingGcode(const std::vector<Point2D>& points, const ProbingGcodeParams& p);

/**
 * @brief `import_height_map`: cada línea `x y z` (o con comas) da la altura del punto con su número; los que faltan se
 * agregan con su X,Y.
 * @param lines Las líneas. @param points Los puntos (por número, desde 1).
 * @return `false` si una línea no se pudo leer (el original falla ahí; lo anterior queda).
 */
bool autolevelImportHeightMap(const std::vector<std::string>& lines, std::vector<ProbePoint>& points);

/**
 * @brief `on_grbl_jog`: el comando de avance manual. Como el original, `step` es una tupla (la coma al final de la
 * línea) y el número sale como `(5.0,)`.
 * @param direction `xplus`, `xminus`, `yplus`, `yminus`, `zplus`, `zminus` u `origin`.
 * @param step Paso. @param feedrate Avance. @param units `MM` o `IN`. @param travelZ `cncjob_al_grbl_travelz`.
 * @return Los comandos (dos con `origin`).
 */
std::vector<std::string> grblJogCommands(const std::string& direction, double step, double feedrate,
                                         const std::string& units, double travelZ);

}  // namespace flatcam
