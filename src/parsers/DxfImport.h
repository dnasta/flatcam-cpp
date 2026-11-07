/**
 * @file DxfImport.h
 * @brief Importar DXF (port de `appParsers/ParseDXF.py`, `ParseDXF_Spline.py`, `Geometry.import_dxf_as_geo` y
 * `Gerber.import_dxf_as_gerber`), con un lector propio de DXF ASCII en lugar de `ezdxf`.
 */
#pragma once

#include <string>

#include "core/Shape.h"

namespace flatcam {

/** @brief Resultado de leer un DXF. */
struct DxfImportResult {
    bool ok{false};      ///< Si se pudo leer.
    std::string error;   ///< El error, si no.
    ShapeList geometry;  ///< Las figuras.
};

/**
 * @brief `Geometry.import_dxf_as_geo`: puntos, líneas, círculos, arcos, elipses, polilíneas (LW y 2D), splines e
 * inserciones de bloques del espacio modelo; primero los polígonos y después las líneas unidas con `linemerge`.
 *
 * Rarezas del original que se reproducen: si todas las líneas se unen en una sola, falla (una `LineString` no es
 * iterable en Shapely 1.8); `SOLID` y `TRACE` no aportan nada; las curvas (`bulge`) de las polilíneas se ignoran; los
 * nudos de los splines se reemplazan por uniformes; en las inserciones, las filas y columnas se copian antes de escalar
 * y la escala es alrededor del centro de cada figura.
 * @param dxfText El archivo (DXF ASCII). @return El resultado.
 */
DxfImportResult importDxfAsGeometry(const std::string& dxfText);

/**
 * @brief `Gerber.import_dxf_as_gerber`: lo mismo pasado por `linemerge` (falla si hay polígonos, como el original),
 * con el `Polygon()` vacío inicial del Gerber al principio de la lista.
 * @param dxfText El archivo. @return El resultado: las figuras de la apertura "0".
 */
DxfImportResult importDxfAsGerber(const std::string& dxfText);

}  // namespace flatcam
