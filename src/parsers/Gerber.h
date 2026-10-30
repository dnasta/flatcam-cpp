/**
 * @file Gerber.h
 * @brief Lectura de archivos Gerber RS-274X (port de `Gerber` en `appParsers/ParseGerber.py`).
 */
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/Geometry.h"
#include "parsers/ApertureMacro.h"

namespace flatcam {

/**
 * @brief Geometría que resultó de un trazo, un flash o una región, tal como la guarda FlatCAM en cada apertura.
 *
 * Equivale a los diccionarios `{'solid': ..., 'follow': ..., 'clear': ...}` de `apertures[id]['geometry']`.
 */
struct ApertureGeometry {
    std::optional<Shape> solid;   ///< Figura sólida (polaridad oscura).
    std::optional<Shape> follow;  ///< Línea o punto central del trazo.
    std::optional<Shape> clear;   ///< Figura de borrado (polaridad clara).
};

/**
 * @brief Una apertura definida con `%ADD`.
 *
 * Según @ref type solo se usan algunos de los campos:
 * `"C"` (círculo): `size`; `"R"` (rectángulo) y `"O"` (obround): `width`, `height`;
 * `"P"` (polígono regular): `diam`, `nVertices`, `rotation`; `"AM"`: `macro` y `modifiers`;
 * `"REG"`: apertura interna `0` que guarda las regiones.
 */
struct Aperture {
    std::string type;                      ///< `"C"`, `"R"`, `"O"`, `"P"`, `"AM"` o `"REG"`.
    std::optional<double> size;            ///< Tamaño (el parser original lo usa para el ancho de los trazos).
    double width{0.0};                     ///< Ancho (R, O).
    double height{0.0};                    ///< Alto (R, O).
    double diam{0.0};                      ///< Diámetro (P).
    int nVertices{0};                      ///< Cantidad de vértices (P).
    std::optional<double> rotation;        ///< Rotación en grados (P).
    std::shared_ptr<ApertureMacro> macro;  ///< Macro (AM).
    std::vector<std::string> modifiers;    ///< Parámetros de la macro (AM).
    bool hasGeometry{false};               ///< `true` si ya se creó la lista `geometry`.
    std::vector<ApertureGeometry> geometry;///< Geometría producida con esta apertura.
};

/**
 * @brief Opciones que controlan cómo se construye la geometría de un Gerber.
 *
 * Los valores por defecto son los de FlatCAM 8.994 / 2024.4 (`gerber_*` en sus preferencias).
 */
struct GerberOptions {
    int circleSteps{64};                 ///< `gerber_circle_steps`: segmentos por circunferencia completa.
    bool useBufferForUnion{true};        ///< `gerber_use_buffer_for_union`.
    std::string buffering{"full"};       ///< `gerber_buffering`: `"full"` o `"none"`.
    bool extraBuffering{true};           ///< `gerber_extra_buffering`: reduce cada polígono 1e-7 al final.
    bool simplification{false};          ///< `gerber_simplification`.
    double simpTolerance{0.0005};        ///< `gerber_simp_tolerance`.
    bool cleanApertures{true};           ///< `gerber_clean_apertures`: descarta aperturas sin geometría.
    Units defaultUnits{Units::Millimeters};  ///< `gerber_def_units`.
    char defaultZeros{'T'};              ///< `gerber_def_zeros`: `'L'`, `'T'` o `'D'`.
};

/**
 * @brief Lector de archivos Gerber: construye la geometría de cobre a partir de las órdenes del archivo.
 *
 * Reproduce el parser de FlatCAM: cada trazo se engrosa con el tamaño de su apertura (`ancho / 1.999`), los
 * flashes colocan la forma de la apertura, las regiones (`G36`/`G37`) se rellenan y todo se une al final
 * respetando la polaridad. El resultado queda en @ref Geometry::solidGeometry (figuras sólidas) y
 * @ref Geometry::followGeometry (líneas centrales).
 *
 * Soporta las aperturas C, R, O, P y macros (`%AM`), arcos de un cuadrante (G74) y multicuadrante (G75),
 * regiones, polaridad clara/oscura (`%LP`), unidades (`%MO`, `G70`/`G71`) y los formatos `%FS` de KiCad,
 * Eagle y OrCAD. Los atributos X2 (`%TF`, `%TA`, `%TO`, `%TD`) se ignoran.
 *
 * ### Diferencias respecto de Python
 * - Los modificadores de reflejo, escala, etc. propios de `Gerber` (`mirror`, `scale`, `offset`...) se
 *   agregan en la fase 3.
 * - El ajuste de `extra_buffering` se aplica polígono por polígono, como en Shapely 1.8 (la versión que usa
 *   FlatCAM).
 * - Las expresiones de las macros se evalúan con un intérprete propio, no con `eval()`.
 *
 * @code
 * flatcam::Gerber g;
 * if (g.parseFile("placa.gbl")) {
 *     double area = flatcam::Shape::unaryUnion(g.solidGeometry()).area();
 * }
 * @endcode
 */
class Gerber : public Geometry {
public:
    /**
     * @brief Crea un lector vacío.
     * @param options Opciones de construcción de la geometría.
     */
    explicit Gerber(const GerberOptions& options = GerberOptions());

