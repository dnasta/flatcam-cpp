/**
 * @file ImageImport.h
 * @brief Importar una imagen (port de `Geometry.import_image` de `camlib.py`, que usa `rasterio.features.shapes`): los
 * píxeles que pasan el umbral, como polígonos en mm o pulgadas, reflejados en Y y unidos.
 */
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Las bandas de la imagen como las lee rasterio (`src.read(1..3)`), fila por fila desde arriba. */
struct ImageBands {
    int width{0};                     ///< Ancho en píxeles.
    int height{0};                    ///< Alto en píxeles.
    std::vector<double> red;          ///< Banda 1 (en una imagen con paleta, el índice).
    std::vector<double> green;        ///< Banda 2 (vacía si no hay: se usa la 1).
    std::vector<double> blue;         ///< Banda 3 (vacía si no hay: se usa la 1).
};

/**
 * @brief `import_image`: con `mode == "black"` entran los píxeles con la banda 1 <= `mask[0]`; con `"color"`, los que
 * tienen alguna banda por debajo de su umbral (`mask[1..3]`). Cada píxel mide 25,4 / dpi mm (1 / dpi pulgadas) y la
 * imagen queda reflejada en Y (la fila 0 arriba, en y = 0).
 * @param img Las bandas. @param units `MM` o `IN`. @param dpi Puntos por pulgada. @param mode `black` o `color`.
 * @param mask Los cuatro umbrales.
 * @return La unión (`unary_union`), o vacía si no entra ningún píxel.
 */
Shape importImage(const ImageBands& img, const std::string& units, double dpi, const std::string& mode,
                  const std::array<double, 4>& mask);

}  // namespace flatcam
