/**
 * @file QrCode.h
 * @brief Códigos QR: port de la biblioteca de Python `qrcode` 7.4.2 (la que usa FlatCAM; BSD, ver NOTICE) y de la
 * herramienta `ToolQRCode` (`appTools/ToolQRCode.py`): la matriz, los SVG que escribe la biblioteca, la geometría que
 * arma FlatCAM con ellos y su inserción en un Gerber.
 */
#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/Shape.h"
#include "core/Types.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Error de la biblioteca (`DataOverflowError`, versión inválida...). */
class QrError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/** @brief Nivel de corrección de errores, con los valores de `qrcode.constants`. */
enum class QrErrorCorrection { M = 0, L = 1, H = 2, Q = 3 };

/**
 * @brief Nivel desde la letra del panel. @param letter `L`, `M`, `Q` o `H`. @return El nivel.
 */
QrErrorCorrection qrErrorCorrection(char letter);

/** @brief Un código QR armado: la versión final y los módulos (`True` = oscuro), sin el borde. */
struct QrMatrix {
    int version{1};                              ///< Versión (1 a 40) después de `best_fit`.
    std::vector<std::vector<bool>> modules;      ///< Filas de módulos.
};

/**
 * @brief `QRCode(version, error_correction)` + `add_data(data)` (con `optimize=20`) + `make()` (que busca la versión
 * mínima desde @p version): los mismos trozos, la misma máscara y los mismos módulos que la biblioteca.
 * @param data El texto en UTF-8. @param version Versión pedida (1 a 40). @param ec Nivel de corrección.
 * @return La matriz. @throws QrError Si no entra (versión 41) o la versión no es válida.
 */
QrMatrix qrMake(const std::string& data, int version, QrErrorCorrection ec);

/**
 * @brief El texto de `SvgFragmentImage` (`<svg:svg ...>` con un `<svg:rect>` por módulo, en mm: 10 píxeles = 1 mm).
 * @param qr La matriz. @param boxSize `box_size`. @param border `border` (en módulos).
 * @return El documento.
 */
std::string qrSvgFragment(const QrMatrix& qr, int boxSize, int border);

/**
 * @brief El texto de `SvgPathImage` (el archivo que escribe "Exportar SVG"): un solo `<path>`, con los colores como
 * atributos `fill_color` y `back_color` del `<svg>`, como la biblioteca.
 * @param qr La matriz. @param boxSize `box_size`. @param border `border`. @param fill Color. @param back Fondo.
 * @return El documento.
 */
std::string qrSvgPath(const QrMatrix& qr, int boxSize, int border, const std::string& fill, const std::string& back);

/** @brief La geometría del código, como la arma `execute`. */
struct QrGeometry {
    ShapeList modules;  ///< `qrcode_geometry`: un cuadrado por módulo (de `convert_svg_to_geo`).
    Shape utility;      ///< `qrcode_utility_geometry`: la unión, engrosada y achicada 1e-7.
};

/**
 * @brief `convert_svg_to_geo` del SVG de fragmentos y la unión engrosada.
 * @param qr La matriz. @param boxSize `box_size`. @param border `border`. @param circleSteps `geometry_circle_steps`.
 * @return La geometría.
 */
QrGeometry qrGeometry(const QrMatrix& qr, int boxSize, int border, int circleSteps = 64);

/** @brief El Gerber con el código, como lo deja `make` (modifica el objeto de origen). */
struct QrGerber {
    ShapeList solid;                            ///< La geometría sólida (sin unir).
    std::map<std::string, Aperture> apertures;  ///< Las aperturas, con la nueva (R) y el borrado en la "0".
    std::string newAperture;                    ///< El número de la apertura nueva.
};

/**
 * @brief `make(pos)`: corta del cobre que contiene entera la máscara (la caja del código agrandada el borde), agrega
 * los módulos (o la máscara menos los módulos, en negativo) corridos a @p pos, una apertura R del tamaño del módulo con
 * los cuadrados y un borrado de la máscara en la apertura "0".
 * @param g El Gerber. @param qr La geometría. @param pos Posición. @param boxSize `box_size`. @param border `border`.
 * @param rounded Esquinas redondeadas (`r`) o rectas (`s`). @param negative Polaridad negativa.
 * @return El Gerber.
 */
QrGerber qrInsert(const Gerber& g, const QrGeometry& qr, const Point2D& pos, int boxSize, int border, bool rounded,
                  bool negative);

}  // namespace flatcam
