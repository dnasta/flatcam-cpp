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
 * @brief `HPGL2.parse_lines`: las órdenes `IN`, `SPn`, `PU`/`PD`, `PA x,y`, `CI r`, `AA x,y,a`, `AT x1,y1,x2,y2`,
 * `CO "..."`, en unidades de 1/40 mm.
 *
 * Con @p fixOriginalBugs (de fábrica) se corrigen cuatro errores del original, como desvíos conscientes:
 * - **Varias órdenes por línea:** cada línea se parte en los `;` (el original ignora la línea entera); además `PU` y `PD`
 *   aceptan coordenadas y `PA` varios pares, como en los archivos de otros programas.
 * - **`AA`:** la posición queda en el final del arco (el original la deja con dos puntos en vez de dos números y casi
 *   todo lo que sigue falla; un `CI` se centraba en el primer punto del arco).
 * - **`AT`:** dibuja el arco de la posición actual al final pasando por el punto intermedio (el original le pasa listas
 *   a `three_point_circle`, falla con `TypeError` y corta la lectura sin geometría; y además terminaba el arco en el
 *   punto intermedio).
 * - **Último trazo:** si el archivo termina con la pluma abajo, el trazo se guarda (el original lo pierde).
 *
 * Sin la corrección se comporta exactamente como el original (así se compara contra él en los tests). En los dos
 * modos, nada cuenta antes de `IN`, y cambiar de pluma con un trazo de un solo punto, bajar la pluma sin trazo empezado
 * o dibujar sin `SP` hacen fallar la lectura, como en el original.
 * @param text El archivo. @param circleSteps `geometry_circle_steps` (64).
 * @param fixOriginalBugs Corregir los cuatro errores del original.
 * @return El resultado.
 */
Hpgl2Result parseHpgl2(const std::string& text, int circleSteps = 64, bool fixOriginalBugs = true);

}  // namespace flatcam
