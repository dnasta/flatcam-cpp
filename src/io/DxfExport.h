/**
 * @file DxfExport.h
 * @brief Exportar una Geometría a DXF (port de `GeometryObject.export_dxf`): el documento R2010 vacío de
 * `ezdxf.new('R2010')` (plantilla generada) con una `LWPOLYLINE` por contorno, agujero o línea.
 */
#pragma once

#include <string>

#include "core/Shape.h"

namespace flatcam {

/**
 * @brief El texto DXF de `export_dxf` + `saveas`: cada polígono da su contorno y sus agujeros; cada línea, su
 * polilínea; los puntos no se exportan. Los identificadores siguen la numeración de ezdxf 0.14 y los GUID del
 * encabezado son nuevos en cada llamada y `$TDCREATE` / `$TDUPDATE` llevan la hora (como en el original).
 * @param shapes Figuras, en orden. @return El documento.
 */
std::string exportDxf(const ShapeList& shapes);

}  // namespace flatcam