    /** @brief Destructor. */
    ~Gerber() override;

    /**
     * @brief Lee un archivo Gerber (`parse_file`).
     *
     * Cada línea se divide en sentencias terminadas en `*`, salvo las que terminan en `%`.
     * @param path Ruta del archivo.
     * @param follow Parámetro heredado de FlatCAM; no tiene efecto, igual que en el original.
     * @return `true` si se generó geometría; `false` si el archivo no se pudo abrir, no produjo figuras o el
     *         análisis falló (ver @ref lastError).
     */
    bool parseFile(const std::string& path, bool follow = false);

    /**
     * @brief Interpreta un Gerber ya dividido en sentencias (`parse_lines`).
     * @param lines Sentencias del archivo, una por elemento.
     * @return `true` si se generó geometría; `false` en caso contrario (ver @ref lastError).
     */
    bool parseLines(const std::vector<std::string>& lines);

    /**
     * @brief Interpreta un número de coordenada Gerber (`parse_gerber_number`).
     * @param number Texto del número, con signo opcional y sin punto decimal.
     * @param intDigits Cantidad de dígitos enteros del formato.
     * @param fracDigits Cantidad de dígitos decimales del formato.
     * @param zeros `'L'` o `'D'` (ceros iniciales omitidos o sin supresión) o `'T'` (ceros finales omitidos).
     * @return Valor en las unidades del archivo.
     */
    static double parseGerberNumber(const std::string& number, int intDigits, int fracDigits, char zeros);

    /**
     * @brief Crea la figura de un flash (`create_flash_geometry`).
     * @param location Posición del centro.
     * @param aperture Apertura que se coloca.
     * @param stepsPerCircle Segmentos por circunferencia completa.
     * @return Figura del flash.
     * @throws GeometryError si el tipo de apertura no se puede dibujar.
     */
    static Shape createFlashGeometry(const Point2D& location, const Aperture& aperture, int stepsPerCircle);

    /** @brief Aperturas definidas en el archivo, por identificador. @return El mapa de aperturas. */
    const std::map<std::string, Aperture>& apertures() const { return apertures_; }

    /** @brief Macros de apertura definidas en el archivo, por nombre. @return El mapa de macros. */
    const std::map<std::string, std::shared_ptr<ApertureMacro>>& apertureMacros() const { return macros_; }

    /** @brief Dígitos enteros del formato de coordenadas. @return Cantidad de dígitos. */
    int intDigits() const { return intDigits_; }
    /** @brief Dígitos decimales del formato de coordenadas. @return Cantidad de dígitos. */
    int fracDigits() const { return fracDigits_; }
    /** @brief Supresión de ceros del archivo (`'L'`, `'T'` o `'D'`). @return El carácter. */
    char zeros() const { return zeros_; }
    /** @brief `true` si la última polaridad leída fue clara. @return El estado. */
    bool isLpc() const { return isLpc_; }
    /** @brief Texto fuente leído, una sentencia por línea (`source_file`). @return El texto. */
    const std::string& sourceFile() const { return sourceFile_; }
    /** @brief Opciones con las que se construyó el lector. @return Las opciones. */
    const GerberOptions& options() const { return options_; }
    /** @brief Mensaje del último error de lectura, vacío si no hubo. @return El mensaje. */
    const std::string& lastError() const { return lastError_; }

private:
    /** @brief Interpreta una definición de apertura (`aperture_parse`). */
    std::optional<std::string> apertureParse(const std::string& id, const std::string& type,
                                             const std::optional<std::string>& params);

    GerberOptions options_;
    std::map<std::string, Aperture> apertures_;
    std::map<std::string, std::shared_ptr<ApertureMacro>> macros_;
    int intDigits_{3};
    int fracDigits_{4};
    char zeros_{'T'};
    bool isLpc_{false};
    bool conversionDone_{false};
    std::string sourceFile_;
    std::string lastError_;
};

}  // namespace flatcam
