/**
 * @file ApertureMacro.h
 * @brief Macros de apertura de Gerber (`%AM...%`), port de `camlib.ApertureMacro`.
 */
#pragma once

#include <map>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/**
 * @brief Macro de apertura de un archivo Gerber.
 *
 * Una macro es una lista de primitivas (círculos, líneas, polígonos...) con expresiones aritméticas que
 * dependen de los parámetros (`$1`, `$2`, ...) con los que se la instancia en una definición de apertura
 * (`%ADD`). @ref makeGeometry evalúa las expresiones y combina las primitivas: las de exposición 1 se suman
 * y las de exposición 0 se restan, como pide la especificación de Gerber.
 *
 * Sintaxis soportada (igual que en FlatCAM):
 * - comentarios: `0 texto`
 * - variables: `$4=$1x0.5`
 * - primitivas: 1 (círculo), 2 y 20 (línea por vector), 21 (línea por centro), 22 (línea por esquina),
 *   4 (contorno), 5 (polígono), 6 (moiré) y 7 (térmica).
 * - operadores: `+`, `-`, `x` o `X` (producto), `/` y paréntesis.
 *
 * ### Diferencia respecto de Python
 * FlatCAM reemplaza las variables por su texto y evalúa con `eval()`. Acá las expresiones se evalúan con un
 * intérprete propio y las variables se resuelven por nombre exacto. Para macros con 10 o más parámetros el
 * resultado es el correcto de la especificación, mientras que el reemplazo de texto de Python podía
 * confundir `$1` con el comienzo de `$10`.
 */
class ApertureMacro {
public:
    /**
     * @brief Resultado de una primitiva: su exposición y su figura.
     */
    struct Primitive {
        int exposure{1};  ///< 1 suma la figura, 0 la resta.
        Shape geometry;   ///< Figura de la primitiva.
    };

    /**
     * @brief Crea una macro vacía.
     * @param name Nombre de la macro (el que sigue a `%AM`).
     */
    explicit ApertureMacro(std::string name = "") : name_(std::move(name)) {}

    /** @brief Nombre de la macro. @return El nombre. */
    const std::string& name() const { return name_; }

    /** @brief Texto crudo acumulado de la macro. @return El texto. */
    const std::string& raw() const { return raw_; }

    /**
     * @brief Agrega un fragmento de texto a la macro (`append`).
     * @param data Texto a agregar.
     */
    void append(const std::string& data) { raw_ += data; }

    /**
     * @brief Instancia la macro con los parámetros dados y devuelve su figura (`make_geometry`).
     * @param modifiers Parámetros de la definición de apertura, como texto (`$1`, `$2`, ...).
     * @return Figura resultante (vacía si la macro no dibuja nada).
     * @throws GeometryError si una primitiva no está soportada o una expresión es inválida.
     */
    Shape makeGeometry(const std::vector<std::string>& modifiers) const;

    /**
     * @brief Evalúa una expresión aritmética de Gerber.
     * @param text Expresión, por ejemplo `$1x0.5+2`.
     * @param variables Valores de las variables, indexados por su nombre sin el `$`.
     * @return Valor de la expresión. Las variables no definidas valen 0.
     * @throws GeometryError si la expresión no es válida.
     */
    static double evaluate(const std::string& text, const std::map<std::string, double>& variables);

    /** @brief Primitiva 1: círculo `(exposición, diámetro, x, y)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeCircle(const std::vector<double>& mods);
    /** @brief Primitivas 2 y 20: línea por vector `(exposición, ancho, x0, y0, x1, y1, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeVectorLine(const std::vector<double>& mods);
    /** @brief Primitiva 21: línea por centro `(exposición, ancho, alto, x, y, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeCenterLine(const std::vector<double>& mods);
    /** @brief Primitiva 22: línea por esquina `(exposición, ancho, alto, x, y, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeLowerLeftLine(const std::vector<double>& mods);
    /** @brief Primitiva 4: contorno `(exposición, n, x0, y0, ..., xn, yn, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeOutline(const std::vector<double>& mods);
    /** @brief Primitiva 5: polígono regular `(exposición, n, x, y, diámetro, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makePolygon(const std::vector<double>& mods);
    /** @brief Primitiva 6: moiré `(x, y, diámetro, grosor, separación, anillos, grosor de cruz, largo de cruz, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeMoire(const std::vector<double>& mods);
    /** @brief Primitiva 7: térmica `(x, y, diámetro exterior, diámetro interior, separación, ángulo)`. @param mods Parámetros. @return Primitiva. */
    static Primitive makeThermal(const std::vector<double>& mods);

private:
    /**
     * @brief Convierte el texto crudo en listas numéricas de primitivas (`parse_content`).
     * @param locvars Variables locales; se completan con las definiciones del macro.
     * @return Lista de primitivas; cada una es `[código, parámetros...]`.
     */
    std::vector<std::vector<double>> parseContent(std::map<std::string, double>& locvars) const;

    std::string name_;  ///< Nombre de la macro.
    std::string raw_;   ///< Texto crudo, con las partes separadas por `*`.
};

}  // namespace flatcam
