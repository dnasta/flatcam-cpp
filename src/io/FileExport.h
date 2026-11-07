/**
 * @file FileExport.h
 * @brief Exportar Gerber y Excellon (port de `App.export_gerber` / `GerberObject.export_gerber` y de
 * `App.export_excellon` / `ExcellonObject.export_excellon`).
 */
#pragma once

#include <string>

namespace flatcam {

class Gerber;
class Excellon;

/** @brief Datos de la cabecera de un archivo exportado. */
struct ExportHeader {
    std::string name;                    ///< Nombre del objeto (`Filename:`).
    std::string createdOn;               ///< Fecha ya formateada (`"%A, %d %B %Y at %H:%M"`).
    std::string version{"2024.4"};       ///< Versión de FlatCAM que se declara.
    std::string versionDate{"2024/04/15"};  ///< Fecha de esa versión.
};

/** @brief Formato de un Gerber exportado (`gerber_exp_*`; de fábrica pulgadas 2:4 con ceros "L"). */
struct GerberExportFormat {
    std::string units{"IN"};  ///< `"IN"` o `"MM"`.
    int whole{2};             ///< Dígitos enteros.
    int fract{4};             ///< Dígitos decimales.
    std::string zeros{"L"};   ///< `"L"` o `"T"`.
};

/** @brief Formato de un Excellon exportado (`excellon_exp_*`; de fábrica pulgadas, decimal, 2:4). */
struct ExcellonExportFormat {
    std::string units{"INCH"};        ///< `"INCH"` o `"METRIC"`.
    int whole{2};                     ///< Dígitos enteros.
    int fract{4};                     ///< Dígitos decimales.
    std::string zeros{"LZ"};          ///< `"LZ"` o `"TZ"` (solo sin decimales).
    std::string format{"dec"};        ///< `"dec"` o `"ndec"`.
    std::string slotType{"routing"};  ///< `"routing"` o `"drilling"`.
};

/**
 * @brief Texto de un Gerber exportado, como `export_gerber` (las unidades de la aplicación son milímetros: a pulgadas
 * el factor es 0,03937, como el original).
 *
 * Solo la apertura "0" exporta sus regiones (G36/G37, con los agujeros en polaridad clara); el resto exporta el trazo
 * o destello (`follow`) y las zonas claras. Los tamaños de las aperturas se escriben con `str()` de Python.
 * @param g Gerber. @param fmt Formato. @param h Cabecera.
 * @return El texto, o vacío si el Gerber no tiene aperturas (`'fail'`).
 */
std::string exportGerber(const Gerber& g, const GerberExportFormat& fmt, const ExportHeader& h);

/**
 * @brief Texto de un Excellon exportado, como `export_excellon`. Rareza que se reproduce: el original lee las ranuras
 * con `slot.x` (son tuplas) y falla en la primera, así que de las ranuras solo quedan los `G05` / `Tnn` de las
 * herramientas hasta la primera que tiene ranuras.
 * @param e Excellon. @param fmt Formato. @param h Cabecera.
 * @return El texto, o vacío si no hay agujeros ni ranuras (`'fail'`).
 */
std::string exportExcellon(const Excellon& e, const ExcellonExportFormat& fmt, const ExportHeader& h);

}  // namespace flatcam
