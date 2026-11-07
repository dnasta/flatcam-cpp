/**
 * @file Hpgl2.h
 * @brief Importar HPGL2 (port de `appParsers/ParseHPGL2.py` y `App.open_hpgl2`).
 */
#pragma once

#include <map>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Resultado de leer un HPGL2. */
struct Hpgl2Result {
    bool ok{false};                    ///< Si hay geometría (si no, "no es un archivo HPGL2 o está vacío").
    std::string error;                 ///< El error del intérprete, si lo hubo.
    ShapeList solidGeometry;           ///< `solid_geometry`: la unión de todos los trazos (sus partes).
    std::map<int, ShapeList> tools;    ///< Los trazos de cada pluma (`SPn`), en orden.
};

/**
 * @brief `HPGL2.parse_lines`: una orden por línea (`IN`, `SPn`, `PU`/`PD`, `PA x,y`, `CI r`, `AA x,y,a`,
 * `AT x1,y1,x2,y2`, `CO "..."`), en unidades de 1/40 mm.
 *
 * Rarezas del original que se reproducen: las líneas con varias órdenes separadas por `;` se ignoran; nada cuenta
 * antes de `IN`; el último trazo se pierde si no se levanta la pluma ni se cambia de pluma; bajar la pluma sin un trazo
 * empezado o dibujar sin `SP` hacen fallar la lectura; después de un `AA` la posición queda con dos puntos en vez de
 * dos números: un `CI` se centra en el primer punto del arco (lo que hace Shapely con esa tupla) y casi todo lo demás
 * falla; un `AT` (arco por tres puntos) siempre corta la lectura sin geometría (el original le pasa listas a
 * `three_point_circle`, que falla con `TypeError`).
 * @param text El archivo. @param circleSteps `geometry_circle_steps` (64). @return El resultado.
 */
Hpgl2Result parseHpgl2(const std::string& text, int circleSteps = 64);

}  // namespace flatcam
