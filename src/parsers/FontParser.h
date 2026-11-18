/**
 * @file FontParser.h
 * @brief Textos con fuentes TrueType: port de `appParsers/ParseFont.py` de FlatCAM 2024.4.
 *
 * Lo usa la herramienta Texto del editor de Geometría. Como el original, cada contorno de cada letra es un polígono
 * aparte armado con los **puntos de la letra tal cual los da FreeType** (también los puntos de control de las curvas,
 * sin aplanarlas), cargada con hinting a 72 ppp, y escalado con los factores fijos del original. El kerning se
 * reproduce con la falla del original: se busca entre las letras cuyo código es el índice de cada letra. Necesita FreeType
 * (`FLATCAM_HAVE_FREETYPE`); sin ella `FontParser::available()` es falso y ninguna fuente se puede usar.
 *
 * En el SVG el original también intenta escribir los textos (`getsvgtext`), pero llama a `ParseFont()` sin la
 * aplicación y el error queda atrapado: nunca importa un texto (ver la sección 14 de AGENTS.md).
 */
#ifndef FLATCAM_PARSERS_FONTPARSER_H
#define FLATCAM_PARSERS_FONTPARSER_H

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/**
 * @brief Las fuentes instaladas por tipo y el paso de un texto a polígonos (`ParseFont`).
 */
class FontParser {
public:
    /** @brief Si el port se compiló con FreeType. @return Verdadero si hay FreeType. */
    static bool available();

    /**
     * @brief Las carpetas de fuentes del sistema (`get_mac_font_paths`, `get_linux_font_paths`,
     * `get_win32_font_path`), con sus subcarpetas en macOS y Linux.
     *
     * Desvío: en Windows el original lee del registro la carpeta del usuario y la lista de fuentes instaladas; el port
     * usa `%WINDIR%\\Fonts`. En Linux no se ejecuta `/usr/sbin/chkfontpath`.
     * @return Las carpetas que existen.
     */
    static std::vector<std::string> systemFontDirectories();

    /**
     * @brief Los archivos `.ttf` de unas carpetas (`get_fonts`), con su ruta absoluta y sin repetir, en el orden en que
     * los lista cada carpeta (como `glob`).
     * @param dirs Carpetas.
     * @return Las rutas.
     */
    static std::vector<std::string> fontFiles(const std::vector<std::string>& dirs);

    /**
     * @brief El nombre completo (identificador 4) y la familia (identificador 1) de la tabla `name` de una fuente
     * (`get_font_name`): el primer registro de cada uno, en UTF-16 BE si tiene algún byte 0 y si no en Latin-1.
     * @param path Archivo de la fuente.
     * @return Nombre y familia, en UTF-8.
     * @throws std::runtime_error si el archivo no es una fuente TrueType que se pueda leer.
     */
    static std::pair<std::string, std::string> fontName(const std::string& path);

    /**
     * @brief Reparte las fuentes por tipo según su nombre (`get_fonts_by_types`), con las mismas reglas del original
     * (incluida su falla: una "Oblique" que no es negrita queda en las cursivas con su nombre completo).
     * @param files Archivos de fuentes (los que no se pueden leer se saltean).
     */
    void loadFonts(const std::vector<std::string>& files);

    /**
     * @brief Pasa un texto a polígonos (`font_to_geometry`).
     * @param text Texto (UTF-8); cada carácter es una letra, también los saltos de línea.
     * @param fontName Nombre de la fuente (la familia que muestra el combo).
     * @param fontType "regular", "bold", "italic" o "bi".
     * @param fontSize Tamaño en puntos (el original hace `int()` del texto del combo).
     * @param mm Verdadero si las unidades son milímetros (si no, pulgadas).
     * @param coordx X del origen (se suma a la posición en unidades de FreeType, como el original).
     * @param coordy Y del origen.
     * @return Los polígonos, o `std::nullopt` si la fuente no está en la lista de ese tipo o no hay FreeType (el
     *         original avisa "Font not supported, try another one." y devuelve un texto).
     * @throws std::runtime_error si FreeType no puede abrir la fuente.
     */
    std::optional<std::vector<Shape>> fontToGeometry(const std::string& text, const std::string& fontName,
                                                     const std::string& fontType, int fontSize, bool mm,
                                                     double coordx = 0, double coordy = 0) const;

    std::map<std::string, std::string> regular;     ///< `regular_f`: nombre -> archivo.
    std::map<std::string, std::string> bold;        ///< `bold_f`.
    std::map<std::string, std::string> italic;      ///< `italic_f`.
    std::map<std::string, std::string> boldItalic;  ///< `bold_italic_f`.
};

}  // namespace flatcam

#endif
